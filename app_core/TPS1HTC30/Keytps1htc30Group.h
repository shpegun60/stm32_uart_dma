/*
 * Keytps1htc30Group.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TPS1HTC30_KEYTPS1HTC30GROUP_H_
#define TPS1HTC30_KEYTPS1HTC30GROUP_H_


#include "stm32_meta.h"
#include "tools/math/InRangeDetector.h"
#include "tools/adc/Stm32ADCdma.h"

#include <functional>

class Key_tps1htc30Group
{
private:
	static constexpr int N_ch = 12;
	static constexpr int Groups = 2;
	static constexpr int N_ch_div = (N_ch >> 1);

	static constexpr float Rsns = 1000.0;		// Rsns resistance
	static constexpr float Ksns = 1300.0; 		// Rsns resistance
	static constexpr uint32_t K_Points = 1000; 	// K calibration points
	static constexpr uint32_t K_delay = 2000; 	// K calibration delay

	Key_tps1htc30Group() = default;
public:
	~Key_tps1htc30Group() = default;

	struct KeyInit {
		Stm32Pin_O o_en;
		Stm32Pin_I i_fault;
		uint8_t adc_ch;
	};

	struct GroupInit {
		Stm32Pin_O o_latch;
		Stm32Pin_O o_diag_en;
	};

	enum class error : uint8_t {
		NoInit,
		PowerOff,
		Calibration,
		OpenLoadCHK,

		Work,
		Fault,
		OpenLoad,
		OK,
		NOT_Finded
	};

	// IKey interface
	void set_out(const bool en, const uint8_t group_id, const uint8_t key_id);
	void set_latch(const bool en, const uint8_t group_id);
	void calibrationStart();
	void setOpenLoadMeasure(const uint8_t group_id, const uint8_t key_id);
	float getCurrent(const uint8_t group_id, const uint8_t key_id);
	uint8_t getError(const uint8_t group_id, const uint8_t key_id);

	// --------------------
	inline uint8_t getGroups() {return Groups;}
	inline uint8_t getKeys(const uint8_t group_id) {return m_initPinCnt[group_id];}


	//initialization functions
	static Key_tps1htc30Group* const getInstance();
	inline void initAdc(const Stm32ADC_dma::Stm32ADC_init& settings) { m_adc.init(settings); }
	inline Stm32ADC_dma* const getAdc() { return &m_adc; }
	bool initKey(const KeyInit& setting, const uint8_t group_id);
	bool initGroup(const GroupInit& setting, const uint8_t group_id);
	bool start();
	void proceed(const uint32_t current_time);

	struct KeyGroup;
	bool findFaultPin(const uint16_t GPIO_Pin, KeyGroup** const group, uint8_t* const key_id);
	// iterate on all keys
	void iterateAll(std::function<bool(KeyGroup*, uint8_t, uint8_t)> foo);

private:
	inline float calculateCurrent(float voltage) {
		const float current = ((voltage/*- m_v_rms*/) / Rsns) * Ksns;
		return (current > 0.0) ? current : 0.0;
	}

public:
	struct KeyGroup {
		// key pins ------------------------------------------
		Stm32Pin_O o_en[N_ch_div];
		Stm32Pin_I i_fault[N_ch_div];
		uint8_t adc_ch[N_ch_div];
		bool out_state[N_ch_div];
		// key data ------------------------------------------
		float current[N_ch_div];
		uint32_t adc_offset[N_ch_div];
		InRangeDetector_f m_range[N_ch_div];
		error err[N_ch_div] = {error::NoInit};

		// group pins-----------------------------------------
		Stm32Pin_O o_latch;
		Stm32Pin_O o_diag_en;
	};


private:
	KeyGroup m_keys[Groups];
	Stm32ADC_dma m_adc;
	uint8_t m_initPinCnt[Groups] = {0};

	// calibration values-------------
	bool m_needCalibration = false;
	uint8_t m_group_it = 0;
	uint8_t m_key_it = 0;
	uint32_t m_calib_counter = 0;
	uint32_t m_summ = 0;
	uint32_t last_time = 0;

	// openload check
	bool m_openLoadChk = false;
};

#endif /* TPS1HTC30_KEYTPS1HTC30GROUP_H_ */
