#include "chplrt.h"
#include "chplcomm_locales.h"
#include "error.h"

int _chpl_comm_default_num_locales(void) {
  chpl_error("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
}


void _chpl_comm_verify_num_locales(int32_t proposedNumLocales) {
}
