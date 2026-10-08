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

static err_t tcp_echoserver_accept(void *arg,
                                   struct tcp_pcb *newpcb,
                                   err_t err);

static err_t tcp_echoserver_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err);

void tcp_echoserver_init(void)
{
    struct tcp_pcb *pcb;

    pcb = tcp_new();

    if (pcb != NULL)
    {
        err_t err = tcp_bind(pcb, IP_ADDR_ANY, TCP_SERVER_PORT);

        if (err == ERR_OK)
        {
            pcb = tcp_listen(pcb);

            if (pcb != NULL)
            {
                tcp_accept(pcb, tcp_echoserver_accept);
            }
        }
        else
        {
            tcp_abort(pcb);
        }
    }
}

static err_t tcp_echoserver_accept(void *arg,
                                   struct tcp_pcb *newpcb,
                                   err_t err)
{
    if (err == ERR_OK)
    {
    	 tcp_recv(newpcb, tcp_echoserver_recv);

    	    const char *message = "AASHIQUA";

    	    tcp_write(newpcb,
    	              message,
    	              strlen(message),
    	              TCP_WRITE_FLAG_COPY);

    	    tcp_output(newpcb);
    }

    return ERR_OK;
}

static err_t tcp_echoserver_recv(void *arg,
                                 struct tcp_pcb *tpcb,
                                 struct pbuf *p,
                                 err_t err)
{
    if (p == NULL)
    {
        tcp_close(tpcb);
        return ERR_OK;
    }

    if (err == ERR_OK)
    {
        tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);
        tcp_output(tpcb);

        pbuf_free(p);
    }

    return ERR_OK;
}
