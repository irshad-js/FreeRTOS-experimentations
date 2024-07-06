/*
 ============================================================================
 Name        : main.c
 Author      : Irshad
 Version     :
 Copyright   : Template file
 Description : FreeRTOS and Segger RTT initialization template
 ============================================================================
 */


#include <stdio.h>

#include "system_config.h"

#include "FreeRTOS.h"

#include "task.h"

#define Segger_Trace_Print 1

TaskHandle_t xTaskHandle1 = NULL;

TaskHandle_t xTaskHandle2 = NULL;

void VTask1_handler (void * params);

void VTask2_handler (void * params);

void PrintMessage(char * params);

#define TRUE 1

#define FALSE 0

#define AVAILABLE TRUE

#define NOT_AVAILABLE FALSE

uint8_t UART_KEY = 1;

static void prvSetupHardware(void){

        REG_SETF(CRG_COM,CLK_COM_REG,UART_ENABLE,1);

        UART->UART_SRR_REG = 0x7;

        REG_SETF(UART,UART_LCR_REG,UART_DLAB,1);

        UART->UART_DLF_REG = (0xFF & 0x00001106);

        UART->UART_RBR_THR_DLL_REG = (0xFF & (0x00001106 >> 8));

        UART->UART_IER_DLH_REG = (0xFF & (0x00001106 >> 16));

        UART->UART_LCR_REG = 3; // no parity, 1 stop bit 8 data length and clear UART_DLAB to 0

        GPIO->P0_09_MODE_REG = 2; // UART TX
}

int main(void){

        DWT->CTRL |= 1<<0; // Cycle counting bit of ARM

        REG_SETF(CRG_TOP, CLK_AMBA_REG, HCLK_DIV, 0);

        REG_SETF(CRG_TOP, CLK_AMBA_REG, PCLK_DIV, 0);

        REG_SETF(CRG_TOP,PMU_CTRL_REG,COM_SLEEP,0);

        while(!REG_GETF(CRG_TOP,SYS_STAT_REG,COM_IS_UP));

        REG_SETF(CRG_TOP,PMU_CTRL_REG,PERIPH_SLEEP,0);

        while(!REG_GETF(CRG_TOP,SYS_STAT_REG,PER_IS_UP));

        REG_SETF(CRG_TOP, CLK_CTRL_REG, SYS_CLK_SEL,0);

        REG_SETF(GPREG,SET_FREEZE_REG,FRZ_SYS_WDOG,1);

        while (!REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_XTAL32M));

        prvSetupHardware();

		#ifdef Segger_Trace_Print

        SEGGER_SYSVIEW_Conf();

        SEGGER_SYSVIEW_Start();

		#endif

        xTaskCreate(VTask1_handler, "First_Task",130 ,NULL,2,&xTaskHandle1 );

        xTaskCreate(VTask2_handler, "Second_Task",130 ,NULL,2,&xTaskHandle2 );

        vTaskStartScheduler();

        for(;;);
}


void PrintMessage(char * params){

        while(*params!=0){

                while((UART->UART_LSR_REG&0x20)==0);

                UART->UART_RBR_THR_DLL_REG = (0xFF&(*params));

                while((UART->UART_LSR_REG&0x40)==0);

                params++;
        }
}

void VTask1_handler (void * params){

        while(1){

                if(UART_KEY == AVAILABLE){

                UART_KEY = NOT_AVAILABLE;

                PrintMessage("Called from First handler\r\n");

                UART_KEY = AVAILABLE;

#ifdef Segger_Trace_Print
                SEGGER_SYSVIEW_Print("Task 1 Yielded");
#endif
                taskYIELD();

                }
        }

}

void VTask2_handler (void * params){

        while(1){

                if(UART_KEY == AVAILABLE){

                UART_KEY = NOT_AVAILABLE;

                PrintMessage("Called from Second handler\r\n");

#ifdef Segger_Trace_Print
                SEGGER_SYSVIEW_Print("Task 2 Yielded");
#endif
                UART_KEY = AVAILABLE;

                taskYIELD();
                }
        }
}

