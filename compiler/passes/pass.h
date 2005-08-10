#ifndef _PASS_H_
#define _PASS_H_

#include "chplalloc.h"
#include "chplenum.h"
#include "vec.h"

class ModuleSymbol;

class Pass : public gc {
 public:
  char* args;
  moduleSet whichModules;

  Pass(void);

  void setArgs(char* init_args);
  virtual void run(Vec<ModuleSymbol*>* modules) = 0;
};

class DummyPass : public Pass {
 public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
