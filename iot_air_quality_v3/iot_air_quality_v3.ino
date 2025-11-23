#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "SdsDustSensor.h"
#include <SD.h>
#include <SPI.h>

// ---------- OLED SH1106 ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- SDS011 ----------
HardwareSerial mySerial(2);
SdsDustSensor sds(mySerial);

// ---------- SD Card ----------
#define SD_CS 5   // chân CS của module SD (GPIO5)

File dataFile;

// ---------- Biến thời gian ----------
unsigned long lastSave = 0;
const unsigned long interval = 5000; // 5s ghi 1 lần

void setup() {
  Serial.begin(9600);

  // --- OLED ---
  Wire.begin(21, 22);
  delay(200);
  if(!display.begin(0x3C, true)) {
    Serial.println(F("❌ Không tìm thấy OLED SH1106!"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("ESP32 + SDS011 + SD");
  display.display();
  delay(1000);

  // --- SDS011 ---
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  sds.begin();
  sds.wakeup();
  sds.setActiveReportingMode();
  sds.setContinuousWorkingPeriod();

  // --- SD Card ---
  Serial.println("💾 Đang khởi tạo thẻ SD...");
  if(!SD.begin(SD_CS)) {
    Serial.println("❌ Lỗi SD Card!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("SD init failed!");
    display.display();
    while(1);
  }
  Serial.println("✅ SD Card ready!");
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("SD Ready!");
  display.display();
  delay(1000);
}

void loop() {
  PmResult pm = sds.readPm();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("Air Quality Monitor");
  display.drawLine(0, 10, 127, 10, SH110X_WHITE);

  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(" µg/m³, PM10 = ");
    Serial.println(pm.pm10);

    // --- Hiển thị OLED ---
    display.setCursor(0, 20);
    display.print("PM2.5: ");
    display.print(pm.pm25);
    display.println(" ug/m3");

    display.setCursor(0, 35);
    display.print("PM10 : ");
    display.print(pm.pm10);
    display.println(" ug/m3");

    display.setCursor(0, 50);
    if (pm.pm25 < 25) display.print("Air: Good");
    else if (pm.pm25 < 75) display.print("Air: Moderate");
    else display.print("Air: Bad");

    display.display();

    // --- Ghi dữ liệu vào SD mỗi 5 giây ---
    if (millis() - lastSave >= interval) {
      lastSave = millis();

      dataFile = SD.open("/dust_log.csv", FILE_APPEND);
      if (dataFile) {
        dataFile.print(millis()/1000);
        dataFile.print(",");
        dataFile.print(pm.pm25);
        dataFile.print(",");
        dataFile.println(pm.pm10);
        dataFile.close();
        Serial.println("💾 Ghi SD thành công!");
      } else {
        Serial.println("❌ Không mở được file SD!");
      }
    }

  } else {
    Serial.println(pm.statusToString());
    display.setCursor(0, 25);
    display.println("⚠️ Sensor read error!");
    display.display();
  }

  delay(1000);
}
