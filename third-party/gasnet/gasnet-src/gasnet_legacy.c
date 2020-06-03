/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_legacy.c $
 * Description: GASNet-1 legacy API support layer
 * Copyright 2018, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

#include <gasnet_am.h>

// remove safety checks that protect the g2ex data elsewhere
#undef gasneti_thunk_client
#undef gasneti_thunk_endpoint
#undef gasneti_thunk_tm
#undef gasneti_thunk_segment

gex_Client_t      gasneti_thunk_client   = NULL;
gex_EP_t          gasneti_thunk_endpoint = NULL;
gex_TM_t          gasneti_thunk_tm       = NULL;
gex_Segment_t     gasneti_thunk_segment  = NULL;

// conduits may override this to relocate the legacy handlers
// Note that legacy handlers are registered conditionally on the use of gasnet_attach or GEX_FLAG_USES_GASNET1
#ifndef GASNETI_LEGACY_HANDLER_BASE
#define GASNETI_LEGACY_HANDLER_BASE 95
#endif

#define _hidx_gasneti_legacy_memset_reqreph  (GASNETI_LEGACY_HANDLER_BASE+0)
static gex_AM_Entry_t *gasneti_legacy_handlers;
static int gasneti_legacy_handlers_registered = 0;

/* ------------------------------------------------------------------------------------ */

extern void gasneti_legacy_alloc_tm_hook(gasneti_TM_t _tm) {
  if (_tm->_flags & GEX_FLAG_USES_GASNET1) {
    gex_TM_t tm = gasneti_export_tm(_tm);
    gex_Client_t client = gex_TM_QueryClient(tm);
    if (gasneti_thunk_client && gasneti_thunk_client != client) {
      const char *name = gex_Client_QueryName(gasneti_thunk_client);
      if (gex_Client_QueryFlags(gasneti_thunk_client) & GASNETI_FLAG_INIT_LEGACY) name = "Legacy gasnet_init() call";   
      gasneti_fatalerror("%s: Only one client per process may use GASNet-1 APIs, and this was already claimed by <%s>", gex_Client_QueryName(client), name);
    }
    // save the GEX objects for use by g2ex, and mark them as used by g2ex
    gasneti_thunk_tm = tm;
    gasneti_thunk_client = client;
    gasneti_import_client(client)->_flags |= GEX_FLAG_USES_GASNET1;
    gasneti_thunk_endpoint = gex_TM_QueryEP(tm);
    gasneti_import_ep(gasneti_thunk_endpoint)->_flags |= GEX_FLAG_USES_GASNET1;

    if (!gasneti_legacy_handlers_registered) {
      int len = 0;
      int numreg = 0;
      while (gasneti_legacy_handlers[len].gex_fnptr) len++; /* calc len */
      if (gasneti_amregister(gasneti_import_ep(gasneti_thunk_endpoint)->_amtbl, gasneti_legacy_handlers, len, 
                              GASNETI_LEGACY_HANDLER_BASE, GASNETI_CLIENT_HANDLER_BASE, 0, &numreg) != GASNET_OK)
         gasneti_fatalerror("Error registering g2ex legacy AM handlers");
      gasneti_assert(numreg == len);
      gasneti_legacy_handlers_registered = 1;
    }
  }
}

extern void gasneti_legacy_attach_checks(int checksegment) {
  gasneti_assert(gasneti_thunk_tm);
  gasneti_assert(gasneti_thunk_endpoint);
  gasneti_assert(gasneti_thunk_client);
  if (checksegment) {
    gasneti_assert(gasneti_thunk_segment);
    gasneti_assert(gex_Segment_QueryFlags(gasneti_thunk_segment)  & GEX_FLAG_USES_GASNET1);
    gasneti_assert(gex_Segment_QueryClient(gasneti_thunk_segment) == gasneti_thunk_client);
  }
  gasneti_assert(gex_TM_QueryFlags(gasneti_thunk_tm) & GEX_FLAG_USES_GASNET1);
  gasneti_assert(gex_TM_QueryEP(gasneti_thunk_tm) == gasneti_thunk_endpoint);
  gasneti_assert(gex_EP_QueryFlags(gasneti_thunk_endpoint) & GEX_FLAG_USES_GASNET1);
  gasneti_assert(gex_EP_QueryClient(gasneti_thunk_endpoint) == gasneti_thunk_client);
  gasneti_assert(gex_Client_QueryFlags(gasneti_thunk_client) & GEX_FLAG_USES_GASNET1);
}

extern void gasneti_legacy_segment_attach_hook(gasneti_EP_t ep) {
  if (ep == gasneti_import_ep(gasneti_thunk_endpoint)) {
     // We just attached a segment to the g2ex EP, set the g2ex thunk segment
     gasneti_assert(ep->_flags & GEX_FLAG_USES_GASNET1);
     ep->_segment->_flags |= GEX_FLAG_USES_GASNET1;
     gasneti_thunk_segment = gasneti_export_segment(ep->_segment);
  }
}

/* ------------------------------------------------------------------------------------ */
// Legacy memset support

GASNETI_INLINE(gasneti_legacy_memset_reqreph_inner)
void gasneti_legacy_memset_reqreph_inner(gex_Token_t token,
  gex_AM_Arg_t val, void *nbytes_arg, void *dest, void *op) {
  size_t nbytes = (uintptr_t)nbytes_arg;
  if (dest && nbytes) {  // Request: memset
    memset(dest, (int)(uint32_t)val, nbytes);
    gasneti_sync_writes();
    gex_AM_ReplyShort(token, gasneti_handleridx(gasneti_legacy_memset_reqreph), 0,
                        0, PACK(0), PACK(0), PACK(op));
  } else { // Reply: completion
    gasneti_assert(!val && !nbytes && !dest);
    gasneti_assert(op);
    if (gasneti_op_is_eop(op)) {
      gasneti_eop_t * eop = op;
      gasneti_eop_markdone(eop);
    } else { 
      gasneti_iop_t * iop = op;
      gasneti_iop_markdone(iop, 1, 0);
    }
  }
}
SHORT_HANDLER(gasneti_legacy_memset_reqreph,4,7,
              (token, a0, UNPACK(a1),      UNPACK(a2),      UNPACK(a3)     ),
              (token, a0, UNPACK2(a1, a2), UNPACK2(a3, a4), UNPACK2(a5, a6)));


#define GASNETI_TRACE_MEMSET(node,dest,val,nbytes) do {                                  \
  GASNETI_TRACE_PRINTF(D,("GASNET_MEMSET: " GASNETI_RADDRFMT" val=%02x nbytes=%" PRIuPTR,\
                          GASNETI_RADDRSTR(gasneti_thunk_tm,(node),(dest)), (val),       \
                          (uintptr_t)(nbytes)));                                         \
  } while(0)

extern gex_Event_t gasneti_legacy_memset_nb(gex_Rank_t node, void *dest, int val, size_t nbytes GASNETI_THREAD_FARG) {
  GASNETI_TRACE_MEMSET(node,dest,val,nbytes); 
  gasneti_assert_reason(gasneti_legacy_handlers_registered, "gasnet_memset* requires gasnet_attach() or GEX_FLAG_USES_GASNET1");
  if_pf (!nbytes) return 0;
  gasneti_boundscheck(gasneti_thunk_tm,node,dest,nbytes);
  GASNETI_CHECKPSHM_MEMSET(gasneti_thunk_tm,node,dest,val,nbytes);
  gasneti_eop_t *eop = gasneti_eop_create(GASNETI_THREAD_PASS_ALONE);

  gex_AM_RequestShort(gasneti_thunk_tm, node, gasneti_handleridx(gasneti_legacy_memset_reqreph), 0,
                      (gex_AM_Arg_t)val, PACK(nbytes), PACK(dest), PACK(eop));

  return gasneti_eop_to_event(eop);
}

extern int gasneti_legacy_memset_nbi(gex_Rank_t node, void *dest, int val, size_t nbytes GASNETI_THREAD_FARG) {
  GASNETI_TRACE_MEMSET(node,dest,val,nbytes); 
  gasneti_assert_reason(gasneti_legacy_handlers_registered, "gasnet_memset* requires gasnet_attach() or GEX_FLAG_USES_GASNET1");
  if_pf (!nbytes) return 0;
  gasneti_boundscheck(gasneti_thunk_tm,node,dest,nbytes);
  GASNETI_CHECKPSHM_MEMSET(gasneti_thunk_tm,node,dest,val,nbytes);
  gasneti_iop_t *iop = gasneti_iop_register(1, 0 GASNETI_THREAD_PASS);

  gex_AM_RequestShort(gasneti_thunk_tm, node, gasneti_handleridx(gasneti_legacy_memset_reqreph), 0,
                      (gex_AM_Arg_t)val, PACK(nbytes), PACK(dest), PACK(iop));

  return 0;
}

static gex_AM_Entry_t _gasneti_legacy_handlers[] = {
  gasneti_handler_tableentry_with_bits(gasneti_legacy_memset_reqreph,4,7,REQREP,SHORT,0),

  GASNETI_HANDLER_EOT
};
static gex_AM_Entry_t *gasneti_legacy_handlers = _gasneti_legacy_handlers;

