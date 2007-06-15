#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"


static void mapFormalsToActuals(CallExpr* call, ASTMap* map) {
  currentFilename = call->filename;
  currentLineno = call->lineno;
  for_formals_actuals(formal, actual, call) {
    if (formal->requiresCPtr() || formal->isTypeVariable) {
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(actual)) {
        map->put(formal, symExpr);
      } else if (CallExpr* call = dynamic_cast<CallExpr*>(actual)) {
        if (call->isPrimitive(PRIMITIVE_CAST))
          map->put(formal, call);
        else
          INT_FATAL("Illegal reference actual encountered in inlining");
      } else {
        INT_FATAL("Illegal reference actual encountered in inlining");
      }
    } else {
      char* temp_name =  stringcat("_inline_temp_", formal->cname);
      VarSymbol* temp = new VarSymbol(temp_name, actual->typeInfo());
      temp->isCompilerTemp = true;
      call->getStmtExpr()->insertBefore(new DefExpr(temp));
      call->getStmtExpr()->insertBefore
        (new CallExpr(PRIMITIVE_MOVE, temp, actual->copy()));
      map->put(formal, new SymExpr(temp));
    }
  }
}


static void inline_call(CallExpr* call, Vec<Expr*>* stmts) {
  FnSymbol* fn = call->findFnSymbol();
  ASTMap map;
  mapFormalsToActuals(call, &map);
  AList* fn_body = fn->body->body->copy();
  reset_file_info(fn_body, call->lineno, call->filename);
  CallExpr* return_stmt = dynamic_cast<CallExpr*>(fn_body->last());
  if (!return_stmt || !return_stmt->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(call, "Cannot inline function, function is not normalized");
  Expr* return_value = return_stmt->get(1);
  SymExpr* se = dynamic_cast<SymExpr*>(return_value);
  if (!se || se->var->astType == SYMBOL_ARG)
    INT_FATAL(fn, "inlined function cannot return an argument symbol");
  return_stmt->remove();
  return_value->remove();
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn_body);
  for_alist(Expr, stmt, fn_body)
    stmts->add(stmt);
  for_alist(Expr, stmt, fn_body) {
    stmt->remove();
    call->getStmtExpr()->insertBefore(stmt);
  }
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
      if (Expr* expr = dynamic_cast<Expr*>(map.get(sym->var)))
        sym->replace(expr->copy());
    }
  }
  if (fn->retType == dtVoid)
    call->getStmtExpr()->remove();
  else
    call->replace(return_value);
}


static void inline_calls(BaseAST* base, Vec<FnSymbol*>* inline_stack = NULL) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->primitive || !call->getStmtExpr())
        continue;
      FnSymbol* fn = call->findFnSymbol();
      if (!fn || !fn->hasPragma("inline"))
        continue;
      Vec<FnSymbol*> stack;
      if (!inline_stack)
        inline_stack = &stack;
      else if (inline_stack->in(fn))
        INT_FATAL(fn, "Recursive inlining detected");
      inline_stack->add(fn);
      Vec<Expr*> stmts;
      inline_call(call, &stmts);
      forv_Vec(Expr, stmt, stmts)
        inline_calls(stmt, inline_stack);
      inline_stack->pop();
      if (report_inlining)
        printf("chapel compiler: reporting inlining"
               ", %s function was inlined\n", fn->cname);
    }
  }
}


void inlineFunctions(void) {
  if (no_inline || fBaseline)
    return;
  forv_Vec(ModuleSymbol, mod, allModules)
    inline_calls(mod);
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
        if (fn->hasPragma("inline")) {
          fn->defPoint->remove();
        }
      }
    }
  }
}


void inlineCall(CallExpr* call) {
  Vec<Expr*> stmts;
  inline_call(call, &stmts);
}
