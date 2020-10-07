#include "stm32f10x.h"      
#include "stdio.h"   
#include "string.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "spi.h"


void PeripheralInit_SPI1_Master(){
 
    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;
 
    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);
 
    // initialize clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
 
    // do not initialize A4/SS because a software SS will be used
 
    // initialize A5/SCK alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    // initialize A6/MISO input pull-up (50MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    // initialize A7/MOSI alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    // initialize SPI master
    // for slave, no need to define SPI_BaudRatePrescaler
    SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
    SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge;          // CPHA = 1
    SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB2 72/64 = 1.125 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
    SPI_InitDef.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitDef);
 
    SPI_Cmd(SPI1, ENABLE);
 
}

void PeripheralInit_SPI2_Master(){
 
    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;
 
    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);
 
    // initialize clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); // this is on a different bus
 
    // do not initialize B12/SS because a software SS will be used
 
    // initialize B13/SCK alternate function push-pull (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    // initialize B14/MISO input pull-up (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    // initialize B15/MOSI alternate function push-pull (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
 
    //  initialize SPI master
    // for slave, no need to define SPI_BaudRatePrescaler
    SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
 //   SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitDef.SPI_CPHA = SPI_CPHA_1Edge;              // CPHA = 1
    SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use software SS
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // APB1 36/32 = 1.125 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // APB1 36/128 = 0.28 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // APB1 36/8 = 4.5 MHz
    SPI_InitDef.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitDef);
 
    SPI_Cmd(SPI1, ENABLE);
}
uint8_t SPI_send(uint8_t outByte){
 
    // Approach 1, from Brown's book
    // SPI_I2S_SendData(SPI2, outByte); // send
    // while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    // return SPI_I2S_ReceiveData(SPI2); // read received
 
    // Approach 2,
    // from http://www.lxtronic.com/index.php/basic-spi-simple-read-write
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1, outByte); // send
    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    return SPI_I2S_ReceiveData(SPI1); // read recieived
}
void PeripheralInit_GPIO() {
 
    GPIO_InitTypeDef GPIO_InitDef;  // GPIO init
    GPIO_StructInit(&GPIO_InitDef); // initialize init struct
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
 
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;         // GPIO pin 3
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_Out_PP;  // GPIO mode output push-pull
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;  // GPIO port output speed, 2 MHz
    GPIO_Init(GPIOA, &GPIO_InitDef);    // initialize pin on GPIOA port
 
	
	
	    GPIO_InitDef.GPIO_Pin = GPIO_Pin_2;         // GPIO pin 3
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // GPIO mode output push-pull
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;  // GPIO port output speed, 2 MHz
     GPIO_Init(GPIOA, &GPIO_InitDef);    // initialize pin on GPIOA port
}
void deselect_Spi(void){
	    GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
 
void select_Spi(void){
	    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}



