// Constants
const int TRIG_PIN = 2;  // Ultrasonic sensor trigger pin
const int ECHO_PIN = 3;  // Ultrasonic sensor echo pin
const int MAX_DISTANCE = 200;  // Maximum distance for ultrasonic sensor in centimeters

// Motor pins
const int LEFT_MOTOR_PIN1 = 4;
const int LEFT_MOTOR_PIN2 = 5;
const int RIGHT_MOTOR_PIN1 = 6;
const int RIGHT_MOTOR_PIN2 = 7;

// Motor speed
const int MOTOR_SPEED = 150;

// Function to move the robot forward
void moveForward() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// Function to turn the robot left
void turnLeft() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// Function to turn the robot right
void turnRight() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
}

// Function to stop the robot
void stop() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);
  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

// Function to calculate distance using the ultrasonic sensor
int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Function to check if there is an obstacle in front of the robot
bool obstacleDetected() {
  int distance = getDistance();
  if (distance > 0 && distance <= 20) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  if (obstacleDetected()) {
    // If an obstacle is detected, turn right
    turnRight();
    delay(1000);
  } else {
    // If no obstacle is detected, move forward
    moveForward();
  }
}
