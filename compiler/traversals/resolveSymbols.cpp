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
  MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr);
  if (member_access) {
    args->insertAtTail(member_access->base->copy());
  }
  args->insertAtTail(expr->argList->copy());
  return args;
}


static int
is_builtin(FnSymbol *fn) {
  if (fn->hasPragma("builtin")) {
    return 1;
  }
  return 0;
}


void ResolveSymbols::postProcessExpr(Expr* expr) {
  CallExpr* call = dynamic_cast<CallExpr*>(expr);

  if (call) {
    SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
    if (base && !strcmp(base->var->name, "__primitive"))
      return;
  }

  // Resolve CallExprs
  if (call) {
    if (!call->isAssign()) {
      CallExpr *assign = dynamic_cast<CallExpr*>(call->parentExpr);
      if (!assign || !assign->isAssign() ||  assign->get(1) != expr) {
        Vec<FnSymbol*> fns;
        call_info(call, fns);
        if (fns.n == 0) { // for 0-ary (CallExpr(MemberAccess))
          call_info(call->baseExpr, fns);
        }
        if (fns.n != 1) {
          // HACK: Special case where write(:nilType) requires dynamic
          // dispatch; Take the other one.
          TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
          if (fns.n == 2 && !strcmp(fns.v[1]->name, "fwrite") &&
              fns.v[1]->formals->first()->sym->type == fileType->definition && 
              fns.v[1]->formals->get(2)->sym->type == dtNil) {

          } else if (fns.n == 2 && !strcmp(fns.v[0]->name, "fwrite") &&
                     fns.v[0]->formals->first()->sym->type == fileType->definition && 
                     fns.v[0]->formals->get(2)->sym->type == dtNil) {
            fns.v[0] = fns.v[1];
          } else {
            if (call->partialTag != PARTIAL_NEVER)
              return;
            if (fns.n > 1)
              USR_WARNING(expr, "Dynamic dispatch not yet supported");
            if (call->opTag == OP_NONE)
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
            if (fns.n == 0) { // for 0-ary (CallExpr(MemberAccess))
              call_info(call->baseExpr, fns);
            }
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
        if (fns.v[0]->hasPragma("builtin"))
          new_expr->makeOp();
        expr->replace(new_expr);
        expr = new_expr;
      }
    }
  }

  // Resolve AssignOp to members or setter functions
  if (CallExpr* aop = dynamic_cast<CallExpr*>(expr)) {
    if (aop->isAssign()) {
      if (SymExpr* var = dynamic_cast<SymExpr*>(aop->get(1))) {
        Vec<FnSymbol*> fns;
        call_info(aop, fns);
        int notbuiltin = 0;
        forv_Vec(FnSymbol, f, fns) {
          if (!is_builtin(fns.v[0])) {
            notbuiltin = 1;
          }
        }
        if (!notbuiltin) {
          aop->makeOp();
          return;
        }
        if (fns.n == 1) {
          CallExpr *new_expr = new CallExpr(fns.v[0], var->copy(), aop->get(2)->copy());
          aop->replace(new_expr);
          expr = new_expr;
        } else {
          if (!fns.n) {
            return;
          }
          INT_FATAL(expr, "Unable to resolve setter function");
        }
      } else if (CallExpr* paren = dynamic_cast<CallExpr*>(aop->get(1))) {
        Vec<FnSymbol*> fns;
        call_info(aop, fns);
        if (fns.n == 1) {
          AList<Expr>* arguments = new AList<Expr>();
          if (!strcmp("=this", fns.v[0]->name))
            arguments->insertAtTail(paren->baseExpr->copy());
          arguments->insertAtTail(copy_argument_list(paren));
          arguments->insertAtTail(aop->get(2)->copy());
          CallExpr *new_expr = new CallExpr(fns.v[0], arguments);
          new_expr->opTag = paren->opTag;
          aop->replace(new_expr);
          expr = new_expr;
        } else {
          INT_FATAL(expr, "Unable to resolve setter function");
        }
      } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(aop->get(1))) {
        resolve_member_access(aop, &member_access->member_offset, 
                              &member_access->member_type);
        if (member_access->member_offset < 0) {
          Vec<FnSymbol *> assign_fns;
          call_info(aop, assign_fns, CALL_INFO_FIND_FUNCTION);
          if (assign_fns.n != 1) 
            INT_FATAL(expr, "Unable to resolve member access");
          FnSymbol *f_assign = assign_fns.v[0];
          Expr *rhs = aop->get(2)->copy();
          AList<Expr>* assign_arguments = new AList<Expr>(member_access->base->copy());
          assign_arguments->insertAtTail(rhs);
          Expr* assign_function = new SymExpr(f_assign);
          CallExpr *new_expr = new CallExpr(assign_function, assign_arguments);
          expr->replace(new_expr);
          if (aop->get(1)->astType == EXPR_MEMBERACCESS) {
            expr = aop->get(1);
          }
        } else {
          if (ClassType* struct_scope =
              dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
            member_access->member = 
              Symboltable::lookupInScope(member_access->member->name,
                                         struct_scope->structScope);
          }
        }
      }
    }
  }

  // Resolve MemberAccesses
  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    /***
     *** Resolve methods with arguments at CallExpr
     ***/
    if (CallExpr* paren_op = dynamic_cast<CallExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }
    if (CallExpr* aop = dynamic_cast<CallExpr*>(expr->parentExpr))
      if (aop->isAssign() && aop->get(1) == expr)
        return;

    resolve_member_access(member_access, &member_access->member_offset,
                          &member_access->member_type);
    if (member_access->member_offset < 0) {
      Vec<FnSymbol *> fns;
      call_info(member_access, fns);
      if (fns.n == 1) {
        Expr *new_expr = new CallExpr(fns.v[0], member_access->base->copy());
        expr->replace(new CallExpr(fns.v[0], member_access->base->copy()));
        expr = new_expr;
      } else
        INT_FATAL(expr, "Unable to resolve member access");
    } else {
      if (ClassType* struct_scope =
          dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);
      }
    }
  }

  // Resolve default constructors
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
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
