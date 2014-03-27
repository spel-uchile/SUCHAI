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
#include "taskTest.h"
#include "taskDispatcher.h"
#include "taskConsole.h"
#include "taskExecuter.h"
#include "taskHouskeeping.h"
#include "taskFlightPlan.h"
#include "taskComunications.h"
#include "taskDeployment.h"

/* Command Includes */
#include "cmdIncludes.h"

/* Config Words */
PPC_DEFAULT_CW1();
PPC_DEFAULT_CW2();
PPC_DEFAULT_CW3();

xQueueHandle dispatcherQueue, executerCmdQueue, executerStatQueue, i2cRxQueue;
xSemaphoreHandle dataRepositorySem, consolePrintfSem, rtcPrintSem;

xTaskHandle taskDeploymentHandle, taskDispatcherHandle, taskExecuterHandle;
xTaskHandle taskComunicationsHandle, taskConsoleHandle, taskFlightPlanHandle,
            taskFlightPlan2Handle, taskHouskeepingHandle;

int main(void)
{
    /* Initializing shared Queues */
    dispatcherQueue = xQueueCreate(25,sizeof(DispCmd));
    executerCmdQueue = xQueueCreate(1,sizeof(ExeCmd));
    executerStatQueue = xQueueCreate(1,sizeof(int));
    i2cRxQueue = xQueueCreate(I2C_MTU, sizeof(char));   //TRX_GOMSPACE

    /* Initializing shared Semaphore */
    dataRepositorySem = xSemaphoreCreateMutex();
    consolePrintfSem = xSemaphoreCreateMutex();
    rtcPrintSem = xSemaphoreCreateMutex();

    /* Configure Peripherals */
    /* NOTA: EL TIMER 1 Y SU INTERRUPCION ESTAN CONFIGURADOS POR EL S.0. (FreeRTOS) */
    default_PIC_config();

    /* System initialization */
    taskDeployment(NULL);

    /* Crating base tasks */
    printf("\n[main] Starting base tasks...\r\n");
    xTaskCreate(taskExecuter, (signed char *)"EXE", 3*configMINIMAL_STACK_SIZE, NULL, 4, &taskExecuterHandle);
    xTaskCreate(taskDispatcher, (signed char *)"DIS", 1.5*configMINIMAL_STACK_SIZE, NULL, 3, &taskDispatcherHandle);

    /* Initializing LibCSP*/
    dep_csp_initialization();

    /* Creating other tasks*/
    dep_launch_tasks(NULL);

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
    xSemaphoreTake(consolePrintfSem, portMAX_DELAY);
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