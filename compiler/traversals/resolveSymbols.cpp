#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "../passes/filesToAST.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"
#include "stringutil.h"


void ResolveSymbols::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->primitive) {
      call->makeOp();
    }
    if (call->opTag == OP_GET_MEMBER || call->opTag == OP_SET_MEMBER)
      resolve_member_access(call, &call->member_offset, &call->member_type);
    if (call->opTag == OP_INIT) {
      Type* type = call->get(1)->typeInfo();
      if (type->defaultValue) {
        call->replace(new SymExpr(type->defaultValue));
      } else if (type->defaultConstructor) {
        Vec<FnSymbol*> fns;
        call_info(call, fns);
        if (fns.n == 1)
          call->replace(new CallExpr(fns.v[0]));
        else
          INT_FATAL(call, "Cannot resolve default constructor");
      } else {
        INT_FATAL("Type has neither default value nor default constructor");
      }
      return;
    }
    if (call->opTag != OP_NONE || call->primitive)
      return;
    if (!call->isAssign()) {
      Vec<FnSymbol*> fns;
      call_info(call, fns);
      if (fns.n != 1) {
        // HACK: Special case where write(:nilType) requires dynamic
        // dispatch; Take the other one.
        if (fns.n == 2 && !strcmp(fns.v[1]->name, "fwrite") &&
            fns.v[1]->formals->first()->sym->type == dtFile && 
            fns.v[1]->formals->get(2)->sym->type == dtNil) {
        } else if (fns.n == 2 && !strcmp(fns.v[0]->name, "fwrite") &&
                   fns.v[0]->formals->first()->sym->type == dtFile && 
                   fns.v[0]->formals->get(2)->sym->type == dtNil) {
          fns.v[0] = fns.v[1];
        } else {
          if (call->partialTag != PARTIAL_NEVER)
            return;
          if (fns.n > 1)
            USR_WARNING(expr, "Dynamic dispatch not yet supported");
          INT_FATAL(expr, "Unable to resolve function");
          return;
        }
      }

      AList<Expr>* arguments = call->argList->copy();
      // HACK: to handle special case for a.x(1) translation
      Expr *baseExpr = call->baseExpr;
      if (CallExpr* basecall = dynamic_cast<CallExpr*>(call->baseExpr)) {
        if (basecall->partialTag != PARTIAL_NEVER) {
          Vec<FnSymbol*> fns;
          call_info(basecall, fns);
          if (fns.n == 0) {
            baseExpr = basecall->baseExpr;
            arguments->insertAtHead(basecall->argList->copy());
          }
        }
      }
      if (!strcmp("this", fns.v[0]->name)) {
        arguments->insertAtHead(baseExpr->copy());
      }
      CallExpr *new_expr = new CallExpr(fns.v[0], arguments);
      expr->replace(new_expr);
      if (fns.v[0]->hasPragma("builtin"))
        new_expr->makeOp();
    } else {
      // Resolve AssignOp to members or setter functions
      Vec<FnSymbol*> fns;
      call_info(call, fns);
      if (fns.n == 1 && !fns.v[0]->hasPragma("builtin")) {
        call->baseExpr->replace(new SymExpr(fns.v[0]));
      } else if (fns.n > 1) {
        int notbuiltin = 0;
        forv_Vec(FnSymbol, f, fns) {
          if (!f->hasPragma("builtin")) {
            notbuiltin = 1;
          }
        }
        if (!notbuiltin) {
          call->makeOp();
        } else {
          INT_FATAL(expr, "Unable to resolve setter function");
        }
      } else {
        call->makeOp(); // assume op if can't resolve
      }
    }
  }
}


void resolveSymbols(void) {
  if (no_infer)
    return;
  Pass* pass = new ResolveSymbols();
  pass->run(Symboltable::getModules(pass->whichModules));
}
