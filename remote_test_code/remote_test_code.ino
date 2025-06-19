#include <IRremote.h>

#define IR_RECEIVE_PIN 2  // Connect OUT pin of VS1838 here

void setup() {
  Serial.begin(9600);
  delay(2000);  // Give time to open Serial Monitor
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR Receiver started on ESP32");
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.print("IR Code: 0x");
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume(); // Ready to receive next code
  }
}