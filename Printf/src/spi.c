#include "uart.h"
#include "fifo.h"

#include "n32h47x_48x.h"
#include "n32h47x_48x_spi.h"
#include "n32h47x_48x_rcc.h"
#include "n32h47x_48x_gpio.h"

void spi_init(int id, uint32_t baud, int mode)
{
	if(id == 0){		
		/* PA4 NSS    GPIO
		 * PA5 SCK   AF1
		 * PA6 MISO  AF1
		 * PA7 MOSI  AF1
		 */
			RCC_EnableAHB1PeriphClk(RCC_AHB_PERIPHEN_GPIOA,ENABLE);
		  RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_SPI1,ENABLE);
		
			GPIO_InitType GPIO_InitStructure;
			/* Initialize GPIO_InitStructure */
			GPIO_InitStruct(&GPIO_InitStructure);

			GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
			GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
			GPIO_InitStructure.GPIO_Alternate = GPIO_AF1;
			GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			
			GPIO_InitStructure.Pin = GPIO_PIN_4;
			GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
			GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
			GPIO_WriteBits(GPIOA, GPIO_PIN_4, Bit_SET);
					
		  SPI_InitType SPI_InitStruct;
			RCC_ClocksType RCC_clock;
			RCC_GetClocksFreqValue(&RCC_clock);
			
			uint32_t clk = RCC_clock.Pclk2Freq;
			uint32_t div = clk/baud;
			if(div<=2){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_2;
			}else if(div<=4){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_4;
			}else if(div<=8){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_8;
			}else if(div<=16){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_16;
			}else if(div<=32){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_32;
			}else if(div<=64){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_64;
			}else if(div<=128){
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_128;
			}else{
				SPI_InitStruct.BaudRatePres  = SPI_BR_PRESCALER_256;
			}
			if(mode & 0x01){
				SPI_InitStruct.CLKPHA = SPI_CLKPHA_SECOND_EDGE;
			}else{
				SPI_InitStruct.CLKPHA = SPI_CLKPHA_FIRST_EDGE;
			}
			if(mode & 0x02){
				SPI_InitStruct.CLKPOL = SPI_CLKPOL_HIGH;
			}else{
				SPI_InitStruct.CLKPOL = SPI_CLKPOL_LOW;
			}
			SPI_InitStruct.CRCPoly = 0x0f;
			SPI_InitStruct.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
			SPI_InitStruct.DataLen = SPI_DATA_SIZE_8BITS;
			SPI_InitStruct.FirstBit = SPI_FB_MSB;
			SPI_InitStruct.NSS = SPI_NSS_SOFT;
			SPI_InitStruct.SpiMode = SPI_MODE_MASTER;
		  SPI_Init(SPI1,&SPI_InitStruct);
			SPI_Enable(SPI1,ENABLE);
	}else if(id == 1){

	}
}


uint32_t spi_trans(int id, int cs, uint8_t* tx_buffer, uint8_t* rx_buffer, uint32_t len, int flag)
{
	uint8_t rx;

	if(id == 0){
		if(cs == 0){
			GPIO_WriteBits(GPIOA, GPIO_PIN_4, Bit_RESET);
		}else if(cs == 1){

		}

		for(uint32_t i=0; i<len; i++){
			while(SPI_I2S_GetStatus(SPI1, SPI_I2S_BUSY_FLAG) != 0);
			if(tx_buffer != (uint8_t*)0){
				SPI_I2S_TransmitData(SPI1, tx_buffer[i]);
			}else{
				SPI_I2S_TransmitData(SPI1, 0xFF);
			}
			
			while(SPI_I2S_GetStatus(SPI1, SPI_I2S_RNE_FLAG) == 0);
			rx = SPI_I2S_ReceiveData(SPI1);
			if(rx_buffer != (uint8_t*)0){
				rx_buffer[i] = rx;
			}
		}
		
		if(flag){
			if(cs == 0){
				GPIO_WriteBits(GPIOA, GPIO_PIN_4, Bit_SET);
			}else if(cs == 1){

			}
		}
	}else if(id == 1){

	}

	return len;
}

