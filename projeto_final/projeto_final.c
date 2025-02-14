#include <stdio.h>            // Biblioteca padrão de entrada e saída
#include "pico/stdlib.h"      // Biblioteca padrão do Pico
#include "hardware/pio.h"     // Biblioteca de comunicação PIO
#include "pico/cyw43_arch.h"  // Biblioteca de comunicação CYW43, para uso do WiFi
#include "include/buzzer.h"   // Biblioteca de controle do buzzer
#include "include/button.h"   // Biblioteca de controle dos botões
#include "include/timer.h"    // Biblioteca de controle do timer
#include "neo_pixel.h"        // Biblioteca de controle dos LEDs

volatile uint tempo = 60;               // Tempo da sessao 60 segundos
volatile bool isSessao = false;         // Flag de sessao
volatile bool buttonPressed = false;    // Flag de botao pressionado
volatile bool stop_buzzer = false;      // Flag de parada do buzzer
volatile uint ciclos = 0;               // Ciclos de respiracao concliudos
volatile bool desenhaInstrucao = false; // Flag de desenho de instrucao
volatile uint led_index = 0;            // Indice do LED na matriz

struct tcp_pcb *pcb; // Estrutura de controle do cliente
struct repeating_timer timer_post; // Timer para envio de dados
char request[200];// Buffer de requisição
const char *text_request = "GET /update.json?api_key=OBED9VOECZ8RH5XF&field1=%d HTTP/1.1\r\n"
                           "Host: api.thingspeak.com\r\n"
                           "Connection: close\r\n\r\n"; // Texto da requisição

ip_addr_t server_ip;
bool send = false;

bool repeating_timer_callback_post(struct repeating_timer *t)
{
    send = true;
    return true;
}

void conecta_wifi();
void ip_servidor();
void send_dados();

int main()
{
    stdio_init_all(); // Inicializa a comunicação serial
    conecta_wifi();
    ip_servidor();
    add_repeating_timer_ms(10000, repeating_timer_callback_post, NULL, &timer_post);
    setup_buttons();             // Configura os botões
    pwm_init_buzzer(BUZZER_PIN); // Inicializa o buzzer
    init_i2c();                  // Inicializa o display
    reset_display();             // Reseta o display
    desenha_tela_inicial();      // Desenha a tela inicial
    setup_timer();               // Configura o timer

    while (true)
    {
        if (send)
        {
            send_dados();
        }

        play_tone(BUZZER_PIN, 1000, 5000); // Toca um tom no buzzer
        printf("Hello, world!\n");         // Imprime uma mensagem no terminal, usado para debug
        sleep_ms(1000);                    // Delay de 1 segundo
    }
}

void send_dados()
{
    snprintf(request, sizeof(request), text_request, tempo);

    pcb = tcp_new();
    client_create(pcb, &server_ip, 80);
    client_write(pcb, request);
    sleep_ms(100);
    client_close(pcb);
}

void ip_servidor()
{
    resolve_name("api.thingspeak.com", &server_ip);
    printf("IP resolvido: %s\n", ipaddr_ntoa(&server_ip));
}

void conecta_wifi()
{
    if (!connect_wifi("Infoway_Pedro", "Kupaki@j"))
    {
        printf("Falha ao conectar ao Wi-Fi\n");
    }
    else
    {
        printf("Conectado ao Wi-Fi\n");
    }
}