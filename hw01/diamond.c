#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/*
 * prints a diamond made out of '*' and ' ' characters in n rows.
 * can be inverted if given a negative amount of rows.
 */

int getdigit();

/*
 * prints the outside of the diamond. Relegated to a function because
 * the outside needs to be reprinted in case of reverse
 */
void printoutside(int desiredrows, int currentrow, bool reversed) {
    for (int s = 0; s < desiredrows - currentrow; s++) { //spaces | stars
	if (!reversed)
	    printf(" ");
	else
	    printf("*");
    }
}

/*
 * The meat and taters. Organizes data and then prints a diamond.
 */
int main() {
    printf("I will print a diamond for you. Enter a size between 1-9: ");

    int n = getdigit();
    bool nReached = false; //tells if we reached the midpoint of the diamond
    bool reversed = false;

    if (n < 0) {
	reversed = true;
	n *= -1;
    }

    for (int i = 0; i < n && i >= 0; i++) { //for each line...
	printoutside(n, i, reversed);

	//print inside
	for (int j = 0; j < 1 + (2 * i); j++) { //stars | spaces
	    if (!reversed)
		printf("*");
	    else
		printf(" ");
	}

	if (reversed) //print the second half of the border for reverse cases
	    printoutside(n, i, reversed);

	if (i == n - 1) //if inside is done printing...
	    nReached = true;
	if (nReached) 
	    i -= 2; //effectively reverses for-loop (i--)
	printf("\n"); //start new line
    }
}

/*
 * Gets a single digit from the command line using getchar(). Ignores any non-numbers
 * except '-'. Note that using an even number of '-' characters before the first digit
 * will result in double negation and the number will be positive.
 */
int getdigit() {
    int digit = getchar() - '0';

    if (digit <= 9 && digit >= 0) 
	return digit;
    else if (digit == EOF)
	return -1;
    else if (digit + '0' == '-')
	return getdigit() * -1;
    else 
	return getdigit();
}
