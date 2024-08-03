#ifndef __WIFI_H__
#define __WIFI_H__	
#include "common.h"

u8 atk_8266_wifi_MQTT_Init(void);
u8 Data_Post_To_Cloud(char *str,float temperature,u8 *ack,u16 waittime);
	

#endif
