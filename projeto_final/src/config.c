#include "include/config.h"

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
    printf("Sessão resetada\n");
}