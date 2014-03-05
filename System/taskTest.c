#include "taskTest.h"


void taskTest(void *param)
{
    xQueueHandle cmdQueue = (xQueueHandle)param;

    const unsigned long Delayms = 9000 / portTICK_RATE_MS;

    DispCmd NewCmd;
    char cmdid[6];

    NewCmd.cmdId = 0x1002;
    NewCmd.idOrig = 0x1104;
    NewCmd.sysReq = 0;
    NewCmd.execCmd.fnct =  cmdNULL;
    NewCmd.execCmd.param = 0;

    while(1)
    {

#if _VERBOSE_
        /* Print the command code */
        Hex16ToAscii(NewCmd.cmdId,cmdid);
        con_printf(">>[FlightPlan] Se genera comando: ");
        con_printf(cmdid); con_printf("\n\0");
#endif
        if(NewCmd.cmdId!=DAT_DUMMYCMD)
        {
            /* Queue NewCmd - Blocking */
            xQueueSend(cmdQueue, (const void *) &NewCmd, portMAX_DELAY);
        }

        con_printf("[Test] running... \r\n");
        vTaskDelay(Delayms);
    }
}
