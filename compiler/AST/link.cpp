#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"
#include "baseAST.h"

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


void ILink::preinsert(ILink* newlink) {
  ILink* first;
  ILink* last;

  first = newlink;
  while (first->prev && !first->prev->isNull()) {
    first = first->prev;
  }
  last = newlink;
  while (last->next && !last->next->isNull()) {
    last = last->next;
  }
  if (!prev->isNull()) {
    prev->next = first;
  }
  first->prev = prev;
  prev = last;
  last->next = this;
}


void ILink::postinsert(ILink* newlink) {
  if (!prev->isNull()) {
    prev->next = newlink;
  }
  newlink->prev = prev;
  newlink->next = this;
  prev = newlink;
}


void ILink::add(ILink* newlink) {
  newlink->next = next;
  newlink->prev = this;
  next->prev = newlink;
  next = newlink;
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


ILink* ILink::extract(void) {
  if (prev && !prev->isNull()) {
    prev->next = next;
  }
  if (next && !next->isNull()) {
    next->prev = prev;
  }
  next = nilILink;
  prev = nilILink;
  return this;
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


void ILink::replace(ILink* old_link, ILink* new_link) {
  /* Find first link in new list */
  ILink* first = new_link;
  while (first->prev && !first->prev->isNull()) {
    first = first->prev;
  }

  /* If first is not new_link, is this an error? */
  if (first != new_link) {
    INT_FATAL(old_link, "Cannot call replace() using a replacement AST that "
	      "isn't the head of a list");
  }

  /* Find last link in new list */
  ILink* last = new_link;
  while (last->next && !last->next->isNull()) {
    last = last->next;
  }

  /* Set prev link */
  first->prev = old_link->prev;
  if ((old_link->prev) && (!(old_link->prev->isNull()))) {
    old_link->prev->next = first;
  }

  /* Set next link */
  last->next = old_link->next;
  if ((old_link->next) && (!(old_link->next->isNull()))) {
    old_link->next->prev = last;
  }

  /* Wipe out old links--it's been replaced */
  old_link->prev = nilILink;
  old_link->next = nilILink;
}
