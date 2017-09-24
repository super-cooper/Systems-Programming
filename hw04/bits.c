#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Prints binary representation of a char (ch)
 */
void print_bits(int ch) {
    char mask = 0x01;
    char out[CHAR_BIT];

    for (int i = 0; i < CHAR_BIT; i++) { //for each bit in ch...
	char result = ch & mask;
	if (result > 0) //if the bit is a 1 at position i, 
	    out[i] = '1';
	else 
	    out[i] = '0'; 
	mask <<= 1;
    }

    for (int i = CHAR_BIT - 1; i >= 0; i--) { //for each bit in output string,
	printf("%c", out[i]);
    }
}

/*
 * Turns binary into a char and prints the resulting char
 */
void decode_bits(int ch) {
    static char buffer = 0x00; //output buffer
    static int calls = 0; //how many calls to this function
    unsigned char res = ((char) UCHAR_MAX); //opposite mask from encode
    res <<= (CHAR_BIT - 1); //10000000
    if (isspace(ch)) //skip spaces
	return;
    if (ch == '1') 
	buffer += res >> calls;
    if (++calls == CHAR_BIT) { //if we've built a full char...
	printf("%c", buffer); //print it
	printf("\n");
	exit(0);
    }
}
