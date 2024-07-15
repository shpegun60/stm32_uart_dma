/*
 * ProtocolSender.cpp
 *
 *  Created on: Jul 10, 2024
 *      Author: admin
 */

#include "ProtocolSender.h"
#include "assertsEngine/assert_engine.h"

static void uart_rx_callback(uint8_t const * data, uint32_t const size, RawParser_dynamic_t* const raw_p)
{
	rawP_dynamic_receiveArray(raw_p, data, size);
}

static void uart_tx_callback(const status_t status, pool_container_t* const pack_pool)
{
	poolContainer_nextReadPos(pack_pool);
}
//
//static void uart_error_callback(const status_t status, void* const captured)
//{
//	stm32_uart_communicator_t* const self = (stm32_uart_communicator_t*)captured;
//}


static void pingWorker(ringbuf_t* const data, const u8 id, ProtocolSender* const ctx)
{
	RawParser_dynamic_t* const parser = ctx->getParserNew();
	pool_container_t* const pack_pool = ctx->getPool();

	if(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = static_cast<ProtocolSender::pool_elem*>(poolContainer_getWriteBuffer(pack_pool));

		rawP_dynamic_install_txbuf(parser, buffer->data);
		rawP_dynamic_startTransmittPacket(parser, 3);

		rawp_stream_add_u8(parser, id);
		rawp_stream_add_u8(parser, 0xdd);
		rawp_stream_add_u8(parser, 0xee);

		ringbuf_t* const ring = rawP_dynamic_finishTransmittPacket(parser);
		buffer->size = ring->base.head;
		poolContainer_nextWritePos(pack_pool);
	}
}

static void setKey(ringbuf_t* const data, const u8 id, ProtocolSender* const ctx)
{
	RawParser_dynamic_t* const parser = ctx->getParserNew();
	pool_container_t* const pack_pool = ctx->getPool();
	Key_tps1htc30Group* const key =  ctx->getKeyClass();

	const u8 group_id = ringbuf_getc(data);
	const u8 key_id = ringbuf_getc(data);
	const u8 en = ringbuf_getc(data);

	key->set_out(en, group_id, key_id);

	if(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(pack_pool);
		rawP_dynamic_install_txbuf(parser, buffer->data);

		rawP_dynamic_startTransmittPacket(parser, 10);
		rawp_stream_add_u8(parser, id);
		ringbuf_t* const ring = rawP_dynamic_finishTransmittPacket(parser);
		buffer->size = ring->base.head;
		poolContainer_nextWritePos(pack_pool);
	}
}


static void rx_callback(ringbuf_t* const rxStream, ProtocolSender* const self)
{
	RawParser_dynamic_t* const parser = self->getParserNew();
	pool_container_t* const pack_pool = self->getPool();
	CallbackManager_t* const callb = self->getCmdWorker();

	const u8 cmd = ringbuf_getc(rxStream);
	CallbackManager_proceed(callb, cmd, rxStream);

	while(!FIFO_IS_FULL(pack_pool)) {
		ProtocolSender::pool_elem* const buffer = static_cast<ProtocolSender::pool_elem*>(poolContainer_getWriteBuffer(pack_pool));
		rawP_dynamic_install_txbuf(parser, buffer->data);

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

	RawP_dynamic_strategy_t rawP_init = {
			.packStart = 0x1a,
			//rx -------------
			.rxBufferSize = 128,
			.rxFrameSize = 256,
			.rxFrameBuff = NULL,
			//tx--------------
			.txFrameSize = 128,
			.txFrameBuff = buffer->data,
			.crc_strategy = CRC8
	};

	rawP_dynamic_init(&rawP_new, &rawP_init);
	rawP_dynamic_subscribe(&rawP_new, reinterpret_cast<RawPdynamicCallback>(rx_callback), this);


	UART_SetRxCallback(&stm32_uart, reinterpret_cast<uart_rx_callback_t>(uart_rx_callback), &rawP_new);
	UART_SetTxcallback(&stm32_uart, reinterpret_cast<uart_tx_callback_t>(uart_tx_callback), &pack_pool);
	//	UART_SetErrorCallback(&self->stm32_uart, uart_error_callback, self);

	CallbackManager_init(&callb, 5, this);
	CallbackManager_subscribe(&callb, 0x01, reinterpret_cast<CallbackWorker>(pingWorker));
	CallbackManager_subscribe(&callb, 0x02, reinterpret_cast<CallbackWorker>(setKey));


	m_keyGroup = Key_tps1htc30Group::getInstance();
	return STATUS_OK;
}

void ProtocolSender::proceed(const uint32_t current_time)
{
	// proceed RX
	rawP_dynamic_proceed(&rawP_new);

	// proceed TX
	{
		if(!UART_IsTxBusy(&stm32_uart) && FIFO_NOT_EMPTY(&pack_pool)) {
			pool_elem* const buffer = static_cast<pool_elem*>(poolContainer_readBuffer(&pack_pool));
			UART_SendBuffer(&stm32_uart, buffer->data, buffer->size);
		}
	}

	// Periodic send
	{
		if(proto_timer.isExpired()) {

			if(FIFO_NOT_FULL(&pack_pool)) {
				ProtocolSender::pool_elem* const buffer = (ProtocolSender::pool_elem*) poolContainer_getWriteBuffer(&pack_pool);
				rawP_dynamic_install_txbuf(&rawP_new, buffer->data);


				u8 groups = m_keyGroup->getGroups();
				u8 key_0 = m_keyGroup->getKeys(0);
				u8 key_1 = m_keyGroup->getKeys(1);
				reg len = 3 + (key_0 + key_1) * 8;

				rawP_dynamic_startTransmittPacket(&rawP_new, len);
				rawp_stream_add_u8(&rawP_new, groups);
				rawp_stream_add_u8(&rawP_new, key_0);
				rawp_stream_add_u8(&rawP_new, key_1);


				m_keyGroup->iterateAll([this](Key_tps1htc30Group::KeyGroup* key, uint8_t group_id, uint8_t key_id) -> bool {
					u8 err = (u8)key->err[key_id];
					u8 en = (u8)key->out_state[key_id];

					rawp_stream_add_u8(&rawP_new, group_id);
					rawp_stream_add_u8(&rawP_new, key_id);
					rawp_stream_add_u8(&rawP_new, en);
					rawp_stream_add_u8(&rawP_new, err);
					rawp_stream_add_f64(&rawP_new, key->current[key_id]);
					return false;
				});

				ringbuf_t* const ring = rawP_dynamic_finishTransmittPacket(&rawP_new);
				buffer->size = ring->base.head;
				poolContainer_nextWritePos(&pack_pool);
			}

			proto_timer.start(50);
		}

	}

}

