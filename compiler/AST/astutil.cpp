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
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast))
      fns->add(a);
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
  forv_Vec(ModuleSymbol, mod, allModules)
    collect_asts(asts, mod);
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
  forv_Vec(ModuleSymbol, mod, allModules)
    collect_asts_postorder(asts, mod);
}


void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    if (!dynamic_cast<Symbol*>(next_ast))
      collect_top_asts(asts, next_ast);
  }
  asts->add(ast);
}


void collect_top_asts(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(Expr, expr, ast) {
    collect_top_asts(asts, expr);
  }
}


void collect_asts(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(Expr, expr, ast) {
    collect_asts(asts, expr);
  }
}


void collect_asts_postorder(Vec<BaseAST*>* asts, AList* ast) {
  for_alist(Expr, expr, ast) {
    collect_asts_postorder(asts, expr);
  }
}


void reset_file_info(BaseAST* baseAST, int lineno, char* filename) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, baseAST);
  forv_Vec(BaseAST, ast, asts) {
    ast->lineno = lineno;
    ast->filename = filename;
  }
}


void reset_file_info(AList* exprs, int lineno, char* filename) {
  for_alist(Expr, expr, exprs) {
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
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      if (fn->calledBy)
        fn->calledBy->clear();
      else
        fn->calledBy = new Vec<CallExpr*>();
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
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
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      if (a->sym->astType == SYMBOL_VAR || a->sym->astType == SYMBOL_ARG) {
        def_set.set_add(a);
        a->sym->uses.clear();
        a->sym->defs.clear();
      }
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* a = dynamic_cast<SymExpr*>(ast)) {
      if (a->var->defPoint && def_set.set_in(a->var->defPoint)) {
        if (CallExpr* call = dynamic_cast<CallExpr*>(a->parentExpr)) {
          if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == a) {
            a->var->defs.add(a);
            continue;
          } else if (call->primitive &&
                     !strcmp(call->primitive->name, "fscanf") &&
                     call->get(3) == a) {
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
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      defExpr->sym->type = dtUnknown;

      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
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
      if (_t* _y = dynamic_cast<_t*>(b)) {         \
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
    if (SymExpr* sym_expr = dynamic_cast<SymExpr*>(ast)) {
      XSUB(sym_expr->var, Symbol);
    } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      XSUB(defExpr->sym->type, Type);
    } else if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(ast)) {
      XSUB(goto_stmt->label, LabelSymbol);
    } else if (VarSymbol* ps = dynamic_cast<VarSymbol*>(ast)) {
      XSUB(ps->type, Type);
    } else if (FnSymbol* ps = dynamic_cast<FnSymbol*>(ast)) {
      XSUB(ps->type, Type);
      XSUB(ps->retType, Type);
      XSUB(ps->_this, Symbol);
    } else if (ArgSymbol* ps = dynamic_cast<ArgSymbol*>(ast)) {
      XSUB(ps->type, Type);
    }
  }
}


void sibling_insert_help(BaseAST* sibling, BaseAST* ast) {
  Expr* parentExpr = NULL;
  Symbol* parentSymbol = NULL;
  SymScope* parentScope = NULL;
  if (Expr* expr = dynamic_cast<Expr*>(sibling)) {
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
  if (Expr* expr = dynamic_cast<Expr*>(parent)) {
    parentExpr = expr;
    parentSymbol = expr->parentSymbol;
    BlockStmt* block = dynamic_cast<BlockStmt*>(expr);
    if (block && block->blkScope)
      parentScope = block->blkScope;
    else
      parentScope = expr->parentScope;
  } else if (Symbol* symbol = dynamic_cast<Symbol*>(parent)) {
    parentSymbol = symbol;
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(symbol))
      parentScope = fn->argScope;
    else if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(symbol))
      parentScope = mod->block->blkScope;
    else if (ClassType* ct = dynamic_cast<ClassType*>(symbol->type))
      parentScope = ct->structScope;
    else
      parentScope = symbol->parentScope;
  } else if (Type* type = dynamic_cast<Type*>(parent)) {
    parentSymbol = type->symbol;
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(type->symbol))
      parentScope = fn->argScope;
    else if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(symbol))
      parentScope = mod->block->blkScope;
    else if (ClassType* ct = dynamic_cast<ClassType*>(type))
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
  if (dynamic_cast<ModuleSymbol*>(ast))
    return;

  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = parentScope;
    expr->parentSymbol = parentSymbol;
    expr->parentExpr = parentExpr;

    if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(expr)) {
      if (blockStmt->blockTag != BLOCK_SCOPELESS) {
        if (blockStmt->blkScope &&
            blockStmt->blkScope->astParent == blockStmt)
          INT_FATAL(blockStmt, "Unexpected scope in BlockStmt");
        if (!blockStmt->blkScope) {
          blockStmt->blkScope = new SymScope(blockStmt, parentScope);
          blockStmt->blkScope->astParent = blockStmt;
        }
        parentScope = blockStmt->blkScope;
      }
    }
    if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
      if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(def_expr->sym)) {
        ModuleSymbol* outer = dynamic_cast<ModuleSymbol*>(def_expr->parentSymbol);
        mod->defPoint->sym = NULL;
        mod->defPoint = NULL;
        parentExpr->remove();
        if (!outer)
          USR_FATAL(mod, "Nested module is not defined at module level");
        mod->initFn->insertAtHead(new CallExpr(PRIMITIVE_USE, new SymExpr(outer->name)));
        parentScope = mod->block->blkScope;
      } else {
        if (def_expr->sym && !dynamic_cast<UnresolvedSymbol*>(def_expr->sym)) {
          def_expr->parentScope->define(def_expr->sym);
        }
      }
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
        if (fn->argScope)
          INT_FATAL(fn, "Unexpected scope in FnSymbol");
        fn->argScope = new SymScope(fn, parentScope);
        fn->argScope->astParent = fn;
        parentScope = fn->argScope;
      }
      if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
        if (ClassType* type = dynamic_cast<ClassType*>(typeSym->type)) {
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
  if (dynamic_cast<ModuleSymbol*>(ast))
    return;

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, a, asts)
    remove_help(a);

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = NULL;
    expr->parentSymbol = NULL;
    expr->parentExpr = NULL;
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->blockTag != BLOCK_SCOPELESS) {
        if (block->blkScope) {
          if (block->blkScope->astParent == block) {
            delete block->blkScope;
            block->blkScope = NULL;
          }
        }
      }
    }
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (defExpr->sym && !dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
        if (defExpr->sym->parentScope)
          defExpr->sym->parentScope->undefine(defExpr->sym);
        defExpr->sym->parentScope = NULL;
      }
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
        if (fn->argScope) {
          delete fn->argScope;
          fn->argScope = NULL;
        }
      }
      if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        if (ClassType* type = dynamic_cast<ClassType*>(typeSym->type)) {
          if (type->structScope) {
            delete type->structScope;
            type->structScope = NULL;
          }
        }
      }
    }
  }
}


// Return the corresponding Symbol in the formal list of the actual a
ArgSymbol*
actual_to_formal(Expr *a) {
  if (CallExpr *call = dynamic_cast<CallExpr*>(a->parentExpr)) {
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
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
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
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (FnSymbol* next = call->isResolved())
        if (!fns.set_in(next))
          pruneVisit(next, fns, types);
    } else if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
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
      if (dynamic_cast<ClassType*>(ts->type)) {
        //        printf("removing %s\n", ts->cname);
        ts->defPoint->remove();
      }
    }
  }
}
