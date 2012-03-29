#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "chplio.h"
#include "chplrt.h"



char* chpl_refToString(void* ref) {
  char buff[32];
  sprintf(buff, "%p", ref);
  return chpl_glom_strings(1, buff);
}


typedef struct _chpl_wide_voidStar {
  int32_t locale;
  void* addr;
} chpl_wide_voidStar;


char* chpl_wideRefToString(void* wideref) {
  char buff[32];
  int32_t locale = ((chpl_wide_voidStar*)wideref)->locale;
  void* ref = ((chpl_wide_voidStar*)wideref)->addr;
  sprintf(buff, "%" PRId32 ":%p", locale, ref);
  return chpl_glom_strings(1, buff);
}
