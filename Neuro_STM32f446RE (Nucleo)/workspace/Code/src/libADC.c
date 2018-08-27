/************************************* Library **********************************************/
#include "libADC.h"

/********************************** Use Functions *******************************************/

void InitADC (void){ //PA0 - input ADC1 IN0
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN|RCC_APB2ENR_SYSCFGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER 	|= GPIO_MODER_MODER0;										//MODER 11
	GPIOA->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR0;									//OSPEED 11 - very high speed
	GPIOA->PUPDR 	&= ~GPIO_PUPDR_PUPDR0;      								//PUPDR 00
	
	ADC->CCR	 |= ADC_CCR_ADCPRE_1; 											//Presc = 6 => ADC 10Mhz
	
	ADC1->CR2 	 |= ADC_CR2_ADON;												//A/D Converter ON

	ADC1->SMPR2  |= (ADC_SMPR2_SMP0_2 | ADC_SMPR2_SMP0_1 | ADC_SMPR2_SMP0_0); 	//Sample time 480 cycles                                                                                                                                                                            
    ADC1->CR2 	 |= ADC_CR2_EXTSEL; 											//                                                                                                         
    ADC1->CR2 	 |= ADC_CR2_EXTEN;												//
    ADC1->CR2 	 &= ~ADC_CR2_CONT; 												//Single conversion mode 
    ADC1->SQR3	 &= ~ADC_SQR3_SQ1_0; 											//Choose IN0

}

uint16_t StartConvADC (void)
{
	ADC1->CR2 |= ADC_CR2_SWSTART;
		
	while (!(ADC1->SR & ADC_SR_EOC));
	return (ADC1->DR);
}
