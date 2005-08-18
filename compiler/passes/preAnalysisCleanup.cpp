#include "runAnalysis.h"
#include "preAnalysisCleanup.h"
#include "symbol.h"
#include "symtab.h"


static int
checkGeneric(BaseAST *ast, Vec<VariableType *> *variableTypeSymbols = 0) {
  int result = 0;
  switch (ast->astType) {
    case EXPR_SYM: {
      SymExpr* v = dynamic_cast<SymExpr* >(ast);
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(v->var))
        if (ts->definition->astType == TYPE_VARIABLE || ts->definition->isGeneric) {
          if (!variableTypeSymbols)
            return 1;
          else {
            variableTypeSymbols->set_add((VariableType*)ts->definition);
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
      if (checkGeneric(a, variableTypeSymbols)) {
        if (!variableTypeSymbols)
          return 1;
        else
          result = 1;
      }
  }
  return result;
}

int
tagGenerics(FnSymbol *f) {
  int changed = 0;
  Vec<VariableType *> variableTypeSymbols;
  if (f->whereExpr || checkGeneric(f->body, &variableTypeSymbols)) {
    changed = !f->isGeneric || changed;
    f->isGeneric = 1; 
    if (int i = f->nestingDepth()) {
      for (int j = 1; j <= i; i++) {
        FnSymbol *ff = f->nestingParent(i);
        changed = !ff->isGeneric || changed;
        variableTypeSymbols.set_to_vec();
        qsort(variableTypeSymbols.v, variableTypeSymbols.n, sizeof(variableTypeSymbols.v[0]), compar_baseast);
        ff->isGeneric = 1;
        ff->variableTypeSymbols.move(variableTypeSymbols);
      }
    }
  }
  return changed;
}

int
tagGenerics(Type *t) {
  Vec<VariableType *> variableTypeSymbols;
  forv_Vec(FnSymbol, fn, t->methods)
    if (fn->isGeneric)
      variableTypeSymbols.set_union(fn->variableTypeSymbols);
  if (ClassType *st = dynamic_cast<ClassType *>(t)) {
    forv_Vec(Symbol, s, st->fields)
      assert(!dynamic_cast<TypeSymbol*>(s)); // play it safe
    forv_Vec(TypeSymbol, s, st->types) {
      if (s->definition->astType == TYPE_VARIABLE) 
        variableTypeSymbols.set_add((VariableType*)s->definition);
      else
        if (s->definition->isGeneric)
          variableTypeSymbols.set_union(s->definition->variableTypeSymbols);
    }    
  }
  if (variableTypeSymbols.n) {
    variableTypeSymbols.set_to_vec();
    qsort(variableTypeSymbols.v, variableTypeSymbols.n, sizeof(variableTypeSymbols.v[0]), compar_baseast);
    t->isGeneric = 1;
    t->variableTypeSymbols.move(variableTypeSymbols);
  }
  return variableTypeSymbols.n != 0;
}

void 
tagGenerics(Vec<BaseAST *> &asts) {
  // set isGeneric
  forv_BaseAST(s, asts)
    if (FnSymbol *f = dynamic_cast<FnSymbol*>(s))
      f->isGeneric = 0; // trust no one
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
