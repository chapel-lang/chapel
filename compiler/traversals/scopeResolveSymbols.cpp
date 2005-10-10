#include <typeinfo>
#include "scopeResolveSymbols.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "../traversals/fixup.h"


ScopeResolveSymbols::ScopeResolveSymbols() {
  defList = new Map<SymScope*,Vec<VarSymbol*>*>();
}


void ScopeResolveSymbols::postProcessExpr(Expr* expr) {
  if (SymExpr* sym_use = dynamic_cast<SymExpr*>(expr)) {
    if (sym_use->var->isUnresolved) {
      char* name = sym_use->var->name;
      
      if (!strcmp(name, "__primitive")) {
        return;
      }

      Symbol* sym_resolve = Symboltable::lookup(name);

      // Check VarSymbols are defined before used
      if (VarSymbol* var_resolve = dynamic_cast<VarSymbol*>(sym_resolve)) {
        // If it is not an access of a member in a class
        if (var_resolve->parentScope->type != SCOPE_CLASS) {
          // And if it is not imported from a different module
          if (var_resolve->getModule() ==
              Symboltable::getCurrentScope()->getModule()) {
            Vec<VarSymbol*>* sym_defs = defList->get(var_resolve->parentScope);
            if (!sym_defs || !sym_defs->set_in(var_resolve)) {
              USR_FATAL(expr, "Variable '%s' used before it is defined", name);
            }
          }
        }
      }

      if (sym_resolve) {
        if (!dynamic_cast<FnSymbol*>(sym_resolve)) {
          sym_use->var = sym_resolve;
        }
      } else {
        if (!strcmp(name, "domain"))
          return;
        USR_FATAL(expr, "Symbol '%s' is not defined", name);
      }
    }
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (VarSymbol* sym = dynamic_cast<VarSymbol*>(def_expr->sym)) {
      Vec<VarSymbol*>* syms = defList->get(Symboltable::getCurrentScope());
      if (syms) {
        syms->set_add(sym);
      } else {
        syms = new Vec<VarSymbol*>();
        syms->set_add(sym);
        defList->put(Symboltable::getCurrentScope(), syms);
      }
    }
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->retType == dtUnknown &&
          def_expr->exprType &&
          def_expr->exprType->typeInfo() != dtUnknown) {
        fn->retType = def_expr->exprType->typeInfo();
        def_expr->exprType = NULL;
      }
    } else if (def_expr->sym->type == dtUnknown &&
               def_expr->exprType &&
               def_expr->exprType->typeInfo() != dtUnknown) {
      def_expr->sym->type = def_expr->exprType->typeInfo();
      def_expr->exprType = NULL;
    }
  }

  if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    if (castExpr->type == dtUnknown &&
        castExpr->newType &&
        castExpr->newType->typeInfo() != dtUnknown) {
      castExpr->type = castExpr->newType->typeInfo();
      castExpr->newType = NULL;
    }
  }
}
