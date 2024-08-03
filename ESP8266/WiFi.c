#include "common.h"
#include "stdlib.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//��������Ҫ�������ƶ��ϴ�����
//��Ҫ�õ�WIFi.c����common.c
								  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA ���Ӱ�����


/*
��Ԫ��
    "product_key": "a1fmOzBOIDW",
    "device_name": "Temperature01",
    "device_secret": "7c1df343a7bd95ec2427cdb460925588"
	����
	{"clientId":"a1fmOzBOIDW.Temperature01|securemode=2,signmethod=hmacsha256,timestamp=1719973767245|",
	"username":"Temperature01&a1fmOzBOIDW",
	"mqttHostUrl":"a1fmOzBOIDW.iot-as-mqtt.cn-shanghai.aliyuncs.com",
	"passwd":"e8f091cefc101dbb736fa2187403805113e3031a4da1ea4e6790cb1430997963",
	"port":1883}
	
*/



u8 atk_8266_wifi_MQTT_Init(void)
{
//        u8 *p;
        atk_8266_send_cmd("AT+CWMODE=1","OK",20);        //����WIFI STAģʽ
        //atk_8266_send_cmd("AT+RST","OK",20);        //��λ
        delay_ms(1000);  //��ʱ2s���ȴ������ɹ�
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        atk_8266_send_cmd("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"","OK",50);        //����ʱ��� SNTP ������
        //sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
		while(atk_8266_send_cmd("AT+CWJAP=\"Xiaomi14\",\"12349876\"","OK",50));        //����WiFi������WiFi������
        printf("wifi�������ӳɹ�....\r\n");
        
        //AT+MQTTUSERCFG=0,1,"NULL","�û���","����",0,0,""
        //����������Clientld��Username��Password�ɰ�����Aliyun���ù��߻��
        atk_8266_send_cmd("AT+MQTTUSERCFG=0,1,\"NULL\",\"Temperature01&a1fmOzBOIDW\",\"370f57b1a1435439c9e96a8178c105686681e65d\",0,0,\"\"","",50);        
        printf("�û��豸����...\r\n");
    
        //ʹ�ñ����clientId����;  clientId������ע��ÿ������ǰ��ӷָ�����\����������    
        //AT+MQTTCLIENTID=0,"ClienId" 
        atk_8266_send_cmd("AT+MQTTCLIENTID=0,\"485519efE6DD|securemode=3\\,signmethod=hmacsha1\\,timestamp=789|\"","",50);         
        printf("���ӿͻ���...\r\n");
    
        //�������ǵ��豸��ע�⽫�������:1883�˿ں�ȥ������Ϊ���ᵽ������
        //AT+MQTTCONN=0,"��������",1883,1
        atk_8266_send_cmd("AT+MQTTCONN=0,\"a1fmOzBOIDW.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1","",50);        
        printf("���Ӱ����Ʒ�����...\r\n");
        
        //����ָ�� AT+MQTTSUB=0,"topic",1
        //AT+MQTTSUB=0,"/sys/${ProductKey}/${deviceName}/thing/service/property/set",1
        atk_8266_send_cmd("AT+MQTTSUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/service/property/set\",1","",50);    
        printf("�ͻ��˶�����Ϣ...\r\n");
        

//        atk_8266_send_cmd("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\"method\":\"thing.service.property.set\"\,\"id\":\"2012934116\"\,\"params\":{\"temperature\":52.5}\,\"version\":\"1.0.0\"}\",1,0","",50);
        return 0;        
}

//�����ϴ��ƶ�   ����json��ʽ
u8 Data_Post_To_Cloud(char *str,float temp,u8 *ack,u16 waittime)
{
    u8 res=0; 
    USART3_RX_STA=0;

    u3_printf("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"1012934116\\\"\\,\\\"params\\\":");    //��������
    u3_printf("{\\\"%s\\\":%f",str,temp);    //��ģ��  �¶�
	//u3_printf("{\\\"temperature\\\":%f",temperature);    //��ģ��  �¶�
//    u3_printf("\\,\\\"Humidity\\\":%f",temperature);    //��ģ��  ʪ��
    u3_printf("}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n");    //��������
    
    printf("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"2012934116\\\"\\,\\\"params\\\":{\\\"%s\\\":%f}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n",str,temp);    //��������
//    printf("AT+MQTTSUB=0,\"/sys/a18ZTDYMXDe/Dev_1/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"2012934115\\\"\\,\\\"params\\\":");    //��������
//    printf("{\\\"temperature\\\":%f",a);    //��������
//    printf("}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0");    //��������
//    printf("\r\n");    //��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
	
}


//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
//u8 netpro=0;	//����ģʽ
//u8 atk_8266_wifista_test(void)
//{
//	//u8 netpro=0;	//����ģʽ
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP����
//	u8 *p;
//	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//����״̬
//	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
//	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
//	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP�������ر�(��APģʽ��Ч) 
//	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
//	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
//	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//����Ŀ��·����,���һ��IP
//PRESTA:
//	netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//ѡ������ģʽ
//	if(netpro&0X02)   //UDP
//	{
//				LCD_Clear(WHITE);
//				POINT_COLOR=RED;
//				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
//				Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
//				if(atk_8266_ip_set("WIFI-STA Զ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
//				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
//				delay_ms(200);
//				atk_8266_send_cmd("AT+CIPMUX=0","OK",20);  //������ģʽ
//				delay_ms(200);
//				LCD_Clear(WHITE);
//				while(atk_8266_send_cmd(p,"OK",500));
//	}
//	else     //TCP
//	{
//		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
//		{
//			LCD_Clear(WHITE);
//			POINT_COLOR=RED;
//			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
//			Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
//			if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
//			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
//			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
//			while(atk_8266_send_cmd(p,"OK",200))
//			{
//					LCD_Clear(WHITE);
//					POINT_COLOR=RED;
//					Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
//					Show_Str_Mid(0,80,"ATK-ESP ����TCP Serverʧ��",12,240); //����ʧ��	
//					key=KEY_Scan(0);
//					if(key==WKUP_PRES)goto PRESTA;
//			}	
//			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��			
//		}
//		else					//TCP Server
//		{
//				LCD_Clear(WHITE);
//				POINT_COLOR=RED;
//				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240); 
//				Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
//				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
//				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
//				atk_8266_send_cmd(p,"OK",50);    
//		}
//	}
//			LCD_Clear(WHITE);
//			POINT_COLOR=RED;
//			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
//			Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);			
//			LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
//			Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
//			LCD_Fill(30,80,239,80+12,WHITE);
//			atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
//			sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
//			Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�	
//			Show_Str(30,80,200,12,"״̬:",12,0); 		//����״̬
//			Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
//			Show_Str(30,100,200,12,"��������:",12,0); 	//��������
//			Show_Str(30,115,200,12,"��������:",12,0);	//��������
//			atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
//			POINT_COLOR=BLUE;
//			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
//			USART3_RX_STA=0;
//			while(1)
//			{
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)			//WK_UP �˳�����		 
//				{ 
//					res=0;					
//					atk_8266_quit_trans();	//�˳�͸��
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
//					break;												 
//				}
//				else if(key==KEY0_PRES)	//KEY0 �������� 
//				{
//				
//					if((netpro==3)||(netpro==2))   //UDP
//					{
//						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//						timex=100;
//					}
//					else if((netpro==1))   //TCP Client
//					{
//						atk_8266_quit_trans();
//						atk_8266_send_cmd("AT+CIPSEND","OK",20);         //��ʼ͸��           
//						sprintf((char*)p,"ATK-8266%s����%d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						u3_printf("%s",p);
//						timex=100;
//					}
//					else    //TCP Server
//					{
//						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//						timex=100;
//					}
//				}else;
//			
//				if(timex)timex--;
//				if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//				t++;
//				delay_ms(10);
//				if(USART3_RX_STA&0X8000)		//���յ�һ��������
//				{ 
//					rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
//					USART3_RX_BUF[rlen]=0;		//��ӽ����� 
//					printf("%s",USART3_RX_BUF);	//���͵�����   
//					sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 
//					LCD_Fill(30+54,115,239,130,WHITE);
//					POINT_COLOR=BRED;
//					Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
//					POINT_COLOR=BLUE;
//					LCD_Fill(30,130,239,319,WHITE);
//					Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//��ʾ���յ�������  
//					USART3_RX_STA=0;
//					if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
//					else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
//				}  
//				if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
//				{
////			//		LCD_Fill(30+54,125,239,130,WHITE);
////					LCD_Fill(60,80,120,92,WHITE);
//					constate=atk_8266_consta_check();//�õ�����״̬
//					if(constate=='+')Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
//					else Show_Str(30+30,80,200,12,"����ʧ��",12,0); 	 
//					t=0;
//				}
//				if((t%20)==0)LED0=!LED0;
//				atk_8266_at_response(1);
//			}
//	myfree(SRAMIN,p);		//�ͷ��ڴ� 
//	return res;		
//} 




























