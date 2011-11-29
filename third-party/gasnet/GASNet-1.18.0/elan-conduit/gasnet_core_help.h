/*   $Source: /var/local/cvs/gasnet/elan-conduit/gasnet_core_help.h,v $
 *     $Date: 2006/03/19 02:07:58 $
 * $Revision: 1.7 $
 * Description: GASNet elan conduit core Header Helpers (Internal code, not for client use)
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

#define GASNETI_EADDRFMT "0x%08x"
#define GASNETI_EADDRSTR(ptr) ((uint32_t)(uintptr_t)(ptr))

GASNETI_END_EXTERNC

#endif
