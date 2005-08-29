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
  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(stmt)) {
    if (dynamic_cast<CondStmt*>(blockStmt->parentStmt)) {
      if (blockStmt->body->length() == 1) {
        if (BlockStmt* inner = dynamic_cast<BlockStmt*>(blockStmt->body->only())) {
          inner->remove();
          blockStmt->replace(inner);
        }
      }
    }
  }
}

static int
check_type(Type *t) {
  return t && t != dtUnknown && t != dtNil;
}

void PreAnalysisHacks::postProcessExpr(Expr* expr) {
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
}

void PreAnalysisHacks::postProcessType(Type* type) {
  if (UserType* userType = dynamic_cast<UserType*>(type)) {
    if (userType->defType == dtUnknown && 
        userType->defExpr && check_type(userType->defExpr->typeInfo())) {
      userType->defType = userType->defExpr->typeInfo();
      userType->defExpr = NULL;
      if (!userType->defaultValue) {
        if (userType->defType->defaultValue) {
          userType->defaultValue = userType->defType->defaultValue->copy();
          ASTContext context;
          context.parentScope = userType->symbol->defPoint->parentScope;
          context.parentSymbol = userType->symbol;
          context.parentStmt = NULL;
          context.parentExpr = NULL;
          insertHelper(userType->defaultValue, context);
        } else {
          userType->defaultConstructor = userType->defType->defaultConstructor;
        }
      }
    }
  }
}
