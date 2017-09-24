#include <stdio.h>
#include "getnum.h"
#include <stdlib.h>

extern int error;

/*
 * Takes numbers in formats of base 2, 8, 10, 16, and prints them as 
 * octals
 */
int main() {
    int neg = 0;
    long x = getnum();
    neg = x < 0 ? 1 : 0;
    x = x < 0 ? -x : x;
    if (error) {
	printf("ERROR\n");
	exit(1);
    }
    if (neg)
	printf("-");
    printf("0");
    //Influenced by algorithm found on c4learn
    long remainders[32], i = 0, len = 0;
    while (x) {                    //find octal digits
	remainders[i++] = x % 8;   //store them backwards
	x /= 8;
	len++;
    }
    for (i = len - 1; i >= 0; i--) //print them in order
	printf("%ld", remainders[i]);
    printf("\n");
}
