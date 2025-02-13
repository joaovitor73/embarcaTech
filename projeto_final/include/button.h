#ifndef BUTTON_H
#define BUTTON_H

#include "pico/stdlib.h"

#define BTN_A 5
#define BTN_B 6
#define BTN_STICK 22
#define DEBOUNCE_TIME 200000 // 200ms em us

void btns_callback(uint gpio, uint32_t events);
void setup_buttons();

#endif