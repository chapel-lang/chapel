#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

void insertLineNumbers() {
  compute_call_sites();
  Vec<FnSymbol *> fns;

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr * call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_GET_LINENO) ||
          call->isPrimitive(PRIMITIVE_GET_FILENAME)) {
        fns.add_exclusive(call->getFunction());
      }
    }
  }

  forv_Vec(FnSymbol, fn, fns) {

    if (fn->getModule()->modtype == MOD_USER) {
      Vec<BaseAST *> asts;
      collect_asts(&asts, fn->body);

      forv_Vec(BaseAST, ast, asts) {
        if (CallExpr * call = dynamic_cast<CallExpr*>(ast)) {
          if (call->isPrimitive(PRIMITIVE_GET_LINENO)) {
            call->replace(new SymExpr(new_IntSymbol(call->lineno)));
          }
          if (call->isPrimitive(PRIMITIVE_GET_FILENAME)) {
            call->replace(new SymExpr(new_StringSymbol(call->filename)));
          }
        }
      }

      continue;
    }



    ArgSymbol * line = 
      new ArgSymbol(INTENT_BLANK, "_line_number", dtInt[INT_SIZE_32]);
    ArgSymbol * filename = 
      new ArgSymbol(INTENT_BLANK, "_filename", dtString);
    fn->insertFormalAtTail(line);
    fn->insertFormalAtTail(filename);
    Vec<BaseAST *> asts;
    collect_asts(&asts, fn->body);

    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr * call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isPrimitive(PRIMITIVE_GET_LINENO)) {
          call->replace(new SymExpr(line));
        }
        if (call->isPrimitive(PRIMITIVE_GET_FILENAME)) {
          call->replace(new SymExpr(filename));
        }
      }
    }
    forv_Vec(CallExpr, caller, *fn->calledBy) {
      FnSymbol * fn = caller->getFunction();
      int length = fn->formals->length();
      if ((length == 0) || 
          (strcmp(fn->getFormal(length)->name, "_filename"))) {
        if (fn->getModule()->modtype == MOD_STANDARD) {
          caller->insertAtTail(new CallExpr(PRIMITIVE_GET_LINENO));
          caller->insertAtTail(new CallExpr(PRIMITIVE_GET_FILENAME));
        } else {
          caller->insertAtTail(new SymExpr(new_IntSymbol(caller->lineno)));
          caller->insertAtTail(new SymExpr(new_StringSymbol(caller->filename)));
        }
      } else {
        caller->insertAtTail(fn->getFormal(length-1));
        caller->insertAtTail(fn->getFormal(length));
      }
        
      fns.add_exclusive(caller->getFunction());
    }
  }

}

