/*
 ============================================================================
 Name        : main.c
 Author      : Irshad
 Version     :
 Copyright   : Template file
 Description : FreeRTOS and Segger RTT Queue
 ============================================================================
 */

#include <stdio.h>

#include "system_config.h"

#include "FreeRTOS.h"

#include "task.h"

#include "queue.h"

#include "timers.h"

#define Segger_Trace_Print 1

TaskHandle_t MenuPrint_task_Handle;

TaskHandle_t CommandHandler_task_Handle;

TaskHandle_t CommandProcess_task_Handle;

TaskHandle_t UARTWrite_task_Handle;

uint32_t Notification_Value;

char RecievedByte;

void MenuPrint_task(void * params);

void CommandHandler_task(void * params);

void CommandProcess_task(void * params);

void UARTWrite_task(void * params);

void PrintMessage(char * params);

#define TRUE 1

#define FALSE 0

#define AVAILABLE TRUE

#define NOT_AVAILABLE FALSE

uint8_t UART_KEY = 0;

void Timer_Handler(void);

void UART_Handler(void);

void OpenDomain(void);

void TimerInit(void);

void LED(TimerHandle_t xTimer);

void Led_ON(void);

void Led_OFF(void);

void LED_Toggle_OFF(void);

void Led_Toggle(void);

char MessageContainer[] =
		{
				"\
		\r\nLED_ON             ----> 1 \
		\r\nLED_OFF            ----> 2 \
		\r\nLED_TOGGLE         ----> 3 \
		\r\nLED_TOGGLE_OFF     ----> 4 \
		\r\nLED_READ_STATUS    ----> 5 \
		\r\nEXIT_APP           ----> 0 \
		\r\nType your option here : " };

char * SecondaryMessage;

// Command Structure

typedef struct APP_CMD {
	uint8_t CommandNumber;
} APP_CMD_t;

QueueHandle_t CommandQueue = NULL;

QueueHandle_t UARTQueue = NULL;

TimerHandle_t SoftwareLEDTimer = NULL;

char CommandBuffer[25];

uint8_t CommandLen = 0;

static void prvSetupHardware(void) {

	GPIO->P0_25_MODE_REG = FUNC_GPIO + DIR_OUTPUT; //

	REG_SETF(CRG_TOP, CLK_AMBA_REG, HCLK_DIV, 0);

	REG_SETF(CRG_TOP, CLK_AMBA_REG, PCLK_DIV, 0);

	REG_SETF(CRG_TOP, CLK_CTRL_REG, SYS_CLK_SEL, 0);

	REG_SETF(GPREG, SET_FREEZE_REG, FRZ_SYS_WDOG, 1);

	while (!REG_GETF(CRG_TOP, CLK_CTRL_REG, RUNNING_AT_XTAL32M))
		;

	OpenDomain();

	REG_SETF(CRG_COM, CLK_COM_REG, UART_ENABLE, 1);

	UART->UART_SRR_REG = 0x7;

	REG_SETF(UART, UART_LCR_REG, UART_DLAB, 1);

	UART->UART_DLF_REG = (0xFF & 0x00001106);

	UART->UART_RBR_THR_DLL_REG = (0xFF & (0x00001106 >> 8));

	UART->UART_IER_DLH_REG = (0xFF & (0x00001106 >> 16));

	UART->UART_LCR_REG = 3; // no parity, 1 stop bit 8 data length and clear UART_DLAB to 0

	GPIO->P0_09_MODE_REG = 2; // UART TX

	GPIO->P0_08_MODE_REG = 1; // UART RX

	NVIC_EnableIRQ(UART_IRQn);

	NVIC_SetPriority(UART_IRQn, 5);

	REG_SETF(UART, UART_IER_DLH_REG, ERBFI_DLH0, 1); // enable data available IRQ

	TimerInit();

}

void PrintMessage(char * params) {

	while (*params != 0) {

		while ((UART->UART_LSR_REG & 0x20) == 0)
			;

		UART->UART_RBR_THR_DLL_REG = (0xFF & (*params));

		while ((UART->UART_LSR_REG & 0x40) == 0)
			;

		params++;
	}
}

int main(void) {

	DWT->CTRL |= (1 << 0);       //Enable CYCCNT in DWT_CTRL.

	prvSetupHardware();

	SEGGER_SYSVIEW_Conf();

	SEGGER_SYSVIEW_Start();

	CommandQueue = xQueueCreate(10, sizeof(APP_CMD_t*));

	UARTQueue = xQueueCreate(10, sizeof(char*));

	if ((CommandQueue != NULL) & (UARTQueue != NULL)) {

		xTaskCreate(MenuPrint_task, "Menu Printing Task", 500, NULL, 1,
				&MenuPrint_task_Handle);

		xTaskCreate(CommandHandler_task, "Command Handler Task", 500, NULL, 2,
				&CommandHandler_task_Handle);

		xTaskCreate(CommandProcess_task, "Command Processing Task", 500, NULL,
				2, &CommandProcess_task_Handle);

		xTaskCreate(UARTWrite_task, "UART Write Task", 500, NULL, 2,
				&UARTWrite_task_Handle);

		vTaskStartScheduler();
	} else {

		PrintMessage("Queue Creation failed \n\r");

		while (1)
			;

	}

}

void MenuPrint_task(void * params) {

	char * MessagePointer;

	MessagePointer = MessageContainer;

	while (1) {

		xQueueSend(UARTQueue, &MessagePointer, portMAX_DELAY);

		xTaskNotifyWait(0, 0, 0, portMAX_DELAY);
	}

}

void CommandHandler_task(void * params) {

	APP_CMD_t *NewCommand;

	while (1) {

		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		taskENTER_CRITICAL();

		NewCommand->CommandNumber = CommandBuffer[0] - 48; // ASCI conversion

		taskEXIT_CRITICAL();

		xQueueSend(CommandQueue, &NewCommand, portMAX_DELAY);

	}

}

void CommandProcess_task(void * params) {

	APP_CMD_t *ReceivedCommand;

	while (1) {

		xQueueReceive(CommandQueue, (void*) &ReceivedCommand, portMAX_DELAY);

		switch (ReceivedCommand->CommandNumber) {

		case 1:
			Led_ON();
			break;

		case 2:
			Led_OFF();
			break;

		case 3:
			Led_Toggle();
			break;

		case 4:
			LED_Toggle_OFF();
			break;
		default:
			break;
		}

	}

}

void UARTWrite_task(void * params) {

	char * CharBuffer;

	while (1) {

		xQueueReceive(UARTQueue, &CharBuffer, portMAX_DELAY);

		PrintMessage(CharBuffer);

	}

}

void Led_ON(void) {

	SecondaryMessage = "\n\r LED is turned on \n\r";

	GPIO->P0_DATA_REG |= (1 << 25);

	xQueueSend(UARTQueue, &SecondaryMessage, portMAX_DELAY);

}

void Led_OFF(void) {

	SecondaryMessage = "\n\r LED is turned off \n\r";

	GPIO->P0_DATA_REG &= ~(1 << 25);

	xQueueSend(UARTQueue, &SecondaryMessage, portMAX_DELAY);

}

void LED(TimerHandle_t xTimer) {

	GPIO->P0_DATA_REG ^= (1 << 25);

}

void LED_Toggle_OFF(void) {

	xTimerStop(SoftwareLEDTimer, portMAX_DELAY);

	SecondaryMessage = "\n\r LED toggle stopped \n\r";

	xQueueSend(UARTQueue, &SecondaryMessage, portMAX_DELAY);

}

void Led_Toggle(void) {

	if (SoftwareLEDTimer == NULL) {

		uint8_t ToggleTime = pdMS_TO_TICKS(500);

		SecondaryMessage = "\n\r LED toggle started \n\r";

		SoftwareLEDTimer = xTimerCreate("LED-TIMER", ToggleTime, pdTRUE, NULL,
				LED);

		xTimerStart(SoftwareLEDTimer, portMAX_DELAY);

		xQueueSend(UARTQueue, &SecondaryMessage, portMAX_DELAY);

	}

}

void Timer_Handler(void) {

	TIMER->TIMER_CLEAR_IRQ_REG = 0;

}

void UART_Handler(void) {

	traceISR_ENTER();

	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

	uint32_t TempStatus = (UART->UART_IIR_FCR_REG);

	UART_KEY = AVAILABLE;

	RecievedByte = UART->UART_RBR_THR_DLL_REG;

	if (RecievedByte != '\r')
		CommandBuffer[0] = RecievedByte & 0xFF;

	else if (RecievedByte == '\r') {

		xTaskGenericNotifyFromISR(MenuPrint_task_Handle, 0, eNoAction, NULL,
				&pxHigherPriorityTaskWoken);

		xTaskGenericNotifyFromISR(CommandHandler_task_Handle, 0, eNoAction,
				NULL, &pxHigherPriorityTaskWoken);

		if (pxHigherPriorityTaskWoken == pdTRUE)
			taskYIELD();

	}

	traceISR_EXIT();

}

void TimerInit(void) {

	traceISR_ENTER();

	TIMER->TIMER_CTRL_REG = 0;    // Disable TIMER block

	NVIC_DisableIRQ(TIMER_IRQn);  // Disable TIMER interrupt

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_EN, 0);    // Disable TIMER block

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_CLK_EN, 0); // Disable the clock to the TIMER block

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_IRQ_EN, 1); // Enable interrupts to be generated by TIMER

	TIMER->TIMER_PRESCALER_REG = 31;  // DIVN / (prescaler + 1)

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_SYS_CLK_EN, 0x1); // TIMER uses the DIVN clock

	TIMER->TIMER_RELOAD_REG = 500001; // interrupt generation every 500 milli seconds

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_ONESHOT_MODE_EN, 0x0); // Counter mode enabled - Counter Refills when limit exceeded

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_COUNT_DOWN_EN, 0x0);  // TIMER counts up

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_FREE_RUN_MODE_EN, 0); // Disable free run mode

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_CLK_EN, 1); // Enable TIMER block

	REG_SETF(TIMER, TIMER_CTRL_REG, TIM_EN, 1); // Enable the TIMER block

	TIMER->TIMER_CLEAR_IRQ_REG = 1; // Acknowledge TIMER IRQ

	NVIC_EnableIRQ(TIMER_IRQn);

	NVIC_SetPriority(TIMER_IRQn, 5); // Higher priority on UART

	traceISR_EXIT();

}

void OpenDomain(void) {

	REG_SETF(CRG_TOP, PMU_CTRL_REG, COM_SLEEP, 0); // exit communication P.D. from sleep

	while ( REG_GETF(CRG_TOP,SYS_STAT_REG,COM_IS_UP) == 0)
		;

	REG_SETF(CRG_TOP, PMU_CTRL_REG, TIM_SLEEP, 0); // exit timers P.D. from sleep

	while ( REG_GETF(CRG_TOP,SYS_STAT_REG,TIM_IS_UP) == 0)
		;

	REG_SETF(CRG_TOP, PMU_CTRL_REG, PERIPH_SLEEP, 0); // exit peripheral P.D. from sleep

	while ( REG_GETF(CRG_TOP,SYS_STAT_REG,PER_IS_UP) == 0)
		;

}

/**
 * @brief Malloc fail hook
 */
void vApplicationMallocFailedHook(void) {
	/* vApplicationMallocFailedHook() will only be called if
	 configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	 function that will get called if a call to pvPortMalloc() fails.
	 pvPortMalloc() is called internally by the kernel whenever a task, queue,
	 timer or semaphore is created.  It is also called by various parts of the
	 demo application.  If heap_1.c or heap_2.c are used, then the size of the
	 heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	 FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	 to query the size of free heap space that remains (although it does not
	 provide information on how the remaining heap might be fragmented). */

}

/**
 * @brief Application idle task hook
 */
void vApplicationIdleHook(void) {
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	 task. It is essential that code added to this hook function never attempts
	 to block in any way (for example, call xQueueReceive() with a block time
	 specified, or call vTaskDelay()).  If the application makes use of the
	 vTaskDelete() API function (as this demo application does) then it is also
	 important that vApplicationIdleHook() is permitted to return to its calling
	 function, because it is the responsibility of the idle task to clean up
	 memory allocated by the kernel to any task that has since been deleted. */

	__WFI();

}

/**
 * @brief Application stack overflow hook
 */
void vApplicationStackOverflowHook(void) {

	/* Run time stack overflow checking is performed if
	 configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 function is called if a stack overflow is detected. */

}

/**
 * @brief Application tick hook
 */
void vApplicationTickHook(void) {
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that
 * is used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
		StackType_t ** ppxIdleTaskStackBuffer, uint32_t * pulIdleTaskStackSize) {
	/* If the buffers to be provided to the Idle task are declared inside this
	 * function then they must be declared static - otherwise they will be
	 * allocated on the stack and so not exists after this function exits. */
	static StaticTask_t xIdleTaskTCB;
	static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE] __attribute__((aligned(32)));

	/* Pass out a pointer to the StaticTask_t structure in which the Idle
	 * task's state will be stored. */
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

	/* Pass out the array that will be used as the Idle task's stack. */
	*ppxIdleTaskStackBuffer = uxIdleTaskStack;

	/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	 * Note that, as the array is necessarily of type StackType_t,
	 * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t ** ppxTimerTaskTCBBuffer,
		StackType_t ** ppxTimerTaskStackBuffer,
		uint32_t * pulTimerTaskStackSize) {
	/* If the buffers to be provided to the Timer task are declared inside this
	 * function then they must be declared static - otherwise they will be
	 * allocated on the stack and so not exists after this function exits. */
	static StaticTask_t xTimerTaskTCB;
	static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH] __attribute__((aligned(32)));

	/* Pass out a pointer to the StaticTask_t structure in which the Timer
	 * task's state will be stored. */
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

	/* Pass out the array that will be used as the Timer task's stack. */
	*ppxTimerTaskStackBuffer = uxTimerTaskStack;

	/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	 * Note that, as the array is necessarily of type StackType_t,
	 * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
