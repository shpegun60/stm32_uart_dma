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
#include "moving_averrage/FPMovingAverageArray.h"
#include <vector>

class Stm32ADC_dma
{
public:
	struct Init {
		ADC_HandleTypeDef* hadc;
		float filter;
	};

public:
	Stm32ADC_dma() = default;
	~Stm32ADC_dma();

	// start, init, proceed functions ----------------------------------------
	void init(const Init&);
	bool start();
	bool proceed(const u32 current_time);

	inline void setFilter(const float filter) { ma.setAlpha(filter); }

	// getting results -----------------------------------------------
	inline u32 getAdc(const u8 i) const {return ma.getOut(i);}
	inline u8 getChannels() const { return n_channels; }

	// from interrupt must be call ------------------------------------------
	inline void calculateFinished() { calculated = true; }
	static inline Stm32ADC_dma* const adcToClass(ADC_HandleTypeDef* const hadc);
private:
	// adc instances
	ADC_HandleTypeDef* m_hadc = nullptr;
	u8 n_channels = 0;

	// adc values
	u16* adc_values = nullptr;
	volatile bool calculated = false;

	// filter instance
	FP_MovingAverageArray ma;

	// objects container
	static std::vector<Stm32ADC_dma*> m_objects;
};



inline Stm32ADC_dma* const Stm32ADC_dma::adcToClass(ADC_HandleTypeDef* const hadc)
{
	for(Stm32ADC_dma* const it : m_objects) {
		if(it->m_hadc == hadc) {
			return it;
		}
	}
	return nullptr;
}

#endif /* HAL_ADC_MODULE_ENABLED */

#endif /* TOOLS_ADC_STM32ADCDMA_H_ */
