/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_hashtable.h,v $
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

#ifndef GASNET_COLL_HASHTABLE_H_
#define GASNET_COLL_HASHTABLE_H_

#include "portable_inttypes.h"

/* Part I: Table */

typedef struct gasnete_table_item
{
	uint32_t key;
	void * data;
} gasnete_table_item_t;

typedef struct gasnete_table
{
	gasnete_table_item_t *slots;
	uint32_t size;
	uint32_t num;
} gasnete_table_t;

#define TABLE_SIZE(tab)	(tab->size) /**< return the maximum size of the queue */
#define TABLE_NUM(tab)	(tab->num)	/**< return the current number of items in the table */

gasnete_table_t * gasnete_table_create(uint32_t size);

gasnete_table_item_t * gasnete_table_search(const gasnete_table_t * const table, uint32_t key);

uint32_t gasnete_table_insert(gasnete_table_t * const table, gasnete_table_item_t item);

uint32_t gasnete_table_remove(gasnete_table_t * const table, uint32_t key, gasnete_table_item_t * deleted);

void gasnete_table_copy(const gasnete_table_t * const src, gasnete_table_t * const dst);

void gasnete_table_free(gasnete_table_t * const table);

/* Part II: Hashtable */

typedef struct gasnete_hashtable
{
  gasnete_table_t ** buckets;
  uint32_t size; /**< hash table size (# of buckets) */
  uint32_t num;  /**< number of elements in the hash table */
} gasnete_hashtable_t;

/**
 * Hash function that determines the bucket for the element with a key
 */
static inline uint32_t gasnete_hashtable_hash(gasnete_hashtable_t * ht, uint32_t key)
{
  return (key % ht->size);
}

/**
 * Create a hash table
 */
gasnete_hashtable_t * gasnete_hashtable_create(uint32_t size);

uint32_t gasnete_hashtable_search(gasnete_hashtable_t * ht, uint32_t key, void ** data);

uint32_t gasnete_hashtable_insert(gasnete_hashtable_t * ht, uint32_t key, void * data);

uint32_t gasnete_hashtable_remove(gasnete_hashtable_t * ht, uint32_t key, void ** data);

void gasnete_hashtable_free(gasnete_hashtable_t * ht);

#endif /* GASNET_COLL_HASHTABLE_H_ */
