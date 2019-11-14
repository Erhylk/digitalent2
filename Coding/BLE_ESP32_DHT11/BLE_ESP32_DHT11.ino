#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <DHT.h>

#include <iostream>
#include <string>

BLECharacteristic *pCharacteristic;

bool deviceConnected = false;
const int LED = 2; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.

/*
 * Definisi DHT11
 */
#define DHTPIN 23 // pino de dados do DHT11
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int humidity;
int temperature;

// Kode UUID dari:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define DHTDATA_CHAR_UUID       "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" 


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.println(rxValue[0]);

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }

     //Teks Masukan led, A:on , B:off
      if (rxValue.find("A") != -1) { 
        Serial.println("Turning ON!");
        digitalWrite(LED, HIGH);
      }
      else if (rxValue.find("B") != -1) {
        Serial.println("Turning OFF!");
        digitalWrite(LED, LOW);
      }
    }
};

void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("ESP32 DHT11"); // Give it a name

  // BLE Service
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // UART service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Karakter BLE untuk kirim data
  pCharacteristic = pService->createCharacteristic(
                      DHTDATA_CHAR_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  //karakteristik BLE untuk menerima data
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Mulai service
  pService->start();

  // Mulai ESP32
  pServer->getAdvertising()->start();
  Serial.println("Menunggu klien untuk terhubung...");
}

void loop() {
  if (deviceConnected) {

    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(temperature) || isnan(humidity)) 
    {
      Serial.println("Failed to read from DHT");
    }
    else 
    {
      Serial.print("Kelembaban: ");
      Serial.print(humidity);
      Serial.print(" %\t");
      Serial.print("Suhu: ");
      Serial.print(temperature);
      Serial.println(" *C");
    }
    
    char humidityString[2];
    char temperatureString[2];
    dtostrf(humidity, 1, 2, humidityString);
    dtostrf(temperature, 1, 2, temperatureString);

    char dhtDataString[16];
    sprintf(dhtDataString, "%d,%d", temperature, humidity);
    
    pCharacteristic->setValue(dhtDataString);
    
    pCharacteristic->notify(); // Kirim nilai ke aplikasi!
    Serial.print("*** Terkirim ke BLE Scanner ");
    Serial.print(dhtDataString);
    Serial.println(" ***");
  }
  delay(1000);
}
