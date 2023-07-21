#include <Servo.h>
#include <Adafruit_TCS3200.h>

// Pin definitions
#define TRIG_PIN 2
#define ECHO_PIN 3
#define LEFT_MOTOR_PIN1 4
#define LEFT_MOTOR_PIN2 5
#define RIGHT_MOTOR_PIN1 6
#define RIGHT_MOTOR_PIN2 7
#define S0_PIN 8
#define S1_PIN 9
#define S2_PIN 10
#define S3_PIN 11
#define OUT_PIN 12

// Constants
#define MAX_DISTANCE 300   // Maximum distance to measure (in cm)
#define ROBOT_WIDTH 10     // Width of the robot (in cm)
#define MOVE_DELAY 300     // Delay between movements (in milliseconds)
#define START_COLOR "RED"  // Color of the start point
#define END_COLOR "BLACK"  // Color of the end point

// Servo object for the ultrasonic sensor
Servo servo;

// Color sensor object
Adafruit_TCS3200 colorSensor = Adafruit_TCS3200(S2_PIN, S3_PIN, OUT_PIN);

// Function prototypes
int measureDistance();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMoving();
void solveMaze();
String detectColor();

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize motor pins as outputs
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);

  // Attach servo to the trig pin
  servo.attach(TRIG_PIN);

  // Calibrate servo
  servo.write(90);

  // Initialize color sensor
  colorSensor.begin();

  // Set color sensor frequency scaling to 20%
  colorSensor.setFrequencyScaling(Adafruit_TCS3200::FREQUENCY_20PCT);

  // Delay for sensor initialization
  delay(2000);
}

void loop() {
  solveMaze();
}

// Function to measure distance using the ultrasonic sensor
int measureDistance() {
  // Send ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance
  int distance = duration * 0.034 / 2;

  return distance;
}

// Function to move the robot forward
void moveForward() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  delay(MOVE_DELAY);
}

// Function to move the robot backward
void moveBackward() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
  delay(MOVE_DELAY);
}

// Function to turn the robot left
void turnLeft() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
  delay(MOVE_DELAY);
}

// Function to turn the robot right
void turnRight() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
  delay(MOVE_DELAY);
}

// Function to stop the robot
void stopMoving() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// Function to solve the maze using flood fill algorithm
void solveMaze() {
  String color = detectColor();

  if (color == START_COLOR) {
    Serial.println("Start point detected!");
    // You can add your specific actions for the start point here
  } else if (color == END_COLOR) {
    Serial.println("End point detected!");
    stopMoving();
    while (true) {
      // Stay at the end point indefinitely
    }
  }

  int distance = measureDistance();

  // Check if the robot has reached the destination
  if (distance <= ROBOT_WIDTH) {
    stopMoving();
    Serial.println("Reached destination!");
    return;
  }

  // Check possible directions
  int leftDistance, frontDistance, rightDistance;

  // Turn left and measure distance
  turnLeft();
  leftDistance = measureDistance();

  // Turn right and measure distance
  turnRight();
  turnRight();
  rightDistance = measureDistance();

  // Turn back to the front
  turnLeft();

  // Measure distance in the front
  frontDistance = measureDistance();

  // Make a decision based on the distances
  if (leftDistance > MAX_DISTANCE && frontDistance > MAX_DISTANCE && rightDistance > MAX_DISTANCE) {
    // Dead end, turn back
    turnLeft();
    turnLeft();
  } else if (leftDistance > MAX_DISTANCE && frontDistance > MAX_DISTANCE) {
    // No left and front paths, turn right
    turnRight();
  } else if (leftDistance > MAX_DISTANCE && rightDistance > MAX_DISTANCE) {
    // No left and right paths, move forward
    moveForward();
  } else if (frontDistance > MAX_DISTANCE && rightDistance > MAX_DISTANCE) {
    // No front and right paths, turn left
    turnLeft();
  } else if (leftDistance > MAX_DISTANCE) {
    // Only left path available, turn left
    turnLeft();
  } else if (frontDistance > MAX_DISTANCE) {
    // Only front path available, move forward
    moveForward();
  } else if (rightDistance > MAX_DISTANCE) {
    // Only right path available, turn right
    turnRight();
  } else {
    // All paths available, choose the one with the most distance
    if (leftDistance >= frontDistance && leftDistance >= rightDistance) {
      turnLeft();
    } else if (frontDistance >= leftDistance && frontDistance >= rightDistance) {
      moveForward();
    } else {
      turnRight();
    }
  }

  // Recursive call to continue solving the maze
  solveMaze();
}

// Function to detect color using the color sensor
String detectColor() {
  // Set sensor to measure red
  colorSensor.setFrequencyScaling(Adafruit_TCS3200::FREQUENCY_20PCT);
  delay(100);

  // Read red value
  uint16_t red = colorSensor.readRed();

  // Set sensor to measure green
  colorSensor.setFrequencyScaling(Adafruit_TCS3200::FREQUENCY_20PCT);
  delay(100);

  // Read green value
  uint16_t green = colorSensor.readGreen();

  // Set sensor to measure blue
  colorSensor.setFrequencyScaling(Adafruit_TCS3200::FREQUENCY_20PCT);
  delay(100);

  // Read blue value
  uint16_t blue = colorSensor.readBlue();

  // Compare color values to determine the detected color
  if (red > 150 && green < 100 && blue < 100) {
    return "RED";
  } else if (red < 100 && green < 100 && blue < 100) {
    return "BLACK";
  } else {
    return "UNKNOWN";
  }
}
