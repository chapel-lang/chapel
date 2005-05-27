#include "pass.h"

class PrintProgram : public Pass {
 public:
  PrintProgram(void);
  void run(ModuleList* moduleList);
};
