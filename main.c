#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

void delay(__IO uint32_t time)
{
		while(time !=0) time--;
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
		
	while(1)
	{
		delay(400000); // This generates 500ms (for 8MHz clk) delay but this kind of delay is not recommended use timer functions later
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		delay(400000);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	
	return 0;
}
