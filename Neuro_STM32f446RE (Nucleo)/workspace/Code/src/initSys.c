/************************************* Library **********************************************/
#include "initSys.h"

/********************************** Use Functions *******************************************/

void InitRCC(void){
	//RCC->CR &= ~0xFFFFFFFF;                      										// clean CR
	//RCC->CFGR &= ~0xFFFFFFFF;															// clean CFGR
	//PWR->CR &= ~PWR_CR_VOS;
	//PWR->CR |= PWR_CR_VOS_1;

	RCC->CR |= ((uint32_t)RCC_CR_HSION); 												// Enable HSI
	while (!(RCC->CR & RCC_CR_HSIRDY));													// Ready start HSI

	//FLASH->ACR &= ~0xFFFFFFFF;
	//while (!(FLASH->ACR & FLASH_ACR_LATENCY)){}

	FLASH->ACR |= FLASH_ACR_PRFTEN 		|
				  FLASH_ACR_LATENCY_3WS 		|
				  FLASH_ACR_ICEN   	 				|
				  FLASH_ACR_DCEN;														// Cloclk Flash memory
	//while (!(FLASH->ACR & FLASH_ACR_LATENCY_3WS)){}


	RCC->CFGR &= ~RCC_CFGR_HPRE 	&													// clean AHB Pr
				 ~RCC_CFGR_PPRE1 	&													// clean APB1 Pr
				 ~RCC_CFGR_PPRE2;														// clean APB2 Pr

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1	 |													// AHB = SYSCLK/1 = 120/1
				 RCC_CFGR_PPRE1_DIV4 |													// APB1 = HCLK/4 = 120/4 = 30
				 RCC_CFGR_PPRE2_DIV2;													// APB2 = HCLK/2 = 60

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC & 													//Clean PLLCFGR & PLLSRC=0 => HSI clock selected
				 ~RCC_PLLCFGR_PLLM 	 &
				 ~RCC_PLLCFGR_PLLN 	 &
				 ~RCC_PLLCFGR_PLLP	 &
				 ~RCC_PLLCFGR_PLLR;

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI | 												// source HSI = 16MHz
				 RCC_PLLCFGR_PLLM_3 	| 												// DIV_M: source HSI/8 = 2 MHz 11111000
				 RCC_PLLCFGR_PLLN_3		|												// clock = 2 MHz * 120 = 240 MHz
				 RCC_PLLCFGR_PLLN_4		|
				 RCC_PLLCFGR_PLLN_5		|
				 RCC_PLLCFGR_PLLN_6		|
				 RCC_PLLCFGR_PLLR_1;													// unused in this project
	//already RCC_PLLCFGR_PLLP = 0; PLLN/2 = 240 MHz / 2 = 120 MHz
	RCC->CR |= RCC_CR_PLLON;                      										// enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}      										// wait till PLL is ready

	//RCC->CFGR &= ~RCC_CFGR_SW;                 											// clean SW
	RCC->CFGR |= RCC_CFGR_SW_PLL;                 										// select source SYSCLK = PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {} 								// wait till PLL is used
}

void genMCO2(void){ //PA8
	RCC->CFGR &= ~RCC_CFGR_MCO2;														//MCO2 take clock from SYSCLK
	RCC->CFGR &= ~RCC_CFGR_MCO2PRE;
	RCC->CFGR |=  RCC_CFGR_MCO2PRE_1 | RCC_CFGR_MCO2PRE_2;								//Div = 4 (120/4=30Mhz)

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;												//Turn on clock on AHB1 to GPIOA

	GPIOC->MODER 	|= GPIO_MODER_MODER9_1;												//MODER 10
	GPIOC->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;					//OSPEED 11 - very high speed
	GPIOC->PUPDR 	|= GPIO_PUPDR_PUPDR9_0;      										//PUPDR 01
	GPIOC->AFR[1] 	|= 0x00000000;														//Pc9 - MCO2 - AF0
}

void InitGPIO (void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN |												//Turn on clock on AHB1 to GPIOB & A
					RCC_AHB1ENR_GPIOAEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER5;													//clean moder PB5
	GPIOB->MODER |= GPIO_MODER_MODER5_0;												//PB5 PP GP output
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPDR5;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT_5;
	GPIOB->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR5;											//very high speed

	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;												//PA5 PP GP output
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR5;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;
	GPIOA->OSPEEDR|= GPIO_OSPEEDER_OSPEEDR5;											//very high speed
}

void InitTIM4 (void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;										//Enable TIM4
	//PSC = 1
	TIM4->PSC = 30000 - 1;
	TIM4->ARR = 25;																			//30Mhz/60k = 500Hz
	TIM4->DIER |= TIM_DIER_UIE;																	//Interrupt enable
	TIM4->CR1 |= TIM_CR1_CEN;																		//Counter enabled

	NVIC_EnableIRQ(TIM4_IRQn);																	//Interrupt
}
void LedErOn (void){
	GPIOD->ODR = GPIO_ODR_ODR_14;														//turn on led red
}
