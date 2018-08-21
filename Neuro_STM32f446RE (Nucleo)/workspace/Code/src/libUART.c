/************************************* Library **********************************************/
#include "libUART.h"

/********************************** Use Functions *******************************************/
//tasks: add USART1, PA9 - TX, PA10 - RX
void InitUART(void){
	RCC->APB2ENR 		|= RCC_APB2ENR_USART1EN; 													//Turn on clock on APB1 to UASRT2
	RCC->AHB1ENR		|= RCC_AHB1ENR_GPIOAEN;															//Turn on clock on AHB1 to GPIOA
	//PA9 - TX, PA10 - RX: PU AF
	GPIOA->MODER 		|= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;				//MODER 10
	GPIOA->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10;	//OSPEED 11 - very high speed
	GPIOA->PUPDR 		|= GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0;       //PUPDR 01
	GPIOA->AFR[1] 	|= 0x00000770;																			//PA9,PA10 - AF7

	//USART1->BRR =(APB1CLK+BAUDRATE/2)/BAUDRATE => (60 000 000 + 4800) /9600 = 0x186A Exp:27AC
	USART1->BRR 		= 0x186A;											// (f(APB1)+ 4800)/9600, f(APB1) == 42 MHz
	USART1->CR1 	 |= USART_CR1_UE | 							//Enable USART1
								 		USART_CR1_TE | 							//Transmitter USART1
								 		USART_CR1_RE |							//Resiver USART1
								 		USART_CR1_RXNEIE;						//RX Interrupt Enable
	//USART1->CR2 = 0;
	//USART1->CR3 = 0;
	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn,4);
}

void SendUSART1 (char chr){
	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = chr;
}

void SendStringUSART1(char *str){
	uint8_t i = 0;
	while(str[i]){
			SendUSART1(str[i++]);
		}
}
void SendDataUSART1 (uint8_t data){
	while (!(USART1->SR & USART_SR_TC));
	USART1->DR = data;
}
