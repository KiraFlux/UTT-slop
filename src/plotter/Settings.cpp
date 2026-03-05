#include "Settings.h"
#include <EEPROM.h>
#include <GyverOLED.h>

extern GyverOLED<SSH1106_128x64, OLED_NO_BUFFER> oled;

// GLOABLS

int stepDelay;
int servoUp;
int servoDown;
int servoWait;
int circleSegs;

const Settings defaults = {
  EEPROM_MARKER,
  300,
  80,
  50,
  700,
  80
};

// FUNCTIONS

void loadSettings() {
  Settings s;
  EEPROM.get(EEPROM_ADDR, s);
  if (s.marker != EEPROM_MARKER) {
    s = defaults;
    EEPROM.put(EEPROM_ADDR, s);
  }
  stepDelay = s.stepDelay;
  servoUp = s.servoUp;
  servoDown = s.servoDown;
  servoWait = s.servoWait;
  circleSegs = s.circleSegs;
}

void saveSettings() {
  Settings s;
  s.marker = EEPROM_MARKER;
  s.stepDelay = stepDelay;
  s.servoUp = servoUp;
  s.servoDown = servoDown;
  s.servoWait = servoWait;
  s.circleSegs = circleSegs;

  Settings old;
  EEPROM.get(EEPROM_ADDR, old);
  if (memcmp(&s, &old, sizeof(Settings)) != 0) {
    EEPROM.put(EEPROM_ADDR, s);
  }
}

void resetSettings() {
  stepDelay = defaults.stepDelay;
  servoUp = defaults.servoUp;
  servoDown = defaults.servoDown;
  servoWait = defaults.servoWait;
  circleSegs = defaults.circleSegs;
  saveSettings();

  oled.clear();
  oled.home();
  oled.setScale(2);
  oled.println("Сброс!");
  oled.setScale(1);
  oled.println();
  oled.println("Настройки по");
  oled.println("умолчанию");
  delay(1500);
}
