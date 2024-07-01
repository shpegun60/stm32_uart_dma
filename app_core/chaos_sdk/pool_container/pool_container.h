#ifndef POOL_CONTAINER_H
#define POOL_CONTAINER_H

#include "my_ctype/my_ctypes.h"
#include "pool_container_head.h"

typedef struct {
    u16 * size;
    u8 ** pool;
    u16 columns;
    u16 rows;

    reg msk;

    reg tail;
    reg head;
    u8 wrFull;
    u8 rdEmpty;
} pool_container_t;

pool_container_t *poolContainer_new(const u16 rows, const u16 columns);
int poolContainer_init(pool_container_t * const self, const u16 rows, const u16 columns);
void poolContainer_clear(pool_container_t * const self);

/* free memory associated with the pool_container_t  */
int poolContainer_delete(pool_container_t **self, u8 isHeap);

//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int poolContainer_writeArr(pool_container_t * const self, u8 * const data, const u16 len);
reg poolContainer_getWriteMeta(pool_container_t * const self, u8 ** const data, u16 ** const size);
void poolContainer_nextWritePos(pool_container_t * const self);
//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
reg poolContainer_readArr(pool_container_t * const self, u8 ** const data);
void poolContainer_nextReadPos(pool_container_t * const self);

#endif /* POOL_CONTAINER_H */
