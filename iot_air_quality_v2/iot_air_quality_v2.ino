#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "SdsDustSensor.h"

// ---------------- OLED SH1106 ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- SDS011 ----------------
HardwareSerial mySerial(2);  // UART2 trên ESP32
SdsDustSensor sds(mySerial);

void setup() {
  Serial.begin(9600);

  // Khởi động I2C cho OLED
  Wire.begin(21, 22);   // SDA, SCL
  delay(200);

  // Khởi tạo OLED SH1106
  if(!display.begin(0x3C, true)) {   // hoặc 0x3D nếu scanner báo khác
    Serial.println(F("❌ Không tìm thấy OLED SH1106!"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("ESP32 + SDS011 + SH1106");
  display.display();
  delay(1000);

  // Khởi tạo SDS011
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX
  sds.begin();
  sds.wakeup();
  sds.setActiveReportingMode();
  sds.setContinuousWorkingPeriod();
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

    // --- Hiển thị lên OLED ---
    display.setCursor(0, 20);
    display.print("PM2.5: ");
    display.print(pm.pm25);
    display.println(" ug/m3");

    display.setCursor(0, 35);
    display.print("PM10 : ");
    display.print(pm.pm10);
    display.println(" ug/m3");

    display.setCursor(0, 50);
    if (pm.pm25 < 25)
      display.print(" Air: Good");
    else if (pm.pm25 < 75)
      display.print(" Air: Moderate");
    else
      display.print(" Air: Bad");

  } else {
    Serial.println(pm.statusToString());
    display.setCursor(0, 25);
    display.println(" Sensor read error!");
  }

  display.display();
  delay(2000);
}
