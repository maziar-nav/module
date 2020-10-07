/*
 * nmea0183.h
 *
 *  Created on: 5 sep 2020
 *  Author: maziar soliemani
 */


#ifndef NMEA0183_H_
#define NMEA0183_H_
#include "stm32f10x.h"
void payloadTosixbit(uint16_t *bitVector, uint16_t numBits,  uint16_t *byteArray);
void CONVERT_MESSAGE1(uint16_t *bitVector, uint32_t value, uint8_t numBits);
void CONVERT_MESSAGE5(uint16_t *bitVector, uint32_t value, uint8_t numBits);
int NMEA_GPRMC_Comma_Pos(u8 *buf,u8 cx);
int NMEA_GPVTG_Comma_Pos(u8 *buf,u8 cx);
void get_GPS_datA(void);
char checksum(const char *s);
void addSTRING(uint16_t *bitVector,char *BUF, uint32_t start_bit, uint8_t numBits);
#endif /* SI4463_H_ */
