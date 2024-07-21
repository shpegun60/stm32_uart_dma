/*
 * CrcCalculator.h
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#ifndef CHAOS_PP_CRC_CRCCALCULATOR_H_
#define CHAOS_PP_CRC_CRCCALCULATOR_H_

#include "CrcBase.h"

class CrcCalculator : public CrcBase
{
public:
	typedef void (*crc_init_f)(void* const crc);
	typedef void (*crc_array_f)(void* const crc, const u8* const data, const reg len);
	typedef void (*crc_byte_f)(void* const crc, const u8 data);

public:
	// constructor
	explicit CrcCalculator(const u8 size, crc_init_f init_f, crc_array_f array_f, crc_byte_f byte_f);
	// Copy constructor
	CrcCalculator(const CrcCalculator& other);
	//destructor
	~CrcCalculator() = default;

	inline void init() { m_init_f(m_crc); }
	inline void array(const u8* const data, const reg len) { m_array_f(m_crc, data, len); }
	inline void byte(const u8 data) { m_byte_f(m_crc, data); }

private:
	// Function pointers for CRC operations
	crc_init_f m_init_f = nullptr;
	crc_array_f m_array_f = nullptr;
	crc_byte_f m_byte_f = nullptr;
};

#endif /* CHAOS_PP_CRC_CRCCALCULATOR_H_ */
