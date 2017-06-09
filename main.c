#include "stm32f4xx.h"// Device header

volatile uint32_t msTicks;
volatile char RxBuf[];
void SysTick_Handler(void)
{
	msTicks++;
}

void delay(__IO uint32_t dValue)
{
	uint32_t curTick = msTicks;
	while ((msTicks - curTick) < dValue);
}

void USART_puts(USART_TypeDef *USARTx, volatile char *str)
{
	while(*str){
		//while(!(USARTx->SR & 0x040)); // Wait for the sending finish
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
		USART_SendData(USARTx, *str);
		str++;
	}
}

//USART Interrupt request handler
void USART2_IRQHandler(void)
{
		if(USART_GetITStatus(USART2, USART_IT_RXNE)){
			static int cnt = 0;
			//flag is set
			char c = USART2->DR; // Transfer the byte in data register
			if (c != '\n') {
				RxBuf[cnt] = c;
				cnt++;
			} else {
				RxBuf[cnt] = '\0';
				cnt = 0;
				USART_puts(USART2, RxBuf);
			}
		}
}

int main()
{
	// Initialize GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Configure SysTick
	if (SysTick_Config( SystemCoreClock / 1000)) { // 1ms SysTick
		/*ToDO Give Error Indicator*/
	}
	
	//Initialize USART
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // enable USART clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // enable gpio A ports
	//Setup gpio pins Tx(PA2) and Rx(PA3)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // Declare USART generate interrupts for Rx only
	
	//Initialize NVIC we want use USART in interrupt mode, so we need NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //highest priorty for peripheral
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE); // Enable USART2 Peripheral
	
	while(1)
	{
		USART_puts(USART2, "Hello World\n");

		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay(200);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);	
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);	
		GPIO_ResetBits(GPIOD, GPIO_Pin_14);	
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);	
		delay(200);
	}
}
