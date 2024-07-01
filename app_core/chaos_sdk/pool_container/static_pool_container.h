#ifndef STATIC_POOL_CONTAINER_H
#define STATIC_POOL_CONTAINER_H


#include "my_ctype/my_ctypes.h"
#include "pool_container_head.h"

//typedef struct {
//	u16 size;
//	u8 data[STATIC_POOL_CONTAINER_COLUMNS];
//} static_pool_elem;

typedef struct {
    u16 size[STATIC_POOL_CONTAINER_RAWS];
    u8 pool[STATIC_POOL_CONTAINER_RAWS][STATIC_POOL_CONTAINER_COLUMNS];

    //static_pool_elem pool[STATIC_POOL_CONTAINER_RAWS];

    reg tail;
    reg head;
    u8 wrFull;
    u8 rdEmpty;
} static_pool_container_t;


static_pool_container_t* staticPoolContainer_new(void);
void staticPoolContainer_init(static_pool_container_t * const self);
#define staticPontainer_clear(self) staticPoolContainer_init((self))

/* free memory associated with the static_Pool_container_t  */
int staticPoolContainer_delete(static_pool_container_t **self);


//------------------------------------ WRITE FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
int staticPoolContainer_writeArr(static_pool_container_t * const self, u8 * const data, const u16 len);
reg staticPoolContainer_getWriteMeta(static_pool_container_t * const self, u8 ** const data, u16 ** const size);
void staticPoolContainer_nextWritePos(static_pool_container_t * const self);

//------------------------------------ READ FUNCTIONS-------------------------------------------------------------------------------------------------------------------------
reg staticPoolContainer_readArr(static_pool_container_t * const self, u8 ** const data);
void staticPoolContainer_nextReadPos(static_pool_container_t * const self);


#endif /* STATIC_POOL_CONTAINER_H */
