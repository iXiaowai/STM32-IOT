#include "common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� ������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//�û�������

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="ALIENTEK";			//·����SSID��
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="15902020353"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid="ATK-ESP8266";			//����SSID��
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password="12345678"; 		//�������� 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART3_RX_STA;
//     1,����USART3_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART3_RX_BUF);	//���͵�����
		if(mode)USART3_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u16 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u16 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
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
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(atk_8266_check_cmd("+CIPSTATUS:0")&&
		 atk_8266_check_cmd("+CIPSTATUS:1")&&
		 atk_8266_check_cmd("+CIPSTATUS:2")&&
		 atk_8266_check_cmd("+CIPSTATUS:4"))
		return 0;
	else return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd("AT+CIPSTATUS",":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd("+CIPSTATUS:"); 
	res=*p;									//�õ�����״̬	
	return res;
}
//�������
const u8* kbd_tbl[13]={"1","2","3","4","5","6","7","8","9",".","0","#","DEL"}; 
u8* kbd_fn_tbl[2];
//���ؼ��̽��棨�ߴ�Ϊ240*140��
//x,y:������ʼ���꣨320*240�ֱ��ʵ�ʱ��x����Ϊ0��
//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}

//��ȡAP+STA ip��ַ����ָ��λ����ʾ
//ipbuf:ip��ַ���������
void atk_8266_get_ip(u8 x,u8 y)
{
		u8 *p;
		u8 *p1;
		u8 *p2;
		u8 *ipbuf;
		u8 *buf;
		p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	  ipbuf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		buf=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{ 
			*ipbuf=0;
		}
		else
		{		
			p=atk_8266_check_cmd("APIP,\"");
			p1=(u8*)strstr((const char*)(p+6),"\"");
			p2=p1;
			*p1=0;
			ipbuf=p+6;
			sprintf((char*)buf,"AP IP:%s �˿�:%s",ipbuf,(u8*)portnum);
			p=(u8*)strstr((const char*)(p2+1),"STAIP,\"");
			p1=(u8*)strstr((const char*)(p+7),"\"");
			*p1=0;
			ipbuf=p+7;
			sprintf((char*)buf,"STA IP:%s �˿�:%s",ipbuf,(u8*)portnum);

			myfree(SRAMIN,p);		//�ͷ��ڴ�
			myfree(SRAMIN,p1);		//�ͷ��ڴ�
			myfree(SRAMIN,p2);		//�ͷ��ڴ�
			myfree(SRAMIN,ipbuf);		//�ͷ��ڴ�
			myfree(SRAMIN,buf);		//�ͷ��ڴ�
		}
}

//ATK-ESP8266ģ����Ϣ��ʾ
//x,y:��ʾ��Ϣ����ʼ����.
//wanip:0,ȫ��������ʾ;1,������wanip.
void atk_8266_msg_show(u16 x,u16 y,u8 wanip)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
	atk_8266_send_cmd("AT+RST","OK",20);
	delay_ms(1000);//��ʱ2s�ȴ�ģ������
	delay_ms(1000);//
	delay_ms(1000);
	delay_ms(1000);
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	if(wanip==0)//ȫ����
	{
		atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
		p=atk_8266_check_cmd("SDK version:");
		p1=(u8*)strstr((const char*)(p+12),"(");
		*p1=0;
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//��ȡ����ģʽ
		p=atk_8266_check_cmd(":");
  	atk_8266_send_cmd("AT+CWSAP?","+CWSAP:",20);	//��ȡwifi����
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
	}
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	myfree(SRAMIN,p1);		//�ͷ��ڴ� 
	myfree(SRAMIN,p2);		//�ͷ��ڴ� 
}
//����ģʽѡ��
//����ֵ:
//0,TCP������
//1,TCP�ͻ���
//2,UDPģʽ
u8 atk_8266_netpro_sel(u16 x,u16 y,u8* name) 
{
	u8 key=0,t=0,*p;
	u8 netpro=0;
	p=mymalloc(SRAMIN,50);//����50���ֽڵ��ڴ�
	sprintf((char*)p,"%s ����ģʽѡ��",name);
	while(1)
	{
		key=KEY_Scan(0); 
		if(key)
		{
			if(key==WKUP_PRES)break;       //WK_UP����
			if(key==KEY0_PRES)//KEY0����
			{
				if(netpro<2)netpro++;
				else netpro=0;
			}else if(key==KEY1_PRES)//KEY1����
			{
				if(netpro>0)netpro--;
				else netpro=2; 
			}
				
		} 
		delay_ms(10);
		atk_8266_at_response(1);
		if((t++)==20){t=0;LED0=!LED0;}//LED��˸
	} 
	myfree(SRAMIN,p);
	return netpro;//��������ģʽѡ��ֵ 
} 

//STAģʽ�µ�AP��TCP��UDP����ģʽ����
u8 atk_8266_mode_cofig(u8 netpro)
{
	//u8 netpro;
	u8 ipbuf[16]; 	//IP����
	u8 *p;
	u8 key;
	p=mymalloc(SRAMIN,32);//����32���ֽڵ��ڴ�
PRESTA:		
	netpro|=(atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]))<<4;	//����ģʽѡ��
	if(netpro&0X20)
	{
//		LCD_Clear(WHITE);
		if(atk_8266_ip_set("WIFI-APԶ��UDP IP����","UDPģʽ",(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
		else sprintf((char*)p,"AT+CIPSTART=0,\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������,��ID�ţ�STAģʽ��Ϊ0
		delay_ms(200);
//		LCD_Clear(WHITE);
		atk_8266_send_cmd(p,"OK",200);
	}
	else if(netpro&0X10)     //AP TCP Client    ͸��ģʽ����
	{
		if(atk_8266_ip_set("WIFI-AP Զ��IP����","TCP Client",(u8*)portnum,ipbuf))goto PRESTA;	//IP����
		if(netpro&0X03)sprintf((char*)p,"AT+CIPSTART=1,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪclientʱ��Ϊ1
		else sprintf((char*)p,"AT+CIPSTART=0,\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,��ID�ţ�STAģʽΪserverʱ��Ϊ0
		while(atk_8266_send_cmd(p,"OK",200))
			{ 
				key=KEY_Scan(0);
				if(key==WKUP_PRES)goto PRESTA;
			}				
	}
	else;   //������ģʽ��������
	myfree(SRAMIN,p);
	return netpro;
}





//IP����
//title:ip���ñ���
//mode:����ģʽ
//port:�˿ں�
//*ip:ip������(����IP���ϲ㺯��)
//����ֵ:0,ȷ������;1,ȡ������.
u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip) 
{
	u8 res=0;
 	u8 key;
	u8 timex=0;  
	u8 iplen=0;			//IP���� 

	kbd_fn_tbl[0]="����";
	kbd_fn_tbl[1]="����"; 
	ip[0]=0; 		
	while(1)
	{ 
//		key=atk_8266_get_keynum(0,180);
		if(key)
		{   
			if(key<12)
			{
				if(iplen<15)
				{ 
					ip[iplen++]=kbd_tbl[key-1][0];
				}
			}else
			{
				if(key==13)if(iplen)iplen--;	//ɾ��  
				if(key==14&&iplen)break; 		//ȷ������ 
				if(key==15){res=1;break;}		//ȡ������
			} 
			ip[iplen]=0; 
		} 
		timex++;
		if(timex==20)
		{
			timex=0;
			LED0=!LED0;
		}
		delay_ms(10);
		atk_8266_at_response(1);//WIFIģ�鷢����������,��ʱ�ϴ�������
	} 
	return res;
}
//���Խ�����UI
void atk_8266_mtest_ui(u16 x,u16 y)
{ 
 	atk_8266_msg_show(x,y+125,0);
}


















































