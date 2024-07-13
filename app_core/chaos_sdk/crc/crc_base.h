/*
 * crc_base.h
 *
 *  Created on: Jul 12, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_CRC_CRC_BASE_H_
#define CHAOS_SDK_CRC_CRC_BASE_H_

#include "my_ctype/my_ctypes_def.h"

#define CRC_BASE_INIT(chield, size) crc_base_init(&(chield)->base, size)
#define CRC_BASE_DUPLICATE(chield_from) crc_base_duplicate(&(chield_from)->base)

typedef struct {
	void* crc;
	u8 crc_size;
} crc_base_t;

void crc_base_init(crc_base_t* const self, const reg size);
crc_base_t* const crc_base_duplicate(crc_base_t* const from);
bool crc_compleate(crc_base_t* const self1, crc_base_t* const self2);



#endif /* CHAOS_SDK_CRC_CRC_BASE_H_ */
