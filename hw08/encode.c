/*
 * Compresses files using the Huffman Tree Method
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "hashtable.h"

#define BYTE 8
#define ASCII 256
#define MB 1024 * 1024 + 1

typedef struct node {
    char value;
    int freq;
    struct node *next;
    struct node *prev;
    struct node *left;
    struct node *right;
    char *path;
} node;

// sentinels
node *head;
node *tail;

// Huffman Tree root
node *root;


/*
 * Function used for sorting the linked list of chars with qsort
 */
int cmpcounts(const void *a, const void *b) {
    // get char vals of params
    char c1 = *(char *) a;
    char c2 = *(char *) b;
    // compare the counts of each char
    return ht_get(c1) - ht_get(c2);
}


/*
 * Formats a character for visual appeal
 */
char *fmtchar(char *c) {
    if (*c == EOF)
	return "EOF";
    else if (*c == '\n')
	return "\\n";
    else {
	char *dest = malloc(2);
	strncpy(dest, c, 1);
	dest[1] = '\0';
	return dest;
    }
}



/*
 * Appends a new node on to the end of the linked list and
 * returns the new node
 */
node *llappend(node *pos, char val, int freq) {
    node *new = malloc(sizeof(node));
    new->prev = pos;
    new->next = tail;
    pos->next = new;
    tail->prev = new;
    new->value = val;
    new->freq = freq;
    new->left = NULL;
    new->right = NULL;
    return new;
}


/*
 * Inserts a new new node before the given position in the linked list
 */
void lladd(node *new, node *pos) {
    new->next = pos;
    new->prev = pos->prev;
    pos->prev->next = new;
    pos->prev = new;
}



/*
 * Print the linked list in order
 */
void llprint() {
    node *itr = head->next;
    while (itr != tail) {
	if (itr->value == '\0') {
	    printf("(%d)->", itr->freq);
	} else {
	    char *fmt = fmtchar(&itr->value);
	    printf("%s(%d)->", fmt, itr->freq);
	}
	itr = itr->next;
    }
    printf("\n\n");
}



/*
 * Frees all memory from the linked list (and huffman tree)
 */
void treeclear(node *root) {
    if (root->left != NULL)
	treeclear(root->left);
    if (root->right != NULL)
	treeclear(root->right);
    if (root->path != NULL)
	free(root->path);
    free(root);
}




/*
 * Prints a pre-order traversal of the tree
 */
void printpreorder(node *root) {
    char *path = ht_getpath(root->value);
    if (path != NULL) {
	char *fmt = fmtchar(&root->value);
	printf("%s: %s\n", path, fmt);
    }
    if (root->left != NULL)
	printpreorder(root->left);
    if (root->right != NULL)
	printpreorder(root->right);
}



/*
 * Because for some ungodly reason just ht_put(EOF, 1) 
 * before the linked list construction doesn't work
 */
void addeof(void) {
    node *itr = head->next;
    node *eof = malloc(sizeof(node));
    eof->value = EOF;
    eof->freq = 1;
    eof->left = NULL;
    eof->right = NULL;
    while (itr->freq <= eof->freq && itr->next != tail)
	itr = itr->next;
    lladd(eof, itr);
}



/*
 * Tells if a node is a leaf or not
 */
int isleaf(node *v) {
    return v->left == NULL && v->right == NULL;
}



/*
 * Builds paths for the leaves using preorder traversals
 */
void buildpreorder(node *root, char *path, char *prev_dir) {
    char *cppath = malloc(BYTE);
    strcpy(cppath, path);
    // create path for this node
    if (root->path == NULL) {
	strcat(cppath, prev_dir);
	char *new = malloc(BYTE);
	strcpy(new, cppath);
	root->path = new;
    }
    
    strcpy(cppath, root->path);

    if (isleaf(root)) {
	char *new = malloc(BYTE);
	strcpy(new, cppath);
	ht_setpath(root->value, new);
    }

    if (root->left != NULL)
	buildpreorder(root->left, cppath, "0");
    if (root->right != NULL)
	buildpreorder(root->right, cppath, "1");
    free(cppath);
}



/**
 * Builds huffman paths and inserts them into the hash table
 */
void buildpaths(node *root) {
    char *path = malloc(BYTE);
    memset(path, '\0', BYTE);
    if (root->left != NULL)
	buildpreorder(root->left, path, "0");
    memset(path, '\0', BYTE);
    if(root->right != NULL)
	buildpreorder(root->right, path, "1");
    free(path);
}



/**
 * Writes to the file in its compressed representation
 */
void compress(char *buffer, FILE *file) {
    printf("Remainder of file:\n");
    int i = 0;
    size_t j = 0;
    unsigned char mask = 128;
    char out = 0;
    // for every char in the buffer
    while (buffer[i] != '\0') {
	j = 0;
	char c = buffer[i++];
	char *path = ht_getpath(c);
	size_t len = strlen(path);
	// for every bit in the path
	while (j < len) {
	    if ((int) mask < 1) {
		// we've made a byte
		fputc(out, file);
		out = 0;
		mask = 128;
	    }

	    if (path[j] == '1') {
		out += mask;
		printf("1");
	    } else 
		printf("0");
	    mask >>= 1;
	    j++;
	}
	printf(" ");
    }
    printf("\n");
}




/*
 * Writes the compressed file (FINALLY)
 */
void writefile(char *buffer, char *chars, char *fname) {
    printf("Tree:\n");
    strcat(fname, ".huff");
    FILE *file = fopen(fname, "w");
    int i = 0;

    // add the tree to the file
    while (chars[i] != '\0' && ht_contains(chars[i])) {
	fputs(ht_getpath(chars[i]), file);
	printf("%s ", ht_getpath(chars[i]));
	fputs(" ", file);
	fputc(chars[i], file);
	char *fmt = fmtchar(&chars[i]);
	printf("%s ", fmt);
	fputs(" ", file);
	i++;
    }
    fputs("\n", file);
    printf("\n\n");

    compress(buffer, file);
    fclose(file);
}
    


/*
 * Main function, compresses a file using the Huffman Tree method
 */
int main(int argc, char **argv) {

    // the file to read
    char *fname = malloc(64);
    // file descriptor
    int fd;
    // list of unique chars in the file
    char chars[ASCII];
    int in = 0;
    
    if (argc >= 2) {
	free(fname);
	fname = argv[1];
	fd = open(fname, O_RDONLY);
    } else {
	// when reading from stdin
	fd = STDIN_FILENO;
	strcpy(fname, "STDIN");
	in = 1;
    }

    char buffer[MB];
    // also indicates position of EOF
    int bytes_read;

    bytes_read = read(fd, buffer, MB);

    if (close(fd) != 0) {
	printf("Trouble reading file");
	exit(1);
    }

    printf("INPUT: \n%s\n\n", fname);
    
    // keeps track of how many unique chars there are in the buffer
    int char_types = 0;

    // build counts for all chars in the buffer, represented as a hash table
    for (int i = 0; i < bytes_read; i++) {
	if (!ht_contains(buffer[i])) {
	    // initializes the char in the hash table
	    ht_put(buffer[i], 0);
	    // records existence of the char 
	    chars[char_types++] = buffer[i];
	}

	// increment the char's count by 1
	ht_put(buffer[i], ht_get(buffer[i]) + 1);
    }


    printf("Frequency Counts: \n");
    for (int i = 0; i < char_types; i++) {
	char *fmt = fmtchar(&chars[i]);
	printf("%8d %8s %8d\n",(int) chars[i], fmt, ht_get(chars[i]));
    } printf("\n");


    // build sorted linked list of chars

    // first sort the list of unique chars by their counts
    qsort(chars, char_types, sizeof(char), cmpcounts);

    // init sentinels
    head = malloc(sizeof(node));
    head->next = tail;
    tail = malloc(sizeof(node));
    tail->prev = head;

    node *iterator = head;

    // then just add them linearly to the linked list :)
    for (int i = 0; i < char_types; i++) {
	iterator = llappend(iterator, chars[i], ht_get(chars[i]));
    }

    addeof();
    char_types++;

    llprint();

    // we don't need the hash table anymore
    // ht_clear();

    // build a tree using Huffman's algorithm
    int llnodecount = char_types;

    // debug
    //llprint();
    //printf("Size: %d\n", llnodecount);

    while (llnodecount > 1) {
	iterator = head->next;
	// 2 smallest nodes in the linked list
	node *node1 = iterator;
	node *node2 = iterator->next;

	// their combined frequencies
	int combined_freq = node1->freq + node2->freq;

	// creating a new parent 
	// with the above nodes as its children
	// and its frequency being theirs combined
	node *parent = malloc(sizeof(node));
	parent->freq = combined_freq;
	parent->left = node1;
	parent->right = node2;

	// move new node to proper position on the linked list
	node *locator = node2;
	while (locator != tail && !(locator->freq >= parent->freq))
		locator = locator->next;

	lladd(parent, locator);

	node2->next->prev = head;
	head->next = node2->next;

	// remove these from the linked list
	node1->prev = node1->next = node2->prev = node2->next = NULL;
	llnodecount--;

	if (llnodecount == 1)
	    root = parent;

	// debugging
	//llprint();
	//printf("Size: %d\n", llnodecount);
    }

    buildpaths(root);

    printf("EOF: %s\n\n", ht_getpath(EOF));
    
    writefile(buffer, chars, fname);

    ht_clear();
    treeclear(root);
    free(head);
    free(tail);

    if (in)
	free(fname);
}
