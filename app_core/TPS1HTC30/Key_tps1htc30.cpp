/*
 * Key_tps1htc30.cpp
 *
 *  Created on: May 31, 2024
 *      Author: admin
 */

#include "Key_tps1htc30.h"
#include <cmath>

void Key_tps1htc30::init(const KeyInit& settings)
{
	o_en_pin = settings.o_en;
	o_diag_en_pin = settings.o_diag_en;
	i_fault_pin = settings.i_fault;
	o_latch_pin = settings.o_latch;

	set_out(false);
	set_latch(false);

	m_adc.init(settings.Stm32ADC);
	m_adc.start();

	m_range.init(/*min*/0.05, /*max*/4.0, K_delay);
}

void Key_tps1htc30::proceed(const uint32_t current_time)
{
	m_adc.getAll(&m_adc_val, &m_voltage);
	calculateCurrent(m_voltage);

	if(m_error == error::Calibration) {

		if(m_calib_counter == 0) {
			m_summ = 0.0;
			m_v_rms = 0.0;
			set_out(false);
		}

		if((current_time - last_time) < K_delay) {
			return;
		}


		const float voltage_2 = m_voltage * m_voltage;
		m_summ += voltage_2;
		++m_calib_counter;

		if(m_calib_counter == K_Points) {
			m_v_rms = std::sqrt(m_summ / static_cast<float>(K_Points));
			m_error = error::PowerOff;
			set_out(m_out_state_before_calibration);
		}
		return;
	} else if (m_error == error::OpenLoadCHK) {

		set_out(false);
		set_onlyDiag(true);
		if((current_time - last_time) < K_delay) {
			return;
		}

		m_error = ( (!i_fault_pin.get_input()) || (m_voltage > 3.1) ) ? error::OpenLoad : error::OK;
	}


	last_time = current_time;

	// -------------------------------------------------------------------------------

	if(!m_out_state) {
		return;
	}

	const InRangeDetector_f::error rangeErr = m_range.proceed(current_time, m_current);

	if(!i_fault_pin.get_input()) {
		m_error = error::Fault;
	} else if(rangeErr == InRangeDetector_f::error::OutOfMinimumBoarder) {
		m_error = error::OpenLoad;
	} else if(rangeErr == InRangeDetector_f::error::OutOfMaximumBoarder) {
		m_error = error::Fault;
	} else {
		m_error = error::Work;
	}

	// -------------------------------------------------------------------------------
}


void Key_tps1htc30::set_out(bool en)
{
	set_on(en);
	m_out_state = en;

	if(!en) {
		m_range.reset();

		if(m_error != error::Calibration && m_error != error::OpenLoadCHK) {
			m_error = error::PowerOff;
		}
	}
}

void Key_tps1htc30::startCalibrationProcess()
{
	m_error = error::Calibration;
	m_calib_counter = 0;
	m_out_state_before_calibration = m_out_state;
}

void Key_tps1htc30::startOpenLoadMeasurement()
{
	m_error = error::OpenLoadCHK;
}
