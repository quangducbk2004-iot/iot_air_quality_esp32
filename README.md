# 🌫️ Air Quality Monitoring System (ESP32-Based)

An embedded air quality monitoring system designed to measure and log environmental parameters including **PM2.5, PM10, temperature, humidity, and atmospheric pressure** in real-time.

Built on **ESP32**, the system integrates multiple sensors using standard communication protocols (I2C, UART, SPI) and implements reliable data acquisition, validation, and storage mechanisms.

---

## 📌 Features

- Real-time monitoring of:
  - PM2.5 / PM10 (SDS011 – UART)
  - Temperature / Humidity / Pressure (BME280 – I2C)
- OLED display for local visualization
- SD card logging via SPI
- Periodic sampling (5-second interval)
- UART frame parsing with checksum validation
- Moving average filtering for noise reduction
- State-machine based firmware architecture
- Power consumption evaluation (~1–1.5W)

---
## 🔧 Hardware Components

| Component | Function |
|-----------|----------|
| ESP32 | Main microcontroller |
| BME280 | Temperature, Humidity, Pressure |
| SDS011 | Laser-based PM2.5 / PM10 sensor |
| OLED Display | Real-time local display |
| SD Card Module | Data storage via SPI |

---

## 🛠️ Firmware Design

The firmware follows a modular architecture:

- **Sensor Acquisition Layer**
- **Data Processing Layer**
- **Storage & Display Layer**

### Communication Protocols

- **I2C** – BME280 environmental sensor  
- **UART** – SDS011 PM sensor (binary frame parsing)  
- **SPI** – SD card logging  

### UART Frame Validation

The SDS011 data frame is validated using:

- Frame header verification
- Data length checking
- Checksum calculation
- Error rejection mechanism

---

## 📊 Signal Processing

- Moving average filtering applied to reduce measurement noise
- Statistical evaluation (mean, standard deviation)
- Environmental response validation under controlled test conditions

---

## 🧪 System Validation

Test scenarios included:

- Sealed chamber baseline test
- Humidity increase test
- Humidity reduction test
- Dust stimulation test
- Long-duration stability testing (4+ hours)

---

## ⚡ Power Analysis

Estimated total system consumption: **~1–1.5W**

Major power contributors:
- SDS011 laser module
- ESP32 active mode

---

## 📂 Project Structure
/src
├── main.ino
├── sensor_bme280.cpp
├── sensor_sds011.cpp
├── sd_logger.cpp
└── display_oled.cpp
/docs
├── system_design.pdf
└── test_results.pdf

---

## 🚀 How to Run

1. Connect hardware as described in the schematic.
2. Install required libraries:
   - Adafruit BME280
   - SDS011 library
   - SD library
3. Upload firmware to ESP32 via Arduino IDE or PlatformIO.
4. Open Serial Monitor to observe debug logs.

---

## 🎯 Learning Outcomes

- Multi-protocol embedded communication (I2C, UART, SPI)
- Binary frame parsing and checksum validation
- State-machine based firmware design
- Embedded data logging and validation
- Power consumption analysis
- System reliability testing

---

## 📄 License

This project is developed for academic and research purposes.
