/*
 LDR data blynk app

 This example shows how to log LDR(light depended resistant) sensor data from esp module to blynk app.

 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "********"; //Authentication token 
char ssid[] = "********";  //Service Set IDentifier i.e, wifi name
char pass[] = "*******";   //wifi password

void setup()
{
  Serial.begin(9600); //baud rate
  Blynk.begin(auth, ssid, pass);

}
void loop() {
  Blynk.run();
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
  Blynk.virtualWrite(V0,voltage);
}
