/*
 * Automated Traffic Light Signal Detector - DEBUG VERSION
 * Using HC-SR04 Ultrasonic Sensor and Arduino Uno
 * 
 * Logic: Near (<=15cm) = GREEN, Far (>15cm) = RED
 * YELLOW = Transition phase between states
 * 
 * This version includes extra debugging to help diagnose sensor issues
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
int lastDistance = -1;
int sameDistanceCount = 0;
String currentState = "RED";
String previousState = "RED";
unsigned long yellowStartTime = 0;
const int YELLOW_DURATION = 1000;  // Yellow light duration in ms
unsigned long lastMeasurementTime = 0;

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
  
  Serial.println("Traffic Light System Initialized - DEBUG MODE");
  Serial.println("Monitoring traffic...");
  Serial.println("If distance stays constant, check sensor connections!");
}

void loop() {
  // Measure distance with enhanced error checking
  distance = getDistance();
  
  // Debug: Check if distance is stuck
  if (distance == lastDistance) {
    sameDistanceCount++;
    if (sameDistanceCount >= 10) {
      Serial.println("WARNING: Distance stuck at same value! Check sensor!");
      sameDistanceCount = 0; // Reset counter
    }
  } else {
    sameDistanceCount = 0;
  }
  lastDistance = distance;
  
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
  Serial.print("\",\"duration\":");
  Serial.print(duration);
  Serial.println("\"}");
  
  // Small delay before next measurement
  delay(200);
}

// Measure distance using HC-SR04 sensor with enhanced error detection
int getDistance() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Send 10μs pulse to trigger pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read echo pin and calculate duration with timeout
  // Timeout of 30000μs = ~5.1m max distance
  duration = pulseIn(echoPin, HIGH, 30000);
  
  // Check if we got a valid reading
  if (duration == 0) {
    // No echo received - sensor might be disconnected or object too far
    Serial.println("DEBUG: No echo received from sensor!");
    return 400; // Return max distance
  }
  
  // Calculate distance (speed of sound = 343 m/s = 0.0343 cm/μs)
  int dist = duration * 0.0343 / 2;
  
  // Validate distance is in reasonable range
  if (dist < 2 || dist > 400) {
    Serial.print("DEBUG: Invalid distance reading: ");
    Serial.println(dist);
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
