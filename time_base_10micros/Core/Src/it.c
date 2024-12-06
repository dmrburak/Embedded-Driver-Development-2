/*
 * it.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */

#include "main.h"

extern TIM_HandleTypeDef htimer2;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htimer2);
}
