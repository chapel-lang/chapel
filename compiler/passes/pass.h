#ifndef _PASS_H_
#define _PASS_H_

#include "gc_cpp.h"

class ModuleSymbol;

class Pass : public gc {
 public:
  virtual void run(ModuleSymbol* moduleList) = 0;
};

class DummyPass : public Pass {
 public:
  virtual void run(ModuleSymbol* moduleList);
};

#endif

