#include "MenuSystem.h"
#include "Settings.h"

#include <Arduino.h>
#include <GyverOLED.h>

extern GyverOLED<SSH1106_128x64, OLED_NO_BUFFER> oled;

// STATE

Menu menus[MAX_MENUS];
byte menuCount = 0;
Menu* cur = nullptr;
byte cursor = 0;
byte scroll = 0;
bool editMode = 0;

// BUILDER

void initMenu(const char* title) {
  menuCount = 1;
  menus[0].title = title;
  menus[0].parent = nullptr;
  menus[0].count = 0;
  cur = &menus[0];
}

void addParagraph(Menu* m, const char* name, Action func) {
  if (m->count >= MAX_ITEMS) return;
  byte i = m->count++;
  m->names[i] = name;
  m->actions[i] = func;
  m->submenus[i] = nullptr;
  m->values[i] = nullptr;
}

void addParagraph(const char* name, Action func) {
  addParagraph(&menus[0], name, func);
}

Menu* addSubmenu(Menu* parent, const char* name) {
  if (menuCount >= MAX_MENUS || parent->count >= MAX_ITEMS) return nullptr;
  byte id = menuCount++;
  menus[id].title = name;
  menus[id].parent = parent;
  menus[id].count = 0;

  byte i = parent->count++;
  parent->names[i] = name;
  parent->actions[i] = nullptr;
  parent->submenus[i] = &menus[id];
  parent->values[i] = nullptr;
  return &menus[id];
}

Menu* addSubmenu(const char* name) {
  return addSubmenu(&menus[0], name);
}

void addValue(Menu* m, const char* name, int* val, int vmin, int vmax, int vstep) {
  if (m->count >= MAX_ITEMS) return;
  byte i = m->count++;
  m->names[i] = name;
  m->actions[i] = nullptr;
  m->submenus[i] = nullptr;
  m->values[i] = val;
  m->vmins[i] = vmin;
  m->vmaxs[i] = vmax;
  m->vsteps[i] = vstep;
}

void addValue(const char* name, int* val, int vmin, int vmax, int vstep) {
  addValue(&menus[0], name, val, vmin, vmax, vstep);
}

// UI

static byte totalItems() {
  return cur->count + (cur->parent ? 1 : 0);
}

void drawMenu() {
  oled.clear();
  oled.home();

  if (editMode) {
    oled.setScale(1);
    oled.println(cur->names[cursor]);
    oled.println();
    oled.setScale(3);
    oled.print("  ");
    oled.println(*cur->values[cursor]);
    oled.setScale(1);
    oled.println();
    oled.print("  [");
    oled.print(cur->vmins[cursor]);
    oled.print(" .. ");
    oled.print(cur->vmaxs[cursor]);
    oled.println("]");
    oled.println();
    oled.println("  Нажми для выхода");
    return;
  }

  oled.setScale(2);
  oled.println(cur->title);
  oled.setScale(1);

  byte total = totalItems();
  if (!total) return;

  if (cursor < scroll) scroll = cursor;
  if (cursor >= scroll + VISIBLE) scroll = cursor - VISIBLE + 1;

  byte end = min(scroll + VISIBLE, total);

  for (byte i = scroll; i < end; i++) {
    oled.print(i == cursor ? "> " : "  ");
    if (i < cur->count) {
      oled.print(cur->names[i]);
      if (cur->submenus[i]) {
        oled.print(" >>");
      } else if (cur->values[i]) {
        oled.print(": ");
        oled.print(*cur->values[i]);
      }
    } else {
      oled.print("<< Назад");
    }
    oled.println();
  }
}

void moveCursor(int d) {
  if (editMode) {
    int* v = cur->values[cursor];
    *v += d * cur->vsteps[cursor];
    if (*v < cur->vmins[cursor]) *v = cur->vmins[cursor];
    if (*v > cur->vmaxs[cursor]) *v = cur->vmaxs[cursor];
    drawMenu();
    return;
  }

  byte total = totalItems();
  if (!total) return;
  if (d > 0 && cursor < total - 1) cursor++;
  if (d < 0 && cursor > 0) cursor--;
  drawMenu();
}

void selectItem() {
  if (editMode) {
    editMode = false;
    saveSettings();
    drawMenu();
    return;
  }

  if (cur->parent && cursor == cur->count) {
    cur = cur->parent;
    cursor = scroll = 0;
    drawMenu();
    return;
  }

  if (cur->submenus[cursor]) {
    cur = cur->submenus[cursor];
    cursor = scroll = 0;
    drawMenu();
    return;
  }

  if (cur->values[cursor]) {
    editMode = true;
    drawMenu();
    return;
  }

  if (cur->actions[cursor]) {
    cur->actions[cursor]();
    drawMenu();
  }
}
