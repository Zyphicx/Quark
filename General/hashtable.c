#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

HashTable *table_new(size_t size){
	HashTable *table = (HashTable *)malloc(sizeof(HashTable));
	table->entries = malloc(size * sizeof(HashEntry *));
	table->size = size;
	HashEntry **entry;
	for(entry = table->entries; entry < (table->entries + size); entry++){
		*entry = NULL;
	}
	return table;
}

void table_free(HashTable *table){
	HashEntry **entry;

	for(entry = table->entries; entry < (table->entries + table->size); entry++){
		while(*entry != NULL){
			//printf("Thing - %s\n", (char *)((*entry)->value));
			HashEntry *temp = (*entry)->next;
			entry_free(*entry);
			*entry = temp;
		}
	}
	free(table->entries);
	free(table);
}

int hash(char *value, int size){
	int hash = 0;

	while(*value){
		hash >>= 3;
		hash ^= ((*value - 1) * 23);
		++value;
	}

	return hash % size;
}

void table_insert(HashTable *table, HashEntry *entry){
	int hashval = hash(entry->key, table->size);

	if(table->entries[hashval] == NULL){
		table->entries[hashval] = entry;
	}else{
		HashEntry *e;
		for(e = table->entries[hashval]; e->next != NULL; e = e->next) //strcmp if equal?
			;
		e->next = entry;
	}
}

HashEntry *table_lookup(HashTable *table, char *s){
	int hashval = hash(s, table->size);
	HashEntry *entry;

	if((entry = table->entries[hashval]) != NULL){
		while(strcmp(entry->key, s) != 0) {
			if((entry = entry->next) == NULL)
				return NULL;
		}
	}

	return entry;
}

HashEntry *table_drop(HashTable *table, HashEntry *entry){
	int hashval = hash(entry->key, table->size);
	HashEntry *e;

	if((e = table->entries[hashval]) != NULL){
		if(entry->next == NULL){
			if(e == entry){
				table->entries[hashval] = NULL;
			}
			while(e->next != NULL){
				if(e->next == entry){
					e->next = NULL;
				}else{
					e = e->next;
				}
			}
		}else{
			if(e == entry){
				table->entries[hashval] = entry->next;
			}
			while(e->next != NULL){
				if(e->next == entry){
					e->next = entry->next;
				}else{
					e = e->next;
				}
			}
		}
	}else{
		return NULL;
	}

	return entry;
}

HashEntry *entry_create(char *key, void *value){
	HashEntry *entry = (HashEntry *)malloc(sizeof(HashEntry));

	entry->key = malloc(strlen(key) + 1);
	strcpy(entry->key, key);

	entry->value = value;

	entry->next = NULL;
	return entry;
}

void entry_free(HashEntry *entry){
	if(entry->key != NULL)
		free(entry->key);

	/* if(entry->value != NULL)
		free(entry->value);*/

	free(entry);
}

void entry_setval(HashEntry *entry, void *value){
	if(entry->value != NULL)
		free(entry->value);
	entry->value = malloc(sizeof(value));
	entry->value = value;
}

/*int main(int argc, char *argv[]){
	//if(argc == 2)
		//printf("%u\n", hash(argv[1], table->size));

	HashTable *table = table_new();

	FILE *fp = fopen(argv[1], "r");
	char buffer[100];

	while(fgets(buffer, 100, fp)){
		printf("%s\n", buffer);
		insert(table, entry_create(buffer, "Works!"));

		HashEntry *entry = lookup(table, buffer);

		printf("%s, %s\n", entry->key, (char *)entry->value);
		entry_free(entry);
	}
	return 1;
}*/
