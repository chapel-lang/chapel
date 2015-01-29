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
  if (def->sym->hasFlag(FLAG_COMPILER_NESTED_FUNCTION)) {
    Expr* stmt = def->getStmtExpr();
    if (!stmt) {
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
        if (AggregateType* ct = toAggregateType(ts->type)) {
          def->replace(new UnresolvedSymExpr(def->sym->name));
          ct->addDeclarations(def, true);
          return;
        }
      }
    }
    def->replace(new UnresolvedSymExpr(def->sym->name));
    stmt->insertBefore(def);
  } else if (!strncmp("_iterator_for_loopexpr", def->sym->name, 22)) {
    FnSymbol* parent = toFnSymbol(def->parentSymbol);
    INT_ASSERT(!strncmp("_parloopexpr", parent->name, 12) ||
               !strncmp("_seqloopexpr", parent->name, 12));
    while (!strncmp("_parloopexpr", parent->defPoint->parentSymbol->name, 12) ||
           !strncmp("_seqloopexpr", parent->defPoint->parentSymbol->name, 12))
      parent = toFnSymbol(parent->defPoint->parentSymbol);
    if (TypeSymbol* ts = toTypeSymbol(parent->defPoint->parentSymbol)) {
      AggregateType* ct = toAggregateType(ts->type);
      INT_ASSERT(ct);
      ct->addDeclarations(def->remove(), true);
    } else {
      parent->defPoint->insertBefore(def->remove());
    }
  }
}


//
// recursive helper function for destructureTuple below
//
static void
insertDestructureStatements(Expr* S1, Expr* S2, CallExpr* lhs, Expr* rhs) {
  int i = 0;

  S1->getStmtExpr()->insertAfter(
    buildIfStmt(new CallExpr("!=",
                             new SymExpr(new_IntSymbol(lhs->numActuals())),
                             new CallExpr(".", rhs->copy(),
                                          new_StringSymbol("size"))),
                new CallExpr("compilerError", new_StringSymbol("tuple size must match the number of grouped variables"), new_IntSymbol(0))));

  for_actuals(expr, lhs) {
    i++;
    expr->remove();
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(expr)) {
      if (!strcmp(se->unresolved, "chpl__tuple_blank")) {
        continue;
      }
    }
    CallExpr* nextLHS = toCallExpr(expr);
    Expr* nextRHS = new CallExpr(rhs->copy(), new_IntSymbol(i));
    if (nextLHS && nextLHS->isNamed("_build_tuple")) {
      insertDestructureStatements(S1, S2, nextLHS, nextRHS);
    } else {
      VarSymbol* ltmp = newTemp();
      ltmp->addFlag(FLAG_MAYBE_PARAM);
      S1->insertBefore(new DefExpr(ltmp));
      S1->insertBefore(new CallExpr(PRIM_MOVE, ltmp,
                         new CallExpr(PRIM_ADDR_OF, expr)));
      S2->insertBefore(new CallExpr("=", ltmp, nextRHS));
    }
  }
}


//
// destructureTupleAssignment
//
//   (i,j) = expr;    ==>    i = expr(1);
//                           j = expr(2);
//
//   note: handles recursive tuple destructuring, (i,(j,k)) = ...
//
static void
destructureTupleAssignment(CallExpr* call) {
  CallExpr* parent = toCallExpr(call->parentExpr);
  if (parent && parent->isNamed("=") && parent->get(1) == call) {
    VarSymbol* rtmp = newTemp();
    rtmp->addFlag(FLAG_EXPR_TEMP);
    rtmp->addFlag(FLAG_MAYBE_TYPE);
    rtmp->addFlag(FLAG_MAYBE_PARAM);
    Expr* S1 = new CallExpr(PRIM_MOVE, rtmp, parent->get(2)->remove());
    Expr* S2 = new CallExpr(PRIM_NOOP);
    call->getStmtExpr()->replace(S1);
    S1->insertAfter(S2);
    S1->insertBefore(new DefExpr(rtmp));
    insertDestructureStatements(S1, S2, call, new SymExpr(rtmp));
    S2->remove();
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
    if (ts->hasFlag(FLAG_SYNC))
      fn->addFlag(FLAG_SYNC);
    if (ts->hasFlag(FLAG_SINGLE))
      fn->addFlag(FLAG_SINGLE);
    if (ts->hasFlag(FLAG_ATOMIC_TYPE))
      fn->addFlag(FLAG_ATOMIC_TYPE);
  }
}


static void change_cast_in_where(FnSymbol* fn) {
  if (fn->where) {
    Vec<BaseAST*> asts;
    collect_asts(fn->where, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isNamed("_cast")) {
          call->primitive = primitives[PRIM_IS_SUBTYPE];
          call->baseExpr->remove();
        }
      }
    }
  }
}


void cleanup(void) {
  Vec<BaseAST*> asts;
  collect_asts(rootModule, asts);

  forv_Vec(BaseAST, ast, asts) {
    SET_LINENO(ast);
    if (DefExpr* def = toDefExpr(ast)) {
      normalize_nested_function_expressions(def);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    SET_LINENO(ast);
    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_build_tuple"))
        destructureTupleAssignment(call);
    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        flatten_primary_methods(fn);
        change_cast_in_where(fn);
      }
    }
  }
}
