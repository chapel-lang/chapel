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
}
