#ifndef _SUN_STDINT_H_
#define _SUN_STDINT_H_

// The existence of this file is a stupid hack because our Suns don't
// seem to support stdint.h

#include <inttypes.h>

//
// our Suns don't define these macros, so we'll do it here, even
// though it would be more appropriate in inttypes.h
//

#ifndef SCNd8
#define SCNd8 "hhd"
#endif

#ifndef SCNu8
#define SCNu8 "hhu"
#endif

#endif
