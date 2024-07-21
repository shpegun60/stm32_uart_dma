/*
 * Stm32ADCdma.cpp
 *
 *  Created on: Jul 2, 2024
 *      Author: admin
 */

#include "Stm32ADCdma.h"


#ifdef HAL_ADC_MODULE_ENABLED

Stm32ADC_dma* Stm32ADC_dma::instance = nullptr;

Stm32ADC_dma::~Stm32ADC_dma()
{
	// TODO Auto-generated destructor stub
	delete[] adc_values;
}

bool Stm32ADC_dma::start()
{
	if(n_channels) {
		HAL_ADC_Start_DMA(m_hadc, (uint32_t*)adc_values, n_channels);
		return true;
	}

	return false;
}

void Stm32ADC_dma::init(const Stm32ADC_init& settings)
{
	m_hadc = settings.hadc;
	n_channels = settings.hadc->Init.NbrOfConversion;
	adc_values = new uint16_t[n_channels];

	Stm32ADC_dma::instance = this;

	ma.init(settings.filter, 16);
	ma.allocArrays(n_channels);
}

bool Stm32ADC_dma::proceed(const uint32_t current_time)
{
	if(calculated) {
		ma.proceed(adc_values);

//		HAL_ADC_Stop_DMA(m_hadc);
//		HAL_ADC_Start_DMA(m_hadc, (uint32_t*)adc_values, n_channels);
		calculated = false;

		return true;
	}

	return false;
}




void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	Stm32ADC_dma* const root = Stm32ADC_dma::getObject(hadc);
	if(root) {
		root->calculateFinished();
	}
}

#endif /* HAL_ADC_MODULE_ENABLED */
