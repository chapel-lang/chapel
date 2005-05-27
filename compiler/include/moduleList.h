#ifndef _MODULELIST_H_
#define _MODULELIST_H_

#include "alist.h"
#include "symbol.h"


// BLC: This class is introduced in order to break a circular
// dependence in which alist.h needs to include traversal.h to support
// traversals, and pass/traversal.h need to include alist.h in order
// to support AList<ModuleSymbol>s.
//
// We use a ModuleSymbol* so that a single ModuleSymbol may belong
// to multiple ModuleLists.
class ModuleList : public AList<ModuleSymbol> {
 public:
  moduleSet filter;
  ModuleSymbol* cursor;

  ModuleSymbol* first(void);
  ModuleSymbol* next(void);
  ModuleSymbol* filteredNext(void);
};

#endif
