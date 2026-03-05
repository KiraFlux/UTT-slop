#include "Marker.h"
#include "Config.h"
#include "Settings.h"

// SERVO

Servo servo;

// FUNCTIONS

void initMarker() {
  servo.attach(SERVO_PIN);
}

void markerUp() {
  servo.write(servoUp);
  delay(servoWait);
}

void markerDown() {
  servo.write(servoDown);
  delay(servoWait);
}

void makeDot() {
  markerDown();
  markerUp();
}
