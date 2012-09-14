/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_firehose.c,v $
 *     $Date: 2009/08/03 00:49:22 $
 * $Revision: 1.16 $
 * Description: Client-specific firehose code
 * Copyright 2003, E. O. Lawrence Berekely National Laboratory
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

extern int
firehose_move_callback(gasnet_node_t node,
                       const firehose_region_t *unpin_list,
                       size_t unpin_num,
                       firehose_region_t *pin_list,
                       size_t pin_num)
#if FIREHOSE_VAPI_USE_FMR
{
    GASNETC_TRACE_WAIT_BEGIN();
    int vstat;
    EVAPI_fmr_map_t map;
    EVAPI_fmr_hndl_t *handles;
    int repin_num;
    int h, i;

    map.page_array_len = 0;

    /* Perform all the unpins with a single unmap call per HCA: */
    if (unpin_num) {
      handles = alloca(unpin_num * sizeof(EVAPI_fmr_hndl_t));
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        for (i = 0; i < unpin_num; ++i) {
	  if (h == 0) { GASNETC_TRACE_UNPIN(&unpin_list[i]); }
	  handles[i] = unpin_list[i].client.handle[h];
        }
        vstat = EVAPI_unmap_fmr(gasnetc_hca[h].handle, unpin_num, handles);
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_unmap_fmr");
      }
    }

    /* Reuse the unmapped FMRs where possible */
    repin_num = MIN(unpin_num, pin_num);
    for (i = 0; i < repin_num; i++) {
      GASNETC_FOR_ALL_HCA_INDEX(h) {
	pin_list[i].client.handle[h] = unpin_list[i].client.handle[h];
      }
    }
    
    /* Destroy excess FMRs (if any) */
    for (i = repin_num; i < unpin_num; i++) {
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        vstat = EVAPI_free_fmr(gasnetc_hca[h].handle, unpin_list[i].client.handle[h]);
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_free_fmr");
      }
    }

    /* Allocate more FMRs (if needed) */
    for (i = repin_num; i < pin_num; i++) {
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        vstat = EVAPI_alloc_fmr(gasnetc_hca[h].handle, &gasnetc_hca[h].fmr_props,
			        &(pin_list[i].client.handle[h]));
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_alloc_fmr");
      }
    }

    /* Now perform all the mappings */
    for (i = 0; i < pin_num; i++) {
      firehose_region_t *region = pin_list + i;

      gasneti_assert(region->addr % GASNET_PAGESIZE == 0);
      gasneti_assert(region->len % GASNET_PAGESIZE == 0);

      map.start = (uintptr_t)region->addr;
      map.size  = region->len;
      GASNETC_FOR_ALL_HCA_INDEX(h) {
        vstat = EVAPI_map_fmr(gasnetc_hca[h].handle, region->client.handle[h], &map,
			      &(region->client.lkey[h]), &(region->client.rkey[h]));
        GASNETC_VAPI_CHECK(vstat, "from EVAPI_map_fmr");
      }
      GASNETC_TRACE_PIN(&pin_list[i]);
    }

    GASNETC_TRACE_WAIT_END(FIREHOSE_MOVE);
    return 0;
}
#elif GASNET_CONDUIT_VAPI
{
    GASNETC_TRACE_WAIT_BEGIN();
    int           vstat;
    VAPI_mr_t     mr_in;
    int repin_num;
    int h, i;

    mr_in.type    = VAPI_MR;
    mr_in.acl     = VAPI_EN_LOCAL_WRITE |
		    VAPI_EN_REMOTE_WRITE |
		    VAPI_EN_REMOTE_READ;

    repin_num = MIN(unpin_num, pin_num);

    /* Take care of any unpairable unpins first */
    for (i = repin_num; i < unpin_num; i++) {
      GASNETC_FOR_ALL_HCA_INDEX(h) {
	vstat = VAPI_deregister_mr(gasnetc_hca[h].handle,
				   unpin_list[i].client.handle[h]);
        GASNETC_VAPI_CHECK(vstat, "from VAPI_deregister_mr");
      }
      GASNETC_TRACE_UNPIN(&unpin_list[i]);
    }

    /* Perform replacements where possible */
    for (i = 0; i < repin_num; i++) {
      firehose_region_t *region = pin_list + i;
      firehose_client_t *client = &region->client;
      VAPI_mr_t mr_out;

      gasneti_assert(region->addr % GASNET_PAGESIZE == 0);
      gasneti_assert(region->len % GASNET_PAGESIZE == 0);

      mr_in.start = (uintptr_t)region->addr;
      mr_in.size  = region->len;

      GASNETC_FOR_ALL_HCA_INDEX(h) {
	mr_in.pd_hndl = gasnetc_hca[h].pd;

	vstat = VAPI_reregister_mr(gasnetc_hca[h].handle,
				   unpin_list[i].client.handle[h],
				   VAPI_MR_CHANGE_TRANS,
				   &mr_in, &client->handle[h], &mr_out);
        GASNETC_VAPI_CHECK(vstat, "from VAPI_reregister_mr");

	client->lkey[h]     = mr_out.l_key;
	client->rkey[h]     = mr_out.r_key;
      }
      GASNETC_TRACE_UNPIN(&unpin_list[i]);
      GASNETC_TRACE_PIN(&pin_list[i]);
    }

    /* Take care of any unpairable pins */
    for (i = repin_num; i < pin_num; i++) {
      firehose_region_t *region = pin_list + i;
      firehose_client_t *client = &region->client;
      VAPI_mr_t mr_out;
    
      gasneti_assert(region->addr % GASNET_PAGESIZE == 0);
      gasneti_assert(region->len % GASNET_PAGESIZE == 0);
    
      mr_in.start = (uintptr_t)region->addr;
      mr_in.size  = region->len;
    
      GASNETC_FOR_ALL_HCA_INDEX(h) {
	mr_in.pd_hndl = gasnetc_hca[h].pd;

	vstat = VAPI_register_mr(gasnetc_hca[h].handle, &mr_in, &client->handle[h], &mr_out);
        GASNETC_VAPI_CHECK(vstat, "from VAPI_register_mr");
    
	client->lkey[h]     = mr_out.l_key;
	client->rkey[h]     = mr_out.r_key;
      }
      GASNETC_TRACE_PIN(&pin_list[i]);
    }

    GASNETC_TRACE_WAIT_END(FIREHOSE_MOVE);
    return 0;
}
#elif GASNET_CONDUIT_IBV
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
	rc = ibv_dereg_mr(unpin_list[i].client.handle[h]);
        GASNETC_VAPI_CHECK(rc, "from ibv_dereg_mr");
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
	if_pf (client->handle[h] == NULL) gasneti_fatalerror("ibv_reg_mr failed in firehose_move_callback");
    
	client->lkey[h]     = client->handle[h]->lkey;
	client->rkey[h]     = client->handle[h]->rkey;
      }
      GASNETC_TRACE_PIN(&pin_list[i]);
    }

    GASNETC_TRACE_WAIT_END(FIREHOSE_MOVE);
    return 0;
}
#else
  #error "Unknown IB API"
#endif

extern int
firehose_remote_callback(gasnet_node_t node,
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
	gasneti_assert(args->len <= gasnetc_putinmove_limit);
	memcpy(args->addr, args->data, args->len);
	gasneti_sync_writes();
	return 0;
    #endif
}
