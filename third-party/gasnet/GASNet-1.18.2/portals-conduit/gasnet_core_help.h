/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_core_help.h,v $
 *     $Date: 2007/08/26 06:01:24 $
 * $Revision: 1.3 $
 * Description: GASNet PORTALS conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

/* Set the CHUNKSIZE for the Portals Conduit here, its value will be
 * used in the conduit-specific header files as well as in gasnet_core.h.
 * This value determines the max size of an AM Medium.  It also determine
 * the minimum number of credit each node must be allocated.  The larger
 * the value, the more space that must be allocated to AM Send and Receive buffers
 * which will impact the memory footprint of large process-count jobs.
 * Values of 1024 and 2048 have been tested.
 * See gasnet_portals.h for additional information.
 */
#define GASNETC_CHUNKSIZE 2048

GASNETI_BEGIN_EXTERNC

#include <gasnet_help.h>

GASNETI_END_EXTERNC

#endif
