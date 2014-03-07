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
    i2cRxQueue = xQueueCreate(100, sizeof(char));   //TRX_GOMSPACE

    /* Initializing shared Semaphore */
    dataRepositorySem = xSemaphoreCreateMutex();
    consolePrintfSem = xSemaphoreCreateMutex();
    rtcPrintSem = xSemaphoreCreateMutex();

    /* Crating base tasks (others are created inside taskDeployment) */
    xTaskCreate(taskExecuter, (signed char *)"executer", 5*configMINIMAL_STACK_SIZE, NULL, 4, &taskExecuterHandle);
    xTaskCreate(taskDispatcher, (signed char *)"dispatcher", 2*configMINIMAL_STACK_SIZE, NULL, 3, &taskDispatcherHandle);
    xTaskCreate(taskDeployment, (signed char *)"deployment", 2*configMINIMAL_STACK_SIZE, NULL, 3, &taskDeploymentHandle);

    /* Configure Peripherals */
    /* NOTA: EL TIMER 1 Y SU INTERRUPCION ESTAN CONFIGURADOS POR EL S.0. */
    default_PIC_config();

    /* Start the scheduler. Should never return */
    con_printf("\nStarting FreeRTOS [->]\r\n");
    vTaskStartScheduler();

    while(1)
    {
        /*
         * El sistema solo llega hasta aca si el Scheduler falla debido
         * a falta de memoria u otro problema
         */
        con_printf("\n>>FreeRTOS [FAIL]\n");
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
    con_printf(">> Stak overflow! - TaskName: ");
    con_printf((char *)pcTaskName);
    con_printf("\n");

    ppc_reset(NULL);
}
