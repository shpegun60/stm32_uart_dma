/*
 * app_core.cpp
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */
#include "app_core.h"
#include "main.h"

#include "tools/uart/stm32_uart_container.h"
#include "tools/timer/TimingManager.h"



stm32_DMA_uart_t* const getUartInstance()
{
	extern UART_HandleTypeDef huart1;
	static stm32_DMA_uart_t* const uart_instance = UART_new(&huart1, 128);

	return uart_instance;
}

void uart_rx_callback(uint8_t const * data, uint32_t const size, void* const captured)
{

}

void uart_tx_callback(const status_t status, void* const captured)
{

}

void uart_error_callback(const status_t status, void* const captured)
{

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//	stm32_DMA_uart_t* const uart_instance = getUartInstance();
	//
	//	if(uart_instance->huart == huart) {
	//		UART_TXCallback(uart_instance);
	//	}
	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_TXCallback(uart_instance);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	stm32_DMA_uart_t* const uart_instance = getUartInstance();
//
//	if(uart_instance->huart == huart) {
//		UART_RXCallback(uart_instance);
//	}

	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_RXCallback(uart_instance);
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
//	stm32_DMA_uart_t* const uart_instance = getUartInstance();
//
//	if(uart_instance->huart == huart) {
//		UART_ErrorCallback(uart_instance);
//	}

	stm32_DMA_uart_t* const uart_instance = getContainerUartInstance(huart);
	if(uart_instance) {
		UART_ErrorCallback(uart_instance);
	}
}


void initUart()
{
	initUartsContainer();
	stm32_DMA_uart_t* const uart_instance = getUartInstance();
	UART_SetRxCallback(uart_instance, uart_rx_callback, uart_instance);
	UART_SetTxcallback(uart_instance, uart_tx_callback, uart_instance);
	UART_SetErrorCallback(uart_instance, uart_error_callback, uart_instance);
}



void app_main(void)
{
	uint8_t txBuff[] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	TimingManager led_timer;
	initUart();

	while(true) {

		if(led_timer.isExpired()) {
			stm32_DMA_uart_t* const uart_instance = getUartInstance();
			UART_SendBuffer(uart_instance, txBuff, sizeof(txBuff));
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

			led_timer.start(100);
		}

	}
}
