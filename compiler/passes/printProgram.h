#include "pass.h"

class PrintProgram : public Pass {
 public:
  PrintProgram(void);
  virtual void run(Vec<ModuleSymbol*>* modules);
};
