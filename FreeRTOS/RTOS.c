#include "RTOS.h"



float temperature = 0.0;//内部温度
float temperature2 = 0.0;//外部温度


TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );


TaskHandle_t    task1_handler;
void ADC_task( void * pvParameters );

TaskHandle_t    task2_handler;
void DS18B20_task( void * pvParameters );


TaskHandle_t    task3_handler;
void DataToIOT( void * pvParameters );

/**
 * @brief       FreeRTOS入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,					//任务函数
                (char *                 )   "start_task",				//任务名称
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,		//任务栈大小
                (void *                 )   NULL,						//任务参数
                (UBaseType_t            )   START_TASK_PRIO,			//任务优先级
                (TaskHandle_t *         )   &start_task_handler );		//任务句柄
    vTaskStartScheduler();
}


void start_task( void * pvParameters)
{    
    taskENTER_CRITICAL();               /* 进入临界区 */
    xTaskCreate((TaskFunction_t         )   ADC_task,
                (char *                 )   "task1_ADC",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIO,
                (TaskHandle_t *         )   &task1_handler );
                 
    xTaskCreate((TaskFunction_t         )   DS18B20_task,
                (char *                 )   "task2_DS18B20",                  
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIO,
                (TaskHandle_t *         )   &task2_handler );
                
    xTaskCreate((TaskFunction_t         )   DataToIOT,
                (char *                 )   "task3_DataToIOT",                  
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_SIZE,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIO,
                (TaskHandle_t *         )   &task3_handler );    
	//vTaskDelete(NULL);
    vTaskDelete(start_task_handler);
	taskEXIT_CRITICAL();                /* 退出临界区 */
}


/* 任务一，实现adc内部温度检测 */
void ADC_task( void * pvParameters )
{
    while(1)
    {
        printf("ADC_task正在运行！！！\r\n");
        temperature = GetTemp();
		printf("temperature = %f\r\n",temperature);
        vTaskDelay(500);
    }
}



/* 任务二，实现外部温度检测 */
void DS18B20_task( void * pvParameters )
{
	int cnt = 5;
	while (cnt--)
	{
		ds18b20_init();
		delay_ms(200);
	}
    while(1)
    {
        printf("DS18B20_task正在运行！！！\r\n");
		
        temperature2 = ds18b20_get_temperature()/10;
		printf("temperature2 = %f\r\n",temperature);
        vTaskDelay(500);
    }
}


/* 任务三，实现上传数据 */
void DataToIOT( void * pvParameters )
{
    
    while(1)
    {
        printf("DataToIOT正在运行！！！\r\n");
		//(char *str,float temp,u8 *ack,u16 waittime)
        Data_Post_To_Cloud("temperature",temperature,"",100);   //数据上传到云端
		Data_Post_To_Cloud("temperature2",temperature2,"",100);   //数据上传到云端

		
        vTaskDelay(500);
    }
}
