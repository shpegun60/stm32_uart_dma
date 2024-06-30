/*************************************************************************//**
 * @file
 * @brief       This file is part of the AFBR-S50 API.
 * @details     This file provides debug functionality.
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

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "debug.h"
#include "main.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

extern uint32_t _Min_Stack_Size;
extern uint32_t _estack;

#define STACK_TOP   (uint32_t*)&_estack
#define STACK_BASE  (uint32_t *)((uint32_t)&_estack - (uint32_t)&_Min_Stack_Size)

/*! The pattern to be filled to unused stack space. */
#define STACK_PATTERN 0xAAAAAAAAU

uint32_t Debug_GetStackUsage(void)
{
    uint32_t size = 0;
    const uint32_t* const top = STACK_TOP;
    const uint32_t* const base = STACK_BASE;

    for (const uint32_t* p = base; p < top; p++) {
        if (*p != STACK_PATTERN) {
            size = (uint32_t)top - (uint32_t)p;
            break;
        }
    }

    /* reset for next readout */
    Debug_ResetStackUsage();

    return size;
}

void Debug_ResetStackUsage(void)
{
    /* Reset the unused stack memory in RAM for debugging purposes.
     * The memory from top of the stack until the current stack pointer
     *  is cleared to a 0xAAA... pattern. */

    uint32_t * sp = 0;
    __asm__ __volatile__ ("mov %0, sp" : "=r"(sp));
    const uint32_t* const base = STACK_BASE;

    while (--sp >= base) {
        *sp = STACK_PATTERN;
    }
}
