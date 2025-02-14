#include "neo_pixel.h"
#include "ws2818b.pio.h"
#include "include/config.h"
static PIO np_pio;
static uint sm;
static pixel_t leds[LED_COUNT];

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin)
{
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0)
    {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    npClear();
}

/**
 * Define a cor de um LED específico.
 */
void npSetLED(uint index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index < LED_COUNT)
    {
        leds[index].R = r;
        leds[index].G = g;
        leds[index].B = b;
    }
}

/**
 * Limpa todos os LEDs.
 */
void npClear()
{
    for (uint i = 0; i < LED_COUNT; i++)
    {
        npSetLED(i, 0, 0, 0);
    }
    npWrite();
}

/**
 * Envia os dados para os LEDs.
 */
void npWrite()
{
    for (uint i = 0; i < LED_COUNT; i++)
    {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    // sleep_us(100);
}

/**
 * Executa a contagem regressiva de 4 a 1 com LEDs verdes.
 */
void npCountdown()
{
    npSetLED(led_index, 0, 255, 0); // Verde
    npSetLED(led_index-1, 0, 0, 0);
    npSetLED(led_index+1, 0, 0, 0);
    npWrite();
}
