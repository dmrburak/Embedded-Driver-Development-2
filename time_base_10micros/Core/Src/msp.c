/*
 * msp.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */

#include "main.h"

TIM_HandleTypeDef htimer2;

void HAL_MspInit(void)
{
	/*	Here we'll do low level processor specific inits.	*/

	// 1. Set up the priority grouping of the arm cortex mx processor.
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor.
	SCB->SHCSR |= (0x7 << 16); // Memory fault, Bus fault, and Usage fault system exceptions are enabled.

	// 3. Configure the priority for the system exceptions.
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}





void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//1.enable the clock for the TIM2 peripheral
	__HAL_RCC_TIM2_CLK_ENABLE();

	//2.enable the IRQ of TIM2
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	//3.setup the priority for TIM2
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);//subpriority is zero because we're using NVIC_PRIORITYGROUP_4
}






