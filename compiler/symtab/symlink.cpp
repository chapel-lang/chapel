#include "symlink.h"

SymLink::SymLink(Symbol* init_pSym) :
  pSym(init_pSym)
{}


void SymLink::traverse(Traversal* traversal) {
  INT_FATAL(this, "traverse not yet implemented for SymLinks");
}


/*
void SymLink::traverseLinks(SymtabTraversal* traversal) {
  traversal->preProcessSymbol(pSym);
  SymLink* nextLink = dynamic_cast<SymLink*>(next);
  nextLink->traverseList(traversal);
  traversal->postProcessSymbol(pSym);
}
*/


void SymLink::print(FILE* outfile) {
  pSym->print(outfile);
}

void SymLink::codegen(FILE* outfile) {
  fprintf(outfile, "This is SymLink's codegen method.\n");
}

