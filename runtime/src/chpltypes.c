#include <stdlib.h>
#include <string.h>
#include "chpltypes.h"

void _copy_string(_string* lhs, _string rhs) {
  *lhs = (char*)realloc(*lhs, (strlen(rhs)+1)*sizeof(char));
  strcpy(*lhs, rhs);
}
