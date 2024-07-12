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
//	RawParser_dma_t* const raw_p = (RawParser_dma_t*)captured;
//	RawParser_dma_receiveArray(raw_p, data, size);

	RawParser_dynamic_t* const raw_p = (RawParser_dynamic_t*)captured;

	rawParser_dynamic_receiveArray(raw_p, data, size);
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


static void pingWorker(u8* const data, const reg size, const u8 id, void* const ctx)
{
	ProtocolSender* const parrent = (ProtocolSender*)ctx;
	pool_container_t* const pack_pool = parrent->getPool();
	RawParser_dma_t* const parser = parrent->getParser();

	u8 t1 = 0x01;
	u8 t2 = 0x11;
	u8 t3 = 0x22;
	u8 t4 = 0x33;
	u16 t5 = 0xAA55;

	//	u8 read = 0;
	//	u16 read16 = 0;
	//	u16 read16_2 = 0;
	//	RawParser_dma_universalRead(parser, sizeof(u8), sizeof(u8), &read);
	//	RawParser_dma_universalRead(parser, sizeof(u16), sizeof(u16), (u8*)&read16);
	//	RawParser_dma_universalRead(parser, sizeof(u16), sizeof(u16), (u8*)&read16_2);

	if(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(pack_pool);
		rawParser_dma_setUserBufferTX(parser, buffer->data);

		RawParser_dma_startTransmittPacket(parser, 10);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &id);
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

static void setKey(u8* const data, const reg size, const u8 id, void* const ctx)
{
	ProtocolSender* const parrent = (ProtocolSender*)ctx;
	pool_container_t* const pack_pool = parrent->getPool();
	RawParser_dma_t* const parser = parrent->getParser();
	Key_tps1htc30Group* const key =  parrent->getKeyClass();

	u8 group_id;
	u8 key_id;
	u8 en;

	RawParser_dma_universalRead(parser, sizeof(u8), sizeof(u8), &group_id);
	RawParser_dma_universalRead(parser, sizeof(u8), sizeof(u8), &key_id);
	RawParser_dma_universalRead(parser, sizeof(u8), sizeof(u8), &en);

	key->set_out(en, group_id, key_id);

	if(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(pack_pool);
		rawParser_dma_setUserBufferTX(parser, buffer->data);

		RawParser_dma_startTransmittPacket(parser, 1);
		RawParser_dma_universalWrite(parser, sizeof(u8), sizeof(u8), &id);
		RawParser_Frame_t*const frame = RawParser_dma_finishTransmittPacket(parser);
		buffer->size = frame->size;
		poolContainer_nextWritePos(pack_pool);
	}
}



status_t ProtocolSender::init(UART_HandleTypeDef *const huart)
{
	M_Assert_BreakSaveCheck(huart == NULL, M_EMPTY, return ERROR_FAIL, "null input");

	poolContainer_init(&pack_pool, 4, sizeof(pool_elem) + D_RAW_P_TX_BUF_SIZE);

	M_Assert_BreakSaveCheck(UART_Init(&stm32_uart, huart, 128) != STATUS_OK,
			M_EMPTY, return ERROR_FAIL,
					"uart init fail");



	M_Assert_BreakSaveCheck(rawParser_dma_init(&raw_p, 0x1a) != D_RAW_P_OK,
			M_EMPTY, return ERROR_FAIL,
					"kbus init fail");
	rawParser_dynamic_init_default(&rawP_new, 0x1a, 256, 256, 0, CRC8);

	UART_SetRxCallback(&stm32_uart, uart_rx_callback, &rawP_new/*&raw_p*/);
	UART_SetTxcallback(&stm32_uart, uart_tx_callback, &pack_pool);
	//	UART_SetErrorCallback(&self->stm32_uart, uart_error_callback, self);



	pool_elem* const buffer = (pool_elem*) poolContainer_getWriteBuffer(&pack_pool);
	rawParser_dma_setUserBufferTX(&raw_p, buffer->data);

	M_Assert_BreakSaveCheck(CallbackManager_init(&callb, 5, this),
			M_EMPTY, return ERROR_FAIL,
					"callback manager init fail");
	CallbackManager_subscribe(&callb, 0x01, pingWorker);
	CallbackManager_subscribe(&callb, 0x02, setKey);


	m_keyGroup = Key_tps1htc30Group::getInstance();
	return STATUS_OK;
}

void ProtocolSender::proceed(const uint32_t current_time)
{

	rawParser_dynamic_proceed(&rawP_new);
//	// proceed TX
//	{
//		if(!UART_IsTxBusy(&stm32_uart) && FIFO_NOT_EMPTY(&pack_pool)) {
//			pool_elem* const buffer = (pool_elem*) poolContainer_readBuffer(&pack_pool);
//			UART_SendBuffer(&stm32_uart, buffer->data, buffer->size);
//		}
//	}
//
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

