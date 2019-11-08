#include <AWS_IOT.h>
#include <WiFi.h>
 
#include <DHT.h>
 
// include your personal WiFi and AWS configuration.
#include "config.h"
 
#define DHTTYPE DHT11
 
uint8_t DHTPin = 13;
 
DHT dht(DHTPin, DHTTYPE);
 
AWS_IOT hornbill;
 
int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0;
char payload[512];
char rcvdPayload[512];
 
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}
 
 
 
void setup() {
    Serial.begin(9600);
    //pinMode(DHTPin, INPUT);
   
 
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
        // wait 5 seconds for connection:
        delay(5000);
    }
 
    Serial.println("Connected to wifi");
 
    if(hornbill.connect(HOST_ADDRESS, CLIENT_ID,
                aws_root_ca_pem, certificate_pem_crt, private_pem_key)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);
 
        if(0 == hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfull");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }
 
    delay(2000);
    dht.begin();
 
}
 
void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

   //Convert float to string
    String hh = String(h);
    String tt = String(t);

    
    //Convert to JSON
    String message = "{ \"Device_ID\": \"ESP32\", \"Humidity\": \"" + hh + "\", \"Temperature\": \""+ tt +"\"}";
    
    Serial.print("JSON Message =");
    Serial.println(message);
    
    //convert string to char
    const char * msg = message.c_str();
   
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
   
    if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
    }
    else
    {
        sprintf(payload,"%s",msg); // Create the payload for publishing
       
        if(hornbill.publish(TOPIC_NAME,payload) == 0)   // Publish the message(Temp and humidity)
        {        
            Serial.print("Publish Message:");  
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed");
        }
        // publish the temp and humidity every 5 seconds.
        vTaskDelay(5000 / portTICK_RATE_MS);
       
    tick++;
    }
    
}
