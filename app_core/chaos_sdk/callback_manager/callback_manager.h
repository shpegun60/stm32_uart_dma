#ifndef __INC_CALLBACK_MANAGER_H__
#define __INC_CALLBACK_MANAGER_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "my_ctype/my_ctypes_def.h"

typedef void (*CallbackWorker)(u8* const data, const reg size, const u8 id, void* const ctx);

typedef struct {
    CallbackWorker* workers;
    void* ctx;
    u8 n;
} CallbackManager_t;

CallbackManager_t* const CallbackManager_new(const u8 n, void* const ctx);
bool CallbackManager_init(CallbackManager_t* const self, const u8 n, void* const ctx);
bool CallbackManager_subscribe(CallbackManager_t * const self, const u8 id, const CallbackWorker worker);
bool CallbackManager_delete(CallbackManager_t ** self);

//**********************************************************************************************************************************************************************
// callback manager call function with warning!!!
bool CallbackManager_proceed(const CallbackManager_t* const self, const u8 id,
                            u8* const data, const reg size);

#ifdef __cplusplus
}
#endif
#endif /* __INC_CALLBACK_MANAGER_H__ */
