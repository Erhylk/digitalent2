#include <ESP8266WiFi.h>

//masukan ssid dan password WIFI kita
const char* ssid     = "Teras Karya";
const char* password = "anaknegeri";

// Set server pada port 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output0State = "OFF";
String output1State = "OFF";
String output2State = "OFF";
String output3State = "OFF";
String output4State = "OFF";

// Assign output variables to GPIO pins
const int output0 = D0;
const int output1 = D1;
const int output2 = D2;
const int output3 = D3;
const int output4 = D4;


void setup() {
  Serial.begin(115200);

  // Atur Pin D0-D4 sebagai output
  pinMode(output0, OUTPUT);
  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);
  pinMode(output4, OUTPUT);
  
  digitalWrite(output0, LOW);
  digitalWrite(output1, LOW);
  digitalWrite(output2, LOW);
  digitalWrite(output3, LOW);
  digitalWrite(output4, LOW);

  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void clientApp (){

    WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
//    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
//        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the PIN on and off
            if (header.indexOf("GET /D0/ON") >= 0) {
              //Serial.println("PIN D0 ON");
              output0State = "ON";
              digitalWrite(output0, HIGH);
            } else if (header.indexOf("GET /D0/OFF") >= 0) {
              //Serial.println("PIN D0 off");
              output0State = "OFF";
              digitalWrite(output0, LOW);
            } else if (header.indexOf("GET /D1/ON") >= 0) {
              //Serial.println("PIN D1 ON");
              output1State = "ON";
              digitalWrite(output1, HIGH);
            } else if (header.indexOf("GET /D1/OFF") >= 0) {
              //Serial.println("PIN D1 OFF");
              output1State = "OFF";
              digitalWrite(output1, LOW);
            }else if (header.indexOf("GET /D2/ON") >= 0) {
              //Serial.println("PIN D2 ON");
              output2State = "ON";
              digitalWrite(output2, HIGH);
            } else if (header.indexOf("GET /D2/OFF") >= 0) {
              //Serial.println("PIN D2 OFF");
              output2State = "OFF";
              digitalWrite(output2, LOW);
            }else if (header.indexOf("GET /D3/ON") >= 0) {
              //Serial.println("PIN D3 ON");
              output3State = "ON";
              digitalWrite(output3, HIGH);
            } else if (header.indexOf("GET /D3/OFF") >= 0) {
              //Serial.println("PIN D3 OFF");
              output3State = "OFF";
              digitalWrite(output3, LOW);
            }else if (header.indexOf("GET /D4/ON") >= 0) {
              //Serial.println("PIN D4 ON");
              output4State = "ON";
              digitalWrite(output4, HIGH);
            } else if (header.indexOf("GET /D4/OFF") >= 0) {
              //Serial.println("PIN D4 OFF");
              output4State = "OFF";
              digitalWrite(output4, LOW);
            }
            
//            // Display the HTML web page
//            client.println("<!DOCTYPE html><html>");
//            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//            client.println("<link rel=\"icon\" href=\"data:,\">");
//            
//            // CSS to style the on/off buttons 
//            // Feel free to change the background-color and font-size attributes to fit your preferences
//            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
//            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
//            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
//            client.println(".button2 {background-color: #77878A;}</style></head>");
//            
//            // Web Page Heading
//            client.println("<body><h1>PROJECT SAMPLE WEB SERVER NODEMCU</h1>");
//            
//            // Display current state, and ON/OFF buttons for PIN D0
//            client.println("<p>PIN D0 - State " + output0State + "</p>");
//            // If the output5State is off, it displays the ON button       
//            if (output0State=="OFF") {
//              client.println("<p><a href=\"/D0/ON\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/D0/OFF\"><button class=\"button button2\">OFF</button></a></p>");
//            } 
//               
//            // Display current state, and ON/OFF buttons for PIN D1  
//            client.println("<p>PIN D1 - State " + output1State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output1State=="OFF") {
//              client.println("<p><a href=\"/D1/ON\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/D1/OFF\"><button class=\"button button2\">OFF</button></a></p>");
//            }

//            // Display current state, and ON/OFF buttons for PIN D2  
//            client.println("<p>PIN D2 - State " + output2State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output2State=="OFF") {
//              client.println("<p><a href=\"/D2/ON\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/D2/OFF\"><button class=\"button button2\">OFF</button></a></p>");
//            }
//
//            // Display current state, and ON/OFF buttons for PIN D3  
//            client.println("<p>PIN D3 - State " + output3State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output3State=="OFF") {
//              client.println("<p><a href=\"/D3/ON\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/D3/OFF\"><button class=\"button button2\">OFF</button></a></p>");
//            }
//
//            // Display current state, and ON/OFF buttons for PIN D4  
//            client.println("<p>PIN D4 - State " + output4State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output4State=="OFF") {
//              client.println("<p><a href=\"/D4/ON\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/D4/OFF\"><button class=\"button button2\">OFF</button></a></p>");
//            }
//            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
//    Serial.println("Client disconnected.");
//    Serial.println("");
  }
}
void loop(){
 clientApp();
}
