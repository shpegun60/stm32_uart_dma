#include "crc16.h"


#ifdef _MY_CRC16_ENA
#include "my_ctype/my_ctype_cast.h"

/*
*************************************************************************************************
  Name  : CRC-16-T10-DIF
*************************************************************************************************
*/

// base init implementation ----------------------------
//typedef void (*crc_init_func)(void* const crc);
void crc16_t10_dif_init_base(u16* const crc)
{
	*crc = CRC16INIT;
}


#ifdef _MY_CRC16_TABLE_CALC_ENA

static const u16 crc16_t10dif_table[256] =
{
    0x0000U, 0x8BB7U, 0x9CD9U, 0x176EU, 0xB205U, 0x39B2U, 0x2EDCU, 0xA56BU, 0xEFBDU, 0x640AU, 0x7364U, 0xF8D3U, 0x5DB8U, 0xD60FU, 0xC161U, 0x4AD6U,
    0x54CDU, 0xDF7AU, 0xC814U, 0x43A3U, 0xE6C8U, 0x6D7FU, 0x7A11U, 0xF1A6U, 0xBB70U, 0x30C7U, 0x27A9U, 0xAC1EU, 0x0975U, 0x82C2U, 0x95ACU, 0x1E1BU,
    0xA99AU, 0x222DU, 0x3543U, 0xBEF4U, 0x1B9FU, 0x9028U, 0x8746U, 0x0CF1U, 0x4627U, 0xCD90U, 0xDAFEU, 0x5149U, 0xF422U, 0x7F95U, 0x68FBU, 0xE34CU,
    0xFD57U, 0x76E0U, 0x618EU, 0xEA39U, 0x4F52U, 0xC4E5U, 0xD38BU, 0x583CU, 0x12EAU, 0x995DU, 0x8E33U, 0x0584U, 0xA0EFU, 0x2B58U, 0x3C36U, 0xB781U,
    0xD883U, 0x5334U, 0x445AU, 0xCFEDU, 0x6A86U, 0xE131U, 0xF65FU, 0x7DE8U, 0x373EU, 0xBC89U, 0xABE7U, 0x2050U, 0x853BU, 0x0E8CU, 0x19E2U, 0x9255U,
    0x8C4EU, 0x07F9U, 0x1097U, 0x9B20U, 0x3E4BU, 0xB5FCU, 0xA292U, 0x2925U, 0x63F3U, 0xE844U, 0xFF2AU, 0x749DU, 0xD1F6U, 0x5A41U, 0x4D2FU, 0xC698U,
    0x7119U, 0xFAAEU, 0xEDC0U, 0x6677U, 0xC31CU, 0x48ABU, 0x5FC5U, 0xD472U, 0x9EA4U, 0x1513U, 0x027DU, 0x89CAU, 0x2CA1U, 0xA716U, 0xB078U, 0x3BCFU,
    0x25D4U, 0xAE63U, 0xB90DU, 0x32BAU, 0x97D1U, 0x1C66U, 0x0B08U, 0x80BFU, 0xCA69U, 0x41DEU, 0x56B0U, 0xDD07U, 0x786CU, 0xF3DBU, 0xE4B5U, 0x6F02U,
    0x3AB1U, 0xB106U, 0xA668U, 0x2DDFU, 0x88B4U, 0x0303U, 0x146DU, 0x9FDAU, 0xD50CU, 0x5EBBU, 0x49D5U, 0xC262U, 0x6709U, 0xECBEU, 0xFBD0U, 0x7067U,
    0x6E7CU, 0xE5CBU, 0xF2A5U, 0x7912U, 0xDC79U, 0x57CEU, 0x40A0U, 0xCB17U, 0x81C1U, 0x0A76U, 0x1D18U, 0x96AFU, 0x33C4U, 0xB873U, 0xAF1DU, 0x24AAU,
    0x932BU, 0x189CU, 0x0FF2U, 0x8445U, 0x212EU, 0xAA99U, 0xBDF7U, 0x3640U, 0x7C96U, 0xF721U, 0xE04FU, 0x6BF8U, 0xCE93U, 0x4524U, 0x524AU, 0xD9FDU,
    0xC7E6U, 0x4C51U, 0x5B3FU, 0xD088U, 0x75E3U, 0xFE54U, 0xE93AU, 0x628DU, 0x285BU, 0xA3ECU, 0xB482U, 0x3F35U, 0x9A5EU, 0x11E9U, 0x0687U, 0x8D30U,
    0xE232U, 0x6985U, 0x7EEBU, 0xF55CU, 0x5037U, 0xDB80U, 0xCCEEU, 0x4759U, 0x0D8FU, 0x8638U, 0x9156U, 0x1AE1U, 0xBF8AU, 0x343DU, 0x2353U, 0xA8E4U,
    0xB6FFU, 0x3D48U, 0x2A26U, 0xA191U, 0x04FAU, 0x8F4DU, 0x9823U, 0x1394U, 0x5942U, 0xD2F5U, 0xC59BU, 0x4E2CU, 0xEB47U, 0x60F0U, 0x779EU, 0xFC29U,
    0x4BA8U, 0xC01FU, 0xD771U, 0x5CC6U, 0xF9ADU, 0x721AU, 0x6574U, 0xEEC3U, 0xA415U, 0x2FA2U, 0x38CCU, 0xB37BU, 0x1610U, 0x9DA7U, 0x8AC9U, 0x017EU,
    0x1F65U, 0x94D2U, 0x83BCU, 0x080BU, 0xAD60U, 0x26D7U, 0x31B9U, 0xBA0EU, 0xF0D8U, 0x7B6FU, 0x6C01U, 0xE7B6U, 0x42DDU, 0xC96AU, 0xDE04U, 0x55B3U,
};


#define CRC16_FAST_BYTE_PROCEED(crc, data)															\
		((crc) << 8U) ^ crc16_t10dif_table[(((crc) >> 8U) ^ (data)) & 0x00FFU]

#define CRC16_FAST_ARRAY_PROCEED(crc, data, len)													\
		while ((len)--) {																			\
			(crc) = CRC16_FAST_BYTE_PROCEED(crc, *(data)++);										\
		}

// fast implementation (CRC MSB -> LSB)------------------------------------------------------------------------------------------------------------------------------
u16 fast_crc16_t10_dif_array(u8 * data, unsigned int len)
{
    u16 crc = CRC16INIT;
    CRC16_FAST_ARRAY_PROCEED(crc, data, len);
    return crc;
}

u16 fast_crc16_t10_dif_byte(const u16 crc, const u8 data)
{
	return CRC16_FAST_BYTE_PROCEED(crc, data);
}

// base implementation ----------------------------
//typedef void (*crc_array_func)(void* const crc, const u8* const data, const reg len);
void fast_crc16_t10_dif_array_base(u16* const crc, const u8* data, reg len)
{
	u16 crc16 = *crc;
	CRC16_FAST_ARRAY_PROCEED(crc16, data, len);
	*crc = crc16;
}

//typedef void (*crc_byte_func)(void* const crc, const u8 data);
void fast_crc16_t10_dif_byte_base(u16* const crc, const u8 data)
{
	u16 crc16 = *crc;
	crc16 = CRC16_FAST_BYTE_PROCEED(crc16, data);
	*crc = crc16;
}


#undef CRC16_FAST_BYTE_PROCEED
#undef CRC16_FAST_ARRAY_PROCEED

#endif /* _MY_CRC16_TABLE_CALC_ENA */


#ifdef _MY_CRC16_GENERIC_CALC_ENA

#define CRC16_SLOW_BYTE_PROCEED(crc, data)															\
	    (crc) ^= (data) << 8U;																		\
	    																							\
	    for (u8 bit = 0; bit < 8; ++bit) {															\
	        (crc) = ((crc) & 0x8000U) ? (((crc) << 1U) ^ CRC16POLY) : ((crc) << 1U);				\
	    }


#define CRC16_SLOW_ARRAY_PROCEED(crc, data, len)													\
		while ((len)--) {																			\
			CRC16_SLOW_BYTE_PROCEED(crc, *(data)++)													\
		}

// slow implementation (CRC MSB -> LSB)------------------------------------------------------------------------------------------------------------------------------
u16 slow_crc16_t10_dif_array(u8 * data, unsigned int len)
{
    u16 crc = CRC16INIT;
    CRC16_SLOW_ARRAY_PROCEED(crc, data, len);
    return crc;
}

u16 slow_crc16_t10_dif_byte(u16 crc, const u8 data)
{
	CRC16_SLOW_BYTE_PROCEED(crc, data);
    return crc;
}

// base implementation ----------------------------
//typedef void (*crc_array_func)(void* const crc, const u8* const data, const reg len);
void slow_crc16_t10_dif_array_base(u16* const crc, const u8* data, reg len)
{
	u16 crc16 = *crc;
	CRC16_SLOW_ARRAY_PROCEED(crc16, data, len);
	*crc = crc16;
}

//typedef void (*crc_byte_func)(void* const crc, const u8 data);
void slow_crc16_t10_dif_byte_base(u16* const crc, const u8 data)
{
	u16 crc16 = *crc;
	CRC16_SLOW_BYTE_PROCEED(crc16, data);
	*crc = crc16;
}


#undef CRC16_SLOW_BYTE_PROCEED
#undef CRC16_SLOW_ARRAY_PROCEED
#endif /* _MY_CRC16_GENERIC_CALC_ENA */


#ifndef _MY_CRC_TEST_DISABLE

#include <stdio.h>

int crc16_test(u8 *data, reg len, u16 * res)
{
    u16 crc16[4] = {CRC16INIT, CRC16INIT, CRC16INIT, CRC16INIT};

    printf("\n\n----> crc16 test <-------------------------------------------\n");
    printf("sizeof buffer: %d\n\n", (int)len);

#ifdef _MY_CRC16_TABLE_CALC_ENA

    crc16[0] = fast_crc16_t10_dif_array(data, len);
    printf("crc16 --> fast_crc16_t10_dif_array: 0x%x", crc16[0]);

    CRC16START(crc16[1]);
    for(reg i = 0; i < len; ++i) {
        crc16[1] = fast_crc16_t10_dif_byte(crc16[1], data[i]);
    }
    CRC16FINAL(crc16[1]);
    printf("\ncrc16 --> fast_crc16_t10_dif_byte: 0x%x", crc16[1]);

#endif /* _MY_CRC16_TABLE_CALC_ENA */


#ifdef _MY_CRC16_GENERIC_CALC_ENA

    crc16[2] = slow_crc16_t10_dif_array(data, len);
    printf("\ncrc16 --> slow_crc16_t10_dif_array: 0x%x", crc16[2]);

    CRC16START(crc16[3]);
    for(reg i = 0; i < len; ++i) {
        crc16[3] = slow_crc16_t10_dif_byte(crc16[3], data[i]);
    }
    CRC16FINAL(crc16[3]);
    printf("\ncrc16 --> slow_crc16_t10_dif_byte: 0x%x", crc16[3]);

#endif /* _MY_CRC16_GENERIC_CALC_ENA */

    int counter_not_valid = 0;

#if defined(_MY_CRC16_TABLE_CALC_ENA) && defined (_MY_CRC16_GENERIC_CALC_ENA)

    for(int i = 1; i < 4; ++i) {
        if(crc16[0] != crc16[i]) {
            ++counter_not_valid;
        }
    }

    *res = crc16[0];

#elif defined(_MY_CRC16_TABLE_CALC_ENA) && !defined (_MY_CRC16_GENERIC_CALC_ENA)
    if(crc16[0] != crc16[1]) {
        ++counter_not_valid;
    }

    *res = crc16[0];
#else
    if(crc16[2] != crc16[3]) {
        ++counter_not_valid;
    }

    *res = crc16[2];
#endif /* defined(_MY_CRC16_TABLE_CALC_ENA) && defined (_MY_CRC16_GENERIC_CALC_ENA) */


    printf("\nerror counter %d\n", counter_not_valid);
    fflush(stdout);
    return counter_not_valid;
}

#endif /* _MY_CRC_TEST_DISABLE */


#endif /* _MY_CRC16_ENA */
