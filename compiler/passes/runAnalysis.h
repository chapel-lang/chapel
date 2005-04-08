#ifndef _RUN_ANALYSIS_H_
#define _RUN_ANALYSIS_H_

#include "pass.h"

class RunAnalysis : public Pass {
 public:
  static int isRunning;
  static int runCount;
  static Stmt* entryStmtList;
  void run(ModuleSymbol* moduleList);
};

#endif
