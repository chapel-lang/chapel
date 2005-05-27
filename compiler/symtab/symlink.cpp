#include "symlink.h"
#include "symtabTraversal.h"

SymLink::SymLink(Symbol* init_pSym) :
  pSym(init_pSym)
{}


void SymLink::traverse(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "traverse not yet implemented for SymLinks");
}


void SymLink::traverse(SymtabTraversal* traversal) {
  Symbol* overload = pSym;
  while (overload) {
    Symbol* next = overload->overload;
    traversal->processSymbol(overload);
    overload = next;
  }
}


void SymLink::print(FILE* outfile) {
  pSym->print(outfile);
}

void SymLink::codegen(FILE* outfile) {
  fprintf(outfile, "This is SymLink's codegen method.\n");
}
