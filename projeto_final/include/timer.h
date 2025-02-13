#ifndef TIMER_H
#define TIMER_H

#include "pico/stdlib.h"

bool repeating_timer_callback(struct repeating_timer *t);
void setup_timer();

#endif