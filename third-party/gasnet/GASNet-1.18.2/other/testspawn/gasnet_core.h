/*   $Source: /var/local/cvs/gasnet/other/testspawn/gasnet_core.h,v $
 *     $Date: 2005/04/18 00:31:55 $
 * $Revision: 1.3 $
 * Description:
 * Copyright 2005, Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_H
#define _GASNET_CORE_H

#include <gasnet_core_help.h>

#if GASNETI_STATS_OR_TRACE
  #define GASNETC_LOCK_STAT_INIT ,0 
#else
  #define GASNETC_LOCK_STAT_INIT  
#endif

#define GASNET_HSL_INITIALIZER { \
  GASNETI_MUTEX_INITIALIZER      \
  GASNETC_LOCK_STAT_INIT         \
  }

#define GASNET_BLOCKUNTIL(cond)		gasneti_polluntil(cond)
#define gasnet_hold_interrupts()	do {} while (0)
#define gasnet_resume_interrupts()	do {} while (0)
#define gasnet_AMMaxArgs()		16
#define gasnet_AMMaxMedium()		512
#define gasnet_AMMaxLongRequest()	512
#define gasnet_AMMaxLongReply()		512
#define gasnet_exit(X)			do {} while (0)

#endif
