#ifndef _LINK_H_
#define _LINK_H_

#include <stdio.h>
#include "loc.h"
#include "misc.h"
#include "vec.h"

class Traversal;

class ILink : public Loc {
 public:
  ILink* prev;
  ILink* next;

  ILink(void);

  virtual void traverse(Traversal* traversal, bool atTop = true);
  virtual void traverseDef(Traversal* traversal, bool atTop = true);

  virtual void print(FILE* outfile);
  virtual void printDef(FILE* outfile);
  void println(FILE* outfile);
  virtual void codegen(FILE* outfile);

  void remove(void);
};

#endif
