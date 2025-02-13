#include "include/timer.h"
#include "include/config.h"

struct repeating_timer timer;

bool repeating_timer_callback(struct repeating_timer *t)
{
    printf("Tempo: %d\n", tempo);
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

void setup_timer()
{
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
    printf("Timer configurado\n");
}