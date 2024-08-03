#include "RTOS.h"



float temperature = 0.0;//�ڲ��¶�
float temperature2 = 0.0;//�ⲿ�¶�


TaskHandle_t    start_task_handler;
void start_task( void * pvParameters );


TaskHandle_t    task1_handler;
void ADC_task( void * pvParameters );

TaskHandle_t    task2_handler;
void DS18B20_task( void * pvParameters );


TaskHandle_t    task3_handler;
void DataToIOT( void * pvParameters );

/**
 * @brief       FreeRTOS��ں���
 * @param       ��
 * @retval      ��
 */
void freertos_demo(void)
{    
    xTaskCreate((TaskFunction_t         )   start_task,					//������
                (char *                 )   "start_task",				//��������
                (configSTACK_DEPTH_TYPE )   START_TASK_STACK_SIZE,		//����ջ��С
                (void *                 )   NULL,						//�������
                (UBaseType_t            )   START_TASK_PRIO,			//�������ȼ�
                (TaskHandle_t *         )   &start_task_handler );		//������
    vTaskStartScheduler();
}


void start_task( void * pvParameters)
{    
    taskENTER_CRITICAL();               /* �����ٽ��� */
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
	taskEXIT_CRITICAL();                /* �˳��ٽ��� */
}


/* ����һ��ʵ��adc�ڲ��¶ȼ�� */
void ADC_task( void * pvParameters )
{
    while(1)
    {
        printf("ADC_task�������У�����\r\n");
        temperature = GetTemp();
		printf("temperature = %f\r\n",temperature);
        vTaskDelay(500);
    }
}



/* �������ʵ���ⲿ�¶ȼ�� */
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
        printf("DS18B20_task�������У�����\r\n");
		
        temperature2 = ds18b20_get_temperature()/10;
		printf("temperature2 = %f\r\n",temperature);
        vTaskDelay(500);
    }
}


/* ��������ʵ���ϴ����� */
void DataToIOT( void * pvParameters )
{
    
    while(1)
    {
        printf("DataToIOT�������У�����\r\n");
		//(char *str,float temp,u8 *ack,u16 waittime)
        Data_Post_To_Cloud("temperature",temperature,"",100);   //�����ϴ����ƶ�
		Data_Post_To_Cloud("temperature2",temperature2,"",100);   //�����ϴ����ƶ�

		
        vTaskDelay(500);
    }
}
