#include <stdlib.h>
#include <stdio.h>
#include "link.h"

Link::Link(void) :
  prev(NULL),
  next(NULL)
{}


bool Link::isNull(void) {
  return false;
}


void Link::printList(FILE* outfile, char* separator) {
  Link* ptr;

  print(outfile);
  ptr = next;
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->print(outfile);
    ptr = ptr->next;
  }
}


void Link::insert(Link* newlink) {
  if (prev != NULL) {
    prev->next = newlink;
  }
  newlink->prev = prev;
  newlink->next = this;
  prev = newlink;
}


void Link::add(Link* newlink) {
  newlink->next = next;
  newlink->prev = this;
  next = newlink;
}


void Link::append(Link* newlink) {
  Link* lastlink;

  lastlink = this;
  while (lastlink->next != NULL) {
    lastlink = lastlink->next;
  }
  lastlink->next = newlink;
  newlink->prev = lastlink;
  if (newlink->next != NULL) {
    fprintf(stderr, "WARNING: linked\n");
  }
}
