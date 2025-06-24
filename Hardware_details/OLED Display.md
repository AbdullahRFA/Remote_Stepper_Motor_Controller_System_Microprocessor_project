# 📺 0.96" OLED Display (SSD1306) Guide

The 0.96-inch OLED display based on the **SSD1306 controller** is a lightweight and low-power screen perfect for microcontroller projects like ESP32 and Arduino. This guide walks through its specifications, wiring, and how to use it in code.

---

## 🔧 Specifications

| Feature        | Details                        |
|----------------|-------------------------------|
| Screen Size    | 0.96 inches                    |
| Resolution     | 128x64 pixels                  |
| Interface      | I²C (also available in SPI)    |
| Controller     | SSD1306                        |
| Color          | Monochrome (usually White/Blue)|
| Operating Voltage | 3.3V – 5V                   |
| Library        | `Adafruit_SSD1306`, `Adafruit_GFX` |

---

## 🔌 Pinout (I2C Version)

| OLED Pin | Connect To         |
|----------|--------------------|
| **GND**  | GND                |
| **VCC**  | 3.3V or 5V         |
| **SCL**  | ESP32 GPIO 22 / Arduino A5 |
| **SDA**  | ESP32 GPIO 21 / Arduino A4 |

⚠️ *Use 3.3V for ESP32 to avoid damage (though many modules have regulators).*

---

## 📦 Libraries Required

Install the following libraries via Arduino Library Manager:

- `Adafruit SSD1306`
- `Adafruit GFX`

Or install via GitHub:
- [SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [GFX](https://github.com/adafruit/Adafruit-GFX-Library)

---
## 🧑‍💻 Author

Developed by **Abdullah Nazmus-Sakib**  
Feel free to fork, improve, and share!

## 📄 Sample Code (ESP32 / Arduino)

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width
#define SCREEN_HEIGHT 64 // OLED display height

// Create display object (I2C address 0x3C is common)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  delay(1000);
  display.clearDisplay(); // Clear the buffer

  // Display "Hello, ESP32!"
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Hello,");
  display.setCursor(0, 30);
  display.println("AbdullahRFA!");
  display.display(); // Show on screen
}

void loop() {
  // Nothing to do here
}