/*
  Hall light and agri sync

  
  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "*********";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*****";
char pass[] = "******";

int motorstatus;

SimpleTimer timer;

WidgetBridge bridge1(V1);

BLYNK_CONNECTED() {
  // Place the AuthToken of the second hardware here
  bridge1.setAuthToken("************");
}

void sendSensor() {
  int motorstatus = digitalRead(D5);
  if (motorstatus == 1)
  { Serial.println("motor on");
    bridge1.virtualWrite(V2, 1);
    bridge1.virtualWrite(V5, 1);
  }
  if (motorstatus == 0)
  {
    Serial.println("motor off");
    bridge1.virtualWrite(V3, 1);
    bridge1.virtualWrite(V5, 0);
  }

}


void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(D5, OUTPUT);
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
