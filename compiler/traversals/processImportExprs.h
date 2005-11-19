#ifndef _PROCESS_IMPORT_EXPRS_H_
#define _PROCESS_IMPORT_EXPRS_H_

#include "traversal.h"

class ProcessImportExprs : public Traversal {
public:
  void postProcessExpr(Expr* expr);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

void processImportExprs(void);

#endif
