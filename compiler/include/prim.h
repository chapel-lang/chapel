/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _prim_H_
#define _prim_H_

class Prim;
class IF1;
class Code;
class AST;
class ParseAST;
class PNode;
class AType;
class EntrySet;

enum PrimType {
  PRIM_TYPE_ALL,
  PRIM_TYPE_ANY,
  PRIM_TYPE_SYMBOL,
  PRIM_TYPE_A,
  PRIM_TYPE_REF, PRIM_TYPE_CONT,
  PRIM_TYPE_ANY_NUM_A, PRIM_TYPE_ANY_NUM_B, PRIM_TYPE_ANY_NUM_AB,
  PRIM_TYPE_ANY_INT_A, PRIM_TYPE_ANY_INT_B,
  PRIM_TYPE_BOOL, PRIM_TYPE_SIZE
};

enum PrimOptions {
  PRIM_NON_FUNCTIONAL = 1
};

class Primitives : public gc {
 public:
  Map<char *, Prim *> prim_map[2][2];
  Vec<Prim *> prims;
  Prim *find(ParseAST *c);
  Prim *find(Code *c);
  Prim *find(PNode *p);
  Primitives(IF1 *if1);
};

class Prim : public gc {
 public:
  int index;
  char *string;
  char *name;
  int nargs; // -n means at least n
  int nrets;
  int pos;   // position of primitive symbol
  uint nonfunctional:1;
  PrimType *arg_types; // vector excluding primitive symbol
  PrimType *ret_types;
  Vec<AType *> args;
  Prim(int aindex, char *astring, char *aname, int anargs, int apos, int anrets,
       PrimType *aarg_types, PrimType *aret_types, int options);
};
#define forv_Prim(_c, _v) forv_Vec(Prim, _c, _v)

typedef void (*PrimitiveTransferFunctionPtr)(PNode *pn, EntrySet *es);

// registered primitive transfer functions
class RegisteredPrim : public gc {
 public:
  PrimitiveTransferFunctionPtr	fn;
  uint is_functional : 1;
  RegisteredPrim(PrimitiveTransferFunctionPtr afn, int is_func = 0) 
    : fn(afn), is_functional(is_func) {}
};

#include "../analysis/prim_data.h"

#endif
