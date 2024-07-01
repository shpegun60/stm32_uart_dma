#ifndef ASSERT_SAFE_INVOKE_H
#define ASSERT_SAFE_INVOKE_H

#define AE_SAFE_FOO(foo) ( (__typeof(foo))(null_func) )
#define AE_SAFE_FUNC_PTR(type, name) type name = ( (type)(null_func) )

// declaration of default function
void null_func(void);

#endif /* ASSERT_SAFE_INVOKE_H */


/*
 ******************************************************************************
 * EXAMPLE
 ******************************************************************************


#include <stdio.h>
typedef double (* calc_foo)(int a, int b, int c, int d, int e, int i);
int main() {
    AE_SAFE_FUNC_PTR(calc_foo, foo);

    int sum = foo(2, 3, 4, 5, 6, 7); // not work, programm is exit with error 3
    printf("SUM= %d", sum);
}

Output: -->

Program returned: 3
Program stdout
Error: Invalid function invocation

*/
