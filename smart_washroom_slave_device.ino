
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "**************";

char ssid[] = "******";
char pass[] = "******";
int counter = 0;
int personcount = 0;
int hitObject = false;
int ir = D4; //black - o/p, blue - gnd, brown - 5v
int val = 1 , pval = 1;
int flushrelay = D5;
int pinData;
int pinData2;
int first = 0;
BLYNK_WRITE(V3) {
  pinData = param.asInt(); // pinData variable will store value that came via Bridge
}
BLYNK_WRITE(V4) {
  pinData2 = param.asInt(); // pinData variable will store value that came via Bridge
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  pinMode(ir, INPUT); // D4
  pinMode(flushrelay, OUTPUT); // solinoid //D5  //flush
}

void loop()
{
  Blynk.run();

  if (pinData == 1)
  {
    val = digitalRead(ir); // 0 for detect
    if (val != pval) {
      if (val == 0) {
        counter++;
        personcount++;
        Serial.print("counter =");
        Serial.println(counter);
        Serial.print("personcount =");
        Serial.println(personcount);
        Serial.println("  ");

      }
      else
      {
        digitalWrite(flushrelay, HIGH); // solenoid
        delay(5000);
        digitalWrite(flushrelay, LOW); // solenoid
      }
      delay(50);
    }
    pval = val; // 0 = 0;
    if (counter >= 5 ||  (pinData2 == 1) )
    {
      counter = 0;
      digitalWrite(flushrelay, HIGH); // solenoid
      delay(10000);
      digitalWrite(flushrelay, LOW); // solenoid
      Serial.println("pre clean user ");
    }
    first = millis();
  }
  else
  {
    if (millis() > (first + 60000))
    {
      Serial.println("cleaning process is going");
      Blynk.virtualWrite(V3, 1);
      pinData = 1;
    }

  }
  Blynk.virtualWrite(V0, counter);
  Blynk.virtualWrite(V1, personcount);
}
