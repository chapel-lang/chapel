#include "alist.h"
#include "type.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"
#include "astutil.h"

void addToRootSet(FnSymbol* fn, Expr* expr, bool nullify);
void buildRootSetForFunction(FnSymbol* fn, Expr* base, DefExpr* def, bool nullify);

void addToRootSet(FnSymbol* fn, Expr* expr, bool nullify) {
  if (nullify)
    fn->insertAtHead(new CallExpr(PRIMITIVE_GC_ADD_NULL_ROOT, expr));
  else
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
void buildRootSetForFunction(FnSymbol* fn, Expr* base, DefExpr* def, bool nullify) {
  if (!base) {
    /* This is the first level of recursion.  Scan the formals. */
    for_formals(formal, fn) {
      if (ClassType* ct = dynamic_cast<ClassType*>(formal->type)) {
        if (ct->classTag == CLASS_CLASS) {
          addToRootSet(fn, new SymExpr(formal), false);
        } else if (ct->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new SymExpr(formal), formal->defPoint, false);
        }
      }
    }
    /* Now scan the Function body */
    for_alist(expr, fn->body->body) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
          addToRootSet(fn, new SymExpr(def->sym), nullify);
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def, nullify);
          }
        }
      } else if (BlockStmt* blk = dynamic_cast<BlockStmt*>(expr)) {
        buildRootSetForFunction(fn, blk, NULL, nullify);
      }
    }
  } else if (!def) {
    /* Looking at a BlockStmt */
    BlockStmt* blk = dynamic_cast<BlockStmt*>(base);
    INT_ASSERT(blk);
    for_alist(ast, blk->body) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            addToRootSet(fn, new SymExpr(def->sym), nullify);
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def, nullify);
          }
        }
      }
    }
  } else {
    /* Looking at a record. */
    ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (ClassType* classfield = dynamic_cast<ClassType*>(field->type)) {
        if (classfield->classTag == CLASS_CLASS) {
          addToRootSet(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                           base->copy(),
                                           new SymExpr(field)), nullify);
        } else if (classfield->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                                   base->copy(),
                                                   new SymExpr(field)),
                                  field->defPoint, nullify);
        }
      }
    }
  }
}


void buildRootSetForModule(ModuleSymbol* module) {
  for_alist(expr, module->block->body) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<VarSymbol*>(def->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            addToRootSet(chpl_main, new SymExpr(def->sym), true);
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
    buildRootSetForFunction(fn, NULL, NULL, true);
  }

  forv_Vec(ModuleSymbol, mod, allModules) {
    buildRootSetForModule(mod);
  }
}

