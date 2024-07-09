/*
 * stm32_meta.h
 *
 *  Created on: May 13, 2024
 *      Author: admin
 */

#ifndef APP_DRV_STM32_META_H_
#define APP_DRV_STM32_META_H_

#include "main.h"

#ifdef HAL_TIM_MODULE_ENABLED
// PWM & TIM -----------------------------------------------------------
struct Stm32Timer
{
	// compare value -------------------------------------
	inline void set_compare(uint32_t val)
	{
		__HAL_TIM_SET_COMPARE(tim, ch, val);
	}

	inline void set_compare_as_percent(const float percent)
	{
		const float reg_percent =  (percent > 100.0) ? 100.0 : percent;
		const uint32_t autoreload =__HAL_TIM_GET_AUTORELOAD(tim);
		const uint32_t compare = static_cast<uint32_t>( (static_cast<float>(autoreload) * reg_percent) / 100.0 );
		__HAL_TIM_SET_COMPARE(tim, ch, compare);
	}

	inline uint32_t get_compare() const
	{
		const uint32_t compare = __HAL_TIM_GET_COMPARE(tim, ch);
		return compare;
	}

	// autoreload value -------------------------------------
	inline uint32_t get_autoreload() const
	{
		const uint32_t autoreload =__HAL_TIM_GET_AUTORELOAD(tim);
		return autoreload;
	}

	inline void set_autoreload(uint32_t val)
	{
		__HAL_TIM_SET_AUTORELOAD(tim, val);
	}

	//peryphery depend function see datasheet  stm32h743zi datasheet
	uint32_t get_timer_frequency() const
	{
		uint32_t timer_freq;

		/*!< D2_APB2PERIPH peripherals */
		if (tim->Instance == TIM1 || tim->Instance == TIM8 || tim->Instance == TIM15 || tim->Instance == TIM16 || tim->Instance == TIM17) {
			timer_freq = HAL_RCC_GetPCLK2Freq();
		}
		/*!< D2_APB1PERIPH peripherals */
		else {
			timer_freq = HAL_RCC_GetPCLK1Freq();
		}

		const uint32_t prescaler = tim->Instance->PSC;
		const uint32_t timer_frequency = timer_freq / (prescaler + 1);

		return timer_frequency << 1;
	}

	//**********************
	TIM_HandleTypeDef* tim = nullptr;
	uint32_t ch = 0;
};
#endif /* HAL_TIM_MODULE_ENABLED */



#ifdef HAL_GPIO_MODULE_ENABLED
// IO -----------------------------------------------------------
struct Stm32Pin_I
{
	inline bool get_input() const {
		return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET;
	}

	//**********************
	GPIO_TypeDef* GPIOx = nullptr;
	uint16_t GPIO_Pin = 0;
};

// IO -----------------------------------------------------------
struct Stm32Pin_O
{

	inline void set_output(bool state) {
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, (state) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	inline void toggle_output() {
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
	//**********************
	GPIO_TypeDef* GPIOx = nullptr;
	uint16_t GPIO_Pin = 0;
};


#endif /* HAL_GPIO_MODULE_ENABLED */



template <size_t N>
constexpr unsigned int power_of_two()
{
	//static_assert(N <= 256, "Input must be less than 256");
	static_assert(N >= 0, "Input must be positive");
	static_assert((N & (N - 1)) == 0, "Input must be a power of 2");

	return N == 1 ? 0 : 1 + power_of_two<N / 2>();
}




#endif /* APP_DRV_STM32_META_H_ */
