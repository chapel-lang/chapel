#include <stdlib.h>
#include <stdio.h>
#include "link.h"

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
  if (newlink->next != NULL) {
    //    fprintf(stderr, "WARNING: linked\n");
  }
}
