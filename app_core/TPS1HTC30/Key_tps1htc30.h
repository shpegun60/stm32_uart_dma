/*
 * Key_tps1htc30.h
 *
 *  Created on: May 31, 2024
 *      Author: admin
 */

#ifndef TPS1HTC30_KEY_TPS1HTC30_H_
#define TPS1HTC30_KEY_TPS1HTC30_H_

#include "stm32_meta.h"
#include "tools/adc/Stm32ADCregular.h"
#include "tools/math/InRangeDetector.h"

class Key_tps1htc30
{
public:

	struct KeyInit {
		Stm32Pin_O o_en;
		Stm32Pin_O o_diag_en;
		Stm32Pin_I i_fault;
		Stm32Pin_O o_latch;
		Stm32ADC_regular::Stm32ADC_init Stm32ADC;
	};

	enum class error : uint8_t {
		PowerOff,
		Calibration,
		OpenLoadCHK,

		Work,
		Fault,
		OpenLoad,
		OK
	};


	Key_tps1htc30() = default;
	~Key_tps1htc30() = default;

	void init(const KeyInit&);
	void proceed(const uint32_t current_time);

	// IKey interface
	uint8_t getError() { return static_cast<uint8_t>(m_error); }
	void set_out(bool en);
	void set_latch(bool en) { o_latch_pin.set_output(en); }
	float get_current() { return m_current; }
	void startCalibrationProcess();
	void startOpenLoadMeasurement();

private:
	inline void calculateCurrent(float voltage) {
		const float current = ((voltage - m_v_rms) / Rsns) * Ksns;
		m_current = (current > 0.0) ? current : 0.0;
	}

	inline void set_onlyDiag(bool en) {
		o_en_pin.set_output(false);
		o_diag_en_pin.set_output(en);
	}

	inline void set_on(bool en) {
		o_en_pin.set_output(en);
		o_diag_en_pin.set_output(en);
	}
private:
	static constexpr float Rsns = 1000.0;		// Rsns resistance
	static constexpr float Ksns = 1300.0; 		// Rsns resistance
	static constexpr uint32_t K_Points = 1000; 	// K calibration points
	static constexpr uint32_t K_delay = 2000; 	// K calibration delay

	//----------------------------
	Stm32Pin_O o_en_pin;
	Stm32Pin_O o_diag_en_pin;
	Stm32Pin_I i_fault_pin;
	Stm32Pin_O o_latch_pin;
	bool m_out_state = false;

	// adc ------------------------------
	Stm32ADC_regular m_adc;
	float m_voltage = 0.0;
	uint32_t m_adc_val = 0;
	float m_current = 0.0;
	InRangeDetector_f m_range;

	// error ------------------------------
	error m_error = error::PowerOff;

	// calibration values ----------------
	float m_summ = 0.0;
	float m_v_rms = 0.0;
	uint32_t m_calib_counter = 0;
	uint32_t last_time = 0;
	bool m_out_state_before_calibration = false;
};

#endif /* TPS1HTC30_KEY_TPS1HTC30_H_ */
