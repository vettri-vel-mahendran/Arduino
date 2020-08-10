/*
  DHT11 temperature sensor

  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#include <dht11.h>
#define DHT11PIN D4

dht11 DHT11;

void setup()
{
  Serial.begin(9600);

}

void loop()
{
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);

  delay(3000);

}
