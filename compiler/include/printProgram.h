#include "pass.h"

class PrintProgram : public Pass {
 public:
  void run(Stmt* program);
};
