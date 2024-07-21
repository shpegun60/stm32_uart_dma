/*
 * RawParserDynamic.h
 *
 *  Created on: Jul 18, 2024
 *      Author: admin
 */

#ifndef CHAOS_PP_KBUS_RAWPARSERDYNAMIC_H_
#define CHAOS_PP_KBUS_RAWPARSERDYNAMIC_H_

#include "buffers/RingBuff.h"
#include "crc/CrcCalculator.h"
#include "crc/CrcFactory.h"

#include <map>
#include <functional>

class RawParserDynamic
{
public:

	using sender_t = std::function<void(RingBuff&)>;
	using receiver_t = std::function<void(RingBuff&, const u8 id)>;


	struct Init {
		u8 SB;
		reg rxRingSize;
		reg rxFrameSize;
		void* rxBuffer;
		reg txFrameSize;
		void* txBuffer;
		CrcFactory::CrcStrategy crc_strategy;
	};

private:
	struct RX_handler {
		// flags
		bool 			triggerSB = false;      	// trigger for read start byte
		reg				tmpReceiveLen = 0;			// receive length temp register
		u8 rxState;
		RingBuff 		inputRing;
		RingBuff 		frameStream;
		//crc
		CrcCalculator* 	checkCRC 	= nullptr;
		CrcBase* 		rxCRC 		= nullptr;
		u8 crcIndex = 0;
		//functions map
		std::map<u8, receiver_t> handlers;
	};

	struct TX_handler {
		RingBuff 		frameStream;
		CrcCalculator* 	txCRC 	= nullptr;
		reg 			lengthControl = 0;
		sender_t sender = nullptr;
	};
public:
	RawParserDynamic() = default;
	~RawParserDynamic();

	/*
	 * *********************************************************
	 * INIT
	 * *********************************************************
	 */
	bool init(const Init& strategy);					// init parser with next strategy
	void setSender(const sender_t f);					// set TX function
	void subscribe(const u8 id, const receiver_t f);	// set RX functions
	bool writeTo(void* const buff);						// install TX buffer to write

	/*
	 * *********************************************************
	 * RX
	 * *********************************************************
	 */
	// IT ---------------------------------------------------------------
	inline void proceedIncomeByte(const u8 b);
	inline void proceedIncomeBytes(u8* const data, const reg len);
	// LOOP -------------------------------------------------------------
	void proceed(const u32 time);

	// stream for reading data from RX frame buffer
	template<class T> inline RawParserDynamic& operator>>(T& value) { value = m_in.frameStream.get<T>(); return *this; }

	/*
	 * *********************************************************
	 * TX
	 * *********************************************************
	 */
	bool newPack(u16 len);
	RingBuff& endPack();
	template<class T> bool addTx(const T c);

	// stream for pushing data to TX frame buffer
	template<class T> inline RawParserDynamic& operator<<(const T value) { addTx<T>(value); return *this; }


private:
	// RX private-----------------------------------
	void _proceedByte(const u8 b, const bool newFrame);
	inline void _callRxHandler();
	// TX private -----------------------------------
	inline void _pushToTX(const u8 c);

private:
	static constexpr u8 EXTENDED_LEN_CMD 		= 0xFF;
	static constexpr u8 EXTENDED_LEN_SEPARATOR 	= 0xFB;
	u8 SB;
	RX_handler m_in;
	TX_handler m_out;
};

/*
 * *********************************************************
 * RX inline functions realize
 * *********************************************************
 */
inline void RawParserDynamic::proceedIncomeByte(const u8 b)
{
	m_in.inputRing.putc(b);
}

inline void RawParserDynamic::proceedIncomeBytes(u8* const data, const reg len)
{
	m_in.inputRing.put(data, len);
}


inline void RawParserDynamic::_callRxHandler()
{
	const u8 cmd = m_in.frameStream.getc();
	const auto it = m_in.handlers.find(cmd);

	if (it != m_in.handlers.end()) {
		it->second(m_in.frameStream, cmd);
	}
}

/*
 * *********************************************************
 * TX
 * *********************************************************
 */
inline void RawParserDynamic::_pushToTX(const u8 c)
{
	if(c == SB) {
		const u8 arr[2] = {SB, SB};
		m_out.frameStream.put(arr, sizeof(arr));
	} else {
		m_out.frameStream.putc(c);
	}
}

template<class T>
bool RawParserDynamic::addTx(const T c)
{
	const reg remaining_len = m_out.lengthControl;
	if(remaining_len < sizeof(T)) {
		return false;
	}

	// union cast
	union {T from; u8 to[sizeof(T)];} u;
	u.from = c;

	LittleEndianGeneric(&u.from, sizeof(T));

	if(m_out.txCRC) {
		m_out.txCRC->array(u.to, sizeof(T));
	}

	for(u8 i = 0; i != sizeof(T); ++i) {
		_pushToTX(u.to[i]);
	}

	m_out.lengthControl = remaining_len - sizeof(T);

	return true;
}

template<> bool RawParserDynamic::addTx<u8>(const u8 c);

#endif /* CHAOS_PP_KBUS_RAWPARSERDYNAMIC_H_ */
