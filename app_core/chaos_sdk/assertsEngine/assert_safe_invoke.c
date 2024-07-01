#include "assert_safe_invoke.h"
#include <stdio.h>
#include <stdlib.h>

#define AE_EXIT_HANDLER(error, txt) exit(error)

void null_func(void)
{
    char err_txt[] = "Error: Invalid function invocation\t\n";
    printf("%s", err_txt);
    AE_EXIT_HANDLER(3, err_txt);
}
