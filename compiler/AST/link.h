#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>

class Link {
 public:
  Link* prev;
  Link* next;

  Link(void);

  virtual void print(FILE*) = 0;
  virtual void printList(FILE*) = 0;
  virtual bool isNull(void);

  void insert(Link* newlink);
  void add(Link* newlink);
  void append(Link* newlink);
};

#endif
