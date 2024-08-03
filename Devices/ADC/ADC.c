#include "stdio.h"
#include "ADC.h"




//void delay_10us(uint32_t ms)
//{
//    uint32_t i, j;
//    for (i = 0; i < ms; i++)
//        for (j = 0; j < 2000; j++);
//}

void ADC_GPIO_init(void)
{
    ADC_InitTypeDef ADC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//打开GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//打开ADC1的时钟
 
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;//设置为1号口
    GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AIN;//配置模式为模拟输入
    GPIO_Init(GPIOA,&GPIO_InitStruct);//初始化   
    ADC_DeInit(ADC1);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);//定义时钟分频器
    
    ADC_TempSensorVrefintCmd(ENABLE);//打开传感器
    
    ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//设置为独立模式
    ADC_InitStruct.ADC_ScanConvMode=DISABLE;//设置扫描或单通道  
    ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//设置为单次转换
    ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//选择外部通道
    ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//设置左右对齐的方式
   
    ADC_InitStruct.ADC_NbrOfChannel=1;//选择规则通道1
    ADC_Init(ADC1,&ADC_InitStruct);//初始化
    
    ADC_Cmd(ADC1,ENABLE);//上电
      
    ADC_StartCalibration(ADC1);//开始校准
    while(ADC_GetCalibrationStatus(ADC1));//检测校准的状态  
}
	

//void USART1_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);//打开USART1的时钟
//    
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;//设置为9号口
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//设置为推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//设置为10号口
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//设置为浮空输入
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	USART_InitStructure.USART_BaudRate = 9600;//设置波特率
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8数据位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;//奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART1, &USART_InitStructure);
//    
//	USART_Cmd(USART1, ENABLE);
//}



double read_ADC(uint8_t adc_ch)
{
   ADC_RegularChannelConfig(ADC1, adc_ch, 1, ADC_SampleTime_239Cycles5);
   ADC_Cmd(ADC1,ENABLE); // 启动ADC转换
    
   while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));// 等待ADC转换结束
   return ADC_GetConversionValue(ADC1); // 获取ADC转换结果
}
double GetTemp(void)
{
   double Temp;
   double Vsense;
   Vsense=read_ADC(ADC_Channel_16) * (3.3/4096);
   Temp=(1.43-Vsense) / 0.0043 + 25;
	return Temp;
//   delay_10us(4000);
//   printf("当前温度为：%.3f\n",Temp);
}


//int main()
//{
//   double Temp;
//   double Vsense;
//   USART1_init();
//   ADC_GPIO_init(); 
//   while(1){ 
//       Vsense=read_ADC(ADC_Channel_16) * (3.3/4096);
//       Temp=(1.43-Vsense) / 0.0043 + 25;
//       delay_10us(4000);
//       printf("当前温度为：%.3f\n",Temp);
//   }
//}


