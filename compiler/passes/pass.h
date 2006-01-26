#ifndef _PASS_H_
#define _PASS_H_

#include "chpl.h"
#include "chplenum.h"

class ModuleSymbol;

class Pass : public gc {
 public:
  moduleSet whichModules;
  Pass(void);
  virtual void run(Vec<ModuleSymbol*>* modules) = 0;
};

#endif
