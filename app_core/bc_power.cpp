/*
 * bc_power.cpp
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */

#include "main.h"
#include "my_cpp_types.h"

#include "bc_power.h"

BC_Power::BC_Power()
{
	// TODO Auto-generated constructor stub
	//init pool buffer -------------------------------
	m_txPool.init(4, sizeof(txPoolElem) + 128);
	txPoolElem* const elem = static_cast<txPoolElem*>(m_txPool.getWriteBuffer());

	//init uart -------------------------------
	extern UART_HandleTypeDef huart1;
	UART_Init(&m_uart, &huart1, 128);
	UART_SetTxcallback(&m_uart, reinterpret_cast<uart_tx_callback_t>(uart_tx_callback), &m_txPool);
	UART_SetRxCallback(&m_uart, reinterpret_cast<uart_rx_callback_t>(uart_rx_callback), &m_parser);

	//init rawparser -------------------------------
	RawParserDynamic::Init rawp_init = {
			.SB = 0x1A,
			.rxRingSize = 128,
			.rxFrameSize = 256,
			.rxBuffer = nullptr,
			.txFrameSize = 128,
			.txBuffer = elem->data,
			.crc_strategy = CrcFactory::CrcStrategy::CRC8
	};

	m_parser.init(rawp_init);
	m_parser.subscribe(0x01, [&](RingBuff& data, const u8 id) {
		u8 c;
		//u8 d = data.get<u8>();
		//data >> b >> c;
//		m_parser >> b >> c;

		//data.put<u8>(12345);

		if(!m_txPool.isFull()) {
//			txPoolElem* const elem = static_cast<txPoolElem*>(m_txPool.getWriteBuffer());
//			m_parser.writeTo(elem->data);

			m_parser.newPack(10);
			m_parser.addTx(id);
			//m_parser << id;
			RingBuff& tx_data = m_parser.endPack();

//			elem->size = tx_data.getHead();
//			m_txPool.nextWritePos();
		}
	});


	init_hw();
}


BC_Power& BC_Power::instance()
{
	static BC_Power instance;
	return instance;
}

void BC_Power::loop()
{
	m_adc.start();

	while(true) {
		const u32 time = HAL_GetTick();
		m_adc.proceed(time);

		m_parser.proceed(time);
		if(!m_txPool.isEmpty() && !UART_IsTxBusy(&m_uart)) {
			txPoolElem* const elem = static_cast<txPoolElem*>(m_txPool.getWriteBuffer());
			UART_SendBuffer(&m_uart, elem->data, elem->size);
		}
	}
}

void BC_Power::uart_tx_callback(const status_t status, PoolContainer* const pool)
{
	if(!pool->isEmpty()) {
		pool->nextReadPos();
	}
}

void BC_Power::uart_rx_callback(u8* const data, u32 const size, RawParserDynamic* const raw_p)
{
	raw_p->proceedIncomeBytes(data, size);
}


//--------------------------------------------------------
void BC_Power::init_hw()
{
	//init adc ----------------------------------------
	extern ADC_HandleTypeDef hadc1;
	Stm32ADC_dma::Init adc_init = {
			.hadc = &hadc1,
			.filter = 1/512
	};

	m_adc.init(adc_init);
}
