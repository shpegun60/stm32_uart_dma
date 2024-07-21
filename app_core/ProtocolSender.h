/*
 * ProtocolSender.h
 *
 *  Created on: Jul 10, 2024
 *      Author: admin
 */

#ifndef PROTOCOLSENDER_H_
#define PROTOCOLSENDER_H_

#include "uart/stm32_uart.h"
#include "buffers/pool_container.h"
#include "raw_parser/rawp_dynamic.h"

#include "callback_manager/callback_manager.h"

#include "TPS1HTC30/Keytps1htc30Group.h"
#include "timer/TimingManager.h"


class ProtocolSender
{
public:
	struct pool_elem {
		u16 size;
		u8 data[];
	};

	ProtocolSender() = default;
	~ProtocolSender() = default;

	status_t init(UART_HandleTypeDef* const huart);
	void proceed(const uint32_t current_time);

	// getters
	inline pool_container_t* const getPool() { return &pack_pool; }
	inline RawParser_dynamic_t* const getParserNew() { return &rawP_new; }
	inline stm32_DMA_uart_t* const getUart() { return &stm32_uart; }
	inline Key_tps1htc30Group* const getKeyClass() { return m_keyGroup; }
	inline CallbackManager_t* const getCmdWorker() { return &callb; }
private:
	pool_container_t pack_pool;
	RawParser_dynamic_t rawP_new;
	stm32_DMA_uart_t stm32_uart;
	CallbackManager_t callb;

	// main class to synchronize data
	Key_tps1htc30Group* m_keyGroup = nullptr;
	TimingManager proto_timer;

};

#endif /* PROTOCOLSENDER_H_ */
