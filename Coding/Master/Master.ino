#include<SPI.h>
char buff[]="hello slave\n";


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
SPI.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
for(int i=0; i<sizeof buff; i++)
SPI.transfer(buff[i]);
Serial.println(buff);
delay(1000);

}
