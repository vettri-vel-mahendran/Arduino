/*
  NTP-Timestamp data to cloud MQTT as JSON 

 This example shows how to log data from espwifi module
 to an Cloud MQTT.

 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>   // Read the rest of the article
#include <stdlib.h>
#include <time.h>
#include <ArduinoJson.h>

const char *ssid =  "******";   // cannot be longer than 32 characters!
const char *pass =  "******";   // Password
const char *mqtt_server = "m15.cloudmqtt.com";
const int mqtt_port = 15158;
const char *mqtt_user = "******";
const char *mqtt_pass = "******";
const char *mqtt_client_name = "******"; // Client connections cant have the same connection name

unsigned long previousMillis = 0;
const long interval = 10000;
float timezone = 5.5 * 3600;
int dst = 0;

WiFiClient wclient;  //Declares a WifiClient Object using ESP8266WiFi
PubSubClient client(wclient, mqtt_server,  mqtt_port);  //instanciates client object

//Function is called when, a message is recieved in the MQTT server.
void callback(const MQTT::Publish& pub) {
  Serial.println(pub.payload_string());

  if (pub.payload_string() == "on") {
    //Code to blink the LED -- its strange that I can't blink the LED for more than 1sec.
    digitalWrite(LED_BUILTIN, LOW);
    delay(4000);
    Serial.println("led on");
  }
  if (pub.payload_string() == "off") {
    //Code to blink the LED -- its strange that I can't blink the LED for more than 1sec.
    digitalWrite(LED_BUILTIN, HIGH);
    delay(4000);
    Serial.println("led off");
  }
}

void setup() {
  // Setup console
  Serial.begin(115200);  //set the baud rate
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);                      // Wait for a 2 second
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while (!time(nullptr)) {
    Serial.print("*");
    delay(1000);
  }
  Serial.println("\nTime response....OK");
}
void loop() {
  if (WiFi.status() != WL_CONNECTED) {  //wifi not connected?
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }
  if (WiFi.status() == WL_CONNECTED) {
    //client object makes connection to server
    if (!client.connected()) {
      Serial.println("Connecting to MQTT server");
      //Authenticating the client object
      if (client.connect(MQTT::Connect("mqtt_client_name")
                         .set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server");
        //Subscribe code
        client.set_callback(callback);
        client.subscribe("Arduino");
      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }
    if (client.connected())
      client.loop();
  }
  Sendjsondata();
}
void Sendjsondata() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  String a = String(p_tm->tm_mday) + "/" + String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + "  " + String(p_tm->tm_hour) + ":" + String(p_tm->tm_min + 30) + ":" + String(p_tm->tm_sec);
  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  root[ "power"] = "power";
  root[ "timestamp"] = a;
  root[ "voltage"] = 230;
  JsonArray data = root.createNestedArray("data");
  data.add(48.756080);
  serializeJsonPretty(root, Serial);
  String dis = doc.as<String>();
  client.publish("Arduino", String(dis) );
}
