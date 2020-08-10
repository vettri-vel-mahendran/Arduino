/*
  Hall light and agri sync

  
  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "**********";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "****";
char pass[] = "*****";
float starttime, stoptime, runtime, totaltime = 0, totalcost = 0;
const uint16_t kIrLed = 3;  // ESP8266 GPIO pin to use. Recommended: 4 D2). connected to Rx (gpio3)

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t LightOff[71] = {9256, 4454,  662, 480,  664, 480,  664, 480,  664, 480,  664, 478,  666, 478,  656, 488,  656, 486,  668, 1586,  660, 1594,  662, 1592,  654, 1598,  658, 484,  660, 1594,  662, 1592,  666, 1588,  658, 486,  658, 1596,  660, 482,  662, 482,  662, 480,  664, 478,  664, 478,  656, 486,  656, 1598,  658, 484,  660, 1594,  662, 1590,  656, 1598,  658, 1596,  662, 1592,  654, 1600,  658, 41046,  9180, 2234,  604};
uint16_t LighrR[71] = {9250, 4452,  654, 488,  656, 486,  658, 484,  658, 484,  660, 482,  660, 482,  662, 480,  662, 480,  654, 1600,  658, 1596,  662, 1592,  654, 1600,  656, 486,  658, 1594,  662, 1592,  654, 1600,  658, 486,  658, 484,  660, 1594,  664, 478,  654, 488,  654, 488,  656, 486,  658, 484,  658, 1594,  664, 1590,  656, 486,  656, 1596,  660, 1594,  664, 1588,  658, 1596,  660, 1594,  664, 41032,  9236, 2174,  654};
uint16_t LightG[71] = {9234, 4474,  634, 510,  634, 510,  634, 510,  634, 510,  634, 510,  634, 508,  634, 510,  634, 508,  634, 1620,  636, 1618,  638, 1616,  640, 1614,  612, 532,  612, 1642,  604, 1650,  606, 1648,  608, 1646,  612, 532,  612, 1642,  604, 540,  604, 538,  604, 538,  604, 538,  606, 538,  604, 538,  606, 1648,  608, 534,  610, 1644,  612, 1642,  604, 1650,  606, 1648,  608, 1646,  610};
uint16_t LightB[71] = {9212, 4508,  608, 534,  610, 536,  608, 536,  608, 536,  606, 538,  606, 538,  606, 538,  604, 540,  604, 1652,  604, 1650,  606, 1648,  608, 1648,  610, 534,  610, 1646,  610, 1646,  612, 1644,  602, 542,  612, 1642,  604, 1652,  606, 538,  604, 540,  604, 542,  602, 542,  602, 540,  602, 1652,  604, 538,  604, 540,  604, 1650,  606, 1648,  608, 1648,  610, 1648,  610, 1644,  612, 41096,  9202, 2228,  610};


BLYNK_WRITE(V2)
{
  int pinValue3 = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue3 == 1)
  {
    irsend.sendRaw(LightG, 71, 38);
    Serial.println("Green");
  }
  else
  {
    irsend.sendRaw(LightOff, 71, 38);
    Serial.println("off");
  }
}
BLYNK_WRITE(V3)
{
  int pinValue3 = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue3 == 1)
  {
    irsend.sendRaw(LightB, 71, 38);
    Serial.println("Blue");
  }
  else
  {
    irsend.sendRaw(LightOff, 71, 38);
    Serial.println("OFF");
  }
}
BLYNK_WRITE(V4)
{
  int pinValue3 = param.asInt(); // assigning incoming value from pin V1 to a variable
  if (pinValue3 == 1)
  {
    irsend.sendRaw(LighrR, 71, 38);
    Serial.println("Red");
  }
  else
  {
    irsend.sendRaw(LightOff, 71, 38);
    Serial.println("off");
  }
}

// This code will update the virtual port 5
BLYNK_WRITE(V5) {
  int pinData = param.asInt();
}

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}
