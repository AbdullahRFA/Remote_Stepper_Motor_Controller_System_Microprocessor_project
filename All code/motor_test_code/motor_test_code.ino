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