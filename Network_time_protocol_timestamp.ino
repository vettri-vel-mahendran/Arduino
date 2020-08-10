/*
  NTP Timestamp

  This example shows how to log timestamp data using Network time protocol from esp module.

  
  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <time.h>

const char* ssid = "*******";
const char* password = "*******";

float timezone = 6 * 3600;
int dst = 0;

String timestamp;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  Serial.println();

  Serial.print("Connecting");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

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

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  /*
    Serial.print(p_tm->tm_mday);
    Serial.print("/");
    Serial.print(p_tm->tm_mon + 1);
    Serial.print("/");
    Serial.print(p_tm->tm_year + 1900);

    Serial.print(" ");

    Serial.print(p_tm->tm_hour);
    Serial.print(":");
    Serial.print(p_tm->tm_min);
    Serial.print(":");
    Serial.println(p_tm->tm_sec);
  */
  String timestamp = String(p_tm->tm_mday) + "/" + String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + "  " + String(p_tm->tm_hour - 1) + ":" + String(p_tm->tm_min + 30) + ":" + String(p_tm->tm_sec);
  Serial.println(timestamp);
  delay(1000);

}
