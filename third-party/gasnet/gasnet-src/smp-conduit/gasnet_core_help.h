/*   $Source: bitbucket.org:berkeleylab/gasnet.git/smp-conduit/gasnet_core_help.h $
 * Description: GASNet smp conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

#include <gasnet_help.h>

#define GASNETC_MAX_ARGS   16

#define GASNETC_MAX_MEDIUM MIN(65536, GASNETC_MAX_MEDIUM_NBRHD_DFLT)

#define GASNETC_MAX_LONG   ((size_t)0x7fffffff) /* unlimited */

#endif
