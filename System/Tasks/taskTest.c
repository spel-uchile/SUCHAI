#include "taskTest.h"
#include "i2c.h"
#include "i2c_comm.h"
#include "csp_if_lo.h"
#include "csp_thread.h"
#include "csp_if_i2c.h"
#include "csp_port.h"

extern xQueueHandle i2cRxQueue;


void taskClientCSP(void *param)
{
    const unsigned long Delayms = 10000 / portTICK_RATE_MS;
//    int csp_node = *(int *)param;

    int pingResult = 0;
    int len = 6;

    vTaskDelay(3000 / portTICK_RATE_MS);

    while(1)
    {
        printf("[CLI] Sending ping\n");
        pingResult = csp_ping(1, 3000, len, CSP_O_NONE);

        #if SCH_GRL_VERBOSE
            printf("Ping with payload of %d bytes, took %d ms\n", len, pingResult);
        #endif

        vTaskDelay(Delayms);
    }
}

void taskServerCSP(void *param)
{
    printf(">>[ServerCSP] Started\n");
    
    /* Pointer to current connection and packet */
    csp_conn_t *conn;
    csp_packet_t *packet;

    /* Bind all ports to socket */
    csp_socket_t *sock = csp_port_get_socket(CSP_ANY);

    /* Process incoming connections */
    while (1)
    {
        /* Wait for connection, 10000 ms timeout */
        if ((conn = csp_accept(sock, 10000)) == NULL)
            continue;

        /* Read packets. Timout is 1000 ms */
        while ((packet = csp_read(conn, 1000)) != NULL)
        {
            int i;

            switch (csp_conn_dport(conn))
            {
                case 10:
                    /* Print data in this port */
                    printf("[SRV] New packet: ");
                    for(i=0; i<packet->length; i++)
                        printf("%c", packet->data[i]);
                    printf("\n");

                default:                 
                    /* Let the service handler reply pings, buffer use, etc. */
                    csp_service_handler(conn, packet);
                    break;
            }
        }

        /* Close current connection, and handle next */
        csp_close(conn);
    }
}

void taskRxI2C(void *param)
{
    printf("[RxI2C] Started\n");
    int n_recv = 0;
    char new_data = 0;
    portBASE_TYPE result = pdFALSE;
    i2c_frame_t *frame = (i2c_frame_t *) csp_buffer_get(100);

    while(1)
    {
        result = xQueueReceive(i2cRxQueue, &new_data, 50/ portTICK_RATE_MS);

        //No more data received
        if(result != pdPASS)
        {
            if(n_recv > 0)
            {
                frame->len = n_recv;
                csp_i2c_rx(frame, NULL);

                frame = (i2c_frame_t *) csp_buffer_get(100);
                frame->len = 0;
                n_recv = 0;
            }
        }
        //New data received
        else
        {
            frame->data[n_recv] = (uint8_t)new_data;
            n_recv++;
        }
    }
}