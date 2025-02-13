#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_PIN 21

void pwm_init_buzzer(uint pin);
void play_tone(uint pin, uint frequency, uint duration_ms);

#endif