/*
 * RawParser.cpp
 *
 *  Created on: Mar 4, 2024
 *      Author: oleksii
 */

#include "RawParser.h"

#include <cstring>

void KMsgTools::pointerCopy(unsigned int n, void* from, void* to, KMsgTools::DIR direction)
{
    unsigned char * fromPtr = (unsigned char *)from;
    unsigned char * toPtr = (unsigned char *)to;

    if(direction) {
        while(n--) {
            *toPtr++ = *(fromPtr + n);
            //(*pos)++;
        }
    } else {
        while(n--) {
            *toPtr++ = *fromPtr++;
            //(*pos)++;
        }
    }
}

//=============================    KIncomeMsg   =====================================//


KIncomeMsg::KIncomeMsg(uint8_t lenght)
{
	//printf("KIncomeMsg::KIncomeMsg(%d)\n", lenght);
	data = new uint8_t[lenght];
	len = lenght;
}

KIncomeMsg::~KIncomeMsg()
{
	//printf("KIncomeMsg::~KIncomeMsg\n");
	delete [] data;
}

uint8_t* KIncomeMsg::read_array(uint8_t arr_len, int pos)
{
	if (pos>=0) m_readPos = pos;
	if ((m_readPos + arr_len) > len) {
		return nullptr;
	}
	auto res  = new uint8_t[arr_len];
	memcpy(res, data+m_readPos, arr_len);
	return res;
}


//=================================   KOutcomeMsg =====================================//

KOutcomeMsg::KOutcomeMsg(uint8_t id, uint8_t cap):
		m_capacity(cap),
		m_writePos(0)
{
	m_data = new uint8_t[cap+3];
	m_data[0] = m_startByte;
	m_writePos = 2;
	//add_u8(0x00); //len
	add_u8(id);   //
}

KOutcomeMsg::~KOutcomeMsg()
{
	delete [] m_data;
}

void KOutcomeMsg::add_array(uint8_t data_len, uint8_t* data)
{
	if ((m_writePos + data_len) > m_capacity)
			_resize(m_writePos+2*data_len+12);

	for (int  i = 0; i  < data_len; ++i) {
		add_u8(data[i]);
	}
	//memcpy(m_data+m_writePos, data, data_len);
	//m_writePos += data_len;
	//m_dataLen += data_len;
}

void  KOutcomeMsg::_resize(uint8_t len)
{
	if (m_capacity>= len) return;

	auto newData =  new uint8_t[len];
	memcpy(newData, m_data,m_capacity);
	delete [] m_data;
	m_data = newData;
	m_capacity = len;
}

void KOutcomeMsg::_addCrc()
{
	//add length
	m_data[1] = (m_dataLen >= m_startByte) ? m_dataLen + 1 : m_dataLen;
	//add crc
	uint8_t crc = 0xFF;
	bool lastWasSB = 0;
	for (int  i = 1; i < m_writePos; ++i) {
		if (!lastWasSB && m_data[i] == m_startByte) {
			lastWasSB = 1;
			continue;
		}
		lastWasSB = 0;
		crc = KMsgTools::proceedCrc(crc, m_data[i]);
	}
	//m_data[m_writePos] = m_crc;
	add_u8(crc);
}


//=============================    KRaawParser  =====================================//

RawParser::RawParser()
{
	m_frameCrc = 0xFF;
}

RawParser::~RawParser()
{
	// TODO Auto-generated destructor stub
}

void RawParser::init(uint8_t SB, uint8_t circBufferSize)
{
	m_startByte = SB;
	if (m_buffer  != nullptr) {
		delete [] m_buffer;
	}
	m_buffer = new uint8_t[circBufferSize];
	m_bufferSize = circBufferSize;
	m_writePos = 0;
	m_readPos = 0;
}

std::shared_ptr<KIncomeMsg> RawParser::getAvaliableMsg()
{
	bool packReady = 0;

	auto proceedUnshieldedDataStream = [&](uint8_t ch, bool newFrame){
		if (newFrame) {
			m_frameCrc = 0xFF;
			m_pos = 0;
			if (m_receivePackNow) {
				_proceedError(RawParser::SB_RECEIVED_DURING_OLD_PACK);
			}
			m_receivePackNow = true;
		}

		if (!m_receivePackNow)
			return;

		if (m_pos == 0) {                                                //receive len byte
			auto len = ch;
			if (len > m_startByte) len -= 1;
			_allocateIncomeMsg(len);
		} else if ((m_pos - 1) < m_msg->lenght()) {                      // receive data byte
			m_msg->data[m_pos-1] = ch;
		} else if ((m_pos - 1) == m_msg->lenght() && m_frameCrc == ch) { // receive crc byte
			packReady = true;
			m_receivePackNow = 0;
		} else {
			_proceedError(RawParser::INCORRECT_CRC);
			m_receivePackNow = 0;
			return;
		}

		m_pos++;
		m_frameCrc = KMsgTools::proceedCrc(m_frameCrc, ch);
	};

	while (m_readPos != m_writePos) {
		uint8_t ch = m_buffer[m_readPos];
		m_readPos = (m_readPos + 1) % m_bufferSize;

		if (m_lastWasSB) {                                  // New frame
			if(ch == m_startByte) {                         // {SB}{SB} -> {SB}
				proceedUnshieldedDataStream(ch, false);
			} else {                               		    // {SB}{!SB} -> {SB} and new frame
				proceedUnshieldedDataStream(ch, true);
			}
			m_lastWasSB = false;
		} else if (ch == m_startByte)	{                  	// {!SB}{SB} -> set flag and skip step
			m_lastWasSB = true;
		} else	{                                           // {!SB}{!SB} -> {!SB}
			proceedUnshieldedDataStream(ch, false);
		}

		if (packReady) {
			return _moveMsgToPtr();
		}

	}

	return std::shared_ptr<KIncomeMsg>();
}

void RawParser::_allocateIncomeMsg(uint8_t len)
{

	if (m_msg != nullptr) {
		delete m_msg;
	}

	m_msg  = new KIncomeMsg(len);
}

std::shared_ptr<KIncomeMsg>  RawParser::_moveMsgToPtr()
{
	std::shared_ptr<KIncomeMsg> res(m_msg);
	m_msg = nullptr;

	return res;
}

uint8_t KMsgTools::proceedCrc(uint8_t crc, uint8_t ch) {
	crc ^= ch;
	for (int i = 0; i < 8; i++)
		crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
	return crc;
}

//======================================  KBUS =======================================//

KBus::KBus()
{
	m_parser.init(0x1A, 60);
}

void KBus::proceedIncomeBytes(uint8_t* bs, uint8_t len)
{

	for (int i =0;i <len;++i) {
		m_parser.proceedByte(bs[i]);
	}
}

void KBus::addHandler(uint8_t msgId, std::function<void(std::shared_ptr<KIncomeMsg>)> handler)
{
	m_handlers[msgId] = handler;
}

void KBus::proceed(uint32_t time)
{
	auto msg = m_parser.getAvaliableMsg();
	if (msg->lenght() == 0) return;
	auto id = msg->read_u8();
	if (auto it = m_handlers.find(id); it != m_handlers.end()) it->second(msg);
}

std::shared_ptr<KOutcomeMsg> KBus::createMsg(uint8_t id, uint8_t capacity)
{
	return std::shared_ptr<KOutcomeMsg>(new KOutcomeMsg(id, capacity));
}

void KBus::send(std::shared_ptr<KOutcomeMsg> msg)
{
	if(m_sender == nullptr) return;

	msg->_addCrc();
	m_msgToSend.push(msg);
	m_sender(msg->m_data,  msg->m_writePos);
}

void KBus::proceedSendFinished()
{
	m_msgToSend.pop();
}

void KBus::setSender(std::function<void(uint8_t* ,uint8_t)> f)
{
	m_sender = f;
}
