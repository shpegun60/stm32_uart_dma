/*
 * uart_workers.h
 *
 *  Created on: Jul 1, 2024
 *      Author: admin
 */

#ifndef UART_WORKERS_H_
#define UART_WORKERS_H_

#include "my_ctype/my_ctypes_def.h"
#include "preprocessor/preprocessor_ctx.h"

void pingWorker(u8* const inputData, u8* const outputData, reg* const size, const reg maxOutBufferSize, PREPROCESSOR_CTX_TYPE(ctx));


#endif /* UART_WORKERS_H_ */
