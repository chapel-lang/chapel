/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_core_internal.h,v $
 *     $Date: 2010/04/17 02:36:40 $
 * $Revision: 1.12.2.1 $
 * Description: GASNet smp conduit header for internal definitions in Core API
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

#include <gasnet_internal.h>
#include <gasnet_handler.h>

#if GASNET_DEBUG
typedef struct {
  int8_t   isReq; 
  int8_t   handlerRunning; 
  int8_t   replyIssued;    
} gasnetc_bufdesc_t;
#endif

typedef struct {
  uint8_t  requestBuf[GASNETC_MAX_MEDIUM];
  uint8_t  replyBuf[GASNETC_MAX_MEDIUM];
} gasnetc_threadinfo_t;

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* ------------------------------------------------------------------------------------ */
/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2
} gasnetc_category_t;

/* ------------------------------------------------------------------------------------ */
#if GASNETI_CLIENT_THREADS
  #define gasnetc_mythread() ((void**)(gasnete_mythread()))
#else
  extern void *_gasnetc_mythread;
  #define gasnetc_mythread() &_gasnetc_mythread
#endif


#endif
