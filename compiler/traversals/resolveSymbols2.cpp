#include "resolveSymbols2.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void ResolveSymbols2::preProcessStmt(Stmt* stmt) {

}


void ResolveSymbols2::preProcessExpr(Expr* expr) {
  MemberAccess* dot;

  if (dot = dynamic_cast<MemberAccess*>(expr)) {
    UnresolvedSymbol* UnresolvedMember;
    Symbol* ResolvedMember;
    if (UnresolvedMember = dynamic_cast<UnresolvedSymbol*>(dot->member)) {
      if (resolve_symbol(UnresolvedMember, dot, ResolvedMember)) {
	INT_FATAL(dot, "Major error resolving MemberAccess in ResolveSymbols2");
      }
      dot->member = ResolvedMember;
    }
  }
}


void ResolveSymbols2::preProcessSymbol(Symbol* symbol) {

}


void ResolveSymbols2::preProcessType(Type* type) {

}


void ResolveSymbols2::run(ModuleSymbol* moduleList) {
  Traversal::run(moduleList);
}
