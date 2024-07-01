/*
 * assert_func.h
 *
 *  Created on: Mar 28, 2024
 *      Author: admin
 */

#ifndef _ASSERT_FUNC_NAME_H_
#define _ASSERT_FUNC_NAME_H_
#
#if defined(__GNUC__) || defined(__clang__)
#	define CHAOS_FUNC_NAME __func__
#elif defined(_MSC_VER)
#	define CHAOS_FUNC_NAME __FUNCTION__
#else
#	define CHAOS_FUNC_NAME ""
#endif /* CHAOS_FUNC_NAME definition */
#
#endif /* _ASSERT_FUNC_NAME_H_ */
