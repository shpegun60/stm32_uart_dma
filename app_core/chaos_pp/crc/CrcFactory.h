/*
 * CrcFactory.h
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */

#ifndef CHAOS_PP_CRC_CRCFACTORY_H_
#define CHAOS_PP_CRC_CRCFACTORY_H_

#include "CrcCalculator.h"

class CrcFactory
{
public:
	typedef enum {
		CRC8,
		CRC16,
		CRC32,
		CRC64,
		NONE
	} CrcStrategy;

	static CrcCalculator* const createCRC(const CrcStrategy type);
};

#endif /* CHAOS_PP_CRC_CRCFACTORY_H_ */
