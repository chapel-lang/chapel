#ifndef _RUN_ANALYSIS_H_
#define _RUN_ANALYSIS_H_

#include "pass.h"

class RunAnalysis : public Pass {
 public:
  void run(Module* moduleList);
};

#endif
