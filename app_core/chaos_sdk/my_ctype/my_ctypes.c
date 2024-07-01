#include "my_ctypes.h"

/*
 * *************************************************
 * Define user copy
 * *************************************************
 */

const reg typeLengthMappingArray[TYPE_ARRAY_LENGTH] = {
		0x00U,                  //VOID_TYPE           (0)
		// unsigned type
		sizeof(u8),             //UINT8_TYPE          (1)
		sizeof(u16),            //UINT16_TYPE         (2)
		sizeof(u32),            //UINT32_TYPE         (3)
		sizeof(u64),            //UINT64_TYPE         (4)
		//signed type
		sizeof(c8),             //CHAR_TYPE           (5)
		sizeof(i8),             //INT8_TYPE           (6)
		sizeof(i16),            //INT16_TYPE          (7)
		sizeof(i32),            //INT32_TYPE          (8)
		sizeof(i64),            //INT64_TYPE          (9)
		// floating point type
		sizeof(f32),            //FLOAT_TYPE          (10)
		sizeof(f64),            //DOUBLE_TYPE         (11)
		sizeof(f128),           //LONG_DOUBLE_TYPE    (12)
		// other types
		sizeof(b),              //BOOL_TYPE           (13)
		sizeof(i24),            //UINT24_TYPE         (14)
		sizeof(u24),            //INT24_TYPE          (15)
		sizeof(reg),            //REG_TYPE            (16) // platform depend type
		sizeof(sreg)            //SREG_TYPE           (17) // platform depend type
};

// copy types -------------------------------------------------
void cTypeMemcpy(reg type, u8* from, u8* to)
{
	reg len = getMYCTypeLen(type);
	MY_CTYPE_USER_DATA_MEMCPY(len, from, to);
}

void cTypeRevcpy(reg type, u8* from, u8* to)
{
	reg len = getMYCTypeLen(type);
	MY_CTYPE_USER_DATA_REVCPY(len, from, to);
}

// init data type ---------------------------------------
void cTypePointerInit(const reg type, void* ptr)
{
	const reg len = getMYCTypeLen(type);
	memset(ptr, 0, len);
}


// string compleate------------------------------------------
int cTypeStrnCmp(reg n, const c8* str1, const c8* str2)
{
	if(n == 0 || str1 == NULL || str2 == NULL) {
		return 0;
	}

	do {
		if(*str1++ != *str2++) {
			return 1;
		}

	} while (--n);

	return 0;
}


// this function is not work if text auto filled '\0' symbols
int cTypeStrnRevCmp(reg n, const c8* str1, const c8* str2)
{
	if(n == 0 || str1 == NULL || str2 == NULL) {
		return 0;
	}

	const char* str2_rev_ptr = str2 + n - 1;
	do {
		if(*str1++ != *str2_rev_ptr--) {
			return 1;
		}

	} while (--n);

	return 0;
}


