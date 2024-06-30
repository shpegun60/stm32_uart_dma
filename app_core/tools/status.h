/*************************************************************************//**
 * @file
 * @brief       This file is part of the AFBR-S50 API.
 * @details     Provides status codes for the AFBR-S50 API.
 *
 * @copyright
 *
 * Copyright (c) 2023, Broadcom Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef ARGUS_STATUS_H
#define ARGUS_STATUS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*!***************************************************************************
 * @defgroup    argus_status Status Codes
 * @ingroup     argus
 *
 * @brief       Status and Error Code Definitions
 *
 * @details     Defines status and error codes for function return values.
 *              Basic status number structure:
 *              - 0 is OK or no error.
 *              - negative values determine errors.
 *              - positive values determine warnings or status information.
 *              .
 *
 * @addtogroup  argus_status
 * @{
 *****************************************************************************/

/*!***************************************************************************
 * @brief   Type used for all status and error return values.
 * @details Basic status number structure:
 *           - 0 is OK or no error.
 *           - negative values determine errors.
 *           - positive values determine warnings or status information.
 *           .
 *****************************************************************************/
typedef int32_t status_t;

/*! API status and error return codes. */
enum Status
{
    /**********************************************************************************************
     ********** Generic Status ********************************************************************
     *********************************************************************************************/

    /*! 0: Status for success/no error. */
    STATUS_OK = 0,

    /*! 0: Status for device/module/hardware idle. Implies #STATUS_OK. */
    STATUS_IDLE = 0,

    /*! 1: Status to be ignored. */
    STATUS_IGNORE = 1,

    /*! 2: Status for device/module/hardware busy. */
    STATUS_BUSY = 2,

    /*! 3: Status for device/module/hardware is currently initializing. */
    STATUS_INITIALIZING = 3,

    /*! -1: Error for generic fail/error. */
    ERROR_FAIL = -1,

    /*! -2: Error for process aborted by user/external. */
    ERROR_ABORTED = -2,

    /*! -3: Error for invalid read only operations. */
    ERROR_READ_ONLY = -3,

    /*! -4: Error for out of range parameters. */
    ERROR_OUT_OF_RANGE = -4,

    /*! -5: Error for invalid argument passed to an function. */
    ERROR_INVALID_ARGUMENT = -5,

    /*! -6: Error for timeout occurred. */
    ERROR_TIMEOUT = -6,

    /*! -7: Error for not initialized modules. */
    ERROR_NOT_INITIALIZED = -7,

    /*! -8: Error for not supported. */
    ERROR_NOT_SUPPORTED = -8,

    /*! -9: Error for yet not implemented functions. */
    ERROR_NOT_IMPLEMENTED = -9,
};

/*! @} */
#ifdef __cplusplus
} // extern "C"
#endif
#endif /* ARGUS_STATUS_H */
