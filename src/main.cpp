#include <Arduino.h>

#include <ArduinoBLE.h>
#include <LowPower.h>
#include <SD.h>

File dataFile;

void saveToCSV(const String deviceType, const String deviceName, const String deviceAddress) {
  dataFile = SD.open("scanned_devices.csv", FILE_WRITE);

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

void setup() {
  Serial.begin(9600);

  Serial.println("Starting BLE...");
  if (!BLE.begin()) {Serial.println("Starting BLE failed!"); while (1);}

  Serial.println("Initializing SD card...");
  if (!SD.begin(4)) {Serial.println("SD card initialization failed!"); while (1);}

  dataFile = SD.open("scanned_devices.csv", FILE_WRITE);
  if (dataFile) {dataFile.println("Type,Name,Address"); dataFile.close();}
  else {Serial.println("Error opening data file");}
}

void loop() {
  Serial.println("Scanning for devices...");
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

    saveToCSV("peripheral", peripheral.localName(), peripheral.address());
  }

  if (central) {
    Serial.println("Fount central device");
    Serial.print(" > name: ");
    Serial.println(central.localName());
    Serial.print(" > address: ");
    Serial.println(central.address());
    Serial.println();

    saveToCSV("central", central.localName(), central.address());
  }

  delay(10000);
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}