/*
 * uart_communicator.c
 *
 *  Created on: Jul 1, 2024
 *      Author: admin
 */

#include "uart_communicator.h"

#include "tools/uart/stm32_uart_container.h"
#include "assertsEngine/assert_engine.h"
#include "raw_parser/raw_parser_dma/raw_parser_dma_test.h"


static void uart_rx_callback(uint8_t const * data, uint32_t const size, void* const captured)
{
	stm32_uart_communicator_t* const self = (stm32_uart_communicator_t*)captured;
	RawParser_dma_receiveArray(&self->raw_p, data, size);
}

static void uart_tx_callback(const status_t status, void* const captured)
{
	stm32_uart_communicator_t* const parrent = (stm32_uart_communicator_t*)captured;
	pool_container_t* const pack_pool = &parrent->pack_pool;
	poolContainer_nextReadPos(pack_pool);
}
//
//static void uart_error_callback(const status_t status, void* const captured)
//{
//	stm32_uart_communicator_t* const self = (stm32_uart_communicator_t*)captured;
//}

static void pingWorker(u8* const data, const reg size, void* const ctx)
{
	stm32_uart_communicator_t* const parrent = (stm32_uart_communicator_t*)ctx;
	pool_container_t* const pack_pool = &parrent->pack_pool;
	RawParser_dma_t* const parser = &parrent->raw_p;

	u8 t1 = 0x01;
	u8 t2 = 0x11;
	u8 t3 = 0x22;
	u8 t4 = 0x33;
	u16 t5 = 0xAA55;

	u8 read = 0;
	u16 read16 = 0;
	u16 read16_2 = 0;
	RawParser_dma_universalRead(parser, sizeof(u8), sizeof(u8), &read);
	RawParser_dma_universalRead(parser, sizeof(u16), sizeof(u16), (u8*)&read16);
	RawParser_dma_universalRead(parser, sizeof(u16), sizeof(u16), (u8*)&read16_2);

	if(!FIFO_IS_FULL(pack_pool)) {
		pool_elem_t* const buffer = (pool_elem_t*) poolContainer_getWriteBuffer(pack_pool);
		rawParser_dma_setUserBufferTX(parser, buffer->data);

		RawParser_dma_startTransmittPacket(parser, 10);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t1);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t2);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t3);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &t4);
		RawParser_dma_universalWrite(parser, sizeof(u16), sizeof(u16), (u8*)&t5);

		RawParser_Frame_t*const frame = RawParser_dma_finishTransmittPacket(parser);
		buffer->size = frame->size;
		poolContainer_nextWritePos(pack_pool);
	}
}


status_t init_communicator(stm32_uart_communicator_t* const self, UART_HandleTypeDef* const huart)
{
	M_Assert_BreakSaveCheck(self == NULL || huart == NULL, M_EMPTY, return ERROR_FAIL, "null input");

	M_Assert_BreakSaveCheck(UART_Init(&self->stm32_uart, huart, 128) != STATUS_OK,
			M_EMPTY, return ERROR_FAIL,
					"uart init fail");

	UART_SetRxCallback(&self->stm32_uart, uart_rx_callback, self);
	UART_SetTxcallback(&self->stm32_uart, uart_tx_callback, self);
	//	UART_SetErrorCallback(&self->stm32_uart, uart_error_callback, self);


	poolContainer_init(&self->pack_pool, 4, sizeof(pool_elem_t) + D_RAW_P_TX_BUF_SIZE);

	M_Assert_BreakSaveCheck(rawParser_dma_init(&self->raw_p, 0x1a) != D_RAW_P_OK,
			M_EMPTY, return ERROR_FAIL,
					"kbus init fail");

	pool_elem_t* const buffer = (pool_elem_t*) poolContainer_getWriteBuffer(&self->pack_pool);
	rawParser_dma_setUserBufferTX(&self->raw_p, buffer->data);

	M_Assert_BreakSaveCheck(CallbackManager_init(&self->callb, 5, self),
			M_EMPTY, return ERROR_FAIL,
					"callback manager init fail");
	CallbackManager_subscribe(&self->callb, 0x01, pingWorker);

	//int a = rawParserDmaTest(rand(), 1000, 1);

	return STATUS_OK;
}

void proceed_communicator(stm32_uart_communicator_t* const self, const uint32_t current_time)
{

	// proceed RX
	{
		RawParser_Frame_t* const frame = RawParser_dma_proceed(&self->raw_p);
		u8* const data = frame->data;
		reg size = frame->size;

		if(size == 0 || frame == NULL) {
			goto transmitt;
		}

		const u8 cmd = data[0];

		CallbackManager_proceed(&self->callb, cmd, (data + 1), (size - 1));
	}

	// proceed TX
	transmitt:
	{
		if(!UART_IsTxBusy(&self->stm32_uart) && !FIFO_IS_EMPTY(&self->pack_pool)) {
			pool_elem_t* const buffer = (pool_elem_t*) poolContainer_readBuffer(&self->pack_pool);
			UART_SendBuffer(&self->stm32_uart, buffer->data, buffer->size);
		}
	}
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


