/*
 * PoolContainer.cpp
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#include "PoolContainer.h"
#include <cstring>

bool PoolContainer::init(const u16 n_buffers, const u16 max_size)
{
	if (!RingBase::init(n_buffers) || max_size == 0) {
		return false;
	}

	m_pool.reserve(n_buffers);
	for (u16 i = 0; i < n_buffers; ++i) {
		m_pool.emplace_back(std::make_unique<u8[]>(max_size));
	}

	m_max_size = max_size;
	return true;
}

reg PoolContainer::write(const void* data, const reg len)
{
	if (!data || len > m_max_size || m_pool.empty()) {
		return 0;
	}

	reg head_reg = getHead();
	const reg msk_reg = getMask();

	const reg wr_pos = head_reg & msk_reg;
	void* const pool_ptr = m_pool[wr_pos].get();

	std::memcpy(pool_ptr, data, len);
	++head_reg;
	setHead(head_reg);
	return len;
}


void* const PoolContainer::getWriteBuffer()
{
	const reg head_reg 	= getHead();
	const reg msk_reg 	= getMask();
	const reg wr_pos 	= head_reg & msk_reg;

	return m_pool[wr_pos].get();
}

void* const PoolContainer::getReadBuffer()
{
	const reg tail_reg 	= getTail();
	const reg msk_reg 	= getMask();
	const reg rd_pos 	= tail_reg & msk_reg;

	return m_pool[rd_pos].get();
}



