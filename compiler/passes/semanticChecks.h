#ifndef _SEMANTIC_CHECKS_H_
#define _SEMANTIC_CHECKS_H_

#include "pass.h"

// After parsing
class SemanticCheckI : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

// After scope resolution
class SemanticCheckII : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

// After analysis
class SemanticCheckIII : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
