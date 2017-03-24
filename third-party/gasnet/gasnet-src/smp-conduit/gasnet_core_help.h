/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_core_help.h $
 * Description: GASNet smp conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

#include <gasnet_help.h>

#define GASNETC_MAX_ARGS   16

#if GASNET_PSHM
  #define GASNETC_MAX_MEDIUM MIN(65536, GASNETI_MAX_MEDIUM_PSHM)
#else
  #define GASNETC_MAX_MEDIUM 65536  /* limited only by buffering constraints */
#endif

#define GASNETC_MAX_LONG   ((size_t)0x7fffffff) /* unlimited */

#endif
