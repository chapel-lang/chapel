#include "expr.h"
#include "link.h"
#include "nils.h"
#include "stmt.h"
#include "symbol.h"

ILink* nilILink = NULL;

Stmt* nilStmt = NULL;
Expr* nilExpr = NULL;
Symbol* nilSymbol = NULL;
Type* nilType = NULL;

VarSymbol* nilVarSymbol = NULL;
FnSymbol* nilFnSymbol = NULL;
FnDefStmt* nilFnDefStmt = NULL;
ClassType* nilClassType = NULL;


void initNils(void) {
  nilILink = new ILink();

  nilStmt = new Stmt(STMT);
  nilExpr = new Expr(EXPR);
  nilSymbol = new Symbol(SYMBOL, "nilSymbol");
  nilType = new Type(TYPE, nilExpr);

  nilVarSymbol = new VarSymbol("nilVarSymbol", nilType);
  nilFnSymbol = new FnSymbol("nilFnSymbol");
  nilFnDefStmt = new FnDefStmt(nilFnSymbol);
  nilClassType = new ClassType(nilClassType);
  nilClassType->addDefinition(nilStmt);

  verifyNilsUncorrupted("initialized incorrectly");
}


void verifyNilsUncorrupted(char* message) {
  if (nilILink->next != NULL || nilILink->prev != NULL) {
    INT_FATAL("nilILink has been %s", message);
  }

  if (nilStmt->next != nilILink || nilStmt->prev != nilILink) {
    INT_FATAL("nilStmt has been %s", message);
  }
  if (nilExpr->next != nilILink || nilExpr->prev != nilILink ||
      nilExpr->stmt != nilStmt || nilExpr->parent != NULL) {
    INT_FATAL("nilExpr has been %s", message);
  }
  if (nilSymbol->next != nilILink || nilSymbol->prev != nilILink ||
      nilSymbol->type != NULL) {
    INT_FATAL("nilSymbol has been %s", message);
  }
  if (nilType->next != nilILink || nilType->prev != nilILink ||
      nilType->name != nilSymbol || nilType->defaultVal != nilExpr) {
    INT_FATAL("nilType has been %s", message);
  }

  if (nilVarSymbol->next != nilILink || nilVarSymbol->prev != nilILink ||
      nilVarSymbol->type != nilType) {
    INT_FATAL("nilVarSymbol has been %s", message);
  }
  if (nilFnSymbol->next != nilILink || nilFnSymbol->prev != nilILink ||
      nilFnSymbol->formals != nilSymbol || nilFnSymbol->body != nilStmt ||
      nilFnSymbol->parentFn != NULL) {
    INT_FATAL("nilFnSymbol has been %s", message);
  }
  if (nilFnDefStmt->next != nilILink || nilFnDefStmt->prev != nilILink ||
      nilFnDefStmt->fn != nilFnSymbol) {
    INT_FATAL("nilFnDefStmt has been %s", message);
  }
  if (nilClassType->next != nilILink || nilClassType->prev != nilILink ||
      nilClassType->name != nilSymbol || nilClassType->parentClass != NULL ||
      nilClassType->definition != nilStmt) {
    INT_FATAL("nilClassType has been %s", message);
  }
}
