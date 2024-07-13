/**
 * @file    assert_engine.h
 * @brief   This library for check expressions and send message if expressions is true on RUNTIME and on DEBUG version.
 *          If NDEBUG is not defined some expressions has change to empty define. (programmer must uncommit NDEBUG if code version released)
 *
 * @date
 * @author Shpegun60
 */

#ifndef __ASSERT_ENGINE_H_
#define __ASSERT_ENGINE_H_ 1
#
#// this library recomended include only .c or .cpp file!!!!!!!!!!!!!!!!
#
#//   C++ linking for mixed C++/C code
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#
#include "assert_engine_port.h"
#include "assert_func.h"
#include <stdbool.h>
#
#/*
# * *****************************************************************************************************************************************************************************
# *  smart asserts settings
# * *****************************************************************************************************************************************************************************
# */
#
#define NDEBUG                              	// uncommit this if program in release version
// =====> if NDEBUG is uncommited bellow defines have no meaning!!!!! <=====================================
//#define M_MESSAGE_ALWAYS_ENABLE         		1   // enabling runtime assert message filter and additon library information (see additional information macro)
//#define M_ASSERT_MSG_TEXT_DISABLE       		1   // disabling sending assert message text and do not saving this in .text section
//#define M_ASSERT_EXPR_DISABLE           		1   // disabling sending assert expression text, value and do not saving this in .text section
//#define M_ASSERT_FILE_LINE_TEXT_DISABLE 		1   // disabling sending assert file, line text and do not saving this in .text section
//#define M_ASSERT_FUNCTION_NAME_TEXT_DISABLE 	1	// disabling sending assert function-name text, value and do not saving this in .text section
#define M_ASSERT_OPTIONAL_CHECKS_DISABLE 		1  	// disabling optional checks in user code
#/*
# * ***************** SETTINGS APPLYING (DO NOT CHANGE!!!) ******************************************************
# */
#
#/// ==========> M_ASSERT_MSG_TEXT_DISABLE <==================================
#if defined(M_ASSERT_MSG_TEXT_DISABLE)
#   define ASSERT_MSG_TXT(...) 0 //"MSG"
#else
#   define ASSERT_MSG_TXT(...) __VA_ARGS__
#endif /* defined(M_ASSERT_EXPR_TEXT_DISABLE) */
#
#
#//// ==========> M_ASSERT_EXPR_DISABLE <====================================
#if defined(M_ASSERT_EXPR_DISABLE)
#   define ASSERT_EXPR_TXT(...) 0 //"EXPR"
#else
#   define ASSERT_EXPR_TXT(...) __VA_ARGS__
#endif /* defined(M_ASSERT_EXPR_TEXT_DISABLE) */
#
#//// ==========> M_ASSERT_FILE_LINE_TEXT_DISABLE <==========================
#if defined(M_ASSERT_FILE_LINE_TEXT_DISABLE)
#   define ASSERT_FILE(...)     0 //"FILE"
#else
#   define ASSERT_FILE(...)     __VA_ARGS__
#endif /* defined(M_ASSERT_FILE_LINE_TEXT_DISABLE) */
#
#//// ==========> M_ASSERT_FUNCTION_NAME_TEXT_DISABLE <==========================
#if defined(M_ASSERT_FUNCTION_NAME_TEXT_DISABLE)
#   define ASSERT_FUNCTION(...)     0 //"FUNC"
#else
#   define ASSERT_FUNCTION(...)     __VA_ARGS__
#endif /* defined(M_ASSERT_FUNCTION_NAME_TEXT_DISABLE) */
#
#//// ==========> M_ASSERT_OPTIONAL_CHECKS_DISABLE <=========================
#if defined(M_ASSERT_OPTIONAL_CHECKS_DISABLE)
#	define _OPTIONAL_CHECK_SWITCH(...)
#else
#	define _OPTIONAL_CHECK_SWITCH(...) __VA_ARGS__
#endif /* defined(M_ASSERT_OPTIONAL_CHECKS_DISABLE) */
#
#
#
#/*
# * ***********************************************************************
# *  smart asserts additional information macro
# * ***********************************************************************
# */
#define M_EMPTY     /* ignored expression */
#define M_ALWAYS 1  /* always proceed expression */
#define M_LIB_DATA_DEF          "[d]" /* adds text for some library data, user must adds to message list 0--> messageEna, 1-->libDescr  (0|1, "descr") */
#define M_LIB_NAME_DEF          "[s]" /* adds text for some library data, user must adds to message list 0--> libDescr ("descr") */
#define M_LIB_ENA_DEF           "[e]" /* adds text for some library data, user must adds to message list 0--> messageEna (0|1) */
#define M_LIB_ALWAYS_DIS_DEF    "[0]" /* always disable message in smart assert, nothing to adds    list 0--> libDescr ("descr")*/
#define M_LIB_ALWAYS_ENA_DEF    "[1]" /* always enable message in smart assert, nothing to adds     list 0--> libDescr ("descr")*/
#
#
#
#
#/// ==========> APPLY MACRO <================================================
#define ASSERT_ERROR(Expr_txt, Expr, File, Line, Func, Msg, ...)   __M_Error(ASSERT_EXPR_TXT(Expr_txt), Expr, ASSERT_FILE(File), Line, ASSERT_FUNCTION(Func), ASSERT_MSG_TXT(Msg, ##__VA_ARGS__))
#define ASSERT_WARNING(Expr_txt, Expr, File, Line, Func, Msg, ...) __M_Warning(ASSERT_EXPR_TXT(Expr_txt), Expr, ASSERT_FILE(File), Line, ASSERT_FUNCTION(Func), ASSERT_MSG_TXT(Msg, ##__VA_ARGS__))
#
#
#ifndef NDEBUG /* --------------------------------------------------------------------------------------------------------- */

void __M_Chaos_exit_set(const bool en);
void __M_Error(const char* const expr_str, const unsigned char expr,
                            const char* const file, const int line, const char* const func_name,
                            const char* const msg, ...);
void __M_Warning(const char* const expr_str, const unsigned char expr,
                            const char* const file, const int line, const char* const func_name,
                            const char* const msg, ...);
/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable this expression expr
 * ***********************************************************************************************************************************************
 */
// this macros will disable if NDEBUG is defined
#   define M_Assert_disableExpr(...) __VA_ARGS__

/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, ...)																					\
        _OPTIONAL_CHECK_SWITCH(__M_Assert_Break(Expr, #Expr, beforeExpr, afterExpr, Msg, ##__VA_ARGS__))
#   define __M_Assert_Break(Expr, Expr_txt, beforeExpr, afterExpr, Msg, ...)                                                            			\
    do {                                                                                                                                			\
        if (Expr) {                                                                                                                     			\
            beforeExpr;                                                                                                                 			\
            ASSERT_ERROR((Expr_txt), (1), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);                             			\
            afterExpr; /* ignored because programm is break, but compiler not known it */                                               			\
        }                                                                                                                               			\
    } while(0U)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                               			\
    do {                                                                                                                                			\
        if (Expr) {                                                                                                                     			\
            beforeExpr;                                                                                                                 			\
            ASSERT_ERROR((#Expr), (1), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);                                			\
            afterExpr; /* ignored because programm is break, but compiler not known it */                                               			\
        }                                                                                                                               			\
    } while(0U)


// else breaking asserts
#   define M_Assert_BreakElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                                			\
        _OPTIONAL_CHECK_SWITCH(__M_Assert_BreakElse(Expr, #Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ##__VA_ARGS__))
#   define __M_Assert_BreakElse(Expr, Expr_txt, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                    			\
    do {                                                                                                                                			\
        if(Expr) {                                                                                                                      			\
            trueExpr;                                                                                                                   			\
        } else { /* ignored else expression if NDEBUG */                                                                               				\
            falseBeforeExpr;                                                                                                            			\
            ASSERT_ERROR("if " Expr_txt " not confirmed: passed to else", (0), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);	\
            falseAfterExpr;                                                                                                             			\
        }                                                                                                                               			\
    } while(0U)

#   define M_Assert_BreakElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                       			\
    do {                                                                                                                                			\
        if(Expr) {                                                                                                                      			\
            trueExpr;                                                                                                                   			\
        } else {  /* ignored else expression if NDEBUG */                                                                               			\
            falseBeforeExpr;                                                                                                            			\
            ASSERT_ERROR("if " #Expr " not confirmed: passed to else", (0), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);		\
            falseAfterExpr;                                                                                                             			\
        }                                                                                                                               			\
    } while(0U)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */
#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, ...)                                                                      			\
		_OPTIONAL_CHECK_SWITCH(__M_Assert_Warning(Expr, (#Expr), beforeExpr, afterExpr, Msg, ##__VA_ARGS__))
#   define __M_Assert_Warning(Expr, Expr_txt, beforeExpr, afterExpr, Msg, ...)                                                          			\
    do {                                                                                                                                			\
        if (Expr) {                                                                                                                     			\
            beforeExpr;                                                                                                                 			\
            ASSERT_WARNING(Expr_txt, (1), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);                                      	\
            afterExpr;                                                                                                                  			\
        }                                                                                                                               			\
    } while(0U)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                             			\
    do {                                                                                                                                			\
        if (Expr) {                                                                                                                     			\
            beforeExpr;                                                                                                                 			\
            ASSERT_WARNING((#Expr), (1), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);                                      	\
            afterExpr;                                                                                                                  			\
        }                                                                                                                               			\
    } while(0U)


// else warning asserts
#   define M_Assert_WarningElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                              			\
        _OPTIONAL_CHECK_SWITCH(__M_Assert_WarningElse(Expr, #Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ##__VA_ARGS__))
#   define __M_Assert_WarningElse(Expr, Expr_txt, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                  			\
    do {                                                                                                                                			\
        if(Expr) {                                                                                                                      			\
            trueExpr;                                                                                                                   			\
        } else { /* ignored else expression if NDEBUG */                                                                                			\
            falseBeforeExpr;                                                                                                            			\
            ASSERT_WARNING("if " #Expr " not confirmed: passed to else", (0), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);		\
            falseAfterExpr;                                                                                                             			\
        }                                                                                                                               			\
    } while(0U)
#
#   define M_Assert_WarningElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                     			\
    do {                                                                                                                                			\
        if(Expr) {                                                                                                                      			\
            trueExpr;                                                                                                                   			\
        } else { /* ignored else expression if NDEBUG */                                                                                			\
            falseBeforeExpr;                                                                                                            			\
            ASSERT_WARNING("if " #Expr " not confirmed: passed to else", (0), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), (Msg), ##__VA_ARGS__);		\
            falseAfterExpr;                                                                                                             			\
        }                                                                                                                               			\
    } while(0U)


/*
 * ***********************************************************************************************************************************************
 *  functions
 * ***********************************************************************************************************************************************
 */
#	if defined(M_ASSERT_OPTIONAL_CHECKS_DISABLE)
#   	define M_Assert_SafeFunctionCall(foo, ...)                                                                          				\
			do {                                                                                                                            \
				if(foo) {                                                                                                                   \
					__VA_ARGS__;                                                                                                       		\
				}                                                                                                                           \
			} while(0U)
#else
#   	define M_Assert_SafeFunctionCall(foo, ...)                                                                              							\
			do {                                                                                                                                			\
				if(foo) {                                                                                                                       			\
					__VA_ARGS__;                                                                                                            				\
				} else {                                                                                                                        			\
					ASSERT_WARNING((#foo), (0), (__FILE__), (__LINE__), (CHAOS_FUNC_NAME), ("NO exists function"));                                       	\
				}                                                                                                                               			\
			} while(0U)
#endif /* defined(M_ASSERT_OPTIONAL_CHECKS_DISABLE) */



#else /* defined NDEBUG ---------------------------------------------------------------------------------------------------------------------------------------- */

#define __M_Chaos_exit_set(en)
/*
 * ***********************************************************************************************************************************************
 *  defines simple if NDEBUG disable expr
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_disableExpr(...)
/*
 * ***********************************************************************************************************************************************
 *  defines for break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Break(Expr, beforeExpr, afterExpr, Msg, ...)

#   define M_Assert_BreakSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                           \
    do {                                                                                                                            \
        if (Expr) {                                                                                                                 \
            beforeExpr;                                                                                                             \
            afterExpr;                                                                                                              \
        }                                                                                                                           \
    } while(0U)

// else breaking asserts
#   define M_Assert_BreakElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                            \
    do {                                                                                                                            \
        trueExpr;                                                                                                                   \
    } while(0U)

#   define M_Assert_BreakElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                   \
    do {                                                                                                                            \
        if(Expr) {                                                                                                                  \
            trueExpr;                                                                                                               \
        }                                                                                                                           \
    } while(0U)

/*
 * ***********************************************************************************************************************************************
 *  defines no break program
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_Warning(Expr, beforeExpr, afterExpr, Msg, ...)

#   define M_Assert_WarningSaveCheck(Expr, beforeExpr, afterExpr, Msg, ...)                                                         \
    do {                                                                                                                            \
        if (Expr) {                                                                                                                 \
            beforeExpr;                                                                                                             \
            afterExpr;                                                                                                              \
        }                                                                                                                           \
    } while(0U)

// else warning asserts
#   define M_Assert_WarningElse(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                          \
    do {                                                                                                                            \
        trueExpr;                                                                                                                   \
    } while(0U)

#   define M_Assert_WarningElseSaveCheck(Expr, trueExpr, falseBeforeExpr, falseAfterExpr, Msg, ...)                                 \
    do {                                                                                                                            \
        if(Expr) {                                                                                                                  \
            trueExpr;                                                                                                               \
        }                                                                                                                           \
    } while(0U)


/*
 * ***********************************************************************************************************************************************
 *  functions
 * ***********************************************************************************************************************************************
 */

#   define M_Assert_SafeFunctionCall(foo, ...)                                                                          			\
    do {                                                                                                                            \
        if(foo) {                                                                                                                   \
        	__VA_ARGS__;                                                                                                       		\
        }                                                                                                                           \
    } while(0U)



#endif /* NDEBUG */

#include <stdio.h>

void __M_DBG(const char* const msg, ...);
void __M_DBG_ERR(const char* const msg, ...);
void __M_DBG_FILE(FILE * file, const char* const msg, ...);


void __M_assert_test();


#define PRINT_ONCE(...)                                                                                                             \
    do {                                                                                                                            \
        static int __printed = 0;                                                                                                   \
        if(!__printed) {                                                                                                            \
            printf(__VA_ARGS__);                                                                                                    \
            __printed = 1;                                                                                                          \
        }                                                                                                                           \
    } while(0U)


//#define TO_TXT(a) #a

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ASSERT_ENGINE_H_ */
