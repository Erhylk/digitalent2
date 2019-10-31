#include <WiFi.h>
#include "DHT.h"
#include <ThingSpeak.h>

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

char ssid[] = "sjsj";
char pass[] = "01234567";
int status = WL_IDLE_STATUS;

unsigned long myChannelNumber = 892512;
const char * myWriteAPIKey = "J6YTWRTC37GBNQE9";

void setup(){
Serial.begin(115200);
  delay(100);
//  pinMode(DHTPin, INPUT);
  dht.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);
  //connect to your local wi-fi network
  WiFi.begin(ssid, pass);
  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
//  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
//  server.on("/", handle_OnConnect);
//  server.onNotFound(handle_NotFound);
//  server.begin();
//  Serial.println("HTTP server started");
  ThingSpeak.begin(client);

}

void loop(){
  delay(2000);

  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  ThingSpeak.setField(1,f);
  ThingSpeak.setField(2,h);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

//  Serial.print("HUMIDITY: ");
//  Serial.print(h);
//  Serial.println(" %\t");
//  Serial.print("TEMPERATURE: ");
//  Serial.print(f);
//  Serial.println(" *F\t");

  delay(20000);
}
