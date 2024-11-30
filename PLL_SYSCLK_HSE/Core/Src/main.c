/*
 * main.c
 *
 *  Created on: Nov 30, 2024
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
void SystemClockConfig_HSE(uint8_t clock_freq);


UART_HandleTypeDef huart2;


int main(void)
{

	char msg[100];

	HAL_Init();

	SystemClockConfig_HSE(SYS_CLOCK_FREQ_32MHz);

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

void SystemClockConfig_HSE(uint8_t clock_freq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency = 0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	//according to our application of exercise.
		osc_init.PLL.PLLMUL = RCC_PLL_MUL9;
		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK  \
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_2;


	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, FLatency) != HAL_OK)
	{
		Error_handler();
	}

	//Systick configuration : We need to configure it because we have modified the HCLK.
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000); // 1000 is number of ticks we have to pass, in order to generate delay of 1ms.

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);



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


