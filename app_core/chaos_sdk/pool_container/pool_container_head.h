/*
 * pool_container_head.h
 *
 *  Created on: Mar 27, 2024
 *      Author: admin
 */

#ifndef _POOL_CONTAINER_HEAD_H_
#define _POOL_CONTAINER_HEAD_H_
#
#
#ifndef STATIC_POOL_CONTAINER_COLUMNS
#	define STATIC_POOL_CONTAINER_COLUMNS 256 // maximum number of bytes per one array
#endif /* STATIC_POOL_CONTAINER_COLUMNS */
#
#ifndef STATIC_POOL_CONTAINER_RAWS
#	define STATIC_POOL_CONTAINER_RAWS 16 // maximum number of arrays
#endif /* STATIC_POOL_CONTAINER_RAWS */
#
#
#define POOL_C_IS_EMPTY(handle)\
    (handle)->rdEmpty
#define POOL_C_IS_FULL(handle)\
    (handle)->wrFull
#
#
#
#
#
#
#/* USER DEFINES CHECK ----------------------------------------------------------------------------------------------------------------------------------------
# */
#if ((STATIC_POOL_CONTAINER_COLUMNS - 1) & STATIC_POOL_CONTAINER_COLUMNS) || (STATIC_POOL_CONTAINER_COLUMNS == 0) || (STATIC_POOL_CONTAINER_COLUMNS > 16384) // columns buffer is power of 2 size check
#	error STATIC_Pool_CONTAINER_COLUMNS must be is power of 2
#endif //check if power of 2 STATIC_POOL_CONTAINER_COLUMNS
#
#if ((STATIC_POOL_CONTAINER_RAWS - 1) & STATIC_POOL_CONTAINER_RAWS) || (STATIC_POOL_CONTAINER_RAWS == 0) || (STATIC_POOL_CONTAINER_RAWS > 16384) // raws buffer is power of 2 size check
#	error STATIC_Pool_CONTAINER_RAWS must be is power of 2
#endif //check if power of 2 STATIC_POOL_CONTAINER_RAWS
#
#
#if (STATIC_POOL_CONTAINER_COLUMNS > 16384) // columns buffer if too long check
#	error STATIC_POOL_CONTAINER_COLUMNS is too long
#endif //check STATIC_POOL_CONTAINER_COLUMNS
#
#if (STATIC_POOL_CONTAINER_RAWS > 16384) // raws buffer if too long check
#	error STATIC_POOL_CONTAINER_RAWS is too long
#endif //check STATIC_POOL_CONTAINER_RAWS
#
#
#
#endif /* _POOL_CONTAINER_HEAD_H_ */
