#ifndef _PRE_ANALYSIS_CLEANUP_H_
#define _PRE_ANALYSIS_CLEANUP_H_

#include "pass.h"

class PreAnalysisCleanup : public Pass {
 public:
  void run(ModuleList* moduleList);
};

void tagGenerics(Vec<BaseAST *> &asts);
int tagGenerics(FnSymbol *f);
int tagGenerics(Type *t);

#endif
