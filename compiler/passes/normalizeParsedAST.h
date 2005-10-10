#ifndef _NORMALIZE_PARSED_AST_H_
#define _NORMALIZE_PARSED_AST_H_

#include "pass.h"

class NormalizeParsedAST : public Pass {
public:
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif
