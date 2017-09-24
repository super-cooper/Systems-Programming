#include <stdio.h>
#include "getnum.h"
#include "bits.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#define STDIN 0

extern int error;

/*
 * Takes numbers in formats of base 2, 8, 10, 16, and prints them
 * as binary
 */
int main() {	    
    int neg = 0;
    long x = getnum();                      //to base 10
    neg = x < 0 ? 1 : 0;                    //find out if negative
    x = neg ? -x : x;                       //absolute value
    if (error) {                            //if improperly formatted
	printf("ERROR\n");
	exit(1);                           //move to next arg
    } else 
    if (neg)
	printf("-");
    printf("0b");
    //print the bits of the string
    while (x) {
	if (x & 1)
	    printf("1");
	else 
	    printf("0");
	x >>= 1;
    }
    printf("\n");
} 
