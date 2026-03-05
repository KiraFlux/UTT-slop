#include <Arduino.h>

#include "Motion.h"
#include "Config.h"
#include "Settings.h"

// STATE

long posX = 0;
long posY = 0;

static bool stX = 0;
static bool stY = 0;

// CONVERTION

long mmToStepsX(float mm) {
  return (long)(mm * TICKS_PER_MM_X + 0.5f);
}

long mmToStepsY(float mm) {
  return (long)(mm * TICKS_PER_MM_Y + 0.5f);
}

float stepsToMmX(long steps) {
  return (float)steps / TICKS_PER_MM_X;
}

float stepsToMmY(long steps) {
  return (float)steps / TICKS_PER_MM_Y;
}

float getPosXmm() {
  return stepsToMmX(posX);
}

float getPosYmm() {
  return stepsToMmY(posY);
}

void resetPosition() {
  posX = 0;
  posY = 0;
}

void setPositionMM(float x, float y) {
  posX = mmToStepsX(x);
  posY = mmToStepsY(y);
}

void setPositionSteps(long x, long y) {
  posX = x;
  posY = y;
}

// INIT

void initMotion() {
  pinMode(X_DIR, OUTPUT);
  pinMode(X_STEP, OUTPUT);
  pinMode(X_EN, OUTPUT);
  pinMode(X_MS1, OUTPUT);
  pinMode(X_MS2, OUTPUT);
  
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(Y_EN, OUTPUT);
  pinMode(Y_MS1, OUTPUT);
  pinMode(Y_MS2, OUTPUT);
  
  digitalWrite(X_EN, 0);
  digitalWrite(Y_EN, 0);

  digitalWrite(X_MS1, 0);
  digitalWrite(X_MS2, 0);
  digitalWrite(Y_MS1, 0);
  digitalWrite(Y_MS2, 0);
  
  resetPosition();
}

// STEP

void stepX(bool dir) {
  digitalWrite(X_DIR, dir);
  digitalWrite(X_STEP, stX);
  stX = !stX;
  
  posX += dir ? 1 : -1;
}

void stepY(bool dir) {
  digitalWrite(Y_DIR, dir);
  digitalWrite(Y_STEP, stY);
  stY = !stY;
  
  posY += dir ? 1 : -1;
}

// MOVE

void moveTicksXY(long tx, long ty) {
  bool dirX = tx >= 0;
  bool dirY = ty >= 0;
  tx = abs(tx);
  ty = abs(ty);
  long maxT = max(tx, ty);
  if (maxT == 0) return;
  
  long errX = 0, errY = 0;

  for (long i = 0; i < maxT; i++) {
    errX += tx;
    errY += ty;
    if (errX >= maxT) { stepX(dirX); errX -= maxT; }
    if (errY >= maxT) { stepY(dirY); errY -= maxT; }
    delayMicroseconds(stepDelay);
  }
}

void moveTicksX(long t) {
  bool dir = t >= 0;
  t = abs(t);
  for (long i = 0; i < t; i++) {
    stepX(dir);
    delayMicroseconds(stepDelay);
  }
}

void moveTicksY(long t) {
  bool dir = t >= 0;
  t = abs(t);
  for (long i = 0; i < t; i++) {
    stepY(dir);
    delayMicroseconds(stepDelay);
  }
}

void moveMMXY(float dx, float dy) {
  moveTicksXY(mmToStepsX(dx), mmToStepsY(dy));
}

void moveMMX(float dx) {
  moveTicksX(mmToStepsX(dx));
}

void moveMMY(float dy) {
  moveTicksY(mmToStepsY(dy));
}

void moveAbsMM(float x, float y) {
  long targetX = mmToStepsX(x);
  long targetY = mmToStepsY(y);
  
  moveTicksXY(targetX - posX, targetY - posY);
}

void moveAbsSteps(long x, long y) {
  moveTicksXY(x - posX, y - posY);
}
