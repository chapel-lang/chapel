#include "preAnalysisHacks.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"

void PreAnalysisHacks::postProcessStmt(Stmt* stmt) {
  if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
    loop->iterators->only()->replace(
      new CallExpr(
        new MemberAccess(
          loop->iterators->only()->copy(),
          new UnresolvedSymbol("_forall"))));
  }
}

static int
check_type(Type *t) {
  return t && t != dtUnknown && t != dtNil;
}

void PreAnalysisHacks::postProcessExpr(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strcmp("typeof", base->var->name)) {
        Type* type = call->argList->only()->typeInfo();
        if (type != dtUnknown) {
          call->replace(new SymExpr(type->symbol));
        } else if (SymExpr* base = dynamic_cast<SymExpr*>(call->argList->only())) {
          if (base->var->defPoint->exprType) {
            call->replace(base->var->defPoint->exprType->copy());
          } else {
            // NOTE we remove the typeof function even if we can't
            // resolve the type before analysis
            Expr* arg = call->argList->only();
            arg->remove();
            call->replace(arg);
          }
        } else {
          // NOTE we remove the typeof function even if we can't
          // resolve the type before analysis
          Expr* arg = call->argList->only();
          arg->remove();
          call->replace(arg);
        }
      }
    }
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->retType == dtUnknown &&
          def_expr->exprType && check_type(def_expr->exprType->typeInfo())) {
        fn->retType = def_expr->exprType->typeInfo();
        def_expr->exprType = NULL;
      }
    } else if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def_expr->sym)) {
      if (arg->intent == INTENT_TYPE && 
          def_expr->exprType && check_type(def_expr->exprType->typeInfo())) {
        arg->type = getMetaType(def_expr->exprType->typeInfo());
      }
    } else if (def_expr->sym->type == dtUnknown && 
               def_expr->exprType && check_type(def_expr->exprType->typeInfo())) {
      def_expr->sym->type = def_expr->exprType->typeInfo();
      def_expr->exprType = NULL;
    }
  }


  if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    if (castExpr->type == dtUnknown &&
        castExpr->newType && check_type(castExpr->newType->typeInfo())) {
      castExpr->type = castExpr->newType->typeInfo();
    }
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (def_expr->sym->type == dtUnknown &&
        !def_expr->exprType &&
        def_expr->init && check_type(def_expr->init->typeInfo())) {
      def_expr->sym->type = def_expr->init->typeInfo();
    }
  }

  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strcmp(base->var->name, "domain")) {
        if (call->argList->length() != 1)
          USR_FATAL(call, "Domain type cannot yet be inferred");
        if (call->argList->only()->typeInfo() != dtInteger)
          USR_FATAL(call, "Non-arithmetic domains not yet supported");
        base->replace(new SymExpr("_construct__adomain"));
      }
    }
  }

  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strcmp(base->var->name, "_construct__aarray")) {
        if (DefExpr* def = dynamic_cast<DefExpr*>(call->parentExpr)) {
          call->parentStmt->insertAfter(
            new ExprStmt(new CallExpr(new MemberAccess(new SymExpr(def->sym), new UnresolvedSymbol("myinit")))));
          call->parentStmt->insertAfter(
            new ExprStmt(new CallExpr(OP_GETSNORM,
                                      new MemberAccess(new SymExpr(def->sym), new UnresolvedSymbol("dom")),
                                      call->argList->last()->copy())));
          call->argList->last()->replace(new_IntLiteral(2));
        }
      }
    }
  }

  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      if (!strcmp(base->var->name, "_construct__adomain_lit")) {
        Stmt* stmt = call->parentStmt;
        VarSymbol* _adomain_tmp = new VarSymbol("_adomain_tmp");
        stmt->insertBefore(
          new ExprStmt(
            new DefExpr(_adomain_tmp, NULL,
              new CallExpr(base->var,
                new_IntLiteral(call->argList->length())))));
        call->replace(new SymExpr(_adomain_tmp));
        int dim = 1;
        for_alist(Expr, arg, call->argList) {
          stmt->insertBefore(
            new ExprStmt(
              new CallExpr(
                new MemberAccess(
                  new SymExpr(_adomain_tmp),
                  new UnresolvedSymbol("_set")),
                new_IntLiteral(dim), arg->copy())));
          dim++;
        }
      }
    }
  }
}

void PreAnalysisHacks::postProcessType(Type* type) {
  if (UserType* userType = dynamic_cast<UserType*>(type)) {
    if (userType->underlyingType == dtUnknown && 
        userType->typeExpr && check_type(userType->typeExpr->typeInfo())) {
      userType->underlyingType = userType->typeExpr->typeInfo();
      userType->typeExpr = NULL;
      if (!userType->defaultValue) {
        if (userType->underlyingType->defaultValue) {
          userType->defaultValue = userType->underlyingType->defaultValue;
          ASTContext context;
          context.parentScope = userType->symbol->defPoint->parentScope;
          context.parentSymbol = userType->symbol;
          context.parentStmt = NULL;
          context.parentExpr = NULL;
          insertHelper(userType->defaultValue, context);
        } else {
          userType->defaultConstructor = userType->underlyingType->defaultConstructor;
        }
      }
    }
  }
}
