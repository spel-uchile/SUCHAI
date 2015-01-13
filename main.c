#if defined(__XC16__)
    #include <xc.h>
#else
    #include <p24FJ256GA110.h>
#endif

/* RTOS Includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "list.h"

/* Drivers includes */
#include "pic_pc104_config.h"

/* Task includes */
//#include "taskTest.h"
//#include "taskDispatcher.h"
//#include "taskConsole.h"
//#include "taskExecuter.h"
//#include "taskHouskeeping.h"
//#include "taskFlightPlan.h"
//#include "taskComunications.h"
#include "suchaiDeployment.h"

/* Command Includes */
#include "cmdIncludes.h"

/* Config Words */
PPC_DEFAULT_CW1();
PPC_DEFAULT_CW2();
PPC_DEFAULT_CW3();

xQueueHandle dispatcherQueue, i2cRxQueue, executerCmdQueue, executerStatQueue;
xSemaphoreHandle statusRepositorySem, consolePrintfSem, rtcPrintSem;

xTaskHandle taskDeploymentHandle, taskDispatcherHandle;
xTaskHandle taskComunicationsHandle, taskConsoleHandle, taskFlightPlanHandle,
            taskFlightPlan2Handle, taskFlightPlan3Handle, taskHouskeepingHandle;

int main(void)
{
    /* Initializing shared Queues */
    dispatcherQueue = xQueueCreate(25, sizeof(DispCmd));
    #if(SCH_TASKEXECUTER_INSIDE_TASKDISPATCHER==1)
        //no Queue creation
    #else
        executerCmdQueue = xQueueCreate(1,sizeof(ExeCmd));
        executerStatQueue = xQueueCreate(1,sizeof(int));
    #endif
    i2cRxQueue = xQueueCreate(I2C_MTU, sizeof(char));   //TRX_GOMSPACE

    /* Initializing shared Semaphore */
    statusRepositorySem = xSemaphoreCreateMutex();
    consolePrintfSem = xSemaphoreCreateMutex();
    rtcPrintSem = xSemaphoreCreateMutex();

    /* Configure Peripherals */
    /* NOTA: EL TIMER 1 Y SU INTERRUPCION ESTAN CONFIGURADOS POR EL S.0. (FreeRTOS) */
    default_PIC_config();

    /* Initializing LibCSP*/
    com_csp_initialization(); //Issue #8: Initialize libcsp before trx

    /* System initialization */
    dep_init_suchai_hw();
    dep_init_suchai_repos();

    /* Crating SUCHAI tasks */
    dep_init_suchai_tasks();

    /* Start the scheduler. Should never return */
    printf("\nStarting FreeRTOS [->]\r\n");
    vTaskStartScheduler();

    while(1)
    {
        /*
         * El sistema solo llega hasta aca si el Scheduler falla debido
         * a falta de memoria u otro problema
         */
        printf("\n>>FreeRTOS [FAIL]\n");
        ppc_reset(NULL);
    }

    return 0;
}

void vApplicationIdleHook(void)
{
    /*
     * **configUSE_IDLE_HOOK must be set to 1**
     * Esta funcion se ejecuta cuando el procesador
     * no esta siendo utilizado por otra tarea
     */

    ClrWdt();
    Idle();
}

void vApplicationStackOverflowHook(xTaskHandle* pxTask, signed char* pcTaskName)
{
    printf(">> Stak overflow! - TaskName: %s\n", pcTaskName);
    ppc_reset(NULL);
}

#define STDIN   0
#define STDOUT  1
#define STDERR  2
#define LF   '\n'
#define CR   '\r'
#define STDOUT_NO_CR

void    mon_putc(char ch);

int __attribute__((__weak__, __section__(".libc")))
write(int handle, void * buffer, unsigned int len)
{
    portBASE_TYPE semStatus = xSemaphoreTake(consolePrintfSem, 2000/portTICK_RATE_MS);
    if(semStatus != pdPASS){return 0;}

    int i = 0;
    switch (handle)
    {
        case STDOUT:
        case STDERR:
            while (i < len)
                mon_putc(((char*)buffer)[i++]);
            break;
    }
    xSemaphoreGive(consolePrintfSem);
    return (len);  // number of characters written
}

#define STDOUT_NO_CR_WITH_LF
void mon_putc(char ch)
{
    while(U1STAbits.UTXBF);  /* wait if the buffer is full */
#ifndef STDOUT_NO_CR_WITH_LF
    if (LF == ch)
        putcUART1(CR);
#endif
#ifdef STDOUT_NO_CR
    if (CR == ch)
        return;
#endif
    putcUART1(ch);
}

BOOL shouldDelayTask( portTickType * const pxPreviousWakeTime, portTickType xTimeIncrement)
{
    portTickType xTickCount = xTaskGetTickCount();

//    printf("  [shouldDelayTask] xLastWakeTime = %u, xDelay_ticks = %u, "
//            "xTickCount = %u \r\n", *pxPreviousWakeTime,
//            xTimeIncrement, xTickCount);


    portTickType xTimeToWake;
    BOOL xShouldDelay = FALSE;

    /* Generate the tick time at which the task wants to wake. */
    xTimeToWake = *pxPreviousWakeTime + xTimeIncrement;

    if( xTickCount < *pxPreviousWakeTime )
    {
            /* The tick count has overflowed since this function was
            lasted called.  In this case the only time we should ever
            actually delay is if the wake time has also	overflowed,
            and the wake time is greater than the tick time.  When this
            is the case it is as if neither time had overflowed. */
            if( ( xTimeToWake < *pxPreviousWakeTime ) && ( xTimeToWake > xTickCount ) )
            {
                    xShouldDelay = TRUE;
                    //printf(" asdasd 1\r\n");
            }
    }
    else
    {
            /* The tick time has not overflowed.  In this case we will
            delay if either the wake time has overflowed, and/or the
            tick time is less than the wake time. */
            if( ( xTimeToWake < *pxPreviousWakeTime ) || ( xTimeToWake > xTickCount ) )
            {
                    xShouldDelay = TRUE;
                    //printf(" asdasd 2\r\n");
            }
    }

//    /* Update the wake time ready for the next call. */
//    *pxPreviousWakeTime = xTimeToWake;

    if( xShouldDelay != FALSE )
    {

    }

    return xShouldDelay;
}