#include "geysa.h"
#include <string.h>
#include <typeinfo>
#include "astutil.h"
#include "verify.h"
#include "fixup.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"


static void verifyDefPoint(Symbol* sym);
static void verifyParentScope(Symbol* sym);


void Verify::preProcessStmt(Stmt* stmt){
  stmt->verify();

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
    if (defExpr->sym->parentSymbol) {
      INT_FATAL(defExpr->sym, "Symbol has parentSymbol set");
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (typeSym->definition->parentSymbol) {
        INT_FATAL(typeSym->definition, "Type has parentSymbol set");
      }
    }
  }
}


void Verify::run(Vec<ModuleSymbol*>* modules) {
  Fixup* fixup = new Fixup(true);
  fixup->run(modules);

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
      if (!sym->isUnresolved) {
        verifyParentScope(sym);
        verifyDefPoint(sym);
        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
          if (ts->type->astType != TYPE_META)
            INT_FATAL(sym, "TypeSymbol::type is not a MetaType");
          if (ts->definition->symbol != sym)
            INT_FATAL(sym, "TypeSymbol::definition->symbol is not TypeSymbol");
          if (ts->definition->metaType != sym->type)
            INT_FATAL(sym, "TypeSymbol::definition->meta_type is not TypeSymbol::type");
        }
      }
    }
  }

  forv_Vec(ModuleSymbol, mod, *modules) {
    mod->startTraversal(this);
  }
}


/**
 **  Verify that Symbol::parentScope is correct
 **/
static void verifyParentScope(Symbol* sym) {
  if (!sym->parentScope)
    return;

  /** 
   **  Symbol is a constant
   **/
  if(strcmp(sym->name, "") == 0) { 
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
  if (sym->parentScope && sym->parentScope->type == SCOPE_INTRINSIC)
    return;

  if (sym != sym->defPoint->sym)
    INT_FATAL(sym, "Incorrect defPoint for symbol '%s'", sym->name);
}


void verify(void) {
  Pass* pass = new Verify();
  pass->run(Symboltable::getModules(pass->whichModules));
}
