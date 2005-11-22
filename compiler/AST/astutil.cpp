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
      if (call->findFnSymbol()) {
        if (call->findFnSymbol()->calledBy) { // yuck, got some
                                              // functions being
                                              // called that are no
                                              // longer in the tree,
                                              // e.g., _INIT_CONFIG
          call->findFnSymbol()->calledBy->add(call);
          call->parentFunction()->calls->add(call);
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
