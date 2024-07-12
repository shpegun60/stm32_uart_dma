/*
 * crc_base.h
 *
 *  Created on: Jul 10, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_CRC_CRC_FABRIC_H_
#define CHAOS_SDK_CRC_CRC_FABRIC_H_

#include "my_ctype/my_ctypes_def.h"
#include "crc_base.h"

#define CRC_INIT(handle) (handle)->crc_init((handle)->base.crc)
#define CRC_ARRAY(handle, data, len) (handle)->crc_array((handle)->base.crc, (data), (len))
#define CRC_BYTE(handle, data) (handle)->crc_byte((handle)->base.crc, (data))

typedef void (*crc_init_func)(void* const crc);
typedef void (*crc_array_func)(void* const crc, const u8* const data, const reg len);
typedef void (*crc_byte_func)(void* const crc, const u8 data);

typedef enum {
	CRC8,
	CRC16,
	CRC32,
	CRC64,
	NONE
} crc_type;

typedef struct {
	crc_base_t base;

	crc_init_func crc_init;
	crc_array_func crc_array;
	crc_byte_func crc_byte;
} crc_obj_t;



crc_obj_t* const createCRC(const crc_type type);


#endif /* CHAOS_SDK_CRC_CRC_FABRIC_H_ */
