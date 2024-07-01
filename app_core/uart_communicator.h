/*
 * uart_communicator.h
 *
 *  Created on: Jul 1, 2024
 *      Author: admin
 */

#ifndef UART_COMMUNICATOR_H_
#define UART_COMMUNICATOR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "tools/status.h"

#include "tools/uart/stm32_uart.h"
#include "raw_parser/raw_parser_dma/rawparser_dma.h"

#include "callback_manager/callback_manager.h"

typedef struct stm32_uart_communicator stm32_uart_communicator_t;
struct stm32_uart_communicator {
	RawParser_dma_t raw_p;
	stm32_DMA_uart_t stm32_uart;
	CallbackManager_t callb;
};

status_t init_communicator(stm32_uart_communicator_t* const self, UART_HandleTypeDef* const huart);
void proceed_communicator(stm32_uart_communicator_t* const self, const uint32_t current_time);

#ifdef __cplusplus
}
#endif

#endif /* UART_COMMUNICATOR_H_ */
