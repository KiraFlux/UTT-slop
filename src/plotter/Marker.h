#ifndef MARKER_H
#define MARKER_H

#include <Arduino.h>
#include <Servo.h>

// SERVO

extern Servo servo;

// FUNCTIONS

void initMarker();
void markerUp();
void markerDown();
void makeDot();

#endif // MARKER_H
