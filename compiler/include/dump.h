/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _dump_H_
#define _dump_H_

#include "geysa.h"

class FA;
class Sym;
class Fun;

void dump_html(FA *pdb, char *fn);
void dump_sym_name(FILE *fp, Sym *s);
void dump_fun_list(FILE *fp, Vec<Fun *> &funs);

#endif
