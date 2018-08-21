#include "main.h"

int main(void){
	InitRCC();
	genMCO2();
	InitGPIO();
	//InitUART();
	//InitTIM2();

	xTaskCreate(vTaskLed1,"LED1",32,NULL,1,NULL);
	//xTaskCreate(vTaskBut2,"BUT",32,NULL,1,NULL);
	//xTaskCreate(vTaskTest,"Test",32,NULL,1,NULL);

	SendData = xQueueCreate( 5, sizeof( uint8_t ) );

	vTaskStartScheduler();								//planner

	while(1)LedErOn();
}

//************************************* Tasks *********************************************

void vTaskLed1 (void *argument){
	while(1){
		GPIOB->ODR |= GPIO_ODR_ODR_5;			//turn on led
		vTaskDelay(3000);
		GPIOB->ODR &= ~GPIO_ODR_ODR_5;			//turn off led
		vTaskDelay(3000);
	}
}

void vTaskBut2 (void *argument){
	while(1){
		uint8_t i = 0;
		while((GPIOA->IDR & GPIO_IDR_IDR_0) != 0){
			vTaskDelay(100);
			SendStringUSART2("Button\r\n");
		}
		vTaskDelay(100);
		if(uxQueueMessagesWaiting(SendData) != 0){
			xQueueReceive(SendData, &i, 0);
		}
		SendDataUSART2(i);
		vTaskDelay(100);

	}
}

void vTaskTest (void *argument){
	uint8_t i = 30;
	while(1){
			xQueueSend(SendData, &i, 0);
			i++;
			vTaskDelay(200);
	}
}

//********************************* Interraptions *****************************************


void USART1_IRQHandler(void){
	if ((USART1->SR & USART_SR_RXNE) != 0){	//check about data on RX
		USART1->SR &= ~USART_SR_RXNE;

		if(USART1->DR == '0'){
			SendUSART1('0');
			SendUSART1('\n');
			SendStringUSART1("OFF\r\n");
			GPIOD->ODR &= ~GPIO_ODR_ODR_15;			//turn off led blue
		}
		if(USART1->DR == '1'){
			SendUSART1('1');
			SendUSART1('\n');
			SendStringUSART1("ON\r\n");
			GPIOD->ODR |= GPIO_ODR_ODR_15;			//turn on led blue
		}
	}
}
