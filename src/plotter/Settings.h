#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>

// EEPROM

#define EEPROM_ADDR   0
#define EEPROM_MARKER 0xAB

// SETTINGS

struct Settings {
  byte marker;
  int  stepDelay;
  int  servoUp;
  int  servoDown;
  int  servoWait;
  int  circleSegs;
};

// GLOABALS

extern int stepDelay;
extern int servoUp;
extern int servoDown;
extern int servoWait;
extern int circleSegs;

extern const Settings defaults;

// FUNCTIONS

void loadSettings();
void saveSettings();
void resetSettings();

#endif // SETTINGS_H
