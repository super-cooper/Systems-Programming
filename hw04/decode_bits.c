#include <stdio.h>
#include "bits.h"

int main() {
    int input;
    while ((input = getchar()) != EOF) {
	decode_bits(input);
    } 
}
