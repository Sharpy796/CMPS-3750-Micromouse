#include <Stepper.h>
// THIS IS NOT WHAT WE ARE USING, DO NOT INCLUDE THIS

const int PIN_TRIG = 9;
const int PIN_ECHO = 10;
float duration, distance;

// Distance limits (Centimeters)
const float DIST_MIN = 5.0f;
const float DIST_MAX = 8.0f;

// Stepper Variables
const int STEPS_PER_REVOLUTION = 2038;
const int PRECISION = 5;
const int RPM = 20;
Stepper myStepper = Stepper(STEPS_PER_REVOLUTION, 4, 5, 6, 7); // IN1, IN2, IN3, IN4 pins

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
	pinMode(PIN_ECHO, INPUT);
	myStepper.setSpeed(RPM);
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
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("- Too close!");
	  // myStepper.setSpeed(RPM);
    // myStepper.step(STEPS_PER_REVOLUTION/PRECISION);
    // delay(60000/RPM/PRECISION);
  } else if (DIST_MAX < distance) { // too far!
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("- Too far!");
	  // myStepper.setSpeed(RPM);
    // myStepper.step(-STEPS_PER_REVOLUTION/PRECISION);
    // delay(60000/RPM/PRECISION);
  } else { // just right!
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("- Just right!");
	  // myStepper.setSpeed(0);
    // myStepper.step(0);
  }
}
