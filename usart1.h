#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>
/*
 * UART.h
 *
 *  Created on: 5 sep 2020
 *  Author: maziar soliemani
 */



#include "stm32f10x.h"

void bitStuff(uint16_t *buff, uint16_t  size);
void nrzi (uint16_t *bitVector, uint16_t size);
void HDLCFormat(uint16_t *buff, uint16_t size);
//void message5_classA(uint8_t *buff, uint16_t size);
//void message1_classA(uint8_t *buff, uint16_t size);

void  reverseEachByte(uint16_t *bitVector, uint16_t size);
void putBits(uint16_t *bitVector, uint32_t value, uint8_t numBits);


void payloadToBytes(uint16_t *bitVector, uint16_t numBits, uint8_t *byteArray);
void addBits(uint16_t *bitVector, uint16_t size, uint32_t value, uint8_t numBits);
void delay_ms(unsigned int d);
void delay_us(unsigned int d);
uint8_t SI4463_Device_Info( void );

 

void USART2_Config(int BAUD);
void USART1_Config(int BAUD);
int fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
int UART0_Getstring2(void);
int SER_GetChar1 (void);
void usart_dma_init(void);
void USARTSendDMA(char *pucBuffer);	
void BAUD_RATE(USART_TypeDef* USARTx,int baudrate);
void IWDG_Init (void);
void TIM4_init(void);
void timsavwdt(void);
void wdtg(void);	
void DMASEND(void);

#endif /* __USART1_H */
