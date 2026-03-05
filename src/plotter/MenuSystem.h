#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include "Config.h"

// TYPES

typedef void (*Action)();

struct Menu {
  const char* title;
  const char* names[MAX_ITEMS];
  Action actions[MAX_ITEMS];
  Menu* submenus[MAX_ITEMS];
  int* values[MAX_ITEMS];
  int vmins[MAX_ITEMS];
  int vmaxs[MAX_ITEMS];
  int vsteps[MAX_ITEMS];
  Menu* parent;
  byte count;
};

// STATE

extern Menu menus[MAX_MENUS];
extern byte menuCount;
extern Menu* cur;
extern byte cursor;
extern byte scroll;
extern bool editMode;

// BUILDER

void initMenu(const char* title);
void addParagraph(Menu* m, const char* name, Action func);
void addParagraph(const char* name, Action func);
Menu* addSubmenu(Menu* parent, const char* name);
Menu* addSubmenu(const char* name);
void addValue(Menu* m, const char* name, int* val, int vmin, int vmax, int vstep);
void addValue(const char* name, int* val, int vmin, int vmax, int vstep);

// UI

void drawMenu();
void moveCursor(int d);
void selectItem();

#endif // MENU_SYSTEM_H
