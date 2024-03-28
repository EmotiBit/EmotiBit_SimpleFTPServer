#include <Arduino.h>
#include <FileTransferManager.h>

const char* ssid = "NETWORK_NAME";
const char* password = "NETWORK_PASS";
FileTransferManager fileTransferManager;

// Definitions taken from EmotiBitVersionController.cpp
// ToDo: move these definitions out of VersionController into a smaller header that can be included in examples
#ifdef ARDUINO_FEATHER_ESP32
const int EMOTIBIT_VDD_ENABLE_PIN = 32; // Enable emotibit power; ESP32 boards
const int EMOTIBIT_SD_CS_PIN = 4; // Chip select pin on EmotiBit for ESP32 
#elif defined ADAFRUIT_FEATHER_M0
const int EMOTIBIT_VDD_ENABLE_PIN = 6; // Enable emotibit power; ESP32 boards
const int EMOTIBIT_SD_CS_PIN = 19; // Chip select pin on EmotiBit for ESP32 
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  
  // Enable power on EmotiBit board
  pinMode(EMOTIBIT_VDD_ENABLE_PIN, OUTPUT);
  // Choose based on EmotiBit version you are using
  //digitalWrite(EMOTIBIT_VDD_ENABLE_PIN, LOW); // for EmotiBit V2
  digitalWrite(EMOTIBIT_VDD_ENABLE_PIN, HIGH); // for EmotiBit V3+
  delay(1000);

  if (SD.begin(EMOTIBIT_SD_CS_PIN)) {
      Serial.println("SD opened!");
  }
  else
  {
    Serial.println("SD not initialized. Please check battery and SD card on EmotiBit");
    while(1);
  }
  Serial.println("Setting Protocol");
  fileTransferManager.setProtocol(FileTransferManager::Protocol::FTP);
  Serial.println("Setting Auth");
  fileTransferManager.setFtpAuth("ftp", "ftp");
  Serial.println("Setting Mode");
  fileTransferManager.setMode(FileTransferManager::Mode::FILE_TRANSFER);
  fileTransferManager.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  fileTransferManager.handleTransfer();
}
