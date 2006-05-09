#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "../traversals/traversal.h"


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
  if (Type* type = dynamic_cast<Type*>(ast)) {
    asts->add(type->metaType);
    asts->add(type->metaType->symbol);
  }
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
  if (Type* type = dynamic_cast<Type*>(ast)) {
    asts->add(type->metaType);
    asts->add(type->metaType->symbol);
  }
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
    } else if (CastExpr* castExpr = dynamic_cast<CastExpr*>(ast)) {
      XSUB(castExpr->type, Type);
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
      int constructor = 0;
      if (ClassType *ct = dynamic_cast<ClassType*>(fn->typeBinding))
        if (ct->defaultConstructor == fn)
          constructor = 1;
        DefExpr* formalDef = fn->formals->first();
        for_alist(Expr, actual, call->argList) {
          ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
          if (formal->intent == INTENT_TYPE ||
              (!constructor && (formal->type == dtMethodToken ||
                               formal->type == dtSetterToken)))
            actual->remove();
          formalDef = fn->formals->next();
        }
        if (!strcmp(fn->name, "_chpl_alloc"))
          call->get(1)->remove();
      }
    }
  }
}


void remove_static_formals() {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      int constructor = 0;
      if (fn->typeBinding)
        if (ClassType *ct = dynamic_cast<ClassType*>(fn->typeBinding->definition))
          if (ct->defaultConstructor == fn)
            constructor = 1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        if (formal->intent == INTENT_TYPE ||
            (!constructor && (formal->type == dtMethodToken ||
                              formal->type == dtSetterToken)))
          formalDef->remove();
      }
    }
  }
}
