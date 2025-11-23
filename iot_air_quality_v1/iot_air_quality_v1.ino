#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  Wire.begin(21,22);   // SDA, SCL
  delay(200);

  if(!display.begin(0x3C, true)) {   // hoặc 0x3D tùy I2C Scanner
    Serial.println(F("❌ OLED SH1106 not found"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.println("OLED SH1106 OK!");
  display.display();
  delay(1000);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("PM2.5: ");
  display.println(52.8);
  display.print("PM10 : ");
  display.println(76.3);
  display.display();
  delay(2000);
}