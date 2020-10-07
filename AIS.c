/*************************************
 * AIS.c
 * CLASS A TRANMISSION       
 * navbarsazan moj nilogon jonob .co
 * Author:maziar soleimani

**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "stdio.h"   
#include "string.h"
#include <stdlib.h>
#include <AIS.h>
#include <nmea0183.h>
//#define  A  0P00000000
int cts_val=0;
int ac=0;
extern uint16_t sixbitA5[71];
uint8_t sixbitB5[10];
uint8_t message1[32];
uint8_t message1A[32];
uint8_t message5[64];
uint8_t message5A[64];
extern uint16_t sixbitA1[28];
uint16_t d7[512];
extern __IO u16 ADC_ConvertedValue;	
extern __IO u32  lati;
extern __IO u32 lang;
extern double sog;
char name[20]="MAZIARRRRRRRRRRRRRRR";
char callsign[7]="MAZIAR";
uint8_t *inter_buff;
uint8_t buff_tx11[32]={0xaa,0xcc,0xcc,0xcc,0xfe,0x9a,0xa2,0x2c,0xd2,0x9f,0x55,0x55,0x55,0x55,0xa0,0x96,0x08,0x8a,0xbe,0x56,0xd2,0x76,0x95,0xd6,0xf7,0x42,0xba,0xad,0x52,0x69,0x35,0xe5};
uint8_t buff_tx12[32]={0xe9,0x4d,0x69,0x24,0xbe,0x96,0xd7,0x9,0x31,0x4a,0xad,0x52,0xd4,0xb5,0x52,0xaa,0xad,0x2b,0x4a,0xad,0x54,0xf0,0xbf,0x55,0x55,0x55,0x55,0x55,0x7f,0x55,0x55,0x54};
//uint8_t buff_tx3[32]={0xAA,0xCC,0xCC,0xCC,0xFE,0x95,0x29,0x55,0x89,0x52,0xAA,0x75,0x46,0x5E,0x91,0x11,0x2D,0xEA,0x37,0x8E,0x2C,0xF3,0x3D,0xAD,0x56,0x31,0x8B,0x70,0xFE,0xAA,0xAA,0xAA};
//!AIVDM,1,1,,A,16BSWQhP00S`fl2@UOvevOvP20S<,0*3E
	uint16_t rev[512];
	uint8_t messa[100];

static const uint16_t CRC16_XMODEM_TABLE[] =
    { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108,
            0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231,
            0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339,
            0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462,
            0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
            0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653,
            0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b,
            0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4,
            0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc,
            0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5,
            0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd,
            0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6,
            0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae,
            0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
            0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f,
            0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188,
            0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080,
            0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 0x83b9,
            0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1,
            0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea,
            0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2,
            0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db,
            0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
            0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c,
            0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844,
            0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d,
            0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75,
            0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e,
            0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26,
            0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f,
            0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17,
            0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };

	void  reverseEachByte(uint16_t *bitVector, uint16_t size)
{     int j=0;
	 
	int i=0;
    if(size % 8 == 0){
    for (  i = 0; i < size; i += 8 ) {
        for (  j = 0; j < 8; ++j ) {
            rev[i+j]=bitVector[i+7-j];
					 // printf("%c",rev[i+j]);
					  
        }
    }
}	
		for (  i = 0; i < size; ++i ) {
			
			
		}
	}	 
uint16_t crc16(uint8_t *bytes, uint16_t length)
{
    uint16_t crc = 0xffff;
    int b;
    for (  b = 0; b < length; ++b ) {
        crc = ((crc << 8) & 0xff00) ^ CRC16_XMODEM_TABLE[((crc >> 8) & 0xff) ^ bytes[b]];
    }
    return ~crc;
}

void putBits(uint16_t *bitVector, uint32_t value, uint8_t numBits)
{
    // This is used for HDLC framing
    uint16_t pos = 0;
	    int bit=0;
    for (  bit = 0; bit < numBits; ++bit, value >>= 1 )  {
        bitVector[pos++] = value & 0x01;
    }
}


void payloadToBytes(uint16_t *bitVector, uint16_t numBits,  uint8_t *byteArray)
{
	   int i=0;
	uint8_t b=0;
    for (  i = 0; i < numBits; i += 8 ) {
        uint8_t byte = 0;
        for (  b = 0; b < 8; ++b ) {
            byte |= (bitVector[i+b] << (7-b));
        }
        byteArray[i/8] = byte;
				//printf("0x%x,",byteArray[i/8]);
				
    }
}
void addBits(uint16_t *bitVector, uint16_t size, uint32_t value, uint8_t numBits)
{      uint8_t bit=0;
    if(numBits > 0  && numBits <= 32){
    uint16_t pos = size;
    for (  bit = 0; bit < numBits; ++bit, value >>= 1 )  {
        bitVector[pos + numBits-bit-1] = value & 1;
    }

    size += numBits;
}
}

void bitStuff(uint16_t *buff, uint16_t  size)
{
    uint16_t numOnes = 0;
	  int i=0;
    for (  i = 0; i < size; ++i ) {
        switch(buff[i]) {
            case 0:
                numOnes = 0;
                break;
            case 1:
                ++numOnes;
                if ( numOnes == 5 ) {
                    // Insert a 0 right after this one
                    memmove(buff + i + 2, buff + i + 1, 2*(size-i-1));
                    buff[i+1] = 0;
                    ++size;
									  ++ac;
                }
                break;
           
        }
    }
}


void nrzi (uint16_t *bitVector, uint16_t size){
	    int ii=0;
    	uint8_t prevBit = 1;
	    bitVector[0]=prevBit;
	
	    for (  ii = 1; ii < size; ++ii ) {
        if ( bitVector[ii] == 0x00 ) {
            bitVector[ii]=(!prevBit);
            prevBit = !prevBit;
        }
        else {
            bitVector[ii]=(prevBit);
					
        }
			    	//printf("%d",bitVector[ii]);
				
			}
		}



void HDLCFormat(uint16_t *buff, uint16_t size){
	  memmove(rev+40, rev, 2*(size+ac));
    addBits(rev,0, 0x00, 8);  
    addBits(rev+8,0,0x5555557E, 32);//preamble
	  addBits(rev+size+40+ac,0,0x7e, 8);//preamble
	 // addBits(rev+234,size,1, 1);
	  addBits(rev+size+48+ac,0,0, 24-ac);//check
	 //putBits(rev+232,1431655806, 32);//preamble

}
 


 
/*AIS MESSAGE TYPE 5-- 
FOR MORE INFORMATINO VISIT   https://www.navcen.uscg.gov/?pageName=AISMessagesAStatic */
void message1_classA(uint8_t *buff, uint16_t size){
	  size=0;
	  addBits(d7, 0,1, 6);//Message ID
		addBits(d7,6, 0, 2);//Repeat indicator
    addBits(d7,8, 422444444, 30);//mmsi
	  addBits(d7,38, 0,4);//Navigational status
  	addBits(d7,42, 142, 8);//rate of turn
  	addBits(d7,50, (sog*10),10);//SOG
   	addBits(d7,60, 0,1);//position accuracy
    addBits(d7,61, lang,28);//LANG
  	addBits(d7,89, lati,27);//LATI
	  addBits(d7,116,87,12);//cog
	  addBits(d7,128, 0,9);//True heading
  	addBits(d7,137, 0x08208208,6);//Time stamp
	  addBits(d7,143, 0,2);//special maneuvre indicator
    addBits(d7,145, 0x82080,3);//spare
	  addBits(d7+148,148, 0,1);//RAIM-flag
 	  addBits(d7,149, 0,19);//Communication state (see below)
	 // payloadTosixbit(d7,168,sixbitA1);
    //CONVERT_MESSAGE1(d7, 0, 0);
	  reverseEachByte(d7, 168);
	  payloadToBytes(rev, 168,message1A);
  	cts_val=crc16(message1A, 21);
	  addBits(rev+168,size ,cts_val, 16); 
		bitStuff(rev, 184);
		HDLCFormat(rev, 184);
	  nrzi (rev, 256);
	 	payloadToBytes(rev, 256,message1A);
 }
 





 
void message5_classA(uint8_t *buff, uint16_t size){
	   size=0;
	  addBits(d7,   size,5, 6);//Message ID
		addBits(d7+6  , size, 0, 2);//Repeat indicator
    addBits(d7+8  , size, 422444444, 30);//mmsi
	  addBits(d7+38 , size, 0,2);//	AIS version indicator
  	addBits(d7+40 , size, 0x4D0443CE, 30);//IMO number
	  addSTRING(d7,callsign,70,6);//CALL SIGN 
    addSTRING(d7,name,112,6);//name 
	
   	 //addBits(d7+112,size, 0x35224D04,32);//NAME   
	  //addBits(d7+144,size, 0x43CE3418,32);//NAME
	  // addBits(d7+176,size, 0x20820820,32);//NAME
	  //addBits(d7+208,size, 0x820820,24);//NAME
    addBits(d7+232,size, 0,8);//Type of ship and cargo type
  	addBits(d7+240,size, 14342,30);//Overall dimension/reference for position
	  addBits(d7+270,size, 00,4);//Type of electronic position fixing device
	  addBits(d7+274,size, 0,20);//ETA
  	addBits(d7+294,size, 0,8);//Maximum present static draught
	  addBits(d7+302,size, 145865225,120);//Destination
    addBits(d7+422,size, 1,1);//DTE
	  addBits(d7+423,size, 1,1);//spare
	  payloadTosixbit(d7,424,sixbitA5);
	  CONVERT_MESSAGE5(d7, 0, 0);
	  reverseEachByte(d7, 424);
	  payloadToBytes(rev, 424,message5);
  	cts_val=crc16(message5, 53);
	  addBits(rev+424,size ,cts_val, 16); 
		bitStuff(rev, 440);
		HDLCFormat(rev, 440);
	  nrzi (rev, 512);
	 	payloadToBytes(rev, 512,message5A);
 }

 





