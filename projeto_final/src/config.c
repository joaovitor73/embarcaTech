#include "include/config.h"

void reset_sessao()
{
    tempo = 60;
    isSessao = false;
    buttonPressed = false;
    stop_buzzer = false;
    ciclos = 0;
    desenhaInstrucao = false;
    led_index = 0;
    printf("Sessão resetada\n");
}