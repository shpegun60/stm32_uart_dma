/*
 * PID.h
 *
 *  Created on: May 21, 2024
 *      Author: admin
 */

#ifndef _PID_H_
#define _PID_H_

#include <cstdint>


/// Standard PID (proportional, integral, derivative) controller. Derivative
/// component is filtered using an exponential moving average filter.
class PID
{
public:
	PID() = default;
	virtual ~PID() = default;

	/// @param  kp
	///         Proportional gain
	/// @param  ki
	///         Integral gain
	/// @param  kd
	///         Derivative gain
	/// @param  Ts
	///         Sampling time (seconds)
	/// @param  fc
	///         Cutoff frequency of derivative EMA filter (Hertz),
	///         zero to disable the filter entirely

	void init(const float kp, const float ki, const float kd,
			const float Ts, const float f_c, const float minOutput, const float maxOutput)
	{
		setKp(kp);
		setKi(ki);
		setKd(kd);

		m_TSample_s = Ts;

		m_minOutput = minOutput;
		m_maxOutput = maxOutput;

		m_task = 0.0;

		setEMACutoff(f_c);
		reset();
	}

	/// Update the controller: given the current position, compute the control
	/// action.
	float update(const float input, const float dt_s);
	float get_output() const { return m_output; }

	void setKp(const float kp) { m_kp = kp; }	///< Proportional gain
	void setKi(const float ki) { m_ki = ki; } 	///< Integral gain
	void setKd(const float kd) { m_kd = kd; } 	///< Derivative gain

	void setT_sample(const float T_sample)
	{
		m_TSample_s = T_sample;
		setEMACutoff(cutoff_freq);
	}

	float getKp() const { return m_kp; }    ///< Proportional gain
	float getKi() const { return m_ki; } 	///< Integral gain
	float getKd() const { return m_kd; } 	///< Derivative gain

	float getT_sample() const { return m_TSample_s; }
	/// Set the cutoff frequency (-3 dB point) of the exponential moving average
	/// filter that is applied to the input before taking the difference for
	/// computing the derivative term.
	void setEMACutoff(const float f_c)
	{
		const float f_n = f_c * m_TSample_s; // normalized sampling frequency
		m_emaAlpha = (f_c <= 0.001) ? 1.0 : calcAlphaEMA(f_n);
		cutoff_freq = f_c;
	}

    /// Compute the weight factor α for an exponential moving average filter
    /// with a given normalized cutoff frequency `fn`.
    static float calcAlphaEMA(float fn);

	/// Set the reference/target/setpoint of the controller.
	void setTask(const float task) {m_task = task;}
	/// @see @ref setSetpoint(int16_t)
	float getTask() const { return m_task; }

	/// Set the maximum/minimum control output magnitude, which clamps
    /// the control output
	void setBorders(const float minOutput, const float maxOutput) { m_minOutput = minOutput; m_maxOutput = maxOutput; }
	/// @see @ref setBorders(float)
	float getMaxOutput() const { return m_maxOutput; }
	/// @see @ref setBorders(float)
	float getMinOutput() const { return m_minOutput; }

	/// Reset the sum of the previous errors to zero.
	void reset()
	{
		m_prevInput = 0.0;
		m_integral 	= 0.0;
		m_output = 0.0;
	}

private:
	float m_kp = 1.0, m_ki = 0.0, m_kd = 0.0;      	///< Coefficients of pid
	float m_TSample_s = 0.001;   					///< Sampling time seconds
	float m_maxOutput = 0.0, m_minOutput = 0.0;     ///< Boarders for PID
	float m_task = 0.0;     						///< Task for PID

	// other --------------------------------------------------------------------------------
	float cutoff_freq = 0.0;		///< Cutoff frequency.
	float m_emaAlpha = 1.0;         ///< Weight factor of derivative EMA filter.

	float m_prevInput = 0.0;        ///< (Filtered) previous input for derivative.
	float m_integral = 0.0;       	///< Sum of previous errors for integral.

	// output
	float m_output = 0.0;
};


#endif /* _PID_H_ */
