#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "optimizations.h"


static void mapFormalsToActuals(CallExpr* call, ASTMap* map) {
  currentFilename = call->filename;
  currentLineno = call->lineno;
  for_formals_actuals(formal, actual, call) {
    if (formal->requiresCPtr() || formal->isTypeVariable) {
      if (SymExpr* se = toSymExpr(actual))
        map->put(formal, se->var);
      else
        INT_FATAL(actual, "illegal reference actual encountered in inlining");
    } else {
      char* temp_name =  astr("_inline_", formal->cname);
      VarSymbol* temp = new VarSymbol(temp_name, actual->typeInfo());
      temp->isCompilerTemp = true;
      call->getStmtExpr()->insertBefore(new DefExpr(temp));
      call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, temp, actual->copy()));
      map->put(formal, temp);
    }
  }
}


static void inline_call(CallExpr* call, Vec<BaseAST*>& new_asts) {
  FnSymbol* fn = call->isResolved();
  ASTMap map;
  mapFormalsToActuals(call, &map);
  BlockStmt* block = fn->body->copy(&map);
  reset_file_info(block, call->lineno, call->filename);
  CallExpr* return_stmt = toCallExpr(block->body.last());
  if (!return_stmt || !return_stmt->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(call, "Cannot inline function, function is not normalized");
  Expr* return_value = return_stmt->get(1);
  SymExpr* se = toSymExpr(return_value);
  if (!se || se->var->astType == SYMBOL_ARG)
    INT_FATAL(fn, "inlined function cannot return an argument symbol");
  return_stmt->remove();
  return_value->remove();
  call->getStmtExpr()->insertBefore(block);
  //  map.get_values(new_asts);
  collect_asts(&new_asts, block);
  if (fn->retType == dtVoid)
    call->getStmtExpr()->remove();
  else
    call->replace(return_value);
}


static void
inline_calls(Vec<BaseAST*>& asts, Vec<FnSymbol*>* inline_stack = NULL) {
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->hasPragma("inline")) {
          Vec<FnSymbol*> stack;
          if (!inline_stack)
            inline_stack = &stack;
          else if (inline_stack->in(fn))
            INT_FATAL(fn, "Recursive inlining detected");
          inline_stack->add(fn);
          Vec<BaseAST*> new_asts;
          inline_call(call, new_asts);
          inline_calls(new_asts, inline_stack);
          inline_stack->pop();
          if (report_inlining)
            printf("chapel compiler: reporting inlining"
                   ", %s function was inlined\n", fn->cname);
        }
      }
    }
  }
}


void inlineFunctions(void) {
  if (fNoInline || fBaseline)
    return;
  Vec<BaseAST*> asts;
  asts.copy(gAsts);
  inline_calls(asts);
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasPragma("inline")) {
      fn->defPoint->remove();
    } else {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }
}


void inlineCall(CallExpr* call) {
  Vec<BaseAST*> asts;
  inline_call(call, asts);
}
