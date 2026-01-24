#include <GyverOLED.h>
#include <GyverEncoder.h>
#include <Servo.h>

Servo servo;

GyverOLED<SSH1106_128x64, OLED_NO_BUFFER> oled;

#define ENC_A 3
#define ENC_B 2
#define ENC_SW A3
Encoder enc(ENC_A, ENC_B, ENC_SW);

#define W 128
#define H 64
#define HEADER_H 16
#define ITEM_H 16
#define ITEMS_ON_SCREEN 5

enum UiMode {
  UI_MENU,
  UI_VALUE
};

UiMode uiMode = UI_MENU;

void markerUp() {
  servo.write(50);
  delay(700);
}

void markerDown() {
  servo.write(80);
  delay(700);
}

void makeDot() {
  markerDown();
  markerUp();
}

struct Menu;

typedef void (*Action)();

struct MenuItem {
  const char* text;
  Menu* submenu;
  Action action;
  uint8_t* value;
};

struct Menu {
  const char* title;
  MenuItem* items;
  uint8_t size;
  Menu* parent;
};

void moveTick(long ticks, bool dir);

Menu* currentMenu;
uint8_t cursor = 0;
uint8_t scroll = 0;
uint8_t* currentValue = nullptr;

void drawMenu();
void drawValue();
uint16_t readLineSensor();
bool whiteLine();
bool blackLine();
uint16_t lineWhite = 1023;
uint16_t lineBlack = 0;

void actionCalibration() {
  oled.clear();
  oled.home();
  oled.println("КАЛИБРОВКА");
  oled.println();
  oled.println("Ждите");

  for (int i = 0; i < 5000; ++i) {
    moveTick(1, 0);
    lineWhite = min(lineWhite, analogRead(A6));
    lineBlack = max(lineBlack, analogRead(A6));
  }

  oled.clear();
  oled.home();
  oled.println("КАЛИБРОВКА");
  oled.println();
  oled.println("ЗАВЕРШЕНА");
  oled.println();
  oled.print(lineWhite);
  oled.print("   ");
  oled.println(lineBlack);

  delay(1500);
}

void actionBackToMain();

MenuItem settingsItems[] = {
  {"Назад", nullptr, actionBackToMain, nullptr},
  {"Калибровка", nullptr, actionCalibration, nullptr},
};

Menu settingsMenu = {
  "НАСТРОЙКА",
  settingsItems,
  2,
  nullptr
};

void printModeProcessing(uint8_t mode) {
  oled.clear();
  oled.home();
  oled.setScale(2);
  oled.print("Режим ");
  oled.println(mode);
  oled.setScale(1);
  oled.println();
  oled.println("Выполняется");
}

void printModeDone(uint8_t mode) {
  oled.clear();
  oled.home();
  oled.setScale(2);
  oled.print("Режим ");
  oled.println(mode);
  oled.setScale(1);
  oled.println();
  oled.println("Выполнен");
}

bool st = 0;

void moveTick(long ticks = 1, bool dir = 0) {
  digitalWrite(4, dir);
  digitalWrite(10, dir);
  for (int i = 0; i < ticks; ++i) {
    digitalWrite(5, st);
    digitalWrite(11, st);
    st = !st;
    delayMicroseconds(500);
  }
}

void moveMM(long mm, bool dir = 0) {
  moveTick(mm * (2000.0 / 78.25), dir);
}

void homeX(bool dir) {
  while (whiteLine()) {
    moveTick(1, dir);
  } if (!dir) {
    while (blackLine()) {
      moveTick(1, 0);
    }
 } moveMM(8, 0);
}

void mode1() {
  printModeProcessing(1);
  delay(1000);
  digitalWrite(13, 1);
  homeX(0);
  delay(1000);
  moveMM(10);
  makeDot();
  moveMM(20);
  markerDown();
  moveMM(30);
  delay(500);
  markerUp();
  moveMM(150, 1);
  digitalWrite(13, 0);
  printModeDone(1);
  delay(1500);
}

bool morze[10][5] = {
  {1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1},
  {0, 0, 1, 1, 1},
  {0, 0, 0, 1, 1},
  {0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0},
  {1, 1, 0, 0, 0},
  {1, 1, 1, 0, 0},
  {1, 1, 1, 1, 0},
};

void mode2() {
  printModeProcessing(2);
  delay(1000);
  digitalWrite(13, 1);
  while (digitalRead(A1)) {}
  long last = millis();
  int c = 0;
  while (millis() - last <= 2000) {
    if (!digitalRead(A1)) {
      delay(30);
      c++;
      while (!digitalRead(A1)) {}
      delay(30);
      last = millis();
    }
  } oled.println();
  oled.print(c);
  homeX(0);
  moveMM(70);
  for (int i = 0; i < 5; ++i) {
    if (morze[c][i]) {
      markerDown();
      moveMM(10);
      markerUp();
      moveMM(5);
    } else {
      makeDot();
      moveMM(5);
    }
  } homeX(1);
  moveMM(80, 1);
  digitalWrite(13, 0);
  printModeDone(2);
  delay(1500);
}

void mode3() {
  printModeProcessing(3);
  delay(1000);
  digitalWrite(13, 1);
  homeX(0);
  delay(500);
  while (whiteLine()) moveTick();
  delay(500);
  while (blackLine()) moveTick();
  moveMM(8 + 10);
  makeDot();
  moveMM(50, 1);
  homeX(1);
  moveMM(80, 1);
  digitalWrite(13, 0);
  printModeDone(3);
  delay(1500);
}

void mode4() {
  printModeProcessing(4);
  delay(1000);
  digitalWrite(13, 1);
  homeX(0);
  moveMM(100);
  while (whiteLine()) moveTick();
  long c = 0;
  while (blackLine()) {
    moveTick();
    ++c;
  } if (c > 35) {
    moveMM(8 + 5);
    makeDot();
    moveMM(30, 1);
    makeDot();
    moveMM(50, 1);
  } else {
    moveMM(8 + 15);
    makeDot();
    moveMM(30);
    makeDot();
    moveMM(60, 1);
  } homeX(1);
  moveMM(80, 1);
  digitalWrite(13, 0);
  printModeDone(4);
  delay(1500);
}

void mode5() {
  printModeProcessing(5);
  digitalWrite(13, 1);
  // mode code
  digitalWrite(13, 0);
  printModeDone(5);
  delay(1500);
}

void mode6() {
  printModeProcessing(6);
  digitalWrite(13, 1);
  markerDown();
  markerUp();
  moveTick(2000, 0);
  markerDown();
  markerUp();
  digitalWrite(13, 0);
  printModeDone(6);
  delay(1500);
}

MenuItem mainItems[] = {
  {"Настройка", &settingsMenu, nullptr, nullptr},
  {"Режим 1", nullptr, mode1, nullptr},
  {"Режим 2", nullptr, mode2, nullptr},
  {"Режим 3", nullptr, mode3, nullptr},
  {"Режим 4", nullptr, mode4, nullptr},
  {"Режим 5", nullptr, mode5, nullptr},
  {"Режим 6", nullptr, mode6, nullptr},
};

Menu mainMenu = {
  "ГЛАВНАЯ",
  mainItems,
  7,
  nullptr
};

void actionBackToMain() {
  currentMenu = &mainMenu;
}

void drawMenu() {
  oled.clear();

  oled.home();
  oled.setScale(2);
  oled.println(currentMenu->title);
  oled.setScale(1);

  for (uint8_t i = 0; i < ITEMS_ON_SCREEN; i++) {
    uint8_t idx = scroll + i;
    if (idx >= currentMenu->size) break;

    uint8_t y = HEADER_H + i * ITEM_H;

    if (idx == cursor) {
      oled.print(" > ");
    } else {
      oled.print("   ");
    }

    oled.println(currentMenu->items[idx].text);
  }
}

void moveCursor(int8_t d) {
  if (d > 0 && cursor < currentMenu->size - 1) cursor++;
  if (d < 0 && cursor > 0) cursor--;

  if (cursor < scroll) scroll = cursor;
  if (cursor >= scroll + ITEMS_ON_SCREEN)
    scroll = cursor - ITEMS_ON_SCREEN + 1;

  drawMenu();
}

void selectItem() {
  MenuItem& it = currentMenu->items[cursor];

  if (it.submenu) {
    it.submenu->parent = currentMenu;
    currentMenu = it.submenu;
    cursor = scroll = 0;
    drawMenu();
    return;
  }

  if (it.action) {
    it.action();
    drawMenu();
  }
}

void back() {
  if (uiMode == UI_VALUE) {
    uiMode = UI_MENU;

    drawMenu();
    return;
  }


  if (currentMenu->parent) {
    currentMenu = currentMenu->parent;
    cursor = scroll = 0;
    drawMenu();
  }
}

uint16_t readLineSensor() {
  return map(analogRead(A6), lineWhite, lineBlack, 100, 900);
}

bool whiteLine() {
  return readLineSensor() < 400;
}

bool blackLine() {
  return readLineSensor() > 400;
}

void setupMotors() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  digitalWrite(6, 0);
  digitalWrite(12, 0);
}

void setupEnc() {
  enc.setType(TYPE2);
  enc.setFastTimeout(30);
  enc.setBtnPinMode(HIGH_PULL);
  enc.setPinMode(HIGH_PULL);
}

void setupOled() {
  oled.init();
  oled.flipH(true);
  oled.flipV(true);
}

void setup() {
  pinMode(A1, INPUT_PULLUP);
  
  setupOled();

  servo.attach(9);
  markerUp();

  setupEnc();

  pinMode(13, OUTPUT);

  setupMotors();

  currentMenu = &mainMenu;
  drawMenu();

  Serial.begin(9600);
}

void loop() {
  enc.tick();

  if (enc.isRight()) moveCursor(1);
  if (enc.isLeft())  moveCursor(-1);
  if (enc.isClick()) selectItem();
}
