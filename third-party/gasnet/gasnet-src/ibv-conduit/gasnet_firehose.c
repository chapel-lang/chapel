/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/gasnet_firehose.c $
 * Description: Client-specific firehose code
 * Copyright 2003, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

/* Implement client-specific callbacks for use by firehose-region */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_extended_internal.h>

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_TRACE_MR(_event, _verb, _region) do {                  \
	const firehose_region_t *_reg = (_region);                       \
	int _pages = (int)(_reg->len/GASNET_PAGESIZE);                   \
	GASNETI_TRACE_PRINTF(D, ("FIREHOSE_MOVE: " _STRINGIFY(_verb)     \
				 " %d page(s) at " GASNETI_LADDRFMT,     \
				 _pages, GASNETI_LADDRSTR(_reg->addr))); \
	GASNETC_STAT_EVENT_VAL(_event, _pages);                          \
  } while(0)
  #define GASNETC_TRACE_PIN(_region)	GASNETC_TRACE_MR(FIREHOSE_PIN, pin, (_region))
  #define GASNETC_TRACE_UNPIN(_region)	GASNETC_TRACE_MR(FIREHOSE_UNPIN, unpin, (_region))
#else
  #define GASNETC_TRACE_PIN(_region) 	((void)0)
  #define GASNETC_TRACE_UNPIN(_region) 	((void)0)
#endif

static int
region_is_mapped(firehose_region_t *region)
{
  // Consider a region to be mapped unless msync() fails with ENOMEM
  // Inspired by https://renatocunha.com/2015/12/msync-pointer-validity/
  // TODO: any better method(s) available?
  int ret = msync((void*)(uintptr_t)region->addr, region->len, MS_ASYNC);
  return !(ret && (ENOMEM == errno));
}

extern int
firehose_move_callback(gex_Rank_t node,
                       const firehose_region_t *unpin_list,
                       size_t unpin_num,
                       firehose_region_t *pin_list,
                       size_t pin_num)
{
    GASNETC_TRACE_WAIT_BEGIN();
    int    rc;
    int h, i;

    const enum ibv_access_flags access = (enum ibv_access_flags)
	       (IBV_ACCESS_LOCAL_WRITE |
		IBV_ACCESS_REMOTE_WRITE |
		IBV_ACCESS_REMOTE_READ);


    /* Take care of any unpins first */
    for (i = 0; i < unpin_num; i++) {
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        if (!unpin_list[i].client.handle[h]) {
          gasneti_assert(!h); // If this is not the first HCA then something is very wrong
          break;
        }
	rc = ibv_dereg_mr(unpin_list[i].client.handle[h]);
        GASNETC_IBV_CHECK(rc, "from ibv_dereg_mr");
      }
      GASNETC_TRACE_UNPIN(&unpin_list[i]);
    }

    /* Take care of any pins */
    for (i = 0; i < pin_num; i++) {
      firehose_region_t *region = pin_list + i;
      firehose_client_t *client = &region->client;
    
      gasneti_assert(region->addr % GASNET_PAGESIZE == 0);
      gasneti_assert(region->len % GASNET_PAGESIZE == 0);
    
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        client->handle[h] = ibv_reg_mr(gasnetc_hca[h].pd, (void*)(uintptr_t)region->addr, region->len, access);
	if_pf (NULL == client->handle[h]) {
          if ((EFAULT == errno) && region_is_mapped(region)) {
            // Appears to be a failure as would occur with read-only memory (Bug 3338).
            // Continue with a NULL handle and zero lkey/rkey (should fail if used).
            // This results in a cached firehose, to avoid repeated registration failures.
            // Our conduit-specific defn of FH_MAY_MERGE prevents merging with valid regions.
            gasneti_assert(!h); // If this is not the first HCA then something is very wrong
	    GASNETI_TRACE_PRINTF(D, ("FIREHOSE_MOVE: read-only memory found at " GASNETI_LADDRFMT,
				     GASNETI_LADDRSTR(region->addr)));
	    client->lkey[h] = client->rkey[h] = 0;
            break;
          } // else fatal
	  gasneti_fatalerror("ibv_reg_mr failed in firehose_move_callback errno=%d (%s)", errno, strerror(errno));
        }
	client->lkey[h]     = client->handle[h]->lkey;
	client->rkey[h]     = client->handle[h]->rkey;
      }
      GASNETC_TRACE_PIN(&pin_list[i]);
    }

    GASNETC_TRACE_WAIT_END(FIREHOSE_MOVE);
    return 0;
}

extern int
firehose_remote_callback(gex_Rank_t node,
                         const firehose_region_t *pin_list, size_t num_pinned,
                         firehose_remotecallback_args_t *args)
{
    #if GASNETC_PIN_SEGMENT
	/* DO NOTHING.  IF WE GET CALLED WE COMPLAIN. */
	gasneti_fatalerror("invalid attempted to call firehose_remote_callback()");
	return -1;
    #else
	/* Memcpy payload into place */
	gasneti_assert(args != NULL);
	gasneti_assert(args->addr != NULL);
	gasneti_assert(args->len > 0);
	GASNETI_MEMCPY(args->addr, args->data, args->len);
	gasneti_sync_writes();
	return 0;
    #endif
}
