/*
 * crc_base.c
 *
 *  Created on: Jul 12, 2024
 *      Author: admin
 */
#include "crc_base.h"
#include <string.h>
#include <stdlib.h>

void crc_base_init(crc_base_t* const self, const reg size)
{
	self->crc = malloc(size);
	self->crc_size = size;
}

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
	const reg size1 = self1->crc_size;
	const reg size2 = self2->crc_size;
	const void* const ptr1 = self1->crc;
	const void* const ptr2 = self2->crc;

	if(size1 != size2) {
		return false;
	}

	return memcmp(ptr1, ptr2, size1) == 0;
}



