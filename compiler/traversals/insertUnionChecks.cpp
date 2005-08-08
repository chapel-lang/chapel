#include <typeinfo>
#include "insertUnionChecks.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


static ExprStmt* buildCheckStmt(ClassType* classType, unionCall type, 
                                MemberAccess* accessExpr) {
  CallExpr* fnCall = classType->buildSafeUnionAccessCall(type,
                                                         accessExpr->base,
                                                         accessExpr->member);
  return new ExprStmt(fnCall);
}


void InsertUnionChecks::preProcessExpr(Expr* expr) {
  if (MemberAccess* unionExpr = dynamic_cast<MemberAccess*>(expr)) {
    if (ClassType* unionType = 
        dynamic_cast<ClassType*>(unionExpr->base->typeInfo())) {
      if (unionType->classTag == CLASS_UNION) {
        if (expr->isWritten()) {
          ExprStmt* testStmt = buildCheckStmt(unionType, UNION_SET, unionExpr);
          expr->parentStmt->insertAfter(testStmt);
        }
        if (expr->isRead()) {
          ExprStmt* testStmt = buildCheckStmt(unionType, UNION_CHECK, unionExpr);
          expr->parentStmt->insertBefore(testStmt);
        }
      }
    }
  }
}
