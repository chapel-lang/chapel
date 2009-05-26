#ifndef _stdchplrt_H_
#define _stdchplrt_H_

/* This is similar to stdchpl.h, but pared down for the runtime
   Chapel code -- in particular, things like chplcgfns.h are not
   needed for the generated runtime code, and cause problems. */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <chplio_md.h>

#include "chplcast.h"
#include "chplio.h"
#include "chpl_mem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "chpltypes.h"
#include "error.h"

#endif
