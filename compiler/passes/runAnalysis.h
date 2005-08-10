#ifndef _RUN_ANALYSIS_H_
#define _RUN_ANALYSIS_H_

#include "pass.h"
#include "alist.h"

class Stmt;

class RunAnalysis : public Pass {
 public:
  static int isRunning;
  static int runCount;
  static AList<Stmt>* entryStmtList;
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
