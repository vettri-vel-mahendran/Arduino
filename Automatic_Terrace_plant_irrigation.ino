/*
  Automatic Terrace Plant Irrigation

  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran
*/
const int sensor_pin = A0;  /* Soil moisture sensor O/P pin */
int relay = 2;

void setup() {
  Serial.begin(9600); /* Define baud rate for serial communication */
  pinMode(relay , OUTPUT);
  pinMode(A0 , INPUT);
}

void loop() {
  int moisture;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture = ( 73 - ( (sensor_analog / 1023.00) * 100 ) );
  Serial.print("Moisture = ");
  Serial.print(moisture);
  Serial.print("%\n\n");

  if ((moisture <= 0) || (moisture >= 20))
  {
    digitalWrite(relay, LOW);
  }
  if ((moisture <= 20) || (moisture >= 60))

  {
    digitalWrite(relay, HIGH);
  }
}


