/*   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/gasnet_core_help.h $
 * Description: GASNet ucx conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright 2019, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNETEX_H
  #error This file is not meant to be included directly- clients should include gasnetex.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

#include <gasnet_help.h>

#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
#define GASNETC_PIN_SEGMENT 1
#else
#define GASNETC_PIN_SEGMENT 0
#endif

#endif
