#ifndef NEO_PIXEL_H
#define NEO_PIXEL_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#define LED_COUNT 4 // Quantidade de LEDs na contagem regressiva.
#define LED_PIN 7   // Pino de controle dos LEDs.

#define NUM_PIXELS 25

typedef struct
{
    uint8_t G, R, B;
} pixel_t;

// Funções de controle
void npInit(uint pin);
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b);
void npClear();
void npWrite();
void npCountdown(); // Função para a contagem regressiva.

#endif // NEO_PIXEL_H
