/******************************************************************************
 *** Rename Overloaded Functions
 ***
 *** This traversal mangles the c name for overloaded functions.
 ***
 ***/

#ifndef _RENAME_OVERLOADED_FUNCTIONS_H_
#define _RENAME_OVERLOADED_FUNCTIONS_H_

#include "traversal.h"

class RenameOverloadedFunctions : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
};

#endif
