#include <stdlib.h>
#include <string.h>
#include "chplmem.h"
#include "chplrt.h"
#include "chpltypes.h"

void* nil = (void*)0x01;

void _copy_string(_string* lhs, _string rhs) {
  *lhs = (char*)_chpl_realloc(*lhs, (strlen(rhs)+1), sizeof(char), "string");
  strcpy(*lhs, rhs);
}
