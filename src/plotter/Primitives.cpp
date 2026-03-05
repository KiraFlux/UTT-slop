#include <GyverOLED.h>

#include "Primitives.h"
#include "Config.h"
#include "Settings.h"
#include "Motion.h"
#include "Marker.h"

extern GyverOLED<SSH1106_128x64, OLED_NO_BUFFER> oled;

// UI

void showMode(byte m, bool done) {
  oled.clear();
  oled.home();
  oled.setScale(2);
  oled.print("Режим ");
  oled.println(m);
  oled.setScale(1);
  oled.println();
  oled.println(done ? "Выполнен" : "Выполняется");
}

void showMessage(const char* line1, const char* line2) {
  oled.clear();
  oled.home();
  oled.setScale(2);
  oled.println(line1);
  oled.setScale(1);
  if (line2) {
    oled.println();
    oled.println(line2);
  }
}

// PRIMITIVES

void circleRel(float r, int seg) {
  float step = 2.0 * PI / seg;
  float x0 = r, y0 = 0;
  moveMMXY(x0, y0);
  markerDown();
  for (int i = 1; i <= seg; i++) {
    float a = step * i;
    float x = r * cos(a);
    float y = r * sin(a);
    moveMMXY(x - x0, y - y0);
    x0 = x;
    y0 = y;
  }
  markerUp();
}

void circleAt(float r, int seg) {
  moveMMXY(r, 0);
  float step = 2.0 * PI / seg;
  float x0 = 0, y0 = 0;
  markerDown();
  for (int i = 1; i <= seg; i++) {
    float a = step * i;
    float x = r * cos(a) - r;
    float y = r * sin(a);
    moveMMXY(x - x0, y - y0);
    x0 = x;
    y0 = y;
  }
  markerUp();
  moveMMXY(-r, 0);
}

void squareRel(float size) {
  markerDown();
  moveMMXY(size, 0);
  moveMMXY(0, size);
  moveMMXY(-size, 0);
  moveMMXY(0, -size);
  markerUp();
}

void rectRel(float w, float h) {
  markerDown();
  moveMMXY(w, 0);
  moveMMXY(0, h);
  moveMMXY(-w, 0);
  moveMMXY(0, -h);
  markerUp();
}

void lineRel(float dx, float dy) {
  markerDown();
  moveMMXY(dx, dy);
  markerUp();
}

void diagonals(float size) {
  markerDown();
  moveMMXY(size, size);
  markerUp();
  
  moveMMXY(-size, 0);
  
  markerDown();
  moveMMXY(size, -size);
  markerUp();
}
