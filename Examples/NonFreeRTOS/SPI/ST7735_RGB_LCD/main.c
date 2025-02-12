/**
 * ST7735 RGB 128x160 LCD Demo
 * 
 * 
 * AIR32        ST7735
 * - PA2        BL
 * - PA3        CS
 * - PA4        DC(Data/Command)
 * - PA5        SCK/SCL
 * - PA6        RES
 * - PA7        SI/SDA
 * - GND        GND
 * - 3.3V       VCC
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "air32f10x.h"
#include "debug.h"
#include "st7735.h"

void APP_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6);
}

void APP_SPI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_7);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}   


int main(void)
{
  RCC_ClocksTypeDef clocks;
  uint16_t i, color = 0;

  Delay_Init();
  USART_Printf_Init(115200);
  RCC_GetClocksFreq(&clocks);

  printf("\n");
  printf("SYSCLK: %ld, HCLK: %ld, PCLK1: %ld, PCLK2: %ld, ADCCLK: %ld\n",
         clocks.SYSCLK_Frequency, clocks.HCLK_Frequency,
         clocks.PCLK1_Frequency, clocks.PCLK2_Frequency, clocks.ADCCLK_Frequency);
  printf("AIR32F103 ST7735 Demo.\n");

  APP_GPIO_Config();
  APP_SPI_Config();
  ST7735_Init();

  while (1)
  {
    ST7735_FillAddrWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1, 0x001F);
    Delay_Ms(500);
    ST7735_FillAddrWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1, 0xF800);
    Delay_Ms(500);
    ST7735_FillAddrWindow(0, 0, ST7735_WIDTH - 1, ST7735_HEIGHT - 1, 0x07E0);
    Delay_Ms(500);

    for (i = 0; i < ST7735_HEIGHT; i += 20)
    {
      ST7735_FillAddrWindow(0, i, ST7735_WIDTH - 1, i + 20 - 1, color++);
      Delay_Ms(200);
    }
    for (i = 0; i < ST7735_WIDTH; i += 16)
    {
      ST7735_FillAddrWindow(i, 0, i + 16 - 1, ST7735_HEIGHT - 1, color++);
      Delay_Ms(200);
    }
  }
}
