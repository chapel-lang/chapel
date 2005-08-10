#ifndef _SYMTAB_TRAVERSAL_H_
#define _SYMTAB_TRAVERSAL_H_

#include "../passes/pass.h"
#include "symscope.h"

class SymtabTraversal : public Pass {
 public:
  virtual void preProcessScope(SymScope*);
  virtual void postProcessScope(SymScope*);
  virtual void processSymbol(Symbol*);

  virtual void run(Vec<ModuleSymbol*>* modules = NULL);
  virtual void run(FnSymbol* function);
  virtual void run(Type* type);
};

#endif
