#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"


void build(BaseAST* ast) {
  cleanup(ast);
  scopeResolve(ast);
  normalize(ast);
}

void collect_functions(Vec<FnSymbol*>* fns) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts)
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast))
      fns->add(a);
}

void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  asts->add(ast);
  if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
    if (dynamic_cast<UnresolvedSymbol*>(sym->var))
      asts->add(sym->var);
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var))
      if (var->immediate)
        asts->add(sym->var);
  }
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
  if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
    if (dynamic_cast<UnresolvedSymbol*>(sym->var))
      asts->add(sym->var);
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var))
      if (var->immediate)
        asts->add(sym->var);
  }
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
  if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
    if (dynamic_cast<UnresolvedSymbol*>(sym->var))
      asts->add(sym->var);
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var))
      if (var->immediate)
        asts->add(sym->var);
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


void clear_file_info(BaseAST* baseAST) {
  reset_file_info(baseAST, -1, "<internal>");
}


void compute_call_sites() {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      fn->calledBy = new Vec<CallExpr*>();
      fn->calls = new Vec<CallExpr*>();
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
          call->getFunction()->calls->add(call);
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
      def_set.set_add(a);
      a->sym->uses = new Vec<SymExpr*>();
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* a = dynamic_cast<SymExpr*>(ast)) {
      if (a->var->defPoint && def_set.set_in(a->var->defPoint)) {
        a->var->uses->add(a);
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
        for_alist(DefExpr, tmp, fn->formals) {
          tmp->sym->type = dtUnknown;
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
      XSUB(ps->_setter, Symbol);
      XSUB(ps->_getter, Symbol);
    } else if (ArgSymbol* ps = dynamic_cast<ArgSymbol*>(ast)) {
      XSUB(ps->type, Type);
      if (ps->isGeneric && ps->genericSymbol) {
        BaseAST *b = map->get(ps->genericSymbol);
        if (b) {
          if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(b)) {
            if (ts->definition->astType != TYPE_VARIABLE)
              ps->isGeneric = 0;
            ps->genericSymbol = ts;
          } else {
            INT_FATAL("Major error in update_symbols");
          }
        }
      }
    }
  }
}


void remove_named_exprs() {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (NamedExpr* named = dynamic_cast<NamedExpr*>(ast)) {
      Expr* actual = named->actual;
      actual->remove();
      named->replace(actual);
    }
  }
}


void remove_static_actuals() {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        DefExpr* formalDef = fn->formals->first();
        for_alist(Expr, actual, call->argList) {
          ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
          if (formal->intent == INTENT_TYPE ||
              (fn->fnClass != FN_CONSTRUCTOR && (formal->type == dtMethodToken ||
                                                 formal->type == dtSetterToken)))
            actual->remove();
          formalDef = fn->formals->next();
        }
      }
    }
  }
}


void remove_static_formals() {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        if (formal->intent == INTENT_TYPE ||
            (fn->fnClass != FN_CONSTRUCTOR && (formal->type == dtMethodToken ||
                                               formal->type == dtSetterToken)))
          formalDef->remove();
      }
    }
  }
}


void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Stmt* parentStmt,
                 Symbol* parentSymbol,
                 SymScope* parentScope) {
  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
    parentStmt = NULL;
  }

  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    stmt->parentScope = parentScope;
    stmt->parentSymbol = parentSymbol;
    stmt->parentStmt = parentStmt;

    if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
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
    parentStmt = stmt;
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = parentScope;
    expr->parentSymbol = parentSymbol;
    expr->parentStmt = parentStmt;
    expr->parentExpr = parentExpr;

    if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
      if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(def_expr->sym)) {
        ModuleSymbol* outer = dynamic_cast<ModuleSymbol*>(def_expr->parentSymbol);
        parentStmt->remove();
        if (!outer)
          USR_FATAL(mod, "Nested module is not defined at module level");
        mod->initFn->insertAtHead(new CallExpr(PRIMITIVE_USE, new SymExpr(outer->name)));
        parentScope = mod->modScope;
      } else {
        if (def_expr->sym && !def_expr->sym->isUnresolved) {
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
        if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
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
  get_ast_children(ast, asts, 0, 1);
  forv_Vec(BaseAST, ast, asts)
    insert_help(ast, parentExpr, parentStmt, parentSymbol, parentScope);
}


void remove_help(BaseAST* ast) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    stmt->parentScope = NULL;
    stmt->parentSymbol = NULL;
    stmt->parentStmt = NULL;
  }
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    expr->parentScope = NULL;
    expr->parentSymbol = NULL;
    expr->parentStmt = NULL;
    expr->parentExpr = NULL;
  }
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
    if (!dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
      if (defExpr->sym->parentScope)
        defExpr->sym->parentScope->undefine(defExpr->sym);
      defExpr->sym->parentScope = NULL;
    }
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    remove_help(ast);

  if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
    if (block->blockTag != BLOCK_SCOPELESS) {
      if (block->blkScope && block->blkScope->astParent == block)
        block->blkScope = NULL;
    }
  }
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
      fn->argScope = NULL;
    }
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (ClassType* type = dynamic_cast<ClassType*>(typeSym->definition)) {
        type->structScope = NULL;
      }
    }
  }
}


// Return the corresponding Symbol in the formal list of the actual a
Symbol*
actual_to_formal( Expr *a) {
  if (SymExpr *se = dynamic_cast<SymExpr*>(a)) {
    if (CallExpr *ce = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (SymExpr *fe = dynamic_cast<SymExpr*>(ce->baseExpr)) {
        if (FnSymbol *f = dynamic_cast<FnSymbol*>(fe->var)) {
          AList<Expr>    *actuals = ce->argList;
          AList<DefExpr> *formals = f->formals;
          if (actuals->length() != formals->length()) {
            INT_FATAL( "length of actuals and formals not the same");
          }

          Expr *e = actuals->first();
          for_alist( DefExpr, farg, formals) {
            if (a == e) {
              return (ArgSymbol*)(farg->sym);
            }
            e = actuals->next();
          }
        }
      }
    }
  }

  INT_FATAL( "shouldn't get here");
  return NULL;
}
