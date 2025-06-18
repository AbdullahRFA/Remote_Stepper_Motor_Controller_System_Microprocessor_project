// Built-in LED pin (GPIO 2 for most ESP32 boards)
const int ledPin = 2;

void setup() {
  // Start Serial Monitor at 115200 baud
  Serial.begin(115200);
  
  // Configure LED pin as output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Turn the LED ON
  digitalWrite(ledPin, HIGH);
  Serial.println("LED is ON");
  delay(100);  // Wait for 1 second

  // Turn the LED OFF
  digitalWrite(ledPin, LOW);
  Serial.println("LED is OFF");
  delay(100);  // Wait for 1 second
}