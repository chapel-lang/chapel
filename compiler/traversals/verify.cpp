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

    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      Symbol* formal;
      for (formal = fn->formals; formal; formal = nextLink(Symbol, formal)) {
        if (!removeVerifySymbol(syms, formal)) {
          INT_FATAL(formal, "Formal not in Symboltable");
        }
      }
    }

    if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (EnumType* enum_type = dynamic_cast<EnumType*>(type_sym->type)) {
        EnumSymbol* tmp;
        for (tmp = enum_type->valList; tmp; tmp = nextLink(EnumSymbol, tmp)) {
          if (!removeVerifySymbol(syms, tmp)) {
            INT_FATAL(tmp, "EnumSymbol not in Symboltable");
          }
        }
      }
    }
  }
}


void Verify::run(ModuleSymbol* moduleList) {
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

  ModuleSymbol* mod = moduleList;
  while (mod) {
    mod->startTraversal(this);
    mod = nextLink(ModuleSymbol, mod);
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
   **  Symbol is function overloaded to match found in scope
   **/
  FnSymbol* fn_match = dynamic_cast<FnSymbol*>(match);
  while (fn_match) {
    if (sym == fn_match) {
      return;
    }
    fn_match = fn_match->overload;
  }

  /**
   **  Symbol is getter of match found in scope
   **/
  FnSymbol* fn_sym = dynamic_cast<FnSymbol*>(sym);
  if (fn_sym && fn_sym->_getter) {
    match =
      Symboltable::lookupInScope(sym->name, dynamic_cast<StructuralType*>(fn_sym->classBinding->type)->structScope);
    if (fn_sym->_getter == match) {
      return;
    }
  }

  INT_FATAL(sym, "Incorrect parentScope for symbol '%s'", sym->name);
}


/**
 **  Verify that Symbol::defPoint is correct
 **/
static void verifyDefPoint(Symbol* sym) {
  if (typeid(*sym) == typeid(UnresolvedSymbol)) {
    return;
  }
  if (typeid(*sym) == typeid(LabelSymbol)) {
    return;
  }
  if (typeid(*sym->type) == typeid(SumType)) { // SHOULD BE REMOVED
    return;
  }
  if (sym->parentScope && sym->parentScope->type == SCOPE_INTRINSIC) {
    return;
  }

  Symbol* tmp = sym->defPoint->sym;
  while (tmp) {
    if (tmp == sym) {
      return;
    }
    tmp = nextLink(Symbol, tmp);
  }
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym->defPoint->sym)) {
    Symbol* formals = fn->formals;
    while (formals) {
      if (formals == sym) {
        return;
      }
      formals = nextLink(Symbol, formals);
    }
  }
  if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(sym->defPoint->sym)) {
    if (EnumType* enum_type = dynamic_cast<EnumType*>(type_sym->type)) {
      EnumSymbol* tmp = enum_type->valList;
      while (tmp) {
        if (tmp == sym) {
          return;
        }
        tmp = nextLink(EnumSymbol, tmp);
      }
    }
  }
  INT_FATAL(sym, "Incorrect defPoint for symbol '%s'", sym->name);
}
