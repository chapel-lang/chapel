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

char *AInfo::pathname() { 
  return xast->filename;
}

int AInfo::line() {
  return xast->lineno;
}

Sym *AInfo::symbol() {
  return NULL;
}

AST *AInfo::copy(Map<PNode *, PNode*> *nmap) {
  return NULL;
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
	Expr *e = dynamic_cast<Expr *>(s);
	if (e) {
	  e->ainfo = new AInfo;
	  e->ainfo->xast = e;
	  exprs++;
	} else {
	  Stmt *st = dynamic_cast<Stmt *>(s);
	  st->ainfo = new AInfo;
	  st->ainfo->xast = s;
	  stmts++;
	}
      }
    }
  }
  if (verbose_level > 1)
    printf("map_symbols: BaseASTs: %d, Symbols: %d, Types: %d, Exprs: %d, Stmts: %d\n", 
	   syms.n, symbols, types, exprs, stmts);
}

static void
build_types(Vec<BaseAST *> &syms) {
  Vec<Type *> types;
  forv_BaseAST(s, syms) {
    Type *t = dynamic_cast<Type *>(s);
    if (t) 
      types.add(t);
  }
  forv_Type(t, types) {
    switch (t->astType) {
      default: assert(!"case");
      case TYPE_NULL:  // used like "void" for function return type
	//t->asymbol->alias = sym_void;
	t->asymbol->type_kind = Type_ALIAS;
	break;
      case TYPE_BUILTIN:
	
      case TYPE_ENUM:
      case TYPE_DOMAIN:
      case TYPE_SUBDOMAIN:
      case TYPE_INDEX:
      case TYPE_SUBINDEX:
      case TYPE_ARRAY:
      case TYPE_USER:
      case TYPE_CLASS:
	break;
    }
  }
}

static void
import_symbols(BaseAST *a) {
  Vec<BaseAST *> syms;
  close_symbols(a, syms);
  map_symbols(syms);
  build_types(syms);
}

void
analyze_new_ast(BaseAST* a) {
  import_symbols(a);
}


