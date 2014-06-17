/*   $Source: /var/local/cvs/gasnet/vapi-conduit/gasnet_core_help.h,v $
 *     $Date: 2006/03/19 02:08:30 $
 * $Revision: 1.12 $
 * Description: GASNet vapi conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

GASNETI_BEGIN_EXTERNC

#include <gasnet_help.h>

#if defined(GASNETC_PTHREAD_CREATE_OVERRIDE)
  #if !defined(PTHREAD_MUTEX_INITIALIZER)
    #error "Pthread support missing or broken"
  #endif
  extern int gasnetc_pthread_create(gasneti_pthread_create_fn_t *, pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
#endif

#if defined(GASNET_SEGMENT_FAST)
  #define GASNETC_PIN_SEGMENT 1
#else
  #define GASNETC_PIN_SEGMENT 0
#endif

GASNETI_END_EXTERNC

#endif
