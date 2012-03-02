#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "symbol.h"
#include "astutil.h"
#include "map.h"
#include "vec.h"

class VisibleFunctionBlock {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock() { }
};


#endif
