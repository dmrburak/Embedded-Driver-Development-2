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

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t data);

UART_HandleTypeDef huart2;

char *user_data = "The application is running.\r\n";

uint8_t data_buffer[100];
uint32_t count = 0;
uint8_t rcvd_data;
uint8_t reception_complete = FALSE;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();


	uint16_t length_of_data = strlen(user_data);

	if ( HAL_UART_Transmit(&huart2, (uint8_t*)user_data, length_of_data, HAL_MAX_DELAY) != HAL_OK )
	{
		Error_handler();
	}

	while(reception_complete != TRUE)
	{
	    HAL_UART_Receive_IT(&huart2,&rcvd_data,1);
	}

	while(1);

	return 0;
}


void SystemClockConfig(void)
{
	/*We use SystemClockConfig when we need special clock settings.	 */

	/*By default MCU will be running with the internal RC oscillator */
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rcvd_data == '\r')
	{
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		HAL_UART_Transmit(&huart2, &data_buffer, count, HAL_MAX_DELAY);
	}else
	{
		data_buffer[count++] = rcvd_data;
	}
}


uint8_t convert_to_capital(uint8_t data)
{
	if(islower(data))
	{
		return toupper(data);
	}else{
		return data;
	}
}

void Error_handler(void)
{
	while(1); // If there's an error, compiler will be stuck in this line. Doesn't continues.
}


