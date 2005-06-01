#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"


ILink::ILink(void) :
  prev(NULL),
  next(NULL)
{}


void ILink::traverse(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse ILink");
}


void ILink::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse definitions of ILink");
}


void ILink::print(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of ILink");
  }
}


void ILink::printDef(FILE* outfile) {
  INT_FATAL("printDef() is unimplemented for a subclass of ILink");
}


void ILink::println(FILE* outfile) {
  this->print(outfile);
  fprintf(outfile, "\n");
}


void ILink::codegen(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of ILink");
  }
}


void ILink::remove(void) {
  if (!next || !prev) {
    INT_FATAL("Calling remove() on an ILink that's not in a list");
  }
  next->prev = prev;
  prev->next = next;
  next = NULL;
  prev = NULL;
}
