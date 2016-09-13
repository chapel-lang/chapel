#ifndef _RNG_H
#define _RNG_H

/***********************************************************
 *                                                         *
 *  splitable random number generator to use:              *
 *     (default)  sha1 hash                                *
 *     (UTS_ALFG) additive lagged fibonacci generator      *
 *                                                         *
 ***********************************************************/

#if defined(UTS_ALFG)
#  include "alfg.h"
#  define RNG_TYPE 1
#elif defined(BRG_RNG)
#  include "brg_sha1.h"
#  define RNG_TYPE 0
#elif defined(DEVINE_RNG)
#  include "devine_sha1.h"
#  define RNG_TYPE 0
#else
#  error "No random number generator selected."
#endif

#endif /* _RNG_H */
