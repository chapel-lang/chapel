#include "runAnalysis.h"
#include "preAnalysisCleanup.h"
#include "symbol.h"
#include "symtab.h"


static int
checkGeneric(BaseAST *ast, Vec<Symbol *> *genericSymbols = 0) {
  int result = 0;
  switch (ast->astType) {
    case EXPR_SYM: {
      SymExpr* v = dynamic_cast<SymExpr* >(ast);
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(v->var))
        if (ts->definition->isGeneric) {
          if (!genericSymbols)
            return 1;
          else {
            assert(dynamic_cast<VariableType*>(ts->definition));
            genericSymbols->set_add(ts);
            result = 1;
          }
        }
      break;
    }
    default: break;
  }
  DefExpr* def_expr = dynamic_cast<DefExpr*>(ast);
  if (!def_expr || !dynamic_cast<FnSymbol*>(def_expr->sym)) {
    Vec<BaseAST *> asts;
    get_ast_children(ast, asts);
    forv_BaseAST(a, asts)
      if (checkGeneric(a, genericSymbols)) {
        if (!genericSymbols)
          return 1;
        else
          result = 1;
      }
  }
  return result;
}

static int
genericFunctionArg(FnSymbol *f, Vec<Symbol *> &genericSymbols) {
  for (DefExpr* formal = f->formals->first(); formal; 
       formal = f->formals->next()) {
    if (ArgSymbol *ps = dynamic_cast<ArgSymbol *>(formal->sym)) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol *>(ps->genericSymbol)) {
        if (ts->definition->isGeneric) {
          assert(dynamic_cast<VariableType*>(ts->definition));
          genericSymbols.set_add(ts);
          return 1;
        }
      }
      if (ps->type->isGeneric) {
        genericSymbols.set_add(ps->type->symbol);
        return 1;
      }
      if (ps->intent == INTENT_TYPE || ps->intent == INTENT_PARAM) {
        genericSymbols.set_add(ps);
        return 1;
      }
    }
  }
  return 0;
}

int
tagGenerics(FnSymbol *f) {
  int changed = 0;
  Vec<Symbol *> genericSymbols;
  if (genericFunctionArg(f, genericSymbols) || checkGeneric(f->body, &genericSymbols)) {
    changed = !f->isGeneric || changed;
    f->isGeneric = 1; 
    if (int i = f->nestingDepth()) {
      for (int j = 1; j <= i; i++) {
        FnSymbol *ff = f->nestingParent(i);
        changed = !ff->isGeneric || changed;
        genericSymbols.set_to_vec();
        qsort(genericSymbols.v, genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
        ff->isGeneric = 1;
        ff->genericSymbols.move(genericSymbols);
      }
    }
  }
  return changed;
}

int
tagGenerics(Type *t) {
  Vec<Symbol *> genericSymbols;
  forv_Vec(FnSymbol, fn, t->methods)
    if (fn->isGeneric)
      genericSymbols.set_union(fn->genericSymbols);
  if (ClassType *st = dynamic_cast<ClassType *>(t)) {
    forv_Vec(Symbol, s, st->fields)
      assert(!dynamic_cast<TypeSymbol*>(s)); // play it safe
    forv_Vec(TypeSymbol, s, st->types) {
      if (s->definition->astType == TYPE_VARIABLE) 
        genericSymbols.set_add(s->definition->symbol);
      else
        if (s->definition->isGeneric)
          genericSymbols.set_union(s->definition->genericSymbols);
    }    
  }
  if (genericSymbols.n) {
    genericSymbols.set_to_vec();
    qsort(genericSymbols.v, genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
    t->isGeneric = 1;
    t->genericSymbols.move(genericSymbols);
  }
  return genericSymbols.n != 0;
}

void 
tagGenerics(Vec<BaseAST *> &asts) {
  // set isGeneric
  forv_BaseAST(s, asts) {
    if (FnSymbol *f = dynamic_cast<FnSymbol*>(s))
      f->isGeneric = 0; // trust no one
    if (Type *t = dynamic_cast<Type*>(s))
      t->isGeneric = 0; // trust no one
  }
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_BaseAST(s, asts) {
      if (FnSymbol *f = dynamic_cast<FnSymbol*>(s))
        if (!f->isGeneric)
          changed = tagGenerics(f) || changed;
      if (Type *t = dynamic_cast<Type*>(s))
        if (!t->isGeneric)
          changed = tagGenerics(t) || changed;
    }
  }
}

void PreAnalysisCleanup::run(Vec<ModuleSymbol*>* modules) {
  Vec<Stmt *> stmts;
  Vec<Symbol *> symbols;
  Vec<BaseAST *> asts;

  // Collect program stmts and symbols
  forv_Vec(ModuleSymbol, mod, *modules) {
    for_alist(Stmt, s, mod->stmts)
      stmts.add(s);
    getSymbols(mod->modScope, symbols);
  }
  symbols.set_to_vec();
  qsort(symbols.v, symbols.n, sizeof(symbols.v[0]), compar_baseast);
  tagGenerics(*(Vec<BaseAST*>*)&symbols);
}
