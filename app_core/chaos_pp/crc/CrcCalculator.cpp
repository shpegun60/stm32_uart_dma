/*
 * CrcCalculator.cpp
 *
 *  Created on: Jul 17, 2024
 *      Author: admin
 */

#include "CrcCalculator.h"

// constructor
CrcCalculator::CrcCalculator(const u8 size, crc_init_f init_f, crc_array_f array_f, crc_byte_f byte_f)
: CrcBase(size),
  m_init_f(init_f),
  m_array_f(array_f),
  m_byte_f(byte_f)
{

}

// Copy constructor
CrcCalculator::CrcCalculator(const CrcCalculator& other)
: CrcBase(other)
{
	// Perform shallow copy of function pointers
	m_init_f = other.m_init_f;
	m_array_f = other.m_array_f;
	m_byte_f = other.m_byte_f;
}
