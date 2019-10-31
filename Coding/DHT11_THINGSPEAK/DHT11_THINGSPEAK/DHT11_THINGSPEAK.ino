#include <WiFi.h>
#include "DHT.h"
//#include <ThingSpeak.h>

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

const char* ssid = "sjsj";  // Enter SSID here
const char* pass = "01234567";  //Enter Password here
String thingSpeakAddress = "api.thingspeak.com";
String request_string;
String writeAPIKey;
String tsfield1Name;



void setup()
{
  Serial.begin(115200);
       delay(10);
       dht.begin();
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}


void loop()
{
  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  kirim_thingspeak(t, h);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void kirim_thingspeak(float SUHU, float KELEMBABAN) {
  if (client.connect("api.thingspeak.com", 80)) {
    request_string = "/update?";
    request_string += "key=";
    request_string += "YV7XVW7R1BDWJYLF";
    request_string += "&";
    request_string += "field1";
    request_string += "=";
    request_string += SUHU;
    request_string += "&";
    request_string += "field2";
    request_string += "=";
    request_string += KELEMBABAN;
    Serial.println(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
                 
    client.print(String("GET ") + request_string + " HTTP/1.1\r\n" +
                 "Host: " + thingSpeakAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");

  }
}
