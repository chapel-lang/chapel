#ifndef _PASS_H_
#define _PASS_H_

#include "chplalloc.h"
#include "chplenum.h"
#include "vec.h"

class ModuleSymbol;

class Pass : public gc {
 public:
  moduleSet whichModules;
  Pass(void);
  virtual void run(Vec<ModuleSymbol*>* modules) = 0;
};

#endif
