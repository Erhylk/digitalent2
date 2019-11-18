#include <SPI.h>
#include <LoRa.h>
const uint32_t CH_04_900 = 0xE3EE14;
#define ss 10
#define rst 5
#define dio0 2

int counter = 0;

const int pinADC = A1;
int sensitivitas = 66; //tegantung sensor arus yang digunakan, yang ini 5A
int nilaiadc= 0;
int teganganoffset = 2500; //nilai pembacaan offset saat tidak ada arus yang lewat
double tegangan = 0;
double nilaiarus = 0;
double rata;

//waterflow
byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

float calibrationFactor = 98; The hall-effect flow sensor outputs approximately 98
double rata;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

void setup(){
Serial.begin(115200); //baud komunikasi serial monitor 9600bps
Serial.print("MODULE ARUS"); 

while(!Serial);
Serial.println("Lora Sender");
LoRa.setPins(ss,rst, dio0);
while (!LoRa.begin(911.72E6)) {
Serial.println(".");
delay(500);
}
LoRa.setSyncWord(CH_04_900);
Serial.println("LoRa Initializing OK!");

//waterflow
pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
    // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

}

void data_olah(){
  rata=0;
  for (int i=0;i<10;i++){
  nilaiadc = analogRead(pinADC);
  tegangan = (nilaiadc / 1024.0) * 5000;
  nilaiarus = ((tegangan - teganganoffset) / sensitivitas);
  rata=rata+nilaiarus;
  Serial.print("Nilai ADC yang terbaca = " );
  Serial.print(nilaiadc);

  Serial.print("\t tegangan (mV) = ");
  Serial.print(tegangan,3);
 
  Serial.print("\t Arus = ");
  Serial.println(nilaiarus,3);
  delay(1000);
  }
}

void loop(){
//Waterflow
if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");        
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    Serial.print("\t");       // Print tab space
  Serial.print(totalMilliLitres/1000);
  Serial.print("L");
    

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
//  ACS
   data_olah();
//  Serial.print("Nilai ADC yang terbaca = " );
//  Serial.print(nilaiadc);

//  Serial.print("\t tegangan (mV) = ");
//  Serial.print(tegangan,3);
 
  Serial.print("\t Arus = ");
  Serial.println(rata,3);
  LoRa.beginPacket();
  LoRa.print(rata);
  LoRa.endPacket();

//  delay(1000);
}
