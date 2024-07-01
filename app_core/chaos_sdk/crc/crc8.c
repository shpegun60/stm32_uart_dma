#include "crc8.h"

#ifdef _MY_CRC8_ENA
/*
*************************************************************************************************
  Name  : CRC-8-Dallas/Maxim
*************************************************************************************************
*/

#ifdef _MY_CRC8_TABLE_CALC_ENA
static const u8 crc8_maxim_table[256] =
{
    0x00U, 0x31U, 0x62U, 0x53U, 0xC4U, 0xF5U, 0xA6U, 0x97U, 0xB9U, 0x88U, 0xDBU, 0xEAU, 0x7DU, 0x4CU, 0x1FU, 0x2EU,
    0x43U, 0x72U, 0x21U, 0x10U, 0x87U, 0xB6U, 0xE5U, 0xD4U, 0xFAU, 0xCBU, 0x98U, 0xA9U, 0x3EU, 0x0FU, 0x5CU, 0x6DU,
    0x86U, 0xB7U, 0xE4U, 0xD5U, 0x42U, 0x73U, 0x20U, 0x11U, 0x3FU, 0x0EU, 0x5DU, 0x6CU, 0xFBU, 0xCAU, 0x99U, 0xA8U,
    0xC5U, 0xF4U, 0xA7U, 0x96U, 0x01U, 0x30U, 0x63U, 0x52U, 0x7CU, 0x4DU, 0x1EU, 0x2FU, 0xB8U, 0x89U, 0xDAU, 0xEBU,
    0x3DU, 0x0CU, 0x5FU, 0x6EU, 0xF9U, 0xC8U, 0x9BU, 0xAAU, 0x84U, 0xB5U, 0xE6U, 0xD7U, 0x40U, 0x71U, 0x22U, 0x13U,
    0x7EU, 0x4FU, 0x1CU, 0x2DU, 0xBAU, 0x8BU, 0xD8U, 0xE9U, 0xC7U, 0xF6U, 0xA5U, 0x94U, 0x03U, 0x32U, 0x61U, 0x50U,
    0xBBU, 0x8AU, 0xD9U, 0xE8U, 0x7FU, 0x4EU, 0x1DU, 0x2CU, 0x02U, 0x33U, 0x60U, 0x51U, 0xC6U, 0xF7U, 0xA4U, 0x95U,
    0xF8U, 0xC9U, 0x9AU, 0xABU, 0x3CU, 0x0DU, 0x5EU, 0x6FU, 0x41U, 0x70U, 0x23U, 0x12U, 0x85U, 0xB4U, 0xE7U, 0xD6U,
    0x7AU, 0x4BU, 0x18U, 0x29U, 0xBEU, 0x8FU, 0xDCU, 0xEDU, 0xC3U, 0xF2U, 0xA1U, 0x90U, 0x07U, 0x36U, 0x65U, 0x54U,
    0x39U, 0x08U, 0x5BU, 0x6AU, 0xFDU, 0xCCU, 0x9FU, 0xAEU, 0x80U, 0xB1U, 0xE2U, 0xD3U, 0x44U, 0x75U, 0x26U, 0x17U,
    0xFCU, 0xCDU, 0x9EU, 0xAFU, 0x38U, 0x09U, 0x5AU, 0x6BU, 0x45U, 0x74U, 0x27U, 0x16U, 0x81U, 0xB0U, 0xE3U, 0xD2U,
    0xBFU, 0x8EU, 0xDDU, 0xECU, 0x7BU, 0x4AU, 0x19U, 0x28U, 0x06U, 0x37U, 0x64U, 0x55U, 0xC2U, 0xF3U, 0xA0U, 0x91U,
    0x47U, 0x76U, 0x25U, 0x14U, 0x83U, 0xB2U, 0xE1U, 0xD0U, 0xFEU, 0xCFU, 0x9CU, 0xADU, 0x3AU, 0x0BU, 0x58U, 0x69U,
    0x04U, 0x35U, 0x66U, 0x57U, 0xC0U, 0xF1U, 0xA2U, 0x93U, 0xBDU, 0x8CU, 0xDFU, 0xEEU, 0x79U, 0x48U, 0x1BU, 0x2AU,
    0xC1U, 0xF0U, 0xA3U, 0x92U, 0x05U, 0x34U, 0x67U, 0x56U, 0x78U, 0x49U, 0x1AU, 0x2BU, 0xBCU, 0x8DU, 0xDEU, 0xEFU,
    0x82U, 0xB3U, 0xE0U, 0xD1U, 0x46U, 0x77U, 0x24U, 0x15U, 0x3BU, 0x0AU, 0x59U, 0x68U, 0xFFU, 0xCEU, 0x9DU, 0xACU,
};

// fast implementation (CRC MSB -> LSB)------------------------------------------------------------------------------------------------------------------------------
u8 fast_crc8_maxim_array(u8 * data, unsigned int len)
{
    u8 crc = CRC8INIT;

    while (len--) {
        crc = crc8_maxim_table[crc ^ *data++];
	}

    return crc;
}

u8 fast_crc8_maxim_byte(const u8 crc, const u8 data)
{
    return crc8_maxim_table[crc ^ data];
}

#endif /* _MY_CRC8_TABLE_CALC_ENA */

#ifdef _MY_CRC8_GENERIC_CALC_ENA

// slow implementation (CRC MSB -> LSB)------------------------------------------------------------------------------------------------------------------------------
u8 slow_crc8_maxim_array(u8 * data, unsigned int len)
{
    u8 crc = CRC8INIT;

    while (len--) {
        crc ^= *data++;

        for (unsigned bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x80U) ? ((crc << 1U) ^ CRC8POLY) : (crc << 1U);
		}
    }

    return crc;
}

u8 slow_crc8_maxim_byte(u8 crc, const u8 data)
{
    crc ^= data;

    for (unsigned bit = 0; bit < 8; ++bit) {
        crc = (crc & 0x80U) ? ((crc << 1U) ^ CRC8POLY) : (crc << 1U);
	}
	
    return crc;
}

#endif /* _MY_CRC8_GENERIC_CALC_ENA */



#ifndef _MY_CRC_TEST_DISABLE

#include <stdio.h>

static uint8_t _proceedCrc_OLD(uint8_t crc, uint8_t ch) { // original crc from old protocol
    crc ^= ch;
    for (int i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    return crc;
}


int crc8_test(u8 *data, reg len, u8 * res)
{
    u8 crc8[5] = {CRC8INIT, CRC8INIT, CRC8INIT, CRC8INIT, CRC8INIT};

    printf("\n\n------> crc 8 test <-------------------------------------------\n");
    printf("data: 0x");
    for(reg i = 0; i < len; ++i) {
        printf("%x.", data[i]);
    }

    printf("\nsizeof buffer: %d\n\n", (int)len);


#ifdef _MY_CRC8_TABLE_CALC_ENA
    crc8[0] = fast_crc8_maxim_array(data, len);
    printf("crc8 --> fast_crc8_maxim_array: 0x%x", crc8[0]);

    CRC8START(crc8[1]);
    for(reg i = 0; i < len; ++i) {
        crc8[1] = fast_crc8_maxim_byte(crc8[1], data[i]);
    }
    CRC8FINAL(crc8[1]);
    printf("\ncrc8 --> fast_crc8_maxim_byte: 0x%x", crc8[1]);
#endif /* _MY_CRC8_TABLE_CALC_ENA */



#ifdef _MY_CRC8_GENERIC_CALC_ENA
    crc8[2] = slow_crc8_maxim_array(data, len);
    printf("\ncrc8 --> slow_crc8_maxim_array: 0x%x", crc8[2]);

    CRC8START(crc8[3]);
    for(reg i = 0; i < len; ++i) {
        crc8[3] = slow_crc8_maxim_byte(crc8[3], data[i]);
    }
    CRC8FINAL(crc8[3]);
    printf("\ncrc8 --> slow_crc8_maxim_byte: 0x%x", crc8[3]);
#endif /* _MY_CRC8_GENERIC_CALC_ENA */

    for(reg i = 0; i < len; ++i) {
        crc8[4] = _proceedCrc_OLD(crc8[4], data[i]);
    }
    printf("\ncrc8 --> _proceedCrc_OLD: 0x%x", crc8[4]);

    int counter_not_valid = 0;

#if defined(_MY_CRC8_TABLE_CALC_ENA) && defined (_MY_CRC8_GENERIC_CALC_ENA)

    for(int i = 1; i < 5; ++i) {
        if(crc8[0] != crc8[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc8[0];
#elif defined(_MY_CRC8_TABLE_CALC_ENA) && !defined (_MY_CRC8_GENERIC_CALC_ENA)

    if(crc8[0] != crc8[1]) {
        ++counter_not_valid;
    }

    if(crc8[0] != crc8[4]) {
        ++counter_not_valid;
    }

    *res = crc8[0];
#else
    if(crc8[2] != crc8[3]) {
        ++counter_not_valid;
    }

    if(crc8[2] != crc8[4]) {
        ++counter_not_valid;
    }

    *res = crc8[2];
#endif /* defined(_MY_CRC8_TABLE_CALC_ENA) && defined (_MY_CRC8_GENERIC_CALC_ENA) */



    printf("\nerror counter %d\n", counter_not_valid);

    fflush(stdout);
    return counter_not_valid;
}

#endif /* _MY_CRC_TEST_DISABLE */

#endif /* _MY_CRC8_ENA */
