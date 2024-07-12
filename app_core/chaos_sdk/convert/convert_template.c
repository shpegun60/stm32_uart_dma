#include "preprocessor/preprocessor_template.h"
#include "my_ctype/my_ctypes.h"


#ifndef T
#define T int
#endif /* T */

/*
 * ******************************************
 * LSB - first
 * ******************************************
 */
T TEMPLATE(convertRead_LSB, T) (u8* data, reg * pos)
{
    reg posInternal = (*pos);
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (data + posInternal), (&value));
    posInternal += sizeof(T);
    (*pos) = posInternal;
    return value;
}

void TEMPLATE(convertWrite_LSB, T) (u8* data, reg* pos, T value)
{
    reg posInternal = (*pos);
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), &value, data + posInternal);
    posInternal += sizeof(T);
    (*pos) = posInternal;
}

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_LSB, T) (u8* data, reg* pos, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + sizeof(T)) > buffSize) {
        (*ok) = false;
        return (T)(0);
    }

    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (data + posInternal), (&value));
    posInternal += sizeof(T);
    (*ok) = true;
    (*pos) = posInternal;
    return value;
}
void TEMPLATE(convertWriteCheck_LSB, T) (u8* data, reg* pos, T value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + sizeof(T)) > buffSize) {
        (*ok) = false;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (&value), (data + posInternal));
    posInternal += sizeof(T);
    (*ok) = true;
    (*pos) = posInternal;
}

// position not a pointer-----------------------------------------------------------------------
T TEMPLATE(convertRead_cpos_LSB, T) (u8* data, reg pos)
{
    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (data + pos), (&value));
    return value;
}

void TEMPLATE(convertWrite_cpos_LSB, T) (u8* data, reg pos, T value)
{
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (&value), (data + pos));
}

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_LSB, T) (u8* data, reg pos, reg buffSize, b* ok)
{
    if((pos + sizeof(T)) > buffSize) {
        (*ok) = false;
        return (T)(0);
    }

    T value;
    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (data + pos), (&value));
    (*ok) = true;
    return value;
}
void TEMPLATE(convertWriteCheck_cpos_LSB, T) (u8* data, reg pos, T value, reg buffSize, b* ok)
{
    if((pos + sizeof(T)) > buffSize) {
        (*ok) = false;
        return;
    }

    MY_CTYPE_USER_DATA_MEMCPY(sizeof(T), (&value), (data + pos));
    (*ok) = true;
}


/*
 * ******************************************
 * MSB - first
 * ******************************************
 */

T TEMPLATE(convertRead_MSB, T) (u8* data, reg* pos)
{
    reg posInternal = (*pos);

    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (data + posInternal), (&value));
    posInternal += sizeof(T);

    (*pos) = posInternal;
    return value;
}

void TEMPLATE(convertWrite_MSB, T) (u8* data, reg* pos, T value)
{
    reg posInternal = (*pos);

    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (&value), (data + posInternal));
    posInternal += sizeof(T);

    (*pos) = posInternal;
}

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_MSB, T) (u8* data, reg* pos, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + sizeof(T)) > buffSize) {
        (*ok) = false;
        return (T)(0);
    }

    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (data + posInternal), (&value));
    posInternal += sizeof(T);
    (*ok) = true;
    (*pos) = posInternal;
    return value;
}
void TEMPLATE(convertWriteCheck_MSB, T) (u8* data, reg* pos, T value, reg buffSize, b* ok)
{
    reg posInternal = (*pos);

    if((posInternal + sizeof(T)) > buffSize) {
        (*ok) = false;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (&value), (data + *pos));
    posInternal += sizeof(T);
    (*ok) = true;
    (*pos) = posInternal;
}

// position not a pointer
T TEMPLATE(convertRead_cpos_MSB, T) (u8* data, reg pos)
{
    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (data + pos), (&value));
    return value;
}

void TEMPLATE(convertWrite_cpos_MSB, T) (u8* data, reg pos, T value)
{
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (&value), (data + pos));
}

//with check buffer-----------------------------------------------------------------------------
T TEMPLATE(convertReadCheck_cpos_MSB, T) (u8* data, reg pos, reg buffSize, b* ok)
{
    if((pos + sizeof(T)) > buffSize) {
        (*ok) = false;
        return (T)(0);
    }

    T value;
    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (data + pos), (&value));
    (*ok) = true;
    return value;
}
void TEMPLATE(convertWriteCheck_cpos_MSB, T) (u8* data, reg pos, T value, reg buffSize, b* ok)
{
    if((pos + sizeof(T)) > buffSize) {
        (*ok) = false;
        return;
    }

    MY_CTYPE_USER_DATA_REVCPY(sizeof(T), (&value), (data + pos));
    (*ok) = true;
}



/*
 * ******************************************
 * test
 * ******************************************
 */
#ifndef CONVERT_TEST_DISABLE

#include <stdlib.h>


int TEMPLATE(convertTest_WRITE_READ, T) (T (*read_ptr)(u8* data, reg * pos), void (*write_ptr)(u8* data, reg* pos, T value), T value)
{
    u8 buff[sizeof(T) * 2 + 2];
    reg pos = 0;
    T value_check;

    int notValidcnt = 0;

    // two write function check
    write_ptr(buff, &pos, value);
    if(pos != sizeof(T)) {
        ++notValidcnt;
    }

    write_ptr(buff, &pos, value);
    if(pos != (sizeof(T) * 2)) {
        ++notValidcnt;
    }
    pos = 0;

    // two read function check
    value_check = read_ptr(buff, &pos);
    if(value_check != value || pos != sizeof(T)) {
        ++notValidcnt;
    }
    value_check = read_ptr(buff, &pos);
    if(value_check != value || pos != (sizeof(T) * 2)) {
        ++notValidcnt;
    }
    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_CPOS, T) (T (*read_ptr)(u8* data, reg pos), void (*write_ptr)(u8* data, reg pos, T value), T value)
{
    u8 buff[sizeof(T) * 2 + 2];
    reg pos = 0;
    T value_check;

    int notValidcnt = 0;

    // two write function check
    write_ptr(buff, pos, value);
    pos += sizeof(T);
    write_ptr(buff, pos, value);
    pos = 0;

    // two read function check
    value_check = read_ptr(buff, pos);
    if(value_check != value) {
        ++notValidcnt;
    }
    pos += sizeof(T);

    value_check = read_ptr(buff, pos);
    if(value_check != value) {
        ++notValidcnt;
    }
    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, T) (T (*read_ptr)(u8* data, reg* pos, reg buffSize, b* ok), void (*write_ptr)(u8* data, reg* pos, T value, reg buffSize, b* ok),  T value)
{
    reg buffsize = sizeof(T) * 2 + 2;
    u8 buff[sizeof(T) * 2 + 2];
    reg pos = 0;
    b ok = false;
    T value_check;

    int notValidcnt = 0;

    // write function check
    ok = true;
    for(reg i = 0; i < sizeof(T); ++i) {
        write_ptr(buff, &pos, value, i, &ok);
        if(ok == true || pos != 0) {
            ++notValidcnt;
        }
        ok = true;
    }
    ok = false;
    pos = 0;

    write_ptr(buff, &pos, value, buffsize, &ok);
    if(pos != sizeof(T) || ok != true) {
        ++notValidcnt;
    }
    ok = false;

    write_ptr(buff, &pos, value, buffsize, &ok);
    if(pos != sizeof(T) * 2 || ok != true) {
        ++notValidcnt;
    }
    ok = false;
    pos = 0;

    // read functions check
    ok = true;
    for(reg i = 0; i < sizeof(T); ++i) {
        value_check = read_ptr(buff, &pos, i, &ok);
        if(ok == true || pos != 0 || value_check != ((T)0)) {
            ++notValidcnt;
        }
        ok = true;
    }
    ok = false;

    value_check = read_ptr(buff, &pos, buffsize, &ok);
    if(value_check != value || (ok == false) || pos != sizeof(T)) {
        ++notValidcnt;
    }
    ok = false;

    value_check = read_ptr(buff, &pos, buffsize, &ok);
    if(value_check != value || (ok == false) || pos != sizeof(T)*2) {
        ++notValidcnt;
    }

    return notValidcnt;
}

int TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, T) (T (*read_ptr)(u8* data, reg pos, reg buffSize, b* ok), void (*write_ptr)(u8* data, reg pos, T value, reg buffSize, b* ok),  T value)
{
    reg buffsize = sizeof(T) * 2 + 2;
    u8 buff[sizeof(T) * 2 + 2];
    reg pos = 0;
    b ok = false;
    T value_check;

    int notValidcnt = 0;

    // write function check
    ok = true;
    for(reg i = 0; i < sizeof(T); ++i) {
        write_ptr(buff, pos, value, i, &ok);
        if(ok == true) {
            ++notValidcnt;
        }
        ok = true;
    }
    ok = false;

    write_ptr(buff, pos, value, buffsize, &ok);
    if(ok != true) {
        ++notValidcnt;
    }
    ok = false;
    pos += sizeof(T);

    write_ptr(buff, pos, value, buffsize, &ok);
    if(ok != true) {
        ++notValidcnt;
    }
    ok = false;
    pos = 0;

    // read functions check
    ok = true;
    for(reg i = 0; i < sizeof(T); ++i) {
        value_check = read_ptr(buff, pos, i, &ok);
        if(ok == true || value_check != ((T)0)) {
            ++notValidcnt;
        }
         ok = true;
    }
    ok = false;
    pos = 0;

    value_check = read_ptr(buff, pos, buffsize, &ok);
    if(value_check != value || (ok == false)) {
        ++notValidcnt;
    }
    ok = false;
    pos += sizeof(T);

    value_check = read_ptr(buff, pos, buffsize, &ok);
    if(value_check != value || (ok == false)) {
        ++notValidcnt;
    }

    return notValidcnt;
}




int TEMPLATE(convertTest, T) (int testN, int randomSeed)
{
    srand(sizeof(T) + randomSeed); // use type size as seed for random generator

    T value = (T)(sizeof(T));
    T valueLast = 0;

    int testCounter = 0;
    while(testN--) {
        /*
         *****************************************************************
            lsb--------------------------------------------------------
         *****************************************************************
        */

        testCounter += TEMPLATE(convertTest_WRITE_READ, T)(TEMPLATE(convertRead_LSB, T), TEMPLATE(convertWrite_LSB, T), value);

        // buffer functions check test-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, T) (TEMPLATE(convertReadCheck_LSB, T), TEMPLATE(convertWriteCheck_LSB, T),  value);

        // cpos
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS, T) (TEMPLATE(convertRead_cpos_LSB, T), TEMPLATE(convertWrite_cpos_LSB, T), value);

        // buffer functions check test cpos-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, T) (TEMPLATE(convertReadCheck_cpos_LSB, T), TEMPLATE(convertWriteCheck_cpos_LSB, T),  value);



        /*
         *****************************************************************
            msb--------------------------------------------------------
         *****************************************************************
        */
        testCounter += TEMPLATE(convertTest_WRITE_READ, T)(TEMPLATE(convertRead_MSB, T), TEMPLATE(convertWrite_MSB, T), value);


        // buffer functions check test-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_BUFFER_CHK, T) (TEMPLATE(convertReadCheck_MSB, T), TEMPLATE(convertWriteCheck_MSB, T),  value);

        // cpos
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS, T) (TEMPLATE(convertRead_cpos_LSB, T), TEMPLATE(convertWrite_cpos_LSB, T), value);

        // buffer functions check test cpos-----------------------------------------------------
        testCounter += TEMPLATE(convertTest_WRITE_READ_CPOS_BUFFER_CHK, T) (TEMPLATE(convertReadCheck_cpos_MSB, T), TEMPLATE(convertWriteCheck_cpos_MSB, T) ,  value);

        valueLast = value;
        value = (T)(rand());
        while((value == 0) || (valueLast == value)) {
            value = (T)(rand());
        }
    }

    return testCounter;
}
#endif /* CONVERT_TEST_DISABLE */


#undef T

