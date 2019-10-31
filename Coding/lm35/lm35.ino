float vref = 3.3;
float resolution = vref/1023;
float Pin = 34;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
float temperatur = analogRead(Pin);
temperatur = (temperatur*resolution);
temperatur = temperatur*100;
Serial.println(temperatur);
delay(1000);
}
