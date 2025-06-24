# 🔁 28BYJ-48 Stepper Motor with ULN2003 Driver

This project demonstrates how to control a **28BYJ-48 stepper motor** using the **ULN2003 driver board**. This combo is perfect for low-cost, low-power stepper applications like robotics, CNCs, camera sliders, or any precision motion system.

---

## 📦 Components Used

| Component           | Description                                 |
|---------------------|---------------------------------------------|
| 🌀 28BYJ-48 Stepper  | 5V unipolar stepper motor (64 steps/rev)   |
| 🔲 ULN2003 Driver    | Darlington array driver for stepper motor  |
| ⚡ Power Supply      | 5V external supply (recommended)            |
| 🧠 Microcontroller   | ESP32 / Arduino Uno / Nano / Mega etc.     |
| 🧰 Jumper Wires      | Male-to-female/female-to-female as needed  |

---

## 🧠 28BYJ-48 Motor Specs

- Operating Voltage: **5V DC**
- Step Angle: **5.625°/64** (gear reduction)
- Gear Ratio: **1/64**
- 4096 steps/revolution (approx.)
- Max current: ~240mA (per coil)
- Internal resistance: ~50 ohms/coil

---

## 🔌 ULN2003 Driver Board

- 4 Input pins (IN1–IN4)
- 4 LED indicators (for coil state)
- Stepper motor JST connector
- Power input: 5V
- Controlled via digital I/O pins from a microcontroller

---

## 🖥️ Wiring with Arduino / ESP32

| ULN2003 IN | Connect To (MCU Pin) |
|------------|----------------------|
| IN1        | GPIO 14              |
| IN2        | GPIO 12              |
| IN3        | GPIO 13              |
| IN4        | GPIO 15              |
| VCC        | 5V                   |
| GND        | GND                  |
| Motor      | JST connector        |

💡 *Note: Use external 5V power source if motor stalls or resets microcontroller.*

---

## 🔃 Steps Per Revolution Calculation

- Motor step angle: **5.625°**
- Gear ratio: **1/64**

Use `stepper.move(4096)` to make one full revolution.

---
## 🧑‍💻 Author

Developed by **Abdullah Nazmus-Sakib**  
Feel free to fork, improve, and share!

## 📄 Example Code Snippet (Arduino + AccelStepper)

```cpp
#include <AccelStepper.h>

// Define motor control pins
#define MOTOR_PIN1 14
#define MOTOR_PIN2 12
#define MOTOR_PIN3 13
#define MOTOR_PIN4 15

// Set up the AccelStepper for 4-wire stepper in half-step mode
AccelStepper stepper(AccelStepper::HALF4WIRE, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

void setup() {
  Serial.begin(115200);

  // Motor settings
  stepper.setMaxSpeed(1000);      // Adjust as needed
  stepper.setAcceleration(500);   // Smooth motion
}

void loop() {
  // Rotate one full revolution CW (2048 half-steps)
  Serial.println("Rotating Clockwise");
  stepper.moveTo(2048);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(1000);

  // Rotate one full revolution CCW
  Serial.println("Rotating Counter-Clockwise");
  stepper.moveTo(0); // Return to zero position
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(1000);
}