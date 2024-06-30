/*
 * app_core.h
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */

#ifndef APP_CORE_H_
#define APP_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tools/uart/stm32_uart.h"


void app_main(void);
stm32_DMA_uart_t* const getUartInstance();

#ifdef __cplusplus
}
#endif


#endif /* APP_CORE_H_ */
