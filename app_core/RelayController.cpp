/*
 * RelayController.cpp
 *
 *  Created on: Jul 2, 2024
 *      Author: admin
 */

#include <RelayController.h>


RelayController::RelayController()
{
	// TODO Auto-generated constructor stub
	init_all();
}

void RelayController::init_all()
{
	//init communicator --------------------------------------------------
	extern UART_HandleTypeDef huart1;
	m_comm.init(&huart1);

	// init adc ---------------------------------------------------------
	extern ADC_HandleTypeDef hadc1;

	Stm32ADC_dma::Stm32ADC_init adc_init =  {
			.hadc = &hadc1,
			.filter = 1.0/512.0
	};

	m_relayGroup = Key_tps1htc30Group::getInstance();
	m_relayGroup->initAdc(adc_init);


	Key_tps1htc30Group::KeyInit key =  {
			.o_en = {
					.GPIOx = O_EN_GPIO_Port,
					.GPIO_Pin = O_EN_Pin
			},
			.i_fault = {
					.GPIOx = I_FAULT_GPIO_Port,
					.GPIO_Pin = I_FAULT_Pin
			},
			.adc_ch = 0
	};

	Key_tps1htc30Group::GroupInit group = {
			.o_latch = {
					.GPIOx = O_LATCH_GPIO_Port,
					.GPIO_Pin = O_LATCH_Pin
			},
			.o_diag_en = {
					.GPIOx = O_DIAG_EN_GPIO_Port,
					.GPIO_Pin = O_DIAG_EN_Pin
			}
	};

	m_relayGroup->initGroup(group, 0);
	m_relayGroup->initKey(key, 0);
	m_relayGroup->start();
}

RelayController* const RelayController::get_instance()
{
	static RelayController* const inst = new RelayController();
	return inst;
}

void RelayController::proceed()
{

	while(true) {
		m_comm.proceed(HAL_GetTick());

		if(led_timer.isExpired()) {
			//UART_SendBuffer(&m_comm.stm32_uart, (uint8_t*)txBuff, sizeof(txBuff) - 1);
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

			led_timer.start(100);
		}

//		Stm32ADC_dma* adc = m_relayGroup->getAdc();
//		if(adc->proceed(0)) {
//			for(int i = 0; i != 10; ++i) {
//				currents[i] = adc->getAdc(i);
//			}
//		}
		m_relayGroup->proceed(HAL_GetTick());
	}
}
