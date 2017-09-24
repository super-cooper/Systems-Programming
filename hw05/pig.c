#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER 2048
#define LONGEST_WORD 200

int isvowel(char c) {
    switch (tolower(c)) {
	case 'a': return 1;
	case 'e': return 1;
	case 'i': return 1;
	case 'o': return 1;
	case 'u': return 1;
	default: return 0;
    }
}

char *pig(char word[LONGEST_WORD], int len) {
    //if starts with vowel...
    if (isvowel((char) word[0])) {
	strcat(word, "yay\0"); //append yay
	return word;
    }

    //for retaining capilalization
    int cap = isupper(word[0]) ? 1 : 0;
    //for keeping track of "qu"
    int q = 0;
    //keep track of evyerthing before the first vowel
    char beginning[LONGEST_WORD];

    //else, if it starts with a consonant
    int i = 0; //we'll need this later
    for (; i < len; i++) { 
	word[i] = tolower(word[i]);
	//check for first vowel
	//y is treated as a consonant if leading, a vowel otherwise
	if (!isvowel((char) word[i]) || (q && word[i] == 'u')) {
	    if (word[i] == 'y') {
		beginning[i] = '\0';
		break;
	    }
	    beginning[i] = word[i]; 
	    q = word[i] == 'q' ? 1 : 0;
	} else {
	    beginning[i] = '\0';
	    break;
	}
    }
	//first occurence of vowel found
	strcat(word, beginning);
	//printf("%s", out2);
	strcat(word, "ay\0");

	//retain capitalization
	if (cap)
	    word[i] = toupper(word[i]); 
    return word + (sizeof(char) * i);
}

int main(int argc, char **argv) {
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
	    //convert to piglatin and add to out
	    strcpy(word, pig(word, word_index));
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
