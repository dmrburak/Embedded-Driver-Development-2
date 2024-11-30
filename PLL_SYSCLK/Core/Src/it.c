/*
 * it.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */

#include "main.h"

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
