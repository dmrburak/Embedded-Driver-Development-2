/*
 * it.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */


void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
