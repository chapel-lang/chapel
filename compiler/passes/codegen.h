#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "pass.h"

class Codegen : public Pass {
  void run(Module* moduleList);
};

#endif
