/*
  PZEM-004T V2.0 data to Blynk Mobile App

 This example shows how to log data from espwifi module
 to an Blynk Server.

  connection  
     node Mcu/Wemos         PZEM-004T V2.0
         vcc                    5v
         gnd                    gnd
         RX                     TX
         TX                     RX
 
 Note :  remove rx and tx pin while uploading and resting 
         connect rx and tx after uploading. 
         
 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
         
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004T.h>
PZEM004T pzem(&Serial);
IPAddress ip(192, 168, 1, 1);

float voltage_blynk = 0;
float current_blynk = 0;
float power_blynk = 0;
float energy_blynk = 0;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "**********************";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*********";
char pass[] = "*********";


unsigned long lastMillis = 0;

void setup()
{
  pzem.setAddress(ip);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();

  /// Read meter PZEM
  float v = pzem.voltage(ip);
  if (v >= 0.0) {
    voltage_blynk = v;  //V
  }

  float i = pzem.current(ip);
  if (i >= 0.0) {
    current_blynk = i;     //A
  }

  float p = pzem.power(ip);
  if (p >= 0.0) {
    power_blynk = p;        //kW
  }

  float e = pzem.energy(ip);
  if (e >= 0.0) {
    energy_blynk = e;   ///kWh
  }

  delay(1000);

  //Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();

    Blynk.virtualWrite(V1, voltage_blynk);
    Blynk.virtualWrite(V2, current_blynk  );
    Blynk.virtualWrite(V3, power_blynk);
    Blynk.virtualWrite(V4, energy_blynk  );
    Blynk.virtualWrite(V5, lastMillis  );
  }
}
