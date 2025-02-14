#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "include/config.h"

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint8_t ssd_buffer[ssd1306_buffer_length];

void init_i2c()
{
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
    printf("Display inicializado\n");
}

void reset_display()
{
    calculate_render_area_buffer_length(&frame_area);
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(ssd_buffer, &frame_area);
}

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

void desenha_tela_monitor(int ciclos, char *mensagem)
{
    reset_display();

    char tempo_str[10];
    sprintf(tempo_str, "%ds", tempo);

    char ciclos_str[10];
    sprintf(ciclos_str, "%d", ciclos);

    ssd1306_draw_string(&ssd_buffer, 95, 5, tempo_str);
    ssd1306_draw_string(&ssd_buffer, 5, 5, ciclos_str);
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

    if (venceu)
        strcpy(resultado, "Voce Venceu!");
    else
        strcpy(resultado, "Tente Novamente!");

    ssd1306_draw_string(&ssd_buffer, 20, 5, titulo);
    ssd1306_draw_string(&ssd_buffer, 20, 20, resultado);
    ssd1306_draw_string(&ssd_buffer, 15, 35, instrucao);
    ssd1306_draw_string(&ssd_buffer, 18, 55, instrucao2);

    render_on_display(&ssd_buffer, &frame_area);
}

void desenha_caractere()
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
    ssd1306_draw_string(&ssd_buffer, 15, 50, textoLinha4);
    render_on_display(&ssd_buffer, &frame_area);
}