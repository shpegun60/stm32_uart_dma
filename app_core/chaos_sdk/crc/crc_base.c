/*
 * crc_base.c
 *
 *  Created on: Jul 12, 2024
 *      Author: admin
 */
#include "crc_base.h"
#include <string.h>

crc_base_t* const crc_base_duplicate(crc_base_t* const from)
{
	if(from == NULL) {
		return NULL;
	}

	crc_base_t* const to = (crc_base_t*)malloc(sizeof(crc_base_t));
	to->crc = malloc(from->crc_size);
	to->crc_size = from->crc_size;

	return to;
}


bool crc_compleate(crc_base_t* const self1, crc_base_t* const self2)
{
	const reg size = self1->crc_size;
	if(size != self2->crc_size) {
		return false;
	}

	const void* ptr1 = self1->crc;
	const void* ptr2 = self2->crc;

	switch(size) {

	case sizeof(u8): {
		u8 val1, val2;
		memcpy(&val1, ptr1, sizeof(u8));
		memcpy(&val2, ptr2, sizeof(u8));

		return val1 == val2;}

	case sizeof(u16): {
		u16 val1, val2;
		memcpy(&val1, ptr1, sizeof(u16));
		memcpy(&val2, ptr2, sizeof(u16));

		return val1 == val2;}

	case sizeof(u32): {
		u32 val1, val2;
		memcpy(&val1, ptr1, sizeof(u32));
		memcpy(&val2, ptr2, sizeof(u32));

		return val1 == val2;}

	case sizeof(u64): {
		u64 val1, val2;
		memcpy(&val1, ptr1, sizeof(u64));
		memcpy(&val2, ptr2, sizeof(u64));

		return val1 == val2;}
	}

	return false;
}



