#ifndef _REMOVE_NESTED_FUNCTIONS_
#define _REMOVE_NESTED_FUNCTIONS_

#include "traversal.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "baseAST.h"

class RemoveNestedFunctions : public Traversal {
public :
  void run(Vec<ModuleSymbol*>* modules);
};

void removeNestedFunctions(void);

#endif
