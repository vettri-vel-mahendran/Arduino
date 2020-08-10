/*
  Light controll using diot app


  created 28 Mar 2019
  modified 28 Mar 2019
  by Vettri vel Mahendran

*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

// Update these with values suitable for you.
// Your WiFi settings
#define ssid      "********"             // the SSID of your WiFi network
#define password  "**********"  // you wifi password
// Your DIoTY settings
#define mqtt_server "mqtt.dioty.co"
#define port        1883
#define dioty_id    "******@gmail.com" // email address used to register with DIoTY
#define dioty_password  "********"      // your DIoTY password
#define clientNr        "01"        // used for client id (increment when deploying
// multiple instances at the same time)

// Some project settings
#define slash     "/"   // all topics are prefixed with slash and your dioty_id
#define topicConnect    "/ESP8266/connected"     // topic to say we're alive
#define topicIn         "/ESP8266/inTopic"       // topic to subscribe
#define GPIO0           0     // using GPIO 0 rather than 2 requires less power


// no changes required after this point

// DIoTY requires topic names to start with your userId, so stick it in the front
#define concat(first, second) first second

const char* connectTopic = concat(concat(slash, dioty_id), topicConnect);
const char* inTopic = concat(concat(slash, dioty_id), topicIn);

// failing to use unique client id's will cause you connection to drop,
// so make it unique's unique by using your userId and a sequence number
const char* client_id = concat(clientNr, dioty_id);


WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];    // message to publish
int value = 0;   // connection attempt

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // The mobile app will publish 'true' or 'false' to the topic
  // Switch on the LED if an 't' was received as first character, otherwise switch it off
  if ((char)payload[0] == 't') {
    Serial.println("payload[0] = 't': " + HIGH);
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off
    Serial.println("payload[0] <> 't': " + LOW);
  }

}

void setup() {
  Serial.begin(115200);
  Serial.println("setup...");
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the GPIO0 pin as an output
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("output " + HIGH);
  setup_wifi();
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    ++value;  //connection attempt
    if (client.connect(client_id, dioty_id, dioty_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      snprintf (msg, 50, "Successful connect #%ld", 26);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish(connectTopic, msg, true);
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
