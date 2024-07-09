/*
ringbuf.c - common ring buffer

MIT License

Copyright (c) 2016 Yongqian Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <string.h>
#include "ringbuf.h"

// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__

// Determine whether x is a power of 2
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

// Initialize buffer
reg ringbuf_init(ringbuf_t* const ring_buf, void *buffer, const reg size)
{
	M_Assert_BreakSaveCheck(!is_power_of_2(size) || (buffer == NULL), M_EMPTY, return CTYPE_FALSE,
			"not valid parameters size: %d", size);

    memset(ring_buf, 0, sizeof(ringbuf_t));

    ring_buf->buffer = buffer;
    ring_buf->size = size;
    ring_buf->base.msk = (size - 1U);
    ring_buf->base.rdEmpty = true;
    return CTYPE_TRUE;
}

// Get data from buffer
reg ringbuf_get(ringbuf_t* const ring_buf, void* const buffer, const reg size)
{
	_INT_SWITCH(M_Assert_Break((buffer == NULL || ring_buf->buffer == NULL), M_EMPTY, return 0, "ring buffer NULL"));
	M_Assert_BreakSaveCheck(ring_buf->base.rdEmpty || size == 0, M_EMPTY, return 0, "ring buffer is empty");

	// move to registers------------------------------------------
	u8* const ring_ptr 			= ring_buf->buffer;

	reg tail_reg 				= ring_buf->base.tail;
	reg head_reg 				= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;

	const reg n_elem 			= (head_reg - tail_reg); 				// get n elements
	const reg size_constr 		= (size < n_elem) ? size : n_elem;		// constrain input size
	const reg tail_pos 			= tail_reg & msk_reg;					// get tail position
	const reg remaining_to_end 	= ring_buf->size - tail_pos;			// get remaining from tail to end

	// do logic --------------------------------------------------
	if(size_constr > remaining_to_end) {
		const reg remaining_n		= size_constr - remaining_to_end;
		/* first get the data from fifo->out until the end of the buffer */
		memcpy(buffer, ring_ptr + tail_pos, remaining_to_end);
		/* then get the rest (if any) from the beginning of the buffer */
		memcpy((u8* const)(buffer) + remaining_to_end, ring_ptr, remaining_n);
	} else {
		/* get all the data */
		memcpy(buffer, ring_ptr + tail_pos, size_constr);
	}

	// write data to memory ------------------------------------
    tail_reg += size_constr;

    // proceed signalls
    FIFO_PROCEED_GET(ring_buf, tail_reg, head_reg, msk_reg);
    return size_constr;
}

// Store data into buffer
reg ringbuf_put(ringbuf_t* const ring_buf, const void *buffer, const reg size)
{
	_INT_SWITCH(M_Assert_Break((buffer == NULL || ring_buf->buffer == NULL), M_EMPTY, return 0, "ring buffer NULL"));
	M_Assert_BreakSaveCheck(ring_buf->base.wrFull || size == 0, M_EMPTY, return 0, "ring buffer is full");
    
	// move to registers------------------------------------------------------
	u8* const ring_ptr 			= ring_buf->buffer;

	const reg tail_reg 			= ring_buf->base.tail;
	reg head_reg 				= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;
	const reg size_reg			= ring_buf->size;

	const reg head_pos 			= head_reg & msk_reg;

	const reg remaining_to_end 	= size_reg - head_pos;					// get remaining from head to end

	const reg n_empty 			= (size_reg - head_reg) + tail_reg;		// black magic for get empty cells
	const reg size_constr 		= (size < n_empty) ? size : n_empty;	// constrain input size
	// do logic ------------------------------------------------------------

	if(size_constr > remaining_to_end) {
		const reg remaining_n		= size_constr - remaining_to_end;
		/* first put the data starting from fifo->in to buffer end */
		memcpy(ring_ptr + head_pos, buffer, remaining_to_end);
		/* then put the rest (if any) at the beginning of the buffer */
		memcpy(ring_ptr, (u8* const)(buffer) + remaining_to_end, remaining_n);
	} else {
		/* put all the data */
		memcpy(ring_ptr + head_pos, buffer, size_constr);
	}

	// write data to memory ------------------------------------------------
	head_reg += size_constr;

	// proceed signalls
	FIFO_PROCEED_PUT(ring_buf, tail_reg, head_reg, msk_reg);
	return size;
}

// Store byte data in buffer
reg ringbuf_putc(ringbuf_t* const ring_buf, const u8 c)
{
	_INT_SWITCH(M_Assert_Break(ring_buf->buffer == NULL, M_EMPTY, return 0, "ring buffer NULL"));
	M_Assert_BreakSaveCheck(ring_buf->base.wrFull, M_EMPTY, return 0, "ring buffer is full");

	// move to registers------------------------------------
	u8* const ring_ptr 			= ring_buf->buffer;
	reg head_reg 				= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;

	const reg head_pos 			= head_reg & msk_reg;

    *(ring_ptr + head_pos) = c;

    ++head_reg;

    // proceed signalls
    FIFO_PROCEED_PUT(ring_buf, ring_buf->base.tail, head_reg, msk_reg);
    
    return 1;
}


#undef _INT_SWITCH
#undef is_power_of_2
