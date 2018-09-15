/************************************* Library **********************************************/
#include "libUART.h"

/******************************** USART Functions *******************************************/

//tasks: add USART1, PA9 - TX, PA10 - RX
void InitUART(void){
	RCC->APB2ENR 		|= RCC_APB2ENR_USART1EN; 													//Turn on clock on APB1 to UASRT2
	RCC->AHB1ENR		|= RCC_AHB1ENR_GPIOAEN;															//Turn on clock on AHB1 to GPIOA
	//PA9 - TX, PA10 - RX: PU AF
	GPIOA->MODER 		|= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;				//MODER 10
	GPIOA->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10;	//OSPEED 11 - very high speed
	GPIOA->PUPDR 		|= GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0;       //PUPDR 01
	GPIOA->AFR[1] 	|= 0x00000770;																			//PA9,PA10 - AF7

	//USART1->BRR =(APB1CLK+BAUDRATE/2)/BAUDRATE => (60 000 000 + 4800) /9600 = 0x186A
	//BRR=0x209 for br=115200
	//BRR=0x412 for br=57600
	//BRR=0x82 for br=460800
	//BRR=0x41 for br=921600

	//
	USART1->BRR		  = 0x41;
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
/************************************** DMA *************************************************/

void InitDMAuart (char *arr)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	DMA2_Stream7->PAR = (uint32_t)&USART1->DR;	//sent to USART1
	DMA2_Stream7->M0AR =(uint32_t)arr;			//source
	DMA2_Stream7->NDTR = 7;											//number of data register
	DMA2_Stream7->CR |= DMA_SxCR_CHSEL_2				//channel 4
									 |	DMA_SxCR_DIR_0					//memory to peripheral
									 						//circular mode |	DMA_SxCR_CIRC
									 |	DMA_SxCR_MINC;					//memory increment


	DMA2_Stream7->CR |= DMA_SxCR_EN;						//stream enable
	USART1->CR3 |= USART_CR3_DMAT;

}
void WriteDMAusart1 (char *arr)
{
	DMA2_Stream7->CR &= ~DMA_SxCR_EN;
	DMA2_Stream7->NDTR = sizeof(arr)-1;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
	DMA2_Stream7->CR |= DMA_SxCR_EN;
}