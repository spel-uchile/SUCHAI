/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2013, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *      Copyright 2013, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "taskComunications.h"


extern xQueueHandle dispatcherQueue; /* Commands queue */
extern xQueueHandle i2cRxQueue;

static void com_RxI2C(xQueueHandle i2c_rx_queue);
static void com_receive_tc(csp_packet_t *packet);
static void com_receive_cmd(csp_packet_t *packet);

void taskComunications(void *param)
{
#if (SCH_TASKCOMUNICATIONS_VERBOSE)
    con_printf(">>[Comunications] Started\r\n");
#endif

    portTickType delay_ms    = 1000;    //Task period in [ms]
    int xsec = (delay_ms/1000);  //se ejecuta cada Xseg ahora
    portTickType delay_ticks = delay_ms / portTICK_RATE_MS; //Task period in ticks (==dalay_ms/10)

    DispCmd TcNewCmd;
    TcNewCmd.cmdId = CMD_CMDNULL;
    TcNewCmd.idOrig = CMD_IDORIG_TCOMUNICATIONS; /* Comunications */
    TcNewCmd.param = 0;

    /* Pointer to current connection, packet and socket */
    csp_conn_t *conn;
    csp_packet_t *packet;
    csp_socket_t *sock = csp_port_get_socket(CSP_ANY);

    unsigned long seconds_cnt = 0; /* Contador para comandos periodicos */
    unsigned int type_cnt = 0; /* Identificador del tipo de beacon siguiente */

    /* Comienza el ciclo de la tarea */
    //portTickType xLastWakeTime = xTaskGetTickCount();
    while(1)
    {
        /* Tarea periodica cada 1 Segundos*/
        //vTaskDelayUntil(&xLastWakeTime, delay_ticks);
        vTaskDelay(delay_ticks);  //just delay is enough
        seconds_cnt += xsec;

        /* Recibir datos a traves de I2C */
        com_RxI2C(i2cRxQueue);

        /* Actualizar y enviar beacon */
        if(seconds_cnt % SCH_TRX_BEACON_PERIOD == 0)
        {
            /* Ajustar el contador a los tipos de beacon que hay*/
            type_cnt = type_cnt % 5;

            TcNewCmd.cmdId = tcm_id_update_beacon;
            TcNewCmd.param = type_cnt;

            /* Queue NewCmd - Non-Blocking (Wait 0.5 task period) */
            xQueueSend(dispatcherQueue, &TcNewCmd, delay_ticks/2);
            type_cnt++;
        }

        /*** Acts as csp server ***/

        /* Wait for connection, 500 ms timeout */
        if ((conn = csp_accept(sock, 500)) == NULL)
            continue; /* Try again later */

        /* Read packets. Timout is 500 ms */
        while ((packet = csp_read(conn, 500)) != NULL)
        {
            switch (csp_conn_dport(conn))
            {
                case SCH_TRX_PORT_TC:
                    /* Process incoming TC */
                    com_receive_tc(packet);
                    csp_buffer_free(packet);
                    break;

                case SCH_TRX_PORT_DEBUG:
                    /* Debug port, only to print strings */
                    printf((char *)(packet->data));
                    csp_buffer_free(packet);
                    break;

                case SCH_TRX_PORT_CONSOLE:
                    /* Debug port, executes console commands */
                    com_receive_cmd(packet);
                    csp_buffer_free(packet);
                    break;
                    
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

/**
 * Parse TC frames and generates corresponding commands
 *
 * @param packet TC Buffer
 */
static void com_receive_tc(csp_packet_t *packet)
{
    static DispCmd newCmd;
    static int i, cmdid, cmdarg;

#if SCH_TASKCOMUNICATIONS_VERBOSE
    printf("[SRV] New TCs: ");
#endif

    /* Read each TC from data repo in format [ID,ARG] */
    for(i=0; i < (packet->length)/2; i+=2)
    {
        cmdid = packet->data16[i];
        cmdarg = packet->data16[i+1];

#if SCH_TASKCOMUNICATIONS_VERBOSE
        printf("(0x%X, 0x%X),", cmdid, cmdarg);
#endif
        /* Check for stop bytes, then add new cmd */
        if((cmdid != CMD_CMDNULL) && (cmdid != CMD_STOP) && (cmdarg != CMD_STOP))
        {
            newCmd.cmdId = cmdid; newCmd.param = cmdarg;
            xQueueSend(dispatcherQueue, &newCmd, portMAX_DELAY);
        }
    }

#if SCH_TASKCOMUNICATIONS_VERBOSE
    printf("\n");
#endif
}

/**
 * Parse tc frame as console commands
 *
 * @param packet TC Buffer
 */
static void com_receive_cmd(csp_packet_t *packet)
{
    /* Read each TC from data repo in format [ID,ARG] */
    int i;
    for(i=0; i < (packet->length); i++)
    {
        con_char_handler(packet->data[i]);
    }

    if(packet->data[i-1] != '\n')
        con_char_handler('\n');
}

/**
 * Receive incoming data from I2C and queue to libcsp
 *
 * @param frame_p i2c_frame_t pointer to a valid frame
 * @param i2c_rx_queue Valid queue to read data from
 */
static void com_RxI2C(xQueueHandle i2c_rx_queue)
{
    static int nrcv = 0;
    static char new_data = 0;
    static i2c_frame_t *frame_p = NULL;
    static portBASE_TYPE result = pdFALSE;

    if(frame_p == NULL)
    {
        frame_p = (i2c_frame_t *) csp_buffer_get(TRX_TMFRAMELEN8);
        frame_p->len = 0;
        nrcv = 0;
        return;
    }

    while(1) //Optimization, before: while(result == pdPASS)
    {
        result = xQueueReceive(i2c_rx_queue, &new_data, 50/ portTICK_RATE_MS);

        //No more data received
        if(result != pdPASS)
        {
            if(nrcv > 0)
            {
                frame_p->len = nrcv;
                csp_i2c_rx(frame_p, NULL);
                csp_buffer_free(frame_p);
                frame_p = NULL;
            }

            break; //Optimization, not conditional while exit
        }
        //New data received
        else
        {
            frame_p->data[nrcv] = (uint8_t)new_data;
            nrcv++;
        }
    }
}
