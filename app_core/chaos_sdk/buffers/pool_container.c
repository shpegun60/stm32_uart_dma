#include "pool_container.h"
#include <stdlib.h>
#include <string.h>

// include assert engine
#include "assertsEngine/assert_engine.h"
#define _INT_SWITCH(...) __VA_ARGS__

// Determine whether x is a power of 2
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))

pool_container_t* const poolContainer_new(const u16 n_buffers, const u16 max_size)
{
	M_Assert_BreakSaveCheck((n_buffers == 0 || max_size == 0), M_EMPTY,
			return NULL,
					"Give me positive values for dimensions genius");

	M_Assert_BreakSaveCheck(!is_power_of_2(n_buffers), M_EMPTY,
			return NULL,
					"n_buffers is not power of 2, val: %d", n_buffers);

	M_Assert_BreakSaveCheck((n_buffers  > 16384 || max_size > 16384), M_EMPTY,
			return NULL,
					"is too long, N_BUF: %d, SIZE: %d", n_buffers, max_size);

	pool_container_t* const self = (pool_container_t *)calloc(1, sizeof(pool_container_t));
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return NULL, "No memory for allocation");


	if(poolContainer_init(self, n_buffers, max_size)) {
		free(self);
		return NULL;
	}

	return self;
}

bool poolContainer_init(pool_container_t* const self, const u16 n_buffers, const u16 max_size)
{
	M_Assert_BreakSaveCheck((n_buffers == 0 || max_size == 0), M_EMPTY,
			return true,
					"Give me positive values for dimensions genius");

	M_Assert_BreakSaveCheck(!is_power_of_2(n_buffers), M_EMPTY,
			return true,
					"n_buffers is not power of 2, val: %d", n_buffers);

	M_Assert_BreakSaveCheck((n_buffers  > 16384 || max_size > 16384), M_EMPTY,
			return true,
					"is too long, N_BUF: %d, SIZE: %d", n_buffers, max_size);

	self->pool = (void**)calloc(n_buffers, sizeof(void*));
	M_Assert_BreakSaveCheck(self->pool == NULL, M_EMPTY,
			return true,
					"no memories for allocation data");

	for (u16 i = 0; i < n_buffers; ++i) {
		self->pool[i] = calloc(max_size, sizeof(u8));
		M_Assert_BreakSaveCheck(self->pool[i] == NULL, M_EMPTY, goto error, "no memories for allocation");
	}


	self->n_buffers = n_buffers;
	self->max_size = max_size;
	self->base.msk = (reg)(n_buffers - 1U);
	poolContainer_clear(self);
	return false;

	// error proceed mark --------------------------------------------
	error:
	for (u16 i = 0; i < n_buffers; ++i) {
		free(self->pool[i]);
	}
	free(self->pool);
	return true;
}

void poolContainer_clear(pool_container_t * const self)
{
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY, return, "No memory allocated");
	self->base.tail = 0;
	self->base.head = 0;
	self->base.wrFull = false;
	self->base.rdEmpty = true;
}

/* free memory associated with the pool_container_t  */
bool poolContainer_delete(pool_container_t **self, u8 isHeap)
{
	M_Assert_BreakSaveCheck(((*self) == NULL || self == NULL), M_EMPTY, return true, "poolContainer_delete: incorrect input values");

	/* Code for further processing and free the
       dynamically allocated memory */

	const u16 n_buffers = (*self)->n_buffers;

	for (u16 i = 0; i < n_buffers; ++i) {
		free((*self)->pool[i]);
	}
	free((*self)->pool);

	(*self)->n_buffers = 0;
	(*self)->max_size = 0;
	(*self)->base.msk = 0;
	poolContainer_clear(*self);

	if(isHeap) {
		free(*self);
		(*self) = NULL;
	}
	return false;
}

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
reg poolContainer_write(pool_container_t * const self, void* const data, const reg len)
{
	_INT_SWITCH(M_Assert_Break((self == NULL || data == NULL), M_EMPTY, return 0, "incorrect input values"));
	_INT_SWITCH(M_Assert_Break(self->pool == NULL, M_EMPTY, return 0, "no allocated memory"));
	M_Assert_BreakSaveCheck(len > self->max_size, M_EMPTY, return 0, "len more than buffer");
	M_Assert_WarningSaveCheck(FIFO_IS_FULL(self), M_EMPTY, return 0, "buffer is full!!!");

	reg head_reg = self->base.head;
	const reg tail_reg = self->base.tail;
	const reg msk_reg = self->base.msk;

	const reg wr_pos = head_reg & msk_reg;
	void* const pool = &self->pool[wr_pos];

	memcpy(pool, data, len);
	++head_reg;

	// proceed signalls ---------------------------------------------
	FIFO_PROCEED_PUT(self, tail_reg, head_reg, msk_reg);
	return len;
}


void* const poolContainer_getWriteBuffer(pool_container_t * const self)
{
	_INT_SWITCH(M_Assert_Break(self == NULL, M_EMPTY, return NULL, "incorrect input values"));
	_INT_SWITCH(M_Assert_Break(self->pool == NULL, M_EMPTY, return NULL, "no allocated memory"));

	const reg head_reg = self->base.head;
	const reg msk_reg = self->base.msk;
	const reg wr_pos = head_reg & msk_reg;

	return &self->pool[wr_pos];
}

void poolContainer_nextWritePos(pool_container_t * const self)
{
	_INT_SWITCH(M_Assert_Break(self == NULL, M_EMPTY, return, "incorrect input values"));
	_INT_SWITCH(M_Assert_Break(self->pool == NULL, M_EMPTY, return, "no allocated memory"));
	M_Assert_WarningSaveCheck(FIFO_IS_FULL(self), M_EMPTY, return, "buffer is full!!!");

	const reg head_reg = self->base.head + 1U;
	const reg tail_reg = self->base.tail;
	const reg msk_reg = self->base.msk;

	// proceed signalls ---------------------------------------------
	FIFO_PROCEED_PUT(self, tail_reg, head_reg, msk_reg);
}

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
void* const poolContainer_readBuffer(pool_container_t * const self)
{
	_INT_SWITCH(M_Assert_Break(self == NULL, M_EMPTY, return NULL, "incorrect input values"));
	_INT_SWITCH(M_Assert_Break(self->pool == NULL, M_EMPTY, return NULL, "no allocated memory"));
	M_Assert_WarningSaveCheck(FIFO_IS_EMPTY(self), M_EMPTY, return NULL, "buffer is empty!!!");

	const reg tail_reg = self->base.tail;
	const reg msk_reg = self->base.msk;
	const reg rd_pos = tail_reg & msk_reg;

	return &self->pool[rd_pos];
}

void poolContainer_nextReadPos(pool_container_t * const self)
{
	_INT_SWITCH(M_Assert_Break(self == NULL, M_EMPTY, return, "incorrect input values"));
	_INT_SWITCH(M_Assert_Break(self->pool == NULL, M_EMPTY, return, "no allocated memory"));
	M_Assert_WarningSaveCheck(FIFO_IS_EMPTY(self), M_EMPTY, return, "buffer is empty!!!");

	const reg head_reg = self->base.head;
	const reg tail_reg = self->base.tail + 1U;
	const reg msk_reg = self->base.msk;

	// write data to memory ------------------------------------
	// proceed signalls
	FIFO_PROCEED_GET(self, tail_reg, head_reg, msk_reg);
}

#undef is_power_of_2
#undef _INT_SWITCH




