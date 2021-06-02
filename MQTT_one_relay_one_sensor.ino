// mqqt command to publish for LED on/off
// mosquitto_pub -h 192.168.1.7 -t "garden/switch1/control" -m "ON"
// mosquitto_pub -h 192.168.1.7 -t "garden/switch2/control" -m "OFF"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Keh Si Haw";
const char* password = "1Qaz2Wsx";
const char* mqtt_server = "192.168.1.165";

const int stateMessageEverySec1 = 5;
const int stateMessageEverySec2 = 10;
#define SWITCH1 4
const char* switch1ControlTopic = "garden/switch1/control";
const char* switch1StateTopic = "garden/switch1/state";

#define LED 13
/*
#define SWITCH2 16
const char* switch2ControlTopic = "esp8285/switch2/control";
const char* switch2StateTopic = "esp8285/switch2/state";
*/
#define sensor_pin A0  // ESP8266 Analog Pin ADC0 = A0

WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi() {
  delay(10);
  Serial.println(); Serial.print("Connecting to "); Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("garden/testOut", "hello world");
      // ... and resubscribe
      client.subscribe(switch1ControlTopic);
     // client.subscribe(switch2ControlTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishRelayState(const char* relayTopic, int relayCode) {
  char* message;
  
  if (digitalRead(relayCode) == LOW) {
    message = "OFF";
  } else {
    message = "ON";
  }
  
  client.publish(relayTopic, message);
}

void processRelayCallback(const char* controlTopic, const char* stateTopic, int relayCode, char* topic, char* command) {
    if (strcmp(controlTopic, topic) == 0) {
     if (strcmp(command, "ON") == 0) {
        Serial.println("ON");
        digitalWrite(relayCode, HIGH);
        digitalWrite(relayCode+9, HIGH);
        publishRelayState(stateTopic, relayCode);
     } else if (strcmp(command, "OFF") == 0) {
        Serial.println("OFF");
        digitalWrite(relayCode, LOW);
        digitalWrite(relayCode+9, LOW);
        publishRelayState(stateTopic, relayCode);
     } else {
         Serial.println("Unknown command");
     }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';
  char *cstring = (char *) payload;
  Serial.print(cstring);
  Serial.println();

  processRelayCallback(switch1ControlTopic, switch1StateTopic, SWITCH1, topic, cstring);
  //processRelayCallback(switch2ControlTopic, switch2StateTopic, SWITCH2, topic, cstring);
}

void setup() {
  pinMode(SWITCH1, OUTPUT); 
  pinMode(LED, OUTPUT);
   
  Serial.begin(9600);
  
  setupWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  digitalWrite(SWITCH1, LOW);
 // digitalWrite(SWITCH2, LOW);
}

unsigned long lastMsg = 0;
unsigned long lastMsg1 = 0;
void loop() {
  
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > stateMessageEverySec1 * 1000) { // So this will be publishing every second
    lastMsg = now;
    publishRelayState(switch1StateTopic, SWITCH1);
   // publishRelayState(switch2StateTopic, SWITCH2);
  }
  if (now - lastMsg1 > stateMessageEverySec2 * 1000) { // So this will be publishing every 30 second
    lastMsg1 = now;
   
     int p;
      p = ( (analogRead(sensor_pin)/1023.00) * 100.00 ) ;
     char moistureTemp[10];
     sprintf(moistureTemp,"%i",p);
   // dtostrf(p, 6, 2, moistureTemp);
      Serial.print  ("Soil moisture in %:");
      Serial.println (p);
      client.publish("garden/soil_moisture", moistureTemp);
   // publishRelayState(switch2StateTopic, SWITCH2);
  }
}
