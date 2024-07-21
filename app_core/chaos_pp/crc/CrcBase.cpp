/*
 * CrcBase.cpp
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#include "CrcBase.h"
#include <cstring>
#include <cstdlib>

// Constructor to allocate memory of given size
CrcBase::CrcBase(const u8 size)
{
    m_crc = std::malloc(size); 	// Allocate memory
    m_crc_size = size; 			// Store the size
}

// Copy constructor
CrcBase::CrcBase(const CrcBase& other)
{
    m_crc_size = other.m_crc_size; 					// Copy the size
    m_crc = std::malloc(m_crc_size); 				// Allocate new memory
    std::memcpy(m_crc, other.m_crc, m_crc_size); 	// Copy data from the other object
}

// Destructor
CrcBase::~CrcBase()
{
    std::free(m_crc); // Free allocated memory
}

// Equality operator
bool CrcBase::operator==(const CrcBase& other) const
{
    if (m_crc_size != other.m_crc_size) { // Check for different sizes
        return false;
    }

    return std::memcmp(m_crc, other.m_crc, m_crc_size) == 0; // Compare memory
}
