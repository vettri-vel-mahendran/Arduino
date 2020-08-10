/*
  Device data to firebase database

 This example shows how to log data from esp module
 to an firebase database.
   
 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "********************"     //projectname-****.firebaseio.com
#define FIREBASE_AUTH "********************"
#define WIFI_SSID "*****"
#define WIFI_PASSWORD "******"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

String path = "/espval";

void printResult(FirebaseData &data);
long value1;  // variable1 to store random value 
long value3;  // variable2 to store random value 


void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  if (!Firebase.beginStream(firebaseData, path))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

}

void loop()
{
  value1 = random(17, 20);
  value2 = random(2, 10);

  Firebase.setInt(firebaseData, "/value1" , value1);
  Firebase.setInt(firebaseData, "/value2" , value2);

}
