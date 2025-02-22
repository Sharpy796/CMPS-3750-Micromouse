//the right motor will be controlled by the motor A pins on the motor driver
const int PWMA = 13;            //speed control pin on the motor driver for the right motor
const int AIN2 = 12;            //control pin 2 on the motor driver for the right motor
const int AIN1 = 11;           //control pin 1 on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int BIN1 = 10;           //control pin 1 on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int PWMB = 8;           //speed control pin on the motor driver for the left motor

int switchPin = 7;             //switch to turn the robot on and off

const int PIN_ECHO = 6;
const int PIN_TRIG = 5;
float duration, distance;

// Distance limits (Centimeters)
const float DIST_MIN = 8.0f;
const float DIST_MAX = 12.0f;
const int SPEED = 200;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(switchPin, INPUT_PULLUP);   //set this as a pullup to sense whether the switch is flipped

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

	pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_TRIG, OUTPUT);

	Serial.begin(9600);
}

void loop() {
  if (digitalRead(switchPin) == LOW) {
    digitalWrite(PIN_TRIG, LOW);
	  delayMicroseconds(2);
	  digitalWrite(PIN_TRIG, HIGH);
	  delayMicroseconds(10);
	  digitalWrite(PIN_TRIG, LOW);

    duration = pulseIn(PIN_ECHO, HIGH);
    distance = (duration*.0343)/2;

    Serial.print("Distance: ");
    Serial.print(distance);

    // I am expecting this to be really choppy--we might need to have it slow down instead of stopping dead when it's done
    if (distance < DIST_MIN) { // too close!
      Serial.println(" (Too close!)");
      digitalWrite(LED_BUILTIN, LOW);
      rightMotor(-SPEED);                                //drive the right wheel forward
      leftMotor(-SPEED);                                 //drive the left wheel forward
    } else if (DIST_MAX < distance) { // too far!
      Serial.println(" (Too far!)");
      digitalWrite(LED_BUILTIN, LOW);
      rightMotor(SPEED);                                //drive the right wheel forward
      leftMotor(SPEED);                                 //drive the left wheel forward
    } else { // just right!
      Serial.println(" (Just right!)");
      digitalWrite(LED_BUILTIN, HIGH);
      rightMotor(0);                                //drive the right wheel forward
      leftMotor(0);                                 //drive the left wheel forward
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    rightMotor(0);
    leftMotor(0);
  }
}

/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
    // digitalWrite(AIN1, HIGH);                          //set pin 1 to low
    // digitalWrite(AIN2, LOW);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
  // analogWrite(PWMA, motorSpeed);                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
    // digitalWrite(BIN1, HIGH);                          //set pin 1 to low
    // digitalWrite(BIN2, LOW);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
  // analogWrite(PWMB, motorSpeed);                 //now that the motor direction is set, drive it at the entered speed
}
