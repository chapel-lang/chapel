/*   $Source: /var/local/cvs/gasnet/other/firehose/firehose_region.c,v $
 *     $Date: 2009/04/27 21:37:04 $
 * $Revision: 1.39 $
 * Description: 
 * Copyright 2004, Paul Hargrove <PHHargrove@lbl.gov>
 * Terms of use are as specified in license.txt
 */
#include <firehose.h>
#include <firehose_internal.h>
#include <gasnet.h>
#include <gasnet_handler.h>

#ifdef FIREHOSE_REGION

#include <firehose_hash.c> /* For possible inlining */

typedef
struct _fh_bucket_t {
        fh_int_t         fh_key;	/* cached key for hash table */
        void            *fh_next;	/* linked list in hash table */
					/* _must_ be in this order */

        /* pointer to the containing region.  holds ref counts, etc */
        firehose_private_t      *priv;
        /* pointer to next bucket in same region */
        struct _fh_bucket_t     *next;
}
fh_bucket_t;

/* IFF firehose_fwd.h DID set these, complain now */
#ifdef FIREHOSE_CLIENT_MAXREGION_SIZE
  #error "Conduits should no longer define FIREHOSE_CLIENT_MAXREGION_SIZE in firehose_fwd.h"
#endif
#ifdef FIREHOSE_CLIENT_MAXREGIONS
  #error "Conduits should no longer define FIREHOSE_CLIENT_MAXREGIONS in firehose_fwd.h"
#endif

/* ##################################################################### */
/* GLOBAL TABLES, LOCKS, ETC.                                            */
/* ##################################################################### */

static firehose_private_t *fhi_lookup_cache;
static firehose_private_t *fhi_priv_freelist;

static size_t fhi_MaxRegionSize;

/* Maximum number of regions to unpin per move callback */
#ifndef FH_MAX_UNPIN_REM
  #define FH_MAX_UNPIN_REM 4
#endif
#ifndef FH_MAX_UNPIN_LOC
  #define FH_MAX_UNPIN_LOC 4
#endif

/* ##################################################################### */
/* FORWARD DECLARATIONS, INTERNAL MACROS, ETC.                           */
/* ##################################################################### */

/* Disqualify remote pending buckets 
 * and local buckets w/ only remote counts and no "free energy" */
#define FH_IS_READY(is_local, priv)            \
	((is_local)                            \
	    ?  (FHC_MAXVICTIM_BUCKETS_AVAIL || \
		FH_IS_LOCAL_FIFO(priv) ||      \
		FH_BUCKET_REFC(priv)->refc_l)  \
	    : !FH_IS_REMOTE_PENDING(priv))

#ifdef FIREHOSE_CLIENT_T
  #define FH_CP_CLIENT(A,B) (A)->client = (B)->client
#else
  #define FH_CP_CLIENT(A,B)
#endif

/* Assumes node field is correct */
#define CP_PRIV_TO_REQ(req, priv) 	do {			\
		(req)->addr = FH_BADDR(priv);			\
		(req)->len = (priv)->len;			\
		(req)->internal = (priv);			\
		FH_CP_CLIENT((req), (priv));			\
	} while(0)

#define CP_REG_TO_PRIV(priv, node, reg) 	do {		    \
		(priv)->fh_key = FH_KEYMAKE((reg)->addr, (node));   \
		(priv)->len = (reg)->len;			    \
		FH_CP_CLIENT((priv), (reg));			    \
	} while(0)
#define CP_PRIV_TO_REG(reg, priv) 	do {			\
		(reg)->addr = FH_BADDR(priv);			\
		(reg)->len = (priv)->len;			\
		FH_CP_CLIENT((reg), (priv));			\
	} while(0)

/* ##################################################################### */
/* VARIOUS HELPER FUNCTIONS                                              */
/* ##################################################################### */

/* compute ending address of "region" */
GASNETI_INLINE(fh_region_end)
uintptr_t fh_region_end(const firehose_region_t *region)
{
	gasneti_assert(region != NULL);
	return (region->addr + (region->len - 1));
}

/* compute ending address of "req" */
GASNETI_INLINE(fh_req_end)
uintptr_t fh_req_end(const firehose_request_t *req)
{
	gasneti_assert(req != NULL);
	return (req->addr + (req->len - 1));
}

/* compute ending address of "priv" */
GASNETI_INLINE(fh_priv_end)
uintptr_t fh_priv_end(const firehose_private_t *priv)
{
	gasneti_assert(priv != NULL);
	return (FH_BADDR(priv) + (priv->len - 1));
}

/* compute ending address of "bucket" */
GASNETI_INLINE(fh_bucket_end)
uintptr_t fh_bucket_end(const fh_bucket_t *bucket)
{
	gasneti_assert(bucket != NULL);
	return fh_priv_end(bucket->priv);
}

/* Compare two buckets with the same (node, address)
 * return non-zero if first is best
 * "best" is the one with the greatest forward extent.
 * In case of a tie on forward extent, the longer region wins.
 * In case of a complete tie, we return 0.
 */
GASNETI_INLINE(fh_bucket_is_better)
int fh_bucket_is_better(const fh_bucket_t *a, const fh_bucket_t *b)
{
  uintptr_t end_a, end_b;

  gasneti_assert(a != NULL);
  gasneti_assert(a->priv != NULL);
  gasneti_assert(b != NULL);
  gasneti_assert(b->priv != NULL);
  gasneti_assert(a->fh_key == b->fh_key);

  end_a = fh_bucket_end(a);
  end_b = fh_bucket_end(b);

  if_pt (end_a != end_b) {
    return (end_a > end_b);
  }
  else {
    return (a->priv->len > b->priv->len);
  }
}

/* Compare two buckets with the same (node, address) to pick the "best" one */
GASNETI_INLINE(fh_best_bucket)
fh_bucket_t *fh_best_bucket(fh_bucket_t *a, fh_bucket_t *b)
{
  return (fh_bucket_is_better(a,b)) ? a : b;
}

/* ##################################################################### */
/* BUCKET TABLE HANDLING                                                 */
/* ##################################################################### */

/* "Best" matches for each bucket go in the first ("best") hash table (1).
 * Any others go (unsorted) in the second ("other") hash table (2).
 *
 * Lookup only needs to consult the "best" list.
 * Insertion may involve bumping an entry from "best" to "other".
 * Deletion may involve promoting an entry from "other" to "best".
 * Only the deletion requires comparisions in the "other" list and
 * then only if we are deleting what is otherwise our best match.
 */
fh_hash_t *fh_BucketTable1;
fh_hash_t *fh_BucketTable2;


#define fh_bucket_uncover(B)	do { ((B)->priv)->visible += 1; } while(0)
#define fh_bucket_cover(B)	do { ((B)->priv)->visible -= 1; } while(0)

GASNETI_INLINE(fh_bucket_cover_and_check)
void fh_bucket_cover_and_check(fh_bucket_t *bucket)
{
  firehose_private_t *priv = bucket->priv;
  fh_bucket_cover(bucket);
  if (priv->visible == 0) {
    fh_fifoq_t *fifo_head = NULL;
    gasnet_node_t node = FH_NODE(bucket);
    if (node == gasneti_mynode) {
      if (FH_IS_LOCAL_FIFO(priv)) {
	fifo_head = &fh_LocalFifo;
      }
    } else {
      if (FH_IS_REMOTE_FIFO(priv)) {
	fifo_head = &fh_RemoteNodeFifo[node];
      }
    }
    if (fifo_head) {
      /* Move to HEAD of the FIFO */
      gasneti_assert(!FH_TAILQ_EMPTY(fifo_head));
      FH_TAILQ_REMOVE(fifo_head, priv);
      FH_TAILQ_INSERT_HEAD(fifo_head, priv);
    } else {
      /* Not yet in FIFO */
    }
  }
}

static fh_bucket_t
*fh_bucket_lookup(gasnet_node_t node, uintptr_t addr)
{
        FH_TABLE_ASSERT_LOCKED;

        FH_ASSERT_BUCKET_ADDR(addr);

        /* Only ever need to lookup in the first table */
        return (fh_bucket_t *)
		fh_hash_find(fh_BucketTable1, FH_KEYMAKE(addr, node));
}

static void
fh_bucket_hash(fh_bucket_t *bucket, fh_int_t key)
{
	fh_bucket_t *other;
	fh_hash_t *hash;

        FH_TABLE_ASSERT_LOCKED;
	gasneti_assert(bucket != NULL);

	bucket->fh_key = key;
	hash = fh_BucketTable1;

	/* check for existing entry, resolving conflict if any */
	other = (fh_bucket_t *)fh_hash_find(fh_BucketTable1, key);
	if_pf (other != NULL) {
		/* resolve conflict */
		if (fh_bucket_is_better(bucket, other)) {
			fh_hash_replace(fh_BucketTable1, other, bucket);
			fh_bucket_uncover(bucket);
			fh_bucket_cover_and_check(other);
			bucket = other;
		}
		hash = fh_BucketTable2;
	} else {
		fh_bucket_uncover(bucket);
	}

	fh_hash_insert(hash, key, bucket);

	return;
}

static void
fh_bucket_unhash(fh_bucket_t *bucket)
{
    fh_int_t key;

    FH_TABLE_ASSERT_LOCKED;
    gasneti_assert(bucket != NULL);

    key = bucket->fh_key;

    /* check for existence in "best" list */
    if_pf ((fh_bucket_t *)fh_hash_find(fh_BucketTable1, key) == bucket) {
        /* found in the "best" list, so must search for a replacement */
        fh_bucket_t *best = (fh_bucket_t *)fh_hash_find(fh_BucketTable2, key);

        if (best != NULL) {
	    fh_bucket_t *other = fh_hash_next(fh_BucketTable2, best);

            while (other) {
                best = fh_best_bucket(other, best);
                other = fh_hash_next(fh_BucketTable2, other);
	    }

	    fh_hash_replace(fh_BucketTable2, best, NULL);
	    fh_hash_replace(fh_BucketTable1, bucket, best);
	    fh_bucket_uncover(best);
	}
	else {
	    fh_hash_replace(fh_BucketTable1, bucket, NULL);
	}

	fh_bucket_cover(bucket); /* NOT fh_bucket_cover_and_check() which would place in FIFO */
    }
    else {
	fh_hash_replace(fh_BucketTable2, bucket, NULL);
    }

    return;
}

static void
fh_bucket_rehash(fh_bucket_t *bucket)
{
    fh_bucket_t *other;
    fh_int_t key;

    FH_TABLE_ASSERT_LOCKED;
    gasneti_assert(bucket != NULL);

    key = bucket->fh_key;

    other = (fh_bucket_t *)fh_hash_find(fh_BucketTable1, key);
    if_pf ((other != bucket) && fh_bucket_is_better(bucket, other)) {
	/* We've moved up in the world */
	fh_hash_replace(fh_BucketTable2, bucket, NULL);
	fh_hash_replace(fh_BucketTable1, other, bucket);
	fh_hash_insert(fh_BucketTable2, key, other);
	fh_bucket_uncover(bucket);
	fh_bucket_cover_and_check(other);
    }

    return;
}

/* XXX: merge freelist w/ page? */
static fh_bucket_t *fhi_bucket_freelist = NULL;

GASNETI_INLINE(fh_bucket_new)
fh_bucket_t *fh_bucket_new(void)
{
    fh_bucket_t *bucket = fhi_bucket_freelist;

    if_pt (bucket != NULL) {
	fhi_bucket_freelist = bucket->fh_next;
    }
    else {
        bucket = gasneti_malloc(sizeof(fh_bucket_t));
    }
    memset(bucket, 0, sizeof(fh_bucket_t));

    return bucket;
}

GASNETI_INLINE(fh_bucket_free)
void fh_bucket_free(fh_bucket_t *bucket)
{
    bucket->fh_next = fhi_bucket_freelist;
    fhi_bucket_freelist = bucket;
}

GASNETI_INLINE(fh_clean_covered)
int fh_clean_covered(int limit, firehose_region_t *reg, fh_fifoq_t *fifo_head) {
  firehose_private_t *priv = FH_TAILQ_FIRST(fifo_head);
  int count = 0;
  while ((count < limit) && priv && !priv->visible) {
    ++count;
    priv =  FH_TAILQ_NEXT(priv);
  }
  if (count) {
    fh_FreeVictim(count, reg, fifo_head);
  }
  return count;
}

GASNETI_INLINE(fh_clean_covered_local)
int fh_clean_covered_local(int limit, firehose_region_t *reg) {
  int count = fh_clean_covered(limit, reg, &fh_LocalFifo);
  fhc_LocalVictimFifoBuckets -= count;
  fhc_LocalOnlyBucketsPinned -= count;
  return count;
}

GASNETI_INLINE(fh_clean_covered_remote)
int fh_clean_covered_remote(gasnet_node_t node, int limit, firehose_region_t *reg) {
  int count = fh_clean_covered(limit, reg, &fh_RemoteNodeFifo[node]);
  fhc_RemoteVictimFifoBuckets[node] -= count;
  return count;
}

/* Also keep a hash table of the local private_t's we create so that we can
 * match them when received in an AM (pin reply or unpin request).
 * XXX: This use is trashing the NODE portion of priv->fh_key.  We've been
 * careful to ensure that the only thing this breaks is the debugging output.
 * However, we should really see about a better way to do the lookup from
 * the unpin request to the private_t.  I see two options:
 * 1) Carry an extra field over the wire (probably the actual private_t *)
 *    that must be passed back to unpin (stored and passed in exactly the
 *    same places that the client_t is)
 * 2) Keep a "PrivTable" which hashes locally pinned regions, but does so
 *    external to the private_t.
 * If only local regions need to be hashed in this way then #2 wins on both
 * network traffic and storage space.
 * If remote regions are also hashed in this way (perhaps for detection of
 * duplicates?), then #2 wins on network traffic but loses slightly on
 * storage.
 */
fh_hash_t *fh_PrivTable;

/* XXX: Would be nice to allow client to supply a hash on client_t.
 * However, that currently creates lifetime problems when creating
 * and destroying private_t's.
 */
#ifndef FIREHOSE_HASH_PRIV
  #define FIREHOSE_HASH_PRIV(addr, len) \
	FH_KEYMAKE((addr), ((len) >> FH_BUCKET_SHIFT))
#endif

GASNETI_INLINE(fh_region_to_priv)
firehose_private_t *
fh_region_to_priv(const firehose_region_t *reg)
{
	firehose_private_t *priv;
        fh_int_t key;

        FH_TABLE_ASSERT_LOCKED;

	key = FIREHOSE_HASH_PRIV(reg->addr, reg->len);
        priv = (firehose_private_t *)fh_hash_find(fh_PrivTable, key);

	return priv;
}

/* Given a node and a region_t, create the necessary hash table entries.
 * The FIFO linkage is NOT initialized */
static firehose_private_t * GASNETI_MALLOC
fh_create_priv(gasnet_node_t node, const firehose_region_t *reg)
{
    uintptr_t end_addr, bucket_addr;
    firehose_private_t *priv;
    fh_bucket_t **prev;

    FH_TABLE_ASSERT_LOCKED;

    priv = fhi_priv_freelist;
    if_pt (priv != NULL) {
	fhi_priv_freelist = priv->fh_next;
    }
    else {
        priv = gasneti_malloc(sizeof(firehose_private_t));
    }
    memset(priv, 0, sizeof(firehose_private_t));

    CP_REG_TO_PRIV(priv, node, reg);

    end_addr = fh_region_end(reg);
    prev = &priv->bucket;
    FH_FOREACH_BUCKET(reg->addr, end_addr, bucket_addr) {
        fh_bucket_t *bd = fh_bucket_new();

	bd->priv = priv;
	fh_bucket_hash(bd, FH_KEYMAKE(bucket_addr, node));

	*prev = bd;
	prev= &bd->next;
    }
    *prev = NULL;

    /* Hash the priv IFF local*/
    if_pt (node == gasneti_mynode) {
	/* preserves the ADDR part but invalidates NODE */
	priv->fh_key = FIREHOSE_HASH_PRIV(reg->addr, reg->len);
	gasneti_assert(fh_hash_find(fh_PrivTable, priv->fh_key) == NULL);
	fh_hash_insert(fh_PrivTable, priv->fh_key, priv);
	gasneti_assert(fh_hash_find(fh_PrivTable, priv->fh_key) == priv);
    }

    return priv;
}

static void
fh_destroy_priv(firehose_private_t *priv)
{
    fh_bucket_t *bucket;
    gasnet_node_t node;

    /* Unhash & free all the buckets */
    bucket = priv->bucket;
    gasneti_assert(bucket != NULL);
    node = FH_NODE(bucket);
    do {
	fh_bucket_t *next = bucket->next;
        fh_bucket_unhash(bucket);
	fh_bucket_free(bucket);
	bucket = next;
    } while (bucket != NULL);

    /* Unhash the priv IFF local*/
    if_pt (node == gasneti_mynode) {
	gasneti_assert(fh_hash_find(fh_PrivTable, priv->fh_key) == priv);
	fh_hash_insert(fh_PrivTable, priv->fh_key, NULL);
	gasneti_assert(fh_hash_find(fh_PrivTable, priv->fh_key) == NULL);
    }

    priv->fh_next = fhi_priv_freelist;
    fhi_priv_freelist = priv;
}

/* Given an existing private_t and a region_t, change the necessary hash
 * table entries.
 */
static void
fh_update_priv(firehose_private_t *priv, const firehose_region_t *reg)
{
    uintptr_t bucket_addr;
    uintptr_t old_start, new_start;
    uintptr_t old_end, new_end;
    gasnet_node_t node = FH_NODE(priv);	/* safe because priv is remote */
    fh_bucket_t *bucket;
    fh_bucket_t **prev;

    FH_TABLE_ASSERT_LOCKED;

    old_start = FH_BADDR(priv);
    old_end = fh_priv_end(priv);
    new_start = reg->addr;
    new_end = fh_region_end(reg);

    if_pt ((old_start == new_start) && (old_end == new_end)) {
	FH_CP_CLIENT(priv, reg);
	return;		/* nothing else needs to change */
    }

#if 0
    bucket = fh_bucket_lookup(node, new_start);
    if (bucket && (fh_bucket_end(bucket) == new_end)) {
	fprintf(stderr, "%d> exact match %p %p (%d, %p, %dk)\n",
		gasneti_mynode, bucket->priv, priv, node, (void *)reg->addr, (int)(reg->len)/1024);
    }
#endif

    /* updates fh_key, len and client */
    CP_REG_TO_PRIV(priv, node, reg);

    /* Create new hash entries for "prefix" */
    bucket = priv->bucket;
    if (new_start < old_start) {
        prev = &priv->bucket;
        FH_FOREACH_BUCKET(new_start, old_start-1, bucket_addr) {
            fh_bucket_t *bd = fh_bucket_new();

	    bd->priv = priv;
	    fh_bucket_hash(bd, FH_KEYMAKE(bucket_addr, node));

	    *prev = bd;
	    prev = &bd->next;
        }
	*prev = bucket;
    }

    /* Rehash existing buckets as needed */
    do {
	fh_bucket_rehash(bucket);
	prev = &bucket->next;
	bucket = bucket->next;
    } while (bucket != NULL);

    /* Create new hash entries for "suffix" */
    if (new_end > old_end) {
	FH_FOREACH_BUCKET(old_end+1, new_end, bucket_addr) {
            fh_bucket_t *bd = fh_bucket_new();

	    bd->priv = priv;
	    fh_bucket_hash(bd, FH_KEYMAKE(bucket_addr, node));

	    *prev = bd;
	    prev= &bd->next;
        }
    }

    return;
}

/*
 * Looks for opportunities to merge adjacent pinned regions.
 * The hash table is such that any region completely covered by
 * the new region will no longer get any hits.  So, such regions will
 * eventually end up being recycled from the FIFO.
 *
 * XXX: Dan has noted that if we tracked which pages were *ever* pinned then
 * we could enlarge regions even more agressively, to cover pages that were
 * once used but not recently enough to be in the table.  However, we'd hit
 * problems when glibc's malloc unmaps freed pages.
 */
GASNETI_INLINE(fhi_merge_regions)
void
fhi_merge_regions(firehose_region_t *pin_region)
{
    uintptr_t	addr = pin_region->addr;
    size_t	len  = pin_region->len;
    size_t	space_avail = fhi_MaxRegionSize - len;
    size_t	extend;
    fh_bucket_t *bd;

    gasneti_assert(len <= fhi_MaxRegionSize);

    /* Look to merge w/ successor */
    if_pt (space_avail && (addr + len != 0) /* avoid wrap around */) {
	uintptr_t next_addr = addr + len;
	bd = fh_bucket_lookup(gasneti_mynode, next_addr);
	if (bd != NULL) {
	    uintptr_t end_addr = fh_priv_end(bd->priv) + 1;
	    gasneti_assert(end_addr > next_addr);

#if 1
	    extend = end_addr - next_addr;
	    if (extend <= space_avail) {
	        /* We cover the other region fully */
		len += extend;
		space_avail -= extend;
	    }
#else
	    extend = MIN(end_addr - next_addr, space_avail);
	    len += extend;
	    space_avail -= extend;
#endif
	}
	gasneti_assert(len <= fhi_MaxRegionSize);
    }

    /* Look to merge w/ predecessor */
    if_pt (space_avail && (addr != 0) /* avoid wrap around */) {
	bd = fh_bucket_lookup(gasneti_mynode, addr - FH_BUCKET_SIZE);
	if (bd != NULL) {
	    const firehose_private_t *priv = bd->priv;

	    gasneti_assert(priv != NULL);
	    gasneti_assert(fh_priv_end(priv) >= (addr - 1));
	    gasneti_assert(fh_priv_end(priv) < (addr + (len - 1)));

#if 1
	    extend = addr - FH_BADDR(priv);
	    if (extend <= space_avail) {
	        /* We cover the other region fully */
	        addr -= extend;
	        len += extend;
	        space_avail -= extend;
	    }
#else
	    extend = MIN(addr - FH_BADDR(priv), space_avail);
	    addr -= extend;
	    len += extend;
	    space_avail -= extend;
#endif
	}
	gasneti_assert(len <= fhi_MaxRegionSize);
    }

    pin_region->addr = addr;
    pin_region->len  = len;
}

/* Spin to get some more space */
GASNETI_INLINE(fhi_wait_for_one)
void
fhi_wait_for_one(const firehose_private_t *priv) {
	firehose_region_t unpin_regions[FH_MAX_UNPIN_LOC];
	int num_unpin;

	/* Verify the state is what we think it is */
	gasneti_assert(FH_BUCKET_REFC(priv)->refc_r > 0);
	gasneti_assert(FH_BUCKET_REFC(priv)->refc_l == 0);
	gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL == 0);

	num_unpin = fh_WaitLocalFirehoses(1, unpin_regions);
	if (num_unpin) {
	    gasneti_assert(num_unpin == 1);
	    num_unpin += fh_clean_covered_local(FH_MAX_UNPIN_LOC - num_unpin, unpin_regions + num_unpin);
	    FH_TABLE_UNLOCK;
	    firehose_move_callback(gasneti_mynode, unpin_regions, 1, NULL, 0);
	    FH_TABLE_LOCK;
	}
#ifdef DEBUG_LOCAL_TABLE
	--fhc_LocalReserved;
#endif
	fhc_LocalOnlyBucketsPinned--;
	gasneti_assert(FHC_MAXVICTIM_BUCKETS_AVAIL > 0);
}

/* add a locally pinned region to the tables */
GASNETI_INLINE(fhi_init_local_region)
firehose_private_t *
fhi_init_local_region(int local_ref, firehose_region_t *region)
{
    firehose_private_t *priv;

    gasneti_assert(region != NULL);
    gasneti_assert((local_ref == 0) || (local_ref == 1));

    priv = fh_region_to_priv(region);
    if_pf (priv != NULL) {
	/* We lost a race and somebody else created our region for us */

	if_pf (local_ref && !FH_IS_READY(1, priv)) {
	    /* The matching region currently has only remote referrences and
	     * we'd overcommit the local-only limit if we acquired it locally.
	     */
	
	    /* Spin to acquire 1 region of space */
	    fhi_wait_for_one(priv);

	    /* We have space, but the region we hit might not exist anymore. */
	    priv = fh_region_to_priv(region);
	}

	if (priv) {
	    gasneti_assert(!local_ref || FH_IS_READY(1, priv));

	    /* 1) acquire the existing pinning before releasing the lock */
	    fh_priv_acquire_local(local_ref, priv);

	    /* 2) release the duplicate we've created */
            FH_TABLE_UNLOCK;
            firehose_move_callback(gasneti_mynode, region, 1, NULL, 0);
            FH_TABLE_LOCK;

	    if (local_ref) {
	        /* 3a) correct resource count */
	        fhc_LocalOnlyBucketsPinned--;
	    }
	    else {
	        /* 3b) ensure region has correct client_t */
	        FH_CP_CLIENT(region, priv);
	    }

	    return priv;
	}
    }

    /* We get here if there was no duplicate entry, or if there was one
     * which disappeared while we polled for additional resources.
     */

    /* Create the new table entries w/ proper ref counts */
    priv = fh_create_priv(gasneti_mynode, region);
    FH_BSTATE_SET(priv, fh_used);
    FH_SET_LOCAL_INUSE(priv);
    FH_BUCKET_REFC(priv)->refc_l = local_ref;
    FH_BUCKET_REFC(priv)->refc_r = !local_ref;
    FH_TRACE_BUCKET(priv, INIT);

    return priv;
}

/* Lookup a region, returning the coresponding priv if found, else NULL.
 */
GASNETI_INLINE(fhi_find_priv)
firehose_private_t *
fhi_find_priv(gasnet_node_t node, uintptr_t addr, size_t len)
{
    firehose_private_t *priv = NULL;
    fh_bucket_t *bd;
		
    FH_TABLE_ASSERT_LOCKED;

    bd = fh_bucket_lookup(node, addr);

    if_pt (bd && ((addr + (len - 1)) <= fh_bucket_end(bd))) {
	/* Firehose HIT */
	priv = bd->priv;
    }

    return priv;
}

/*
 * fh_FreeVictim(count, region_array, head)
 *
 * This function removes 'count' firehoses from the victim FIFO (local or
 * remote), and fills the region_array with regions suitable for move_callback.
 * It returns the amount of regions (not buckets) created in the region_array.
 *
 * NOTE: it is up to the caller to make sure the region array is large enough.
 */
int
fh_FreeVictim(int count, firehose_region_t *reg, fh_fifoq_t *fifo_head)
{
    firehose_private_t	*priv;
    int i;

    FH_TABLE_ASSERT_LOCKED;

    gasneti_assert(count > 0);

    for (i = 0; i < count; ++i) {
	/* There must be enough buckets in the victim FIFO to unpin.  This
	 * criteria should always hold true per the constraints on
	 * fhc_LocalOnlyBucketsPinned. */
	gasneti_assert(!FH_TAILQ_EMPTY(fifo_head));

	/* Now do the real work */
	priv = FH_TAILQ_FIRST(fifo_head);
	FH_TAILQ_REMOVE(fifo_head, priv);
	CP_PRIV_TO_REG(reg+i, priv);
	FH_TRACE_BUCKET(priv, REMFIFO);
	fh_destroy_priv(priv);
    }

    return count;
}

/* ##################################################################### */
/* PINNING QUERIES                                                       */
/* ##################################################################### */

/* If entire region is pinned then return non-zero.
   Note that we are counting on lookup giving the match with greatest
   forward extent.
   We must take care to disqualify regions which cannot be acquired
   without blocking (the FH_IS_READY test).
 */
int
fh_region_ispinned(gasnet_node_t node, uintptr_t addr, size_t len)
{
    fh_bucket_t *bd;
    int retval = 0;

    FH_TABLE_ASSERT_LOCKED;

    bd = fh_bucket_lookup(node, addr);

    if_pt (bd &&
	   FH_IS_READY(node == gasneti_mynode, bd->priv) &&
	   ((addr + (len - 1)) <= fh_bucket_end(bd))) {
	fhi_lookup_cache = bd->priv;
	retval = 1;
    }

    return retval;
}

/* If any part of region is pinned then update region and return non-zero.
   We must take care to disqualify regions which cannot be acquired
   without blocking (the FH_IS_READY test).
*/
int
fh_region_partial(gasnet_node_t node, uintptr_t *addr_p, size_t *len_p)
{
    uintptr_t start_addr, end_addr, bucket_addr;
    int is_local = (node == gasneti_mynode);
    int retval = 0;

    FH_TABLE_ASSERT_LOCKED;

    start_addr = *addr_p;
    end_addr = start_addr + (*len_p - 1);

    FH_FOREACH_BUCKET(start_addr, end_addr, bucket_addr) {
        fh_bucket_t *bd = fh_bucket_lookup(node, bucket_addr);

	if (bd && FH_IS_READY(is_local, bd->priv)) {
	    *addr_p = FH_BADDR(bd->priv);
	    *len_p  = bd->priv->len;
	    fhi_lookup_cache = bd->priv;
	    retval = 1;
	    break;
	}
    }

    return retval;
}

/* ##################################################################### */
/* LOCAL PINNING                                                         */
/* ##################################################################### */
void
fh_acquire_local_region(firehose_request_t *req)
{
    firehose_private_t *priv;
#if GASNET_DEBUG
    int loop_count = 0;
#endif

    gasneti_assert(req != NULL);
    gasneti_assert(req->node == gasneti_mynode);
    gasneti_assert(req->len <= fhi_MaxRegionSize);

    FH_TABLE_ASSERT_LOCKED;

retry:
    priv = fhi_find_priv(gasneti_mynode, req->addr, req->len);
    if_pf (priv == NULL) {
	/* Firehose MISS, now must pin it */
	firehose_region_t pin_region;
        firehose_region_t unpin_regions[FH_MAX_UNPIN_LOC];
	int num_unpin;

	pin_region.addr = req->addr;
	pin_region.len  = req->len;

	fhi_merge_regions(&pin_region);

	num_unpin = fh_WaitLocalFirehoses(1, unpin_regions);
	gasneti_assert ((num_unpin == 0) || (num_unpin == 1));

	/* unpin !visible regions on the Fifo */
	num_unpin += fh_clean_covered_local(FH_MAX_UNPIN_LOC - num_unpin, unpin_regions + num_unpin);

	FH_TABLE_UNLOCK;
	firehose_move_callback(gasneti_mynode,
				unpin_regions, num_unpin,
				&pin_region, 1);
	FH_TABLE_LOCK;

	priv = fhi_init_local_region(1, &pin_region);
#ifdef DEBUG_LOCAL_TABLE
	--fhc_LocalReserved;
#endif
	GASNETI_TRACE_EVENT(C,FH_LOCAL_MISS);
    }
    else if_pf (!FH_IS_READY(1, priv)) {
	/* We hit, but the region currently has only remote referrences and
	 * we'd overcommit the local-only limit if we acquired it locally.
	 */
	
	/* Spin to acquire 1 region of space */
	fhi_wait_for_one(priv);

	/* We have space now, but the region we hit might not exist anymore.
	 * So, we start over.  Note we can only restart ONCE.
	 */
	gasneti_assert(!loop_count++);
	goto retry;
    }
    else {
	/* HIT on a region we can acquire w/o blocking */
	fh_priv_acquire_local(1, priv);
	GASNETI_TRACE_EVENT(C,FH_LOCAL_HIT);
    }

    CP_PRIV_TO_REQ(req, priv);

    return;
}

void
fh_commit_try_local_region(firehose_request_t *req)
{
    firehose_private_t *priv;

    gasneti_assert(req != NULL);
    gasneti_assert(req->node == gasneti_mynode);

    FH_TABLE_ASSERT_LOCKED;

    /* We *MUST* be commiting the most recent lookup */
    priv = fhi_lookup_cache;
    gasneti_assert(priv != NULL);
    gasneti_assert(req->addr >= FH_BADDR(priv));
    gasneti_assert(fh_req_end(req) <= fh_priv_end(priv));

    /* We must be able to acquire w/o overcommiting the FIFO */
    gasneti_assert(FH_IS_READY(1, priv));

    fh_priv_acquire_local(1, priv);
    CP_PRIV_TO_REQ(req, priv);
}

void
fh_release_local_region(firehose_request_t *request)
{
        FH_TABLE_ASSERT_LOCKED;
	gasneti_assert(request != NULL);
	gasneti_assert(request->node == gasneti_mynode);
	gasneti_assert(request->internal != NULL);

	fh_priv_release_local(1, request->internal);
	fh_AdjustLocalFifoAndPin(gasneti_mynode, NULL, 0);

	return;
}

/* ##################################################################### */
/* REMOTE PINNING                                                        */
/* ##################################################################### */

static void
fhi_hang_callback(firehose_private_t *priv, firehose_request_t *req, 
		  firehose_completed_fn_t callback, void * context)
{
    fh_completion_callback_t *ccb = fh_alloc_completion_callback();

    ccb->flags = FH_CALLBACK_TYPE_COMPLETION;
    ccb->request = req;
    ccb->callback = callback;
    ccb->context = context;

    /* CSB: Make sure the end of the list contains a completion end tag */
    if ((void *)priv->fh_tqe_next == NULL)
	ccb->fh_tqe_next = FH_COMPLETION_END;
    else
	ccb->fh_tqe_next = (fh_completion_callback_t *) priv->fh_tqe_next;

    gasneti_assert(ccb->fh_tqe_next != NULL);
    priv->fh_tqe_next = (firehose_private_t *) ccb;
                                                                                                              
    gasneti_assert(req->internal == NULL);
    req->internal = (firehose_private_t *) ccb;

    FH_TRACE_BUCKET(priv, PENDADD);
                                                                                                              
    GASNETI_TRACE_PRINTF(C, ("Firehose Pending ADD priv=%p "
                             "(%p,%d), req=%p", (void *) priv,
                             (void *) FH_BADDR(priv),
			     (int) FH_NODE(priv->bucket),
                             (void *) req));

    req->flags |= FH_FLAG_PENDING;
}

void
fh_acquire_remote_region(firehose_request_t *req, 
		         firehose_completed_fn_t callback, void * context,
                         uint32_t flags,
                         firehose_remotecallback_args_fn_t args_fn)
{
    firehose_private_t *priv;
    gasnet_node_t node;

    gasneti_assert(req != NULL);
    gasneti_assert(req->node != gasneti_mynode);
    gasneti_assert(req->len <= fhi_MaxRegionSize);

    node = req->node;

    FH_TABLE_ASSERT_LOCKED;

    priv = fhi_find_priv(node, req->addr, req->len);
    if_pf (priv == NULL) {
	/* MISS */
    	char payload[(1+FH_MAX_UNPIN_REM)*sizeof(firehose_region_t) +
		     sizeof(firehose_remotecallback_args_t)];
    	firehose_region_t *pin_region = (firehose_region_t *)payload;
    	firehose_region_t *unpin_regions = pin_region + 1;
	size_t payload_size = sizeof(firehose_region_t);
	int num_unpin = 0;

	pin_region->addr = req->addr;
	pin_region->len  = req->len;

	/* Acquire resources for the pinning */
	if_pt (fhc_RemoteBucketsM > fhc_RemoteBucketsUsed[node]) {
	    fhc_RemoteBucketsUsed[node]++;
	}
	else {
	    num_unpin = fh_WaitRemoteFirehoses(node, 1, unpin_regions);
	    gasneti_assert(num_unpin == 1);
	}
	gasneti_assert ((num_unpin == 0) || (num_unpin == 1));

	/* unpin !visible regions on the Fifo */
	num_unpin += fh_clean_covered_remote(node, FH_MAX_UNPIN_REM - num_unpin, unpin_regions + num_unpin);

	payload_size += num_unpin * sizeof(firehose_region_t);

	/* Create the "pending bucket" */
        priv = fh_create_priv(node, pin_region);
        FH_BSTATE_SET(priv, fh_pending);
        FH_SET_REMOTE_PENDING(priv);
	FH_BUCKET_REFC(priv)->refc_r = 1;
        FH_TRACE_BUCKET(priv, INIT);

	fhi_hang_callback(priv, req, callback, context);
                                                                                                              
	/* Assemble AM payload */
	if (flags & FIREHOSE_FLAG_ENABLE_REMOTE_CALLBACK) {
	    payload_size += args_fn(context,
			            (firehose_remotecallback_args_t *)(payload + payload_size));
	    gasneti_assert(payload_size <= gasnet_AMMaxMedium());
	}

	FH_TABLE_UNLOCK;

	#ifdef FIREHOSE_UNBIND_CALLBACK
	if (num_unpin)
	    firehose_unbind_callback(node, unpin_region, 1);
	#endif

	req->flags |= FH_FLAG_INFLIGHT;

	MEDIUM_REQ(4,5,
		   (node,
		    fh_handleridx(fh_am_move_reqh),
		    payload,
		    payload_size,
		    flags,
		    1,
		    num_unpin,
		    PACK(priv)));
    }
    else if_pf (!FH_IS_READY(0, priv)) {
	/* HIT Pending */
	fhi_hang_callback(priv, req, callback, context);
	FH_BUCKET_REFC(priv)->refc_r++;
	gasneti_assert(FH_BUCKET_REFC(priv)->refc_r > 0);
	FH_TRACE_BUCKET(priv, PENDING);
	FH_TABLE_UNLOCK;
    }
    else {
	/* "pure" HIT */
	fh_priv_acquire_remote(node, priv);
	CP_PRIV_TO_REQ(req, priv);
	req->flags |= FH_FLAG_PINNED;
	FH_TABLE_UNLOCK;
    }

    FH_TABLE_ASSERT_UNLOCKED;
}

void
fh_commit_try_remote_region(firehose_request_t *req)
{
    firehose_private_t *priv;

    gasneti_assert(req != NULL);
    gasneti_assert(req->node != gasneti_mynode);

    FH_TABLE_ASSERT_LOCKED;

    /* We *MUST* be commiting the most recent lookup */
    priv = fhi_lookup_cache;
    gasneti_assert(priv != NULL);
    gasneti_assert(req->addr >= FH_BADDR(priv));
    gasneti_assert(fh_req_end(req) <= fh_priv_end(priv));

    fh_priv_acquire_remote(req->node, priv);
    CP_PRIV_TO_REQ(req, priv);
}

void
fh_release_remote_region(firehose_request_t *request)
{
        FH_TABLE_ASSERT_LOCKED;
	gasneti_assert(request != NULL);
	gasneti_assert(request->node != gasneti_mynode);
	gasneti_assert(request->internal != NULL);
	gasneti_assert(!FH_IS_REMOTE_PENDING(request->internal));

	fh_priv_release_remote(request->node, request->internal);

        gasneti_assert(fhc_RemoteVictimFifoBuckets[request->node]
                        <= fhc_RemoteBucketsM);

	return;
}

/*
 * This function is called by the Firehose reply once a firehose request to pin
 * functions covered into a region completes.
 *
 * The function identifies the private_t's that were marked as 'pending'.
 * These 'pending buckets' may or may not have requests associated to them.
 * If they do, then requests pending a callback are added to a list to be run.
 *
 * The function returns the amount of callbacks that were added to the list of
 * pending requests pointing to the 'PendQ' parameter.
 */
int
fh_find_pending_callbacks(gasnet_node_t node, firehose_region_t *region,
			  int nreg, void *context, fh_pollq_t *PendQ)
{
	firehose_private_t		*priv = context;
	fh_completion_callback_t	*ccb;
	int		callspend = 0;

	FH_TABLE_ASSERT_LOCKED;

	/* Sanity checks */
	gasneti_assert(priv != NULL);
	gasneti_assert(node != gasneti_mynode);
	gasneti_assert(node == FH_NODE(priv));
	gasneti_assert(nreg == 1);

	FH_STAILQ_INIT(PendQ);

	/* Make sure the private_t was set as pending */
	gasneti_assert(FH_IS_REMOTE_PENDING(priv));
	FH_BSTATE_ASSERT(priv, fh_pending);

	/* Update priv to reflect the reply */
	fh_update_priv(priv, region);

	/* Now make the private_t not pending */
	FH_BSTATE_SET(priv, fh_used);

	/* Queue the callbacks */
	ccb = (fh_completion_callback_t *) priv->fh_tqe_next;
	gasneti_assert(ccb != NULL);

	while (ccb != FH_COMPLETION_END) {
		firehose_request_t		*req;
		fh_completion_callback_t	*next;

		gasneti_assert(ccb->flags & FH_CALLBACK_TYPE_COMPLETION);

		next = FH_STAILQ_NEXT(ccb);
		req = ccb->request;

		gasneti_assert(req && (req->flags & FH_FLAG_PENDING));
		req->flags &= ~FH_FLAG_PENDING;
		req->internal = priv;
    		CP_PRIV_TO_REQ(req, priv);

		FH_STAILQ_INSERT_TAIL(PendQ, (fh_callback_t *) ccb);
		GASNETI_TRACE_PRINTF(C,
			    ("Firehose Pending Request (%p,%d) "
			     "enqueued %p for callback", 
			     (void *) req->addr, (int) req->len, (void *) req));
		callspend++;

		ccb = next;
	}

	FH_SET_REMOTE_INUSE(priv);

	return callspend;
}

/* ##################################################################### */
/* INITIALIZATION & FINALIZATION                                         */
/* ##################################################################### */

/*
 * XXX:
 * We are constrained in two directions: limits on pages & regions
 * For now we are going to take the easy way out.  Since the code
 * inherited from firehose-page counts the number of private_t's
 * (which are pinned regions for us), we'll just use that single
 * limit.  We then set the limits in fhinfo such that the products
 * of terms will fit the memory limits:
 *	max_LocalRegions  * max_LocalPinSize  <= MAXVICTIM_M
 *	max_RemoteRegions * max_RemotePinSize <= M / (N-1)
 * As with firehose-page, the prepinned regions are counted against
 * the local regions.
 */
void
fh_init_plugin(uintptr_t max_pinnable_memory,
               size_t max_regions, size_t max_region_size,
               const firehose_region_t *regions, size_t num_prepinned,
	       firehose_info_t *fhinfo)
{
	unsigned long param_M, param_VM;
	unsigned long param_R, param_VR;
	unsigned long param_RS;
	int i, j;
	unsigned long firehoses, m_prepinned;
#if 0  /* UNUSED - see param_RS computation for explanation */
	int med_regions;
#endif
	int b_prepinned = 0;
	int num_nodes = gasneti_nodes;
	int dflt_M, dflt_VM;
	int dflt_R, dflt_VR;
	int dflt_RS;

        /* Initialize the Bucket tables */
        fh_BucketTable1 = fh_hash_create(1<<16); /* 64k */
        fh_BucketTable2 = fh_hash_create(1<<17); /* 128k */

#if 0  /* UNUSED - see param_RS computation for explanation */
	/* Count how many regions fit into an AM Medium payload */
	med_regions = (gasnet_AMMaxMedium() 
				- sizeof(firehose_remotecallback_args_t))
				/ sizeof(firehose_region_t);
	gasneti_assert(med_regions > FH_MAX_UNPIN_REM); /* firehose_remotecallback_args_t too big? */
#endif

	/*
	 * Prepin optimization: PHASE 1.
	 *
	 * Count the number of buckets that are set as prepinned.
	 *
	 */
	for (i = 0; i < num_prepinned; i++) {
		b_prepinned += FH_NUM_BUCKETS(regions[i].addr,regions[i].len);
	}
	m_prepinned = FH_BUCKET_SIZE * b_prepinned;

	/* Get limits from the environment */
	param_M  = fh_getenv("GASNET_FIREHOSE_M", (1<<20));
	dflt_M   = !param_M;
	param_VM = fh_getenv("GASNET_FIREHOSE_MAXVICTIM_M", (1<<20));
	dflt_VM  = !param_VM;
	param_R  = fh_getenv("GASNET_FIREHOSE_R", 1);
	dflt_R   = !param_R;
	param_VR = fh_getenv("GASNET_FIREHOSE_MAXVICTIM_R", 1);
	dflt_VR  = !param_VR;
	param_RS = fh_getenv("GASNET_FIREHOSE_MAXREGION_SIZE", (1<<20));
	dflt_RS  = !param_RS;
	GASNETI_TRACE_PRINTF(C, 
	    ("ENV: Firehose M=%ld, MAXVICTIM_M=%ld", param_M, param_VM));
	GASNETI_TRACE_PRINTF(C, 
	    ("ENV: Firehose R=%ld, MAXVICTIM_R=%ld", param_R, param_VR));
	GASNETI_TRACE_PRINTF(C, 
	    ("ENV: Firehose max region size=%ld", param_RS));

	/* Now assign decent "M" defaults based on physical memory */
	if (param_M == 0 && param_VM == 0) {
		if ((fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
			param_M  = m_prepinned;
			param_VM = max_pinnable_memory - param_M;
		} else {
			param_M  = (unsigned long) max_pinnable_memory *
				(1-FH_MAXVICTIM_TO_PHYSMEM_RATIO);
			param_VM = (unsigned long) max_pinnable_memory *
				    FH_MAXVICTIM_TO_PHYSMEM_RATIO;
		}
	}
	else if (param_M == 0)
		param_M = max_pinnable_memory - param_VM;
	else if (param_VM == 0)
		param_VM = max_pinnable_memory - param_M;

	if (param_RS == 0) {
		if ((fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
			param_RS = max_region_size;
		} else {
/* We don't (yet) do accounting against param_R, so the worst case
 * is unpining FH_MAX_UNPIN_REM regions, rather than param_RS/FH_BUCKET_SIZE.
 */
#if 0
			/* We always send one AM to pin one region.  So, we need to
			 * have enough room AM to encode the requested region plus
			 * some number of regions to unpin.  In the worst case, the
			 * regions selected for replacement will be single-bucket
			 * sized (the minimum possible).
			 * So, we require param_RS <= (med_regions-1)*FH_BUCKET_SIZE
			 */
			param_RS = MIN(max_region_size,
				       (med_regions-1)*FH_BUCKET_SIZE);
#else
			param_RS = max_region_size;
#endif
		}
	}
	/* Round down to multiple of FH_BUCKET_SIZE for sanity */
	param_RS &= ~FH_PAGE_MASK;


	/* Try to work it all out with the given RS
 	 * The goal is (currently) to honor the given region size and
         * reduce the number of available regions as needed.
	 */
	if (param_R == 0 && param_VR == 0) {
		if ((fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
			param_R  = num_prepinned;
			param_VR = max_regions - param_R;
		} else {
			double ratio;

			/* try naively... */
			param_R  = (param_M - m_prepinned)  / param_RS;
			param_VR = param_VM / param_RS;
			
			/* then rescale if needed */
			ratio = (max_regions - num_prepinned) /
					(double)(param_R + param_VR);
			if (ratio < 1.) {
				param_R  *= ratio;
				param_VR *= ratio;
			}
		}
	}
	else if (param_R == 0)
		param_R  = max_regions - num_prepinned - param_VR;
	else if (param_VR == 0)
		param_VR = max_regions - num_prepinned - param_R;

	/* Trim and eliminate round-off so that limits are self-consistent */
	param_R  = MIN(param_R,  num_prepinned + ((param_M - m_prepinned) / param_RS));
	param_VR = MIN(param_VR, param_VM / param_RS);
	param_M  = param_RS * (param_R - num_prepinned) + m_prepinned;
	param_VM = param_RS * param_VR;

	/* Report final values */
	GASNETI_TRACE_PRINTF(C, ("param_M=%ld param_VM=%ld", param_M, param_VM));
	GASNETI_TRACE_PRINTF(C, ("param_RS=%ld", param_RS));
	GASNETI_TRACE_PRINTF(C, ("param_R=%ld param_VR=%ld", param_R, param_VR));
	gasneti_envint_display("GASNET_FIREHOSE_M", param_M, dflt_M, 1);
	gasneti_envint_display("GASNET_FIREHOSE_MAXVICTIM_M", param_VM, dflt_VM, 1);
	gasneti_envint_display("GASNET_FIREHOSE_R", param_R, dflt_R, 0);
	gasneti_envint_display("GASNET_FIREHOSE_MAXVICTIM_R", param_VR, dflt_VR, 0);
	gasneti_envint_display("GASNET_FIREHOSE_MAXREGION_SIZE", (int)param_RS, dflt_RS, 1);

	/* 
	 * Validate firehose parameters parameters 
	 */ 
	if ((fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
		/* Want at least 4k buckets of victim FIFO */
		unsigned long	VM_min = FH_BUCKET_SIZE * 4096;

		/* Want at least 32 regions of FIFO */
		unsigned long	VR_min = 32;

		if_pf (param_RS < FH_BUCKET_SIZE)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXREGION_SIZE (%ld) "
			    "is less than the minimum %d",
			    param_RS, FH_BUCKET_SIZE); 

		if_pf (param_VM < VM_min)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_M (%ld) is less than "
			    "the minimum %ld (%ld buckets)",
			    param_VM, VM_min, VM_min >> FH_BUCKET_SHIFT);

		if_pf (param_VR < VR_min)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_R (%ld) is less than "
			    "the minimum %ld", param_VR, VR_min);

		if_pf (param_M < m_prepinned)	/* XXX: need this check? */
			gasneti_fatalerror("Too many bytes in initial"
			    " pinned regions list (%ld) for current "
			    "GASNET_FIREHOSE_M parameter (%ld)", 
			    m_prepinned, param_M);

		if_pf (param_R < num_prepinned)	/* XXX: need this check? */
			gasneti_fatalerror("Too many regions passed on initial"
			    " pinned bucket list (%ld) for current "
			    "GASNET_FIREHOSE_R parameter (%ld)", 
			    (unsigned long)num_prepinned, param_R);
	} else {
		/* Want at least 32 buckets per node */
		unsigned long	M_min = FH_BUCKET_SIZE * num_nodes * 32;

		/* Want at least 256 buckets of victim FIFO */
		unsigned long	VM_min = FH_BUCKET_SIZE * 256;

		/* Want at least 1 region per node */
		/* XXX/PHH THIS IS REALLY A BARE MINIMUM */
		unsigned long	R_min = num_nodes;

		/* Want at least 2 regions of FIFO */
		/* XXX/PHH THIS IS REALLY A BARE MINIMUM */
		unsigned long	VR_min = 2;

		if_pf (param_RS < FH_BUCKET_SIZE)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXREGION_SIZE (%ld) "
			    "is less than the minimum %d",
			    param_RS, FH_BUCKET_SIZE); 

/* We don't (yet) do accounting against param_R, so the worst case
 * is unpining FH_MAX_UNPIN_REM regions, rather than param_RS/FH_BUCKET_SIZE.
 */
#if 0
		if_pf (param_RS > (med_regions-1)*FH_BUCKET_SIZE)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXREGION_SIZE (%ld) "
			    "is too large to encode in an AM Medium payload "
			    "(%d bytes max)",
			    param_RS, FH_BUCKET_SIZE*(med_regions-1));
#endif

		if_pf (param_M < M_min)
			gasneti_fatalerror("GASNET_FIREHOSE_M (%ld) is less "
			    "than the minimum %ld (%ld buckets)",
			    param_M, M_min, M_min >> FH_BUCKET_SHIFT);

		if_pf (param_VM < VM_min)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_M (%ld) is less than "
			    "the minimum %ld (%ld buckets)",
			    param_VM, VM_min, VM_min >> FH_BUCKET_SHIFT);

		if_pf (param_M - m_prepinned < M_min)
			gasneti_fatalerror("Too many bytes in initial"
			    " pinned regions list (%ld) for current "
			    "GASNET_FIREHOSE_M parameter (%ld)", 
			    m_prepinned, param_M);

		if_pf (param_R < R_min)
			gasneti_fatalerror("GASNET_FIREHOSE_R (%ld) is less"
			    "than the minimum %ld", param_R, R_min);

		if_pf (param_VR < VR_min)
			gasneti_fatalerror("GASNET_FIREHOSE_MAXVICTIM_R (%ld) is less than "
			    "the minimum %ld", param_VR, VR_min);

		if_pf (param_R - num_prepinned < R_min)
			gasneti_fatalerror("Too many regions passed on initial"
			    " pinned bucket list (%ld) for current "
			    "GASNET_FIREHOSE_R parameter (%ld)", 
			    (unsigned long)num_prepinned, param_R);
	}

	/* 
	 * Set local parameters
	 */
	fhc_LocalOnlyBucketsPinned = 0;
	fhc_LocalVictimFifoBuckets = 0;
	fhc_MaxVictimBuckets = num_prepinned + param_VR;
	fhi_MaxRegionSize = param_RS;


	/* 
	 * Set remote parameters
	 */
	firehoses = MIN(param_R, (param_M - m_prepinned) / param_RS);
	if (!(fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
		fhc_RemoteBucketsM = num_nodes > 1
				? firehoses / (num_nodes - 1)
				: firehoses;
	}

	GASNETI_TRACE_PRINTF(C, 
		    ("Maximum pinnable=%ld\tMax allowed=%ld", 
		     (firehoses + param_VR) * param_RS + m_prepinned,
		     (unsigned long)max_pinnable_memory));
	gasneti_assert((firehoses + param_VR) * param_RS + m_prepinned
						<= max_pinnable_memory);


	/* Allocate hash table for region tracking */
	i = 1.2 * (param_R + param_VR + num_prepinned);	/* factor 1.2 is arbitrary */
	/* round 'i' up to a power of two: */
	for (j = 1; j < i; j *= 2) { /* nothing */ }
       	fh_PrivTable = fh_hash_create(j);


	/*
	 * Prepin optimization: PHASE 2.
	 *
	 * In this phase, the firehose parameters have been validated and the
	 * buckets are added to the firehose table and sent to the FIFO.
	 */
	for (i = 0; i < num_prepinned; i++) {
		firehose_private_t *priv;
		firehose_region_t *tmp;
	       
		/* We can safely discard the const qualifier, we know
		 * fhi_init_local_region won't actually modify the region.
		 */
		tmp = (firehose_region_t *)&(regions[i]);
		priv = fhi_init_local_region(1, tmp);
		#ifdef DEBUG_BUCKETS
		priv->prepinned = 1;
		#endif
		fhc_LocalOnlyBucketsPinned++;
		#if 0
		  fh_priv_release_local(1, priv);
		#endif
	}


	/* 
	 * Set fields in the firehose information type, according to the limits
	 * established by the firehose parameters.
	 */
	{
		fhinfo->max_LocalRegions  = param_VR;
		fhinfo->max_LocalPinSize  = param_RS;
		if ((fhi_InitFlags & FIREHOSE_INIT_FLAG_LOCAL_ONLY)) {
			fhc_MaxRemoteBuckets = 0;
			fhinfo->max_RemoteRegions = 0;
			fhinfo->max_RemotePinSize = 0;
		} else {
			fhc_MaxRemoteBuckets = param_RS >> FH_BUCKET_SHIFT;
			fhinfo->max_RemoteRegions = fhc_RemoteBucketsM;
			fhinfo->max_RemotePinSize = param_RS;
		}

		GASNETI_TRACE_PRINTF(C, 
		    ("Firehose M=%ld (fh=%ld)\tprepinned=%ld (buckets=%d)",
		    param_M, firehoses, m_prepinned, b_prepinned));
		GASNETI_TRACE_PRINTF(C, ("Firehose Maxvictim=%ld (fh=%d)",
		    param_VM, fhc_MaxVictimBuckets));

		GASNETI_TRACE_PRINTF(C, 
		    ("MaxLocalPinSize=%d\tMaxRemotePinSize=%d", 
		    (int) fhinfo->max_LocalPinSize,
                    (int) fhinfo->max_RemotePinSize));
		GASNETI_TRACE_PRINTF(C, 
		    ("MaxLocalRegions=%d\tMaxRemoteRegions=%d", 
		    (int) fhinfo->max_LocalRegions,
                    (int) fhinfo->max_RemoteRegions));
	}

	return;
}

static void
fh_priv_print_fn(void *val, void *arg)
{
	firehose_private_t *priv = val;
	int lref = FH_IS_LOCAL_FIFO(priv) ? 0 : FH_BUCKET_REFC(priv)->refc_l;
	int rref = FH_IS_LOCAL_FIFO(priv) ? 0 : FH_BUCKET_REFC(priv)->refc_r;

	fprintf(stderr, "[n%d] %p - %p (%4d of %4d pages visible) refc=(%4dL, %4dR)\n",
			(int)gasneti_mynode, (void*)FH_BADDR(priv), (void*)fh_priv_end(priv),
			priv->visible, (int)(priv->len>>FH_BUCKET_SHIFT),
			lref, rref);
}

#ifdef DEBUG_BUCKETS
static void
fh_priv_check_fn(void *val, void *arg)
{
	fh_bucket_t *bucket = val;
	firehose_private_t *priv = bucket->priv;
	int live = (FH_NODE(bucket) == gasneti_mynode)
			? (!FH_IS_LOCAL_FIFO(priv) && FH_BUCKET_REFC(priv)->refc_l)
			: (!FH_IS_REMOTE_FIFO(priv) && FH_BUCKET_REFC(priv)->refc_r);

	if_pf (live && !priv->prepinned) {
		/* XXX: promote to fatalerror? */
		fprintf(stderr, "WARNING: firehose leak detected on node %d - %d:%p %4d pages (%4d pages visible)\n",
			(int)gasneti_mynode, (int)FH_NODE(bucket), (void*)FH_BADDR(priv), 
			(int)(priv->len>>FH_BUCKET_SHIFT), priv->visible);
		priv->prepinned = 1; /* Avoids duplicates in output */
	}
}
#endif

static void
fh_priv_cleanup_fn(void *val, void *arg)
{
	firehose_private_t *priv = val;

	if (FH_IS_LOCAL_FIFO(priv) || !FH_BUCKET_REFC(priv)->refc_l) {
		firehose_region_t unpin_region;
		CP_PRIV_TO_REG(&unpin_region, priv);
		FH_TABLE_UNLOCK;
		firehose_move_callback(gasneti_mynode, &unpin_region, 1, NULL, 0);
		FH_TABLE_LOCK;
	} else {
		/* Indicates pre-pinned (or a leak!!) */
	}
	fh_destroy_priv(priv);
}

void
fh_fini_plugin(void)
{
	firehose_private_t *priv;

	if (fh_verbose) {
		/* Dump the local table, unsorted */
		FH_TABLE_LOCK;
		fprintf(stderr, "[n%d] Final local firehose table:\n", gasneti_mynode);
		fh_hash_apply(fh_PrivTable, &fh_priv_print_fn, NULL);
		FH_TABLE_UNLOCK;
	}

#ifdef DEBUG_BUCKETS
	/* Check the hash table for leaks */
	FH_TABLE_LOCK;
	fh_hash_apply(fh_BucketTable1, &fh_priv_check_fn, NULL);
	fh_hash_apply(fh_BucketTable2, &fh_priv_check_fn, NULL);
	FH_TABLE_UNLOCK;
#endif

	if (fhi_InitFlags & FIREHOSE_INIT_FLAG_UNPIN_ON_FINI) {
		/* Unpin and free everything we pinned: */
		FH_TABLE_LOCK;
		fh_hash_apply(fh_PrivTable, &fh_priv_cleanup_fn, NULL);
		FH_TABLE_UNLOCK;
	}

        fh_hash_destroy(fh_BucketTable2);
        fh_hash_destroy(fh_BucketTable1);
        fh_hash_destroy(fh_PrivTable);

	priv = fhi_priv_freelist;
	while (priv != NULL) {
		firehose_private_t *next = priv->fh_next;
		gasneti_free(priv);
		priv = next;
	}
}

/* ##################################################################### */
/* ACTIVE MESSAGES                                                       */
/* ##################################################################### */

int
fh_move_request(gasnet_node_t node,
		firehose_region_t *new_reg, size_t r_new,
		firehose_region_t *old_reg, size_t r_old,
		void *context)
{
	firehose_private_t	*priv;
	int			num_pin = 0;
	int			i;

	FH_TABLE_LOCK;

	GASNETI_TRACE_PRINTF(C, ("Firehose move request: new=%d, old=%d",
                                 (int) r_new, (int)r_old));

	gasneti_assert(r_new == 1);	/* a feature of FIREHOSE_REGION */

	priv = fhi_find_priv(gasneti_mynode, new_reg->addr, new_reg->len);
	if_pf (priv == NULL) {
		/* MISSED in table */
		fhi_merge_regions(new_reg);
		num_pin = 1;
	}
	else {
		/* HIT in table */
		fh_priv_acquire_local(0, priv);
		CP_PRIV_TO_REG(new_reg, priv);
	}

	GASNETI_TRACE_PRINTF(C, ("Firehose move request: pin new=%d",
				 num_pin));

	/* Release the "old" regions, potentially overcommiting the FIFO */
	for (i=0; i < r_old; ++i) {
		fh_priv_release_local(0, fh_region_to_priv(&(old_reg[i])));
	}

	/* Pin the region if needed and fix any FIFO overcommit */
	fh_AdjustLocalFifoAndPin(node, new_reg, num_pin);

	/* Finish table entry for newly pinned region */
	if_pf (num_pin) {
		(void)fhi_init_local_region(0, new_reg);
	}

	FH_TABLE_UNLOCK;

	return 1;
}

/*
 * Detailed, time-consuming validation of local table state.
 */
#ifdef DEBUG_LOCAL_TABLE
int fhc_LocalReserved = 0;

static void _fhi_acct_fifo_fn(void *key, void *val)
{ firehose_private_t *priv = key;
  int *count_p = val;
  if (FH_IS_LOCAL_FIFO(priv)) (*count_p)++;
}

static void _fhi_acct_lref_fn(void *key, void *val)
{ firehose_private_t *priv = key;
  int *count_p = val;
  fh_refc_t *rp = FH_BUCKET_REFC(priv);
  if (!FH_IS_LOCAL_FIFO(priv) && rp->refc_l) { (*count_p)++; }
}

void _fhi_debug_local_table(const char *func, int line)  {
  int lonly, lref_count, fifo_count;
  firehose_private_t *priv;
  int err = 0;

  if_pf(!fh_PrivTable) return 0;

  /* Count FIFO length (method 1) */
  fifo_count = 0;
  for (priv = FH_TAILQ_FIRST(&fh_LocalFifo); priv; priv = FH_TAILQ_NEXT(priv)) { ++fifo_count; }
  if (fifo_count != fhc_LocalVictimFifoBuckets) {
    fprintf(stderr, "[%d] %s:%d fhc_LocalVictimFifoBuckets (%d) != FIFO entries (%d)\n",
		    gasneti_mynode, func, line, fhc_LocalVictimFifoBuckets, fifo_count);
    fflush(NULL);
    ++err;
  }

  /* Count FIFO length (method 2) */
  fifo_count = 0;
  fh_hash_apply(fh_PrivTable, &_fhi_acct_fifo_fn, &fifo_count);
  if (fifo_count != fhc_LocalVictimFifoBuckets) {
    fprintf(stderr, "[%d] %s:%d fhc_LocalVictimFifoBuckets (%d) != in-FIFO hash entries (%d)\n",
		    gasneti_mynode, func, line, fhc_LocalVictimFifoBuckets, fifo_count);
    fflush(NULL);
    ++err;
  }

  /* Count LocalOnly buckets */
  lref_count = 0;
  fh_hash_apply(fh_PrivTable, &_fhi_acct_lref_fn, &lref_count);
  lonly = fhc_LocalReserved + fifo_count + lref_count;
  if (lonly != fhc_LocalOnlyBucketsPinned) {
    fprintf(stderr, "[%d] %s:%d fhc_LocalOnlyBucketsPinned (%d) != FIFO + local-ref'd + resv'd (%d + %d + %d = %d)\n",
		    gasneti_mynode, func, line, fhc_LocalOnlyBucketsPinned,
		    fifo_count, lref_count, fhc_LocalReserved, lonly);
    fflush(NULL);
    ++err;
  }

  if (err) gasneti_fatalerror("(see above)");
}
#endif

#endif
