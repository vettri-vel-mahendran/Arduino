/*
  PZEM-004T V3.0 data to Blynk Mobile App

 This example shows how to log data from espwifi module
 to an Blynk Server.

  connection  
     node Mcu/Wemos         PZEM-004T V3.0
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
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <PZEM004Tv30.h>
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "*********";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*******";
char pass[] = "*******";

/* Use software serial for the PZEM
   Pin 13 Rx (Connects to the Tx pin on the PZEM)
   Pin 15 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(13, 15);

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  float voltage = pzem.voltage();
  if ( !isnan(voltage) ) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");

  } else {
    Serial.println("Error reading voltage");
  }

  float current = pzem.current();
  if ( !isnan(current) ) {
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    Serial.println("Error reading current");
  }

  float power = pzem.power();
  if ( !isnan(power) ) {
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    Serial.println("Error reading power");
  }

  float energy = pzem.energy();
  if ( !isnan(energy) ) {
    Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
  } else {
    Serial.println("Error reading energy");
  }

  float frequency = pzem.frequency();
  if ( !isnan(frequency) ) {
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  float pf = pzem.pf();
  if ( !isnan(pf) ) {
    Serial.print("PF: "); Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }
  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);
  Blynk.virtualWrite(V3, energy);
  Blynk.virtualWrite(V4, frequency);
  Blynk.virtualWrite(V5, pf);
  Serial.println();
}
