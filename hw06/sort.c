/*
 * sorts strings lad
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define DEPTH 1024

struct pair {
    long lead;
    char *end;
};

//flags
int ignorecase = 0;
int numbers = 0;
int reversed = 0;

/*
 * prints a help statement, plus any invalid
 * args if given
 */
void printhelp(char* arg, int help) {
    printf("Usage: ./sort [args] < <inputfile>\n");
    if (!help)
	printf("Unknown flag: %s\n", arg);
}



/*
 * Intermediary because C sucks
 */
int mystrcmp(const void *s1, const void *s2) {
    return strcmp(* (char * const *) s1, * (char * const *) s2);
}



/*
 * Intermediary because we hate our lives
 */
int mystrcasecmp(const void *s1, const void *s2) {
    return strcasecmp(* (char * const *) s1, * (char * const *) s2);
}



/*
 * converts a string into a long
 * returns 0 if not a number
 */
long mystrtol(char *start, char **rest) {
    //index in string
    int i = 0;
    //total to return
    long val = 0;
    //is negative?
    int neg = 0;
    
    //skip leading whitespace
    while (isspace(start[i])) 
	i++;
    if (start[i] == '-') {
	neg = 1;
	i++;
    }
    
    val = atol(&start[i]);

    while (isdigit(start[i]))
	i++;

    *rest = &start[i];

    return neg ? -val : val;
}



/*
 * compare long/string pairs by their leading number
 */
int cmppairs(const void *p1, const void *p2) {
    struct pair *newp1 = (struct pair *) p1;
    struct pair *newp2 = (struct pair *) p2;
    if (newp1->lead < newp2->lead) 
	return 1;
    else if (newp1->lead > newp2->lead)
	return -1;
    else
	return 0;
}



/*
 * SORT STRANGS
 */
int main(int argc, char **argv) {
    //check flags
    for (int i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	    if (argv[i][1] == 'f') 
		ignorecase = 1;
	    else if (argv[i][1] == 'n')
		numbers = 1;
	    else if (argv[i][1] == 'r') 
		reversed = 1;
	    else {
		if (argv[i][1] == 'h' || argv[i][1] == '?')
		    printhelp(argv[i], 1);
		else 
		    printhelp(argv[i], 0);
		exit(1);
	    }
	}
    }

    //matrix for storing input strings
    char *strarray[DEPTH];
    int input;
    //str.length
    int charcount = 0;
    //line index
    int i = 0;
    //string for each line
    char *line = malloc(DEPTH);

    while ((input = getchar()) != EOF) {
	//if line too long
	if (++charcount >= DEPTH) {
	    //get to end of line
	    if (input != '\n')
		continue;
	}

	//if line finished...
	if (input == '\n') {
	    line[charcount - 1] = '\0';
	    //store the line in the array
	    strarray[i] = malloc(sizeof(char) * DEPTH);
	    strcpy(strarray[i++], line);
	    //printf("%s\n", line);
	    charcount = 0;
	} else {
	    line[charcount - 1] = (char) input;
	}
    }
    free(line);

    if (ignorecase) 
	qsort(strarray, i, sizeof(char *), mystrcasecmp);

    else if (numbers) {
	//GOTEEEEEEEEEEEEEEEEM
	struct pair *deez[i];
	for (int j = 0; j < i; j++) {
	    struct pair *almonds = malloc(sizeof(struct pair));
	    char **after = malloc(sizeof(char *));
	    almonds->lead = mystrtol(strarray[j], after);
	    almonds->end = *after;
	    deez[j] = almonds;
	    free(after);
	    free(almonds);
	}

	qsort(deez, i, sizeof(struct pair *), cmppairs);
	if (reversed) 
	    for (int j = i - 1; j >= 0; j--)
	       printf("%ld%s\n", deez[j]->lead, deez[j]->end);
	else	
	    for (int j = 0; j < i; j++) {
		printf("%ld%s\n", deez[j]->lead, deez[j]->end);
	    }
	free(*deez);
	for (int j = 0; j < i; j++)
	    free(strarray[j]);
	exit(0);
    } else 
	qsort(strarray, i, sizeof(char*), mystrcmp);

    //print all the strings
    if (reversed)
	for (int j = i - 1; j >= 0; j--) {
	    printf("%s\n", strarray[j]);
	    free(strarray[j]);
	}
    else
	for (int j = 0; j < i; j++) {
	    printf("%s\n", strarray[j]);
	    free(strarray[j]);
	}
}
