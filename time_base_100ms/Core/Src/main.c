/*
 * main.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */


#include "stm32f1xx_hal.h"
#include "main.h"
#include "string.h"
#include <ctype.h>

void SystemClockConfig(void);
void Error_handler(void);
void TIMER2_Init(void);
void GPIO_Init(void);


TIM_HandleTypeDef htimer2;


int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	TIMER2_Init();

	//Lets start the timer
	HAL_TIM_Base_Start(&htimer2);

	while(1)
	{
		//Loop until the update event flag is set.
		while( ! (TIM2->SR & TIM_SR_UIF) ); //When we come out of the while loop, that means that hundred ms indeed elapsed. So, UIF bit is set.
		TIM2->SR = 0; // This bit is set by hardware on an update event. It is cleared by software.
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

	}


	return 0;
}


void SystemClockConfig(void)
{
	/*We use SystemClockConfig when we need special clock settings.	 */

	/*By default MCU will be running with the internal RC oscillator */
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	ledgpio.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init(GPIOA, &ledgpio);

}


void TIMER2_Init(void)
{
  htimer2.Instance = TIM2;
  htimer2.Init.Prescaler = 8000-1;	// Prescaler: 7999, prescaler output CNT_CLK = 8 MHz / (7999 + 1) = 1 kHz,  Time period = 1 / CNT_CLK = 1 / 1kHz = 1ms
  htimer2.Init.Period = 100-1;		// Period (ARR): (100ms / time period ) -1 = (100ms / 1ms) - 1 = 100 - 1 = 99
  if( HAL_TIM_Base_Init(&htimer2) != HAL_OK )
  {
    Error_handler();
  }
}


void Error_handler(void)
{
	while(1); // If there's an error, compiler will be stuck in this line. Doesn't continues.
}


