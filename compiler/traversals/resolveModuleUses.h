#ifndef _RESOLVE_MODULE_USES_H_
#define _RESOLVE_MODULE_USES_H_

#include "traversal.h"

class ResolveModuleUses : public Traversal {
public:
  void preProcessExpr(Expr* expr);
  void run(ModuleList* moduleList);
};

#endif
