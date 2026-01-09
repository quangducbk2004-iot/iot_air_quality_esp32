#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_BME280.h>

#include "SdsDustSensor.h"

/* ================= OLED SH1106 ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ================= SDS011 ================= */
HardwareSerial mySerial(2);
SdsDustSensor sds(mySerial);

/* ================= SHT31 ================= */
Adafruit_SHT31 sht31 = Adafruit_SHT31();

/* ================= BME280 ================= */
Adafruit_BME280 bme;

/* ================= SD Card ================= */
#define SD_CS 5
File dataFile;

/* ================= TIME ================= */
unsigned long lastSave = 0;
const unsigned long interval = 5000; // 5 giây

void setup() {
  Serial.begin(9600);

  /* ---------- I2C ---------- */
  Wire.begin(21, 22);

  /* ---------- OLED ---------- */
  delay(200);
  if (!display.begin(0x3C, true)) {
    Serial.println("OLED FAIL");
    while (1);
  }
  display.clearDisplay();
  display.display();

  /* ---------- SDS011 ---------- */
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  sds.begin();
  sds.wakeup();
  sds.setActiveReportingMode();
  sds.setContinuousWorkingPeriod();

  /* ---------- SHT31 ---------- */
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 FAIL");
  }

  /* ---------- BME280 ---------- */
  if (!bme.begin(0x76)) {
    Serial.println("BME280 FAIL");
  }

  /* ---------- SD Card ---------- */
  if (!SD.begin(SD_CS)) {
    Serial.println("SD FAIL");
    while (1);
  }

  Serial.println("SYSTEM READY");
}

void loop() {

  /* ---------- SDS011 ---------- */
  PmResult pm = sds.readPm();

  /* ---------- SHT31 ---------- */
  float shtTemp = sht31.readTemperature();
  float shtHumi = sht31.readHumidity();

  /* ---------- BME280 ---------- */
  float bmeTemp  = bme.readTemperature();
  float bmeHumi  = bme.readHumidity();
  float bmePress = bme.readPressure() / 100.0; // Pa → hPa

  /* ---------- OLED ---------- */
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 0);
  display.print("T SHT:");
  display.print(shtTemp, 1);
  display.print(" BME:");
  display.print(bmeTemp, 1);

  display.setCursor(0, 12);
  display.print("H SHT:");
  display.print(shtHumi, 1);
  display.print(" BME:");
  display.print(bmeHumi, 1);

  display.setCursor(0, 24);
  display.print("P BME:");
  display.print(bmePress, 1);
  display.print(" hPa");

  if (pm.isOk()) {
    display.setCursor(0, 40);
    display.print("PM2.5: ");
    display.print(pm.pm25);

    display.setCursor(0, 52);
    display.print("PM10 : ");
    display.print(pm.pm10);
  } else {
    display.setCursor(0, 40);
    display.print("SDS011 ERROR");
  }

  display.display();

  /* ---------- SERIAL MONITOR (GIỐNG OLED) ---------- */
  Serial.println();
  Serial.print("T SHT:");
  Serial.print(shtTemp, 1);
  Serial.print(" BME:");
  Serial.println(bmeTemp, 1);

  Serial.print("H SHT:");
  Serial.print(shtHumi, 1);
  Serial.print(" BME:");
  Serial.println(bmeHumi, 1);

  Serial.print("P BME:");
  Serial.print(bmePress, 1);
  Serial.println(" hPa");

  if (pm.isOk()) {
    Serial.print("PM2.5: ");
    Serial.println(pm.pm25);
    Serial.print("PM10 : ");
    Serial.println(pm.pm10);
  } else {
    Serial.println("SDS011 ERROR");
  }

  /* ---------- SD LOG ---------- */
  if (pm.isOk() && millis() - lastSave >= interval) {
    lastSave = millis();

    dataFile = SD.open("/dust_log.csv", FILE_APPEND);
    if (dataFile) {
      dataFile.print(millis() / 1000);
      dataFile.print(",");
      dataFile.print(pm.pm25);
      dataFile.print(",");
      dataFile.print(pm.pm10);
      dataFile.print(",");
      dataFile.print(shtTemp);
      dataFile.print(",");
      dataFile.print(shtHumi);
      dataFile.print(",");
      dataFile.print(bmeTemp);
      dataFile.print(",");
      dataFile.print(bmeHumi);
      dataFile.print(",");
      dataFile.println(bmePress);
      dataFile.close();
    }
  }

  delay(1000);
}