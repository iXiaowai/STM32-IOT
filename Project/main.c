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
 
 
//FreeRTOS系统相关头文件
//#include "FreeRTOS.h"
//#include "task.h"

#include "queue.h"
/************************************************
 ALIENTEK STM32F103开发板 扩展实验5
 ATK-RM04 WIFI模块测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


int main(void)
{	  

	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	ADC_GPIO_init(); 
	usart3_init(115200);		//初始化串口3
	atk_8266_wifi_MQTT_Init();   //初始化ESP8266
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
//		Data_Post_To_Cloud(temperature,"",100);   //数据上传到云端
//		Data_Post_To_Cloud(temperature2,"",100);   //数据上传到云端
//		
//	}


	
	
		
	
}
 




