#include "alist.h"
#include "type.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"

void addToRootSet(FnSymbol* fn, Expr* expr);
void buildRootSetForFunction(FnSymbol* fn, Expr* base, DefExpr* def);

void addToRootSet(FnSymbol* fn, Expr* expr) {
  fn->insertAtHead(new CallExpr(PRIMITIVE_GC_ADD_ROOT, expr));
  fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_GC_DELETE_ROOT));
}


/*
  If all is well:
    * fn is never null
    * if base and def are non-null, base is a reference chain to the
      record to be scanned (ie rec1.rec2.rec3), and def is the def
      of that record - in this case rec3.
    * if base is non-null and def is null, base is a BlockStmt which is
      a sub-block of fn.
*/
void buildRootSetForFunction(FnSymbol* fn, Expr* base, DefExpr* def) {
  if (!base) {
    /* This is the first level of recursion.  Scan the formals. */
    for_formals(formal, fn) {
      if (ClassType* ct = dynamic_cast<ClassType*>(formal->type)) {
        if (ct->classTag == CLASS_CLASS) {
          addToRootSet(fn, new SymExpr(formal));
        } else if (ct->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new SymExpr(formal), formal->defPoint);
        }
      }
    }
    /* Now scan the Function body */
    for_asts(expr, fn->body->body) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
          addToRootSet(fn, new SymExpr(def->sym));
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def);
          }
        }
      } else if (BlockStmt* blk = dynamic_cast<BlockStmt*>(expr)) {
        buildRootSetForFunction(fn, blk, NULL);
      }
    }
  } else if (!def) {
    /* Looking at a BlockStmt */
    BlockStmt* blk = dynamic_cast<BlockStmt*>(base);
    assert(blk);
    for_asts(ast, blk->body) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            addToRootSet(fn, new SymExpr(def->sym));
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def);
          }
        }
      }
    }
  } else {
    /* Looking at a record. */
    ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
    assert(ct);
    for_fields(field, ct) {
      if (ClassType* classfield = dynamic_cast<ClassType*>(field->type)) {
        if (classfield->classTag == CLASS_CLASS) {
          addToRootSet(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                           base->copy(),
                                           new SymExpr(field)));
        } else if (classfield->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                                   base->copy(),
                                                   new SymExpr(field)),
                                  field->defPoint);
        }
      }
    }
  }
}


void buildRootSetForModule(ModuleSymbol* module) {
  for_alist(Expr, expr, module->block->body) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<VarSymbol*>(def->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            addToRootSet(chpl_main, new SymExpr(def->sym));
          }
        }
      }
    }
  }
}


void copyCollection(void) {
  if (!copyCollect)
    return;

  chpl_main->insertAtHead(new CallExpr(PRIMITIVE_GC_CC_INIT,
                                       new_IntSymbol(2097152)));

  forv_Vec(FnSymbol, fn, gFns) {
    buildRootSetForFunction(fn, NULL, NULL);
  }

  forv_Vec(ModuleSymbol, mod, allModules) {
    buildRootSetForModule(mod);
  }
}

