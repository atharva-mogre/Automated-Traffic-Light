/*
 * Automated Traffic Light Signal Detector
 * Using HC-SR04 Ultrasonic Sensor and Arduino Uno
 * 
 * Logic: Near (<=15cm) = GREEN, Far (>15cm) = RED
 * YELLOW = Transition phase between states
 */

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int redLED = 7;
const int yellowLED = 6;
const int greenLED = 5;

// Distance threshold (in cm)
const int DISTANCE_THRESHOLD = 15;

// Variables
long duration;
int distance;
String currentState = "RED";
String previousState = "RED";
unsigned long yellowStartTime = 0;
const int YELLOW_DURATION = 1000;  // Yellow light duration in ms

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize HC-SR04 pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize LED pins
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Start with red light (no object)
  setTrafficLight("RED");
  
  Serial.println("Traffic Light System Initialized");
  Serial.println("Monitoring traffic...");
}

void loop() {
  // Measure distance
  distance = getDistance();
  
  // Determine target state based on distance
  // Near (<=15cm) = GREEN (vehicle present)
  // Far (>15cm) = RED (no vehicle)
  String targetState;
  
  if (distance <= DISTANCE_THRESHOLD) {
    targetState = "GREEN";
  } else {
    targetState = "RED";
  }
  
  // Handle state transitions with YELLOW as intermediate phase
  if (targetState != currentState && currentState != "YELLOW") {
    // Start transition to new state via YELLOW
    previousState = currentState;
    currentState = "YELLOW";
    yellowStartTime = millis();
    setTrafficLight("YELLOW");
  } else if (currentState == "YELLOW") {
    // Check if yellow phase is complete
    if (millis() - yellowStartTime >= YELLOW_DURATION) {
      currentState = targetState;
      setTrafficLight(currentState);
    }
  }
  
  // Send data to serial port in JSON format
  Serial.print("{\"distance\":");
  Serial.print(distance);
  Serial.print(",\"state\":\"");
  Serial.print(currentState);
  Serial.println("\"}");
  
  // Small delay before next measurement
  delay(200);
}

// Measure distance using HC-SR04 sensor
int getDistance() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo pin and calculate duration
  duration = pulseIn(echoPin, HIGH, 30000);
  
  // Calculate distance
  int dist = duration * 0.0343 / 2;
  
  // Return max value if no echo received
  if (dist == 0) {
    return 400;
  }
  
  return dist;
}

// Set traffic light LEDs based on state
void setTrafficLight(String state) {
  // Turn off all lights first
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  
  // Turn on appropriate light
  if (state == "RED") {
    digitalWrite(redLED, HIGH);
  } else if (state == "YELLOW") {
    digitalWrite(yellowLED, HIGH);
  } else if (state == "GREEN") {
    digitalWrite(greenLED, HIGH);
  }
}
