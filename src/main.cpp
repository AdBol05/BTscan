#include <Arduino.h>

#include <ArduinoBLE.h>
#include <LowPower.h>
#include <SPI.h>
#include <Wire.h>

#define SDCARD

#ifdef SDCARD
  #include <SD.h>

  void saveToCSV(const String deviceType, const String deviceName, const String deviceAddress) {
    File dataFile = SD.open("scanned_devices.csv", FILE_WRITE);

    if (dataFile) {
      dataFile.print(deviceType);
      dataFile.print(",");
      dataFile.print(deviceName);
      dataFile.print(",");
      dataFile.println(deviceAddress);
      dataFile.close();
      Serial.println("Written to CSV file");
    }
    else {Serial.println("Error opening data file");}
  }
#endif

void setup() {
  Serial.begin(9600);

  delay(3000);

  Serial.print("Starting BLE...");
  if (!BLE.begin()) {Serial.println(" failed!\n"); while (1);}
  else{Serial.println(" success!\n");}

  delay(500);

  #ifdef SDCARD
    Serial.print("Initializing SD card...");
    if (!SD.begin(4)) {Serial.println(" failed!\n"); while (1);}
    else{Serial.println(" success!\n");}

    delay(500);

    /*File file = SD.open("scanned_devices.csv", FILE_WRITE);
    if (file) {file.println("Type,Name,Address"); file.close();}
    else {Serial.println("Error opening data file");}*/
  #endif
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
      saveToCSV("peripheral", peripheral.localName(), peripheral.address());
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
      saveToCSV("central", central.localName(), central.address());
    #endif
  }

  delay(1000);
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}