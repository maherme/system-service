/**
 * @file hashtable.h
 * @brief header file of hashtable.c
 */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

typedef char* ht_key_t;
typedef char* ht_value_t;

/** @struct HtItem
 *  @brief each item of the hash table.
 */
typedef struct HtItem HtItem;
struct HtItem{
    ht_key_t key;
    ht_value_t value;
    HtItem *next; /* used for hash collisions */
};

/** @struct HashTable
 *  @brief contains the hash table and some metadatas.
 */
typedef struct HashTable{
    HtItem **items;
    int size;
    int count;
} HashTable;

HashTable* ht_create_table(int size);
void ht_free_table(HashTable *table);
void ht_insert(HashTable *table, ht_key_t key, ht_value_t value);
HtItem* ht_search(HashTable *table, ht_key_t key);

#endif
