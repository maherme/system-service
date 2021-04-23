/**
 * @file hashtable.c
 * @brief this file contains functions needed to implement a hash table.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../inc/hashtable.h"

/**
 * @brief function wrapper to alloce memory and to control failures.
 *
 * @param[in] mem_size size of memory needed to be allocated.
 * @param[in] nelem number of elements for an array (used for calloc).
 *
 * @return pointer returned by standard function called for allocating memory.
 */
static void *allocate_mem(size_t mem_size, size_t nelem){
    void *alloc_mem = NULL;

    if(nelem == 0){
        alloc_mem = malloc(mem_size);
    }
    else{
        alloc_mem = calloc(nelem, mem_size);
    }

    if(!alloc_mem && mem_size){
        perror("Error: memory could not be allocated");
        exit(EXIT_FAILURE);
    }

    return alloc_mem;
}

/**
 * @brief function to update a string. Similar to strdup
 * but checking malloc error.
 *
 * @param[i] *src pointer to new string value.
 *
 * @return pointer returned to the new string.
 */
char* upd_str(const char *src){

    size_t len = strlen(src)+1;
    char *des = allocate_mem(strlen(src)+1, 0);

    return (char *)memcpy(des, src, len);
}

/**
 * @brief function to calculate the index in the hash table.
 *
 * @param[in] *table pointer to hash table.
 * @param[in] key input key to calculate the index.
 *
 * @return index related to key:value.
 */
static int ht_index(HashTable *table, const ht_key_t key){ /* djb2 algorithm */

    unsigned long sum = 5381;

    for(int i=0; key[i]; i++){
        sum = ((sum << 5) + sum) + key[i];  /* ((sum << 5) + sum) == (33 * sum) */
    }

    return sum % table->size;
}

/**
 * @brief function to create an item in the hash table.
 *
 * @param[in] key key to store in the hash table.
 * @param[in] value value to store in the hash table.
 * @param[in] section section to store in the hash table.
 *
 * @return pointer to the item in the hash table.
 */
static HtItem* ht_create_item(ht_key_t key, ht_value_t value, ht_section_t section){

    HtItem *item = allocate_mem(sizeof(*item), 0);
    item->key = upd_str(key);
    item->value = upd_str(value);
    item->section = upd_str(section);
    item->next = NULL;

    return item;
}

/**
 * @brief function to free an item in the hash table.
 *
 * @param[in] *item pointer to the item will be free.
 *
 * @return void.
 */
static void ht_free_item(HtItem *item){

    free(item->key);
    free(item->value);
    free(item->section);
    free(item);
}

/**
 * @brief function to create a hash table.
 *
 * @param[in] size the size of the hash table.
 *
 * @return pointer to the hast table.
 */
HashTable* ht_create_table(int size){

    HashTable *table = allocate_mem(sizeof(HashTable), 0);
    table->size = size;
    table->count = 0;
    table->items = allocate_mem(table->size, sizeof(*table->items));

    return table;
}

/**
 * @brief function to free the whole hash table.
 *
 * @param[in] *table pointer to the hast table.
 *
 * @return void.
 */
void ht_free_table(HashTable *table){

    for(int i=0; i<table->size; i++){
        HtItem *item = table->items[i];
        while(item){ /* free all items with the same hash */
            HtItem *next = item->next;
            ht_free_item(item);
            item = next;
        }
    }

    free(table->items);
    free(table);
}

/**
 * @brief function to insert an item in the hash table or update if exists.
 *
 * @param[out] *table pointer to the hash table.
 * @param[in] key key to store in the hash table.
 * @param[in] value value to store in the hash table.
 * @param[in] section section to store in the hash table.
 *
 * @return void.
 */
void ht_insert(HashTable *table, ht_key_t key, ht_value_t value, ht_section_t section){

    int index = ht_index(table, key);
    HtItem **slot = &table->items[index];
    HtItem *item = *slot;

    if(!item){
        table->count++; /* new item */
    }

    while(item){ /* check if key already exists */
        if(strcmp(item->key, key) == 0){
            free(item->value); /* upd_str is malloc + memcpy */
            item->value = upd_str(value); /* value is udpated */
            item->section = upd_str(section); /* value is updated */
            return;
        }
        slot = &item->next;
        item = *slot;
    }

    *slot = ht_create_item(key, value, section);
}

/**
 * @brief function to search a value in the hash table.
 *
 * @param[in] *table pointer to the hash table.
 * @param[in] key key of the value for searching.
 *
 * @return the item related with the key passed as argument,
 * if the item is not found it will return NULL.
 */
HtItem* ht_search(HashTable *table, ht_key_t key){

    int index = ht_index(table, key);
    HtItem *item = table->items[index];

    while(item){ /* iterate all items with hash collisions */
        if(strcmp(item->key, key) == 0){
            return item;
        }
        item = item->next;
    }

    return NULL;
}
