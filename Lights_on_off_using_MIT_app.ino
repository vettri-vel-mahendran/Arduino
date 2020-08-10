/*
  Lights On Off using MIT mobile app

 This example shows how to control light using Mobile App which Created by MIT app Inventor.

 connection
        interface relay  with node mcu
        * relay Din pin to node mcu digital pin D5

 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
#include <ESP8266WiFi.h>
const char* ssid = "******";
const char* password = "******";

WiFiServer server(80);

void setup() {
  Serial.begin(115200); //Default Baud Rate for NodeMCU
  delay(10);


  pinMode(D5, OUTPUT); 
  digitalWrite(D5, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }


  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  int val;
  if (req.indexOf("/OFF1") != -1)
    val = 0;
  else if (req.indexOf("/ON1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(D5, val);

  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val) ? "high" : "low";
  s += "</html>";
}

