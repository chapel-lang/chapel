/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _builtin_H_
#define _builtin_H_

class PDB;
class Sym;

#define S(_n) extern Sym *sym_##_n;
#include "builtin_symbols.h"
#undef S

enum Builtin {
#define S(_x) Builtin_##_x,
#include "builtin_symbols.h"
#undef S
  Builtin_MAX
};

void initialize_symbols(PDB *pdb);

#endif
