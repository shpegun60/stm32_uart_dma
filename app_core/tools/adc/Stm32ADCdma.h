/*
 * Stm32ADCdma.h
 *
 *  Created on: Jul 2, 2024
 *      Author: admin
 */

#ifndef TOOLS_ADC_STM32ADCDMA_H_
#define TOOLS_ADC_STM32ADCDMA_H_

#include "main.h"

#ifdef HAL_ADC_MODULE_ENABLED
#include "tools/math/FPMovingAverageArray.h"

class Stm32ADC_dma
{
public:
	struct Stm32ADC_init {
		ADC_HandleTypeDef* hadc;
		float filter;
	};

public:
	Stm32ADC_dma() = default;
	~Stm32ADC_dma();
	inline void calculateFinished() { calculated = true;}

	bool start();
	void init(const Stm32ADC_init&);
	inline void setFilter(const float filter) {ma.setAlpha(filter);}

	inline uint32_t getAdc(int i) const {return ma.getOut(i);}
	inline uint8_t getChannels() const { return n_channels; }

	bool proceed(const uint32_t current_time);
	static inline Stm32ADC_dma* const getObject(ADC_HandleTypeDef* const hadc) {
		if(instance->m_hadc == hadc) {
			return instance;
		}

		return nullptr;
	}
private:
	ADC_HandleTypeDef* m_hadc = nullptr;
	uint8_t n_channels = 0;

	uint16_t* adc_values = nullptr;
	volatile bool calculated = false;
	FP_MovingAverageArray ma;


	// object container
	static Stm32ADC_dma* instance;
};

#endif /* HAL_ADC_MODULE_ENABLED */

#endif /* TOOLS_ADC_STM32ADCDMA_H_ */
