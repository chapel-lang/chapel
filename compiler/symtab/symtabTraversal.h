#ifndef _SYMTAB_TRAVERSAL_H_
#define _SYMTAB_TRAVERSAL_H_

#include "../passes/pass.h"
#include "symscope.h"

class SymtabTraversal : public Pass {
 public:
  bool skipInternalScopes;

  virtual void preProcessScope(SymScope*);
  virtual void postProcessScope(SymScope*);
  virtual void processSymbol(Symbol*);

  virtual void run(void);
  virtual void run(ModuleSymbol* moduleList);
  virtual void run(FnSymbol* function);
};

#endif
