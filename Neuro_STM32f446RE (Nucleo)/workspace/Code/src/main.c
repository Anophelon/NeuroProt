#include "main.h"
int i = 0;//test
int dir = 1;
uint8_t buffer[5] = {'a',1,1,1,1};

int main(void)
{
	InitRCC();
	genMCO2();
	InitGPIO();
	InitUART();
	//InitTIM2();
	InitADC();
<<<<<<< Updated upstream
	InitDMAuart(buffer);
	InitTIM4();

	//xTaskCreate(xTaskNextionHMI,"HMI",128,NULL,1,NULL);
	//xTaskCreate(xTaskConvADC,"ADC",128,NULL,2,NULL);
=======

	xTaskCreate(xTaskNextionHMI,"HMI",512,NULL,1,NULL);
	xTaskCreate(xTaskConvADC,"ADC",512,NULL,2,NULL);
>>>>>>> Stashed changes
	//xTaskCreate(vTaskTest,"Test",32,NULL,1,NULL);
	xTaskCreate(vTaskTest2,"Test",32,NULL,1,NULL);

	SendDataADC = xQueueCreate( 5, sizeof( uint16_t ) );

	vTaskStartScheduler();								//planner
	//while(1);
	while(1)LedErOn();
}

//************************************* Tasks *********************************************

void xTaskNextionHMI (void *argument)
{
	uint16_t adcResult = 0;
	char adcResultArr[3];
	//Set boud rate
	SendStringUSART1("bauds=115200");
	SendDataUSART1(0xFF);
	SendDataUSART1(0xFF);
	SendDataUSART1(0xFF);

	while(1)
	{
		if (uxQueueMessagesWaiting(SendDataADC) != 0)
		{
			xQueueReceive(SendDataADC, &adcResult, 0);							//Receive TaskConvADC
			sprintf(adcResultArr,"%u",adcResult);								//from int to arr char
			//Send to graf a number
			SendStringUSART1("Text1.txt=\"");
			SendStringUSART1(adcResultArr);
			SendStringUSART1("\"");
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			//Send to graf Voltage
			SendStringUSART1("add 3,0,");
			SendStringUSART1(adcResultArr);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
			SendDataUSART1(0xFF);
		}
		vTaskDelay(10);
	}
}

void xTaskConvADC (void *argument)
{
	uint16_t adcResult = 125;
	char adcResultArr[3];

	while(1)
<<<<<<< Updated upstream
	{
		adcResult = StartConvADC();		//max = 0xFFFF = 4095
		adcResult /= 16;				//max = 0xFF   = 255
		/*
         ADC1->CR2 |= ADC_CR2_JSWSTART; //Запуск преобразований
         //while (!(ADC1->SR & ADC_SR_JEOC)); //ждем пока первое преобразование завершится
         ADC1->SR = 0;
         adcResult = ADC1->JDR1;
		*/
		// Test Signal to HMI
=======
	{	
		adcResult = StartConvADC();			//max 0xfff = 4095
		adcResult /= 16;								//max 0xff	= 255
//****************************** Test Signal to HMI **********************************
>>>>>>> Stashed changes
		/*
		adcResult=i;
		if(i>250)
		{dir=0;}
		if(i<5)
		{dir=1;}
		if(dir==1)
		{i+=3;}
		if(dir==0)
		{i-=3;}
		*/


		xQueueSend(SendDataADC, &adcResult, 0);		//Send adcResult to TaskNexionHMI

		GPIOA->ODR ^= GPIO_ODR_ODR_5;			//turn on green led

		vTaskDelay(10);
	}
}
void vTaskTest (void *argument)
{
	while(1)
	{
		

	}
	
}
void vTaskTest2 (void *argument)
{
	while(1)
	{
		GPIOB->ODR |= GPIO_ODR_ODR_5;			//turn on led blue
		//GPIOB->ODR |= GPIO_ODR_ODR_5;			//turn on led blue

		vTaskDelay(500);
		GPIOB->ODR &= ~GPIO_ODR_ODR_5;			//turn on led blue
		//GPIOB->ODR &= ~GPIO_ODR_ODR_5;			//turn on led blue

		vTaskDelay(500);
	}
}

//********************************* Interraptions *****************************************

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;
	GPIOA->ODR |= GPIO_ODR_ODR_5;			//turn on led green
	ADC1->CR2 |= ADC_CR2_JSWSTART;												// Start conversion
	while (!(ADC1->SR & ADC_SR_JEOC));

	buffer[1] = (uint8_t)(ADC1->JDR1); // CH0
	buffer[2] = (uint8_t)(ADC1->JDR2); // CH1
	buffer[3] = (uint8_t)(ADC1->JDR3); // CH4
	buffer[4] = (uint8_t)(ADC1->JDR4); // CH6
	for(int i = 1; i < 5; i++){
		if( buffer[i]=='a'){
			buffer[i]++;
		}
	}
	WriteDMAusart1(buffer);
	//SendStringUSART1(buffer);
	
	GPIOA->ODR &= ~GPIO_ODR_ODR_5;
}
void USART1_IRQHandler(void)
{
	if ((USART1->SR & USART_SR_RXNE) != 0)
	{	//check about data on RX
		USART1->SR &= ~USART_SR_RXNE;

		if(USART1->DR == '0')
		{
			SendUSART1('0');
			SendUSART1('\n');
			SendStringUSART1("OFF\r\n");
			GPIOD->ODR &= ~GPIO_ODR_ODR_15;			//turn off led blue
		}
		if(USART1->DR == '1')
		{
			SendUSART1('1');
			SendUSART1('\n');
			SendStringUSART1("ON\r\n");
			GPIOD->ODR |= GPIO_ODR_ODR_15;			//turn on led blue
		}
	}
}
