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

#define DEBOUNCE_TIME 200000 // 200ms em us de debounce
uint32_t last_time = 0;
ssd1306_t ssd;

#define BTN_A 5
#define BTN_B 6
#define BUZZER_PIN 21
#define BTN_STICK 22
#define BUZZER_FREQUENCY 100

volatile uint tempo = 60;
volatile bool inicio = false;
volatile bool fimTempo = false;
volatile bool isSessao = false;
volatile bool buttonPressed = false;
volatile bool stop_buzzer = false;
volatile uint ciclos = 0;
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;
volatile bool desenhaInstrucao = false;

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint8_t ssd_buffer[ssd1306_buffer_length];

void desenha_tela_inicial()
{
    reset_display();

    char titulo[] = "Monitor de";
    char subtitulo[] = "Respiracao";
    char instrucao[] = "Clique Btn A";
    char instrucao2[] = "para iniciar";

    ssd1306_draw_string(&ssd_buffer, 25, 5, titulo);
    ssd1306_draw_string(&ssd_buffer, 24, 18, subtitulo);
    ssd1306_draw_string(&ssd_buffer, 15, 32, instrucao);
    ssd1306_draw_string(&ssd_buffer, 18, 50, instrucao2);

    render_on_display(&ssd_buffer, &frame_area);
}

void reset_sessao()
{
    tempo = 60;
    inicio = false;
    fimTempo = false;
    isSessao = false;
    buttonPressed = false;
    stop_buzzer = false;
    ciclos = 0;
    desenhaInstrucao = false;
}

void desenha_tela_monitor(int ciclos, char *mensagem)
{
    reset_display();

    char tempo_str[10];
    sprintf(tempo_str, "%ds", tempo);

    char ciclos_str[10];
    sprintf(ciclos_str, "%d", ciclos);

    // Exibir Tempo no canto superior direito
    ssd1306_draw_string(&ssd_buffer, 95, 5, tempo_str);

    // Exibir Ciclos no canto superior esquerdo
    ssd1306_draw_string(&ssd_buffer, 5, 5, ciclos_str);

    // Exibir Mensagem no centro da tela
    ssd1306_draw_string(&ssd_buffer, 30, 30, mensagem);
    ssd1306_draw_string(&ssd_buffer, 45, 55, "Btn A Sair");

    render_on_display(&ssd_buffer, &frame_area);
}

void desenha_tela_final(bool venceu)
{
    reset_display();

    char titulo[] = "Fim da Sessao";
    char resultado[20];
    char instrucao[] = "Clique Btn A";
    char instrucao2[] = "para reiniciar";

    // Define a mensagem de vitória ou derrota
    if (venceu)
        strcpy(resultado, "Voce Venceu!");
    else
        strcpy(resultado, "Tente Novamente!");

    ssd1306_draw_string(&ssd_buffer, 20, 5, titulo);
    ssd1306_draw_string(&ssd_buffer, 15, 20, resultado); // Exibe o resultado
    ssd1306_draw_string(&ssd_buffer, 15, 40, instrucao);
    ssd1306_draw_string(&ssd_buffer, 18, 55, instrucao2);

    render_on_display(&ssd_buffer, &frame_area);
}

void sessao()
{
    printf("Sessão\n");
    if (isSessao)
    {
        printf("Sessão iniciada\n");
        play_tone(BUZZER_PIN, 1000, 5000);
    }
}

// Função de interrupção para todos os botões
void btns_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time()); // captura o momento do acionamento e converte para microsegundos
    if (current_time - last_time > DEBOUNCE_TIME)                  // Verifica se ja passou o tempo do debounce para acionamento novamente do botao
    {
        if (gpio == BTN_A)
        {
            printf("Botão A acionado\n");
            isSessao = !isSessao;
            buttonPressed = !buttonPressed;
            stop_buzzer = !stop_buzzer;

            if (!isSessao)
            {
                desenha_tela_inicial();
                desenhaInstrucao = true;
                reset_sessao();
            }
        }
        last_time = current_time; // Atualiza o tempo para o debounce
    }
}

void init_i2c()
{
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
}

void pwm_init_buzzer(uint pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void play_tone(uint pin, uint frequency, uint duration_ms)
{

    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 2); // 50% de duty cycle

    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    while ((to_ms_since_boot(get_absolute_time()) - start_time < duration_ms) && !stop_buzzer)
    {
        sleep_ms(10); // Pequeno delay para reduzir consumo de CPU
    }

    pwm_set_gpio_level(pin, 0); // Para o buzzer
}

// Função de callback que será chamada a cada intervalo definido pelo temporizador.
// Esta função alterna o estado do LED e imprime uma mensagem na saída serial.
bool repeating_timer_callback(struct repeating_timer *t)
{
    if (isSessao)
    {
        ciclos = (60 - tempo) / 12;

        // Define a fase correta com base no tempo restante
        int fase = ((60 - tempo) % 12) / 4; // 0 = Inspire, 1 = Segure, 2 = Expire
        const char *mensagem;

        if (fase == 0)
            mensagem = "Inspire...";
        else if (fase == 1)
            mensagem = "Segure...";
        else
            mensagem = "Expire...";

        if (tempo <= 0)
        {
            //   beep(BUZZER_PIN, 100);
            desenha_tela_final(true);
            fimTempo = true;
            return true;
        }
        else
        {
            desenha_tela_monitor(ciclos, mensagem);
        }

        tempo = tempo - 1;
        return true;
    }
}

void setup()
{
    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
    gpio_init(BTN_STICK);
    gpio_set_dir(BTN_STICK, GPIO_IN);
    gpio_pull_up(BTN_STICK);
    pwm_init_buzzer(BUZZER_PIN);
    init_i2c();
}

void reset_display()
{
    calculate_render_area_buffer_length(&frame_area);

    memset(&ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(&ssd_buffer, &frame_area);
}

void desenha_caractere() // atualiza o display com o caractere inserido pelo usuario
{
    reset_display();
    char textoLinha1[] = "Tempo: ";
    char textoLinha2[10];
    sprintf(textoLinha2, "%d", tempo);
    char textoLinha3[] = "Sessao: ";
    char textoLinha4[10];
    sprintf(textoLinha4, "%d", isSessao);
    ssd1306_draw_string(&ssd_buffer, 15, 0, textoLinha1);
    ssd1306_draw_string(&ssd_buffer, 15, 20, textoLinha2);
    ssd1306_draw_string(&ssd_buffer, 15, 35, textoLinha3);
    ssd1306_draw_string(&ssd_buffer, 15, 35, textoLinha3);
    ssd1306_draw_string(&ssd_buffer, 15, 50, textoLinha4);
    render_on_display(&ssd_buffer, &frame_area);
}

int main()
{
    stdio_init_all();
    setup();
    reset_display();
    desenha_tela_inicial();
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, btns_callback);

    // contador com temporizador
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    while (true)
    {
        play_tone(BUZZER_PIN, 1000, 5000);
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
