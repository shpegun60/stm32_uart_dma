/*
 * CrcBase.h
 *
 *  Created on: Jul 16, 2024
 *      Author: admin
 */

#ifndef CHAOS_PP_CRC_CRCBASE_H_
#define CHAOS_PP_CRC_CRCBASE_H_

#include "my_cpp_types.h"

class CrcBase
{
public:
    explicit CrcBase(const u8 size);
    CrcBase(const CrcBase& other); 	// Copy constructor
    ~CrcBase(); 					// Destructor

    bool operator==(const CrcBase& other) const; // Equality operator

    inline void* const getCrc() const { return m_crc; }
    inline u8 crcSize() const { return m_crc_size; }

protected:
    void* m_crc = nullptr; 	// Pointer to dynamically allocated memory
    u8 m_crc_size = 0; 		// Size of allocated memory
};
#endif /* CHAOS_PP_CRC_CRCBASE_H_ */
