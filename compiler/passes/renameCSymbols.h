#ifndef _RENAME_C_SYMBOLS_H_
#define _RENAME_C_SYMBOLS_H_

#include "pass.h"

class RenameCSymbols : public Pass {
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif

