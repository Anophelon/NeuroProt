/************************************* Library **********************************************/
#include "libADC.h"

/********************************** Use Functions *******************************************/

void InitADC (void){ //PA0 - input ADC1 IN0
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER 	|= GPIO_MODER_MODER0_1 | GPIO_MODER_MODER0_0;				//MODER 11
	GPIOA->OSPEEDR 	|= GPIO_OSPEEDER_OSPEEDR0;									//OSPEED 11 - very high speed
	GPIOA->PUPDR 	|= GPIO_PUPDR_PUPDR0_0;      								//PUPDR 01
	
	ADC->CCR |= ADC_CCR_ADCPRE_1; 	//Presc = 6
	ADC1->SMPR2 |= (  ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_0); //Задаем время выборки                                                                                                                                                                               
    ADC1->CR2 |= ADC_CR2_EXTSEL; //Преобразование инжектированной группы                                                                                                         
    ADC1->CR2 |= ADC_CR2_EXTEN; //Разрешаем внешний запуск инжектированной группы
    ADC1->CR2 |= ADC_CR2_CONT; 													//Continuous conversion mode
	ADC1->CR2 |= ADC_CR2_SWSTART;//?
    ADC1->SQR3 |= ADC_SQR3_SQ1_0; //Задаем номер канала - ADC1
    ADC1->CR2 |= ADC_CR2_ADON;													//A/D Converter ON

}
