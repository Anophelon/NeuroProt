/************************************* Library **********************************************/
#include "stm32f446xx.h"                  // Device header
/********************************** Use Functions *******************************************/
//


void InitUART(void);
void SendUSART1 (char chr);
void SendStringUSART1 (char *str);
void SendDataUSART1 (uint8_t data);
void InitDMAuart (char *arr);
