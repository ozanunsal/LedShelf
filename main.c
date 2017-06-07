#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

// Global Timing Delay variable
uint32_t TimingDelay;

void SysTick_Handler(void)
{
	if (TimingDelay != 0) TimingDelay--;
}

void delay(uint32_t time)
{
	TimingDelay = time;
	while(TimingDelay != 0);
}

int main()
{
	// Initialize GPIO Structure
		GPIO_InitTypeDef GPIO_InitStructure;
	//Enable the GPIO Clock for port D
	__GPIOD_CLK_ENABLE();
		
	//Configures the GPIO Structure
	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	
	//Initialize the GPIO
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//Initialize Timer
	SysTick_Config(SystemCoreClock / 1000); // Configure Systick interrupts as 1ms
		
	while(1)
	{
		delay(300); // This generates 300ms
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		delay(300);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	
	return 0;
}
