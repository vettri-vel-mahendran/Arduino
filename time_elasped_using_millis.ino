/*
  Time Elasped using Millis funtion

  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
unsigned long startmilli, finished, currentelapsed, previouselapsed, totalelapsed;
int flag1 = 1;
int flag2 = 1;
int serialinput;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    serialinput = Serial.parseInt();
    Serial.println(serialinput);
  }
  if ((serialinput == 1) && (flag1 == 1))
  {
    Serial.println("Start...");
    startmilli = millis();
    flag1 = 0;
    flag2 = 1;
  }
  if ((serialinput == 2) && (flag2 == 1))
  {
    finished = millis();
    Serial.println("Finshed");
    currentelapsed = ((finished - startmilli) / 1000);
    Serial.print(currentelapsed);
    Serial.println("seconds elapsed");
    Serial.println();
    flag2 = 0;
    flag1 = 1;
    totalelapsed = (previouselapsed + currentelapsed);
    Serial.println(totalelapsed / 60);
    previouselapsed = currentelapsed;
    Serial.println("total elapsed minutes");
    Serial.println();
  }

}
