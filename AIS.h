/*
 * AIS.h
 *
 *  Created on: 5 sep 2020
 *  Author: maziar soliemani
 */


#ifndef AIS_H_
#define AIS_H_
#include "stm32f10x.h"

void bitStuff(uint16_t *buff, uint16_t  size);
void nrzi (uint16_t *bitVector, uint16_t size);
void HDLCFormat(uint16_t *buff, uint16_t size);
void message5_classA(uint8_t *buff, uint16_t size);
void message1_classA(uint8_t *buff, uint16_t size);//message1_classA
void  reverseEachByte(uint16_t *bitVector, uint16_t size);
void putBits(uint16_t *bitVector, uint32_t value, uint8_t numBits);
void payloadToBytes(uint16_t *bitVector, uint16_t numBits, uint8_t *byteArray);
void addBits(uint16_t *bitVector, uint16_t size, uint32_t value, uint8_t numBits);
void delay_ms(unsigned int d);
void delay_us(unsigned int d);
uint8_t SI4463_Device_Info( void );

#endif /* SI4463_H_ */
