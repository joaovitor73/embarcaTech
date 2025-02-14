#include "include/config.h"
#include "include/client.h"
#include "lwip/ip_addr.h"

struct tcp_pcb *pcb;
float temperature;
char temperature_send[20];
char request[200];
const char *text_request = "GET /update.json?api_key=OBED9VOECZ8RH5XF&field1=%d HTTP/1.1\r\n"
                           "Host: api.thingspeak.com\r\n"
                           "Connection: close\r\n\r\n";

ip_addr_t server_ip;
bool send = false;
struct repeating_timer timer_post;

bool repeating_timer_callback_post(struct repeating_timer *t)
{
    printf("Enviando temperatura: %f\n", tempo);
    if (send)
    {
        snprintf(request, sizeof(request), text_request, temperature);

        pcb = tcp_new();
        client_create(pcb, &server_ip, 80);
        client_write(pcb, request);
        sleep_ms(100);
        client_close(pcb);
        send = false;
    }

    return true;
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

void init_post_thingspeak()
{
    resolve_name("api.thingspeak.com", &server_ip);
    pcb = tcp_new();
    if (!client_create(pcb, &server_ip, 80))
    {
        printf("Falha ao criar cliente\n");
    }
    else
    {
        printf("Cliente criado\n");
    }
    add_repeating_timer_ms(1000, repeating_timer_callback_post, NULL, &timer_post);
}