#define _GNU_SOURCE 1
#include "sched.h"

#include "hwloc.h"

/* enable those that have the right dependencies on your machine */
#include "hwloc/glibc-sched.h"
#include "hwloc/linux-libnuma.h"
#include "hwloc/linux.h"
//#include "hwloc/cuda.h"
//#include "hwloc/cudart.h"
//#include "hwloc/myriexpress.h"
#include "hwloc/openfabrics-verbs.h"

#include "private/autogen/config.h"
#include "private/cpuid.h"
#include "private/debug.h"
#include "private/misc.h"
#include "private/private.h"
