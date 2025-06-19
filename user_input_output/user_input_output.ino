String userInput = "";      // To store incoming characters
bool running = true;        // Flag to keep the program running

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Serial Input Program");
  Serial.println("Type something and press Enter. Type 'exit' to stop.");
}

void loop() {
  if (running && Serial.available() > 0) {
    char receivedChar = Serial.read();

    // Handle newline or carriage return
    if (receivedChar == '\n' || receivedChar == '\r') {
      if (userInput.length() > 0) {
        // Check if input is "exit"
        if (userInput.equalsIgnoreCase("exit")) {
          Serial.println("Program stopped. Reset ESP32 to restart.");
          running = false;  // Stop processing input
        } else {
          Serial.print("You typed: ");
          Serial.println(userInput);
        }
        userInput = ""; // Clear for next input
      }
    } else {
      userInput += receivedChar;
    }
  }

  // Optional: Blink onboard LED slowly when stopped
  // if (!running) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  //   delay(300);
  //   digitalWrite(LED_BUILTIN, LOW);
  //   delay(300);
  // }
}