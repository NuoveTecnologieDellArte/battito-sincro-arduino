/*
  Basic MQTT example

  This sketch demonstrates the basic capabilities of the library.
  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

*/

#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "FASTWEB-SPAZIOTRA";
const char* password = "spaziotra2022";

// Update these with values suitable for your network.
IPAddress server(167,71,79,43);

//il pin che viene usato
int ledPin = 0;

// Questa callback è esattamente la funzione che viene chiamata nel momento in cui riceve un messaggio
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Pulso");
  pulsa();
}

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Una volta connesso si sottoscrive al topic 'main'
      client.subscribe("main");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  
  client.setServer(server, 1883);
  client.setCallback(callback);
  
  delay(10);

  Serial.println();
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

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void pulsa() {
  delay(100);

  for (int i = 0; i < 255; i++) {
    analogWrite(ledPin, i);
    delay(1);
  }
  for (int i = 255; i > 0; i--) {
    analogWrite(ledPin, i);
    delay(1);
  }
  for (int i = 0; i < 255; i++) {
    analogWrite(ledPin, i);
    delay(1);
  }
  delay(20);

  for (int i = 255; i >= 0; i--) {
    analogWrite(ledPin, i);
    delay(3);
  }
}
