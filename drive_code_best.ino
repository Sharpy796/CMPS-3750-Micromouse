// ----------------------------------------------------------
// LINKED LIST
// ----------------------------------------------------------
#define SIZE 50
class Queue{
  private:
    int data[SIZE], front, back;
  public:
    Queue(){
      front = 0;
      back = 0;
    }

    bool isEmpty(){
      return(front == back);
    }

    bool isFull(){
      return((back + 1) % SIZE == front);
    }

    bool enqueue(int n){
      if(!isFull()){
        data[back] = n;
        back = (back + 1) % SIZE;
        
        return true;
      }
      
      return false;
    }

    int dequeue(){
      if(!isEmpty()){
        int temp = data[front];
        front = (front + 1) % SIZE;
        return temp;
      }

      return -1;
    }
};

class Maze{
  // ----------------------------------------------------------
  // PRIVATE
  // ----------------------------------------------------------
  private:
    static const int width = 12;
    static const int height = 12;

    Queue qx = Queue();
    Queue qy = Queue();

  // ----------------------------------------------------------
  // PUBLIC 
  // ----------------------------------------------------------
  public:
    bool hasConnectionNorth[height][width];
    bool hasConnectionEast[height][width];
    bool hasConnectionSouth[height][width];
    bool hasConnectionWest[height][width];
    int floodValue[height][width];

    // ----------------------------------------------------------
    // CONSTRUCTORS
    // ----------------------------------------------------------
    Maze(){
      bool* upPtr = &(hasConnectionNorth[0][0]);
      bool* rightPtr = &(hasConnectionEast[0][0]);
      bool* downPtr = &(hasConnectionSouth[0][0]);
      bool* leftPtr = &(hasConnectionWest[0][0]);
      int* floodPtr = &(floodValue[0][0]);

      for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
          if(i == 0){
            *upPtr = false;
          } else {
            *upPtr = true;
          }

          if(i == height - 1){
            *downPtr = false;
          } else {
            *downPtr = true;
          }

          if(j == 0){
            *leftPtr = false;
          } else {
            *leftPtr = true;
          }

          if(j == width - 1){
            *rightPtr = false;
          } else {
            *rightPtr = true;
          }

          *floodPtr = -1;

          upPtr++;
          rightPtr++;
          downPtr++;
          leftPtr++;
          floodPtr++;
        }
      }

      Serial.println("Maze created");
    }

    // ----------------------------------------------------------
    // CELL FUNCTIONS
    // ----------------------------------------------------------
    void addWallSouth(int x, int y){
      if(y < height){
        hasConnectionSouth[y][x] = false;
        hasConnectionNorth[y+1][x] = false;
      }
    }

    void addWallEast(int x, int y){
      if(x < width){
        hasConnectionEast[y][x] = false;
        hasConnectionWest[y][x+1] = false;
      }
    }

    void addWallNorth(int x, int y){
      if(y > 0){
        hasConnectionNorth[y][x] = false;
        hasConnectionSouth[y-1][x] = false;
      }
    }

    void addWallWest(int x, int y){
      if(x > 0){
        hasConnectionWest[y][x] = false;
        hasConnectionEast[y][x-1] = false;
      }
    }

    // ----------------------------------------------------------
    // FLOOD FILL
    // ----------------------------------------------------------
    void floodFill(int startX, int startY){
      int* floodPtr = &(floodValue[0][0]);
      for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
          *floodPtr = -1;
          floodPtr++;
        }
      }

      floodValue[startY][startX] = 0;
      qx.enqueue(startX);
      qy.enqueue(startY);

      int curX;
      int curY;
      int curFlood;
      while(!qx.isEmpty() && !qy.isEmpty()){
        curX = qx.dequeue();
        curY = qy.dequeue();
        curFlood = floodValue[curY][curX];
        
        if(hasConnectionNorth[curY][curX]){
          if(floodValue[curY-1][curX] < 0 || floodValue[curY-1][curX] > curFlood + 1){
            floodValue[curY-1][curX] = curFlood + 1;
            qx.enqueue(curX);
            qy.enqueue(curY-1);
          }
        }

        if(hasConnectionEast[curY][curX]){
          if(floodValue[curY][curX+1] < 0 || floodValue[curY][curX+1] > curFlood + 1){
            floodValue[curY][curX+1] = curFlood + 1;
            qx.enqueue(curX+1);
            qy.enqueue(curY);
          }
        }

        if(hasConnectionSouth[curY][curX]){
          if(floodValue[curY+1][curX] < 0 || floodValue[curY+1][curX] > curFlood + 1){
            floodValue[curY+1][curX] = curFlood + 1;
            qx.enqueue(curX);
            qy.enqueue(curY+1);
          }
        }

        if(hasConnectionWest[curY][curX]){
          if(floodValue[curY][curX-1] < 0 || floodValue[curY][curX-1] > curFlood + 1){
            floodValue[curY][curX-1] = curFlood + 1;
            qx.enqueue(curX-1);
            qy.enqueue(curY);
          }
        }
      }
    } 


    /*
      TO SIMPLIFY IMPLEMENTATION TO SAVE PROCESSING TIME, THE FOLLOWING OUTPUT KEY IS USED

      ERRORS:
      0: START COORDINATE IS IDENTICAL TO END COORDINATE - GOAL REACHED
      -1: START CELL HAS NO CONNECTIONS - SENSORS PICKED UP A FALSE WALL

      OUTPUTS:
      1: NEXT CELL IS NORTH
      2: NEXT CELL IS EAST
      3: NEXT CELL IS SOUTH
      4: NEXT CELL IS WEST

      Never eat sour watermelon
    */
    int findNextStep(int startX, int startY, int endX, int endY){
      if(startX == endX && startY == endY){
        return 0; 
      }

      floodFill(endX, endY);

      int curFlood = floodValue[startY][startX];
      if(hasConnectionNorth[startY][startX]){
        if(floodValue[startY-1][startX] < curFlood){
          return 1;
        }
      }

      if(hasConnectionEast[startY][startX]){
        if(floodValue[startY][startX+1] < curFlood){
          return 2;
        }
      }

      if(hasConnectionSouth[startY][startX]){
        if(floodValue[startY+1][startX] < curFlood){
          return 3;
        }
      }

      if(hasConnectionWest[startY][startX]){
        if(floodValue[startY][startX-1] < curFlood){
          return 4;
        }
      }

      return -1;
    }
};

// PINS
//the right motor will be controlled by the motor A pins on the motor driver
#define PWMA 13
#define AIN2 12
#define AIN1 11

//the left motor will be controlled by the motor B pins on the motor driver
#define BIN1 10
#define BIN2 9
#define PWMB 8

#define TRIG_LEFT 7
#define ECHO_LEFT 6
float distanceLeft;

#define TRIG_FRONT 5
#define ECHO_FRONT 4
float distanceFront;

#define TRIG_RIGHT 3
#define ECHO_RIGHT 2
float distanceRight;

#define SWITCH 1

// WALLS
bool walls[] = {false, false, false};

// MAZE
Maze maze = Maze();
bool hasReachedTheEnd = false;

// DIRECTIONS
int leftDirection = 0; // -1 for backward, 0 for stopped, 1 for forward
int rightDirection = 0; // -1 for backward, 0 for stopped, 1 for forward
enum Orientation {
  NORTH = 1,
  EAST,
  SOUTH,
  WEST
};
enum Orientation currentOrientation = SOUTH;
enum Direction {
  FORWARD = 1,
  LEFT,
  BACKWARD,
  RIGHT,
};
#define START_X 0
#define START_Y 0
#define END_X 5
#define END_Y 5
int curX = START_X;
int curY = START_Y;
int goalX = END_X;
int goalY = END_Y;

// DISTANCES (Centimeters)
#define CELL_SIZE 18
// TODO: Fine-tune this number to get it to rotate 90 degrees
double ROTATION_SIZE = (11.0*PI)/4.0;
#define WALL_DISTANCE_AWAY 6.0f
#define SPEED_SOUND .0343

// motors won't run at or below 50%
#define POWER_PERCENT 0.51
// this number is 0 - 255, which represents 0% - 100% power
const double POWER = 255.0*POWER_PERCENT;
#define RPS 2.5
const double SPEED = POWER_PERCENT*RPS; // rotations per second
// centimeters
#define WHEEL_DIAMETER 6.5
const double WHEEL_CIRCUMFERENCE = PI*WHEEL_DIAMETER; //2*pi*radius = pi*diameter (centimeters)
const double CMPS = SPEED*WHEEL_CIRCUMFERENCE; // centimeters per second
const double SECONDS_PER_CENTIMETER = 1.0/CMPS; // seconds per centimeter

const double DRIVE_TIME_CONST = SECONDS_PER_CENTIMETER*CELL_SIZE*1000;
const double TURN_TIME_CONST = SECONDS_PER_CENTIMETER*ROTATION_SIZE*1000;

#define STOP_DELAY 25
bool hasDoneStartThings = false;

void printData() {
  Serial.println("------------------------");
  Serial.print("CELL_SIZE:\t"); Serial.println(CELL_SIZE);
  Serial.print("POWER:\t"); Serial.println(POWER);
  Serial.print("POWER_PERCENT:\t"); Serial.println(POWER_PERCENT);
  Serial.print("RPS:\t"); Serial.println(RPS);
  Serial.print("SPEED:\t"); Serial.println(SPEED);
  Serial.print("WHEEL_DIAMETER:\t"); Serial.println(WHEEL_DIAMETER);
  Serial.print("WHEEL_CIRCUMFERENCE:\t"); Serial.println(WHEEL_CIRCUMFERENCE);
  Serial.print("CMPS:\t"); Serial.println(CMPS);
  Serial.print("SECONDS_PER_CENTIMETER:\t"); Serial.println(SECONDS_PER_CENTIMETER);
  Serial.print("DRIVE_TIME_CONST:\t"); Serial.println(DRIVE_TIME_CONST);
  Serial.print("TURN_TIME_CONST:\t"); Serial.println(TURN_TIME_CONST);
  Serial.println("------------------------");
}


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
  Serial.print(distance); Serial.println(" cm. away.");
  return isWall;
}

bool detectWall(int TRIG, int ECHO) {
  return detectWall(updateSensor(TRIG, ECHO));
}

void detectAllWalls() {
  updateAllSensors(); // (redundant)
  walls[0] = detectWall(distanceLeft);
  walls[1] = detectWall(distanceFront);
  walls[2] = detectWall(distanceRight);
}

void senseMaze() {
  // updateAllSensors(); // oops, updateAllSensors() happens in detectAllWalls()
  detectAllWalls();
  switch (currentOrientation) {
    case 1: // North
      // update west, north, and east walls
      if (walls[0]) {maze.addWallWest(curX,curY);} // Left wall
      if (walls[1]) {maze.addWallNorth(curX,curY);} // Front wall
      if (walls[2]) {maze.addWallEast(curX,curY);} // Right wall
      Serial.println("Facing North");
      break;
    case 2: // East
      // update north, east, and south walls
      if (walls[0]) {maze.addWallNorth(curX,curY);} // Left wall
      if (walls[1]) {maze.addWallEast(curX,curY);} // Front wall
      if (walls[2]) {maze.addWallSouth(curX,curY);} // Right wall
      Serial.println("Facing East");
      break;
    case 3: // South
      // update easts, south, and west walls
      if (walls[0]) {maze.addWallEast(curX,curY);} // Left wall
      if (walls[1]) {maze.addWallSouth(curX,curY);} // Front wall
      if (walls[2]) {maze.addWallWest(curX,curY);} // Right wall
      Serial.println("Facing South");
      break;
    case 4: // West
      // update south, west, and north walls
      if (walls[0]) {maze.addWallSouth(curX,curY);} // Left wall
      if (walls[1]) {maze.addWallWest(curX,curY);} // Front wall
      if (walls[2]) {maze.addWallNorth(curX,curY);} // Right wall
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
    rightDirection = 1;
  } else if (motorPower < 0) { // if the motor should drive backward (negative speed)
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    rightDirection = -1;
  } else { // if the motor should stop
    // briefly move the motor in the opposite direction to prevent it from drifting after stopping
    // if (rightDirection > 0) {
    //   rightMotor(-POWER/POWER_PERCENT);
    // } else if (rightDirection < 0) {
    //   rightMotor(POWER/POWER_PERCENT);
    // }
    // delay(STOP_DELAY);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    rightDirection = 0;
  }
  analogWrite(PWMA, abs(motorPower)); // now that the motor direction is set, drive it
}

void leftMotor(int motorPower) { // function for driving the left motor
  if (motorPower > 0) { // if the motor should drive forward (positive speed)
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    leftDirection = 1;
  } else if (motorPower < 0) { // if the motor should drive backward (negative speed)
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    leftDirection = -1;
  } else { // if the motor should stop
    // // briefly move the motor in the opposite direction to prevent it from drifting after stopping
    // if (leftDirection > 0) {
    //   leftMotor(-POWER/POWER_PERCENT);
    // } else if (leftDirection < 0) {
    //   leftMotor(POWER/POWER_PERCENT);
    // }
    // delay(STOP_DELAY);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    leftDirection = 0;
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
  delay(DRIVE_TIME_CONST);
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
  delay(TURN_TIME_CONST);
  stopMoving();
  shiftOrientation(-1);
  Serial.println("Shifted orientation left.");
}

void turnRight90Degrees() {
  turnRight();
  delay(TURN_TIME_CONST);
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

void shiftPosition(int cells, Orientation direction) {
  Serial.print("Shifted position ");
  Serial.print(cells);
  Serial.print(" cell(s) to the");
  switch (direction) {
    case 1: // North
      curY -= cells;
      Serial.println(" North.");
      break;
    case 2: // East
      curX += cells;
      Serial.println(" East.");
      break;
    case 3: // South
      curY += cells;
      Serial.println(" South.");
      break;
    case 4: // West
      curX -= cells;
      Serial.println(" West.");
      break;
    default:
      Serial.println("... where?");
  }
  Serial.print("Now at (");
  Serial.print(curX);
  Serial.print(",");
  Serial.print(curY);
  Serial.println(")");
  detectOutOfBounds(); // Bugtesting
}

bool detectOutOfBounds() {
  bool outOfBounds = false;
  if (curX < 0) {
    Serial.println("X OUT OF BOUNDS -- LESS THAN 0");
    outOfBounds = true;
  }
  if (curY < 0) {
    Serial.println("Y OUT OF BOUNDS -- LESS THAN 0");
    outOfBounds = true;
  }
  if (curX > 11) {
    Serial.println("X OUT OF BOUNDS -- GREATERN THAN 11");
    outOfBounds = true;
  }
  if (curY > 11) {
    Serial.println("Y OUT OF BOUNDS -- GREATER THAN 11");
    outOfBounds = true;
  }
  return outOfBounds;
}

void swapGoals() {
  hasReachedTheEnd = !hasReachedTheEnd;
  if (hasReachedTheEnd) {
    goalX = START_X;
    goalY = START_Y;
  } else {
    goalX = END_X;
    goalY = END_Y;
  }
}

void driveToNextCellAtOrientation(int goalOrientation) {
  if (goalOrientation == 0) {
    stopMoving();
    Serial.println("At destination!");
    swapGoals();
    delay(5000);
  } else if (goalOrientation < 0) {
    Serial.println("Is there a destination to go to???");
  } else if (goalOrientation >= 1 && goalOrientation <= 4) {
    Serial.print("Goal orientation is ");
    Serial.println(goalOrientation);
    while (currentOrientation != goalOrientation) {
      if ((goalOrientation == currentOrientation + 1 && currentOrientation+1 < 5) ||
          (goalOrientation == NORTH && currentOrientation+1 == 5)) {
        turnRight90Degrees();
      } else {
        turnLeft90Degrees();
      }
      delay(250);
    }
    driveForwardOneCell();
    shiftPosition(1,currentOrientation);
  } else {
    Serial.println("Uhhhhh what happened with finding the next step?");
  }
}

void driveToNextCell() {
  // enum Orientation goalOrientation = compareCellOrientation(0,0,0,1);
  driveToNextCellAtOrientation(maze.findNextStep(curX,curY,goalX,goalY));
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

  Serial.begin(9600);
}

void loop() {
  Serial.println("--- LOOP START ---");

  if (!hasDoneStartThings) {
    printData();
    hasDoneStartThings = true;
    delay(3000);
  }
  
  senseMaze();
  driveToNextCell();
  delay(1000);
}