/*
 * stm32_printf.c
 *
 *  Created on: Mar 14, 2023
 *      Author: admin
 */
#include "main.h"
#include "stm32_printf.h"

extern UART_HandleTypeDef huart1;
#define UART_PRINTF	huart1

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&UART_PRINTF, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}
