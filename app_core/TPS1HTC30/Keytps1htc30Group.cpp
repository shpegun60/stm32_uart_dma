/*
 * Keytps1htc30Group.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#include "Keytps1htc30Group.h"
#include "tools/irq/stm32_irq_block.h"

Key_tps1htc30Group* const Key_tps1htc30Group::getInstance()
{
	static Key_tps1htc30Group* const inst = new Key_tps1htc30Group;
	return inst;
}

bool Key_tps1htc30Group::findFaultPin(const uint16_t GPIO_Pin,
		KeyGroup **const group, uint8_t *const key_id)
{
	for(uint8_t group_id = 0; group_id != Groups; ++group_id) {
		const uint8_t cnt = m_initPinCnt[group_id];

		for(uint8_t _key_id = 0; _key_id != cnt; ++_key_id) {
			const error err = m_keys[group_id].err[_key_id];


			if(m_keys[group_id].i_fault[_key_id].GPIO_Pin == GPIO_Pin && err != error::NoInit) {
				*group = &m_keys[group_id];
				*key_id = _key_id;
				return true;
			}

		}
	}

	return false;
}


bool Key_tps1htc30Group::initKey(const KeyInit &setting, const uint8_t group_id)
{
	uint8_t cnt = m_initPinCnt[group_id];

	if(group_id < Groups && cnt != N_ch_div && setting.adc_ch < m_adc.getChannels() ) {

		m_keys[group_id].o_en[cnt] = setting.o_en;
		m_keys[group_id].i_fault[cnt] = setting.i_fault;
		m_keys[group_id].adc_ch[cnt] = setting.adc_ch;

		m_keys[group_id].o_en[cnt].set_output(false);
		m_keys[group_id].out_state[cnt] = false;
		m_keys[group_id].err[cnt] = error::PowerOff;

		m_initPinCnt[group_id] = cnt + 1;

		return true;
	}

	return false;
}

bool Key_tps1htc30Group::initGroup(const GroupInit &setting, const uint8_t group_id)
{
	if(group_id < Groups) {
		m_keys[group_id].o_diag_en = setting.o_diag_en;
		m_keys[group_id].o_latch = setting.o_latch;

		m_keys[group_id].o_diag_en.set_output(false);
		m_keys[group_id].o_latch.set_output(false);

		return true;
	}

	return false;
}

bool Key_tps1htc30Group::start()
{
	for(uint8_t group_id = 0; group_id != Groups; ++group_id) {
		const uint8_t cnt = m_initPinCnt[group_id];

		for(uint8_t key_id = 0; key_id != cnt; ++key_id) {
			m_keys[group_id].m_range[cnt].init(/*min*/0.05, /*max*/4.0, K_delay);
		}
	}

	return m_adc.start();
}

void Key_tps1htc30Group::proceed(const uint32_t current_time)
{
	if(!m_adc.proceed(current_time)) {
		return;
	}

	if(m_needCalibration) {

		if(m_calib_counter == 0) {
			m_summ = 0;
			m_keys[m_group_it].adc_offset[m_key_it] = 0;
			m_keys[m_group_it].err[m_key_it] = error::Calibration;
			set_out(false, m_group_it, m_key_it);
		}


		if((current_time - last_time) < K_delay) {
			return;
		}

		const uint8_t 	ch 	=  	m_keys[m_group_it].adc_ch[m_key_it];
		const uint32_t 	val = 	m_adc.getAdc(ch);

		m_summ += val;
		++m_calib_counter;

		if(m_calib_counter == K_Points) {
			m_keys[m_group_it].adc_offset[m_key_it] = m_summ / static_cast<float>(K_Points);
			m_keys[m_group_it].err[m_key_it] = error::PowerOff;
			m_calib_counter = 0;

			++m_key_it;

			if(m_key_it == m_initPinCnt[m_group_it]) {
				++m_group_it;
				m_key_it = 0;

				if(m_group_it == Groups) {
					m_needCalibration = false;
				}
			}
		}

		return;
	} else if(m_openLoadChk) {
		set_out(false, m_group_it, m_key_it);
		m_keys[m_group_it].err[m_key_it] = error::OpenLoadCHK;
		m_keys[m_group_it].o_diag_en.set_output(true);

		if((current_time - last_time) < K_delay) {
			return;
		}

		const uint8_t 	ch 	=  	m_keys[m_group_it].adc_ch[m_key_it];
		const uint32_t 	val = 	m_adc.getAdc(ch);

		m_keys[m_group_it].err[m_key_it] = ( (!m_keys[m_group_it].i_fault[m_key_it].get_input()) || (val > 4000) ) ? error::OpenLoad : error::OK;
		m_openLoadChk = false;
	}

	last_time = current_time;

	for(uint8_t group_id = 0; group_id != Groups; ++group_id) {
		const uint8_t cnt = m_initPinCnt[group_id];

		for(uint8_t key_id = 0; key_id != cnt; ++key_id) {

			error err = m_keys[group_id].err[key_id];
			if(err == error::PowerOff || err == error::OpenLoad || err == error::OK) {
				continue;
			}


			const uint8_t ch =  m_keys[group_id].adc_ch[key_id];
			const uint32_t val = m_adc.getAdc(ch);

			const float curr = m_keys[group_id].current[key_id] = calculateCurrent(val);
			const InRangeDetector_f::error rangeErr = m_keys[group_id].m_range[key_id].proceed(current_time, curr);

			if(!m_keys[group_id].i_fault[key_id].get_input()) {
				err = error::Fault;
			} else if(rangeErr == InRangeDetector_f::error::OutOfMinimumBoarder) {
				err = error::OpenLoad;
			} else if(rangeErr == InRangeDetector_f::error::OutOfMaximumBoarder) {
				err = error::Fault;
			} else {
				err = error::Work;
			}

			//IRQ_LOCK();
			m_keys[group_id].err[key_id] = err;
			//IRQ_UNLOCK();
		}
	}
}




// IKey interface --------------------------------------------------------------------------------------
void Key_tps1htc30Group::set_out(const bool en, const uint8_t group_id,
		const uint8_t key_id)
{

	const uint8_t cnt = m_initPinCnt[group_id];

	if(group_id < Groups && key_id < cnt) {
		m_keys[group_id].o_en[key_id].set_output(en);
		m_keys[group_id].o_diag_en.set_output(en);
		m_keys[group_id].err[key_id] = en ? error::Work : error::PowerOff;
	}
}

void Key_tps1htc30Group::set_latch(const bool en, const uint8_t group_id)
{
	if(group_id < Groups) {
		m_keys[group_id].o_latch.set_output(en);
	}
}

float Key_tps1htc30Group::getCurrent(const uint8_t group_id,
		const uint8_t key_id)
{
	const uint8_t cnt = m_initPinCnt[group_id];

	if(group_id < Groups && key_id < cnt) {

	}

	return 0.0;
}

uint8_t Key_tps1htc30Group::getError(const uint8_t group_id,
		const uint8_t key_id)
{
	const uint8_t cnt = m_initPinCnt[group_id];

	if(group_id < Groups && key_id < cnt) {
		return (uint8_t)m_keys[group_id].err[key_id];
	}

	return (uint8_t)error::NOT_Finded;
}

void Key_tps1htc30Group::setOpenLoadMeasure(const uint8_t group_id,
		const uint8_t key_id)
{
	const uint8_t cnt = m_initPinCnt[group_id];

	if(group_id < Groups && key_id < cnt) {
		m_group_it = group_id;
		m_key_it = key_id;
		m_openLoadChk = true;
	}
}

void Key_tps1htc30Group::calibrationStart()
{
	for(uint8_t group_id = 0; group_id != Groups; ++group_id) {
		if(m_initPinCnt[group_id] != 0) {
			m_needCalibration = true;
			m_key_it = 0;
			m_group_it = group_id;
			m_calib_counter = 0;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	const auto inst = Key_tps1htc30Group::getInstance();

	Key_tps1htc30Group::KeyGroup* group;
	uint8_t key_id;

	if(inst->findFaultPin(GPIO_Pin, &group, &key_id)) {
		group->err[key_id] = Key_tps1htc30Group::error::Fault;
	}
}
