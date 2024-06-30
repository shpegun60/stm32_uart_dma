/*
 * Stm32ADCregular.cpp
 *
 *  Created on: Jun 1, 2024
 *      Author: admin
 */

#include "Stm32ADCregular.h"

#ifdef HAL_ADC_MODULE_ENABLED

Stm32ADC_regular::~Stm32ADC_regular() {
	// TODO Auto-generated destructor stub
}

void Stm32ADC_regular::init(const Stm32ADC_init& settings)
{
	m_hadc = settings.hadc;
	m_bitWeight = settings.v_ref / getadcMax();
}

float Stm32ADC_regular::getadcMax()
{
	switch(m_hadc->Init.Resolution) {

	case ADC_RESOLUTION_16B:
		return 65536.0;

	case ADC_RESOLUTION_14B:
	case ADC_RESOLUTION_14B_OPT:
		return 16384.0;

	case ADC_RESOLUTION_12B:
	case ADC_RESOLUTION_12B_OPT:
		return 4096.0;

	case ADC_RESOLUTION_10B:
		return 1024.0;

	case ADC_RESOLUTION_8B:
		return 256.0;
	}

	return 0.0;
}

#endif /* HAL_ADC_MODULE_ENABLED */
