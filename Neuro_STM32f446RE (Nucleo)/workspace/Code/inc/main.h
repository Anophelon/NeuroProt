/************************************* Library **********************************************/
#include "stm32f446xx.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "initSys.h"
#include "libUART.h"
#include "libPWM.h"
#include "libLCD.h"
#include "libADC.h"


/************************************* Defines **********************************************/
xQueueHandle SendDataADC;
void xTaskNextionHMI (void *argument);
void xTaskConvADC (void *argument);
void vTaskTest (void *argument);
void vTaskTest2 (void *argument);

void TIM4_IRQHandler(void);
