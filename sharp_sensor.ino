/*
  Distance measurement using Sharp sensor


  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(500);
  int  dis = analogRead(A0);
  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);

  if (dis > 500)
  {
    Serial.println("object dedected");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }

}
