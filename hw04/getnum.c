#include <stdio.h>
#include <limits.h>
#include "ctype.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

//sorry
#define LONG_LEN 32
#define True 1
#define False 0
#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16
#define A 65
#define a 97
#define B 66
#define b 98
#define C 67
#define c 99
#define D 68
#define d 100
#define E 69
#define e 101
#define F 70
#define f 102

unsigned int calls = 0;
int error = False;

/*
 * Notifies the user of formatting errors
 * Sorry for the ugliness; we were already late and didn't wanna
 * refactor everything
 */
void doerror(int type) {
    char typestr[12];
    switch (type) {
	case BIN:
	    strcpy(typestr,"binary");
	    break;
	case OCT:
	    strcpy(typestr, "octal");
	    break;
	case DEC:
	    strcpy(typestr, "decimal");
	    break;
	case HEX:
	    strcpy(typestr, "hexadecimal");
	    break;
	default:
	    strcpy(typestr, "N/A");
	    break;
    }
    error = True;
    calls = 0;
}

/*
 * Initialize all values in an array to 0
 */
void initarray(int arr[], int len) {
    for (int i = 0; i < len; i++) 
	arr[i] = 0;
}

/*
 * Returns the value of a number in base 10
 */
long tovalue(int arr[], int base) {
    int value = 0;
    int n = 0;
    for (unsigned int i = LONG_LEN - calls; i < LONG_LEN; i++)
	value += arr[i] * pow(base, n++); //self-explanatory
    calls = 0;
    return value;
}

/*
 * converts hex digits
 */
int convert(int num) {
    if (num >= 0 && num <= 9)
	return num;
    else {//I beg for your forgiveness
	num += 48; //unreliable :(
	switch(num) { //we're tired
	    case A:
	    case a:
		return 10;
	    case B:
	    case b:
		return 11;
	    case C:
	    case c:
		return 12;
	    case D: 
	    case d:
		return 13;
	    case E: 
	    case e:
		return 14;
	    case F: 
	    case f:
		return 15;
	    default:
		return -1;
	}
    }
}


/*
 * Converts a char to a value whose place
 * is decided by how many times the function has been called
 * in the current runtime and put into an array based on its
 * base type
 */
long buildarray(int num, int type) {
    static int arr[LONG_LEN];
    if (num == EOF) //if this number is done
	return tovalue(arr, type);
    num = convert(num);
    if (num == -1) {
	doerror(type);
	return 0;
    }
    if (calls == 0)
	initarray(arr, LONG_LEN);
    arr[LONG_LEN - ++calls] = num;
    if (calls == LONG_LEN) //if array full
	calls = 0;
    return 0;
}

/*
 * Takes an input string in the form of binary (0b), octal (0), 
 * decimal, or hexadecimal (0x) and returns the decimal value of 
 * the given input
 */
long getnum(void) {
    int leading = True;
    int neg = False;
    int type;
    int input;
    int firstchar;
    long buffer = 0L;
    while ((input = getchar()) != EOF && input != '\n') {
	input -= 48;
	if (error) { //continue to next whitespace
	    if (isspace(input + 48)) {
		continue;
	    } else
		error = False;
	}

	if (isspace(input + 48)) {
	    if (leading) //skip leading whitespace
		continue;
	    else {
		break;
	    }
	}

	if (leading) { //check for leading char
	    firstchar = input;
	    leading = False;
	    if ((firstchar == '-' - 48) && !neg) { //handle negative
		neg = True;
		leading = True;
		continue;
	    } else if (isdigit(firstchar + 48)) { //valid beginning
		if (firstchar != 0) { //starts with 1-9
		    type = DEC;
		}
		else
		    continue; //starts with 0
	    }
	} //end check for leading digit

	if ('\0' == type) { //checking for base
	    if (input == 'b' - 48) {
		type = BIN;
		continue;
	    } else if (input == 'x' - 48) {
		type = HEX;
		continue;
	    } else if (input <= 7 || input >= 1) {
		type = OCT;
	    } else if (input == '\n') { //single 0 input :)
		return 0;
	    } else { //invalid base
		doerror('\0');

	    }
	}
	buffer += buildarray(input, type);
    }
    buffer += buildarray(EOF, type);
    return neg ? -buffer : buffer;
}
