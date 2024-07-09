#include "callback_manager.h"
#include "assertsEngine/assert_engine.h"

#include <stdlib.h>
#include <string.h>

#define LIB "callback manager"
#define ENA 1

CallbackManager_t* const CallbackManager_new(const u8 n, void* const ctx)
{
    CallbackManager_t * const m_manager = malloc(sizeof(CallbackManager_t));
    M_Assert_BreakSaveCheck(m_manager == (CallbackManager_t *)NULL, M_EMPTY, return m_manager, M_LIB_DATA_DEF "No memory for allocation ", ENA, LIB);

    if(CallbackManager_init(m_manager, n, ctx)) {
    	free(m_manager);
    	return NULL;
    }
    return m_manager;
}

bool CallbackManager_init(CallbackManager_t* const self, const u8 n, void* const ctx)
{
    M_Assert_BreakSaveCheck(self == (CallbackManager_t *)NULL, M_EMPTY, return true, M_LIB_DATA_DEF "no valid input data", ENA, LIB);

     void* const alloc = calloc(n, sizeof(CallbackWorker));
     M_Assert_BreakSaveCheck(alloc == NULL, M_EMPTY, return true, M_LIB_DATA_DEF "no memory for allocation", ENA, LIB);

    self->workers = alloc;
    self->ctx = ctx;
    self->n = n;
    return false;
}

bool CallbackManager_delete(CallbackManager_t ** self)
{
    M_Assert_BreakSaveCheck((self == NULL) || (*self == NULL), M_EMPTY, return true, M_LIB_DATA_DEF "No allocated memory", ENA, LIB);
    free((*self)->workers);
    free(*self);
    *self = NULL;
    return false;
}

bool CallbackManager_subscribe(CallbackManager_t * const self, const u8 id, const CallbackWorker worker)
{
    M_Assert_Break(self == (CallbackManager_t *)NULL, M_EMPTY, return true, M_LIB_DATA_DEF "CallbackManager_addWorker: no valid input data", ENA, LIB);
    self->workers[id] = worker;
    return false;
}


//**********************************************************************************************************************************************************************
bool CallbackManager_proceed(const CallbackManager_t* const self, const u8 id, u8* const data, const reg size)
{
    M_Assert_Break(self == NULL, M_EMPTY, return true, M_LIB_DATA_DEF "no valid input data", ENA, LIB);

    // move to cash
    const u8 n = self->n;
    const CallbackWorker worker = self->workers[id];
    void* const ctx = self->ctx;

    // do logic
    M_Assert_SafeFunctionCall((id < n) && (worker != NULL), {
                                  worker(data, size, ctx);
                                  return false;
                              });
    return true;
}

#undef LIB
#undef ENA
