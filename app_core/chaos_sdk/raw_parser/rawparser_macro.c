#include "rawparser_macro.h"

#ifndef D_RAW_P_TEST_DISABLE

//**************************************************************************************************
// TEST
//**************************************************************************************************
static int pos_i = 0; // into descriptor type position saved
void rawPTestWriteUni(u8* desc, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    u8* to = (u8*)(desc + pos_i);
    pos_i += totalLenInByte;


#if defined(__LITTLE_ENDIAN__)
    while(totalLenInByte--) {
        *to++ = *data++;
    }

    (void)typelenInByte;
#else /* defined(__BIG_ENDIAN__) */
    for(reg i = 0; i < totalLenInByte; i+= typelenInByte) {
        reg n = typelenInByte;
        while(n--) {
            *to++ = *(data + n + i);
        }
    }
#endif /* ORDER SELECTION */

}

void rawPTestWriteType(u8* desc, reg totalLenInByte, reg typelenInByte, u8 *data)
{
    printf("writeTypeFoo size: %d\n", totalLenInByte);
    (void)data;
    (void)desc;
    (void)typelenInByte;
}



int rawParserMacroTest()
{
    printf("\n------------- RAW_PARSER_MACRO_TEST -----------------------------\n");
    printf("%s\n\n", PREPROCESSOR_MACRO_DEBUG(RAW_P_MERGE_OPERATION(sizeof, PREPROCESSOR_PLUS, int, int, int, bool, char)));
    printf("%s\n\n", PREPROCESSOR_MACRO_DEBUG(RAW_P_MERGE_WRITE_OPERATION(sizeof, par1, sizeof,u8*, PREPROCESSOR_COMMA_POINT, int, int, int, bool, char)));
    printf("%s\n\n", PREPROCESSOR_STRINGIFY_VARIADIC(            WRITE_PAYLOAD_MACRO(rawPTestWriteType, NULL, PREPROCESSOR_EMPTY(), {
                                                                 printf("total size: %d\n", totalSize);
                                                                 printf("const: %d\n", const_5);
                                                             }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 11, arr3_ptr, $CONST, 123, i32)));

    i32 a = 1;
    reg b = 2;
    u16 c = 6;

    u32 arr16[10] = {0x01020304,2,3,4,5,6,7,8,9,10};
    u32 arr16_tmp[100] = {0,0,0,0,0,0,0,0,0,0};



    printf("\n ------------------- before data --------------------------------\n");
    for(reg i = 0; i < sizeof(arr16)/sizeof(arr16[0]); ++i) {
        printf("\n data[%d]: %08x\n",(int)i, (int)arr16[i]);
    }

    pos_i = 0;
    WRITE_PAYLOAD_MACRO(rawPTestWriteUni, (u8*)arr16_tmp, PREPROCESSOR_EMPTY(), PREPROCESSOR_EMPTY(), $STATIC_ARRAY, arr16, a, b, c);

    printf("\n ------------------- after data -------------------------------- %d\n", pos_i);
    for(reg i = 0; i < (10 + 9); ++i) {
        printf("\n data[%d]: %08x\n",i, (int)arr16_tmp[i]);
    }


    u8 arr[10] = {0,0,0,0,0,0,0,0,0,0};
    u8 arr2[11] = {0,0,0,0,0,0,0,0,0,0};
    u8 * arr3_ptr = arr2;


    WRITE_PAYLOAD_MACRO(rawPTestWriteType, NULL, PREPROCESSOR_EMPTY(), {
                            printf("total size: %d\n", totalSize);
                            printf("const: %d\n", (int)const_5);
                        }, a, b, c, $STATIC_ARRAY, arr, $POINTER, 11, arr3_ptr, $CONST, 123, i32);

    printf("\n------------- END OF RAW_PARSER_MACRO_TEST -----------------------------\n");


    MY_CTYPE_GET_TYPE(UINT8_TYPE) vvv = 0;

    (void)vvv;
    printf("detect type: %s\n", PREPROCESSOR_STRINGIFY_VARIADIC( MY_CTYPE_GET_TYPE_ID(b) ) );



    fflush(stdout);
    fflush(stderr);

    return 0;
}

#endif /* D_RAW_P_TEST_DISABLE */
