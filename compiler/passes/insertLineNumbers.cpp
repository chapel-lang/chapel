#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"

static void
insertLineNumberInCall(CallExpr* call) {
  FnSymbol * fn = call->getFunction();
  int length = fn->formals->length();
  if (!length || strcmp(fn->getFormal(length)->name, "_filename")) {
    if (fn->getModule()->modtype == MOD_STANDARD) {
      currentLineno = call->lineno;
      currentFilename = call->filename;
      call->insertAtTail(new CallExpr(PRIMITIVE_GET_LINENO));
      call->insertAtTail(new CallExpr(PRIMITIVE_GET_FILENAME));
    } else {
      call->insertAtTail(new SymExpr(new_IntSymbol(call->lineno)));
      call->insertAtTail(new SymExpr(new_StringSymbol(call->filename)));
    }
  } else {
    call->insertAtTail(fn->getFormal(length-1));
    call->insertAtTail(fn->getFormal(length));
  }
}

void insertLineNumbers() {
  compute_call_sites();

  Vec<FnSymbol *> fns;

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr * call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_GET_LINENO) ||
          call->isPrimitive(PRIMITIVE_GET_FILENAME)) {
        fns.add_exclusive(call->getFunction());
      } else if (call->primitive && call->primitive->passLineno) {
        currentLineno = call->lineno;
        currentFilename = call->filename;
        call->insertAtTail(new CallExpr(PRIMITIVE_GET_LINENO));
        call->insertAtTail(new CallExpr(PRIMITIVE_GET_FILENAME));
      }
    }
  }

  forv_Vec(FnSymbol, fn, fns) {

    if (fn->getModule()->modtype == MOD_USER && !fn->isCompilerTemp) {
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
    forv_Vec(CallExpr, call, *fn->calledBy) {
      insertLineNumberInCall(call);
      fns.add_exclusive(call->getFunction());
    }
  }

}

