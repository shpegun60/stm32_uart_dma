## Example using 
```cpp
int can_comm_init(can_communicator_t* const self, CAN_HandleTypeDef * const hcan)
{
	M_Assert_BreakSaveCheck(self == NULL, M_EMPTY,
			return CTYPE_FALSE,
					"null");

	M_Assert_BreakSaveCheck(CallbackManager_init(&self->RX.callb) != 0, M_EMPTY,
			return CTYPE_FALSE,
					"callback manager not init");

	CallbackManager_subscribe(&self->RX.callb, 0x00, Ping, NULL);


	M_Assert_BreakSaveCheck(poolContainer_init(&self->TX.pool, 16, 256), M_EMPTY,
			return CTYPE_FALSE,
					"pool container not init");

	self->BID = 0x01;
	self->TX.remaining_bytes 	= 0;
	self->TX.ptr_align 			= 0;
	self->TX.chunks 			= 0;
	self->TX.chunk_number 		= 0;

	self->RX.msg_flags = 0;

	// initialization periphery
	M_Assert_BreakSaveCheck(bxCan_init(&self->can_handle, hcan, CAN_Filter_init) != 0, M_EMPTY,
			return CTYPE_FALSE,
					"can handler not init");
	return CTYPE_TRUE;
}

```
## for working with printf need (on stm32)  realize next function
```cpp
/*
 * stm32_printf.c
 *
 *  Created on: Mar 14, 2023
 *      Author: admin
 */
#include "main.h"
#include "stm32_printf.h"

extern UART_HandleTypeDef huart1;
#define UART_PRINTF	huart1

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&UART_PRINTF, (uint8_t *) ptr, len, HAL_MAX_DELAY);
	return len;
}

```
## for redusing resourse usage you need configure this library using these macros
```cpp
#
#/*
# * *****************************************************************************************************************************************************************************
# *  smart asserts settings
# * *****************************************************************************************************************************************************************************
# */
#
//#define NDEBUG                              	// uncommit this if program in release version
// =====> if NDEBUG is uncommited bellow defines have no meaning!!!!! <=====================================
//#define M_MESSAGE_ALWAYS_ENABLE         		1   // enabling runtime assert message filter and additon library information (see additional information macro)
//#define M_ASSERT_MSG_TEXT_DISABLE       		1   // disabling sending assert message text and do not saving this in .text section
//#define M_ASSERT_EXPR_DISABLE           		1   // disabling sending assert expression text, value and do not saving this in .text section
//#define M_ASSERT_FILE_LINE_TEXT_DISABLE 		1   // disabling sending assert file, line text and do not saving this in .text section
//#define M_ASSERT_FUNCTION_NAME_TEXT_DISABLE 	1	// disabling sending assert function-name text, value and do not saving this in .text section
//#define M_ASSERT_OPTIONAL_CHECKS_DISABLE 		1  	// disabling optional checks in user code

```
## Test out:
```cpp
Program stdout
Test case 15: Safe function call
Test case 16: Debug message without parameters
Test case 17: Debug message with one parameter: 0
Test case 19: Debug message to file: 0
Program stderr

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	RUNTIME ERROR Assert test: M_Assert_Break without parameters
Expression:	1, value: 1
Source:		/app/example.c, line: 540

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	RUNTIME ERROR Assert test: M_Assert_Break with parameter: 123
Expression:	1, value: 1
Source:		/app/example.c, line: 542

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	RUNTIME ERROR Assert test: M_Assert_Break with two parameters: 123, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 544

WARNING!!!
Function:		main
Assert failed:	RUNTIME WARNING Assert test: M_Assert_Warning without parameters
Expression:	1, value: 1
Source:		/app/example.c, line: 546

WARNING!!!
Function:		main
Assert failed:	RUNTIME WARNING Assert test: M_Assert_Warning with parameter: 2
Expression:	1, value: 1
Source:		/app/example.c, line: 548

WARNING!!!
Function:		main
Assert failed:	RUNTIME WARNING Assert test: M_Assert_Warning with two parameters: 2, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 550

WARNING!!!
Function:		main
Assert failed:	RUNTIME WARNING Assert test:M_Assert_Warning with two parameters: 3, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 552

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	RUNTIME ERROR Assert test: M_Assert_Break with two parameters: 4, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 554

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	RUNTIME ERROR Assert test: M_Assert_BreakSaveCheck with parameter: 6
Expression:	1, value: 1
Source:		/app/example.c, line: 558

WARNING!!!
Function:		main
Assert failed:	RUNTIME WARNING Assert test: M_Assert_WarningSaveCheck with parameter: 7
Expression:	1, value: 1
Source:		/app/example.c, line: 560

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	sfsagsdggdgsg
Expression:	if i == 0 not confirmed: passed to else, value: 0
Source:		/app/example.c, line: 563

WARNING!!!
Function:		main
Assert failed:	NO exists function
Expression:	i, value: 0
Source:		/app/example.c, line: 568

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Library Name:	hhhh
Assert failed:	With library: Custom assert message with two parameters: 1, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 570

WARNING!!!
Function:		main
Library Name:	hhhh
Assert failed:	With library: Custom warning message with two parameters: 2, 456
Expression:	1, value: 1
Source:		/app/example.c, line: 572

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	With error code: Assert with two parameters: 3, 123
Expression:	1, value: 1
Source:		/app/example.c, line: 574

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	Test case 1: Assertion failed without additional data
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 582

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	Test case 2: Assertion failed with one parameter: 0
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 584

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	Test case 3: Assertion failed with two parameters: 0, 123
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 586

WARNING!!!
Function:		main
Assert failed:	Test case 4: Warning without additional data
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 588

WARNING!!!
Function:		main
Assert failed:	Test case 5: Warning with one parameter: 0
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 590

WARNING!!!
Function:		main
Assert failed:	Test case 6: Warning with two parameters: 0, 123
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 592

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Library Name:	lib_name
Assert failed:	Test case 7: Assertion with library data
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 596

WARNING!!!
Function:		main
Library Name:	lib_name
Assert failed:	Test case 8: Warning with library data: lib_name
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 598

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	Test case 9: Assertion with enable code: 4207324
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 600

PROGRAMM EXIT WITH ERROR!!!
Function:		main
Assert failed:	Test case 11: M_Assert_BreakSaveCheck
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 604

WARNING!!!
Function:		main
Assert failed:	Test case 12: M_Assert_WarningSaveCheck
Expression:	test_value == 0, value: 1
Source:		/app/example.c, line: 606

WARNING!!!
Function:		main
Assert failed:	Test case 14: M_Assert_WarningElse
Expression:	if test_value != 0 not confirmed: passed to else, value: 0
Source:		/app/example.c, line: 610
Test case 18: Debug error message with two parameters: 0, error
```
