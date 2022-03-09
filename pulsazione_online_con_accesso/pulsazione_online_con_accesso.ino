#include <SPI.h>
#include <PubSubClient.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

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
    pulsa();
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
      delay(2500);
    }
  }
}

void setup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    wm.setCustomHeadElement("<style>body{background:#D00C2E;color:white;}button{background:white;color:#D00C2E;}h1{color:white;}a{color:white;}:before{filter:invert(1);}:after{filter:invert(1);}</style>");
    //reset settings - wipe credentials for testing
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    wm.setConfigPortalTimeout(5200); //imposto i secondi di limite per impostare il wifi
    res = wm.autoConnect("#CUOREDINAPOLI",""); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

  Serial.begin(115200);
  
  client.setServer(server, 1883);
  client.setCallback(callback);
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
