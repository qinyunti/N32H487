/**
*     Copyright (c) 2023, Nations Technologies Inc.
* 
*     All rights reserved.
*
*     This software is the exclusive property of Nations Technologies Inc. (Hereinafter 
* referred to as NATIONS). This software, and the product of NATIONS described herein 
* (Hereinafter referred to as the Product) are owned by NATIONS under the laws and treaties
* of the People's Republic of China and other applicable jurisdictions worldwide.
*
*     NATIONS does not grant any license under its patents, copyrights, trademarks, or other 
* intellectual property rights. Names and brands of third party may be mentioned or referred 
* thereto (if any) for identification purposes only.
*
*     NATIONS reserves the right to make changes, corrections, enhancements, modifications, and 
* improvements to this software at any time without notice. Please contact NATIONS and obtain 
* the latest version of this software before placing orders.

*     Although NATIONS has attempted to provide accurate and reliable information, NATIONS assumes 
* no responsibility for the accuracy and reliability of this software.
* 
*     It is the responsibility of the user of this software to properly design, program, and test 
* the functionality and safety of any application made of this information and any resulting product. 
* In no event shall NATIONS be liable for any direct, indirect, incidental, special,exemplary, or 
* consequential damages arising in any way out of the use of this software or the Product.
*
*     NATIONS Products are neither intended nor warranted for usage in systems or equipment, any
* malfunction or failure of which may cause loss of human life, bodily injury or severe property 
* damage. Such applications are deemed, "Insecure Usage".
*
*     All Insecure Usage shall be made at user's risk. User shall indemnify NATIONS and hold NATIONS 
* harmless from and against all claims, costs, damages, and other liabilities, arising from or related 
* to any customer's Insecure Usage.

*     Any express or implied warranty with regard to this software or the Product, including,but not 
* limited to, the warranties of merchantability, fitness for a particular purpose and non-infringement
* are disclaimed to the fullest extent permitted by law.

*     Unless otherwise explicitly permitted by NATIONS, anyone may not duplicate, modify, transcribe
* or otherwise distribute this software for any purposes, in whole or in part.
*
*     NATIONS products and technologies shall not be used for or incorporated into any products or systems
* whose manufacture, use, or sale is prohibited under any applicable domestic or foreign laws or regulations. 
* User shall comply with any applicable export control laws and regulations promulgated and administered by 
* the governments of any countries asserting jurisdiction over the parties or transactions.
**/

/**
*\*\file main.c
*\*\author Nations
*\*\version v1.0.0
*\*\copyright Copyright (c) 2023, Nations Technologies Inc. All rights reserved.
**/
#include "main.h"
#include "uart.h"
#include "xprintf.h"
#include "shell.h"
#include "shell_func.h"
#include "spi.h"
#include "flash_itf.h"
#include "ff.h"

void mco_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);
    
    /* Configure MCO1 PA7 alternate function AF13 push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_7;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF13;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
	  RCC_ConfigMcoClkPre(RCC_MCO_PLLCLK_DIV6);
	  RCC_ConfigMco1(RCC_MCO_SYSCLK);
	
}


static void xprintf_out_port(int ch)
{
    uint8_t val = ch;
    uart_send(0,&val,1);
}

static int xprintf_in_port(void)
{
    uint32_t len;
    uint8_t val;
    do
    {
        len = uart_read(0,&val,1);
    }while(len == 0);
    return val;
}

uint32_t shell_read(uint8_t *buff, uint32_t len)
{
	return uart_read(0,buff,len);
}

void shell_write(uint8_t *buff, uint32_t len)
{
	uart_send(0,buff,len);
}


static FATFS fs;             /* Filesystem object */
static BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

void fs_init(void)
{
    FRESULT res;        /* API result code */
		if(FR_OK != (res = f_mount (&fs, "0:", 1)))
	  {
			xprintf("mount err %d, mkfs\r\n",res);
			res = f_mkfs("0:",0,work,sizeof(work));
			if(res == 0)
			{
				xprintf("mkfs ok\r\n");
				if(FR_OK == (res = f_mount (&fs, "0:", 1)))
				{
					xprintf("mount ok\r\n");
				}
				else
				{
					xprintf("mount err %d\r\n",res);
				}
			}
			else
		  {
				xprintf("mkfs err %d\r\n",res);
			}
		}
		else
		{
			xprintf("mount ok\r\n");
		} 
}


/**
*\*\name    main.
*\*\fun     Main program.
*\*\param   none
*\*\return  none
**/
int main(void)
{  
    /* System Clocks Configuration */
    //RCC_Configuration();

    /* Configure the GPIO ports */
    //GPIO_Configuration();
		RCC_ClocksType RCC_clock;
		RCC_GetClocksFreqValue(&RCC_clock);
    SysTick_Config(RCC_clock.SysclkFreq/1000);
	
	  uart_init(0,115200);
		
	  //mco_init();
	 	spi_init(0, 20000000ul, 3);
	  flash_itf_init();
	
    xdev_out(xprintf_out_port);
		xdev_in(xprintf_in_port);
	  shell_set_itf(shell_read, shell_write, (shell_cmd_cfg*)g_shell_cmd_list_ast, 1);
	
	  //fs_init();
    while (1)
    {
			shell_exec();
    /* Output a message on Hyperterminal using printf function */
    //printf("USART Printf Example: retarget the C library printf function to the USART\r\n");
			//uint8_t rx_buffer[64];
			//uint32_t len = uart_read(0,rx_buffer,sizeof(rx_buffer));
			//if(len > 0){
			//	uart_send(0,rx_buffer,len);
			//}
    }
}


/**
*\*\name    RCC_Configuration.
*\*\fun     Configures the different system clocks.
*\*\param   none
*\*\return  none
**/
void RCC_Configuration(void)
{
    /* Enable GPIO clock */
    RCC_EnableAHB1PeriphClk(USARTx_GPIO_CLK,ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO,ENABLE);
    /* Enable USARTy and USARTz Clock */
    USART_APBxClkCmd(USARTx_CLK,ENABLE);
}


/**
*\*\name    GPIO_Configuration.
*\*\fun     Configures the different GPIO ports.
*\*\param   none
*\*\return  none
**/
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);
    
    /* Configure USARTy Tx as alternate function push-pull */
    GPIO_InitStructure.Pin            = USARTx_TxPin;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = USARTx_Tx_GPIO_AF;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitPeripheral(USARTx_GPIO, &GPIO_InitStructure);
}

/**
*\*\name    fputc.
*\*\fun     retarget the C library printf function to the USART
*\*\param   ch
*\*\param   f
*\*\return  none
**/
int fputc(int ch, FILE* f)
{
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET)
    {
    }
    USART_SendData(USARTx, (uint8_t)ch);

    return (ch);
}

