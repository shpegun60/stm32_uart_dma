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

// Determine whether the buffer is full
#define FIFO_IS_EMPTY(chield)\
		(chield)->base.rdEmpty
#define FIFO_IS_FULL(chield)\
		(chield)->base.wrFull

typedef struct fifo_base_ {
	reg msk;
	reg tail;
	reg head;
	bool wrFull;
	bool rdEmpty;
} fifo_base_t;

// The length of the buffer data
STATIC_FORCEINLINE reg ringbuf_use_len(const fifo_base_t * const fifo)
{
	return (fifo->head - fifo->tail);
}


/*
 *  Borrowed from Synchronous FIFO systemVerilog code (AW --> is fifo size, power of 2):
 *
 *  assign ptr_match   = (wr_addr[AW-1:0] == rd_addr[AW-1:0]);
 *  assign wr_full     = ptr_match & (wr_addr[AW]==!rd_addr[AW]);
 *  assign fifo_empty  = ptr_match & (wr_addr[AW]==rd_addr[AW]);
 *
 */
#define FIFO_PROCEED_PUT(chield, tail_reg, head_reg, msk_reg)									\
		do {																					\
			/* proceed signalls */																\
			(chield)->base.head 	= (head_reg);												\
			(chield)->base.wrFull  	= ((head_reg) & (msk_reg)) == ((tail_reg) & (msk_reg));		\
			(chield)->base.rdEmpty 	= false;													\
		} while(0U)


#define FIFO_PROCEED_GET(chield, tail_reg, head_reg, msk_reg)								\
		do {																				\
			(chield)->base.tail 		= (tail_reg);										\
																							\
			/* proceed signalls */															\
			if ((tail_reg) == (head_reg)) {													\
				(chield)->base.wrFull 	= false;											\
				(chield)->base.rdEmpty 	= true;												\
			} else {																		\
				(chield)->base.wrFull  	= ((head_reg & msk_reg) == (tail_reg & msk_reg));	\
				(chield)->base.rdEmpty 	= false;											\
			}																				\
		} while(0U)


#endif /* CHAOS_SDK_FIFO_BASE_H_ */
