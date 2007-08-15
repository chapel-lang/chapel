#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"


void collect_functions(Vec<FnSymbol*>* fns) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts)
    if (FnSymbol* a = toFnSymbol(ast))
      fns->add(a);
}

void collect_stmts(Vec<Expr*>* exprs, Expr* expr) {
  exprs->add(expr);
  if (expr->astType == STMT_BLOCK || expr->astType == STMT_COND) {
    Vec<BaseAST*> next_asts;
    get_ast_children(expr, next_asts);
    forv_Vec(BaseAST, next_ast, next_asts) {
      if (Expr* expr = toExpr(next_ast))
        collect_stmts(exprs, expr);
    }
  }
}

void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  asts->add(ast);
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    collect_asts(asts, next_ast);
  }
}

void collect_asts(Vec<BaseAST*>* asts) {
  collect_asts(asts, theProgram);
}

void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast) {
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    collect_asts_postorder(asts, next_ast);
  }
  asts->add(ast);
}

void collect_asts_postorder(Vec<BaseAST*>* asts) {
  collect_asts_postorder(asts, theProgram);
}


void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    if (!toSymbol(next_ast))
      collect_top_asts(asts, next_ast);
  }
  asts->add(ast);
}


void collect_top_asts(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(expr, ast) {
    collect_top_asts(asts, expr);
  }
}


void collect_asts(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(expr, ast) {
    collect_asts(asts, expr);
  }
}


void collect_asts_postorder(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(expr, ast) {
    collect_asts_postorder(asts, expr);
  }
}


void reset_file_info(BaseAST* baseAST, int lineno, const char* filename) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, baseAST);
  forv_Vec(BaseAST, ast, asts) {
    ast->lineno = lineno;
    ast->filename = filename;
  }
}


void reset_file_info(AList* exprs, int lineno, const char* filename) {
  for_alist(expr, exprs) {
    reset_file_info(expr, lineno, filename);
  }
}


void clear_file_info(BaseAST* baseAST) {
  reset_file_info(baseAST, -1, "<internal>");
}


void compute_call_sites() {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = toFnSymbol(ast)) {
      if (fn->calledBy)
        fn->calledBy->clear();
      else
        fn->calledBy = new Vec<CallExpr*>();
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (!call->primitive && call->findFnSymbol()) {
        if (call->findFnSymbol()->calledBy) { // yuck, got some
                                              // functions being
                                              // called that are no
                                              // longer in the tree,
                                              // e.g., _INIT_CONFIG
          call->findFnSymbol()->calledBy->add(call);
        }
      }
    }
  }
}


void compute_sym_uses(BaseAST* base) {
  Vec<DefExpr*> def_set;
  Vec<BaseAST*> asts;
  if (base == NULL)
    collect_asts(&asts);
  else
    collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = toDefExpr(ast)) {
      if (a->sym->astType == SYMBOL_VAR || a->sym->astType == SYMBOL_ARG) {
        def_set.set_add(a);
        a->sym->uses.clear();
        a->sym->defs.clear();
      }
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* a = toSymExpr(ast)) {
      if (a->var->defPoint && def_set.set_in(a->var->defPoint)) {
        if (CallExpr* call = toCallExpr(a->parentExpr)) {
          if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == a) {
            a->var->defs.add(a);
            continue;
          } else if (call->isResolved()) {
            ArgSymbol* arg = actual_to_formal(a);
            if (arg->intent == INTENT_OUT) {
              a->var->defs.add(a);
              continue;
            }
            if (arg->intent == INTENT_REF ||
                arg->intent == INTENT_INOUT)
              a->var->defs.add(a); // also use
          }
        }
        a->var->uses.add(a);
      }
    }
  }
}


void clear_type_info(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* defExpr = toDefExpr(ast)) {
      defExpr->sym->type = dtUnknown;

      if (FnSymbol* fn = toFnSymbol(defExpr->sym)) {
        for_formals(tmp, fn) {
          tmp->type = dtUnknown;
        }
        fn->retType = dtUnknown;
      }
    }
  }
}


#define XSUB(_x, _t)                               \
  if (_x) {                                        \
    if (BaseAST *b = map->get(_x)) {               \
      if (_t* _y = to##_t(b)) {                    \
        _x = _y;                                   \
      } else {                                     \
        INT_FATAL("Major error in update_symbols"); \
      }                                            \
    }                                              \
  }

void update_symbols(BaseAST* ast, ASTMap* map) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, ast);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* sym_expr = toSymExpr(ast)) {
      XSUB(sym_expr->var, Symbol);
    } else if (DefExpr* defExpr = toDefExpr(ast)) {
      XSUB(defExpr->sym->type, Type);
    } else if (GotoStmt* goto_stmt = toGotoStmt(ast)) {
      XSUB(goto_stmt->label, LabelSymbol);
    } else if (VarSymbol* ps = toVarSymbol(ast)) {
      XSUB(ps->type, Type);
    } else if (FnSymbol* ps = toFnSymbol(ast)) {
      XSUB(ps->type, Type);
      XSUB(ps->retType, Type);
      XSUB(ps->_this, Symbol);
    } else if (ArgSymbol* ps = toArgSymbol(ast)) {
      XSUB(ps->type, Type);
    }
  }
}


void sibling_insert_help(BaseAST* sibling, BaseAST* ast) {
  Expr* parentExpr = NULL;
  Symbol* parentSymbol = NULL;
  SymScope* parentScope = NULL;
  if (Expr* expr = toExpr(sibling)) {
    parentExpr = expr->parentExpr;
    parentSymbol = expr->parentSymbol;
    parentScope = expr->parentScope;
  } else if (sibling)
    INT_FATAL(ast, "major error in sibling_insert_help");
  if (parentSymbol)
    insert_help(ast, parentExpr, parentSymbol, parentScope);
}


void parent_insert_help(BaseAST* parent, Expr* ast) {
  if (!parent || !parent->inTree())
    return;
  Expr* parentExpr = NULL;
  Symbol* parentSymbol = NULL;
  SymScope* parentScope = NULL;
  if (Expr* expr = toExpr(parent)) {
    parentExpr = expr;
    parentSymbol = expr->parentSymbol;
    BlockStmt* block = toBlockStmt(expr);
    if (block && block->blkScope)
      parentScope = block->blkScope;
    else
      parentScope = expr->parentScope;
  } else if (Symbol* symbol = toSymbol(parent)) {
    parentSymbol = symbol;
    if (FnSymbol* fn = toFnSymbol(symbol))
      parentScope = fn->argScope;
    else if (ModuleSymbol* mod = toModuleSymbol(symbol))
      parentScope = mod->block->blkScope;
    else if (ClassType* ct = toClassType(symbol->type))
      parentScope = ct->structScope;
    else
      parentScope = symbol->parentScope;
  } else if (Type* type = toType(parent)) {
    parentSymbol = type->symbol;
    if (FnSymbol* fn = toFnSymbol(type->symbol))
      parentScope = fn->argScope;
    else if (ModuleSymbol* mod = toModuleSymbol(symbol))
      parentScope = mod->block->blkScope;
    else if (ClassType* ct = toClassType(type))
      parentScope = ct->structScope;
    else
      parentScope = type->symbol->parentScope;
  } else if (parent)
    INT_FATAL(ast, "major error in parent_insert_help");
  insert_help(ast, parentExpr, parentSymbol, parentScope);
}


void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Symbol* parentSymbol,
                 SymScope* parentScope) {
  if (ModuleSymbol* mod = toModuleSymbol(ast)) {
      mod->parentScope = parentScope;
      parentSymbol = mod;
      parentExpr = NULL;
  } else if (Symbol* sym = toSymbol(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
  }

  if (Expr* expr = toExpr(ast)) {
    expr->parentScope = parentScope;
    expr->parentSymbol = parentSymbol;
    expr->parentExpr = parentExpr;

    if (BlockStmt* blockStmt = toBlockStmt(expr)) {
      if (blockStmt->blockTag != BLOCK_SCOPELESS) {
        if (blockStmt->blkScope &&
            blockStmt->blkScope->astParent == blockStmt)
          INT_FATAL(blockStmt, "Unexpected scope in BlockStmt");
        blockStmt->blkScope = new SymScope(blockStmt, parentScope);
        parentScope = blockStmt->blkScope;
      }
    }
    if (DefExpr* def_expr = toDefExpr(expr)) {
      if (ModuleSymbol* mod = toModuleSymbol(def_expr->sym)) {
        ModuleSymbol* outer = toModuleSymbol(def_expr->parentSymbol);
        if (!outer)
          USR_FATAL(mod, "Nested module is not defined at module level");
        def_expr->parentScope->define(def_expr->sym);
      } else {
        if (def_expr->sym && !toUnresolvedSymbol(def_expr->sym)) {
          def_expr->parentScope->define(def_expr->sym);
        }
      }
      if (FnSymbol* fn = toFnSymbol(def_expr->sym)) {
        if (fn->argScope)
          INT_FATAL(fn, "Unexpected scope in FnSymbol");
        fn->argScope = new SymScope(fn, parentScope);
        parentScope = fn->argScope;
      }
      if (TypeSymbol* typeSym = toTypeSymbol(def_expr->sym)) {
        if (ClassType* type = toClassType(typeSym->type)) {
          if (type->structScope)
            INT_FATAL(typeSym, "Unexpected scope in FnSymbol");
          type->structScope = new SymScope(type, parentScope);
          parentScope = type->structScope;
        }
      }
    }
    parentExpr = expr;
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    insert_help(ast, parentExpr, parentSymbol, parentScope);
}


void remove_help(BaseAST* ast) {
  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, a, asts)
    remove_help(a);

  if (Expr* expr = toExpr(ast)) {
    expr->parentScope = NULL;
    expr->parentSymbol = NULL;
    expr->parentExpr = NULL;
    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag != BLOCK_SCOPELESS) {
        if (block->blkScope) {
          if (block->blkScope->astParent == block) {
            delete block->blkScope;
            block->blkScope = NULL;
          }
        }
      }
    }
    if (DefExpr* defExpr = toDefExpr(ast)) {
      if (defExpr->sym) {
        if (defExpr->sym->parentScope)
          defExpr->sym->parentScope->undefine(defExpr->sym);
        defExpr->sym->parentScope = NULL;
      }
      if (FnSymbol* fn = toFnSymbol(defExpr->sym)) {
        if (fn->argScope) {
          delete fn->argScope;
          fn->argScope = NULL;
        }
      }
      if (TypeSymbol* typeSym = toTypeSymbol(defExpr->sym)) {
        if (ClassType* type = toClassType(typeSym->type)) {
          if (type->structScope) {
            delete type->structScope;
            type->structScope = NULL;
          }
        }
      }
      if (ModuleSymbol* mod = toModuleSymbol(defExpr->sym)) {
        if (mod->block->blkScope) {
          delete mod->block->blkScope;
          mod->block->blkScope = NULL;
        }
      }
    }
  }
}


// Return the corresponding Symbol in the formal list of the actual a
ArgSymbol*
actual_to_formal(Expr *a) {
  if (CallExpr *call = toCallExpr(a->parentExpr)) {
    if (call->isResolved()) {
      for_formals_actuals(formal, actual, call) {
        if (a == actual)
          return formal;
      }
    }
  }
  INT_FATAL(a, "bad call to actual_to_formals");
  return NULL;
}


static void
pruneVisit(TypeSymbol* ts, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  types.set_add(ts);
  Vec<BaseAST*> asts;
  collect_asts(&asts, ts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (def->sym->type && !types.set_in(def->sym->type->symbol))
        pruneVisit(def->sym->type->symbol, fns, types);
  }
}


static void
pruneVisit(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  fns.set_add(fn);
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* next = call->isResolved())
        if (!fns.set_in(next))
          pruneVisit(next, fns, types);
    } else if (SymExpr* se = toSymExpr(ast)) {
      if (se->var->type && !types.set_in(se->var->type->symbol))
        pruneVisit(se->var->type->symbol, fns, types);
    }
  }
}


void
prune() {
  Vec<FnSymbol*> fns;
  Vec<TypeSymbol*> types;
  pruneVisit(chpl_main, fns, types);
  forv_Vec(FnSymbol, fn, gFns) {
    if (!fns.set_in(fn)) {
      //      printf("removing %s\n", fn->cname);
      fn->defPoint->remove();
    }
  }
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (!types.set_in(ts)) {
      if (toClassType(ts->type)) {
        //        printf("removing %s\n", ts->cname);
        ts->defPoint->remove();
      }
    }
  }
}
