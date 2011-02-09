#include "arg.h"
#include "chplrt.h"
#include "chpl-comm-locales.h"
#include "error.h"

int chpl_comm_default_num_locales(void) {
  return chpl_specify_locales_error();
}


void chpl_comm_verify_num_locales(int32_t proposedNumLocales) {
}
