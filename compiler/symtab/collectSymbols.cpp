#include "collectSymbols.h"
#include "symbol.h"

CollectSymbols::CollectSymbols(Vec<Symbol*>* init_syms) {
  syms = init_syms;
  syms->clear();
}

void CollectSymbols::processSymbol(Symbol* sym) {
  syms->add(sym);
}

void collect_symbols(Vec<Symbol*>* syms) {
  CollectSymbols* traversal = new CollectSymbols(syms);
  traversal->run();
}

void collect_symbols(Vec<Symbol*>* syms, FnSymbol* function) {
  CollectSymbols* traversal = new CollectSymbols(syms);
  traversal->run(function);
}
