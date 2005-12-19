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


static AList<Expr>* copy_argument_list(CallExpr* expr) {
  AList<Expr>* args = new AList<Expr>();
  args->insertAtTail(expr->argList->copy());
  return args;
}


void ResolveSymbols::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isNamed("__primitive"))
      return;
    if (call->opTag == OP_INIT) {
      Type* type = call->get(1)->typeInfo();
      if (type->defaultValue) {
        call->replace(new SymExpr(type->defaultValue));
      } else if (type->defaultConstructor) {
        FnSymbol* fn = type->defaultConstructor;
        if (fn->formals->length() > 0) {
          Vec<Symbol*> defaults;
          for_alist(DefExpr, formalDef, fn->formals) {
            defaults.add(formalDef->sym);
          }
          fn = fn->default_wrapper(&defaults);
        }
        call->replace(new CallExpr(fn));
      } else {
        INT_FATAL("type has no default value or default constructor");
      }
      return;
    }
    if (call->opTag != OP_NONE)
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

      AList<Expr>* arguments = copy_argument_list(call);
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
          if (!fns.v[0]->hasPragma("builtin")) {
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
  } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    // Resolve MemberAccesses
    if (CallExpr* parent = dynamic_cast<CallExpr*>(expr->parentExpr))
      if (parent->isAssign() && parent->get(1) == expr) {
        resolve_member_access(expr->parentExpr, &member_access->member_offset,
                              &member_access->member_type);
        return;
      }
    resolve_member_access(member_access, &member_access->member_offset,
                          &member_access->member_type);
  } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    // Resolve default constructors
    Vec<FnSymbol*> fns;
    call_info(defExpr, defExpr->initAssign, CALL_INFO_FIND_ASSIGN);
    call_info(defExpr, fns, CALL_INFO_FIND_NON_ASSIGN);
    if (fns.n == 1) {
      defExpr->sym->type->defaultConstructor = fns.v[0];
    } if (fns.n > 1) {
      INT_FATAL(expr, "Unable to resolve default constructor");
    } else if (defExpr->exprType) {
      if (CallExpr *fc = dynamic_cast<CallExpr*>(defExpr->exprType))
        if (SymExpr* v = dynamic_cast<SymExpr*>(fc->baseExpr))
          if (FnSymbol *fn = dynamic_cast<FnSymbol*>(v->var))
            defExpr->sym->type->defaultConstructor = fn;
    }
  }
}


void resolveSymbols(void) {
  Pass* pass = new ResolveSymbols();
  pass->run(Symboltable::getModules(pass->whichModules));
}
