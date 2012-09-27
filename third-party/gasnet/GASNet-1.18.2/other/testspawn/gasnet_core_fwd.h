/*   $Source: /var/local/cvs/gasnet/other/testspawn/gasnet_core_fwd.h,v $
 *     $Date: 2005/04/14 17:29:17 $
 * $Revision: 1.2 $
 * Description: 
 * Copyright 2005, Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_FWD_H
#define _GASNET_CORE_FWD_H

#define GASNET_CORE_VERSION      1.4
#define GASNET_CORE_VERSION_STR  _STRINGIFY(GASNET_CORE_VERSION)
#define GASNET_CORE_NAME         TESTSPAWN
#define GASNET_CORE_NAME_STR     _STRINGIFY(GASNET_CORE_NAME)

#define _GASNET_NODE_T
typedef uint16_t gasnet_node_t;

#define GASNET_ALIGNED_SEGMENTS	1

#endif
