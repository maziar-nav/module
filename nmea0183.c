/*************************************
 * nmea0183.c
 * CLASS A TRANMISSION       
 * navbarsazan moj nilogon jonob .co
 * Author:maziar soleimani

**********************************************************************************/
 #include "stm32f10x.h"
#include "usart1.h"
#include "stdio.h"   
#include <stdio.h>
#include "string.h"
#include <stdlib.h>
#include <AIS.h>
#include <nmea0183.h>
#include "usart1.h"
#include <math.h>
char lan[11];
char lan1[11];
uint8_t d99[256];
char lan2[11];
char time[12];
char lat[11];
char lat1[5];
char lat2[5];
char lat3[5];
char sog1[5];
u8 *p1,dx;			 
u8 posx;  
double lang2;
char y;
 double  time1;
double lang1;
double lang2;
double lang3;
//int lang;
double lati1;
double lati2;
double lati3;
extern char T;
extern char  usart_buffer1[700];
extern char  RxBuffer[700];

char H[12];
 __IO u32  lati;
  __IO u32  lang;
extern	double sog;
double sog;
//#define  A  0P00000000

uint8_t cnt[64];
char me ;
char me1 ;
extern uint16_t d7[512];
extern __IO u16 ADC_ConvertedValue;	
extern __IO u32  lati;
extern __IO u32 lang;
uint16_t sixbitA1[28];
uint16_t sixbitA5[45];

char message1_ais[48]="!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*";
char message5_1_ais[78]="!AIVDM,2,1,0,A,11111111111111111111111111111111111111111111111111111111,0*";
char message5_2_ais[36]="!AIVDM,2,2,0,A,000000000000000,2*";
static const char nmea0183[] =
{0,'0',1,'1',2,'2',3,'3',4,'4',5,'5',6,'6',7,'7',8,'8',9,'9',10,':',11,';',12,'<',13,'=',14,'>',15,'?',16,'@',17,'A',18,'B',19,'C',20,'D',21,'E',22,'F',23,'G',24,'H',25,'I',26,'J',27,'K',28,'L',29,
		'M',30,'N',31,'O',32,'P',33,'Q',34,'R',35,'S',36,'T',37,'U',38,'V',39,'W',40,'`',41,'a',42,'b',43,'c',44,'d',45,'e',46,'f',47,'g',48,'h',49,'i',50,'j',51,'k',52,'l',53,'m',54,'n',55,'o',56,'p',57,'q',
58,'r',59,'s',60,'t',61,'u',62,'v',63,'w'};

static const char Name_Message5_Ais[] =
{0,'@',1,'A',2,'B',3,'C',4,'D',5,'E',6,'F',7,'G',8,'H',9,'I',10,'J',11,'K',12,'L',13,'M',14,'N',15,'O',16,'P',17,'Q',18,'R',19,'S',20,'T',21,'U',22,'V',23,'W',24,'X',25,'Y',26,'Z',27,'[',28,'/',29,
		']',30,'^',31,'_',32,' ',33,'!',34,'"',35,'#',36,'$',37,'%',38,'&',39,' ',40,'(',41,')',42,'*',43,'+',44,',',45,'-',46,'.',47,'/',48,'0',49,'1',50,'2',51,'3',52,'4',53,'5',54,'6',55,'7',56,'8',57,'9',
	58,':',59,';',60,'<',61,'=',62,'>',63,'?'};	
void CONVERT_MESSAGE1(uint16_t *bitVector, uint32_t value, uint8_t numBits)
{
	  int bit=0;
    for (  bit = 0; bit < 28; ++bit )  {

		  message1_ais[bit+14] =nmea0183[2*(sixbitA1[bit])+1];
			
			// message1_ais[]=cnt[bit]
    }
	  	me=checksum(message1_ais+1);
		  message1_ais[44]='*';
		 sprintf(message1_ais+45,"%0.2X", me);
 }

void CONVERT_MESSAGE5(uint16_t *bitVector, uint32_t value, uint8_t numBits)
{

    
	    int bit=0;
		
    for (  bit = 0; bit < 56; ++bit )  {

		  message5_1_ais[bit+15] =nmea0183[2*(sixbitA5[bit])+1];

    }
	  	me1=checksum(message5_1_ais+1);
		 
		 sprintf(message5_1_ais+74,"%0.2X", me1);
		
     for (  bit = 56; bit < 71; ++bit )  {

		  message5_2_ais[bit+15] =nmea0183[2*(sixbitA5[bit])+1];

    }
	  	me1=checksum(message5_2_ais+1);
		 
		 sprintf(message5_2_ais+33,"%0.2X", me1);		
	}

void payloadTosixbit(uint16_t *bitVector, uint16_t numBits,  uint16_t *byteArray)
{
	   int i=0;
	uint8_t b=0;
    for (  i = 0; i < numBits; i += 6 ) {
        uint8_t byte = 0;
        for (  b = 0; b < 6; ++b ) {
            byte |= (bitVector[i+b] << (5-b));
        }
        byteArray[i/6] = byte;
				
    }
}
char checksum(const char *s) {
    int c = 0;
    while (*s){
			if(*s =='*'){
				return c;
			}
		  c ^= *s++;
		}
		
    return c;
}


int NMEA_GPVTG_Comma_Pos(u8 *buf,u8 cx)
{	
int v=0;	
	 
	u8 *p=buf;

	while(cx)
	{		     
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
		if(*buf==','){
		if(cx==5 ){	
			for(v=0;v<5;v++){
				buf++;	
				sog1[v]=*buf;		
				
			}
		}	
    cx--;
	}
		buf++;
}
	return buf-p;	
	}


int NMEA_GPRMC_Comma_Pos(u8 *buf,u8 cx)
{	
int v=0;	
	int iv=0;	
	u8 *p=buf;
	while(cx)
	{		
 		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;
		if(*buf==','){
		if(cx==9 ){	
			for(v=0;v<4;v++){
				buf++;	
				H[v]=*buf;		
			}
		}	
	  if(cx==5 ){	
        buf++;
			for(v=0;v<2;v++){
				buf++;	
				lan[v]=*buf;	
			
			}
			lang1=floorf(strtod(lan, NULL)*100) / 100;
			for(v=0;v<2;v++){
				buf++;	
				lan1[v]=*buf;	  			
			}
			lang2=floorf((strtod(lan1, NULL)*100) /60)/100;     // (strtod(lan1, NULL)/60);
				buf++;	
	  	for(v=0;v<5;v++){
				buf++;	
				lan2[v]=*buf;	      		
			}
			lang3=floorf(strtod(lan2, NULL)*100) / 3600;
		}
		lang=(lang3+lang1+lang2 )*600000;
		if(cx==7 ){	
			for(iv=0;iv<2;iv++){
				 buf++;
				lat1[iv]=*buf;			
			}
		 lati1=strtod(lat1, NULL);
		 for(iv=0;iv<2;iv++){
				 buf++;
				lat2[iv]=*buf;			
			}	
     lati2=strtod(lat2, NULL)/60;		 
     buf++;	
		 for(iv=0;iv<5;iv++){
				 buf++;
				lat3[iv]=*buf;			
			}				
			lati3=(strtod(lat3, NULL)/1000/3600);
			
		}
		  lati=(lati3+lati2+lati1)*600000;
			if(cx==3 ){	
			 buf++;
			for(v=0;v<10;v++){
				buf++;		
			}
		}		
	 cx--;	
    if(cx==0 & H[0]!=','){
			time1=strtod(time, NULL); 
		}
		
	}
 	  	buf++;
	

	}
	return buf-p;	 
}

void get_GPS_datA(void) {
	T=2;
	      p1=(u8*)strstr((const char *)usart_buffer1,"GPRMC");
        posx=NMEA_GPRMC_Comma_Pos(p1,9);
		    lang1=strtod(lan, NULL);
			  p1=(u8*)strstr((const char *)usart_buffer1,"GPVTG");
        posx=NMEA_GPVTG_Comma_Pos(p1,9);
			  sog=strtod(sog1, NULL);
}
void addBits1(uint8_t *bitVector, uint16_t size, uint32_t value, uint8_t numBits)
{      uint8_t bit=0;
    if(numBits > 0  && numBits <= 32){
    uint16_t pos = size;
    for (  bit = 0; bit < numBits; ++bit, value >>= 1 )  {
        bitVector[pos + numBits-bit-1] = value & 1;
    }

    size += numBits;
}
}
//61471
char messag[45]="!AIVDM,1,1,,A,13u?etPv2;0n:dDPwUM1U1Cb069D,0*";	

void addSTRING(uint16_t *bitVector,char *BUF, uint32_t start_bit, uint8_t numBits)
{
	int iv=0;

	int iv1=0;
	int p=0;
	for(iv=0;iv<20;iv++){
		for(iv1=1;iv1<128;iv1+=2){
	    if(BUF[iv]==Name_Message5_Ais[iv1]){
		       //d99[p]=nmea0183[iv1-1];
				   addBits(d7,start_bit+p, Name_Message5_Ais[iv1-1],numBits);
			    	p+=6;
				    
				   break;
			     
	 // addBits(d99, 0,*p1-1, 20);//Message ID

			}
}
	}
}

