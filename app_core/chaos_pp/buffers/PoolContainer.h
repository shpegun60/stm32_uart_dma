/*
 * PoolContainer.h
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#ifndef CHAOSPP_POOLCONTAINER_H_
#define CHAOSPP_POOLCONTAINER_H_

#include <vector>
#include <memory>
#include "RingBase.h"


class PoolContainer : public RingBase
{
public:
	PoolContainer() = default;
	// std::vector and std::unique_ptr will take care of memory deallocation
	~PoolContainer() override final = default;

	bool init(const u16 n_buffers, const u16 max_size);

	// write logic -----------------------------------------------------
	reg write(const void* data, const reg len);
	void* const getWriteBuffer();
	inline void nextWritePos() { incrementHead(); }

	// read logic -----------------------------------------------------
	void* const getReadBuffer();
	inline void nextReadPos() { incrementTail(); }

private:
	std::vector<std::unique_ptr<u8[]>> m_pool;
	u16 m_max_size = 0;
};


#endif /* CHAOSPP_POOLCONTAINER_H_ */
