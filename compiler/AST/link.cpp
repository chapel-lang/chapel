#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"

ILink::ILink(void) :
  prev(nilILink),
  next(nilILink)
{}


bool ILink::isNull(void) {
  return (this == nilILink);
}


void ILink::traverse(Traversal* traversal) {
  INT_FATAL("Calling traverse() on an ILink");
}


void ILink::print(FILE* outfile) {
  if (isNull()) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of ILink");
  }
}


void ILink::printList(FILE* outfile, char* separator) {
  ILink* ptr;

  print(outfile);
  ptr = next;
  while (!ptr->isNull()) {
    fprintf(outfile, "%s", separator);
    ptr->print(outfile);
    ptr = ptr->next;
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

  codegen(outfile);
  ptr = next;
  while (!ptr->isNull()) {
    fprintf(outfile, "%s", separator);
    ptr->codegen(outfile);
    ptr = ptr->next;
  }
}


void ILink::insert(ILink* newlink) {
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
  next = newlink;
}


void ILink::append(ILink* newlink) {
  ILink* lastlink;

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
