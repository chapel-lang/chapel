#include "chplrt.h"

#include "chplio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


char* chpl_refToString(void* ref) {
  char buff[32];
  sprintf(buff, "%p", ref);
  return chpl_glom_strings(1, buff);
}


char* chpl_wideRefToString(int32_t locale, void* addr) {
  char buff[32];
  sprintf(buff, "%" PRId32 ":%p", locale, addr);
  return chpl_glom_strings(1, buff);
}
