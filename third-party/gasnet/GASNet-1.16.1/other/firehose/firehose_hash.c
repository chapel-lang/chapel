/*   $Source: /var/local/cvs/gasnet/other/firehose/firehose_hash.c,v $
 *     $Date: 2009/04/27 21:37:04 $
 * $Revision: 1.13 $
 * Description: 
 * Copyright 2004, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#if 0 /* We now #include <firehose_hash.c> in firehose_{page,region}.c */
#include <stdlib.h>
#include <stdio.h>

#include <firehose.h>
#include <firehose_internal.h>
#include <gasnet_internal.h>
#endif

struct _fh_hash_t {
        void   **fh_table;
        size_t   fh_entries;
        size_t   fh_elemsize;
        unsigned fh_mask;

#ifdef FH_HASH_STATS
        int     *fh_col_table;
        int      fh_used;
        int      fh_collisions;
        uint64_t fh_hops;
#endif
};


/* The following functions implement Thomas Wang's integer hashing functions,
 * found at http://www.concentric.net/~Ttwang/tech/inthash.htm. The hashing
 * functions are useful for integer hashing and make use of CPU native
 * instructions such as 'add complement' and 'shift and add'.
 *
 * A 32-bit and a 64-bit version are implemented below.
 */

/* In firehose, hash tables are created for both local bucket addresses and
 * remote firehoses.  Local bucket addresses are hashed on page addresses (as
 * integers) and remote firehoses are hashed on the bitwise or of
 * page_address|remote_node (this assumes 12 available lower order bits for
 * node).
 */

#define IS_POWER_OF_2(x)	(!((x)&((x)-1)))

#ifdef FH_HASH_STATS
#define	_fh_collision(hash)	(hash)->fh_collisions++
#define _fh_entries_add(hash)	(hash)->fh_used++
#define _fh_entries_del(hash)	(hash)->fh_used--
#else
#define	_fh_collision(hash)
#define _fh_entries_add(hash)
#define _fh_entries_del(hash)
#endif

#define FH_KEY_DELETED	((void *)-1)
#define FH_KEY_NONE	((void *)0)
#define FH_IS_EMPTY(x)	(x == FH_KEY_NONE)
#define FH_IS_DELETED(x)(x == FH_KEY_DELETED)

typedef
struct fh_dummy_entry {
	fh_int_t	hash_key;
	void		*hash_next;
}
fh_dummy_entry_t;

/*
int 
inthash(fh_int_t key)
{
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);
	return key;
}
*/

#if 1
GASNETI_ALWAYS_INLINE(inthash)
int
inthash(fh_int_t key)
{
	key += ~(key << 15);
	key ^=  (key >> 10);
	key +=  (key << 3);
	key ^=  (key >> 6);
	key += ~(key << 11);
	key ^=  (key >> 16);
	return (int) key;
}
#else
GASNETI_ALWAYS_INLINE(inthash)
int
inthash(fh_int_t key)
{
	key += ~(key << 32);
	key ^= (key >> 22);
	key += ~(key << 13);
	key ^= (key >> 8);
	key += (key << 3);
	key ^= (key >> 15);
	key += ~(key << 27);
	key ^= (key >> 31);
	return (int) key;
}
#endif

/* fh_hash_create(keylen,entries)
 *
 * Allocates a table large enough to hold 'entries' entries of size 'keylen'.
 * Note that 'entries' must be a power of two.
 */
static
fh_hash_t *
fh_hash_create(size_t entries)
{
	fh_hash_t	*hash;

	if (!IS_POWER_OF_2(entries))
		gasneti_fatalerror("fh_hash_create requires a power of 2!");

	hash = (fh_hash_t *) gasneti_malloc(sizeof(fh_hash_t));
	if (hash == NULL)
		gasneti_fatalerror("Can't allocate memory for hash structure");
	memset(hash, 0, sizeof(fh_hash_t));

	hash->fh_table   = (void **) gasneti_calloc(entries, sizeof(void *));
	hash->fh_mask    = entries-1;
	hash->fh_entries = entries;
	#ifdef FH_HASH_STATS
		hash->fh_col_table = (int *) gasneti_malloc(entries * sizeof(int));
		/*printf("hash create: entries=%d, mask=%x\n", entries, entries-1);*/
		hash->fh_used = 0;
		hash->fh_collisions = 0;
	#endif
	
	return hash;
}

static
void
fh_hash_destroy(fh_hash_t *hash)
{
#ifdef FH_HASH_STATS
	fprintf(stderr, "elements: %d, collisions: %d, avg=%2.5f%%\n",
		hash->fh_used, hash->fh_collisions,
		(float) hash->fh_collisions*100/hash->fh_used);
	{
		int i, hits;
		for (i = 0; i < hash->fh_entries; i++) {
			hits = hash->fh_col_table[i];
			if (hits) {
				/*printf("%d\t%d\n", i, hits);*/
			}
		}
	}
	gasneti_free(hash->fh_col_table);

#endif
	gasneti_free(hash->fh_table);
	gasneti_free(hash);
}


/* The firehose hash uses open addressing as a collision resolution scheme,
 * since space occupied by each entry must be minimized. */
static
void *
fh_hash_find(fh_hash_t *hash, fh_int_t key)
{
	void		*val;
	fh_int_t	keyhash = inthash(key) & hash->fh_mask;

	val = hash->fh_table[keyhash];

	while (val != NULL && key != ((fh_dummy_entry_t *) val)->hash_key) {
		val = ((fh_dummy_entry_t *) val)->hash_next;
	}

	return val;
}

/* 
 * fh_hash_insert(hash, key, val)
 * If val==NULL, the key is removed from the table
 */
static
void *
fh_hash_insert(fh_hash_t *hash, fh_int_t key, void *newval)
{
	fh_int_t	keyhash;
	void		*val;

	keyhash = inthash(key) & hash->fh_mask;
	val = hash->fh_table[keyhash];

	#ifdef FH_HASH_STATS
		hash->fh_col_table[keyhash]++;
	#endif

	/* May be a deletion request */
	if (newval == NULL) {
		fh_dummy_entry_t *prev = NULL;
		fh_dummy_entry_t *cur = (fh_dummy_entry_t *) val;

		while (cur != NULL) {
			/*
			 * If the key matches, adjust list and return the entry.
			 * 
			 */
			if (cur->hash_key == key) {
				if (prev == NULL)
					hash->fh_table[keyhash] = 
						cur->hash_next;
				else
					prev->hash_next = cur->hash_next;

				return cur;
			}
			prev = cur;
			cur = cur->hash_next;
		}

		/*
		 * No keys found matching deletion request
		 *
		 */
		return NULL;
	}
	/* Add the key mapping */
	else {
		_fh_entries_add(hash);

		/* bucket unused, simply copy the new data */
		if (val == NULL) {
			hash->fh_table[keyhash] = newval;
			((fh_dummy_entry_t *) newval)->hash_next = NULL;
		}
		else {
			_fh_collision(hash);

			((fh_dummy_entry_t *) newval)->hash_next = 
			    hash->fh_table[keyhash];

			hash->fh_table[keyhash] = newval;
		}
		return newval;
	}
}

/* 
 * Apply a given function to all entries in the hash.
 * Deletion of the entry from the function is OK.
 */
void fh_hash_apply(fh_hash_t *hash, void (*fn)(void *val, void *arg), void *arg)
{
	fh_int_t i;

	for (i = 0; i < hash->fh_entries; ++i) {
		void *val = hash->fh_table[i];
		while (val != NULL) {
			void *next = ((fh_dummy_entry_t *) val)->hash_next;
			(*fn)(val, arg);
			val = next;
		}
	}
}

#ifdef FIREHOSE_REGION
/* Additional functionality required for FIREHOSE_REGION, which
 * can have multiple hash entries with identical key.
 */

/* Given an (non-NULL) entry, find the next one with the same key */
static
void *
fh_hash_next(fh_hash_t *hash, void *val)
{
	fh_int_t	key = ((fh_dummy_entry_t *) val)->hash_key;

	do {
		val = ((fh_dummy_entry_t *) val)->hash_next;
	} while (val != NULL && key != ((fh_dummy_entry_t *) val)->hash_key);

	return val;
}

/* Given a (non-NULL) entry, by address not by key, replace
 * it with another entry, or delete if replacement is NULL
 */
static
void
fh_hash_replace(fh_hash_t *hash, void *val, void *newval)
{
	fh_int_t         keyhash;
	fh_dummy_entry_t *cur;

	keyhash = inthash(((fh_dummy_entry_t *)val)->hash_key) & hash->fh_mask;
	cur = (fh_dummy_entry_t *)(hash->fh_table[keyhash]);

	/* Handle head of list case first */
	if (cur == ((fh_dummy_entry_t *) val)) {
		if (newval == NULL) {
			/* Delete */
			hash->fh_table[keyhash] = cur->hash_next;
		}
		else {
			/* Replace */
			((fh_dummy_entry_t *) newval)->hash_next =
							 cur->hash_next;
			hash->fh_table[keyhash] = newval;
		}
		return;
	}

	/* Now handle non-head case */
	while (cur != NULL) {
		fh_dummy_entry_t *next = cur->hash_next;

		if (next == ((fh_dummy_entry_t *) val)) {
			if (newval == NULL) {
				/* Delete */
				cur->hash_next = next->hash_next;
			}
			else {
				/* Replace */
				cur->hash_next = newval;
				((fh_dummy_entry_t *) newval)->hash_next = next->hash_next;
			}
			return;
		}

		cur = next;
	}
}
#endif /* defined(FIREHOSE_REGION) */
