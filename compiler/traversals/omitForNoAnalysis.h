#ifndef _OMIT_FOR_NO_ANALYSIS_H_
#define _OMIT_FOR_NO_ANALYSIS_H_

#include "stmt.h"
#include "symtab.h"
#include "traversal.h"

class OmitForNoAnalysis : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
  void run(ModuleSymbol* moduleList);
};

#endif
