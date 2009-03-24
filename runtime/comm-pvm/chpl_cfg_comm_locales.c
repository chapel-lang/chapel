#include "chplrt.h"
#include "chplcomm_locales.h"
#include "error.h"

int _chpl_comm_default_num_locales(void) {
#if defined(GASNET_CONDUIT_PORTALS)
  return _numLocales;
#else
  chpl_error("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
#endif
}


void _chpl_comm_verify_num_locales(int32_t proposedNumLocales) {
}
