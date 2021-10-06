// based on WiFiWebClient example

#include <SPI.h>
#include <WiFiNINA.h>

const int potPin = A1;
int sensorValue = 0;


char ssid[] = "ArduinoWifiTest";
char pass[] = "arduinowifitest";
// make sure these are the same as in wifiLedSide

int status = WL_IDLE_STATUS;
IPAddress server(192,168,4,1);  

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  // Use built in led to indicate connection status (might be delayed)
  // On:Connected, Blinking:Connecting
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

}

void loop() {  
  // check connection statue
  status = WiFi.status();
  
  // if not connected attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    sensorValue = 0;
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    // wait 1 seconds for connection and blink
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
//  Serial.println("Connected to WiFi");
//  printWifiStatus();
  
  // read voltage on potentiometer
  int sensorReading = map(analogRead(potPin),0,1023,0,255);
  
  // If the reading changed, send it through WiFi
  if(sensorValue != sensorReading) {
    sensorValue = sensorReading;
    
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      // send the sensor reading over WiFi
      client.println(String(sensorValue));
      Serial.println("Sent SensorValue: " + String(sensorValue));
      client.stop();
    }
  }

  delay(30);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
