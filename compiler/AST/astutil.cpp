#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "../traversals/traversal.h"


class CollectFunctions : public Traversal {
 public:
  Vec<FnSymbol*>* fns;
  CollectFunctions(Vec<FnSymbol*>* iFns) : fns(iFns) { }
  void postProcessExpr(Expr* expr) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr))
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym))
        fns->add(fn);
  }
};

void collect_functions(Vec<FnSymbol*>* fns) {
  CollectFunctions* traversal = new CollectFunctions(fns);
  traversal->run(&allModules);
}


class PreCollectAST : public Traversal {
 public:
  Vec<BaseAST*>* asts;
  PreCollectAST(Vec<BaseAST*>* iAsts) : asts(iAsts) { }
  void preProcessStmt(Stmt* stmt) {
    asts->add(stmt);
  }
  void preProcessExpr(Expr* expr) {
    asts->add(expr);
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      asts->add(def->sym);
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym))
        if (ts->definition)
          asts->add(ts->definition);
    }
  }
};

class PostCollectAST : public Traversal {
 public:
  Vec<BaseAST*>* asts;
  PostCollectAST(Vec<BaseAST*>* iAsts) : asts(iAsts) { }
  void postProcessStmt(Stmt* stmt) {
    asts->add(stmt);
  }
  void postProcessExpr(Expr* expr) {
    asts->add(expr);
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      asts->add(def->sym);
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym))
        if (ts->definition)
          asts->add(ts->definition);
    }
  }
};

void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  TRAVERSE(ast, new PreCollectAST(asts), true);
  asts->add(ast);
}

void collect_asts(Vec<BaseAST*>* asts) {
  Traversal* traversal = new PreCollectAST(asts);
  traversal->run(&allModules);
}

void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast) {
  TRAVERSE(ast, new PostCollectAST(asts), true);
  asts->add(ast);
}

void collect_asts_postorder(Vec<BaseAST*>* asts) {
  Traversal* traversal = new PostCollectAST(asts);
  traversal->run(&allModules);
}


// utility routines for clearing and resetting lineno and filename
class ClearFileInfo : public Traversal {
public:
  int lineno;
  char* filename;

  ClearFileInfo(int iLineno = -1, char* iFilename = "<internal>") :
    lineno(iLineno), filename(iFilename) { }

  void preProcessStmt(Stmt* stmt) {
    stmt->lineno = lineno;
    stmt->filename = filename;
  }

  void preProcessExpr(Expr* expr) {
    expr->lineno = lineno;
    expr->filename = filename;
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      def->sym->lineno = lineno;
      def->sym->filename = filename;
    }
  }
};


void clear_file_info(BaseAST* baseAST) {
  TRAVERSE(baseAST, new ClearFileInfo(), true);
}


void reset_file_info(BaseAST* baseAST, int lineno, char* filename) {
  TRAVERSE(baseAST, new ClearFileInfo(lineno, filename), true);
}


// compute call sites FnSymbol::calls

class ComputeCallSites : public Traversal {
 public:
  void preProcessExpr(Expr* expr) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (!call->primitive && call->opTag == OP_NONE && call->findFnSymbol()) {
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
};


void compute_call_sites() {
  Vec<FnSymbol*> fs;
  collect_functions(&fs);
  forv_Vec(FnSymbol, f, fs) {
    f->calledBy = new Vec<CallExpr*>();
    f->calls = new Vec<CallExpr*>();
  }
  Pass* pass = new ComputeCallSites();
  pass->run(Symboltable::getModules(MODULES_ALL));
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
    } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(ast)) {
      XSUB(memberAccess->member, Symbol);
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
