/******************************************************************************
 *** Rename Overloaded Functions
 ***
 *** This traversal mangles the c name for overloaded functions.
 ***
 ***/

#ifndef _RENAME_OVERLOADED_FUNCTIONS_H_
#define _RENAME_OVERLOADED_FUNCTIONS_H_

#include "symtabTraversal.h"

class RenameOverloadedFunctions : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

#endif
