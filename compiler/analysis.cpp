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
map_symbols(Vec<BaseAST *> &syms) {
  int symbols = 0, types = 0, exprs = 0, stmts = 0;
  if (verbose_level > 1)
    printf("map_symbols: %d\n", syms.n);
  forv_BaseAST(s, syms) {
    Symbol *sym = dynamic_cast<Symbol *>(s);
    if (sym) {
      sym->asymbol = new ASymbol;
      sym->asymbol->xsymbol = sym;
      symbols++;
      if (verbose_level > 1 && sym->name)
        printf("map_symbols: found Symbol '%s'\n", sym->name);
    } else {
      Type *t = dynamic_cast<Type *>(s);
      if (t) {
	t->asymbol = new ASymbol;
	t->asymbol->xsymbol = t;
	types++;
      } else {
#if 0
	if (e) {
	Expr *e = dynamic_cast<Expr *>(s);
	  e->ainfo = new AInfo;
	  e->ainfo->xast = e;
	  exprs++;
	} else {
	  Stmt *s = dynamic_cast<Stmt *>(s);
	  s->ainfo = new AInfo;
	  s->ainfo->xast = s;
	  stmts++;
	}
#endif	
      }
    }
  }
  if (verbose_level > 1)
    printf("map_symbols: BaseASTs: %d, Symbols: %d, Types: %d, Exprs: %d, Stmts: %d\n", 
	   syms.n, symbols, types, exprs, stmts);
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


