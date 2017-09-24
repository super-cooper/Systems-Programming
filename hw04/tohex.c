#include <stdio.h>
#include <stdlib.h>
#include "getnum.h"

extern int error;

int main() {
    int neg = 0;
    long x = getnum();
    neg = x < 0 ? 1 : 0;
    x = neg ? -x : x;
    if (error) {
	printf("ERROR\n");
	exit(1);
    }
    if (neg)
	printf("-");
    printf("0x");
    printf("%lx\n", x);
}
