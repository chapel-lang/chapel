#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"

class Link : public Loc {
 public:
  Link* prev;
  Link* next;

  Link(void);

  virtual void print(FILE*) = 0;
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual bool isNull(void);

  void insert(Link* newlink);
  void add(Link* newlink);
  void append(Link* newlink);
};

#endif
