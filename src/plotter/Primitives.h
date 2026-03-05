#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <Arduino.h>

// UI

void showMode(byte m, bool done);
void showMessage(const char* line1, const char* line2 = nullptr);

// PRIMITEVES

void circleRel(float r, int seg);
void circleAt(float r, int seg);
void squareRel(float size);
void rectRel(float w, float h);
void lineRel(float dx, float dy);
void diagonals(float size);

#endif // PRIMITIVES_H
