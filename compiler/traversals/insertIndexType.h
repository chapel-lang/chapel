#ifndef _INSERT_INDEXTYPE_H
#define _INSERT_INDEXTYPE_H

#include "traversal.h"
#include "symscope.h"

class InsertIndexType : public Traversal {
 public:
  void preProcessType(Type* type);
  void preProcessStmt(Stmt* stmt);
};

#endif 
