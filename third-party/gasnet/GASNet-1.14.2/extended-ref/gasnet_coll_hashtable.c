/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_hashtable.c,v $
 *     $Date: 2009/10/28 04:17:13 $
 * $Revision: 1.4 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

/**
 * Implement hash table data structure using vectors (chaining) to
 * solve collisions
 *
 * For information about the data structures and algorithms used in the
 * implementation, please see Ch. 12 of Introduction to Algorithms
 * by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest.
 *
 * Lawrence Berkeley National Laboratory
 * 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <gasnet_coll_hashtable.h>

/* Part I: table */

gasnete_table_t * gasnete_table_create(uint32_t size)
{
  gasnete_table_t * table;

  assert(size > 0);
  table = (gasnete_table_t *)malloc(sizeof(gasnete_table_t));
  assert(table != NULL);

  table->slots = (gasnete_table_item_t *)malloc(sizeof(gasnete_table_item_t)*size);
  assert(table->slots != NULL);

  table->size = size;
  table->num = 0;

  return table;
}

gasnete_table_item_t * gasnete_table_search(const gasnete_table_t * const table, uint32_t key)
{
  uint32_t i;
  gasnete_table_item_t * slots;

  assert(table != NULL);
  slots = table->slots;
  for (i=0; i<table->num; i++)
    if (key == slots[i].key)
      return &slots[i];

  return NULL; /* item with key is not found in the table */
}

uint32_t gasnete_table_insert(gasnete_table_t * const table, gasnete_table_item_t item)
{
  if (table->num >= table->size)
    return 1; /* insertion failed because the table is full */

  /* added the item to the end of the table */
  table->slots[table->num] = item;
  table->num++;

  return 0; /* success */
}

uint32_t gasnete_table_remove(gasnete_table_t * const table, uint32_t key, gasnete_table_item_t * deleted)
{
  gasnete_table_item_t item;
  uint32_t i;
  gasnete_table_item_t * slots;

  assert(table != NULL);
  slots = table->slots;
  for (i=0; i<table->num; i++)
    if (key == slots[i].key) {
        item = table->slots[i];
        if (deleted != NULL) {
          deleted->key = item.key;
          deleted->data = item.data;
        }

        if (i < table->num-1)
          table->slots[i] = table->slots[table->num-1];

        table->num--;
        return 0;
      }

#ifdef DEBUG
  printf("Trying to remove an item not in the table!\n");
#endif

  return 1; /* item not in the table */
}

void gasnete_table_copy(const gasnete_table_t * const src, gasnete_table_t * const dst)
{
  uint32_t i;
  gasnete_table_item_t * src_slots, * dst_slots;

  assert(dst->size >= src->num);

  src_slots = src->slots;
  dst_slots = dst->slots;
  for (i=0; i<src->num; i++)
    dst_slots[i] = src_slots[i];

  dst->num = src->num;
}

void gasnete_table_free(gasnete_table_t * const table)
{
  assert(table != NULL);

  free(table->slots);
  free(table);
}


/* Part II: hashtable build over table */

#define TABLE_INIT_SIZE 10

gasnete_hashtable_t * gasnete_hashtable_create(uint32_t size)
{
  gasnete_hashtable_t * ht;
  uint32_t i;

  assert(size > 0);
  ht = (gasnete_hashtable_t *)malloc(sizeof(gasnete_hashtable_t));
  assert(ht != NULL);
  ht->buckets = (gasnete_table_t **)malloc(sizeof(gasnete_table_t *)*size);
  assert(ht->buckets != NULL);
  ht->size = size;
  ht->num = 0;

  for (i=0; i<size; i++) {
    ht->buckets[i] = gasnete_table_create(TABLE_INIT_SIZE);
    assert(ht->buckets[i] != NULL);
  }
 
  return ht;
}

void gasnete_hashtable_free(gasnete_hashtable_t * ht)
{
  uint32_t i;

  assert(ht != NULL);
  assert(ht->buckets != NULL);
   
  for (i=0; i<ht->size; i++) {
    assert(ht->buckets[i] != NULL);
    gasnete_table_free(ht->buckets[i]);
  }

  free(ht->buckets);
  free(ht);
}

uint32_t gasnete_hashtable_search(gasnete_hashtable_t * ht, uint32_t key, void ** data)
{
  gasnete_table_t * table;
  gasnete_table_item_t * item;

  assert(ht != NULL);

  table = ht->buckets[gasnete_hashtable_hash(ht, key)];
  assert (table != NULL);

  item = gasnete_table_search(table, key);
  if (item == NULL)
    return 1; /* cannot find the item with key */

  if (data != NULL)
    *data = item->data;

  return 0; /* success */
}

uint32_t gasnete_hashtable_insert(gasnete_hashtable_t * ht, uint32_t key, void * data)
{
  gasnete_table_t * table;
  gasnete_table_item_t item;
  uint32_t i;

  assert(ht != NULL);

  item.key = key;
  item.data = data;

  i = gasnete_hashtable_hash(ht, key);
  table = ht->buckets[i];
  assert (table != NULL);

  /* double the size of the table if the table is full */
  if (table->num == table->size) {
    gasnete_table_t * new_table;
    new_table = gasnete_table_create(table->size*2);
    assert(new_table != NULL);
    gasnete_table_copy(table, new_table);
    ht->buckets[i] = new_table;
    table = new_table;
  }

  ht->num++;
  return gasnete_table_insert(table, item);
}

uint32_t gasnete_hashtable_remove(gasnete_hashtable_t * ht, uint32_t key, void ** data)
{
  gasnete_table_t * table;
  gasnete_table_item_t item;
  uint32_t i, rv;

  assert(ht != NULL);
  i = gasnete_hashtable_hash(ht, key);
  table = ht->buckets[i];
  assert (table != NULL);
  if (table == NULL)
    return 1;

  rv = gasnete_table_remove(table, key, &item);
  if (rv == 0 && data != NULL)
    *data = item.data;
  
  ht->num--;
  return rv;
}
