/*
 * stm32_uart_container.c
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */

#include "stm32_uart_container.h"
#include <string.h>

#if defined(UART_CONTAINER_ENA) && defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED))

stm32_DMA_uart_t* m_instances[UART_CONTAINER_COUNT];
uint8_t m_instance_counter = 0;

void initUartsContainer(void)
{
	memset(m_instances, 0, sizeof(m_instances));
	m_instance_counter = 0;
}

stm32_DMA_uart_t* const getContainerUartInstance(UART_HandleTypeDef* const huart)
{
	const uint8_t r_inst_cnt = m_instance_counter;

	for(uint8_t i = 0; i < r_inst_cnt; ++i) {
		stm32_DMA_uart_t* const r_inst = m_instances[i];

		//if(r_inst->huart->Instance == huart->Instance) { // must be optimized
		if(r_inst->huart == huart) {
			return r_inst;
		}
	}

	return NULL;
}

status_t pushContainerUartInstance(stm32_DMA_uart_t* const inst)
{
	if(inst == NULL || (m_instance_counter == UART_CONTAINER_COUNT)) {
		return ERROR_FAIL;
	}

	m_instances[m_instance_counter] = inst;
	++m_instance_counter;


	return STATUS_OK;
}


#endif /* defined(UART_CONTAINER_ENA) && defined(HAL_DMA_MODULE_ENABLED) && (defined(HAL_UART_MODULE_ENABLED) || defined(HAL_USART_MODULE_ENABLED)) */
