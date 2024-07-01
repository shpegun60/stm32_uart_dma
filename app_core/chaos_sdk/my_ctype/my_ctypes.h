#ifndef __MY_C_TYPES_H__
#define __MY_C_TYPES_H__ 1

#include "inline.h"
#include "byte_order/endianness.h"
#include "my_ctypes_def.h"
#include "my_ctype_id.h"
#include <string.h>


/************************************************************************************
 *  null pointer macro if not declarated upper
 */
#ifndef NULL
#   define NULL ((void *)0)
#endif /* NULL */

#ifndef MIN
#   define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif /* MIN */

#ifndef MAX
#   define MAX(a,b) (((a) < (b)) ? (b) : (a))
#endif /* MAX */

#ifndef ABS
#   define ABS(N) (((N) < 0.0) ? (-1.0 * (N)) : (N))
#endif /* ABS */

#ifndef NOT_IS_POWER_OF_2
#   define NOT_IS_POWER_OF_2(val) (((val - 1) & val) || (val == 0))
#endif /* NOT_IS_POWER_OF_2 */

/************************************************************************************
 *  macro for cast null pointer to user type
 */
#ifndef NULLPTR
#   define NULLPTR(type) ((type)NULL)
#endif /* NULLPTR */

/************************************************************************************
 *  macro for find sizeof struct-field without declare object
 *  (evaluation at compile time)
 *
 */
#ifndef SIZEOF_STRUCT
#   define SIZEOF_STRUCT(structName, field) ((size_t) sizeof(((structName *)0)->field))
#endif /* SIZEOF_STRUCT */

/************************************************************************************
 *  macro for find typeof struct-field without declare object
 *  WARNING!!! Do not change this macro if you use GNU-C compiler, change operator typeof to __typeof__ if you use ISO-C compiler!!!
 *  OR use only __typeof__ operator because it supported on GNU and ISO compilers
 *
 *  Type evaluation at compile time for type abstraction in C
 */
#ifndef TYPEOF_STRUCT
#   define TYPEOF_STRUCT(structName, field) __typeof__(((structName *)0)->field)
#endif /* TYPEOF_STRUCT */

/************************************************************************************
 *  macro for find typeof from user value
 *  WARNING!!! Do not change this macro if you use GNU-C compiler, change operator typeof to __typeof__ if you use ISO-C compiler!!!
 *  OR use only __typeof__ operator because it supported on GNU and ISO
 *
 *  Type evaluation at compile time
 */
#ifndef TYPEOF_DATA
#   define TYPEOF_DATA(data) __typeof__(data)
#endif /* TYPEOF_DATA */

#ifndef AUTO_VAL
#   define AUTO_VAL(var, value) TYPEOF_DATA(value) var = (value)
#endif /* AUTO_VAL */

#ifndef AUTO_VAL_NO_INIT
#   define AUTO_VAL_NO_INIT(var, value) TYPEOF_DATA(value) var
#endif /* AUTO_VAL_NO_INIT */


/************************************************************************************
 *  macro for declare unused variable in function
 */
#ifndef UNUSED
#   define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */
#endif /* UNUSED */

/************************************************************************************
 *  macro for getting number of elements from static array
 */
#ifndef ARRAY_COUNT
#   define ARRAY_COUNT(arr) (sizeof((arr))/sizeof((arr)[0]))
#endif /* ARRAY_COUNT */


/************************************************************************************
 * Macro to reinterpret your own data safely
 */
#define UNION_CAST(x, destType) \
   (((union {TYPEOF_DATA(x) a; destType b;})x).b)


/************************************************************************************
 *  macro for user bit operations
 */
#define MY_CTYPE_SET_BIT(to, pos, type) (to) |= ((type)(1UL << (pos)))
#define MY_CTYPE_RESET_BIT(to, pos, type) (to) &= ~((type)(1UL << (pos)))
#define MY_CTYPE_TOGGLE_BIT(to, pos, type) (to) ^= (type)(1UL << (pos))
#define MY_CTYPE_CHECK_BIT(from, pos) (((from) >> (pos)) & 1UL)
#define MY_CTYPE_WRITE_BIT(to, pos, value, type) (to) = (type)(((to) & ~(1UL << (pos))) | ((value) << (pos)))
// register operations
#define MY_CTYPE_REG_SET_BIT(_r, _b)    (*((volatile reg*)(_r)) |=  (_b))
#define MY_CTYPE_REG_CLR_BIT(_r, _b)    (*((volatile reg*)(_r)) &= ~(_b))
#define MY_CTYPE_REG_SET(_r, _d)        (*((volatile reg*)(_r))  =  (_d))
#define MY_CTYPE_REG_CPY(_r, _d)        (*((volatile reg*)(_r))  =  *((volatile reg*)(_d)))
#define MY_CTYPE_REG_GET(_r)        	(*((volatile reg*)(_r)))


/************************************************************************************
 *  Extern array which contain`s size for all types
 */
extern const reg typeLengthMappingArray[TYPE_ARRAY_LENGTH];


/************************************************************************************
 *  Macro for user copy
 */
#define  MY_CTYPE_USER_DATA_MEMCPY(n, from, to) memcpy(to, from, n)

STATIC_FORCEINLINE void MY_CTYPE_USER_DATA_REVCPY(reg n, const void* from, void* to)
{
    switch (n) {
    // -------------------
    case 0: break;
    // -------------------
    case sizeof(u8): {
        *(u8*)to = *(const u8*)from;
        break;}
    // -------------------
    case sizeof(u16): {
       u16 val = *(u16*)from;
       *(u16*)to = bswap16(val);
       break;}
    // -------------------
    case sizeof(u32): {
       u32 val = *(u32*)from;
       *(u32*)to = bswap32(val);
       break;}
    // -------------------
   case sizeof(u64): {
       u64 val = *(u64*)from;
       *(u64*)to = bswap64(val);
       break;}
   // -------------------
    default: {
        const u8* from_ptr = (const u8*)from + n - 1;
        u8* to_ptr = (u8*)to;

        do {
            *(to_ptr++) = *(from_ptr--);
        } while (--n);

        break;}
    }
}

/*
 * *************************************************
 * Define user copy
 * WARNING!!! this function without direction
 * and platform dependent
 * *************************************************
 */
STATIC_FORCEINLINE void MY_CTYPE_COPY_REGISTERS(volatile reg* from, volatile reg* to)
{
    MY_CTYPE_REG_CPY(to, from);
}

STATIC_FORCEINLINE void MY_CTYPE_REVCOPY_REGISTERS(volatile reg* from, volatile reg* to)
{
    reg rev;
    reg cpy =  *((volatile reg*)(from));

    MY_CTYPE_USER_DATA_REVCPY(sizeof(reg), &cpy, &rev);
    MY_CTYPE_REG_SET(to, rev);
}

STATIC_FORCEINLINE const reg* myCTypeGetTablePointer()
{
    return &typeLengthMappingArray[VOID_TYPE];
}


STATIC_FORCEINLINE const reg getMYCTypeLen(const reg type)
{
    if(type < TYPE_ARRAY_LENGTH) {
        return typeLengthMappingArray[type];
    }
    return 0;
}


// copy types -------------------------------------------------
void cTypeMemcpy(reg type, u8* from, u8* to);
void cTypeRevcpy(reg type, u8* from, u8* to);

// init data type ---------------------------------------
void cTypePointerInit(const reg type, void* ptr);

// string compleate------------------------------------------
int cTypeStrnCmp(reg n, const c8* str1, const c8* str2);
// string reverse compleate------------------------------------------
int cTypeStrnRevCmp(reg n, const c8* str1, const c8* str2);


#endif // MY_TYPES_H
