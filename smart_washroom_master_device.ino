/*
  Smart washroom master device

  
  
  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "*************";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "*****";
char pass[] = "******";
int s = 0;
int cleanbutton = D1;
int r = 0, pr = 0;
int cleanstatusled = D3;

int s2 = 0;
int userbutton = D2;
int r2 = 0, pr2 = 0;
int userstatusled = D8;

int buzzer = D7 ;
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(cleanbutton, INPUT);
  pinMode(cleanstatusled, OUTPUT);
  pinMode(userbutton, INPUT);
  pinMode(userstatusled, OUTPUT);

  pinMode(buzzer, OUTPUT);
}
WidgetBridge bridge1(V2);

// Timer for blynking
BlynkTimer timer;
/*
  static bool value = true;
  void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
  {
  // Send value to another device
  if (value) {

    bridge1.virtualWrite(V5, 1); // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.

    /////////////////////////////////////////////////////////////////////////////////////////
    // Keep in mind that when performing virtualWrite with Bridge,
    // second board will need to process the incoming command.
    // It can be done by using this handler on the second board:
    //
    //    BLYNK_WRITE(V5){
    //    int pinData = param.asInt(); // pinData variable will store value that came via Bridge
    //    }
    //
    /////////////////////////////////////////////////////////////////////////////////////////
  } else {

    bridge1.virtualWrite(V5, 0); // Sends 0 value to BLYNK_WRITE(V5) handler on receiving side.
  }
  // Toggle value
  value = !value;
  }
*/
BLYNK_CONNECTED() {
  bridge1.setAuthToken("D1MLvTUjuK794bUaLN-hUfHChlmRSpRm"); // Place the AuthToken of the second hardware here
}
void loop() {
  Blynk.run();
  cleanfnc();
  userfnc();
}
void cleanfnc()
{
  r = digitalRead(cleanbutton );
  if (r != pr) {
    if (r == HIGH) {
      s = !s;
      if (s == 1)
      {
        Serial.println("clean mode");
        digitalWrite(cleanstatusled, HIGH);
        bridge1.virtualWrite(V3, 0);
      }
      if (s == 0)
      {
        Serial.println("normal mode");
        digitalWrite(cleanstatusled, LOW);
        bridge1.virtualWrite(V3, 1);
      }

    }
  }
  pr = r;

}

void userfnc()
{

  r2 = digitalRead(userbutton);
  //if (r2 != pr2) {
  if (r2 == HIGH) {
    s2 = !s2;
    if (s2 == 1)
    {
      Serial.println("user ");
      digitalWrite(userstatusled, HIGH);
      bridge1.digitalWrite(D5, HIGH);
      digitalWrite(D7, HIGH);

      delay(10000);
      Serial.println("normal mode");
      digitalWrite(userstatusled, LOW);
      bridge1.digitalWrite(D5, LOW);
      digitalWrite(D7, LOW);


    }
  }
  pr2 = r2;

}
