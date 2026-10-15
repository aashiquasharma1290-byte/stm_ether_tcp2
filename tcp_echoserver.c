/*
 * tcp_echoserver.c
 *
 *  Created on: 08-Oct-2026
 *      Author: aashiquakrisho
 */

#include "tcp_echoserver.h"
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include <string.h>

#define TCP_SERVER_PORT 7


       
/*
 * tcp_echoserver.c
 * STM32H747I-DISCO - lwIP RAW API, no RTOS
 * TCP server on port 7
 */

#include "tcp_echoserver.h"
#include "main.h"
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "lwip/pbuf.h"
#include <string.h>
#include <ctype.h>

#define TCP_SERVER_PORT  7
#define GREETING_MSG     "AASHIQUA"
#define LINE_MAX_LEN     128

extern UART_HandleTypeDef huart1;

static err_t tcp_echoserver_accept(void *arg,
                                   struct tcp_pcb *newpcb,
                                   err_t err);

static err_t tcp_echoserver_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err);

static void uart_print(const char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)s,
                      (uint16_t)strlen(s), 100);
}

static int str_ieq(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (tolower((unsigned char)*a) !=
            tolower((unsigned char)*b))
        {
            return 0;
        }
        a++;
        b++;
    }

    return (*a == '\0' && *b == '\0');
}

static const char *chat_reply(const char *msg)
{
    if (str_ieq(msg, "Hello STM32"))
        return "Hello PC\r\n";

    if (str_ieq(msg, "How are you?"))
        return "I am fine\r\n";

    return NULL;
}

static err_t send_data(struct tcp_pcb *tpcb,
                       const void *data, u16_t len)
{
    err_t result = tcp_write(tpcb, data, len,
                             TCP_WRITE_FLAG_COPY);

    if (result == ERR_OK)
        result = tcp_output(tpcb);

    if (result != ERR_OK)
        uart_print("[TCP] Send failed\r\n");

    return result;
}

void tcp_echoserver_init(void)
{
    struct tcp_pcb *pcb = tcp_new();

    if (pcb == NULL)
    {
        uart_print("[TCP] tcp_new failed\r\n");
        return;
    }

    if (tcp_bind(pcb, IP_ADDR_ANY, TCP_SERVER_PORT) != ERR_OK)
    {
        tcp_close(pcb);
        uart_print("[TCP] Bind failed\r\n");
        return;
    }

    pcb = tcp_listen(pcb);

    if (pcb == NULL)
    {
        uart_print("[TCP] Listen failed\r\n");
        return;
    }

    tcp_accept(pcb, tcp_echoserver_accept);
    uart_print("[TCP] Listening on port 7\r\n");
}

static err_t tcp_echoserver_accept(void *arg,
                                   struct tcp_pcb *newpcb,
                                   err_t err)
{
    LWIP_UNUSED_ARG(arg);

    if (err != ERR_OK || newpcb == NULL)
        return ERR_VAL;

    tcp_recv(newpcb, tcp_echoserver_recv);

    uart_print("[TCP] Client connected\r\n");

    send_data(newpcb, GREETING_MSG,
              (u16_t)strlen(GREETING_MSG));

    return ERR_OK;
}

static err_t tcp_echoserver_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err)
{
    char line[LINE_MAX_LEN];
    const char *reply = NULL;
    struct pbuf *q;
    u16_t total;

    LWIP_UNUSED_ARG(arg);

    if (p == NULL)
    {
        /* PC closed the connection */
        if (tcp_close(tpcb) != ERR_OK)
        {
            tcp_abort(tpcb);
            return ERR_ABRT;
        }

        return ERR_OK;
    }

    if (err != ERR_OK)
    {
        pbuf_free(p);
        return ERR_OK;
    }

    total = p->tot_len;

    /* Recognize short chat messages */
    if (total > 0 && total < LINE_MAX_LEN)
    {
        u16_t len = pbuf_copy_partial(p, line, total, 0);
        line[len] = '\0';

        while (len > 0 &&
               (line[len - 1] == '\r' ||
                line[len - 1] == '\n' ||
                line[len - 1] == ' '))
        {
            line[--len] = '\0';
        }

        uart_print("[PC] ");
        uart_print(line);
        uart_print("\r\n");

        reply = chat_reply(line);
    }
    else
    {
        uart_print("[PC] Message received\r\n");
    }

    if (reply != NULL)
    {
        send_data(tpcb, reply, (u16_t)strlen(reply));
    }
    else
    {
        /* Echo all parts of the received pbuf */
        for (q = p; q != NULL; q = q->next)
        {
            if (tcp_write(tpcb, q->payload, q->len,
                          TCP_WRITE_FLAG_COPY) != ERR_OK)
            {
                uart_print("[TCP] Echo send failed\r\n");
                break;
            }
        }

        tcp_output(tpcb);
    }

    tcp_recved(tpcb, total);
    pbuf_free(p);

    return ERR_OK;
}

