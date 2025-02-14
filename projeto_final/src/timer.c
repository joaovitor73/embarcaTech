#include "include/timer.h"
#include "include/config.h"
#include "../neo_pixel.h"

struct repeating_timer timer;

bool repeating_timer_callback(struct repeating_timer *t)
{
    if (isSessao)
    {
        ciclos = (60 - tempo) / 12;

        // Define a fase correta com base no tempo restante
        int fase = ((60 - tempo) % 12) / 4; // 0 = Inspire, 1 = Segure, 2 = Expire
        const char *mensagem;               // Apaga tudo após a contagem
        if (fase == 0)
        {
            led_index += 1;
            mensagem = "Inspire...";
        }
        else if (fase == 1)
        {
            npClear();
            mensagem = "Segure...";
        }
        else
        {
            led_index -= 1;
            mensagem = "Expire...";
        }
        if (tempo <= 0)
        {
            desenha_tela_final(true);
            npClear();
            return true;
        }
        else
        {
            desenha_tela_monitor(ciclos, mensagem);
        }
        npCountdown();
        tempo = tempo - 1;
        return true;
    }
    else
    {
        npClear();
    }
}

void setup_timer()
{
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    printf("Timer configurado\n");
    npInit(LED_PIN);
}