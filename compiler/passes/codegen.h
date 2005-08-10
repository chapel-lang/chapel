#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "pass.h"

class Codegen : public Pass {
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
