/*
 * CrcFactory.cpp
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */

#include "CrcFactory.h"
#include "crc/my_crc_port.h"
// crc ----------------

#ifdef _MY_CRC8_ENA
#	include "crc/crc8.h"
#endif /* _MY_CRC8_ENA */

#ifdef _MY_CRC16_ENA
#	include "crc/crc16.h"
#endif /* _MY_CRC16_ENA */

#ifdef _MY_CRC32_ENA
#	include "crc/crc32.h"
#endif /* _MY_CRC32_ENA */

#ifdef _MY_CRC64_ENA
#	include "crc/crc64.h"
#endif /* _MY_CRC64_ENA */


CrcCalculator* const CrcFactory::createCRC(const CrcStrategy type)
{
	switch(type) {

	case CrcStrategy::CRC8 : {
#ifdef _MY_CRC8_ENA

		CrcCalculator* const crc = new CrcCalculator(
				sizeof(u8),
				reinterpret_cast<CrcCalculator::crc_init_f>(_MY_CRC8_INIT_BASE),
				reinterpret_cast<CrcCalculator::crc_array_f>(_MY_CRC8_ARRAY_BASE),
				reinterpret_cast<CrcCalculator::crc_byte_f>(_MY_CRC8_BYTE_BASE));
		crc->init();

		return crc;
#else
		return nullptr;
#endif /* _MY_CRC8_ENA */
		break;}

	case CrcStrategy::CRC16: {
#ifdef _MY_CRC16_ENA
		CrcCalculator* const crc = new CrcCalculator(
				sizeof(u8),
				reinterpret_cast<CrcCalculator::crc_init_f>(_MY_CRC16_INIT_BASE),
				reinterpret_cast<CrcCalculator::crc_array_f>(_MY_CRC16_ARRAY_BASE),
				reinterpret_cast<CrcCalculator::crc_byte_f>(_MY_CRC16_BYTE_BASE));
		crc->init();
#else
		return nullptr;
#endif /* _MY_CRC16_ENA */
		break;}

	case CrcStrategy::CRC32: {
#ifdef _MY_CRC32_ENA
		CrcCalculator* const crc = new CrcCalculator(
				sizeof(u8),
				reinterpret_cast<CrcCalculator::crc_init_f>(_MY_CRC32_INIT_BASE),
				reinterpret_cast<CrcCalculator::crc_array_f>(_MY_CRC32_ARRAY_BASE),
				reinterpret_cast<CrcCalculator::crc_byte_f>(_MY_CRC32_BYTE_BASE));
		crc->init();
#else
		return nullptr;
#endif /* _MY_CRC8_ENA */
		break;}

	case CrcStrategy::CRC64: {
#ifdef _MY_CRC64_ENA
		CrcCalculator* const crc = new CrcCalculator(
				sizeof(u8),
				reinterpret_cast<CrcCalculator::crc_init_f>(_MY_CRC64_INIT_BASE),
				reinterpret_cast<CrcCalculator::crc_array_f>(_MY_CRC64_ARRAY_BASE),
				reinterpret_cast<CrcCalculator::crc_byte_f>(_MY_CRC64_BYTE_BASE));
		crc->init();
#else
		return nullptr;
#endif /* _MY_CRC64_ENA */
		break;}

	default: return nullptr;

	}

	return nullptr;
}
