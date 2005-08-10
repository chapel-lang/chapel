#ifndef _APPLY_GETTERS_SETTERS_H_
#define _APPLY_GETTERS_SETTERS_H_

#include "pass.h"

class ApplyGettersSetters : public Pass {
 public:
  void run(Vec<ModuleSymbol*>* modules);
};

#endif
