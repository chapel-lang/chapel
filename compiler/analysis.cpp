/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "analysis.h"
#include "expr.h"
#include "files.h"
#include "link.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

ASymbol::ASymbol() : xsymbol(0) {
}

AInfo::AInfo() : xast(0) {
}

static void
map_symbols(Vec<BaseAST *> &syms) {
  int symbols = 0;
  printf("%d\n", syms.n);
  forv_BaseAST(s, syms) {
    Symbol *sym = dynamic_cast<Symbol *>(s);
    if (sym) {
      sym->asymbol = new ASymbol;
      sym->asymbol->xsymbol = sym;
      symbols++;
      if (sym->name)
        printf("Symbol '%s'\n", sym->name);
    } 
  }
  printf("BaseASTs: %d, Symbols: %d\n", syms.n, symbols);
}

static void
close_symbols(BaseAST *a, Vec<BaseAST *> &syms) {
  Vec<BaseAST *> set;
  set.set_add(a);
  syms.add(a);
  forv_BaseAST(s, syms) {
    Vec<BaseAST *> moresyms;
    s->getBaseASTs(moresyms);
    forv_BaseAST(ss, moresyms) {
      assert(ss);
      if (set.set_add(ss))
        syms.add(ss);
    }
  }
}

static void
import_symbols(BaseAST *a) {
  Vec<BaseAST *> syms;

  close_symbols(a, syms);
  map_symbols(syms);
}

void
analyze_new_ast(BaseAST* a) {
  import_symbols(a);
}


