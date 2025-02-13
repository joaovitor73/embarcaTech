#include "include/buzzer.h"
#include "include/config.h"

void pwm_init_buzzer(uint pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
    printf("Buzzer inicializado\n");
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