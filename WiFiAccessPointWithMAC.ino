/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)

  Modified to compose an AP SSID from a model name and the built in MAC.
  A MAC is 17 characters long because of the delimiters. Each character is two bytes.
  
  Forrest Lee Erickson, 20240913

*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set a model name
//const char *modelName = "PlaceHolderModel"; // Must be less than 32 characters long.
//const char *modelName = "PMD20240913"; // Must be less than 32 characters long.
//                      "12345678901234567890123456789012" for 32 characters
const char *modelName = "Foo"; // Must be less than 32 characters long.

// Buffer for the access point name
char apName[sizeof(*modelName) + 34];  // Adjust size based on your needs. Two bytes per char.


// Set these to your desired credentials.
//const char *ssid = "yourAP";
const char *password = "yourPassword";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.print("\nBuiltin ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  //  WiFi.softAP(ssid, password);

  // Initialize WiFi in AP mode
  WiFi.mode(WIFI_AP);
  String mac = WiFi.macAddress();   // Get the MAC address and convert it to a string
  mac.replace(":", "_"); // Replace colons ':' in MAC with underscores '_'
  snprintf(apName, sizeof(apName), "%s_%s", modelName, mac.c_str());   // Construct the AP name by concatenating the model name and the MAC address
  WiFi.softAP(apName, password);   // Start the access point with the generated name
  Serial.print("Access Point Name: ");
  Serial.println(apName);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
