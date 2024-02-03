#include <Arduino.h>

#include <ArduinoBLE.h>
#include <SPI.h>
#include <Wire.h>

#define SDCARD
#define LED 2
//#define SLEEP

#ifdef SLEEP
  #include <LowPower.h>
#endif

#ifdef SDCARD
  #include <SD.h>

  void saveToCSV(const String deviceType, const String deviceName, const String deviceAddress) {
    digitalWrite(LED, HIGH);
    File dataFile = SD.open("devices.csv", FILE_WRITE);

    dataFile.print(deviceType);
    dataFile.print(",");
    dataFile.print(deviceName);
    dataFile.print(",");
    dataFile.println(deviceAddress);
    dataFile.close();

    Serial.println("Written to CSV file");
    digitalWrite(LED, LOW);
  }
#endif

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(3000);

  Serial.print("Starting BLE...");
  if (!BLE.begin()) {Serial.println(" failed!\n"); while (1);}
  else{Serial.println(" success!\n");}

  delay(500);

  #ifdef SDCARD
    Serial.print("Initializing SD card...");
    if (!SD.begin(4)) {Serial.println(" failed!\nhalting"); while (1);}
    else{Serial.println(" success!\n");}

    delay(500);
  #endif

  digitalWrite(LED, LOW);
  Serial.println("Scanning for devices...");
}

void loop() {
  BLE.scan();
  BLEDevice peripheral = BLE.available();
  BLEDevice central = BLE.central();

  if (peripheral) {
    Serial.println("Fount peripheral device");
    Serial.print(" > name: ");
    Serial.println(peripheral.localName());
    Serial.print(" > adress: ");
    Serial.println(peripheral.address());
    Serial.println();

    #ifdef SDCARD
      saveToCSV("p", peripheral.localName(), peripheral.address());
    #endif
  }

  if (central) {
    Serial.println("Fount central device");
    Serial.print(" > name: ");
    Serial.println(central.localName());
    Serial.print(" > address: ");
    Serial.println(central.address());
    Serial.println();

    #ifdef SDCARD
      saveToCSV("c", central.localName(), central.address());
    #endif
  }

  #ifdef SLEEP
    delay(100);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  #else
    delay(1000);
  #endif
}