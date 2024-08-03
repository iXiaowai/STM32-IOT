#ifndef __ADC_H
#define __ADC_H

#include "sys.h"  // Device header

//void delay_10us(uint32_t ms);
void ADC_GPIO_init(void);
//void USART1_init(void);
double read_ADC(uint8_t adc_ch);
double GetTemp(void);

#endif


