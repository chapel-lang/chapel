#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef SIMPLE_TEST
#include "chplrt.h"
#endif

#include "qio_plugin_lustre.h"

qioerr sys_lustre_get_stripe(qio_file_t* file, int64_t* size_out) {
  chpl_internal_error("No Lustre Support");
  return 0;
}
