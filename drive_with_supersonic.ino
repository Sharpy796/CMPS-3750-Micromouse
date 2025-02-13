#include <Stepper.h>

const int PIN_TRIG = 9;
const int PIN_ECHO = 10;
float duration, distance;

// Distance limits (Centimeters)
const float DIST_MIN = 3.0f;
const float DIST_MAX = 5.0f;

// Stepper Variables
const int STEPS_PER_REVOLUTION = 2038;
const int PRECISION = 20;
const int RPM = 5;
Stepper myStepper = Stepper(STEPS_PER_REVOLUTION, 4, 5, 6, 7); // IN1, IN2, IN3, IN4 pins

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_TRIG, OUTPUT);
	pinMode(PIN_ECHO, INPUT);
	Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN_TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(PIN_TRIG, HIGH);
	delayMicroseconds(10);
	digitalWrite(PIN_TRIG, LOW);

  duration = pulseIn(PIN_ECHO, HIGH);
  distance = (duration*.0343)/2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // I am expecting this to be really choppy--we might need to have it slow down instead of stopping dead when it's done
  if (distance < DIST_MIN) { // too close!
    Serial.println("- Too close!");
	  myStepper.setSpeed(RPM);
    myStepper.step(STEPS_PER_REVOLUTION/PRECISION);
  } else if (DIST_MAX < distance) { // too far!
    Serial.println("- Too far!");
	  myStepper.setSpeed(RPM);
    myStepper.step(-STEPS_PER_REVOLUTION/PRECISION);
  } else { // just right!
    Serial.println("- Just right!");
	  myStepper.setSpeed(0);
  }
}
