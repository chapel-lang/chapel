#ifndef _PASS_H_
#define _PASS_H_

#include "gc_cpp.h"

class Module;
class Stmt;

class Pass : public gc {
 public:
  virtual void run(Module* moduleList) = 0;
};

class DummyPass : public Pass {
 public:
  virtual void run(Module* moduleList);
};

#endif

