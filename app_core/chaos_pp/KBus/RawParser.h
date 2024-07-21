/*
 * RawParser.h
 *
 *  Created on: Mar 4, 2024
 *      Author: oleksii
 */

#ifndef RAWPARSER_H_
#define RAWPARSER_H_

#include <stdint.h>
#include <map>
#include <queue>
#include <memory>
#include <functional>


class KMsgTools
{
public:
	enum DIR {LSB = 0x00, MSB = 0x01};
	static void pointerCopy(unsigned int n, void* from, void* to, KMsgTools::DIR direction);
	static uint8_t proceedCrc(uint8_t crc, uint8_t ch);
};

class KIncomeMsg
{
	friend class RawParser;
public:
	~KIncomeMsg();

	template<class T> T read(int pos = -1);
	inline uint8_t read_u8 (int pos =-1) { return read<uint8_t>(pos); }
	inline uint8_t read_u16(int pos =-1) { return read<uint16_t>(pos); }

	uint8_t* read_array(uint8_t len,int pos =-1);

	inline uint8_t lenght() { return len; }
	inline bool atEnd() { return m_readPos == len; }
	//uint8_t error();


protected:
	KIncomeMsg(uint8_t len);

private:
	//uint8_t error = 0x00;
	uint8_t len = 0;
	uint8_t *data = nullptr;
	uint8_t m_readPos = 0;

	//inline void _pointerCopy(unsigned int n, void* from, void* to, unsigned int* pos, unsigned char direction);
};

class KOutcomeMsg
{
	friend class KBus;
public:
	~KOutcomeMsg();
	template<class T>
	void add(T);

	inline void add_u8(uint8_t v) {add(v);}
	inline void add_u16(uint16_t v) {add(v);}
	void add_array(uint8_t len, uint8_t* data);

protected:
	KOutcomeMsg(uint8_t id, uint8_t cap);


private:
	uint8_t *m_data;
	uint8_t m_capacity;
	uint8_t m_writePos;
	uint8_t m_startByte = 0x1A;
	uint8_t m_dataLen = 0; // data lenght without shielding {SB}{SB}

	const uint8_t m_byteOrder = KMsgTools::LSB;
	void  _resize(uint8_t len);
	void _addCrc();
};


class RawParser
{
public:
	enum Error{NONE, INCORRECT_CRC, SB_RECEIVED_DURING_OLD_PACK};

	RawParser();
	virtual ~RawParser();

	void init(uint8_t SB, uint8_t circBufferSize);

	inline void proceedByte(uint8_t);
	std::shared_ptr<KIncomeMsg> getAvaliableMsg();

private:
	inline void _allocateIncomeMsg(uint8_t len);
	inline std::shared_ptr<KIncomeMsg> _moveMsgToPtr();
	//uint8_t _proceedCrc(uint8_t crc, uint8_t ch);
	void _proceedError(uint8_t) {};

private:

	uint8_t m_startByte = 0x1A;

	// input circular buffer
	uint8_t *m_buffer = nullptr;
	uint8_t m_bufferSize = 0;
	uint8_t m_writePos = 0;
	uint8_t m_readPos = 0;

	//
	KIncomeMsg* m_msg = nullptr;
	uint8_t m_frameCrc = 0x00;
	uint8_t m_pos = 0;
	uint8_t m_lastWasSB = 0;


	//input frame buffer
	//uint8_t m_frameLen = -1;
	//uint8_t *m_frameBuffer = nullptr;
	//uint8_t m_frameBufferLen;

	bool m_receivePackNow = 0;
};




template<class T> T KIncomeMsg::read(int pos)
{
	if (pos>=0) m_readPos = pos;

	uint8_t vlen = sizeof(T);
	if (m_readPos + vlen > len) {
		return T();
	}
	T res;
	KMsgTools::pointerCopy(vlen, data + m_readPos, &res, KMsgTools::LSB);
	m_readPos += vlen;
	return res;
}


template<class T> void KOutcomeMsg::add(T v)
{
	auto vlen = sizeof(T);
	if ((m_writePos + vlen) > m_capacity)
		_resize(m_writePos+2*vlen+12);

	auto fromPtr = reinterpret_cast<uint8_t*>(&v);
	auto toPtr = m_data + m_writePos;
	m_dataLen += vlen;

	if (m_byteOrder == KMsgTools::LSB) {
		while(vlen--) {
			auto ch = *(fromPtr + vlen);

			*toPtr++ = ch;
			if (ch == m_startByte) {
				*toPtr++ = ch;
				m_writePos += 2;
			} else {
				m_writePos += 1;
			}
		}
	} else {
		while(vlen--) {
			auto ch = *fromPtr++;
			*toPtr++ = ch;
			if (ch == m_startByte) {
				*toPtr++ = ch;
				m_writePos += 2;
			} else {
				m_writePos += 1;
			}
		}
	}



}

void RawParser::proceedByte(uint8_t byte)
{
	m_buffer[m_writePos] = byte;
	m_writePos = (m_writePos + 1) % m_bufferSize;
}


class KBus
{
public:
	KBus();
	//interface to periph
	inline void proceedIncomeByte(uint8_t);
	inline void proceedIncomeBytes(uint8_t*, uint8_t);
	void setSender(std::function<void(uint8_t* ,uint8_t)>);
	void proceedSendFinished();

	//inteface to BL
	void proceed(uint32_t time = 0);
	void addHandler(uint8_t msgId, std::function<void(std::shared_ptr<KIncomeMsg>)>);
	void send(std::shared_ptr<KOutcomeMsg>);
	static std::shared_ptr<KOutcomeMsg> createMsg(uint8_t id, uint8_t capacity);

private:

	std::map<uint8_t, std::function<void(std::shared_ptr<KIncomeMsg>)>> m_handlers;
	RawParser m_parser;

	std::function<void(uint8_t* ,uint8_t)> m_sender = nullptr;
	std::queue<std::shared_ptr<KOutcomeMsg>> m_msgToSend;
};


void KBus::proceedIncomeByte(uint8_t b)
{
	m_parser.proceedByte(b);
}

#endif /* RAWPARSER_H_ */
