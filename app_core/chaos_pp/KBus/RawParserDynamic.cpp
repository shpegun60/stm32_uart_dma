/*
 * RawParserDynamic.cpp
 *
 *  Created on: Jul 18, 2024
 *      Author: admin
 */

#include "RawParserDynamic.h"
#include "byte_order/byte_order.h"

RawParserDynamic::~RawParserDynamic()
{
	// call crc destructors
	if(m_in.checkCRC) {
		delete m_in.checkCRC;
		delete m_in.rxCRC;
	}

	if(m_out.txCRC) {
		delete m_out.txCRC;
	}
}

/*
 * *********************************************************
 * INIT
 * *********************************************************
 */

bool RawParserDynamic::init(const Init &strategy)
{
	if((EXTENDED_LEN_CMD != SB) && (SB == 0)) {
		return false;
	}

	SB = strategy.SB;
	// RX init --------------------------------------------------------------
	m_in.inputRing.init(nullptr, strategy.rxRingSize);
	m_in.frameStream.init(strategy.rxBuffer, strategy.rxFrameSize);

	CrcCalculator* const crc = CrcFactory::createCRC(strategy.crc_strategy);
	if(crc) {
		m_in.checkCRC = crc;
		m_in.rxCRC = new CrcBase(*crc);
	}

	// TX init --------------------------------------------------------------
	m_out.frameStream.init(strategy.txBuffer, strategy.txFrameSize);
	m_out.txCRC = CrcFactory::createCRC(strategy.crc_strategy);

	return true;
}

void RawParserDynamic::setSender(const sender_t f)
{
	m_out.sender = f;
}

void RawParserDynamic::subscribe(const u8 id, const receiver_t f)
{
	m_in.handlers[id] = f;
}

bool RawParserDynamic::writeTo(void* const buff)
{
	return m_out.frameStream.installBuffer(buff);
}

/*
 * *********************************************************
 * RX
 * *********************************************************
 */
void RawParserDynamic::_proceedByte(const u8 b, const bool newFrame)
{
	// FSM data fusion -----------------------------
#define RECEIVE_LEN_0					0x00U
#define RECEIVE_LEN_LOW                	0x01U
#define RECEIVE_LEN_HIGH               	0x02U
#define RECEIVE_DATA                   	0x03U
#define RECEIVE_CRC                   	0x04U
	//----------------------------------------------
#define RECEIVE_COMPLEATE              	0x0CU

	if (newFrame) {
		if(m_in.checkCRC) {
			m_in.checkCRC->init();
			m_in.crcIndex = 0;
		}
		m_in.frameStream.clear();
		m_in.tmpReceiveLen = 0;
		m_in.rxState = RECEIVE_LEN_0;
	}

	switch(m_in.rxState) {

	case RECEIVE_LEN_0: {
		if(m_in.checkCRC) {
			m_in.checkCRC->byte(b);
		}

		if(EXTENDED_LEN_CMD == b) {
			m_in.rxState = RECEIVE_LEN_LOW;
		} else {
			const reg rx_len = m_in.tmpReceiveLen = ((b > SB) ? (b - 1U) : b);
			m_in.frameStream.clear();

			if(rx_len > m_in.frameStream.capacity() || rx_len == 0) {
				m_in.rxState = RECEIVE_COMPLEATE;
				return;
			}
			m_in.rxState = RECEIVE_DATA;
		}

		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------
	case RECEIVE_LEN_LOW: {
		if(m_in.checkCRC) {
			m_in.checkCRC->byte(b);
		}

		m_in.tmpReceiveLen = b;    // read low byte
		m_in.rxState = RECEIVE_LEN_HIGH;
		break;}

	case RECEIVE_LEN_HIGH: {
		if(m_in.checkCRC) {
			m_in.checkCRC->byte(b);
		}

		reg rx_len = (static_cast<reg>(b << 8U) | m_in.tmpReceiveLen) & 0x0000FFFFUL ; // read high byte
		LittleEndianGeneric(&rx_len, sizeof(u16));
		m_in.tmpReceiveLen = rx_len;
		m_in.frameStream.clear();

		if(rx_len > m_in.frameStream.capacity() || rx_len == 0) {
			m_in.rxState = RECEIVE_COMPLEATE;
			return;
		}
		m_in.rxState = RECEIVE_DATA;
		break;}

	// ----------------- TWO BYTES LEN -------------------------------------------------------------------------

	case RECEIVE_DATA: {
		m_in.frameStream.putc(b);

		if (/*m_in.frameStream.length()*/m_in.frameStream.getHead() == m_in.tmpReceiveLen) {
			if(m_in.checkCRC && m_in.rxCRC) {
				m_in.rxState = RECEIVE_CRC;
			} else {
				_callRxHandler();
				m_in.rxState = RECEIVE_COMPLEATE;
			}
		}
		break;}

	case RECEIVE_CRC: {
		CrcBase& rx_crc = (*m_in.rxCRC);
		rx_crc[m_in.crcIndex] = b;
		++m_in.crcIndex;

		if(m_in.crcIndex == rx_crc.size()) {
			CrcCalculator& calculated_crc = (*m_in.checkCRC);
			LittleEndianGeneric(rx_crc.crc(), m_in.crcIndex);

			RingBuff::crc_from(calculated_crc, m_in.frameStream);

			if(rx_crc == calculated_crc) {
				_callRxHandler();
			}
			m_in.rxState = RECEIVE_COMPLEATE;
		}
		break;}

	case RECEIVE_COMPLEATE: {

		break;}
	}

#undef RECEIVE_LEN_0
#undef RECEIVE_LEN_LOW
#undef RECEIVE_LEN_HIGH
#undef RECEIVE_DATA
#undef RECEIVE_CRC
	//--------------------------------------------
#undef RECEIVE_COMPLEATE

#undef NEXT_STATE
}

void RawParserDynamic::proceed(const u32 time)
{
	u8 buffer[10];

	while(!m_in.inputRing.isEmpty()) {
		const reg len = m_in.inputRing.get(buffer, sizeof(buffer));

		for(reg i = 0; i != len; ++i) {
			const u8 ch = buffer[i];

			if (m_in.triggerSB) {
				if(SB == ch) { 		//{SB}{SB} -> {SB}
					_proceedByte(ch, false);
				} else { 			//{SB}{!SB} -> {SB} and newframe
					_proceedByte(ch, true);
				}
				m_in.triggerSB = false;
			} else if (SB == ch) { 	//{!SB}{SB} -> set flag and skip step
				m_in.triggerSB = true;
			} else { 				//{!SB}{!SB} -> {!SB}
				_proceedByte(ch, false);
			}
		}
	}
}


/*
 * *********************************************************
 * TX
 * *********************************************************
 */
bool RawParserDynamic::newPack(u16 len)
{
	if(len == 0 || len > m_out.frameStream.capacity()) {
		return false;
	}
	// do logic ------------------------------------------------
	if(m_out.txCRC) {
		m_out.txCRC->init();
	}

	m_out.frameStream.clear();
	m_out.lengthControl = len;

	m_out.frameStream.putc(SB);

	if(len > EXTENDED_LEN_SEPARATOR) {
		LittleEndianGeneric(&len, sizeof(u16));

		const u8 len_l = static_cast<u8>(len & 0xFF);
		const u8 len_h = static_cast<u8>((len >> 8) & 0xFF);

		m_out.frameStream.putc(EXTENDED_LEN_CMD);
		_pushToTX(len_l);
		_pushToTX(len_h);

		if(m_out.txCRC) {
			const u8 arr[3] = {EXTENDED_LEN_CMD, len_l, len_h};
			m_out.txCRC->array(arr, sizeof(arr));
		}

	} else {
		const u16 len_reg = (len >= SB) ? (len + 1U) : len;
		m_out.frameStream.putc(len_reg);

		if(m_out.txCRC) {
			m_out.txCRC->byte(len_reg);
		}
	}
	return true;
}

RingBuff& RawParserDynamic::endPack()
{
	// fill remaining len
	if(m_out.txCRC) {

		while(m_out.lengthControl--) {
			m_out.frameStream.putc(0x00);
			m_out.txCRC->byte(0x00);
		}

		CrcCalculator& crc = *(m_out.txCRC);
		u8 const tx_crc_size = crc.size();
		LittleEndianGeneric(crc.crc(), tx_crc_size);

		for(u8 i = 0; i != tx_crc_size; ++i) {
			_pushToTX(crc[i]);
		}

	} else {
		while(m_out.lengthControl--) {
			m_out.frameStream.putc(0x00);
		}
	}

	if(m_out.sender) {
		m_out.sender(m_out.frameStream);
	}
	return m_out.frameStream;
}


template<> bool RawParserDynamic::addTx<u8>(const u8 c)
{
	const reg remaining_len = m_out.lengthControl;
	if(remaining_len < sizeof(u8)) {
		return false;
	}

	if(m_out.txCRC) {
		m_out.txCRC->byte(c);
	}

	_pushToTX(c);

	m_out.lengthControl = remaining_len - sizeof(u8);
	return true;
}

