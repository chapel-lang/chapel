#include "postAnalysisCleanup.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/resolveSymbols.h"
#include "symtab.h"


void PostAnalysisCleanup::processSymbol(Symbol* sym) {
  if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym)) {
    if (arg->defaultExpr)
      arg->defaultExpr->remove();
  }
}


void postAnalysisCleanup(void) {
  Pass* pass = new PostAnalysisCleanup();
  pass->run(Symboltable::getModules(pass->whichModules));
}
