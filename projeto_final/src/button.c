#include "include/button.h"
#include "include/config.h"

uint32_t last_time = 0;

void btns_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_time > DEBOUNCE_TIME)
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
        last_time = current_time;
    }
}

void setup_buttons()
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
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    gpio_set_irq_enabled_with_callback(BTN_STICK, GPIO_IRQ_EDGE_FALL, true, btns_callback);
    printf("Botões configurados\n");
}