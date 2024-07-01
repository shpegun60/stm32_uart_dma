#include "assert_engine.h"
#include <stdio.h>
#include <stdarg.h>

#ifndef NDEBUG /* ---------------------------------------------------------------------------------------------------------- */

#include <assert.h>


#define __M_IF_ASSERT_ADDITION_DATA(msg) (((msg)[0] == '[') && ((msg)[2] == ']'))
#define __M_ASSERT_DATA(msg) ((msg)[1])

static inline void __M_SEND_ASSERT_MSG(const char* const header,
                                const char* const expr_str, const unsigned char expr,
                                const char* const file, const int line, const char* const func_name,
                                const char* const msg, va_list args)
{
#if !defined(M_ASSERT_MSG_TEXT_DISABLE)
    int assertEna 	= 1;
    char* descr 	= 0;
    const int _additional_data_ex = __M_IF_ASSERT_ADDITION_DATA(msg);

    if(_additional_data_ex) {

        switch(__M_ASSERT_DATA(msg)) {

        case 'd':
            // get library info
            assertEna = va_arg(args, const int);
            descr = va_arg(args, char*);
            break;

        case 's':
            // get library info
            assertEna = 1;
            descr = va_arg(args, char*);
            break;

        case 'e':
            // get library info
            assertEna   = va_arg(args, const int);
            descr       = 0;
            break;

        case '0':
            descr = va_arg(args, char*);
#	if !defined(M_MESSAGE_ALWAYS_ENABLE)
            (void)descr;
            return;
#   endif /* !defined(M_MESSAGE_ALWAYS_ENABLE) */
            break;

        default:  descr = va_arg(args, char*); break;

        }

    }
    (void)assertEna;
#endif /* defined(M_ASSERT_MSG_TEXT_DISABLE) */


#if !defined(M_MESSAGE_ALWAYS_ENABLE) && !defined(M_ASSERT_MSG_TEXT_DISABLE)
    // send message if enabled message
    if(assertEna) {
#endif /* !defined(M_MESSAGE_ALWAYS_ENABLE) && !defined(M_ASSERT_MSG_TEXT_DISABLE) */

        // print header-------------------------------------------------------
        fprintf(stderr, "\n%s\n", header);
        // print function if enable ------------------------------------------
#if !defined(M_ASSERT_FUNCTION_NAME_TEXT_DISABLE)
        fprintf(stderr, "Function:\t%s\n", func_name);
#endif /* !defined(M_ASSERT_FUNCTION_NAME_TEXT_DISABLE) */

#if !defined(M_ASSERT_MSG_TEXT_DISABLE)
        // print library name if exists and enable ------------------------------------------
        if(descr) {
            fprintf(stderr, "Library Name:\t%s\n", descr);
        }
        // print msg text if enable ------------------------------------------
        fprintf(stderr, "Assert failed:\t");
        if(_additional_data_ex) {
            vfprintf(stderr, &msg[3], args);
        } else {
            vfprintf(stderr, msg, args);
        }
        fprintf(stderr, "\n");
#endif /* defined(M_ASSERT_MSG_TEXT_DISABLE) */

        // print expression text if enable ------------------------------------------
#if !defined(M_ASSERT_EXPR_DISABLE)
        fprintf(stderr, "Expression:\t%s, value: %d\n", expr_str, expr);
#endif /* defined(M_ASSERT_EXPR_TEXT_DISABLE) */

        // print file & line if enable ------------------------------------------
#if !defined(M_ASSERT_FILE_LINE_TEXT_DISABLE)
            fprintf(stderr, "Source:\t\t%s, line: %d\n", file, line);
#endif /* defined(M_ASSERT_FILE_LINE_TEXT_DISABLE) */

        fflush(stderr);
#if !defined(M_MESSAGE_ALWAYS_ENABLE) && !defined(M_ASSERT_MSG_TEXT_DISABLE)
    }
#endif /* !defined(M_MESSAGE_ALWAYS_ENABLE) && !defined(M_ASSERT_MSG_TEXT_DISABLE) */

    (void)header;
    (void)expr_str;
    (void)expr;
    (void)file;
    (void)line;
    (void)func_name;
    (void)msg;
    (void)args;
}

static bool _m_chaos_exit_en = false;
void __M_Chaos_exit_set(const bool en) {_m_chaos_exit_en = en;}
void __M_Error(const char* const expr_str, const unsigned char expr,
                            const char* const file, const int line, const char* const func_name,
                            const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    __M_SEND_ASSERT_MSG("PROGRAMM EXIT WITH ERROR!!!", expr_str, expr, file, line, func_name, msg, args);
    va_end(args);

//    if(_m_chaos_exit_en) {
//    	abort(); /* exit programm */
//    }
}

void __M_Warning(const char* const expr_str, const unsigned char expr,
                            const char* const file, const int line, const char* const func_name,
                            const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    __M_SEND_ASSERT_MSG("WARNING!!!", expr_str, expr, file, line, func_name, msg, args);
    va_end(args);
}

#undef __M_IF_ASSERT_ADDITION_DATA
#undef __M_ASSERT_DATA

#endif /* NDEBUG ---------------------------------------------------------------------------------------------------------- */

void __M_DBG(const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stdout, msg, args);
    fprintf(stdout, "\n");
    fflush(stdout);
    va_end(args);
}

void __M_DBG_ERR(const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n");
    fflush(stderr);
    va_end(args);
}

void __M_DBG_FILE(FILE * file, const char* const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(file, msg, args);
    fprintf(file, "\n");
    fflush(file);
    va_end(args);
}

void __M_assert_test()
{
	int i = 1;
	__M_Chaos_exit_set(false);
    // Test M_Assert_Break without parameters
    M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break without parameters");
    // Test M_Assert_Break with one parameter
    M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break with parameter: %d", 123);
    // Test M_Assert_Break with two parameters
    M_Assert_Break(1, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break with two parameters: %d, %d", 123, 456);
    // Test M_Assert_Warning without parameters
    M_Assert_Warning(1, M_EMPTY, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_Warning without parameters");
    // Test M_Assert_Warning with one parameter
    M_Assert_Warning(1, ++i, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_Warning with parameter: %d", i);
    // Test M_Assert_Warning with two parameters
    M_Assert_Warning(1, M_EMPTY, ++i, "RUNTIME WARNING Assert test: M_Assert_Warning with two parameters: %d, %d", i, 456);
    // Test M_Assert_Warning with two parameters and different formatting
    M_Assert_Warning(1, M_EMPTY, M_EMPTY, "RUNTIME WARNING Assert test:" "M_Assert_Warning with two parameters: %d, %d", i, 456);
    // Test M_Assert_Break with two parameters and increment
    M_Assert_Break(1, ++i, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_Break with two parameters: %d, %d", i, 456);
    // Test M_Assert_Break with library data
    M_Assert_Break(1, ++i, M_EMPTY, M_LIB_DATA_DEF"With library: M_Assert_Break with two parameters: %d, %d", 0, "lib_name", i, 456);
    // Test M_Assert_BreakSaveCheck
    M_Assert_BreakSaveCheck(1, ++i, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_BreakSaveCheck with parameter: %d", i);
    // Test M_Assert_WarningSaveCheck
    M_Assert_WarningSaveCheck(1, ++i, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_WarningSaveCheck with parameter: %d", i);
    // Test M_Assert_BreakElse
    //M_Assert_WarningElse(i == 0, M_EMPTY, M_EMPTY, M_EMPTY, "RUNTIME ERROR Assert test: M_Assert_BreakElse");
    M_Assert_BreakElse(i == 0, M_EMPTY, M_EMPTY, M_EMPTY, "sfsagsdggdgsg");
    // Test M_Assert_WarningElse
    M_Assert_WarningElse(i != 0, M_EMPTY, M_EMPTY, M_EMPTY, "RUNTIME WARNING Assert test: M_Assert_WarningElse");
    // Test M_Assert_SafeFunctionCall
    i = 0;
    M_Assert_SafeFunctionCall(i, printf("hello "));
    // Test M_Assert_Break with custom message
    M_Assert_Break(1, ++i, M_EMPTY, "[s]With library: Custom assert message with two parameters: %d, %d", "hhhh",i, 456);
    // Test M_Assert_Warning with custom message
    M_Assert_Warning(1, ++i, M_EMPTY, "[s]With library: Custom warning message with two parameters: %d, %d", "hhhh", i, 456);
    // Test M_Assert_Break with error code
    M_Assert_Break(1, ++i, M_EMPTY, "[e]With error code: Assert with two parameters: %d, %d",1, i, 123);
    // Test M_Assert_Warning with disabled message
    M_Assert_Warning(1, ++i, M_EMPTY, "[0]With disabled message: %d, %d", i, 456);
	
	(void)i;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 int test_value = 0;
    // Test error assertion without additional data
    M_Assert_Break(test_value == 0, M_EMPTY, M_EMPTY, "Test case 1: Assertion failed without additional data");
    // Test error assertion with one additional parameter
    M_Assert_Break(test_value == 0, M_EMPTY, M_EMPTY, "Test case 2: Assertion failed with one parameter: %d", test_value);
    // Test error assertion with two additional parameters
    M_Assert_Break(test_value == 0, M_EMPTY, M_EMPTY, "Test case 3: Assertion failed with two parameters: %d, %d", test_value, 123);
    // Test warning assertion without additional data
    M_Assert_Warning(test_value == 0, M_EMPTY, M_EMPTY, "Test case 4: Warning without additional data");
    // Test warning assertion with one additional parameter
    M_Assert_Warning(test_value == 0, M_EMPTY, M_EMPTY, "Test case 5: Warning with one parameter: %d", test_value);
    // Test warning assertion with two additional parameters
    M_Assert_Warning(test_value == 0, M_EMPTY, M_EMPTY, "Test case 6: Warning with two parameters: %d, %d", test_value, 123);
    // Test error assertion with library data
    M_Assert_Break(test_value == 0, M_EMPTY, M_EMPTY, "[d]Test case 7: Assertion with library data", 1, "lib_name");
    // Test warning assertion with library data
    M_Assert_Warning(test_value == 0, M_EMPTY, M_EMPTY, "[s]Test case 8: Warning with library data: %s", "lib_name");
    // Test error assertion with error code
    M_Assert_Break(test_value == 0, M_EMPTY, M_EMPTY, "[e]Test case 9: Assertion with enable code: %d", 1);
    // Test warning assertion with disabled message
    M_Assert_Warning(test_value == 0, M_EMPTY, M_EMPTY, "[0]Test case 10: Disabled message");
    // Test M_Assert_BreakSaveCheck
    M_Assert_BreakSaveCheck(test_value == 0, M_EMPTY, M_EMPTY, "Test case 11: M_Assert_BreakSaveCheck");
    // Test M_Assert_WarningSaveCheck
    M_Assert_WarningSaveCheck(test_value == 0, M_EMPTY, M_EMPTY, "Test case 12: M_Assert_WarningSaveCheck");
    // Test M_Assert_BreakElse
    M_Assert_BreakElse(test_value == 0, M_EMPTY, M_EMPTY, M_EMPTY, "Test case 13: M_Assert_BreakElse");
    // Test M_Assert_WarningElse
    M_Assert_WarningElse(test_value != 0, M_EMPTY, M_EMPTY, M_EMPTY, "Test case 14: M_Assert_WarningElse");
    // Test M_Assert_SafeFunctionCall
    M_Assert_SafeFunctionCall(test_value == 0, printf("Test case 15: Safe function call\n"));
    // Test debug messages
    __M_DBG("Test case 16: Debug message without parameters");
    __M_DBG("Test case 17: Debug message with one parameter: %d", test_value);
    __M_DBG_ERR("Test case 18: Debug error message with two parameters: %d, %s", test_value, "error");
    __M_DBG_FILE(stdout, "Test case 19: Debug message to file: %d", test_value);

    __M_Chaos_exit_set(true);
    (void)test_value;
}






