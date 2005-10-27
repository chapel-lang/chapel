#ifndef _FUNCTION_RESOLUTION_H_
#define _FUNCTION_RESOLUTION_H_

#include "pass.h"

class FunctionResolution : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
