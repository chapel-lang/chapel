#include "moduleList.h"
#include "runAnalysis.h"
#include "preAnalysisCleanup.h"
#include "symbol.h"
#include "symtab.h"


static int
checkGeneric(BaseAST *ast, Vec<VariableType *> *typeVariables = 0) {
  int result = 0;
  switch (ast->astType) {
    case EXPR_VARIABLE: {
      Variable *v = dynamic_cast<Variable *>(ast);
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(v->var))
        if (ts->definition->astType == TYPE_VARIABLE || ts->definition->isGeneric) {
          if (!typeVariables)
            return 1;
          else {
            typeVariables->set_add((VariableType*)ts->definition);
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
      if (checkGeneric(a, typeVariables)) {
        if (!typeVariables)
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
  Vec<VariableType *> typeVariables;
  if (f->whereExpr || checkGeneric(f->body, &typeVariables)) {
    changed = !f->isGeneric || changed;
    f->isGeneric = 1; 
    if (int i = f->nestingDepth()) {
      for (int j = 1; j <= i; i++) {
        FnSymbol *ff = f->nestingParent(i);
        changed = !ff->isGeneric || changed;
        typeVariables.set_to_vec();
        qsort(typeVariables.v, typeVariables.n, sizeof(typeVariables.v[0]), compar_baseast);
        ff->isGeneric = 1;
        ff->typeVariables.move(typeVariables);
      }
    }
  }
  return changed;
}

int
tagGenerics(Type *t) {
  Vec<VariableType *> typeVariables;
  forv_Vec(FnSymbol, fn, t->methods)
    if (fn->isGeneric)
      typeVariables.set_union(fn->typeVariables);
  if (StructuralType *st = dynamic_cast<StructuralType *>(t)) {
    forv_Vec(Symbol, s, st->fields)
      assert(!dynamic_cast<TypeSymbol*>(s)); // play it safe
    forv_Vec(TypeSymbol, s, st->types) {
      if (s->definition->astType == TYPE_VARIABLE) 
        typeVariables.set_add((VariableType*)s->definition);
      else
        if (s->definition->isGeneric)
          typeVariables.set_union(s->definition->typeVariables);
    }    
  }
  if (typeVariables.n) {
    typeVariables.set_to_vec();
    qsort(typeVariables.v, typeVariables.n, sizeof(typeVariables.v[0]), compar_baseast);
    t->isGeneric = 1;
    t->typeVariables.move(typeVariables);
  }
  return typeVariables.n != 0;
}

static void
getSymbols(SymScope *scope, Vec<Symbol *> &symbols) {
  if (!scope) return;
  forv_Symbol(sym, scope->symbols)
    for (Symbol *s = sym; s; s = s->overload)
      symbols.add(s);
  getSymbols(scope->child, symbols);
  getSymbols(scope->sibling, symbols);
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

void PreAnalysisCleanup::run(ModuleList* moduleList) {
  Vec<Stmt *> stmts;
  Vec<Symbol *> symbols;

  // Collect program stmts and symbols
  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next()) {
    for_alist(Stmt, s, mod->stmts)
      stmts.add(s);
    getSymbols(mod->modScope, symbols);
  }
  for_alist(Stmt, s, RunAnalysis::entryStmtList)
    stmts.add(s);
  tagGenerics(*(Vec<BaseAST*>*)&symbols);
}
