/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/gasnet_core_internal.h $
 * Description: GASNet libfabric (OFI) conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2015, Intel Corporation
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
#define _hidx_gasnetc_exit_reqh               (GASNETC_HANDLER_BASE+1)
/* add new core API handlers here and to the bottom of gasnet_core.c */

#ifndef GASNETE_HANDLER_BASE
  #define GASNETE_HANDLER_BASE  64 /* reserve 64-127 for the extended API */
#elif GASNETE_HANDLER_BASE != 64
  #error "GASNETE_HANDLER_BASE mismatch between core and extended"
#endif

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

#if GASNET_PAR
#define GASNETC_OFI_LOCK() gasneti_spinlock_lock(&gasnetc_ofi_state.lock);
#define GASNETC_OFI_TRYLOCK() gasneti_spinlock_trylock(&gasnetc_ofi_state.lock);
#define GASNETC_OFI_UNLOCK() gasneti_spinlock_unlock(&gasnetc_ofi_state.lock);
#else
#define GASNETC_OFI_LOCK()
#define GASNETC_OFI_TRYLOCK()
#define GASNETC_OFI_UNLOCK()
#endif

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

typedef struct _gasnetc_ofi_state {
	gasneti_atomic_t	lock;
	/* more stuff will be migrated here */
} gasnetc_ofi_state_t;

extern gasnetc_ofi_state_t gasnetc_ofi_state;

#endif
