//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


//
// Move the statements in a block out of the block
//
static void
flatten_scopeless_block(BlockStmt* block) {
  for_alist(stmt, block->body) {
    stmt->remove();
    block->insertBefore(stmt);
  }
  block->remove();
}


//
// Moves expressions that are parsed as nested function definitions
// into their own statement; during parsing, these are embedded in
// call expressions
//
static void normalize_nested_function_expressions(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11)) ||
      (!strncmp("_let_fn", def->sym->name, 7)) ||
      (!strncmp("_if_fn", def->sym->name, 6)) ||
      (!strncmp("_reduce_scan", def->sym->name, 12)) ||
      (!strncmp("_forif_fn", def->sym->name, 9))) {
    Expr* stmt = def->getStmtExpr();
    if (!stmt) {
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
        if (ClassType* ct = toClassType(ts->type)) {
          def->replace(new SymExpr(def->sym->name));
          ct->addDeclarations(def, true);
          return;
        }
      }
    }
    def->replace(new SymExpr(def->sym->name));
    stmt->insertBefore(def);
  }
}


/*** destructure_tuple
 ***
 ***   (i,j) = expr;    ==>    i = expr(1);
 ***                           j = expr(2);
 ***
 ***   NOTE: handles recursive tuple destructuring, (i,(j,k)) = ...
 ***/
static void destructure_tuple(CallExpr* call) {
  CallExpr* parent = toCallExpr(call->parentExpr);
  if (!parent || !parent->isNamed("=") || parent->get(1) != call)
    return;
  Expr* stmt = parent->getStmtExpr();
  VarSymbol* temp = new VarSymbol("_tuple_destruct");
  temp->isCompilerTemp = true;
  stmt->insertBefore(new DefExpr(temp));
  stmt = new CallExpr(PRIMITIVE_MOVE, temp, parent->get(2)->remove());
  parent->replace(stmt);
  int i = 1;
  for_actuals(expr, call) {
    Expr *removed_expr = expr->remove();
    if (SymExpr *sym_expr = toSymExpr(removed_expr)) {
      if (sym_expr->isNamed("_")) {
        i++;
        continue;
      }
    }
    stmt->insertAfter(
      new CallExpr("=", removed_expr,
        new CallExpr(temp, new_IntSymbol(i))));
    i++;
  }
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
    Expr* insertPoint = ts->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    DefExpr* def = fn->defPoint;
    def->remove();
    insertPoint->insertBefore(def);
    if (fn->userString && fn->name != ts->name)
      fn->userString = astr(ts->name, ".", fn->userString);
    if (ts->hasPragma("sync"))
      fn->addPragma("sync");
  }
}


static void change_cast_in_where(FnSymbol* fn) {
  if (fn->where) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->where);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isNamed("_cast")) {
          call->primitive = primitives[PRIMITIVE_ISSUBTYPE];
          call->baseExpr->remove();
        }
      }
    }
  }
}


//
// Make all modules call their parent module's init function to make sure
// it has been run and the module's symbols are initialized.  This is NOT
// the same as using the outer module.
//
static void initializeOuterModules(ModuleSymbol* mod) {
  for_alist(stmt, mod->block->body) {
    if (BlockStmt* b = toBlockStmt(stmt))
      stmt = b->body.first();
    if (DefExpr* def = toDefExpr(stmt)) {
      if (ModuleSymbol* m = toModuleSymbol(def->sym)) {
        if (mod != theProgram) {
          if (!m->initFn || !mod->initFn)
            INT_FATAL("Module with no initialization function");
          m->initFn->insertAtHead(new CallExpr(mod->initFn));
        }
        initializeOuterModules(m);
      }
    }
  }
}


void cleanup(void) {
  initializeOuterModules(theProgram);

  Vec<BaseAST*> asts;
  collect_asts(&asts);

  // handle forall's in array type declaration with initialization
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr *call = toCallExpr(ast)) {
      if (call->isNamed("_build_array_type") && call->numActuals() == 4) {
        if (DefExpr *def = toDefExpr(call->parentExpr)) {
          CallExpr *tinfo = toCallExpr(def->exprType);
          Expr *indices = tinfo->get(3);
          Expr *iter = tinfo->get(4);
          indices->remove();
          iter->remove();
          if (def->init) {
            BlockStmt *forblk = buildForLoopExpr(indices, iter, def->init->copy());
            
            FnSymbol *forall_init = new FnSymbol("_forallinit");
            forall_init->insertAtTail(forblk);
            def->insertBefore(new DefExpr(forall_init));
            def->init->replace(new CallExpr(forall_init));
          }
        } else {
          USR_FATAL(call, "unhandled case of array type loop expression");
        }
      }
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* def = toDefExpr(ast)) {
      normalize_nested_function_expressions(def);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_build_tuple"))
        destructure_tuple(call);
    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        flatten_primary_methods(fn);
        change_cast_in_where(fn);
      }
    }
  }
}
