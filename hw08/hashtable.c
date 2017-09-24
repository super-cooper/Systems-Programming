/**
 *  Simple, poorly-optimized implementation of a hash table 
 *  Inspired by tutorialspoint.com
 */

#include <stdlib.h>

#define SIZE 256

struct DataItem {
    int data;
    int key;
    char *path;
};

struct DataItem *hash_array[SIZE];
struct DataItem *dummy_item;

/*
 * get the has code for an item
 */
int hashcode(int key) {
    return key;
}

/*
 * Helper function for get below
 */
struct DataItem *get(int key) {
    int hash_index = hashcode(key);

    if (hash_array[hash_index] == NULL)
	return NULL;
    else
	return hash_array[hash_index];
}

/*
 * Get data from the hash table
 */
int ht_get(int key) {
    struct DataItem *item = get(key);
    if (item != NULL)
	return item->data;
    else
	return -1;
}

/*
 * Tells if the hash map contains a key
 */
int ht_contains(int key) {
    return get(key) != NULL;
}

/*
 * put an item into the hash table
 */
void ht_put(int key, int data) {
    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    item->data = data;
    item->key = key;

    int hash_index = hashcode(key);

    if (hash_array[hash_index] != NULL)
	free(hash_array[hash_index]);
    hash_array[hash_index] = item;
}

/*
 * remove an item from the hash table
 */
int ht_delete(int key) {
    struct DataItem *item = get(key);

    if (item == NULL)
	return -1;

    int hash_index = hashcode(key);

    //move through the bucket
    while (hash_array[hash_index] != NULL) {
	if (hash_array[hash_index]->key == key) {
	    struct DataItem *tmp = hash_array[hash_index];

	    //this is a global variable to avoid using free and screwing things up
	    hash_array[hash_index] = dummy_item;
	    return tmp->data;
	}

	//continue through bucket
	++hash_index;
	//wrap around the table
	hash_index %= SIZE;
    }
    return -1;
}

/*
 * Free all memory from the hash table
 */
void ht_clear(void) {
    for (int i = 0; i < SIZE; i++) {
	if (hash_array[i] != NULL) {
	    if (hash_array[i]->path != NULL)
		free(hash_array[i]->path);
	    free(hash_array[i]);
	}
    }
    free(dummy_item);
}

/*
 * Set the path mapping of a key
 */
void ht_setpath(int key, char *path) {
    struct DataItem *item = get(key);
    if (item != NULL) {
	if (item->path != NULL)
	    free(item->path);
	item->path = path;
    }
}

/*
 * Get the path for an item
 */
char *ht_getpath(int key) {
    struct DataItem *item = get(key);
    if (item != NULL)
	return item->path;
    else
	return NULL;
}
