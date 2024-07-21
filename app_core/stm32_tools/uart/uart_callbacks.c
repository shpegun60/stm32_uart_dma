/*
 * uart_callbacks.c
 *
 *  Created on: Jul 15, 2024
 *      Author: admin
 */
#include "main.h"
#include "stm32_uart.h"



// callbacks IT --------------------------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_TXCallback(uart_instance);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_RXCallback(uart_instance);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_ErrorCallback(uart_instance);
	}
}
