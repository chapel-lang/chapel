/* -*-Mode: c++;-*-
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _ast_H_
#define _ast_H_

#include <stdio.h>

class Fun;
class PNode;
class Sym;

class AST : public gc {
 public:
  Vec<PNode *> pnodes;

  virtual char *pathname() = 0;
  virtual int line() = 0;
  virtual Sym *symbol() = 0;

  virtual AST *copy(Map<PNode *, PNode*> *nmap = 0) = 0;

  virtual void dump(FILE *fp, Fun *f) = 0;
  virtual void graph(FILE *fp) = 0;
};
#define forv_AST(_x, _v) forv_Vec(AST, _x, _v)

#endif
