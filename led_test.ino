/*
 * LED Pin Diagnostic Test
 * This will help identify which physical LED is connected to which pin
 */

const int redLED = 5;
const int yellowLED = 6;
const int greenLED = 7;

void setup() {
  Serial.begin(9600);
  
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Turn off all LEDs
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  
  Serial.println("LED Test Starting...");
  Serial.println("Watch which LED lights up!");
  delay(2000);
}

void loop() {
  // Test Pin 5 (should be RED)
  Serial.println("\n=== Testing Pin 5 (RED) ===");
  digitalWrite(redLED, HIGH);
  delay(2000);
  digitalWrite(redLED, LOW);
  delay(1000);
  
  // Test Pin 6 (should be YELLOW)
  Serial.println("=== Testing Pin 6 (YELLOW) ===");
  digitalWrite(yellowLED, HIGH);
  delay(2000);
  digitalWrite(yellowLED, LOW);
  delay(1000);
  
  // Test Pin 7 (should be GREEN)
  Serial.println("=== Testing Pin 7 (GREEN) ===");
  digitalWrite(greenLED, HIGH);
  delay(2000);
  digitalWrite(greenLED, LOW);
  delay(1000);
  
  Serial.println("\n--- Test Complete. Repeating... ---\n");
  delay(1000);
}
