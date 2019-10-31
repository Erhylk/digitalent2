#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ghina";
const char* password = "1234567890";

WebServer server(80);

uint8_t led1 = 2; 

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int dutyCycle;

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(dutyCycle));
}

void handle_intensity() {
  server.send(200, "text/html", SendHTML(dutyCycle));
  ledcWrite(ledChannel, dutyCycle);
  Serial.println(dutyCycle);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(led1, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handle_OnConnect);
  server.on("/intent", handle_intensity);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(led1, ledChannel);
}

void loop(void) {
  server.handleClient();
}

String SendHTML(uint8_t led1stat){
  String teks = "<!DOCTYPE html> <html>\n";

  teks += "<head> <meta name='viewport' content='width=device-width, initial-scale=1'>\n";
  teks += "<style> .slidecontainer {width: 100%;}</style></head>\n";
  
  teks += "<body> <div class='slidecontainer'><a href=\"/intent\"\"><input type='range' min='0' max='255' value='0' id='myRange'></a><p>Intensitas: <span id='demo'></span></p></div> </body>\n";
  teks += "<script> var slider = document.getElementById('myRange');var output = document.getElementById('demo');output.innerHTML = slider.value;slider.oninput = function() {output.innerHTML = this.value;}</script>\n";

  teks += "</body> </html>";
  
return teks;
}
