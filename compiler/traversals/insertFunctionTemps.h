#ifndef _INSERT_FUNCTION_TEMPS_H_
#define _INSERT_FUNCTION_TEMPS_H_

#include "traversal.h"

class InsertFunctionTemps : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

void insertFunctionTemps(void);

#endif
