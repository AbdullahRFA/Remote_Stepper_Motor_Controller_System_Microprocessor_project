# 📡 IR Remote & Receiver Module (TSOP1838 + Generic IR Remote)

This documentation covers how to use an **IR Receiver Module (like TSOP1838)** along with a **generic IR remote** to control microcontrollers like **ESP32 / Arduino**. This is ideal for building wireless control systems for motors, lights, or menu navigation.

---

## 🔧 Components Required

| Component         | Description                                  |
|------------------|----------------------------------------------|
| 📡 IR Receiver    | TSOP1838 / VS1838B IR sensor module (38kHz) |
| 🎛️ IR Remote       | Standard 21 or 44-key TV remote             |
| 🧠 Microcontroller | ESP32 / Arduino Uno / Nano / Mega           |
| 🔌 Jumper Wires    | Male-to-female/female-to-female             |

---

## ⚙️ How It Works

- **IR Remote** sends modulated light signals at **38 kHz**.
- **IR Receiver (TSOP1838)** detects these pulses and converts them to digital signals.
- With a library like `IRremote`, you can decode keypresses into unique command values.

---

## 🔌 Wiring

| IR Receiver Pin | Connect To        |
|------------------|------------------|
| **OUT (Signal)** | GPIO 4 (ESP32)   |
| **GND**          | GND              |
| **VCC**          | 3.3V or 5V       |

⚠️ Use 3.3V if using ESP32. 5V is safe for Arduino boards.

---

## 📦 Libraries Required

- **IRremote.h** by `Arduino-IRremote`  
  👉 Install via Library Manager or [GitHub](https://github.com/Arduino-IRremote/Arduino-IRremote)

---
## 🧑‍💻 Author

Developed by **Abdullah Nazmus-Sakib**  
Feel free to fork, improve, and share!

## 📄 Example Code Snippet

```cpp
#include <IRremote.hpp>

// button value in descimal
int off = 11;
int backword = 16;
int forward = 17;
int speed_decrease = 18;
int speed_increase = 19;
int idlePosition = 28;
int recall = 23;
int sys = 12;
int pp = 14;
int tv = 15;
int display = 22;
int pic = 29;
int sleep = 21;
int lock = 24;
int fav = 26;
int smode = 30;
int favscan = 27;


#define IR_RECEIVER_PIN      4                               // IR Receiver pin
#define LED_PIN              15  
#define LED_FEEDBACK_PIN 2 
int state=1;                             // ESP32 inboard pin

void setup()
{
    Serial.begin(115200);                                     // Start Serial Monitor
    IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);   // Start IR Receiver, Enable IRreceiver led
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, state);
    pinMode(LED_FEEDBACK_PIN, OUTPUT);
}

void loop()
{
    if (IrReceiver.decode())        // Check if there is a received code
    {
       digitalWrite(LED_FEEDBACK_PIN, HIGH);  // Turn LED on for feedback
        delay(40);                            // Visible duration
        digitalWrite(LED_FEEDBACK_PIN, LOW);


        Serial.print("Received Data: ");
        Serial.print(IrReceiver.decodedIRData.decodedRawData, DEC);  // Print Received code on Serial Monitor in Hexadecimal
        Serial.print("  -  Received Command: ");
        Serial.println(IrReceiver.decodedIRData.command, DEC);  // Print Received command on Serial Monitor in Hexadecimal

        if(IrReceiver.decodedIRData.command == 0x12){   
          state=!state;     // if you press a button with command number '0'
          digitalWrite(LED_PIN, state);                       // Turn off the led
        }
        if(IrReceiver.decodedIRData.command == 0){
          Serial.println("Your Press 1");
        }
        // if(IrReceiver.decodedIRData.command == idlePosition){
        //   Serial.println("Your are in idle position");
        // }

        delay(500);
        IrReceiver.resume();       // Enable receiving of the next value
    }
}