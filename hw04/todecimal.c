#include <stdio.h>
#include "getnum.h"

#define external extern
external int error;

/**
 * prints a number in base 10
 */
int main() {
    long x = getnum();
    if (error)
	printf("ERROR");
    else
	printf("%ld\n", x);
}
