// Project: Remote Stepper Motor Controller System
// Author: AbdullahRFA

#include <IRremote.hpp>
#include <AccelStepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// === OLED DISPLAY SETUP ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// === IR Remote ===
#define IR_RECEIVER_PIN 4
#define LED_FEEDBACK_PIN 2
int state = 1;

// === IR Button Command Values (Decimal) ===
int forward = 17;
int backword = 16;
int speed_increase = 19;
int speed_decrease = 18;
int stopMotor = 11;
int idlePosition = 28;

// === Stepper Motor Pins ===
#define MOTOR_PIN1 14
#define MOTOR_PIN2 12
#define MOTOR_PIN3 13
#define MOTOR_PIN4 15
AccelStepper stepper(AccelStepper::HALF4WIRE, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

// === Variables ===
int speed = 300;
bool motorActive = false;
bool moveForward = false;
bool moveBackward = false;
bool previousDirectionForward = true;
bool idleToggleState = false;

int stepRequest = 0; // New: Store number key pressed for cycle request
bool cycleAwaitDirection = false;

// EEPROM Addresses
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