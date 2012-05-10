#include "arg.h"
#include "chplrt.h"
#include "chpl-comm-locales.h"
#include "error.h"

int64_t chpl_comm_default_num_locales(void) {
#if defined(GASNET_CONDUIT_PORTALS)
  return chpl_numLocales;
#else
  return chpl_specify_locales_error();
#endif
}


void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
}
