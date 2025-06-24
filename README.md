# 🚀 Remote Stepper Motor Controller System

A complete system to control a **28BYJ-48 Stepper Motor** using an **IR remote**, with live OLED display updates and EEPROM support for speed/direction memory. Developed using an **ESP32**, this system demonstrates real-time control, user interaction, and smooth progress feedback on a display.

---

## 📦 Project Overview

This system allows users to control the movement of a stepper motor through an IR remote. The OLED display gives visual feedback such as current speed, direction, running state, and a real-time **progress bar** while moving. EEPROM is used to remember the last speed and direction even after power loss.

---

## 🛠️ Hardware Used

| Component               | Details                          |
|------------------------|----------------------------------|
| **ESP32**              | Microcontroller                  |
| **28BYJ-48 Stepper**   | Unipolar Stepper Motor           |
| **ULN2003 Driver**     | Stepper Driver Board             |
| **IR Remote**          | Generic IR Remote (NEC Protocol) |
| **IR Receiver**        | TSOP1838 or similar              |
| **OLED Display**       | 128x64 SSD1306 I2C OLED          |
| **Jumper Wires**       | Male-Female                      |
| **Power Supply**       | 5V (for motor), 3.3V for ESP32   |

---

## 🧠 Features

- 📡 IR Remote Control (Direction, Speed, Start/Stop, Idle)
- 💾 EEPROM Support (Speed & Direction saved)
- 🖥️ OLED Feedback (Live status + progress bar)
- 🔢 One-Tap Cycle Execution (0–9 mapped to 1–10 cycles)
- 🔄 Smooth movement via AccelStepper
- 🔌 Idle toggle to pause/resume motion
- 🚨 Unknown command detection

---

## 🎮 IR Remote Button Mapping

| Button     | Function                |
|------------|-------------------------|
| `0-9`      | Select number of cycles (1–10) |
| `FORWARD`  | Start forward movement or run cycle forward |
| `BACKWARD` | Start backward movement or run cycle backward |
| `+`        | Increase speed (+100)   |
| `-`        | Decrease speed (–100)   |
| `STOP`     | Stop motor immediately  |
| `IDLE`     | Pause/Resume motor      |

---

## 📺 OLED Display States

- **Status Screen**:  
  Shows current speed, direction, and state.

- **Command Feedback**:  
  `Cmd: Move 5 Cycle Forward`

- **Progress Bar** (for cycle execution):  
  Animated horizontal bar showing movement progress.

- **Final Status**:  
  `Moved 3 Cycle Forward`

---

## 💾 EEPROM Usage

| Address  | Stored Value         |
|----------|----------------------|
| `0`      | Speed (int)          |
| `4`      | Last Direction (bool)|

---

## 🚀 How to Run

1. Connect all hardware components as per pin mapping in code.
2. Upload the Arduino sketch to the ESP32 board.
3. Power up the ESP32 and motor with appropriate voltage.
4. Use the IR remote to:
   - 🔢 **Select cycles** (`0–9` → 1–10 cycles)
   - ➡️ **Move forward** or ⬅️ **backward**
   - 🔼 **Increase speed** / 🔽 **decrease speed**
   - ⏹️ **Stop motor**
   - ⏸️ **Pause/Resume movement**

---
## 📌 Future Improvements

- 🌐 Add web-based control via Wi-Fi (ESP32 Web Server)
- 🏠 Auto-home calibration using limit switches
- ⛔ Limit switch safety support
- 📐 Support precise angular movement (e.g., 90°, 180°, etc.)

---
## 📸 Screenshots / Demo

> *(Insert images or GIFs of OLED display, wiring setup, and live motor demo here)*

---
## 👨‍💻 Developed By

**Team JU CSE**

- 🧑‍💻 Abdullah (383)  
- 🧑‍💻 Khaled (391)  
- 🧑‍💻 Shanto (379)  
- 🧑‍💻 Tawhid (395)  

> *“Control in your hands – powered by code.”*

---
## 📜 License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for more information.

---

## 🧾 Code Snippets

### Header file
```cpp
#include <IRremote.hpp>
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
```
### OLED DISPLAY SETUP 
```cpp
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
```
### IR Remote
```cpp
#define IR_RECEIVER_PIN 4
#define LED_FEEDBACK_PIN 2
int state = 1;
```
### IR Button Command Values (Decimal)
```cpp
int forward = 17;
int backword = 16;
int speed_increase = 19;
int speed_decrease = 18;
int stopMotor = 11;
int idlePosition = 28;
```
### Stepper Motor Pins
```cpp
#define MOTOR_PIN1 14
#define MOTOR_PIN2 12
#define MOTOR_PIN3 13
#define MOTOR_PIN4 15
AccelStepper stepper(AccelStepper::HALF4WIRE, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);
```
### Variables
```cpp
int speed = 300;
bool motorActive = false;
bool moveForward = false;
bool moveBackward = false;
bool previousDirectionForward = true;
bool idleToggleState = false;

int stepRequest = 0; // New: Store number key pressed for cycle request
bool cycleAwaitDirection = false;
```
### EEPROM Addresses
```cpp
#define EEPROM_SPEED_ADDR 0
#define EEPROM_DIR_ADDR 4

void saveToEEPROM() {
  EEPROM.writeInt(EEPROM_SPEED_ADDR, speed);
  EEPROM.writeBool(EEPROM_DIR_ADDR, previousDirectionForward);
  EEPROM.commit();
}

void loadFromEEPROM() {
  speed = EEPROM.readInt(EEPROM_SPEED_ADDR);
  previousDirectionForward = EEPROM.readBool(EEPROM_DIR_ADDR);
  if (speed < 100 || speed > 1000) speed = 300;
}
```
### Function for dispaly status
```cpp
void displayStatus(const char* action) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println(action);

  display.setCursor(0, 20);
  display.print("Speed: "); display.println(speed);

  display.setCursor(0, 30);
  display.print("Direction: ");
  display.println(previousDirectionForward ? "Forward" : "Backward");

  display.setCursor(0, 50);
  display.print("State: ");
  display.println(motorActive ? "Running" : "Stopped");
  display.display();
}
```
### Function for showing command feedback
```cpp
void showCommandFeedback(int cycles, bool dir) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Cmd: Move ");
  display.print(cycles);
  display.print(" Cycle ");
  display.println(dir ? "Forward" : "Backward");
  display.display();
  delay(1000);
}
```
### Move N Cycles 
```cpp
void moveNCycle(int n, bool dir) {
  stepper.setSpeed(dir ? abs(speed) : -abs(speed));
  int steps = 4096; // one cycle for 28BYJ-48
  stepper.move(steps * (dir ? n : -n));
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  // Display final movement summary
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Moved ");
  display.print(n);
  display.print(" Cycle ");
  display.println(dir ? "Forward" : "Backward");
  display.display();
  delay(1000);
}
```
### Function for showing project intro
```cpp
void showProjectIntro() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Remote Stepper Motor");
  display.println("Controller System");
  display.display();
  delay(1500);  // Show project title

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Presented by:");

  display.println("-------------");
  display.println("- Abdullah(383)");
  display.println("- Khaled(391)");
  display.println("- Shanto(379)");
  display.println("- Tawhid(395)");
  display.display();
  delay(2000);  // Show contributor names
}
```
### Setup
```cpp
void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  display.setTextColor(SSD1306_WHITE);
  showProjectIntro();

  EEPROM.begin(64);
  loadFromEEPROM();
  displayStatus("Stepper Ready");

  IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
  pinMode(LED_FEEDBACK_PIN, OUTPUT);

  stepper.setMaxSpeed(1000);
  stepper.setSpeed(speed);
  stepper.setAcceleration(500);
}
```
### Main loop
```cpp
void loop() {
  if (IrReceiver.decode()) {
    int command = IrReceiver.decodedIRData.command;
    Serial.print("IR Command: "); Serial.println(command);

    // showCommandFeedback(command,dir);

    if (command >= 0 && command <= 9) {
      stepRequest = command + 1; // Consider 0 as 1, 1 as 2, ..., 9 as 10
      cycleAwaitDirection = true;
      displayStatus("Press F/B for Cycle");
    } 
    else if (cycleAwaitDirection && (command == forward || command == backword)) {
        bool dir = (command == forward);
        showCommandFeedback(stepRequest, dir); // Show detailed feedback
        moveNCycle(stepRequest, dir); // Just move requested cycles once
        cycleAwaitDirection = false;
        stepRequest = 0;
    }else {
      switch (command) {
        case 17:
          moveForward = true;
          moveBackward = false;
          motorActive = true;
          previousDirectionForward = true;
          idleToggleState = false;
          displayStatus("Moving Forward");
          break;
        case 16:
          moveBackward = true;
          moveForward = false;
          motorActive = true;
          previousDirectionForward = false;
          idleToggleState = false;
          displayStatus("Moving Backward");
          break;
        case 19:
          speed += 100;
          if (speed > 1000) speed = 1000;
          stepper.setSpeed(speed);
          displayStatus("Speed Increased");
          break;
        case 18:
          speed -= 100;
          if (speed < 100) speed = 100;
          stepper.setSpeed(speed);
          displayStatus("Speed Decreased");
          break;
        case 11:
          moveForward = false;
          moveBackward = false;
          motorActive = false;
          idleToggleState = false;
          stepper.stop();
          displayStatus("Motor Stopped");
          break;
        case 28:
          if (!idleToggleState) {
            motorActive = false;
            idleToggleState = true;
            displayStatus("Paused (Idle)");
          } else {
            motorActive = true;
            if (previousDirectionForward) {
              moveForward = true;
              moveBackward = false;
            } else {
              moveBackward = true;
              moveForward = false;
            }
            idleToggleState = false;
            displayStatus("Resumed from Idle");
          }
          break;
        default:
          displayStatus("Unknown Cmd");
          break;
      }
    }

    saveToEEPROM();
    IrReceiver.resume();
    delay(300);
  }

  if (motorActive) {
    if (moveForward) {
      stepper.setSpeed(abs(speed));
      stepper.runSpeed();
    } else if (moveBackward) {
      stepper.setSpeed(-abs(speed));
      stepper.runSpeed();
    }
  }
}
```