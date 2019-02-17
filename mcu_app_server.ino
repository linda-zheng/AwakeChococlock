/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <WiFiUdp.h>

#include <SoftwareSerial.h>
#define Arduino_RX 4 //pin D2 (GPIO4) on NodeMCU
#define Arduino_TX 5 //pin D1 (GPIO5) on NodeMCU
//set up serial object for communication with arduino
SoftwareSerial ArduinoSerial(Arduino_RX, Arduino_TX);


const char* ssid = "Hello_IoT";
const char* password = "12345678";

// Create an instance of the server
// specify the port to listen on as an argumentii
WiFiServer server(80);
boolean connectWifi();

void setup() {
  Serial.begin(19200);
  delay(10);
  ArduinoSerial.begin(9600);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  

  //Setup WIFI mode, ssid, password
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint 
WiFi.softAP("Hello_IoT", "12345678"); // Provide the (SSID, password); 
//Setup http server
server.begin(); // Start the HTTP Server
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server 
Serial.print("Server IP is: "); // Print the IP to the monitor window 
Serial.println(HTTPS_ServerIP);
  
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  client.setTimeout(100); 
      String req = client.readString();

    Serial.println("Message from client start...)");
       Serial.println(req);
   Serial.println("...Message from client end)");       

  
  // Match the request
  int val;
  if (req.indexOf("/drone/0") != -1)  // led=on
    val = 0;
  else if (req.indexOf("/drone/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }


  client.flush();

   // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.print("Drone is now: ");
 
  if(val == 1) {
    client.print("Up");
    ArduinoSerial.print(1);
  } else {
    client.print("Down");
    ArduinoSerial.print(5);
  }
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
