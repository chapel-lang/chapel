#include "chplrt.h"
#include "chplcomm_locales.h"
#include "error.h"

int _chpl_comm_default_num_locales(void) {
  return 1;
}

void _chpl_comm_verify_num_locales(int32_t proposedNumLocales) {
  if (proposedNumLocales != 1) {
    chpl_error("Only 1 locale may be used for CHPL_COMM layer 'none'\n"
               "To use multiple locales, see $CHPL_HOME/doc/README.multilocale",
               0, 0);
  }
}
