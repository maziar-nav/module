/*
 * si4463.h
 *
 *  Created on: 29 oct 2017
 *  Author: Rekawekp
 */


#ifndef SPI_H_
#define SPI_H_
#include "stm32f10x.h"
void PeripheralInit_SPI2_Master(void);
void PeripheralInit_GPIO(void);
uint8_t SPI_send(uint8_t outByte);
void deselect_Spi(void);
void select_Spi(void);
void PeripheralInit_SPI1_Master(void);
	

#endif /* SI4463_H_ */
