/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_legacy.c $
 * Description: GASNet-1 legacy API support layer
 * Copyright 2018, The Regents of the University of California.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>

// remove safety checks that protect the g2ex data elsewhere
#undef gasneti_thunk_client
#undef gasneti_thunk_endpoint
#undef gasneti_thunk_tm
#undef gasneti_thunk_segment

gex_Client_t      gasneti_thunk_client   = NULL;
gex_EP_t          gasneti_thunk_endpoint = NULL;
gex_TM_t          gasneti_thunk_tm       = NULL;
gex_Segment_t     gasneti_thunk_segment  = NULL;

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

