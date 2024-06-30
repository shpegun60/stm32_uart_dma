/*
 * InRangeDetector.h
 *
 *  Created on: May 28, 2024
 *      Author: admin
 */

#ifndef _INRANGEDETECTOR_H_
#define _INRANGEDETECTOR_H_

#include <cstdint>

template<class T>
class InRangeDetector
{
public:
	InRangeDetector() = default;
	~InRangeDetector() {}

	enum class error: uint8_t {
		None,
		OutOfMinimumBoarder,
		OutOfMaximumBoarder,
	};

	error proceed(const uint32_t current_time, const T value);
	void init(const T min, const T max, const uint32_t dt) { m_minimum = min; m_maximum = max; m_dt = dt; m_time_counter = 0; m_lastError = error::None; }
	void reset();
private:
	uint32_t m_time_counter = 0;


	uint32_t m_dt = 1000;
	uint32_t m_last_time = 0;
	error m_lastError = error::None;

	T m_minimum = T();
	T m_maximum = T();
};



template<class T>
typename InRangeDetector<T>::error InRangeDetector<T>::proceed(const uint32_t current_time, const T value)
{
	if(m_last_time == current_time) {
		return m_lastError;
	}

	const error reg_error = (value < m_minimum) ? error::OutOfMinimumBoarder : ( (value > m_maximum) ? error::OutOfMaximumBoarder : error::None );

	if(reg_error != error::None) {
		const uint32_t next_counter = m_time_counter + 1;

		if(next_counter < m_dt) {
			m_time_counter = next_counter;
		} else if(next_counter == m_dt) {
			m_lastError = reg_error;
		}
	} else {
		const int next_counter = m_time_counter - 1;

		if(next_counter > 0) {
			m_time_counter = next_counter;
		} else {
			m_time_counter = 0;
			m_lastError = error::None;
		}
	}

	m_last_time = current_time;

	return m_lastError;
}

template<class T>
void InRangeDetector<T>::reset()
{
	m_time_counter = 0;
	m_last_time = 0;
	m_lastError = error::None;
}


//--------------------------------------------------------------------------------------------------------------------
typedef InRangeDetector<float> InRangeDetector_f;


#endif /* _INRANGEDETECTOR_H_ */
