#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/*
 * encrypts a file from the command line by rotating it by n characters.
 * DEFAULT=128
 */
int main(int argc, char *argv[]) {
    int input;
    int modifier = (UCHAR_MAX + 1) / 2;
    if (argc == 2)
	modifier = atoi(argv[1]);
    while ((input = getchar()) != EOF) {
	putchar(input + modifier);
    }
}
