/*
ringbuf.c - common ring buffer

Copyright (c) 2024 Shpegun60
 */

#include <string.h>
#include <stdlib.h>
#include "ringbuf.h"

// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__

// Initialize buffer
ringbuf_t* const ringbuf_new(void* const buffer, const reg size)
{
	M_Assert_BreakSaveCheck(!is_power_of_2(size), M_EMPTY, return NULL,
			"not valid parameters size: %d", size);

	ringbuf_t* const self = calloc(1, sizeof(ringbuf_t));

	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return NULL,
			"no memory for allocation");

	if(!ringbuf_init(self, buffer, size)) {
		free(self);
		return NULL;
	}

	return buffer;
}

bool ringbuf_init(ringbuf_t* const ring_buf, void* const buffer, const reg size)
{
	M_Assert_BreakSaveCheck(!is_power_of_2(size) || ring_buf == NULL, M_EMPTY, return false,
			"not valid parameter size: %d", size);

	memset(ring_buf, 0, sizeof(ringbuf_t));

	if(buffer != NULL) {
		ring_buf->buffer = buffer;
	} else {
		void* const tmp = malloc(size);

		M_Assert_BreakSaveCheck(tmp == NULL, M_EMPTY, return false,
				"no memory for allocation");

		ring_buf->buffer = tmp;
	}

	fifo_base_init(&ring_buf->base, size);
	return true;
}

bool ringbuf_install_buf(ringbuf_t* const ring_buf, void* const buffer)
{
	M_Assert_BreakSaveCheck(buffer == NULL, M_EMPTY, return false,
			"not valid parameter");

	ring_buf->buffer = buffer;
	FIFO_CLEAN(ring_buf);
	return true;
}


// Get data from buffer================================================================================================
reg ringbuf_get(ringbuf_t* const ring_buf, void* const buffer, const reg size)
{
	_INT_SWITCH(M_Assert_Break((ring_buf == NULL || buffer == NULL), M_EMPTY, return 0, "ring buffer NULL"));

	// move to registers------------------------------------------
	const u8* const ring_ptr 			= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return 0, "buffer is null");

	reg tail_reg 				= ring_buf->base.tail;
	const reg head_reg 			= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;

	M_Assert_BreakSaveCheck(_FIFO_IS_EMPTY_IMPL(tail_reg, head_reg), M_EMPTY, return 0, "ring buffer is empty");

	const reg n_elem 			= _FIFO_LEN_IMPL(tail_reg, head_reg);	// get n elements
	const reg size_constr 		= (size < n_elem) ? size : n_elem;		// constrain input size
	const reg tail_pos 			= tail_reg & msk_reg;					// get tail position
	const reg remaining_to_end 	= ring_buf->base.cap - tail_pos;		// get remaining from tail to end

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
	ring_buf->base.tail 		= (tail_reg);
	return size_constr;
}



u8 ringbuf_getc(ringbuf_t* const ring_buf)
{
	_INT_SWITCH(M_Assert_Break(ring_buf == NULL || ring_buf->buffer == NULL, M_EMPTY, return 0, "ring buffer NULL"));

	// move to registers------------------------------------------
	const u8* const ring_ptr 	= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return 0, "buffer is null");

	reg tail_reg 			= ring_buf->base.tail;
	const reg head_reg 		= ring_buf->base.head;
	const reg msk_reg		= ring_buf->base.msk;
	const reg tail_pos 		= tail_reg & msk_reg;					// get tail position

	M_Assert_BreakSaveCheck(_FIFO_IS_EMPTY_IMPL(tail_reg, head_reg), M_EMPTY, return 0, "ring buffer is empty");

	// do logic --------------------------------------------------
	const u8 value = *(ring_ptr + tail_pos);

	// write data to memory ------------------------------------
	++tail_reg;

	// proceed signalls
	ring_buf->base.tail 		= (tail_reg);
	return value;
}

#define RINGBUF_GET(TYPE) 													\
    TYPE ringbuf_get_##TYPE(ringbuf_t* const ring_buf) { 					\
        TYPE value; 														\
        if(ringbuf_get(ring_buf, &value, sizeof(TYPE)) != sizeof(TYPE)) {	\
        	return (TYPE)0;													\
		}																	\
        return value; 														\
    }

RINGBUF_GET(u16)
RINGBUF_GET(u32)
RINGBUF_GET(u64)
RINGBUF_GET(i16)
RINGBUF_GET(i32)
RINGBUF_GET(i64)
RINGBUF_GET(f32)
RINGBUF_GET(f64)

#undef RINGBUF_GET


// PEEK data from buffer================================================================================================
reg ringbuf_peek(ringbuf_t* const ring_buf, void* const buffer, const reg size)
{
	_INT_SWITCH(M_Assert_Break((ring_buf == NULL || buffer == NULL), M_EMPTY, return 0, "ring buffer NULL"));

	// move to registers------------------------------------------
	const u8* const ring_ptr 			= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return 0, "buffer is null");

	const reg tail_reg 			= ring_buf->base.tail;
	const reg head_reg 			= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;

	M_Assert_BreakSaveCheck(_FIFO_IS_EMPTY_IMPL(tail_reg, head_reg), M_EMPTY, return 0, "ring buffer is empty");

	const reg n_elem 			= _FIFO_LEN_IMPL(tail_reg, head_reg);	// get n elements
	const reg size_constr 		= (size < n_elem) ? size : n_elem;		// constrain input size
	const reg tail_pos 			= tail_reg & msk_reg;					// get tail position
	const reg remaining_to_end 	= ring_buf->base.cap - tail_pos;		// get remaining from tail to end

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
	return size_constr;
}



u8 ringbuf_peekc(ringbuf_t* const ring_buf)
{
	_INT_SWITCH(M_Assert_Break(ring_buf == NULL || ring_buf->buffer == NULL, M_EMPTY, return 0, "ring buffer NULL"));

	// move to registers------------------------------------------
	const u8* const ring_ptr 	= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return 0, "buffer is null");

	const reg tail_reg 		= ring_buf->base.tail;
	const reg head_reg 		= ring_buf->base.head;
	const reg msk_reg		= ring_buf->base.msk;
	const reg tail_pos 		= tail_reg & msk_reg;					// get tail position

	M_Assert_BreakSaveCheck(_FIFO_IS_EMPTY_IMPL(tail_reg, head_reg), M_EMPTY, return 0, "ring buffer is empty");

	// do logic --------------------------------------------------
	const u8 value = *(ring_ptr + tail_pos);
	return value;
}

#define RINGBUF_PEEK(TYPE) 													\
    TYPE ringbuf_peek_##TYPE(ringbuf_t* const ring_buf) { 					\
        TYPE value; 														\
        if(ringbuf_peek(ring_buf, &value, sizeof(TYPE)) != sizeof(TYPE)) {	\
        	return (TYPE)0;													\
		}																	\
        return value; 														\
    }

RINGBUF_PEEK(u16)
RINGBUF_PEEK(u32)
RINGBUF_PEEK(u64)
RINGBUF_PEEK(i16)
RINGBUF_PEEK(i32)
RINGBUF_PEEK(i64)
RINGBUF_PEEK(f32)
RINGBUF_PEEK(f64)


// Store data into buffer===========================================================================================
reg ringbuf_put(ringbuf_t* const ring_buf, const void *buffer, const reg size)
{
	_INT_SWITCH(M_Assert_Break((ring_buf == NULL || buffer == NULL || ring_buf->buffer == NULL), M_EMPTY, return 0, "ring buffer NULL"));

	// move to registers------------------------------------------------------
	u8* const ring_ptr 			= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return 0, "buffer is null");

	const reg tail_reg 			= ring_buf->base.tail;
	reg head_reg 				= ring_buf->base.head;
	const reg msk_reg			= ring_buf->base.msk;
	const reg xor_msk_reg		= ring_buf->base.xor_msk;
	const reg cap_reg			= ring_buf->base.cap;

	M_Assert_BreakSaveCheck(_FIFO_IS_FULL_IMPL(tail_reg, head_reg, cap_reg, xor_msk_reg), M_EMPTY, return 0, "ring buffer is full");

	const reg head_pos 			= head_reg & msk_reg;
	const reg remaining_to_end 	= cap_reg - head_pos;					// get remaining from head to end
	const reg n_empty 			= (cap_reg - head_reg) + tail_reg;		// black magic for get empty cells
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
	ring_buf->base.head 	= (head_reg);
	return size_constr;
}

// Store byte data in buffer
bool ringbuf_putc(ringbuf_t* const ring_buf, const u8 c)
{
	_INT_SWITCH(M_Assert_Break(ring_buf == NULL || ring_buf->buffer == NULL, M_EMPTY, return false, "ring buffer NULL"));

	// move to registers------------------------------------
	u8* const ring_ptr 			= ring_buf->buffer;
	M_Assert_BreakSaveCheck(ring_ptr == NULL, M_EMPTY, return false, "buffer is null");

	reg head_reg 				= ring_buf->base.head;
	const reg tail_reg 			= ring_buf->base.tail;
	const reg msk_reg			= ring_buf->base.msk;
	const reg xor_msk_reg		= ring_buf->base.xor_msk;
	const reg cap_reg			= ring_buf->base.cap;

	const reg head_pos 			= head_reg & msk_reg;

	M_Assert_BreakSaveCheck(_FIFO_IS_FULL_IMPL(tail_reg, head_reg, cap_reg, xor_msk_reg), M_EMPTY, return false, "ring buffer is full");

	*(ring_ptr + head_pos) = c;

	++head_reg;

	// proceed signalls
	ring_buf->base.head 	= (head_reg);

	return true;
}



#define RINGBUF_PUT(TYPE) 												\
    bool ringbuf_put_##TYPE(ringbuf_t* const ring_buf, TYPE c) { 		\
        return ringbuf_put(ring_buf, &c, sizeof(TYPE)) == sizeof(TYPE); \
    }


RINGBUF_PUT(u16)
RINGBUF_PUT(u32)
RINGBUF_PUT(u64)
RINGBUF_PUT(i16)
RINGBUF_PUT(i32)
RINGBUF_PUT(i64)
RINGBUF_PUT(f32)
RINGBUF_PUT(f64)

#undef RINGBUF_PUT


#undef _INT_SWITCH
