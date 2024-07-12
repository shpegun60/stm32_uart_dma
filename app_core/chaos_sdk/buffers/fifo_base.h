/*
 * fifo_base.h
 *
 *  Created on: Jul 9, 2024
 *      Author: admin
 */

#ifndef CHAOS_SDK_FIFO_BASE_H_
#define CHAOS_SDK_FIFO_BASE_H_


#include "my_ctype/my_ctypes_def.h"
#include "inline.h"

/*
 *  Borrowed from Synchronous FIFO systemVerilog code (AW --> is fifo size, power of 2):
 *
 *  assign ptr_match   = (wr_addr[AW-1:0] == rd_addr[AW-1:0]);
 *  assign wr_full     = ptr_match & (wr_addr[AW]==!rd_addr[AW]);
 *  assign fifo_empty  = ptr_match & (wr_addr[AW]==rd_addr[AW]);
 *
 */

// Determine whether the buffer is empty
#define FIFO_IS_EMPTY(chield) ((chield)->base.tail == (chield)->base.head)
#define FIFO_NOT_EMPTY(chield) ((chield)->base.tail != (chield)->base.head)
// Determine whether the buffer is full
#define FIFO_IS_FULL(chield) ((((chield)->base.head ^ (chield)->base.tail) & (chield)->base.xor_msk) == (chield)->base.cap)
#define FIFO_NOT_FULL(chield) ((((chield)->base.head ^ (chield)->base.tail) & (chield)->base.xor_msk) != (chield)->base.cap)
// The length of the buffer data
#define FIFO_LEN(chield) ((chield)->base.head - (chield)->base.tail)


typedef struct fifo_base_ {
	reg cap; 		/* buffer capacity power of 2 */
	reg msk; 		/* (cap - 1) */
	reg xor_msk; 	/* xor mask  (cap + msk)*/
	reg tail;		/* tail pos */
	reg head;		/* head pos */
} fifo_base_t;

// Determine whether x is a power of 2
#define is_power_of_2(x) ((x) != 0 && (((x) & ((x) - 1)) == 0))
STATIC_FORCEINLINE void fifo_base_init(fifo_base_t * const fifo, const reg cap)
{
	const reg msk = (cap - 1U);

	fifo->cap = cap;
	fifo->msk = msk;
	fifo->xor_msk = cap | msk;
	fifo->tail = 0;
	fifo->head = 0;
}

STATIC_FORCEINLINE void fifo_base_clear(fifo_base_t * const fifo)
{
	fifo->tail = 0;
	fifo->head = 0;
}


#endif /* CHAOS_SDK_FIFO_BASE_H_ */
