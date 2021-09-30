// Based on AP_SimpleWebServer example

#include <SPI.h>
#include <WiFiNINA.h>

const int ledPin = 1;
int brightness = 0;

unsigned long previousMillis = 0; 
const long interval = 1000; 
// stop client if inactive for a while

char ssid[] = "ArduinoWifiTest";
char pass[] = "arduinowifitest";
// Replace these with your own Wifi name and password
// Make sure they're the same on wifiPostSide

int status = WL_IDLE_STATUS;
WiFiServer server(80);



void setup() {
  pinMode(ledPin, OUTPUT);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  Serial.println("Access Point Web Server");

  pinMode(ledPin, OUTPUT);      // set the LED pin mode

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

  // by default the local IP address will be 192.168.4.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

}

void loop() {
  // brightness = (brightness + 8) % 255;
  analogWrite(ledPin, brightness);
  // delay(100);

  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      // a device has connected to the AP
      Serial.println("Device connected to AP");
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
      brightness = 0;
    }
  }

  WiFiClient client = server.available();  
  // listen for incoming clients
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    
    while (client.connected()) {            // loop while the client's connected

      // if inactive over interval, close the connection
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        client.stop();
        Serial.println("client disconnected due to inactive");
      }
      
      delayMicroseconds(10);                // This is required for the Arduino Nano RP2040 Connect - otherwise it will loop so fast that SPI will never be served.
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        // Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character which is the end to the message
          
          if (currentLine.toInt()) {
            brightness = currentLine.toInt();
            Serial.println("Brightnss: "+String(brightness));
            previousMillis = millis();      
            analogWrite(ledPin, brightness);
          }

          currentLine = "";
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        
      }
    }
    Serial.println("end of whileloop");
    
  }
  
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in a browser:
  Serial.print("http://");
  Serial.println(ip);

}
