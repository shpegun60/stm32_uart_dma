
#include "preprocessor/preprocessor_template.h"
#include "my_ctype/my_ctype_cast.h"



#ifndef T
#   define T int
#endif /* T */

// read from (void *) to some type
T TEMPLATE(readRuntimeCast, T)(const void* const data, const reg type, reg* const ok)
{
	T value;
	(*ok) = 1;

#define SWITCH_CREATE_TYPE_READ(type)								\
		case MY_CTYPE_GET_TYPE_ID(type): {							\
			type tmp;												\
			MY_CTYPE_USER_DATA_MEMCPY(sizeof(type), data, &tmp);	\
			value = (T)( tmp );										\
			break;}

	switch(type) {

	// unsigned type---------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_READ(u8)
	SWITCH_CREATE_TYPE_READ(u16)
	SWITCH_CREATE_TYPE_READ(u32)
	SWITCH_CREATE_TYPE_READ(u64)


	// signed type-----------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_READ(c8)
	SWITCH_CREATE_TYPE_READ(i8)
	SWITCH_CREATE_TYPE_READ(i16)
	SWITCH_CREATE_TYPE_READ(i32)
	SWITCH_CREATE_TYPE_READ(i64)

		// floating point type-------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_READ(f32)
	SWITCH_CREATE_TYPE_READ(f64)
	SWITCH_CREATE_TYPE_READ(f128)

		// other types --------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_READ(b)
	SWITCH_CREATE_TYPE_READ(u24)
	SWITCH_CREATE_TYPE_READ(i24)
	SWITCH_CREATE_TYPE_READ(reg)

	default: {
		value = (T)(0);
		(*ok) = 0;
		break;}
	}

	return value;

#undef SWITCH_CREATE_TYPE_READ
}

// write from some type to (void *)
reg TEMPLATE(writeRuntimeCast, T)(void* const data, const reg type, const T value)
{

#define SWITCH_CREATE_TYPE_WRITE(type)										\
		case MY_CTYPE_GET_TYPE_ID(type): {									\
			const type tmp = (type)(value);									\
			MY_CTYPE_USER_DATA_MEMCPY(sizeof(type), &tmp, data);			\
			break;}

	reg ok = 1;

	switch(type) {

	// unsigned type---------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_WRITE(u8)
	SWITCH_CREATE_TYPE_WRITE(u16)
	SWITCH_CREATE_TYPE_WRITE(u32)
	SWITCH_CREATE_TYPE_WRITE(u64)


	// signed type-----------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_WRITE(c8)
	SWITCH_CREATE_TYPE_WRITE(i8)
	SWITCH_CREATE_TYPE_WRITE(i16)
	SWITCH_CREATE_TYPE_WRITE(i32)
	SWITCH_CREATE_TYPE_WRITE(i64)

		// floating point type-------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_WRITE(f32)
	SWITCH_CREATE_TYPE_WRITE(f64)
	SWITCH_CREATE_TYPE_WRITE(f128)

		// other types --------------------------------------------------------------------------------------------
	SWITCH_CREATE_TYPE_WRITE(b)
	SWITCH_CREATE_TYPE_WRITE(u24)
	SWITCH_CREATE_TYPE_WRITE(i24)
	SWITCH_CREATE_TYPE_WRITE(reg)

	default:
		ok = 0;
		break;
	}

	return ok;

#undef SWITCH_CREATE_TYPE_WRITE
}

#undef T

