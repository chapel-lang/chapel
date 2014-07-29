#include "chplrt.h"

#include "chplio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


chpl_string chpl_refToString(void* ref) {
  char buff[32];
  sprintf(buff, "%p", ref);
  return string_copy(buff, 0, NULL);
}


chpl_string chpl_wideRefToString(c_nodeid_t node, void* addr) {
  char buff[32];
  sprintf(buff, "%" FORMAT_c_nodeid_t ":%p", node, addr);
  return string_copy(buff, 0, NULL);
}
