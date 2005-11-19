#ifndef _BUILD_LVALUE_RETURNS_H_
#define _BUILD_LVALUE_RETURNS_H_

#include "traversal.h"

class BuildLValueFunctions : public Traversal {
 public:
  BuildLValueFunctions(void);

  void preProcessStmt(Stmt* stmt);
};

void buildLValueFunctions(void);

#endif
