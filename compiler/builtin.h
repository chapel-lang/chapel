/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _builtin_H_
#define _builtin_H_

class PDB;

#define S(_n) extern Sym *sym_##_n;
#include "builtin_symbols.h"
#undef S

void initialize_symbols(PDB *pdb);

#endif
