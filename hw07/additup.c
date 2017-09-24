/*
 * adds up arbitrarily large numbers in base-10
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define LINE_SIZE 1024

/* node for linked list */
typedef struct node {
    int digit;
    struct node *next, *prev;
} node;



/* points the iterator to the end of the linked list */
node *gotoend(node *tail) {
    return tail->prev;
}



/* points the iterator to the beginning of the linked list */
node *gotofront(node *head) {
    return head->next;
}



/* prints the accumulator as big string of numbers */
void printnum(node *accumulator, node *tail, node *head) {
    accumulator = gotofront(head);
    printf("Total: ");
    while (accumulator != tail) {
        printf("%d", accumulator->digit);
        accumulator = accumulator->next;
    }
    printf("\n");
}


/* frees all nodes in the linked list */
void freeaccumulator(node *tail, node *head) {
    node *ptr = gotofront(head);
    while (ptr != tail) {
        free(ptr->prev);
        ptr = ptr->next;
    }
    free(tail->prev);
    free(tail);
}



/* Adds up arbitrarily large numbers from stdin */
int main() {
    // sum linked lists broh
    struct node *accumulator = malloc(sizeof(node));
    // sentinels
    struct node *head = malloc(sizeof(node));
    struct node *tail = malloc(sizeof(node));
    head->next = accumulator;
    tail->prev = accumulator;
    accumulator->digit = 0;

    //init sentinels
    accumulator->prev = head;
    accumulator->next = tail;

    // get input
    while (1) {
	// for getting a single line from stdin
	char *num = malloc(sizeof(char) * LINE_SIZE);
        // get line of input checking for EOF
        if(fgets(num, LINE_SIZE, stdin) == NULL) {
	    free(num);
	    break;
	}

        accumulator = gotoend(tail);

        int i = 0;
        int carry = 0;
	int leading = 1;
        // find the least-significant digit
        while (isdigit(num[i]) || (leading && isspace(num[i]))) {
	    if (isdigit(num[i]))
		leading = 0;
            i++;
	}
        // catch out of bounds issues
        if (i > 0)
            i--;

        // ADD EM UP COWBOY
	char current = num[i];
        while (isdigit(current)) {
            // if we need to add more links
            if (accumulator == head) {
                accumulator = malloc(sizeof(node));
                accumulator->prev = head;
                accumulator->next = head->next;
                accumulator->next->prev = accumulator;
                head->next = accumulator;
		accumulator->digit = 0;
            }
            // a value idk how to explain it in mathy ways
            int x = accumulator->digit + atoi(&current) + carry;
            carry = 0;
            if (x > 9) {
                carry = 1;
                x -= 10;
            }
            accumulator->digit = x;
            // move on to the next digit
            accumulator = accumulator->prev;
            i--;
	    if ((i < 0 || !isdigit(num[i])) && carry)
		current = '0';
	    else {
		if (i >= 0)
		    current = num[i];
		else
		    current = '\0';
	    }
        }
	// undo last shift
	accumulator = accumulator->next;
        printnum(accumulator, tail, head);
	free(num);
    }
    freeaccumulator(tail, head);
}
