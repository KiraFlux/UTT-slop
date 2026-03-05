#ifndef MOTION_H
#define MOTION_H

// STATE

extern long posX;
extern long posY;

// CONVERTION

long mmToStepsX(float mm);
long mmToStepsY(float mm);

float stepsToMmX(long steps);
float stepsToMmY(long steps);

float getPosXmm();
float getPosYmm();

void resetPosition();
void setPositionMM(float x, float y);
void setPositionSteps(long x, long y);

// INIT

void initMotion();

// STEP

void stepX(bool dir);
void stepY(bool dir);

// MOVE

void moveTicksXY(long tx, long ty);
void moveTicksX(long t);
void moveTicksY(long t);

void moveMMXY(float dx, float dy);
void moveMMX(float dx);
void moveMMY(float dy);

void moveAbsMM(float x, float y);
void moveAbsSteps(long x, long y);

#endif // MOTION_H
