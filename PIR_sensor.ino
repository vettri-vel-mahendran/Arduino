/*
  PIR sensor
  Passive infrared sensor interafce with arduino UNO

  
  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
int pirPin = D7;
int val = LOW;
int relay = D1;
void setup()
{
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
}

void loop()
{
  val = digitalRead(pirPin);
  //low = no motion, high = motion
  if (val == LOW)
  {

    Serial.println("No motion");

    digitalWrite(D1, LOW);
  }
  else
  {
    Serial.println("Motion detected  ALARM");
    digitalWrite(D1, HIGH);
  }

  delay(1000);
}
