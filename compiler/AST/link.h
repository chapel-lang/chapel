#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"

class ILink : public Loc {
 public:
  ILink* prev;
  ILink* next;

  ILink(void);

  virtual void print(FILE*) = 0;
  virtual void printList(FILE* outfile, char* separator = ", ");
  virtual bool isNull(void);

  void insert(ILink* newlink);
  void add(ILink* newlink);
  void append(ILink* newlink);
};

#endif
