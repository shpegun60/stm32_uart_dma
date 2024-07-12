/*
 * crc_from.c
 *
 *  Created on: Jul 12, 2024
 *      Author: admin
 */

#include"crc_from.h"

// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__

void crc_from_ringbuf(crc_obj_t* const crc_base, const ringbuf_t* const ring_buf)
{
	_INT_SWITCH(M_Assert_Break(ring_buf == NULL || crc_base == NULL, M_EMPTY, return,
			"not valid parameters"));

	const u8* const ring_ptr 	= ring_buf->buffer;

	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return,
					"no buffer internal");

	const reg tail_reg 			= ring_buf->base.tail;
	const reg head_reg 			= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;

	M_Assert_BreakSaveCheck(_FIFO_IS_EMPTY_IMPL(tail_reg, head_reg), M_EMPTY, return, "ring buffer is empty");

	const reg tail_pos 			= tail_reg & msk_reg;								// get tail position
	const reg n_elem 			= _FIFO_LEN_IMPL(tail_reg, head_reg); 				// get n elements
	const reg remaining_to_end 	= ring_buf->base.cap - tail_pos;

	if(n_elem > remaining_to_end) {
		const reg remaining_n		= n_elem - remaining_to_end;
		CRC_ARRAY(crc_base, ring_ptr + tail_pos, remaining_to_end);
		CRC_ARRAY(crc_base, ring_ptr, remaining_n);
	} else {
		CRC_ARRAY(crc_base, ring_buf->buffer, n_elem);
	}
}

#undef _INT_SWITCH

