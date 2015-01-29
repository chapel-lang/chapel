#include "chplrt.h"
#include "chpl-comm-locales.h"
#include "error.h"

int64_t chpl_comm_default_num_locales(void) {
  return 1;
}

void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
  if (proposedNumLocales != 1) {
    chpl_error("Only 1 locale may be used for CHPL_COMM layer 'none'\n"
               "To use multiple locales, see $CHPL_HOME/doc/README.multilocale",
               0, 0);
  }
}
