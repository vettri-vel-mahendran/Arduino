/*
  OEE - IoT Based Over All Equipment Effectiveness Analytics system

  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//*****************************************************************
#include <PZEM004Tv30.h>
/* Use software serial for the PZEM
   Pin 13 Rx (Connects to the Tx pin on the PZEM)
   Pin 15 Tx (Connects to the Rx pin on the PZEM)
*/
PZEM004Tv30 pzem(13, 15);
//*****************************************************************
#include <time.h>
// Replace with your network credentials
const char* ssid     = "******";
const char* password = "******";

float timezone = 6 * 3600;
int dst = 0;
//****************************************************************
// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://*******.000webhostapp.com/post-esp-data.php";
String apiKeyValue = "******";
String Machine_Number = "22 IM-1phase";
String NTPtimestamp ;
String onNTPtimestamp = "0";
String offNTPtimestamp = "0";

unsigned long startmilli, finished, currentelapsed, previouselapsed, totalelapsed, runtime;
int flag1 = 1;
int flag2 = 1;
int motorstate = 1;

float voltage;
float current;
float power;
float energy;
float frequency;
float pf;
float ebAmount = 0;
float tariff = 0;
float ebUnits = 0;
//*******************************************************************************
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 25000;           // interval at which to blink (milliseconds)
//******************************************************************************
const char* mssid = "****";
const char* mpassword = "*****";

WiFiServer server(80);
//*****************************************************************************
const int rpmdataIN = 14; //IR sensor INPUT D5

unsigned long rpmprevmillis; // To store time
unsigned long rpmduration; // To store time difference
unsigned long rpmrefresh; // To store time for rpmrefresh of reading

float rpm = 1200; // RPM value

boolean rpmcurrentstate; // Current state of IR input scan
boolean rpmprevstate; // State of IR sensor in previous scan

//*****************************************************************************
#include "FirebaseESP8266.h"
//#include <ESP8266WiFi.h>
#define FIREBASE_HOST "**********.firebaseio.com"
#define FIREBASE_AUTH "************"
#define WIFI_SSID "***********"
#define WIFI_PASSWORD "**********"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;


unsigned long sendDataPrevMillis = 0;

String path = "/espval";

uint16_t count = 0;

void printResult(FirebaseData &data);

//*****************************************************************************
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while (!time(nullptr)) {
    Serial.print("*");
    delay(1000);
  }
  Serial.println("\nTime response....OK");
  //****************************************************************************************
  pinMode(2, OUTPUT);  // Connect Relay to NodeMCU's D4 Pin
  digitalWrite(2, 0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(mssid);

  WiFi.begin(mssid, mpassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  //**************************************************************************
  pinMode(rpmdataIN, INPUT);
  rpmprevmillis = 0;
  rpmprevstate = LOW;
  //***************************************************************************
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  if (!Firebase.beginStream(firebaseData, path))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  //**********************************************************************************
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    motorruntimecalc();
    httpsenddata();
    sensordata();
    rpmdata();
    mitonoff();
    firebasedatabase();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

void sensordata() {
  voltage = pzem.voltage();
  if ( !isnan(voltage) ) {
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  } else {
    voltage = 0;
    Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
  }

  current = pzem.current();
  if ( !isnan(current) ) {
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  } else {
    current = 0;
    Serial.print("Current: "); Serial.print(current); Serial.println("A");
  }

  power = pzem.power();
  if ( !isnan(power) ) {
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  } else {
    power = 0;
    Serial.print("Power: "); Serial.print(power); Serial.println("W");
  }

  energy = pzem.energy();
  if ( !isnan(energy) ) {
    Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
  } else {
    energy = 0;
    Serial.print("Energy: "); Serial.print(energy); Serial.println("kWh");
  }

  frequency = pzem.frequency();
  if ( !isnan(frequency) ) {
    Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
  } else {
    frequency = 0;
    Serial.print("Frequency: "); Serial.print(frequency); Serial.println("Hz");
  }

  pf = pzem.pf();
  if ( !isnan(pf) ) {
    Serial.print("PF: "); Serial.println(pf);
  } else {
    pf = 0;
    Serial.print("PF: "); Serial.println(pf);
  }
  costcal();
}
void httpsenddata()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Your Domain name with URL path or IP address with path
    HTTPClient http;
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&Machine_Number=" + Machine_Number
                             + "&Timestamp=" + NTPtimestamp + "&current=" + current
                             + "&voltage=" + voltage + "&power=" + power + "&energy=" + energy + "&frequency=" + frequency + "&pf=" + pf  + "&ontime=" + onNTPtimestamp + "&offtime=" + offNTPtimestamp + "&runtime=" + runtime + "&rpm=" + rpm + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&Timestamp=Office&current=24.75&voltage=49.54&power=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"current\":\"19\",\"voltage\":\"67\",\"power\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
}
void motorruntimecalc()
{

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  NTPtimestamp = String(p_tm->tm_mday) + "/" + String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + "  " + String(p_tm->tm_hour) + ":" + String(p_tm->tm_min) + ":" + String(p_tm->tm_sec);
  ///Serial.println(NTPtimestamp);

  if ((motorstate == 0) && (flag1 == 1))
  {
    ////Serial.println("motor on...");
    startmilli = millis();
    NTPtimestamp = String(p_tm->tm_mday) + "/" + String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + "  " + String(p_tm->tm_hour) + ":" + String(p_tm->tm_min) + ":" + String(p_tm->tm_sec);
    onNTPtimestamp = NTPtimestamp;
    flag1 = 0;
    flag2 = 0;
  }
  if ((motorstate == 1) && (flag2 == 0))
  {
    NTPtimestamp = String(p_tm->tm_mday) + "/" + String(p_tm->tm_mon + 1) + "/" + String(p_tm->tm_year + 1900) + "  " + String(p_tm->tm_hour) + ":" + String(p_tm->tm_min) + ":" + String(p_tm->tm_sec);
    offNTPtimestamp = NTPtimestamp;
    finished = millis();
    ////Serial.println("Finshed");
    currentelapsed = ((finished - startmilli) / 1000);
    ////Serial.print(currentelapsed);
    ////Serial.println("seconds elapsed");
    ////Serial.println();
    flag2 = 1;
    flag1 = 1;
    ////totalelapsed = (previouselapsed + currentelapsed);
    ////runtime == (totalelapsed / 60 );
    runtime = currentelapsed;
    ////Serial.println(runtime);
    previouselapsed = currentelapsed;
    ////Serial.println("total elapsed minutes");
    ////Serial.println();
  }

}
void costcal()
{
  if (energy > 0) {
    ebUnits = energy;
    if (ebUnits > 0 && ebUnits <= 100) {
      ebAmount = 20 + (ebUnits * 1.5);
      tariff = ebAmount;
    } else if (ebUnits <= 200) {
      ebAmount = 20 + ((ebUnits - 100) * 1.5);
      tariff = ebAmount;
    } else if (ebUnits <= 500) {
      ebAmount = 30 + 100 * 2 + (ebUnits - 200) * 3;
      tariff = ebAmount;
    } else if (ebUnits > 500) {
      ebAmount = 50 + 100 * 3.5 + 300 * 4.6 + (ebUnits - 500) * 6.6;
      tariff = ebAmount;
    }
  }
}

void mitonoff() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }


  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();


  if (req.indexOf("/OFF1") != -1)
    motorstate = 0;
  else if (req.indexOf("/ON1") != -1)
    motorstate = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, motorstate);
  digitalWrite(LED_BUILTIN, motorstate);
  client.flush();

  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (motorstate) ? "high" : "low";
  s += "</html>";
}
void rpmdata()
{
  // RPM Measurement
  rpmcurrentstate = digitalRead(rpmdataIN); // Read IR sensor state
  if ( rpmprevstate != rpmcurrentstate) // If there is change in input
  {
    if ( rpmcurrentstate == HIGH ) // If input only changes from LOW to HIGH
    {
      rpmduration = ( micros() - rpmprevmillis ); // Time difference between revolution in microsecond
      rpm = (60000000 / rpmduration); // rpm = (1/ time millis)*1000*1000*60;
      rpmprevmillis = micros(); // store time for nect revolution calculation
    }
  }
  rpmprevstate = rpmcurrentstate; // store this scan (prev scan) data for next scan

  // LCD Display
  if ( ( millis() - rpmrefresh ) >= 100 )
  {
    Serial.println("rpm:");
    Serial.print(rpm);
    Serial.println(" ");
  }
}

void firebasedatabase()
{
  if (millis() - sendDataPrevMillis > 15000)
  {
    sendDataPrevMillis = millis();
    Firebase.setFloat(firebaseData, "/Current" , current);
    Firebase.setFloat(firebaseData, "/Voltage" , voltage);
    Firebase.setFloat(firebaseData, "/Power" , power);
    //Firebase.setFloat(firebaseData, "/Energy" , energy);
    //Firebase.setFloat(firebaseData, "/Frequency" , frequency);
    //Firebase.setFloat(firebaseData, "/Pf" , pf);
    Firebase.setFloat(firebaseData, "/Speed" , rpm);
  }
}
