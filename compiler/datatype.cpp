#include "datatype.h"

int dtIsDomain(Sym* pdt) {
  if (pdt == NULL || pdt->name == NULL) {
    INT_FATAL(NULL, "bad datatype symbol received in dtIsDomain");
  } else if (strcmp(pdt->name, "sequence") == 0) {
    return 1;
  }
  return 0;
}
