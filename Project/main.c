#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	  
#include "key.h"     
#include "malloc.h"		
#include "usart3.h"
#include "common.h" 
#include "WiFi.h"

#include "ds18b20.h"
#include "ADC.h"
#include "RTOS.h"
 
 
//FreeRTOSϵͳ���ͷ�ļ�
//#include "FreeRTOS.h"
//#include "task.h"

#include "queue.h"
/************************************************
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{	  

	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	ADC_GPIO_init(); 
	usart3_init(115200);		//��ʼ������3
	atk_8266_wifi_MQTT_Init();   //��ʼ��ESP8266
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	
	freertos_demo();
	
	
//	temperature = GetTemp();
//	while(1)
//	{
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		temperature2 = ds18b20_get_temperature()/10;
//	
//		printf("t1 = %f\r\n",temperature);
//		printf("t2 = %f\r\n",temperature2);
//		
//		Data_Post_To_Cloud(temperature,"",100);   //�����ϴ����ƶ�
//		Data_Post_To_Cloud(temperature2,"",100);   //�����ϴ����ƶ�
//		
//	}


	
	
		
	
}
 




