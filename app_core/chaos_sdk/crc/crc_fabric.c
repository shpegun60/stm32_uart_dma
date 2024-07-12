/*
 * crc_base.c
 *
 *  Created on: Jul 10, 2024
 *      Author: admin
 */

#include "crc_fabric.h"
#include "my_crc_port.h"
#include <stdlib.h>

// crc ----------------

#ifdef _MY_CRC8_ENA
#include "crc8.h"

static crc_base_t* const newCRC8(void)
{
	crc_base_t* const crc = malloc(sizeof(crc_base_t));
	crc->crc = malloc(sizeof(u8));
    crc->crc_size = sizeof(u8);
    crc->crc_init = _MY_CRC8_INIT_BASE;
    crc->crc_array = _MY_CRC8_ARRAY_BASE;
    crc->crc_byte = _MY_CRC8_BYTE_BASE;
    _MY_CRC8_INIT_BASE(&crc->crc);
	return crc;
}

#endif /* _MY_CRC8_ENA */


#ifdef _MY_CRC16_ENA
#include "crc16.h"

static crc_base_t* const newCRC16(void)
{
	crc_base_t* const crc = malloc(sizeof(crc_base_t));
	crc->crc = malloc(sizeof(u16));
    crc->crc_size = sizeof(u16);
    crc->crc_init = _MY_CRC16_INIT_BASE;
    crc->crc_array = _MY_CRC16_ARRAY_BASE;
    crc->crc_byte = _MY_CRC16_BYTE_BASE;
    _MY_CRC16_INIT_BASE(&crc->crc);
	return crc;
}

#endif /* _MY_CRC16_ENA */

#ifdef _MY_CRC32_ENA
#include "crc32.h"

static crc_base_t* const newCRC32(void)
{
	crc_base_t* const crc = malloc(sizeof(crc_base_t));
	crc->crc = malloc(sizeof(u32));
    crc->crc_size = sizeof(u32);
    crc->crc_init = _MY_CRC32_INIT_BASE;
    crc->crc_array = _MY_CRC32_ARRAY_BASE;
    crc->crc_byte = _MY_CRC32_BYTE_BASE;
    _MY_CRC32_INIT_BASE(&crc->crc);
	return crc;
}

#endif /* _MY_CRC32_ENA */


#ifdef _MY_CRC64_ENA
#include "crc64.h"


static crc_base_t* const newCRC64(void)
{
	crc_base_t* const crc = malloc(sizeof(crc_base_t));
	crc->crc = malloc(sizeof(u64));
    crc->crc_size = sizeof(u64);
    crc->crc_init = _MY_CRC64_INIT_BASE;
    crc->crc_array = _MY_CRC64_ARRAY_BASE;
    crc->crc_byte = _MY_CRC64_BYTE_BASE;
    _MY_CRC8_INIT_BASE(&crc->crc);
	return crc;
}


#endif /* _MY_CRC64_ENA */




crc_base_t* const createCRC(const crc_type type)
{
	switch(type) {

	case CRC8 :
#ifdef _MY_CRC8_ENA
		return newCRC8();
#else
	 	 return NULL;
#endif /* _MY_CRC8_ENA */

	case CRC16:
#ifdef _MY_CRC16_ENA
		return newCRC16();
#else
		return NULL;
#endif /* _MY_CRC16_ENA */


	case CRC32:
#ifdef _MY_CRC32_ENA
		return newCRC32();
#else
		return NULL;
#endif /* _MY_CRC8_ENA */


	case CRC64:
#ifdef _MY_CRC64_ENA
		return newCRC64();
#else
		return NULL;
#endif /* _MY_CRC64_ENA */

	default: 	return NULL;

	}

	return NULL;
}





