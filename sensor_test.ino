/*
 * HC-SR04 Sensor Diagnostic Test
 * Tests different pin configurations to find the correct wiring
 */

// Try these pin combinations if sensor doesn't work
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.println("HC-SR04 Sensor Test");
  Serial.println("===================");
  Serial.println("Pins: Trig=9, Echo=10");
  Serial.println("");
  delay(2000);
}

void loop() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo
  duration = pulseIn(echoPin, HIGH, 30000);
  
  // Calculate distance
  distance = duration * 0.0343 / 2;
  
  // Display detailed debug info
  Serial.print("Duration: ");
  Serial.print(duration);
  Serial.print(" μs  |  Distance: ");
  
  if (duration == 0) {
    Serial.println("NO ECHO - Check wiring!");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  
  delay(500);
}
