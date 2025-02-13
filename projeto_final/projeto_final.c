#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/pwm.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
// #include "include/ssd1306.h"
#include "include/buzzer.h"
#include "include/button.h"
#include "include/timer.h"
#include "include/display.h"
#include "include/matriz.h"

#include "neo_pixel.h"

volatile uint tempo = 60;
volatile bool inicio = false;
volatile bool fimTempo = false;
volatile bool isSessao = false;
volatile bool buttonPressed = false;
volatile bool stop_buzzer = false;
volatile uint ciclos = 0;
volatile bool desenhaInstrucao = false;
volatile uint led_index = 0;

int main()
{
    stdio_init_all();
    setup_buttons();
    pwm_init_buzzer(BUZZER_PIN);
    init_i2c();
    reset_display();
    desenha_tela_inicial();
    setup_timer();

    while (true)
    {
        play_tone(BUZZER_PIN, 1000, 5000);
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}