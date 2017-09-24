#ifndef HASHTABLE_H
#define HASHTABLE_H

int ht_get(int key);
int ht_contains(int key);
void ht_put(int key, int data);
int ht_delete(int key);
void ht_clear(void);
void ht_setpath(int key, char *path);
char *ht_getpath(int key);

#endif
