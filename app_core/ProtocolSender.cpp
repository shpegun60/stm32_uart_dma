/*
 * ProtocolSender.cpp
 *
 *  Created on: Jul 10, 2024
 *      Author: admin
 */

#include "ProtocolSender.h"
#include "tools/uart/stm32_uart_container.h"
#include "assertsEngine/assert_engine.h"

static void uart_rx_callback(uint8_t const * data, uint32_t const size, void* const captured)
{
	RawParser_dynamic_t* const raw_p = (RawParser_dynamic_t*)captured;
	rawP_dynamic_receiveArray(raw_p, data, size);
}

static void uart_tx_callback(const status_t status, void* const captured)
{
	pool_container_t* const pack_pool = (pool_container_t*)captured;
	poolContainer_nextReadPos(pack_pool);
}
//
//static void uart_error_callback(const status_t status, void* const captured)
//{
//	stm32_uart_communicator_t* const self = (stm32_uart_communicator_t*)captured;
//}



static void rx_callback(ringbuf_t* const rxStream, void* const ctx)
{
	//ProtocolSender* const self = (ProtocolSender*)ctx;
	ProtocolSender& self = *static_cast<ProtocolSender*>(ctx);

	RawParser_dynamic_t* const parser = self.getParserNew();
	pool_container_t* const pack_pool = self.getPool();

	while(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(pack_pool);

		ringbuf_install_buf(&parser->TX.frame_stream, buffer->data);
		rawP_dynamic_startTransmittPacket(parser, 10);

		rawp_stream_add_u8(parser, 0x11);
		rawp_stream_add_u8(parser, 0x22);
		rawp_stream_add_u8(parser, 0x33);
		rawp_stream_add_u8(parser, 0x1a);
		rawp_stream_add_u16(parser, 0xaabb);

		ringbuf_t* const ring = rawP_dynamic_finishTransmittPacket(parser);
		buffer->size = ring->base.head;
		poolContainer_nextWritePos(pack_pool);
	}
}


status_t ProtocolSender::init(UART_HandleTypeDef *const huart)
{
	M_Assert_BreakSaveCheck(huart == NULL, M_EMPTY, return ERROR_FAIL, "null input");

	M_Assert_BreakSaveCheck(UART_Init(&stm32_uart, huart, 128) != STATUS_OK,
			M_EMPTY, return ERROR_FAIL,
					"uart init fail");

	poolContainer_init(&pack_pool, 4, sizeof(pool_elem) + 128);
	pool_elem* const buffer = static_cast<pool_elem*>(poolContainer_getWriteBuffer(&pack_pool));

	RawP_dynamic_init_t rawP_init = {
			.packStart = 0x1a,
			//rx -------------
			.rxBufferSize = 128,
			.rxFrameSize = 256,
			.rxFrameBuff = NULL,
			//tx--------------
			.txFrameSize = 128,
			.txFrameBuff = buffer->data,
			.crc_type = CRC8
	};

	rawP_dynamic_init(&rawP_new, &rawP_init);
	rawP_dynamic_subscribe(&rawP_new, rx_callback, this);


	UART_SetRxCallback(&stm32_uart, uart_rx_callback, &rawP_new/*&raw_p*/);
	UART_SetTxcallback(&stm32_uart, uart_tx_callback, &pack_pool);
	//	UART_SetErrorCallback(&self->stm32_uart, uart_error_callback, self);

	m_keyGroup = Key_tps1htc30Group::getInstance();
	return STATUS_OK;
}

void ProtocolSender::proceed(const uint32_t current_time)
{
	rawP_dynamic_proceed(&rawP_new);

	//	if(proto_timer.isExpired()) {
	//		rawParser_dynamic_startTransmittPacket(&rawP_new, 10);
	//		rawp_stream_add_u8(&rawP_new, 0x11);
	//		rawp_stream_add_u8(&rawP_new, 0x22);
	//		rawp_stream_add_u8(&rawP_new, 0x33);
	//		rawp_stream_add_u8(&rawP_new, 0x1a);
	//		rawp_stream_add_u16(&rawP_new, 0xaabb);
	//		ringbuf_t* const ring = rawParser_dynamic_finishTransmittPacket(&rawP_new);
	//		UART_SendBuffer(&stm32_uart, (u8*)ring->buffer, FIFO_LEN(ring));
	//		proto_timer.start(100);
	//	}

		// proceed TX
		{
			if(!UART_IsTxBusy(&stm32_uart) && FIFO_NOT_EMPTY(&pack_pool)) {
				pool_elem* const buffer = (pool_elem*) poolContainer_readBuffer(&pack_pool);
				UART_SendBuffer(&stm32_uart, buffer->data, buffer->size);
			}
		}

	//	// proceed RX
	//	{
	//		RawParser_Frame_t* const frame = RawParser_dma_proceed(&raw_p);
	//		u8* const data = frame->data;
	//		reg size = frame->size;
	//
	//		if(size == 0 || frame == NULL) {
	//			goto periodicSend;//return;
	//		}
	//
	//		const u8 cmd = data[0];
	//
	//		CallbackManager_proceed(&callb, cmd, (data + 1), (size - 1));
	//	}
	//
	//	// send all state
	//	periodicSend:
	//	{
	//
	//		if(proto_timer.isExpired()) {
	//
	//			while(FIFO_NOT_FULL(&pack_pool)) {
	//				ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(&pack_pool);
	//				rawParser_dma_setUserBufferTX(&raw_p, buffer->data);
	//
	//
	//				u8 groups = m_keyGroup->getGroups();
	//				u8 key_0 = m_keyGroup->getKeys(0);
	//				u8 key_1 = m_keyGroup->getKeys(1);
	//				reg len = 3 + (key_0 + key_1) * 8;
	//
	//				RawParser_dma_startTransmittPacket(&raw_p, len);
	//				RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &groups);
	//				RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &key_0);
	//				RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &key_1);
	//
	//
	//				m_keyGroup->iterateAll([this](Key_tps1htc30Group::KeyGroup* key, uint8_t group_id, uint8_t key_id) -> bool {
	//					u8 err = (u8)key->err[key_id];
	//					u8 en = (u8)key->out_state[key_id];
	//
	//					RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &group_id);
	//					RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &key_id);
	//					RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &en);
	//					RawParser_dma_universalWrite(&raw_p, sizeof(u8), sizeof(u8), &err);
	//					RawParser_dma_universalWrite(&raw_p, sizeof(float), sizeof(float), (u8*)&key->current);
	//					return false;
	//				});
	//
	//				RawParser_Frame_t*const frame = RawParser_dma_finishTransmittPacket(&raw_p);
	//				buffer->size = frame->size;
	//				poolContainer_nextWritePos(&pack_pool);
	//			}
	//
	//
	//			proto_timer.start(50);
	//		}
	//
	//	}

}



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

