/*   $Source: /var/local/cvs/gasnet/other/testspawn/gasnet_extended_fwd.h,v $
 *     $Date: 2005/02/12 11:29:29 $
 * $Revision: 1.2 $
 * Description:
 * Copyright 2005, Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_EXTENDED_FWD_H
#define _GASNET_EXTENDED_FWD_H

#define GASNET_EXTENDED_VERSION      1.4
#define GASNET_EXTENDED_VERSION_STR  _STRINGIFY(GASNET_EXTENDED_VERSION)
#define GASNET_EXTENDED_NAME         TESTSPAWN
#define GASNET_EXTENDED_NAME_STR     _STRINGIFY(GASNET_EXTENDED_NAME)

#define GASNET_BEGIN_FUNCTION() do {} while(0)

#endif

