/*
  Rotary Encoder 

 This example shows how to log data from rotary encoder to display in serial monitor .

 created 28 Mar 2019
 modified 28 Mar 2019
 by Vettri vel Mahendran

 */
int val;
int encoder0PinA = 2;
int encoder0PinB = 3;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;
int flag = 0;
int my;
float rotation;
void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);
  rotation = 0;
}

void loop() {
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    flag = 0;
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
      //Serial.println("counter clockwise");
    } else {
      encoder0Pos++;
      // Serial.println("clockwise");
    }
    my = (encoder0Pos % 360);
    //Serial.println (my);
    
    if (((my == 359) || (my == -359) && (flag == 0) ))
    {
      rotation = rotation + 1;
      Serial.print("rotation =");
      Serial.print(rotation);
      Serial.println(" ");
      flag = 1;
    }
  }
  encoder0PinALast = n;
}
