/*
 * uart_communicator.c
 *
 *  Created on: Jul 1, 2024
 *      Author: admin
 */

#include "uart_communicator.h"
#include "uart_workers.h"

#include "tools/uart/stm32_uart_container.h"
#include "assertsEngine/assert_engine.h"

#include "raw_parser/raw_parser_dma/raw_parser_dma_test.h"


static void uart_rx_callback(uint8_t const * data, uint32_t const size, void* const captured)
{
	stm32_uart_communicator_t* const self = captured;
	RawParser_dma_receiveArray(&self->raw_p, data, size);
}

static void uart_tx_callback(const status_t status, void* const captured)
{
	stm32_uart_communicator_t* const self = captured;
}

static void uart_error_callback(const status_t status, void* const captured)
{
	stm32_uart_communicator_t* const self = captured;
}




status_t init_communicator(stm32_uart_communicator_t* const self, UART_HandleTypeDef* const huart)
{
	M_Assert_BreakSaveCheck(self == NULL || huart == NULL, M_EMPTY, return ERROR_FAIL, "null input");

	M_Assert_BreakSaveCheck(UART_Init(&self->stm32_uart, huart, 128) != STATUS_OK,
			M_EMPTY, return ERROR_FAIL,
					"uart init fail");

	UART_SetRxCallback(&self->stm32_uart, uart_rx_callback, self);
	UART_SetTxcallback(&self->stm32_uart, uart_tx_callback, self);
	UART_SetErrorCallback(&self->stm32_uart, uart_error_callback, self);

	M_Assert_BreakSaveCheck(rawParser_dma_init(&self->raw_p, 0x1a) != D_RAW_P_OK,
			M_EMPTY, return ERROR_FAIL,
					"kbus init fail");

	M_Assert_BreakSaveCheck(CallbackManager_init(&self->callb) != 0,
			M_EMPTY, return ERROR_FAIL,
					"callback manager init fail");

	CallbackManager_subscribe(&self->callb, 0x01, pingWorker, PREPROCESSOR_CTX_TYPE_CAST(self));

	int a = rawParserDmaTest(rand(), 1000, 1);

	return STATUS_OK;
}

void proceed_communicator(stm32_uart_communicator_t* const self, const uint32_t current_time)
{
	RawParser_Frame_t* const frame = RawParser_dma_proceed(&self->raw_p);
	u8* const data = frame->data;
	reg size = frame->size;

	if(size == 0) {
		return;
	}

	const u8 cmd = data[0];
	--size;

	CallbackManager_proceed(&self->callb, cmd,
			(data + 1), NULL, &size, D_RAW_P_TX_BUF_SIZE);
}



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


