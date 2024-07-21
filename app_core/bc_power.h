/*
 * bc_power.h
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */

#ifndef BC_POWER_H_
#define BC_POWER_H_

#include "adc/Stm32ADCdma.h"
#include "Kbus/RawParserDynamic.h"

#include "uart/stm32_uart.h"
#include "buffers/PoolContainer.h"
/*
 * ******************************************************
 * MAIN SINGLETONE CLASS
 * ******************************************************
 */
class BC_Power
{
	BC_Power();
	~BC_Power() = default;

public:
	static BC_Power& instance();
	void loop();

private:
	struct txPoolElem {
		u16 size;
		u8 data[];
	};

	void init_hw();
	static void uart_tx_callback(const status_t status, PoolContainer* const pool);
	static void uart_rx_callback(u8* const data, u32 const size, RawParserDynamic* const raw_p);

private:
	Stm32ADC_dma m_adc;

	// communication
	stm32_DMA_uart_t m_uart;
	PoolContainer m_txPool;
	RawParserDynamic m_parser;
};

#endif /* BC_POWER_H_ */
