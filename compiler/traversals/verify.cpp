#include <string.h>
#include <typeinfo>
#include "verify.h"
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"


static bool removeVerifySymbol(Vec<Symbol*>* syms, Symbol* sym);
static void verifyDefPoint(Symbol* sym);
static void verifyParentScope(Symbol* sym);


Verify::Verify() :
  syms(NULL)
{ }


void Verify::preProcessStmt(Stmt* stmt){
  stmt->verify();

  if (LabelStmt* labelStmt = dynamic_cast<LabelStmt*>(stmt)) {
    if (!removeVerifySymbol(syms, labelStmt->label)) {
      INT_FATAL(stmt, "Label not in Symboltable");
    }
  }

  if (!stmt->prev || !stmt->next) {
    if (!dynamic_cast<BlockStmt*>(stmt) &&
        !dynamic_cast<LabelStmt*>(stmt->parentStmt)) {
      INT_FATAL(stmt, "Statement is not in a list");
    }
  }
}


void Verify::preProcessExpr(Expr* expr){
  expr->verify();

  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (!removeVerifySymbol(syms, defExpr->sym)) {
      INT_FATAL(defExpr, "Symbol in DefExpr not in Symboltable");
    }
  }
}


void Verify::run(Vec<ModuleSymbol*>* modules) {
  Fixup* fixup = new Fixup(true);
  fixup->run(modules);

  syms = new Vec<Symbol*>();
  collect_symbols(syms);

  forv_Vec(Symbol, sym, *syms) {
    verifyParentScope(sym);
    verifyDefPoint(sym);
    if (!sym->isUnresolved) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
        if (ts->type->astType != TYPE_META) {
          INT_FATAL(sym, "TypeSymbol::type is not a MetaType");
        }
        if (ts->definition->symbol != sym) {
          INT_FATAL(sym, "TypeSymbol::definition->symbol is not TypeSymbol");
        }
        if (ts->definition->metaType != sym->type) {
          INT_FATAL(sym, "TypeSymbol::definition->meta_type is not TypeSymbol::type");
        }
      }
    }
  }

  forv_Vec(ModuleSymbol, mod, *modules) {
    mod->startTraversal(this);
  }

  forv_Vec(Symbol, sym, *syms) {
    if (!sym ||
        sym->parentScope->type == SCOPE_INTRINSIC ||
        dynamic_cast<ModuleSymbol*>(sym) ||
        !strcmp("__init_entryPoint", sym->name)) {
      continue;
    }
    TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym);
    if (typeSym) {
      if (dynamic_cast<VariableType*>(typeSym->definition) ||
          dynamic_cast<SumType*>(typeSym->definition)) {
        continue;
      }
    }
    //    INT_FATAL(sym, "Symbol %s not in traversed DefExpr", sym->name);
  }
}


static bool removeVerifySymbol(Vec<Symbol*>* syms, Symbol* sym) {
  for (int i = 0; i < syms->n; i++) {
    if (sym == syms->v[i]) {
      syms->v[i] = NULL;
      return true;
    }
  }
  return false;
}


/**
 **  Verify that Symbol::parentScope is correct
 **/
static void verifyParentScope(Symbol* sym) {
  /**
   **  Unresolved symbols have no scope
   **/
  if (sym->isUnresolved) {
    return;
  }

  if (!sym->parentScope) {
    return;
  }

  Symbol* match = Symboltable::lookupInScope(sym->name, sym->parentScope);

  /**
   **  Symbol is match found in scope
   **/
  if (sym == match) {
    return;
  }

  /**
   **  Symbol is overloaded with match found in scope
   **/
  if (match) {
    for (Symbol* tmp = match->overload; tmp; tmp = tmp->overload) {
      if (sym == tmp) {
        return;
      }
    }
  }

  INT_FATAL(sym, "Incorrect parentScope for symbol '%s'", sym->name);
}


/**
 **  Verify that Symbol::defPoint is correct
 **/
static void verifyDefPoint(Symbol* sym) {
  if (sym->isUnresolved ||
      dynamic_cast<LabelSymbol*>(sym) ||
      dynamic_cast<TypeSymbol*>(sym) ||
      dynamic_cast<SumType*>(sym->type)) {     // SJD: REMOVE
    return;
  }

  if (sym->parentScope && sym->parentScope->type == SCOPE_INTRINSIC) {
    return;
  }

  Symbol* tmp = sym->defPoint->sym;
  if (tmp == sym) {
    return;
  }
  INT_FATAL(sym, "Incorrect defPoint for symbol '%s'", sym->name);
}
