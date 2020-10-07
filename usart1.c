/************************************
* UART1 AND UART3
* DMA FOR SEND AND RECIVE IS ENABLE
* GPS PROCESS USED IN THIS PAGE
* AUTHOR:MAZIAR.SOLEIMANI

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "string.h"
#include <stdlib.h>
#include "misc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_dma.h"
#include <nmea0183.h>

#define res        IWDG->KR  = 0xAAAA;

__IO u16 fla; 
char  usart_buffer[700]  ;
char  usart_buffer1[700]  ;
char  usart_buffer3[700]  ;


extern __IO u16 ADC_ConvertedValue;	 
//int a1=1,a2=1,a3=1,a4=1;
char u;
	char f;
	//u16 lati=0;

	char *pucBuffer;
	char rep=0;
	char oti[13]="5102.84352";
	u8 *buf;
//vu16 GPSMSG_FLAG = 0;							
//vu16 GPSMSG_INDEX = 0;
//vu8 GPS_DEBUG = 0; 
u32 RBKP;
u32 RBKP1;
   
	u32 temp;	   
	float rs;  
	u32 bk1,bk2,bk3,bk4,bk5,bk6,bk7,bk8,bk9;	 
	char T=1;
	char lo=1;
	int i=0;
	int ii=0;
	signed int Y=0;
  char RxBuffer[700];
	char BUF[200];
  char	string[20] ;
	 
int comp[3];
double d14;
void Delay1(unsigned long time)
{unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}

/*************************************
 * AIS.c
 * CLASS A TRANMISSION       
 * navbarsazan moj nilogon jonob .co
 *  

**********************************************************************************/

void TIM4_init(void) {
	// TIMER4
    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  	TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 45000;
    TIMER_InitStructure.TIM_Period =600 ;
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    /* NVIC Configuration */
    /* Enable the TIM4_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
                            /* Start the watchdog       */
void IWDG_Init (void) {

  IWDG->KR  = 0x5555;                             /* enable write to PR, RLR  */
  IWDG->PR  = 5;                            /* Init prescaler           */
  IWDG->RLR = 0Xfff;                           /* Init RLR                 */
  IWDG->KR  = 0xAAAA;                             /* Reload the watchdog      */
  IWDG->KR  = 0xCCCC;                             /* Start the watchdog       */
                          /* Start the watchdog       */
}

/*void TIM4_IRQHandler(void)
{

	res;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		comp[ii]=ADC_ConvertedValue;
			
	 ii+=1;
		
		if(ii==3){
	  ii=0;
 
      if((800>=comp[0]) & (800>=comp[1]) & (800>=comp[2]) & (a1==1)  ){
		        printf("4800=%d",ADC_ConvertedValue);
					 a1=0;
					 a2=1;
					 a3=1;
					 a4=1;
			     BAUD_RATE(USART3,9600);
				}
	    else if((1100<=comp[0]) & (1900>=comp[0]) & (1100<=comp[1]) & (1900>=comp[1]) & (1100<=comp[2]) & (1900>=comp[2])& (a2==1)){
					//printf("9600=%d",ADC_ConvertedValue);
					 a1=1;
					 a2=0;
					 a3=1;
					 a4=1;
					 BAUD_RATE(USART3,38400);
							
				}
		 else if ((2200<=comp[0]) & (3000>=comp[0]) & (2200<=comp[1]) & (3000>=comp[1]) & (2200<=comp[2]) & (3000>=comp[2]) & (a3==1)){
					 a1=1;
					 a2=1;
					 a3=0;
					 a4=1;
					//printf("19200=%d",ADC_ConvertedValue);
							 //USART3->BRR   = 0x0271;   
							// USART2_Config(115200);
			 BAUD_RATE(USART3,19200);

					 
				}	
		 else if ((3300<=comp[0]) & (4095>=comp[0]) &(3300<=comp[1]) & (4095>=comp[1]) & (3300<=comp[2]) & (4095>=comp[2]) & (a4==1)){
					 a1=1;
					 a2=1;
					 a3=1;
					 a4=0;
						//USART2_Config(9600);
			     BAUD_RATE(USART3,4800);
					//printf("38400=%d",ADC_ConvertedValue);
				}	 

}



	
  TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
	*/	


	void BAUD_RATE(USART_TypeDef* USARTx,int baudrate)
{

  uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;
  uint32_t usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;
RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  /* Determine the integer part */
  integerdivider = ((0x19 * apbclock) / (0x04 * (baudrate)));
  tmpreg = (integerdivider / 0x64) << 0x04;
  /* Determine the fractional part */
  fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
  tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & ((uint8_t)0x0F);
  /* Write to USART BRR */
  USARTx->BRR = (uint16_t)tmpreg;
}
void USART1_Config(int BAUD)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4 ,ENABLE); 

	/* USART1 使用IO端口配置 */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);   //初始化GPIOA 
 	USART_InitStructure.USART_BaudRate = BAUD;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	//USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(UART4, &USART_InitStructure);  //初始化USART1

  /* USARTy TX DMA1 Channel (triggered by USARTy Tx event) Config */
	DMA_DeInit(DMA2_Channel3);
  DMA_InitStructure.DMA_BufferSize = 700;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  /* USARTy RX DMA1 Channel (triggered by USARTy Rx event) Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&UART4->DR;//0x40013804;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//DMA_ITConfig(DMA1_Channel5, DMA_IT_HT, ENABLE);
	//NVIC_EnableIRQ(DMA1_Channel5_IRQn);
   USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	 //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   NVIC_EnableIRQ(UART4_IRQn);
   USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	 
	
  DMA_Cmd(DMA2_Channel3, ENABLE);
  USART_Cmd(UART4, ENABLE);	
}
void DMA2_Channel3_IRQHandler(void)
{
	 // DMA_Cmd(DMA1_Channel5, DISABLE);

	// f=USART1->DR;
   //USART1->SR&=~USART_SR_RXNE;
	//printf("MAZIAR");
		 DMA_ClearITPendingBit(DMA2_IT_TC3);

  // printf("%c fvfv fdcvfdxc",RxBuffer[0]);
	
	fla=1;
	 //DMA_ITConfig(DMA1_Channel5, DMA_IT_HT, DISABLE);
	// NVIC_DisableIRQ(DMA1_Channel5_IRQn);
}
void UART4_IRQHandler(void)
{
	volatile unsigned int IIR;

 	    IIR = UART4->SR;
//	printf("    %x     ",USART1_SR);
    if (IIR & USART_SR_IDLE) { 
   DMA_Cmd(DMA2_Channel3, DISABLE);
			if(fla==1){
				// USARTSendDMA(RxBuffer); 
				  fla=0;
			}
   DMA_ClearITPendingBit(DMA2_IT_TC3);
 	 UART4->SR&=~USART_SR_IDLE;
	 u=UART4->SR;
   f=UART4->DR;
	  //strcpy(usart_buffer1 ,RxBuffer); 
     			//strcpy(RxBuffer ,usart_buffer1);
    if(T==1 & RxBuffer[7]!=',' & RxBuffer[1]=='G' & RxBuffer[5]=='C' & RxBuffer[8]!='V' &  RxBuffer[17]!='V'){
			T=2;
			 strcpy(usart_buffer1 ,RxBuffer); 
 
		// printf("MOHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHEEN");
			 get_GPS_datA();
		} 
		
 	  DMA2_Channel3->CNDTR=700 ;
	  DMA_Cmd(DMA2_Channel3, ENABLE);
    memset(RxBuffer,0,strlen(RxBuffer));
		
			
	//USART_Cmd(USART1, DISABLE);// USART1使能
	//USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
	  //NVIC_DisableIRQ(USART1_IRQn );
		}
	}
 void DMASEND(void){
	DMA_InitTypeDef DMA_InitStruct;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)&usart_buffer ;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize = 700;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStruct);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
 }
void USART2_Config(int Baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	
 	USART_DeInit(USART1); 
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1 NVIC 

    //NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	//NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(USART1, &USART_InitStructure); 
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE); ;
	/* Enable the USART1 Receive interrupt: this interrupt is generated when the
	USART1 receive data register is not empty */
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}
 void USARTSendDMA(char *pucBuffer)
{
	//printf("ppp=%d",strlen(pucBuffer));
	strcpy(usart_buffer, pucBuffer);
 	/* Restart DMA Channel*/
	 DMA_Cmd(DMA1_Channel4, DISABLE);
	 DMA1_Channel4->CNDTR = strlen(pucBuffer);
	 DMA_Cmd(DMA1_Channel4, ENABLE);
	
	
}




void DMA1_Channel4_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	//DMA_Cmd(DMA1_Channel2, DISABLE);
	memset(usart_buffer,0,strlen(usart_buffer));
	fla=1;
	//p1=(u8*)strstr((const char *)usart_buffer,"GPRMC");
	//printf("%s",p1);
	
}
 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

int SEND3(int ch, FILE *f)
{
   USART_SendData(USART3, (unsigned char) ch);
  while (!(USART3->SR & USART_FLAG_TXE));
 
  return (ch);
}
int SER_GetChar1 (void) {

  while (!(USART1->SR & 0x0020));
  return (USART1->DR);
 
}




/*-----------------------------------------------------------------------------
 *       SER_PutChar:  Write a character to Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_PutChar (int32_t ch) {
  USART_SendData(USART3, (unsigned char) ch);
  while (!(USART3->SR & USART_FLAG_TXE));
 
  
 
  return (ch);
}


/*-----------------------------------------------------------------------------
 *       SER_GetChar:  Read a character from Serial Port
 *----------------------------------------------------------------------------*/
int32_t SER_GetChar (void) {
#ifdef __DBG_ITM
  if (ITM_CheckChar())
    return ITM_ReceiveChar();
#else
  if (USART1->SR & 0x0020)
    return (USART1->DR);
#endif
  return (-1);
}

void UART0_SendString2 (unsigned char c[]) 
{
	 uint16_t i = 0;
	 while (c[i] != 0) 
	 {
   		SER_PutChar(c[i]);
		  i++;
	 }
}

 
/*******************************************************************/

void UART2_SendString (unsigned char *s) 
{
  	while (*s != 0) 
	{
   		SER_PutChar(*s++);
	}
}
  void wdtg(void){
	/*  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
		PWR_BackupAccessCmd(ENABLE);

		 		bk1=BKP_ReadBackupRegister(BKP_DR1);
		    bk2=BKP_ReadBackupRegister(BKP_DR2);
        bk3=BKP_ReadBackupRegister(BKP_DR3);
        bk4=BKP_ReadBackupRegister(BKP_DR4);
			  bk5=BKP_ReadBackupRegister(BKP_DR5);
        bk6=BKP_ReadBackupRegister(BKP_DR6);
	      bk7=BKP_ReadBackupRegister(BKP_DR7);
			  bk8=BKP_ReadBackupRegister(BKP_DR8);
	      bk9=BKP_ReadBackupRegister(BKP_DR9);
	  //BKP_WriteBackupRegister(BKP_DR10, RBKP+1);
		

	      	//BKP_DeInit();

          Delay1(100);
		    if(bk1==0x00  | bk1==0x2c ){
					printf("No ANTENNA");
				}
				else  {
						      printf("first=hour:%c%c min:%c%c  day:%c%c month:%c%c",bk1 
			&0xff,bk1>>8&0xff,bk2 &0xff,bk2>>8&0xff,bk3 &0xff,bk3>>8&0xff,bk4 &0xff,bk4>>8&0xff);	
				
        printf("last=hour:%c%c min:%c%c  day:%c%c month:%c%c   rep=%d "
					,bk5 &0xff,bk5>>8&0xff,bk6 &0xff,bk6>>8&0xff,bk7 &0xff,bk7>>8&0xff,bk8&0xff,bk8>>8&0xff,bk9&0xff);
				 	       
				}
	
	
	*/
}

 void Uart_Init(u32 Baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
 	USART_DeInit(UART4); 
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

    //Usart1 NVIC 

    //NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	//NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(UART4, &USART_InitStructure); 
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE); ;
}
int SER_GetChar11 (void) {

  while (!(UART4->SR & 0x0020));
  return (UART4->DR);
 
}
