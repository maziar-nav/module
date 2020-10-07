/*****************************************************************************
 * main.c
 * CLASS A AIS TRANMISSION       
 * navbarsazan moj nilogon jonob .co
 * R&D : navbarsazan moj nilogon jonob Team			 
 * Author:maziar soleimani

*****************************************************************************/

#include "stm32f10x.h"
#include "stdio.h"
#include "usart1.h"
#include "delay.h"
#include "string.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "spi.h"
#include "si4463.h"
#include <stdlib.h>
#include "radio_config_Si4463.h"
#include <AIS.h>
#include <nmea0183.h>
#include "stm32f10x_exti.h"
extern uint8_t message1A[32];
extern uint8_t message5A[32];
extern char message5_1_ais[80];
extern char message1_ais[80];
void EXTI4_IRQHandler(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);

GPIO_InitTypeDef  GPIO_InitStructure;
uint8_t buff_tx3[32]={0xAA,0xCC,0xCC,0xCC,0xFE,0x95,0x29,0x55,0x89,0x52,0xAA,0x75,0x46,0x5E,0x91,0x11,0x2D,0xEA,0x37,0x8E,0x2C,0xF3,0x3D,0xAD,0x56,0x31,0x8B,0x70,0xFE,0xAA,0xAA,0xAA};
uint8_t d9[256];
	
void NVIC_Configuration(void){
   NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure one bit for preemption priority */
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	//KEY0
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI_Configuration(void)
{
   EXTI_InitTypeDef EXTI_InitStructure;

   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource2);

   EXTI_InitStructure.EXTI_Line = EXTI_Line2;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

}

 void EXTI2_IRQHandler(void)
{
   if(EXTI->PR & (1UL << 2))
   {
      if(EXTI_GetITStatus(EXTI_Line2) != RESET) 
      {
         EXTI_ClearITPendingBit(EXTI_Line2);
 
      }
   }
	 	while(1){
		 GPIOB->ODR |=  (1UL << (8));   
	}
}	
int main(void)
{
	  USART2_Config(9600);
    USART1_Config(9600);	
	  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );
	/* Configure PC0 in output pushpull mode */

   PeripheralInit_SPI2_Master();
 //PeripheralInit_SPI1_Master();
	 PeripheralInit_GPIO();
	 NVIC_Configuration();
	 EXTI_Configuration();
	 DMASEND();
   SI4463_Init();
   SI4463_Clear_TX_FIFO();
   message1_classA(d9,168);
   message5_classA(d9,424);

	while (1)
	{
	//	printf("Set Pin\r\n");
		
		/* Set PC0 */
		//GPIOC->BSRR = GPIO_Pin_0;
		    SI4463_Send_Data(message1A,32,0);
        delay_ms(500);
		  SI4463_Send_Data(message5A,64,0);
    	   delay_ms(500);
    // USARTSendDMA(message5_1_ais);
		//printf("Reset Pin\r\n");
		
		/* Reset PC0 */
		//GPIOC->BRR = GPIO_Pin_0;
		//delay_ms(500);	
	}
}
