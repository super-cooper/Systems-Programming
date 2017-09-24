#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define MB 1024 * 1024

typedef struct node {
    char value;
    struct node *left;
    struct node *right;
} node;

// root of the huffman tree
node *root;



/**
 * Prints a help message
 */
void printusagehelp(void) {
    printf("Usage: ./decode <file>\n");
}



/*
 * Frees all the tree's memory using a post-order
 * traversal
 */
void freetree(node *root) {
    if (root->left != NULL)
	freetree(root->left);
    if (root->right != NULL)
	freetree(root->right);
    free(root);
}




int isleaf(node *ptr) {
    return ptr->left == NULL && ptr->right == NULL;
}



/*
 * Main function, decodes a compressed file
 */
int main(int argc, char **argv) {
    // check that a file was passed
    if (argc < 2) {
	printusagehelp();
	exit(1);
    }

    char *fname = argv[1];
    // file descriptor
    int fd;

    fd = open(fname, O_RDONLY);

    // buffer for storing file in memory
    char buffer[MB];

    int bytes_read = read(fd, buffer, MB);

    if (close(fd) != 0) {
	printf("Trouble reading file");
	exit(1);
    }

    root = malloc(sizeof(node));
    root->left = NULL;
    root->right = NULL;

    node *tree_ptr = root;
    int get_path = 1;
    int get_char = 0;
    char prev = '\0';
    unsigned int mask = 128;
    int i = 0;
    // parse that file
    for (; i < bytes_read; i++) {
	char c = buffer[i];

	if ((get_path || get_char) && c == ' ' && prev != ' ') {
	    get_path = !get_path;
	    get_char = !get_char;
	    prev = ' ';
	    continue;
	}

	// if done parsing tree
	if (c == '\n' && (get_path || buffer[i - 2] == ' ')) {
	    get_path = 0;
	    get_char = 0;
	    i++;
	    break;
	}

	// build tree
	if (get_path && c != '\n') {
	    if (c == '1') {
		// move right

		if (tree_ptr->right == NULL) {
		    tree_ptr->right = malloc(sizeof(node));
		    tree_ptr->right->right = NULL;
		    tree_ptr->right->left = NULL;
		}

		tree_ptr = tree_ptr->right;
	    } else if (c == '0') {
		// move left

		if (tree_ptr->left == NULL) {
		    tree_ptr->left = malloc(sizeof(node));
		    tree_ptr->left->right = NULL;
		    tree_ptr->left->left = NULL;
		}

		tree_ptr = tree_ptr->left;
	    } else {
		printf("Improperly formatted tree.\n");
		exit(1);
	    }
	} else if (get_char) {
	    if (prev != ' ') {
		printf("Improperly formatted tree.\n");
		exit(0);
	    }

	    tree_ptr->value = c;
	    tree_ptr = root;
	} else {
	    i++;
	    break;
	}
	prev = c;
    }

    // follow paths in file
    for (; i < bytes_read; i++) {
    char c = buffer[i];
    while (mask > 0) {
	if (isleaf(tree_ptr)) {
	    printf("%c", tree_ptr->value);
	    tree_ptr = root;
	}
	
	if ((c & mask) > 0)
	    tree_ptr = tree_ptr->right;
	else 
	    tree_ptr = tree_ptr->left;
	mask >>= 1;
    }
    mask = 128;
}

freetree(root);

printf("\n");

}
