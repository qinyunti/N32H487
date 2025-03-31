#include "uart.h"
#include "fifo.h"


#include "n32h47x_48x.h"
#include "n32h47x_48x_rcc.h"
#include "n32h47x_48x_gpio.h"
#include "n32h47x_48x_exti.h"
#include "n32h47x_48x_usart.h"
#include "n32h47x_48x_dma.h"

#define CriticalAlloc()
#define EnterCritical()	__disable_irq()
#define ExitCritical()  __enable_irq()

uint8_t s_uart_rx0_buffer[1029];
uint8_t s_uart_rx1_buffer[16];

static fifo_st s_uart_fifo_dev[2]=
{
	{
	 .in = 0,
	 .len = 0,
	 .out = 0,
	 .buffer = s_uart_rx0_buffer,
	 .buffer_len = sizeof(s_uart_rx0_buffer),
	},
	{
	 .in = 0,
	 .len = 0,
	 .out = 0,
	 .buffer = s_uart_rx1_buffer,
	 .buffer_len = sizeof(s_uart_rx1_buffer),
	}
};

void uart_rx_cb(int id, uint8_t* buff, uint32_t len)
{
	fifo_in(&(s_uart_fifo_dev[id]), buff, len);
}


void USART3_IRQHandler(void)
{
	uint8_t ch;
	ch = USART_ReceiveData(USART3);
	USART_ClrFlag(USART3, USART_FLAG_RXDNE);
	USART_ClrFlag(USART3, USART_FLAG_IDLEF);	
	USART_ClrFlag(USART3, USART_FLAG_OREF);		
	uart_rx_cb(0, &ch, 1);
}

void uart_init(int id, uint32_t baud)
{
	if(id == 0){
		RCC_EnableAHB1PeriphClk(RCC_AHB_PERIPHEN_GPIOA,ENABLE);
    GPIO_InitType GPIO_InitStructure;

    /* Initialize GPIO_InitStructure */
    GPIO_InitStruct(&GPIO_InitStructure);
    
    /* Configure USART3 Tx PA9 AF17 as alternate function push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_9;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF17;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
		
    /* Configure USART3 Rx PA10 AF18 as alternate function push-pull */
    GPIO_InitStructure.Pin            = GPIO_PIN_10;
    GPIO_InitStructure.GPIO_Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF18;
    GPIO_InitStructure.GPIO_Pull      = GPIO_PULL_UP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
		
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART3,ENABLE);
		USART_InitType USART_InitStructure;
    /* USARTy and USARTz configuration */
	  USART_StructInit(&USART_InitStructure);
    USART_InitStructure.BaudRate            = baud;
    USART_InitStructure.WordLength          = USART_WL_8B;
    USART_InitStructure.StopBits            = USART_STPB_1;
    USART_InitStructure.Parity              = USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.OverSampling        = USART_16OVER;
    USART_InitStructure.Mode                = USART_MODE_RX | USART_MODE_TX;

    /* Configure USART3 */
    USART_Init(USART3, &USART_InitStructure);

    /* Enable the USART3 */
    USART_Enable(USART3, ENABLE);
		
		USART_ConfigInt(USART3, USART_INT_RXDNE, ENABLE);
		NVIC_SetPriority(USART3_IRQn,2);
		NVIC_EnableIRQ(USART3_IRQn);
	}else if(id == 1){

	}
}

uint32_t uart_send(int id, uint8_t* buffer, uint32_t len)
{
	  if(id == 0){
			for(uint32_t i=0;i<len;i++)
			{
				while(USART_GetFlagStatus(USART3, USART_FLAG_TXC) == 0);
				USART_SendData(USART3,buffer[i]);
			}
		}else if(id == 1){

		}
    return len;
}

uint32_t uart_read(int id, uint8_t* buffer, uint32_t len)
{
    uint32_t rlen;
    CriticalAlloc();
    EnterCritical();
    rlen = fifo_out(&(s_uart_fifo_dev[id]), buffer, len);
    ExitCritical();
    return rlen;
}
