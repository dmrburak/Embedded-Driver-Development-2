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

#define TRUE	1
#define FALSE	0

void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;


int main(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;
	char msg[100];


	HAL_Init();
	UART2_Init();


	// osc_init is a local variable. It may contain some garbage values. So let's memset to zero this structure before initializing.
	memset (&osc_init,0,sizeof(osc_init));

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  \
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Saves some current. Because we're using HSE.


	/* Let's reconfigure the systick configuration. In order to make systick to generate interrupt for every 1ms
	 * because that is the requirement for the STM32 device hardware abstraction layer (HAL).
	 *
	 * The systick timer was working with the HCLK of 16MHz. But now the HCLK is changed to 4MHz. So, hence there
	 * is a need to reconfigure the systick. That's why we did here.
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);


	/* Uart2_Init() should be called once again because previously the baud rate was generated using the APB clock
	 * of 16MHz, but now the APB is changed to 2MHz. Hence UART2_Init has to be called once again in order to
	 * change the baud rate.
	 */
	UART2_Init();


	memset(msg,0,sizeof(msg));
	sprintf(msg,"SYSCLK : %ld\r\n",HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"HCLK : %ld\r\n",HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK1 : %ld\r\n",HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg,0,sizeof(msg));
	sprintf(msg,"PCLK2 : %ld\r\n",HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2,(uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	while(1);

	return 0;
}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if ( HAL_UART_Init(&huart2) != HAL_OK)
	{
		//There is a problem.
		Error_handler();
	}
}


void Error_handler(void)
{
	while(1); // If there's an error, compiler will be stuck in this line. Doesn't continues.
}


