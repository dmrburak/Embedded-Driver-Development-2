/*
 * msp.c
 *
 *  Created on: Oct 8, 2024
 *      Author: B U R A K
 */

#include "stm32f1xx_hal.h"


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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	// Here we are going to do the low level initializations of the USART2 peripheral.

	// 1. Enable the clock for the USART2 and GPIOA peripheral.
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();


	// 2. Do the pin muxing configuration. No remap pin configs are PA2 and PA3 for Tx and Rx
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	// 3. Enable the IRQ and set up the priority (NVIC settings).
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}













