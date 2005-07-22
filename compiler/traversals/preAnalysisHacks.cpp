#include "preAnalysisHacks.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"

void PreAnalysisHacks::postProcessStmt(Stmt* stmt) {
  if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (!dynamic_cast<Tuple*>(loop->iterators->only())) {
      loop->iterators->only()->replace(
        new ParenOpExpr(
          new MemberAccess(
            loop->iterators->only()->copy(),
            new UnresolvedSymbol("_forall")),
          new AList<Expr>()));
    }
  }
}

void PreAnalysisHacks::postProcessExpr(Expr* expr) {
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->retType == dtUnknown &&
          def_expr->exprType &&
          def_expr->exprType->typeInfo() != dtUnknown) {
        fn->retType = def_expr->exprType->typeInfo();
        def_expr->exprType = NULL;
      }
    } else if (ParamSymbol* param = dynamic_cast<ParamSymbol*>(def_expr->sym)) {
      if (param->intent == PARAM_TYPE &&
          def_expr->exprType &&
          def_expr->exprType->typeInfo() != dtUnknown) {
        param->type = getMetaType(def_expr->exprType->typeInfo());
      }
    } else if (def_expr->sym->type == dtUnknown &&
               def_expr->exprType &&
               def_expr->exprType->typeInfo() != dtUnknown) {
      def_expr->sym->type = def_expr->exprType->typeInfo();
      def_expr->exprType = NULL;
    }
  }

  if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    if (castExpr->type == dtUnknown &&
        castExpr->newType &&
        castExpr->newType->typeInfo() != dtUnknown) {
      castExpr->type = castExpr->newType->typeInfo();
      castExpr->newType = NULL;
    }
  }

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (def_expr->sym->type == dtUnknown &&
        !def_expr->exprType &&
        def_expr->init &&
        def_expr->init->typeInfo() != dtUnknown) {
      def_expr->sym->type = def_expr->init->typeInfo();
    }
  }
}

void PreAnalysisHacks::postProcessType(Type* type) {
  if (UserType* userType = dynamic_cast<UserType*>(type)) {
    if (userType->defType == dtUnknown &&
        userType->defExpr &&
        userType->defExpr->typeInfo() != dtUnknown) {
      userType->defType = userType->defExpr->typeInfo();
      userType->defExpr = NULL;
      if (!userType->defaultVal) {
        if (userType->defType->defaultVal) {
          userType->defaultVal = userType->defType->defaultVal->copy();
          fixup(userType->symbol->defPoint);
        } else {
          userType->defaultConstructor = userType->defType->defaultConstructor;
        }
      }
    }
  }
}
