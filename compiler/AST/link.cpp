#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"
#include "baseAST.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/fixup.h"


ILink::ILink(void) :
  prev(nilILink),
  next(nilILink)
{}


void ILink::traverse(ILink* &_this, Traversal* traversal, bool atTop) {
  if (isNull()) return;

  if (BaseAST* ast = dynamic_cast<BaseAST*>(_this)) {
    ast->traverse(ast, traversal, atTop);
  }
}


bool ILink::isNull(void) {
  return (this == nilILink);
}


void ILink::print(FILE* outfile) {
  if (isNull()) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of ILink");
  }
}


void ILink::println(FILE* outfile) {
  this->print(outfile);
  fprintf(outfile, "\n");
}


void ILink::printList(FILE* outfile, char* separator) {
  ILink* ptr;

  if (!isNull()) {
    print(outfile);
    ptr = next;
    while (!ptr->isNull()) {
      fprintf(outfile, "%s", separator);
      ptr->print(outfile);
      ptr = ptr->next;
    }
  }
}

void ILink::codegen(FILE* outfile) {
  if (isNull()) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of ILink");
  }
}


void ILink::codegenList(FILE* outfile, char* separator) {
  ILink* ptr;

  if (!isNull()) {
    codegen(outfile);
    ptr = next;
    while (!ptr->isNull()) {
      fprintf(outfile, "%s", separator);
      ptr->codegen(outfile);
      ptr = ptr->next;
    }
  }
}


void ILink::append(ILink* newlink) {
  ILink* lastlink;

  if (newlink->isNull()) {
    return;
  }
  lastlink = this;
  while (!lastlink->next->isNull()) {
    lastlink = lastlink->next;
  }
  lastlink->next = newlink;
  newlink->prev = lastlink;
}


void ILink::filter(bool filter(ILink*), ILink** truelinks, 
		   ILink** falselinks) {
  ILink* link = this;
  ILink* nextlink;
  *truelinks = nilILink;
  *falselinks = nilILink;

  while (!link->isNull()) {
    nextlink = link->next;
    link->next = nilILink;
    if (!nextlink->isNull()) {
      nextlink->prev = nilILink;
    }
    if (filter(link)) {
      *truelinks = appendLink(*truelinks, link);
    } else {
      *falselinks = appendLink(*falselinks, link);
    }
    link = nextlink;
  }
}


ILink* ILink::head(void) {
  ILink* first = this;
  while (first->prev && !first->prev->isNull()) {
    first = first->prev;
  }
  return first;
}


ILink* ILink::tail(void) {
  ILink* last = this;
  while (last->next && !last->next->isNull()) {
    last = last->next;
  }
  return last;
}
