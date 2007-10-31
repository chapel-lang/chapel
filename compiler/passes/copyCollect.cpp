#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"



static void addToRootSet(FnSymbol* fn, Expr* expr, bool nullify = false) {
  if (nullify)
    fn->insertAtHead(new CallExpr(PRIMITIVE_GC_ADD_NULL_ROOT, expr));
  else
    fn->insertAtHead(new CallExpr(PRIMITIVE_GC_ADD_ROOT, expr));
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
static void buildRootSetForFunction(FnSymbol* fn, Expr* base, DefExpr* def) {
  static int totalRoots = 0;
  if (!base) {
    totalRoots = 0;
    /* This is the first level of recursion.  Scan the formals. */
    for_formals(formal, fn) {
      if (ClassType* ct = toClassType(formal->type)) {
        if (ct->classTag == CLASS_CLASS) {
          if (!ct->symbol->hasPragma("ref")) {
            addToRootSet(fn, new SymExpr(formal));
            totalRoots++;
          }
        } else if (ct->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new SymExpr(formal), formal->defPoint);
        }
      }
    }
    /* Now scan the Function body */
    for_alist(expr, fn->body->body) {
      if (DefExpr* def = toDefExpr(expr)) {
        if (ClassType* ct = toClassType(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            if (!ct->symbol->hasPragma("ref")) {
              addToRootSet(fn, new SymExpr(def->sym));
              totalRoots++;
            }
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def);
          }
        }
      } else if (BlockStmt* blk = toBlockStmt(expr)) {
        buildRootSetForFunction(fn, blk, NULL);
      }
    }
  } else if (!def) {
    /* Looking at a BlockStmt */
    BlockStmt* blk = toBlockStmt(base);
    INT_ASSERT(blk);
    for_alist(ast, blk->body) {
      if (DefExpr* def = toDefExpr(ast)) {
        if (ClassType* ct = toClassType(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            if (!ct->symbol->hasPragma("ref")) {
              addToRootSet(fn, new SymExpr(def->sym));
              totalRoots++;
            }
          } else if (ct->classTag == CLASS_RECORD) {
            buildRootSetForFunction(fn, new SymExpr(def->sym), def);
          }
        }
      }
    }
  } else {
    /* Looking at a record. */
    ClassType* ct = toClassType(def->sym->type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (ClassType* classfield = toClassType(field->type)) {
        if (classfield->classTag == CLASS_CLASS) {
          if (!classfield->symbol->hasPragma("ref")) {
            addToRootSet(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                             base->copy(),
                                             new SymExpr(field)));
            totalRoots++;
          }
        } else if (classfield->classTag == CLASS_RECORD) {
          buildRootSetForFunction(fn, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                                   base->copy(),
                                                   new SymExpr(field)),
                                  field->defPoint);
        }
      }
    }
  }
  if (!base) {
    if (totalRoots != 0)
      fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_GC_DELETE_ROOT,
                                                    new_IntSymbol(totalRoots)));
  }
}


static void buildRootSetForModule(ModuleSymbol* module) {
  int totalRoots = 0;
  for_alist(expr, module->block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (toVarSymbol(def->sym)) {
        if (ClassType* ct = toClassType(def->sym->type)) {
          if (ct->classTag == CLASS_CLASS) {
            if (!ct->symbol->hasPragma("ref")) {
              addToRootSet(chpl_main, new SymExpr(def->sym), true);
              totalRoots++;
            }
          }
        }
      }
    }
  }
  if (totalRoots != 0)
    chpl_main->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_GC_DELETE_ROOT,
                                                         new_IntSymbol(totalRoots)));

}


void copyCollection(void) {
  if (!fCopyCollect)
    return;

  chpl_main->insertAtHead(new CallExpr(PRIMITIVE_GC_CC_INIT,
                                       new_IntSymbol(20971520))); //20 MB

  forv_Vec(FnSymbol, fn, gFns) {
    buildRootSetForFunction(fn, NULL, NULL);
  }

  forv_Vec(ModuleSymbol, mod, allModules) {
    buildRootSetForModule(mod);
  }
}

