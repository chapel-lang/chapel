#include "chplrt.h"

#include "chplfp.h"
#include "chpl-mem.h"
#include "chpl-mem-desc.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chpl-comm-compiler-macros.h"
#include "error.h"

#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int64_t real2int( _real64 f) {
  // need to use a union here rather than a pointer cast to avoid gcc
  // warnings when compiling -O3
  union {
    _real64 r;
    uint64_t u;
  } converter;

  converter.r = f;
  return converter.u;
}


int64_t
object2int( _chpl_object o) {
  return (intptr_t) o;
}

/* This should be moved somewhere else, but where is the questions */
const char* chpl_get_argument_i(chpl_main_argument* args, int32_t i)
{
  if( i < 0 ) return NULL;
  if( i > args->argc ) return NULL;
  return args->argv[i];
}

