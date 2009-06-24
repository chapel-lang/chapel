#ifndef _SUN_STDINT_H_
#define _SUN_STDINT_H_

// The existence of this file is a stupid hack because our Suns don't
// seem to support stdint.h

#include <inttypes.h>

#define PRIxPTR "p"

typedef enum {
  false=0,
  true=1
} _Bool;

#endif
