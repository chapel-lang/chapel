#ifndef _INSERT_INDEXTYPE_H
#define _INSERT_INDEXTYPE_H

#include "traversal.h"
#include "symscope.h"

class InsertIndexType : public Traversal {
 public:
  Stmt* currentStmt;
  SymScope* currentScope;
  void preProcessStmt(Stmt* stmt);
  void preProcessType(Type* type);
  void postProcessStmt(Stmt* stmt);
};

#endif 
