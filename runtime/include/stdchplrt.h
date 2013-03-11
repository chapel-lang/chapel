#ifndef _stdchplrt_H_
#define _stdchplrt_H_

/* This is similar to stdchpl.h, but pared down for the runtime
   Chapel code -- in particular, things like chplcgfns.h are not
   needed for the generated runtime code, and cause problems. */

#include "chplrt.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "chplcast.h"
#include "chplio.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpltimers.h"
#include "chpltypes.h"
#include "error.h"

#ifndef __cplusplus
#include "chplgmp.h"
#endif

#endif
