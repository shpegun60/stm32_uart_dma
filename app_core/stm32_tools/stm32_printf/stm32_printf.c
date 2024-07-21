/*
 * stm32_printf.c
 *
 *  Created on: Mar 14, 2023
 *      Author: admin
 */
#include "main.h"

#if defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED))

extern UART_HandleTypeDef huart1;
#define UART_PRINTF	huart1

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&UART_PRINTF, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

#endif /* defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)) */
