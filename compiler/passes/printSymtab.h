#ifndef _PRINTSYMTAB_H_
#define _PRINTSYMTAB_H_

#include "pass.h"

class PrintSymtab : public Pass {
 public:
  void run(ModuleSymbol* moduleList);
};

#endif
