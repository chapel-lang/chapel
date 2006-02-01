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


static Expr *
build_dispatch_tree(CallExpr *call, Vec<FnSymbol*> &fns, Vec<Vec<Vec<Type *> *> *> &dispatch, int a) {
  if (a >= dispatch.v[0]->n) {
    if (fns.set_count() != 1) {
      USR_FATAL(call, "Unable to build table dispatch for ambiguous call");
    }
    return new CallExpr(fns.first(), call->argList->copy());
  }
  Vec<Type *> *t = 0;
  for (int i = 0; i < fns.n; i++) {
    if (fns.v[i]) {
      if (!t) {
        t = dispatch.v[i]->v[a];
        continue;
      }
      Vec<Type *> diff;
      t->set_disjunction(*dispatch.v[i]->v[a], diff);
      if (diff.n) {
        Type *type = diff.first();
        Vec<FnSymbol *> true_fns, false_fns;
        true_fns.fill(fns.n);
        false_fns.fill(fns.n);
        for (int i = 0; i < fns.n; i++) {
          if (fns.v[i]) {
            if (dispatch.v[i]->v[a]->set_in(type))
              true_fns.v[i] = fns.v[i];
            else
              false_fns.v[i] = fns.v[i];
          }
        }
        Expr *true_expr = 0, *false_expr = 0;
        if (true_fns.set_count() == 1)
          true_expr = new CallExpr(true_fns.first(), call->argList->copy());
        else
          true_expr = build_dispatch_tree(call, true_fns, dispatch, a + 1);
        if (false_fns.set_count() == 1)
          false_expr = new CallExpr(false_fns.first(), call->argList->copy());
        else
          false_expr = build_dispatch_tree(call, false_fns, dispatch, a);
        return new CondExpr(new CallExpr(PRIMITIVE_TYPE_EQUAL, 
                                         new SymExpr(type->symbol), 
                                         call->argList->get(a)->copy()), 
                            true_expr, false_expr);
      }
    }
  }
  return build_dispatch_tree(call, fns, dispatch, a+1);
}

static void
dynamic_dispatch(CallExpr *call) {
  Vec<Vec<Vec<Type *> *> *> dispatch;
  Vec<FnSymbol*> fns;
  call_info(call, fns, &dispatch);
  Expr *new_expr = build_dispatch_tree(call, fns, dispatch, 1);  // FORTRAN NUMBERING
  call->replace(new_expr);
}

void ResolveSymbols::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
        call->isPrimitive(PRIMITIVE_SET_MEMBER))
      resolve_member_access(call, &call->member_offset, &call->member_type);
    if (call->isPrimitive(PRIMITIVE_INIT)) {
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
    if (call->primitive)
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
          if (fns.n > 1) {
            dynamic_dispatch(call);
            return;
          }
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

