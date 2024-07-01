/*
 * app_core.cpp
 *
 *  Created on: Jun 30, 2024
 *      Author: admin
 */
#include "app_core.h"
#include "main.h"
#include "tools/timer/TimingManager.h"

#include "uart_communicator.h"


extern UART_HandleTypeDef huart1;

void app_main(void)
{
	stm32_uart_communicator_t m_comm;
	char txBuff[] =  "hello from stm!!!\r\n";
	TimingManager led_timer;

	init_communicator(&m_comm, &huart1);

	while(1) {

		proceed_communicator(&m_comm, HAL_GetTick());

		if(led_timer.isExpired()) {
			//UART_SendBuffer(&m_comm.stm32_uart, (uint8_t*)txBuff, sizeof(txBuff) - 1);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

			led_timer.start(100);
		}

	}
}
