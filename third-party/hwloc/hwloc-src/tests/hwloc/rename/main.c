/* don't let assert() stringify some hwloc names that would not
 * renamed afterwards, and wouldn't be easy to filter out */
#define NDEBUG 1

#define _GNU_SOURCE 1
#include "sched.h"

/* all headers should be explicitly included */

/* autoheaders definitions first, to avoid conflicts later */
#include "private/autogen/config.h"
#include "hwloc/autogen/config.h"

#include "hwloc.h" /* hwloc/rename.h required before everything else */
#include "hwloc/bitmap.h"
#include "hwloc/shmem.h"
#include "hwloc/helper.h"
#include "hwloc/plugins.h"

#if HWLOC_TEST_RENAME_LINUX
#include "hwloc/linux.h"
#endif
#if HWLOC_TEST_RENAME_LINUX_LIBNUMA
#include "hwloc/linux-libnuma.h"
#endif
#if HWLOC_TEST_RENAME_GLIBC_SCHED
#include "hwloc/glibc-sched.h"
#endif
#if HWLOC_TEST_RENAME_OPENFABRICS_VERBS
#include "hwloc/openfabrics-verbs.h"
#endif
#if HWLOC_TEST_RENAME_OPENCL
#include "hwloc/opencl.h"
#endif
#if HWLOC_TEST_RENAME_CUDA
#include "hwloc/cuda.h"
#endif
#if HWLOC_TEST_RENAME_CUDART
#include "hwloc/cudart.h"
#endif
#if HWLOC_TEST_RENAME_NVML
#include "hwloc/nvml.h"
#endif
#if HWLOC_TEST_RENAME_RSMI
#include "hwloc/rsmi.h"
#endif
#if HWLOC_TEST_RENAME_LEVELZERO
#include "hwloc/levelzero.h"
#endif
#include "hwloc/gl.h"
#include "hwloc/windows.h"

#include "private/components.h"
#include "private/internal-components.h"
#include "private/cpuid-x86.h"
#include "private/debug.h"
#include "private/misc.h"
#include "private/private.h"
#include "private/solaris-chiptype.h"
#include "private/xml.h"
