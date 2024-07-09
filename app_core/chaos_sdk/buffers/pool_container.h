#ifndef POOL_CONTAINER_H
#define POOL_CONTAINER_H
#ifdef __cplusplus
extern "C" {
#endif


#include "fifo_base.h"

typedef struct {
	fifo_base_t base;

	void** pool;
    u16 n_buffers;
    u16 max_size;
} pool_container_t;

pool_container_t* const poolContainer_new(const u16 n_buffers, const u16 max_size);
bool poolContainer_init(pool_container_t* const self, const u16 n_buffers, const u16 max_size);
void poolContainer_clear(pool_container_t * const self);
/* free memory associated with the pool_container_t  */
bool poolContainer_delete(pool_container_t **self, u8 isHeap);

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
reg poolContainer_write(pool_container_t * const self, void* const data, const reg len);

void* const poolContainer_getWriteBuffer(pool_container_t * const self);
void poolContainer_nextWritePos(pool_container_t * const self);

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
void* const poolContainer_readBuffer(pool_container_t * const self);
void poolContainer_nextReadPos(pool_container_t * const self);


#ifdef __cplusplus
}
#endif
#endif /* POOL_CONTAINER_H */
