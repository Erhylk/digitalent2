#include <WiFi.h>
#define  LED_BUILTIN 2
//int httpPOST(String uri, String postMessage, String &response);

const char ssid[] = "DTS.3minggu.lagi";
const char pass[] = "8karakter";

WiFiClient client;

int button = 15;
unsigned long myTalkBackID = 35574;
const char * myTalkBackKey = "ZTR1W8UQ1U6JIBP1";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
}

void loop() {
  if(WiFi.status() !=WL_CONNECTED){
    Serial.print("Attemp to Connect to SSID: ");
    Serial.println(String(ssid));
    while(WiFi.status() !=WL_CONNECTED){
      WiFi.begin(ssid, pass);
      Serial.print("Not Connect");
      delay(1000);
      }
     Serial.println("\nConnected.");
  }
     int nilai = digitalRead(button);
     int counter;
     String value;

     counter = nilai;

     if(counter==1){
      value = "TURN_ON";
      Serial.println(counter);
      if(counter==0){
        value = "TURN_OFF";
      }
  }
  //Create Tallback URI
  String tbURI = String("/talkbacks/") + String(myTalkBackID) + String("/commands.json?");
  
  // Create the message body for the POST out of the values
  String postMessage =  String("api_key=") + String(myTalkBackKey) + String("&command_string=") + value + String("&position=") + String("1")  ;

  //Make a string for any commands that might be in the queue
  String newCommand = String();

  //Make the post to thingspeak
  int x = httpPOST(tbURI, postMessage, newCommand);
  client.stop();

  //Check the result
 if(x == 200){
  Serial.println("checking queue..."); 
    // check for a command returned from TalkBack
    if(newCommand.length() != 0){
      Serial.print(" Latest command from queue: ");
      Serial.println(newCommand);
      
      if(newCommand == "TURN_ON"){
        digitalWrite(LED_BUILTIN, HIGH);  
      }

      if(newCommand == "TURN_OFF"){
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    else{
      Serial.println("  Nothing new.");  
    }
 }
 else
 {
  Serial.println("proble checking queue. HTTP error code " + String(x));
 }

 delay(6000);
}

 //General function to POST to Thingspeak
 int httpPOST(String uri, String postMessage, String &response){

  bool connectSuccess = false;
  connectSuccess = client.connect("api.thingspeak.com",80);

  if(!connectSuccess){
      return -301;   
  }
  
  postMessage += "&headers=false";
  
  String Headers =  String("POST ") + uri + String(" HTTP/1.1\r\n") +
                    String("Host: api.thingspeak.com\r\n") +
                    String("Content-Type: application/x-www-form-urlencoded\r\n") +
                    String("Connection: close\r\n") +
                    String("Content-Length: ") + String(postMessage.length()) +
                    String("\r\n\r\n");

   client.print(Headers);
   Serial.println(Headers);
   client.print(postMessage);
   Serial.println(postMessage);

   long startWaitForResponseAt = millis();
   while(client.available() == 0 && millis() - startWaitForResponseAt < 5000){
    delay(100);
    }

    if(client.available() == 0){
      return -304; //Didnt get server
    }
    if(!client.find(const_cast<char *>("HTTP/1.1"))){
      return -303; // Couldn't parse response (didn't find HTTP/1.1)
    }
    
    int status = client.parseInt();
    if(status != 200){
      return status;
    }

    if(!client.find(const_cast<char *>("\n\r\n"))){
    return -303;
    }

    String tempString = String(client.readString());
    response = tempString;

    return status;

}
