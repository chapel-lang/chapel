
#include "TestExport.h"

extern int64_t chpl_helper(void);

int64_t helper(void) {
  return chpl_helper();
}
