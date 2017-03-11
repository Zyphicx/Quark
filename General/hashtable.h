#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

struct hash_entry{
	char *key;
	void *value;
	struct hash_entry *next;
};

struct hash_table{
	size_t size;
	struct hash_entry **entries;
};

typedef struct hash_entry HashEntry;
typedef struct hash_table HashTable;

HashTable *table_new(size_t size);
void table_free(HashTable *table);
int hash(char *value, int size);
void insert(HashTable *table, HashEntry *entry);
HashEntry *drop(HashTable *table, HashEntry *entry);
HashEntry *lookup(HashTable *table, char *s);
HashEntry *entry_create(char *key, void *value);
void entry_free(HashEntry *entry);
void entry_setval(HashEntry *entry, void *value);

#endif