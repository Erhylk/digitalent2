#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = ".";
const char* pass = "wow12345icircuit/presence/ESP32";
const char* mqtt_server = "farmer.cloudmqtt.com";

#define mqtt_port 14749
#define MQTT_USER "yhjzktkl"
#define MQTT_PASSWORD "LjyEy13YD6A6"
#define MQTT_SERIAL_RECEIVED_CH "/icircuit/presence/ESP32/rx"
#define MQTT_SERIAL_PUBLISH_CH "/icircuit/presence/ESP32/tx"

WiFiClient wifiClient;

PubSubClient client(wifiClient);


void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Connect to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() !=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("wifi connect");
  Serial.println("IP address");
  Serial.println(WiFi.localIP());
}

void reconnect(){
while (!client.connected()){
  Serial.print("ATTEP MQTT CONNECT.....");
  String clientId = "ESP32Client-";
  clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)){
      Serial.println("connected");
      client.publish("/icircuit/presence/ESP32/" , "hello world");
      client.subscribe(MQTT_SERIAL_RECEIVED_CH);
    }
    else
    {
      Serial.print("faild, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 s ");
      delay(5000);
    }
    }
  }

  void callback(char* topic, byte *playload, unsigned int length){
    Serial.println("-------new message from broker------");
    Serial.print("channel: ");
    Serial.println(topic);
    Serial.print("data: ");
    Serial.write(playload, length);
    Serial.println();
}

void setup(){
  Serial.begin(115200);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(mqtt_server, 14749);
  client.setCallback(callback);
  reconnect();
}

void publishSerialData(char *serialData){
  if (!client.connected()){
    reconnect();
  }
  client.publish(MQTT_SERIAL_PUBLISH_CH, serialData);
  }
void loop(){
  client,loop();
  if (Serial.available() > 0){
    char mun[501];
    memset(mun,0, 501);
    Serial.readBytesUntil( '\n', mun, 500);
    publishSerialData(mun);
  }
}
