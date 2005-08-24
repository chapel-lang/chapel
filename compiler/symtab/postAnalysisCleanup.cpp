#include "postAnalysisCleanup.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/resolveSymbols.h"


PostAnalysisCleanup::PostAnalysisCleanup() {
  //  whichModules = MODULES_CODEGEN;
}


void PostAnalysisCleanup::processSymbol(Symbol* sym) {
  if (dynamic_cast<ArgSymbol*>(sym) && sym->defPoint->init) {
    sym->defPoint->init->remove();
  }
}


