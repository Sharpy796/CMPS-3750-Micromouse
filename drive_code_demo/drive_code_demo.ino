// PINS
//the right motor will be controlled by the motor A pins on the motor driver
const int PWMA = 13;  //speed control pin on the motor driver for the right motor
const int AIN2 = 12;  //control pin 2 on the motor driver for the right motor
const int AIN1 = 11;  //control pin 1 on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int BIN1 = 10;  //control pin 1 on the motor driver for the left motor
const int BIN2 = 9;   //control pin 2 on the motor driver for the left motor
const int PWMB = 8;   //speed control pin on the motor driver for the left motor

const int TRIG_LEFT = 7;
const int ECHO_LEFT = 6;
float distanceLeft;

const int TRIG_FRONT = 5;
const int ECHO_FRONT = 4;
float distanceFront;

const int TRIG_RIGHT = 3;
const int ECHO_RIGHT = 2;
float distanceRight;

const int SWITCH = 1;

// WALLS
bool walls[] = {false, false, false};

// DIRECTIONS
enum Orientation {
  NORTH = 1,
  EAST,
  SOUTH,
  WEST
};
enum Orientation currentOrientation = NORTH;
enum Direction {
  FORWARD = 1,
  LEFT,
  BACKWARD,
  RIGHT,
};

// DISTANCES (Centimeters)
const int CELL_SIZE = 18; // centimeters
const float WALL_DISTANCE_AWAY = 6.0f;
const double SPEED_SOUND = .0343;

const int POWER = 255; // this number is 0 - 255, which represents 0% - 100% power
const int RPM = 3600; // dummy number, fine-tune this
const int RPS = RPM/60;
const double SPEED = (POWER/255)*RPS; // rotations per second
const double WHEEL_DIAMETER = 3; // centimeters
const double WHEEL_CIRCUMFERENCE = PI*WHEEL_DIAMETER; //2*pi*radius = pi*diameter (centimeters)
const double CMPS = SPEED*WHEEL_CIRCUMFERENCE; // centimeters per second
const double SECONDS_PER_CENTIMETER = 1/CMPS; // seconds per centimeter

// MAP
// TODO: Add map class
bool isAtFinish = false;

/********************************************************************************/
float updateSensor(int TRIG, int ECHO) {
  // Use sensors to recieve a signal
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  float duration = pulseIn(ECHO, HIGH);
  return (duration*SPEED_SOUND)/2;
}

void updateAllSensors() {
  distanceLeft = updateSensor(TRIG_LEFT, ECHO_LEFT);
  distanceFront = updateSensor(TRIG_FRONT, ECHO_FRONT);
  distanceRight = updateSensor(TRIG_RIGHT, ECHO_RIGHT);
}

bool detectWall(float distance) {
  bool isWall = distance <= WALL_DISTANCE_AWAY;
  if (isWall) {
    Serial.print("Wall detected ");
  } else {
    Serial.print("Wall NOT detected ");
  }
  Serial.print(distance);
  Serial.println(" cm. away.");
  return isWall;
}

bool detectWall(int TRIG, int ECHO) {
  return detectWall(updateSensor(TRIG, ECHO));
}

void detectAllWalls() {
  updateAllSensors();
  walls[0] = detectWall(distanceLeft);
  walls[1] = detectWall(distanceFront);
  walls[2] = detectWall(distanceRight);
}

void senseMap() { // TODO: Fill in wall-updating map functions
  updateAllSensors();
  detectAllWalls();
  switch (currentOrientation) {
    case 1: // North
      // update west, north, and east walls
      if (walls[1]) {} // Left wall
      if (walls[2]) {} // Front wall
      if (walls[3]) {} // Right wall
      Serial.println("Facing North");
      break;
    case 2: // East
      // update north, east, and south walls
      if (walls[1]) {} // Left wall
      if (walls[2]) {} // Front wall
      if (walls[3]) {} // Right wall
      Serial.println("Facing East");
      break;
    case 3: // South
      // update west, south, and east walls
      if (walls[1]) {} // Left wall
      if (walls[2]) {} // Front wall
      if (walls[3]) {} // Right wall
      Serial.println("Facing South");
      break;
    case 4: // West
      // update south, west, and east walls
      if (walls[1]) {} // Left wall
      if (walls[2]) {} // Front wall
      if (walls[3]) {} // Right wall
      Serial.println("Facing West");
      break;
    default: // What-
      Serial.println("Where The Heck Are You Oriented");
  }
}

void printDistance(int number, float distance) {
  Serial.print("Distance");
  Serial.print(number);
  Serial.print(": ");
  Serial.print(distance);
  if (detectWall(distance)) {
    Serial.println(" (Too close!)");
  } else {
    Serial.println("");
  }
}

void printAllDistances() {
  Serial.println("------------------ New Loop -------------------");
  printDistance(1, distanceLeft);
  printDistance(2, distanceFront);
  printDistance(3, distanceRight);
}

void rightMotor(int motorPower){
  if (motorPower > 0) { // if the motor should drive forward (positive speed)
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else if (motorPower < 0) { // if the motor should drive backward (negative speed)
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
  } else { // if the motor should stop
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }
  analogWrite(PWMA, abs(motorPower)); // now that the motor direction is set, drive it
}

void leftMotor(int motorPower) { // function for driving the left motor
  if (motorPower > 0) { // if the motor should drive forward (positive speed)
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else if (motorPower < 0) { // if the motor should drive backward (negative speed)
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
  } else { // if the motor should stop
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
  analogWrite(PWMB, abs(motorPower)); // now that the motor direction is set, drive it at the entered speed
}

void driveForward() {
  rightMotor(POWER);
  leftMotor(POWER);
}
void driveBackward() {
  rightMotor(-POWER);
  leftMotor(-POWER);
}
void turnLeft() {
  rightMotor(POWER);
  leftMotor(-POWER);
}
void turnRight() {
  rightMotor(-POWER);
  leftMotor(POWER);
}
void stopMoving() {
  rightMotor(0);
  leftMotor(0);
}

void driveForwardOneCell() {
  driveForward();
  delay(SECONDS_PER_CENTIMETER*CELL_SIZE);
  stopMoving();
  Serial.println("Drove forward one cell.");
}

void shiftOrientation(int direction) { // negative is left, positive is right
  currentOrientation = currentOrientation + direction;
  while (currentOrientation < 1) {
    currentOrientation = currentOrientation + 4;
  }
  while (currentOrientation > 4) {
    currentOrientation = currentOrientation - 4;
  }
}

void turnLeft90Degrees() {
  turnLeft();
  delay(SECONDS_PER_CENTIMETER*3); // TODO: Change this
  stopMoving();
  shiftOrientation(-1);
  Serial.println("Shifted orientation left.");
}

void turnRight90Degrees() {
  turnRight();
  delay(SECONDS_PER_CENTIMETER*3); // TODO: Change this
  stopMoving();
  shiftOrientation(1);
  Serial.println("Shifted orientation right.");
}

enum Orientation compareCellOrientation(int hereX, int hereY, int thereX, int thereY) { // tell us which way we need to be oriented to enter the next cell
  enum Orientation goalDirection = NORTH;
  Serial.print("Goal cell is ");
  if (hereX < thereX) { // goal is east
    goalDirection = EAST;
    Serial.println("east.");
  } else if (hereX > thereX) { // goal is west
    goalDirection = WEST;
    Serial.println("west.");
  } else if (hereY < thereY) { // goal is south
    goalDirection = SOUTH;
    Serial.println("south.");
  } else if (hereY > thereY) { // goal is north
    goalDirection = NORTH; // (redundant?)
    Serial.println("north.");
  } else { // goal is the same spot??
    Serial.println("in the same spot?");
  }
  return goalDirection;
}

void driveToNextCell() {
  enum Orientation goalOrientation = compareCellOrientation(0,0,0,1); // TODO: Change this to account for current locations
  while (currentOrientation != goalOrientation) {
    // Serial.print(goalDireciton)
    if ((goalOrientation == currentOrientation + 1 && currentOrientation+1 < 5) ||
        (goalOrientation == NORTH && currentOrientation+1 == 5)) {
      turnRight90Degrees();
    } else {
      turnLeft90Degrees();
    }
  }
  driveForwardOneCell();
}

/********************************************************************************/
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SWITCH, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
	pinMode(ECHO_LEFT, INPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_RIGHT, INPUT);
  
  // If need be, initialize map here

	Serial.begin(9600);
}

void loop() {
  Serial.println("start of loop.");
  detectAllWalls();
  if (!walls[1]) {
    driveForwardOneCell();
    delay(1000);
    turnRight90Degrees();
  }
  delay(1000);
}