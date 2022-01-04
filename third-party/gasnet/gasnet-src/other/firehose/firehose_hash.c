/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/firehose/firehose_hash.c $
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
#if FH_HASH_KNUTH
        unsigned fh_shift;
#else
        unsigned fh_mask;
#endif

#ifdef FH_HASH_STATS
        int     *fh_col_table;
        int      fh_used;
        int      fh_collisions;
        uint64_t fh_hops;
#endif
};


/* In firehose, hash tables are created for both local bucket addresses and
 * remote firehoses.  Local bucket addresses are hashed on page addresses (as
 * integers) and remote firehoses are hashed on the bitwise or of
 * page_address|remote_node when enough lower order bits are available for node,
 * or their exclusive-or when node numbers are larger than the available bits.
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

typedef
struct fh_dummy_entry {
	fh_key_t	hash_key;
	void		*hash_next;
}
fh_dummy_entry_t;

#if FH_HASH_KNUTH

// Knuth's multiplicative hashing
// See "The Art of Computer Programming, Volume 3, Sorting and Searching", D.E. Knuth, section 6.4

GASNETI_INLINE(knuth_hash)
int
knuth_hash(fh_key_t full_key, fh_hash_t *hash)
{
  // Compute (k * (sqrt(5)-1)/2) mod 1.0
  // The following "just works" to get the 32 most signficant bits of the
  // fractional part due to pre-shifting of the constant and overflow in the
  // multiplication.
  uint32_t kA_fract = 2654435769U * FH_KEY2INT(full_key);

  // Then extract the 'p' *most* significant bits of that fractional part
  int result = kA_fract >> hash->fh_shift;
  gasneti_assert(result >= 0);
  gasneti_assert_int(result ,<, hash->fh_entries);
  return result;
}
#define KEYHASH(key,hash) knuth_hash((key),(hash))

#else

/* The following functions implement Thomas Wang's integer hashing functions,
 * found at http://www.concentric.net/~Ttwang/tech/inthash.htm. The hashing
 * functions are useful for integer hashing and make use of CPU native
 * instructions such as 'add complement' and 'shift and add'.
 *
 * A 32-bit and a 64-bit version are implemented below.
 */

GASNETI_INLINE(inthash)
int
inthash(fh_key_t full_key)
{
	intptr_t key = FH_KEY2INT(full_key);
#if PLATFORM_ARCH_32
	key += ~(key << 15);
	key ^=  (key >> 10);
	key +=  (key << 3);
	key ^=  (key >> 6);
	key += ~(key << 11);
	key ^=  (key >> 16);
#else
	key += ~(key << 32);
	key ^= (key >> 22);
	key += ~(key << 13);
	key ^= (key >> 8);
	key += (key << 3);
	key ^= (key >> 15);
	key += ~(key << 27);
	key ^= (key >> 31);
#endif
	return (int) key;
}
#define KEYHASH(key,hash) \
        (inthash(key) & (hash)->fh_mask);
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

	hash = (fh_hash_t *) gasneti_calloc(1,sizeof(fh_hash_t));

	hash->fh_table   = (void **) gasneti_calloc(entries, sizeof(void *));
#if FH_HASH_KNUTH
        // 32 minus desired bits of hash value
        hash->fh_shift   = 32;
        for (size_t e = entries-1; e; e >>= 1) {
                hash->fh_shift -= 1;
        }
#else
	hash->fh_mask    = entries-1;
#endif
	hash->fh_entries = entries;
#ifdef FH_HASH_STATS
        hash->fh_col_table = (int *) gasneti_calloc(entries, sizeof(int));
    #if FH_HASH_KNUTH
        gasneti_console_message("INFO",
               "hash create: entries=%"PRIuSZ", bits=%u\n",
               entries, (32 - hash->fh_shift));
    #else
        gasneti_console_message("INFO",
               "hash create: entries=%"PRIuSZ", mask=%"PRIxSZ"\n",
               entries, entries-1);
    #endif
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
        gasneti_console_message("INFO",
                "entries: %"PRIuSZ" elements: %d, collisions: %d, avg=%2.5f%%, load %g\n",
		hash->fh_entries, hash->fh_used, hash->fh_collisions,
                (double) hash->fh_collisions*100/hash->fh_used,
                (double) hash->fh_used/hash->fh_entries);
	if (0) {
		int i, hits;
		for (i = 0; i < hash->fh_entries; i++) {
			hits = hash->fh_col_table[i];
			if (hits) {
				printf("%d\t%d\n", i, hits);
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
fh_hash_find(fh_hash_t *hash, fh_key_t key)
{
	void		*val;
	int		keyhash = KEYHASH(key, hash);

	val = hash->fh_table[keyhash];

	while (val != NULL && !FH_KEY_EQ(key, ((fh_dummy_entry_t *) val)->hash_key)) {
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
fh_hash_insert(fh_hash_t *hash, fh_key_t key, void *newval)
{
	int		keyhash;
	void		*val;

	keyhash = KEYHASH(key, hash);
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
			if (FH_KEY_EQ(cur->hash_key, key)) {
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
	int i;

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
	fh_key_t	key = ((fh_dummy_entry_t *) val)->hash_key;

	do {
		val = ((fh_dummy_entry_t *) val)->hash_next;
	} while (val != NULL && !FH_KEY_EQ(key, ((fh_dummy_entry_t *) val)->hash_key));

	return val;
}

/* Given a (non-NULL) entry, by address not by key, replace
 * it with another entry, or delete if replacement is NULL
 */
static
void
fh_hash_replace(fh_hash_t *hash, void *val, void *newval)
{
	int         keyhash;
	fh_dummy_entry_t *cur;

	keyhash = KEYHASH(((fh_dummy_entry_t *)val)->hash_key, hash);
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
