#include <WiFi.h>
//#include "style.css"

// GPIO the LED is attached to
static const int ledPin = 2;

// Setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int dutyCycle = 0;

// Replace with your network credentials
const char* ssid     = "sjsj";
const char* password = "mnbvcxza";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(5);
int pos1 = 0;

void setup() {
  // Configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // Attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  
  Serial.begin(115200);

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

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
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

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
						client.println("<link rel=\"stylesheet\" href=\"https://raw.githubusercontent.com/Erhylk/digitalent2/master/Coding/ESP32_PwmSlider/style.css\"");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
						client.println("</head><body>");
						client.println("<svg height=\"512pt\" viewBox=\"-56 0 512 512\" width=\"512pt\" xmlns=\"http://www.w3.org/2000/svg\"><g fill-rule=\"evenodd\"><path d=\"m139.089844 366.472656h122.46875v84.96875c-6.160156 28.914063-31.234375 50.558594-61.234375 50.558594-30.003907 0-55.078125-21.644531-61.234375-50.558594zm0 0\" fill=\"#67e5dd\"/><path d=\"m200.324219 74.585938c69.441406 0 125.738281 56.292968 125.738281 125.738281 0 87.078125-63.90625 115.398437-63.90625 152.617187 0 4.78125-.207031 9.285156-.601562 13.53125h-122.464844c-.390625-4.246094-.597656-8.75-.597656-13.53125 0-37.21875-63.90625-65.539062-63.90625-152.617187 0-69.445313 56.292968-125.738281 125.738281-125.738281zm0 0\" fill=\"#fec96b\" class=\"led\"/><path d=\"m200.324219 64.585938c-58.6875 0-110.484375 37.414062-128.898438 93.101562-1.734375 5.242188 1.113281 10.898438 6.355469 12.632812 5.246094 1.734376 10.902344-1.109374 12.632812-6.355468 15.699219-47.480469 59.867188-79.378906 109.910157-79.378906 63.816406 0 115.738281 51.917968 115.738281 115.738281 0 49.324219-22.09375 78.792969-39.84375 102.472656-12.914062 17.222656-24.066406 32.101563-24.066406 50.144531 0 1.191406-.015625 2.363282-.042969 3.53125h-41.785156v-22.972656c0-5.523438-4.476563-10-10-10-5.523438 0-10 4.476562-10 10v22.972656h-41.789063c-.027344-1.167968-.042968-2.34375-.042968-3.53125 0-18.046875-11.152344-32.921875-24.0625-50.148437-13.074219-17.441407-27.894532-37.210938-35.242188-65.300781-1.398438-5.34375-6.863281-8.542969-12.203125-7.144532-5.34375 1.398438-8.542969 6.863282-7.144531 12.207032 8.339844 31.886718 25.109375 54.257812 38.585937 72.234374 11.210938 14.957032 20.066407 26.773438 20.066407 38.152344 0 4.789063.203124 9.464844.597656 13.917969v84.585937c0 .269532.019531.53125.042968.792969.003907.066407 0 .128907.007813.195313.035156.367187.09375.730468.167969 1.085937.003906.003907.003906.003907.003906.007813 3.46875 16.289062 12.359375 31.101562 25.039062 41.707031 12.921876 10.8125 29.25 16.765625 45.972657 16.765625 16.722656 0 33.046875-5.953125 45.972656-16.765625 12.675781-10.605469 21.570313-25.417969 25.039063-41.707031 0-.003906.003906-.007813.003906-.011719.074218-.355469.128906-.714844.167968-1.085937.003907-.0625.003907-.128907.007813-.191407.019531-.265625.039063-.527343.039063-.796875v-84.582031c.394531-4.457031.601562-9.128906.601562-13.917969 0-11.382812 8.855469-23.195312 20.066406-38.152344 18.5-24.679687 43.839844-58.476562 43.839844-114.464843 0-74.847657-60.890625-135.738281-135.738281-135.738281zm-51.234375 354.371093h102.46875v22.484375h-102.46875zm102.46875-20h-102.46875v-22.484375h102.46875zm-51.234375 93.042969c-20.332031 0-38.667969-12.296875-47.355469-30.558594h94.710938c-8.691407 18.261719-27.027344 30.558594-47.355469 30.558594zm0 0\"/><path d=\"m200.324219 46.644531c5.523437 0 10-4.476562 10-10v-26.644531c0-5.523438-4.476563-10-10-10-5.523438 0-10 4.476562-10 10v26.644531c0 5.523438 4.476562 10 10 10zm0 0\" class=\"led\"/><path d=\"m36.644531 190.324219h-26.644531c-5.523438 0-10 4.476562-10 10 0 5.523437 4.476562 10 10 10h26.644531c5.523438 0 10-4.476563 10-10 0-5.523438-4.476562-10-10-10zm0 0\" class=\"led\"/><path d=\"m246.109375 220.527344c0-5.523438-4.476563-10-10-10h-71.570313c-5.523437 0-10 4.476562-10 10 0 5.523437 4.476563 10 10 10h25.785157v22.972656c0 5.523438 4.476562 10 10 10 5.523437 0 10-4.476562 10-10v-22.972656h25.785156c5.523437 0 10-4.476563 10-10zm0 0\" class=\"led\"/><path d=\"m77.515625 91.65625c1.953125 1.953125 4.511719 2.929688 7.070313 2.929688 2.558593 0 5.117187-.976563 7.070312-2.929688 3.90625-3.90625 3.90625-10.238281 0-14.140625l-18.84375-18.839844c-3.902344-3.90625-10.234375-3.90625-14.140625 0-3.90625 3.902344-3.90625 10.234375 0 14.140625zm0 0\" class=\"led\"/><path d=\"m70.617188 315.890625-18.84375 18.839844c-3.902344 3.90625-3.902344 10.238281 0 14.140625 1.953124 1.953125 4.511718 2.929687 7.074218 2.929687 2.558594 0 5.117188-.976562 7.070313-2.929687l18.839843-18.839844c3.90625-3.90625 3.90625-10.234375 0-14.140625s-10.238281-3.90625-14.140624 0zm0 0\" class=\"led\"/><path d=\"m316.0625 94.585938c2.558594 0 5.117188-.976563 7.070312-2.929688l18.839844-18.839844c3.90625-3.90625 3.90625-10.238281 0-14.144531-3.902344-3.90625-10.234375-3.902344-14.140625 0l-18.839843 18.839844c-3.90625 3.90625-3.90625 10.238281-.003907 14.144531 1.953125 1.953125 4.515625 2.929688 7.074219 2.929688zm0 0\" class=\"led\"/><path d=\"m330.03125 315.890625c-3.902344-3.90625-10.234375-3.90625-14.140625 0-3.90625 3.902344-3.90625 10.234375 0 14.140625l18.839844 18.839844c1.953125 1.953125 4.511719 2.929687 7.070312 2.929687 2.558594 0 5.117188-.976562 7.070313-2.929687 3.90625-3.902344 3.90625-10.234375 0-14.140625zm0 0\" class=\"led\"/><path d=\"m390.644531 190.324219h-26.644531c-5.523438 0-10 4.476562-10 10 0 5.523437 4.476562 10 10 10h26.644531c5.523438 0 10-4.476563 10-10 0-5.523438-4.476562-10-10-10zm0 0\" class=\"led\"/><path d=\"m200.324219 283.5c-5.507813 0-10 4.492188-10 10s4.492187 10 10 10c5.507812 0 10-4.492188 10-10s-4.492188-10-10-10zm0 0\" class=\"led\"/><path d=\"m74.585938 210.324219c5.507812 0 10-4.492188 10-10 0-5.507813-4.492188-10-10-10-5.511719 0-10 4.492187-10 10 0 5.507812 4.488281 10 10 10zm0 0\" /></g></svg>");
            client.println("<h1>ESP32 with PWM</h1>");
            client.println("<p>Position: <span id=\"pwmPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"255\" class=\"slider\" id=\"ledSlider\" onchange=\"pwm(this.value)\" value=\""+valueString+"\"/>");
            
            client.println("<script>const slider = document.getElementById(\"ledSlider\");");
            client.println("const pwmP = document.getElementById(\"pwmPos\"); pwmP.innerHTML = slider.value;");
						client.println("slider.value = 0;");
            client.println("slider.oninput = function() { slider.value = this.value; pwmP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function pwm(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
           
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              valueString = header.substring(pos1+1);

              dutyCycle = valueString.toInt();
              ledcWrite(ledChannel, dutyCycle);
              Serial.println(valueString); 
            }         
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
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
