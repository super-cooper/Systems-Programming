#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pig.h"

int isvowel(char c) {
    switch(tolower(c)) {
	case 'a': return 1;
	case 'e': return 1;
	case 'i': return 1;
	case 'o': return 1;
	case 'u': return 1;
	case 'y': return 1;
	default: return 0;
    }
}


char *unpig(char word[LONGEST_WORD], int len) {
    //cut off "ay"
    len -= 2;
    word[len] = '\0'; 
    //for words that start with vowels in English
    if (tolower(word[len - 1]) == 'y') {
       word[len - 1] = '\0';
       return word;
    }

    //for retaining capitalization
    int cap = isupper(word[0]) ? 1 : 0;
    //for keeping track of "qu"
    int u = 0;
    //for moving over
    char out[LONGEST_WORD];
    //starting consonants
    char beginning[2];
    //gather last two consonants, or 1 if the penultimate char in word is a vowel
    int i = 0;
    for (; i < 2; i++) {
	if (!isvowel(tolower(word[len - (i + 1)]))) {
	   beginning[i] = tolower(word[len - (i + 1)]);
	} else break;
    }

    //cut off gathered characters
    word[len - i] = '\0';

    //add gathered characters to beginning
    out[0] = '\0';
    for (int j = 2 - i; j >= 0; j--)
	out[2 - j] = beginning[j];
    //strcat(out, beginning);
    //add rest of modified word
    strcat(out, word + (sizeof(char) * (i - 1)));
    strcpy(out, word);
    return word;
}

int main(int argc, char **argv) {
    //sorry for repeat code
    for (int i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	    printf("Usage: %s < <inputfile>\n", argv[0]);
	    return -1;
	}
    }

    int input; 
    char out[BUFFER];
    char word[LONGEST_WORD];
    int word_index = 0;
    int out_index = 0;

    //for the whole file
    while ((input = getchar()) != EOF) {
	if (!isalpha(input) && word_index > 0) {
	    //add null terminator to word string
	    word[word_index] = '\0';
	    // U N P I G
	    strcpy(word, unpig(word, word_index));
	    for (int i = 0; word[i] != '\0'; i++) {
		out[out_index++] = word[i];
	    }
	    //reset word index to new word
	    word_index = 0;
	    //add space to output string
	    out[out_index++] = input;
	} else if (isalpha(input)) { //keep building word
	    word[word_index++] = input;
	} else {
	    //punctuation and newlines n stuff
	    out[out_index++] = input;
	}
    }
    printf("%s", out);
    return 0;
}
