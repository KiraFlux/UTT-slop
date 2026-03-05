#ifndef CONFIG_H
#define CONFIG_H

// PINS

// Ось Y
#define Y_DIR 47
#define Y_STEP 46
#define Y_MS1 45
#define Y_MS2 44
#define Y_EN 43

// Ось X
#define X_DIR 41
#define X_STEP 39
#define X_MS1 37
#define X_MS2 35
#define X_EN 33

// Кнопки
#define BTN_X 27
#define BTN_Y 25

// Энкодер
#define ENC_A 3
#define ENC_B 2
#define ENC_SW 5

// Сервопривод
#define SERVO_PIN 9

// KINEMATICS

#define TICKS_PER_MM_X (10000.0 / 465.50)
#define TICKS_PER_MM_Y 40.0

// MENU LIMITS

#define MAX_ITEMS 16
#define MAX_MENUS 12
#define VISIBLE 5

#endif // CONFIG_H
