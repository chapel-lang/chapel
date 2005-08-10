#ifndef _BUILD_BINARY_H_
#define _BUILD_BINARY_H_

#include "pass.h"

class BuildBinary : public Pass {
 public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
