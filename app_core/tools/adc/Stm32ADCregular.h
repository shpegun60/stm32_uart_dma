/*
 * Stm32ADCregular.h
 *
 *  Created on: Jun 1, 2024
 *      Author: admin
 */

#ifndef TOOLS_STM32ADCREGULAR_H_
#define TOOLS_STM32ADCREGULAR_H_

#include "main.h"

#ifdef HAL_ADC_MODULE_ENABLED

class Stm32ADC_regular
{
public:
	struct Stm32ADC_init {
		ADC_HandleTypeDef* hadc;
		float v_ref;
	};

	Stm32ADC_regular() = default;
	virtual ~Stm32ADC_regular();

	void init(const Stm32ADC_init&);

	inline void stop() { HAL_ADC_Stop(m_hadc); }
	inline void start() { HAL_ADC_Start(m_hadc); }

	inline uint32_t getADC() const { return HAL_ADC_GetValue(m_hadc); }
	inline float getVoltage() const {
		const float v = getADC() * m_bitWeight;
		return v;
	}
	inline void getAll(uint32_t* const adc, float* const voltage) {
		const uint32_t reg_adc = getADC();
		const float v = static_cast<float>(reg_adc) * m_bitWeight;

		*adc = reg_adc;
		*voltage = v;
	}


private:
	static float getadcMax();

private:
	ADC_HandleTypeDef* m_hadc = nullptr;
	float m_bitWeight = 0.0;
};

#endif /* HAL_ADC_MODULE_ENABLED */


#endif /* TOOLS_STM32ADCREGULAR_H_ */
