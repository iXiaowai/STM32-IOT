#include "common.h"
#include "stdlib.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序主要用于向云端上传数据
//主要用到WIFi.c，和common.c
								  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//ATK-ESP8266 WIFI STA 连接阿里云


/*
三元组
    "product_key": "a1fmOzBOIDW",
    "device_name": "Temperature01",
    "device_secret": "7c1df343a7bd95ec2427cdb460925588"
	参数
	{"clientId":"a1fmOzBOIDW.Temperature01|securemode=2,signmethod=hmacsha256,timestamp=1719973767245|",
	"username":"Temperature01&a1fmOzBOIDW",
	"mqttHostUrl":"a1fmOzBOIDW.iot-as-mqtt.cn-shanghai.aliyuncs.com",
	"passwd":"e8f091cefc101dbb736fa2187403805113e3031a4da1ea4e6790cb1430997963",
	"port":1883}
	
*/



u8 atk_8266_wifi_MQTT_Init(void)
{
//        u8 *p;
        atk_8266_send_cmd("AT+CWMODE=1","OK",20);        //设置WIFI STA模式
        //atk_8266_send_cmd("AT+RST","OK",20);        //复位
        delay_ms(1000);  //延时2s，等待重启成功
        delay_ms(1000);
        delay_ms(1000);
        delay_ms(1000);
        atk_8266_send_cmd("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"","OK",50);        //设置时域和 SNTP 服务器
        //sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
		while(atk_8266_send_cmd("AT+CWJAP=\"Xiaomi14\",\"12349876\"","OK",50));        //连接WiFi，输入WiFi和密码
        printf("wifi网络连接成功....\r\n");
        
        //AT+MQTTUSERCFG=0,1,"NULL","用户名","密码",0,0,""
        //连接域名、Clientld、Username、Password由阿里云Aliyun配置工具获得
        atk_8266_send_cmd("AT+MQTTUSERCFG=0,1,\"NULL\",\"Temperature01&a1fmOzBOIDW\",\"370f57b1a1435439c9e96a8178c105686681e65d\",0,0,\"\"","",50);        
        printf("用户设备连接...\r\n");
    
        //使用保存的clientId参数;  clientId参数中注意每个逗号前面加分隔符“\”！！！！    
        //AT+MQTTCLIENTID=0,"ClienId" 
        atk_8266_send_cmd("AT+MQTTCLIENTID=0,\"485519efE6DD|securemode=3\\,signmethod=hmacsha1\\,timestamp=789|\"","",50);         
        printf("连接客户端...\r\n");
    
        //连接我们的设备，注意将域名后的:1883端口号去掉，因为被提到后面了
        //AT+MQTTCONN=0,"连接域名",1883,1
        atk_8266_send_cmd("AT+MQTTCONN=0,\"a1fmOzBOIDW.iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883,1","",50);        
        printf("连接阿里云服务器...\r\n");
        
        //订阅指令 AT+MQTTSUB=0,"topic",1
        //AT+MQTTSUB=0,"/sys/${ProductKey}/${deviceName}/thing/service/property/set",1
        atk_8266_send_cmd("AT+MQTTSUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/service/property/set\",1","",50);    
        printf("客户端订阅消息...\r\n");
        

//        atk_8266_send_cmd("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\"method\":\"thing.service.property.set\"\,\"id\":\"2012934116\"\,\"params\":{\"temperature\":52.5}\,\"version\":\"1.0.0\"}\",1,0","",50);
        return 0;        
}

//数据上传云端   采用json格式
u8 Data_Post_To_Cloud(char *str,float temp,u8 *ack,u16 waittime)
{
    u8 res=0; 
    USART3_RX_STA=0;

    u3_printf("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"1012934116\\\"\\,\\\"params\\\":");    //发送命令
    u3_printf("{\\\"%s\\\":%f",str,temp);    //物模型  温度
	//u3_printf("{\\\"temperature\\\":%f",temperature);    //物模型  温度
//    u3_printf("\\,\\\"Humidity\\\":%f",temperature);    //物模型  湿度
    u3_printf("}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n");    //发送命令
    
    printf("AT+MQTTPUB=0,\"/sys/a1fmOzBOIDW/Temperature01/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"2012934116\\\"\\,\\\"params\\\":{\\\"%s\\\":%f}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0\r\n",str,temp);    //发送命令
//    printf("AT+MQTTSUB=0,\"/sys/a18ZTDYMXDe/Dev_1/thing/event/property/post\",\"{\\\"method\\\":\\\"thing.service.property.set\\\"\\,\\\"id\\\":\\\"2012934115\\\"\\,\\\"params\\\":");    //发送命令
//    printf("{\\\"temperature\\\":%f",a);    //发送命令
//    printf("}\\,\\\"version\\\":\\\"1.0.0\\\"}\",1,0");    //发送命令
//    printf("\r\n");    //发送命令
	
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
	
}


//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
//u8 netpro=0;	//网络模式
//u8 atk_8266_wifista_test(void)
//{
//	//u8 netpro=0;	//网络模式
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP缓存
//	u8 *p;
//	u16 t=999;		//加速第一次获取链接状态
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//连接状态
//	p=mymalloc(SRAMIN,32);							//申请32字节内存
//	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
//	atk_8266_send_cmd("AT+RST","OK",20);		//DHCP服务器关闭(仅AP模式有效) 
//	delay_ms(1000);         //延时3S等待重启成功
//	delay_ms(1000);
//	delay_ms(1000);
//	delay_ms(1000);
//	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
//	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
//	while(atk_8266_send_cmd(p,"WIFI GOT IP",300));					//连接目标路由器,并且获得IP
//PRESTA:
//	netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式
//	if(netpro&0X02)   //UDP
//	{
//				LCD_Clear(WHITE);
//				POINT_COLOR=RED;
//				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240); 
//				Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
//				if(atk_8266_ip_set("WIFI-STA 远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
//				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器
//				delay_ms(200);
//				atk_8266_send_cmd("AT+CIPMUX=0","OK",20);  //单链接模式
//				delay_ms(200);
//				LCD_Clear(WHITE);
//				while(atk_8266_send_cmd(p,"OK",500));
//	}
//	else     //TCP
//	{
//		if(netpro&0X01)     //TCP Client    透传模式测试
//		{
//			LCD_Clear(WHITE);
//			POINT_COLOR=RED;
//			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240); 
//			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
//			if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
//			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
//			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
//			while(atk_8266_send_cmd(p,"OK",200))
//			{
//					LCD_Clear(WHITE);
//					POINT_COLOR=RED;
//					Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
//					Show_Str_Mid(0,80,"ATK-ESP 连接TCP Server失败",12,240); //连接失败	
//					key=KEY_Scan(0);
//					if(key==WKUP_PRES)goto PRESTA;
//			}	
//			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传			
//		}
//		else					//TCP Server
//		{
//				LCD_Clear(WHITE);
//				POINT_COLOR=RED;
//				Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240); 
//				Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
//				atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
//				sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
//				atk_8266_send_cmd(p,"OK",50);    
//		}
//	}
//			LCD_Clear(WHITE);
//			POINT_COLOR=RED;
//			Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
//			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
//			LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
//			Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
//			LCD_Fill(30,80,239,80+12,WHITE);
//			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
//			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
//			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
//			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
//			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
//			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
//			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
//			atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
//			POINT_COLOR=BLUE;
//			Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
//			USART3_RX_STA=0;
//			while(1)
//			{
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)			//WK_UP 退出测试		 
//				{ 
//					res=0;					
//					atk_8266_quit_trans();	//退出透传
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
//					break;												 
//				}
//				else if(key==KEY0_PRES)	//KEY0 发送数据 
//				{
//				
//					if((netpro==3)||(netpro==2))   //UDP
//					{
//						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //发送指定长度的数据
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//						timex=100;
//					}
//					else if((netpro==1))   //TCP Client
//					{
//						atk_8266_quit_trans();
//						atk_8266_send_cmd("AT+CIPSEND","OK",20);         //开始透传           
//						sprintf((char*)p,"ATK-8266%s测试%d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						u3_printf("%s",p);
//						timex=100;
//					}
//					else    //TCP Server
//					{
//						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//						timex=100;
//					}
//				}else;
//			
//				if(timex)timex--;
//				if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//				t++;
//				delay_ms(10);
//				if(USART3_RX_STA&0X8000)		//接收到一次数据了
//				{ 
//					rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
//					USART3_RX_BUF[rlen]=0;		//添加结束符 
//					printf("%s",USART3_RX_BUF);	//发送到串口   
//					sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
//					LCD_Fill(30+54,115,239,130,WHITE);
//					POINT_COLOR=BRED;
//					Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
//					POINT_COLOR=BLUE;
//					LCD_Fill(30,130,239,319,WHITE);
//					Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据  
//					USART3_RX_STA=0;
//					if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//					else t=0;                   //状态为已经连接了,10秒后再检查
//				}  
//				if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
//				{
////			//		LCD_Fill(30+54,125,239,130,WHITE);
////					LCD_Fill(60,80,120,92,WHITE);
//					constate=atk_8266_consta_check();//得到连接状态
//					if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
//					else Show_Str(30+30,80,200,12,"连接失败",12,0); 	 
//					t=0;
//				}
//				if((t%20)==0)LED0=!LED0;
//				atk_8266_at_response(1);
//			}
//	myfree(SRAMIN,p);		//释放内存 
//	return res;		
//} 




























