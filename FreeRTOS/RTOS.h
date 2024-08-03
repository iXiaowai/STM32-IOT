#ifndef _RTOS_H_
#define _RTOS_H_

#include "sys.h"
#include "delay.h"
#include "ADC.h"
#include "usart.h"
#include "WiFi.h"
#include "ds18b20.h"
#include "FreeRTOS.h"
#include "task.h"

#define START_TASK_PRIO         1
#define START_TASK_STACK_SIZE   256

#define TASK1_PRIO         2
#define TASK1_STACK_SIZE   128

#define TASK2_PRIO         3
#define TASK2_STACK_SIZE   128

#define TASK3_PRIO         4
#define TASK3_STACK_SIZE   128

void start_task( void * pvParameters);
void ADC_task( void * pvParameters );
void DS18B20_task( void * pvParameters );
void DataToIOT( void * pvParameters );


void freertos_demo(void);



#endif
