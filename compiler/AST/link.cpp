#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"
#include "baseAST.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/fixup.h"


ILink::ILink(void) :
  prev(NULL),
  next(NULL)
{}


void ILink::traverse(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse ILink");
}


void ILink::traverseList(Traversal* traversal, bool atTop) {
  TRAVERSE(this, traversal, atTop);
  if (next) {
    TRAVERSE_LS(next, traversal, atTop);
  }
}


void ILink::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Cannot traverse definitions of ILink");
}


void ILink::traverseDefList(Traversal* traversal, bool atTop) {
  TRAVERSE_DEF(this, traversal, atTop);
  TRAVERSE_DEF_LS(this->next, traversal, atTop);
}


void ILink::print(FILE* outfile) {
  if (!this) {
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

  if (this) {
    print(outfile);
    ptr = next;
    while (ptr) {
      fprintf(outfile, "%s", separator);
      ptr->print(outfile);
      ptr = ptr->next;
    }
  }
}

void ILink::codegen(FILE* outfile) {
  if (!this) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of ILink");
  }
}


void ILink::codegenList(FILE* outfile, char* separator) {
  ILink* ptr;

  if (this) {
    codegen(outfile);
    ptr = next;
    while (ptr) {
      fprintf(outfile, "%s", separator);
      ptr->codegen(outfile);
      ptr = ptr->next;
    }
  }
}


void ILink::append(ILink* newlink) {
  ILink* lastlink;

  if (!newlink) {
    return;
  }
  lastlink = this;
  while (lastlink->next) {
    lastlink = lastlink->next;
  }
  lastlink->next = newlink;
  newlink->prev = lastlink;
}


void ILink::filter(bool filter(ILink*), ILink** truelinks, 
                   ILink** falselinks) {
  ILink* link = this;
  ILink* nextlink;
  *truelinks = NULL;
  *falselinks = NULL;

  while (link) {
    nextlink = link->next;
    link->next = NULL;
    if (nextlink) {
      nextlink->prev = NULL;
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
  while (first->prev) {
    first = first->prev;
  }
  return first;
}


ILink* ILink::tail(void) {
  ILink* last = this;
  while (last->next) {
    last = last->next;
  }
  return last;
}


int ILink::length(void) {
  if (next) {
    return next->length() + 1;
  }
  else {
    return 1;
  }
}


ILink* ILink::get(int i) {
  if (i <= 0) {
    INT_FATAL(this, "get(i)--invalid i");
  }
  else if (i == 1) {
    return this;
  }
  else if (next) {
    return next->get(i-1);
  }
  else {
    INT_FATAL(this, "get(i)--i is too big");
  }
  return NULL;
}
