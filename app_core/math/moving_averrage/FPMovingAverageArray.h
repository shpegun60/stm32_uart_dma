/*
 * FPMovingAverageArray.h
 *
 *  Created on: Jul 4, 2024
 *      Author: admin
 */

#ifndef TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_
#define TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_

#include "FPMovingAverageBase.h"

class FP_MovingAverageArray: public FP_MovingAverageBase
{
public:
	FP_MovingAverageArray() = default;
	~FP_MovingAverageArray() override final;

	bool init(const u8 n, const f32 alpha, const u8 Q);
	void proceed(const u16* const values);

	inline u32 getOut(const u8 n) const
	{
		if(n < m_n) {
			return m_y[n];
		}
		return 0;
	}
private:
	u32* m_y_states = nullptr;
	u32* m_y = nullptr;
	u8 m_n = 0;
};

#endif /* TOOLS_MATH_FPMOVINGAVERAGEARRAY_H_ */
