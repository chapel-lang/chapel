#ifndef _BUILD_DEFAULT_FUNCTIONS_H_
#define _BUILD_DEFAULT_FUNCTIONS_H_

#include "pass.h"

class BuildDefaultFunctions : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
