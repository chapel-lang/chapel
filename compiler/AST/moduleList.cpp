#include "chplenum.h"
#include "moduleList.h"


ModuleSymbol* ModuleList::filteredNext(void) {
  switch (filter) {
  case MODULES_ALL:
    return cursor;
  case MODULES_USER:
    while (cursor && cursor->modtype != MOD_USER) {
      cursor = AList<ModuleSymbol>::next();
    }
    return cursor;
  case MODULES_CODEGEN:
    while (cursor && cursor->modtype == MOD_INTERNAL) {
      cursor = AList<ModuleSymbol>::next();
    }
    return cursor;
  default:
    INT_FATAL(this, "Unhanded module filter");
    return NULL;
  }
}


ModuleSymbol* ModuleList::first(void) {
  cursor = AList<ModuleSymbol>::first();
  return filteredNext();
}


ModuleSymbol* ModuleList::next(void) {
  if (cursor == NULL) {
    return NULL;
  } else {
    cursor = AList<ModuleSymbol>::next();
    return filteredNext();
  }
}
