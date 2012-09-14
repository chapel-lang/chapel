/*   $Source: /var/local/cvs/gasnet/elan-conduit/gasnet_core_toolhelp.h,v $
 *     $Date: 2006/05/23 12:42:17 $
 * $Revision: 1.3 $
 * Description: GASNet header for elan conduit core (gasnet_tools helper)
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_TOOLHELP_H
#define _GASNET_CORE_TOOLHELP_H

/* use ELAN-specific high-performance nanosecond timer -
   currently only a win on Alpha, where the native timer support is poor
 */
#if PLATFORM_ARCH_ALPHA || defined(GASNETC_FORCE_ELAN_TIMERS)
  #define GASNETC_CONDUIT_SPECIFIC_TIMERS
  typedef uint64_t gasneti_tick_t;
  extern uint64_t gasnetc_clock();
  #define gasneti_ticks_now()      (gasnetc_clock())
  #define gasneti_ticks_to_ns(st)  (st)
#endif


#endif /* _GASNET_CORE_TOOLHELP_H */
