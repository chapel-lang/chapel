#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"

ILink::ILink(void) :
  prev(NULL),
  next(NULL)
{}


bool ILink::isNull(void) {
  return false;
}


void ILink::printList(FILE* outfile, char* separator) {
  ILink* ptr;

  print(outfile);
  ptr = next;
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->print(outfile);
    ptr = ptr->next;
  }
}

void ILink::codegenList(FILE* outfile, char* separator) {
  ILink* ptr;

  codegen(outfile);
  ptr = next;
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->codegen(outfile);
    ptr = ptr->next;
  }
}


void ILink::insert(ILink* newlink) {
  if (prev != NULL) {
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
  while (lastlink->next != NULL) {
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
