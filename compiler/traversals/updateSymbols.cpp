#include "updateSymbols.h"
#include "expr.h"
#include "symbol.h"
#include "stringutil.h"

UpdateSymbols::UpdateSymbols(Map<BaseAST*,BaseAST*>* init_copy_map) {
  copy_map = init_copy_map;
  /** Prune for symbols? **/ /** Performance reasons --SJD */
  /** If so, prune a local copy! **/ /** Also make it a hash */
  /** types too now **/
}


void UpdateSymbols::preProcessExpr(Expr* expr) {
  if (Variable* sym_expr = dynamic_cast<Variable*>(expr)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == sym_expr->var) {
        if (Symbol* new_sym = dynamic_cast<Symbol*>(copy_map->v[i].value)) {
          sym_expr->var = new_sym;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == defExpr->sym->type) {
        if (Type* new_type = dynamic_cast<Type*>(copy_map->v[i].value)) {
          defExpr->sym->type = new_type;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  } else if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == castExpr->newType) {
        if (Type* new_type = dynamic_cast<Type*>(copy_map->v[i].value)) {
          castExpr->newType = new_type;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == memberAccess->member) {
        if (Symbol* new_sym = dynamic_cast<Symbol*>(copy_map->v[i].value)) {
          memberAccess->member = new_sym;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  }
}


void UpdateSymbols::preProcessSymbol(Symbol* sym) {
  if (!dynamic_cast<TypeSymbol*>(sym)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == sym->type) {
        if (Type* new_type = dynamic_cast<Type*>(copy_map->v[i].value)) {
          sym->type = new_type;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  }
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    for (int i = 0; i < copy_map->n; i++) {
      if (copy_map->v[i].key == fn->retType) {
        if (Type* new_type = dynamic_cast<Type*>(copy_map->v[i].value)) {
          fn->retType = new_type;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
    if (fn->_this) {
      for (int i = 0; i < copy_map->n; i++) {
        if (copy_map->v[i].key == fn->_this) {
          if (Symbol* new_sym = dynamic_cast<Symbol*>(copy_map->v[i].value)) {
            fn->_this = new_sym;
          } else {
            INT_FATAL("Major error in UpdateSymbols");
          }
        }
      }
    }
    if (fn->_setter) {
      for (int i = 0; i < copy_map->n; i++) {
        if (copy_map->v[i].key == fn->_setter) {
          if (VarSymbol* new_sym = dynamic_cast<VarSymbol*>(copy_map->v[i].value)) {
            fn->_setter = new_sym;
          } else {
            INT_FATAL("Major error in UpdateSymbols");
          }
        }
      }
    }
    if (fn->_getter) {
      for (int i = 0; i < copy_map->n; i++) {
        if (copy_map->v[i].key == fn->_getter) {
          if (VarSymbol* new_sym = dynamic_cast<VarSymbol*>(copy_map->v[i].value)) {
            fn->_getter = new_sym;
          } else {
            INT_FATAL("Major error in UpdateSymbols");
          }
        }
      }
    }
  }
}
