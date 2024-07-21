/*
 * Stm32ADCdma.cpp
 *
 *  Created on: Jul 2, 2024
 *      Author: admin
 */

#include "Stm32ADCdma.h"


#ifdef HAL_ADC_MODULE_ENABLED

std::vector<Stm32ADC_dma*> Stm32ADC_dma::m_objects;

Stm32ADC_dma::~Stm32ADC_dma()
{
	// TODO Auto-generated destructor stub
	delete[] adc_values;
}

void Stm32ADC_dma::init(const Init& settings)
{
	m_hadc = settings.hadc;
	n_channels = settings.hadc->Init.NbrOfConversion;
	adc_values = new uint16_t[n_channels];

	// push class to vector
	m_objects.push_back(this);
	// init filter
	ma.init(n_channels, settings.filter, 16);
}

bool Stm32ADC_dma::start()
{
	if(n_channels) {
		HAL_ADC_Start_DMA(m_hadc, (u32*)adc_values, n_channels);
		return true;
	}

	return false;
}


bool Stm32ADC_dma::proceed(const u32 current_time)
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


/*
 * *********************************************************************************
 * IT
 * *********************************************************************************
 */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	Stm32ADC_dma* const root = Stm32ADC_dma::adcToClass(hadc);
	if(root) {
		root->calculateFinished();
	}
}

#endif /* HAL_ADC_MODULE_ENABLED */
