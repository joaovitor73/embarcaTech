#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"

// Definição do tempo de debounce para os botões (evita múltiplas leituras por clique)
#define DEBOUNCE_TIME 200000

// Frequência padrão do buzzer
#define BUZZER_FREQUENCY 100

// Variáveis globais compartilhadas entre os módulos
extern volatile uint tempo;
extern volatile bool inicio;
extern volatile bool fimTempo;
extern volatile bool isSessao;
extern volatile bool buttonPressed;
extern volatile bool stop_buzzer;
extern volatile uint ciclos;
extern volatile bool desenhaInstrucao;

// Definição dos pinos usados na aplicação
#define BTN_A 5       // Botão A
#define BTN_B 6       // Botão B (reserva)
#define BTN_STICK 22  // Botão de joystick (reserva)
#define BUZZER_PIN 21 // Pino do buzzer
#define I2C_SDA 14    // Pino de dados I2C (display)
#define I2C_SCL 15    // Pino de clock I2C (display)

void reset_sessao();

#endif // CONFIG_H
