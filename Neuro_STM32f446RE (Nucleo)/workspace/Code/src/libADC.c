/************************************* Library **********************************************/
#include "libADC.h"

/********************************** Use Functions *******************************************/

void InitADC (void){ //PA0 - input ADC1 IN0

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN										//Enable ADC1
				 |	RCC_APB2ENR_SYSCFGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//init GPIO
	GPIOA->MODER 	|= GPIO_MODER_MODER0										//MODER 11
					|  GPIO_MODER_MODER1
					|  GPIO_MODER_MODER4
					|  GPIO_MODER_MODER6;
	GPIOA->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR0									//OSPEED 11 - very high speed
					|  GPIO_OSPEEDER_OSPEEDR1
					|  GPIO_OSPEEDER_OSPEEDR4
					|  GPIO_OSPEEDER_OSPEEDR6;
	GPIOA->PUPDR 	&= ~GPIO_PUPDR_PUPDR0      									//PUPDR 00
					&  ~GPIO_PUPDR_PUPDR1
					&  ~GPIO_PUPDR_PUPDR4
					&  ~GPIO_PUPDR_PUPDR6;

	ADC->CCR	 |= ADC_CCR_ADCPRE_1; 											//Presc = 6 => ADC 10Mhz
<<<<<<< Updated upstream

	//Sample time 480 cycles
		
	ADC1->SMPR2  |= (ADC_SMPR2_SMP0_2| ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0)
				 | 	(ADC_SMPR2_SMP0_2| ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_0)
				 |	(ADC_SMPR2_SMP0_2| ADC_SMPR2_SMP4_1 | ADC_SMPR2_SMP4_0)
				 |	(ADC_SMPR2_SMP0_2| ADC_SMPR2_SMP6_1 | ADC_SMPR2_SMP6_0);
		/* 
	ADC1->SMPR2  |= ( ADC_SMPR2_SMP0_0)
				 | 	( ADC_SMPR2_SMP1_0)
				 |	 (ADC_SMPR2_SMP4_0)
				 |	( ADC_SMPR2_SMP6_0);
*/	
	ADC1->CR2 	|= ADC_CR2_JEXTEN;
	ADC1->CR2 	|= ADC_CR2_CONT; 												//Continuous conversion mode
	ADC1->CR2 	|= ADC_CR2_JEXTSEL;
	ADC1->CR1 	|= ADC_CR1_SCAN;												//Scan mode
	ADC1->CR1 	|= ADC_CR1_RES_1;
	//ADC1->CR1 	|= ADC_CR1_JAUTO;
=======
	

	ADC1->SMPR2  |= (ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0); 	//Sample time 480 cycles                                                                                                                                                                            
  ADC1->CR2 	 |= ADC_CR2_EXTSEL; 											//                                                                                                         
  ADC1->CR2 	 |= ADC_CR2_EXTEN;												//
	ADC1->CR1 |=  ADC_CR1_SCAN;
	ADC1->CR2 	 |= ADC_CR2_EOCS;
  ADC1->CR2 	 &= ~ADC_CR2_CONT; 												//Single conversion mode 
  ADC1->SQR3	 &= ~ADC_SQR3_SQ1_0; 											//Choose IN0
	ADC1->CR2 	 |= ADC_CR2_ADON;												//A/D Converter ON

>>>>>>> Stashed changes

	ADC1->JSQR = ADC_JSQR_JL;
	ADC1->JSQR &= ~ADC_JSQR_JSQ1;												//CH0 1st
	ADC1->JSQR |= ADC_JSQR_JSQ2_0												//CH1 2nd
			   |  ADC_JSQR_JSQ3_2												//CH4 3rd
			   |  ADC_JSQR_JSQ4_1 | ADC_JSQR_JSQ3_2;							//CH6 4th

	ADC1->CR2 |= ADC_CR2_ADON; 													// Turn on ADC
	ADC1->CR2 |= ADC_CR2_JSWSTART;												// Start conversion
}

uint16_t StartConvADC (void)
{

	ADC1->CR2 |= ADC_CR2_SWSTART;
	while (!(ADC1->SR & ADC_SR_EOC));
	return (ADC1->DR);
}
