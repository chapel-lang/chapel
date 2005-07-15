#include <string.h>
#include <typeinfo>
#include "verify.h"
#include "fixup.h"
#include "expr.h"
#include "moduleList.h"
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
      INT_FATAL(labelStmt, "Label not in Symboltable");
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


void Verify::run(ModuleList* moduleList) {
  Fixup* fixup = new Fixup(true);
  fixup->run(moduleList);

  syms = new Vec<Symbol*>();
  collect_symbols(syms);

  forv_Vec(Symbol, sym, *syms) {
    verifyParentScope(sym);
    verifyDefPoint(sym);
    if (dynamic_cast<TypeSymbol*>(sym)) {
      if (sym->type->symbol != sym) {
        INT_FATAL(sym, "TypeSymbol::type::symbol is not TypeSymbol");
      }
    }
  }

  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    mod->startTraversal(this);
    mod = moduleList->next();
  }

  forv_Vec(Symbol, sym, *syms) {
    if (sym &&
        sym->parentScope->type != SCOPE_INTRINSIC &&
        !dynamic_cast<ModuleSymbol*>(sym) &&
        !dynamic_cast<SumType*>(sym->type) &&
        strcmp("__init_entryPoint", sym->name)) {
      //      INT_FATAL(sym, "Symbol not in traversed DefExpr");
    }
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
  if (dynamic_cast<UnresolvedSymbol*>(sym)) {
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
  if (dynamic_cast<UnresolvedSymbol*>(sym) ||
      dynamic_cast<LabelSymbol*>(sym) ||
      dynamic_cast<TypeSymbol*>(sym) ||
      dynamic_cast<ForwardingSymbol*>(sym) ||
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
