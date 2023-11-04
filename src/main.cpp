#include <ArduinoBLE.h>
#include <LowPower.h>
#include <SD.h>

const unsigned long SCAN_INTERVAL = 10000; // 10 seconds in milliseconds
File dataFile;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  
  Serial.println("Initializing SD card...");
  
  if (!SD.begin(4)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  
  dataFile = SD.open("scanned_devices.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Device Type,Device Name,Device Address");
    dataFile.close();
  } else {
    Serial.println("Error opening data file");
  }
  
  Serial.println("Scanning for devices...");
  BLE.scan();
}

void loop() {
  BLEDevice peripheral = BLE.available();
  
  if (peripheral) {
    Serial.print("Peripheral Device Name: ");
    Serial.println(peripheral.localName());
    Serial.print("Peripheral Device Address: ");
    Serial.println(peripheral.address());
    Serial.println();
    
    saveToCSV("Peripheral", peripheral.localName(), peripheral.address());
  }
  
  BLECentral central = BLE.central();
  
  if (central) {
    Serial.print("Central Device Name: ");
    Serial.println(central.localName());
    Serial.print("Central Device Address: ");
    Serial.println(central.address());
    Serial.println();
    
    saveToCSV("Central", central.localName(), central.address());
  }
  
  // Enter low-power sleep mode
  LowPower.deepSleep(SCAN_INTERVAL * 1000); // Sleep for SCAN_INTERVAL milliseconds
}

void saveToCSV(const char* deviceType, const char* deviceName, const char* deviceAddress) {
  dataFile = SD.open("scanned_devices.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(deviceType);
    dataFile.print(",");
    dataFile.print(deviceName);
    dataFile.print(",");
    dataFile.println(deviceAddress);
    dataFile.close();
  } else {
    Serial.println("Error opening data file");
  }
}
