#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "../passes/runAnalysis.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void ResolveSymbols::preProcessExpr(Expr* &expr) {
  if (MemberAccess* dot = dynamic_cast<MemberAccess*>(expr)) {
    if (RunAnalysis::runCount) {
      if (UnresolvedSymbol* UnresolvedMember = dynamic_cast<UnresolvedSymbol*>(dot->member)) {
	Symbol* ResolvedMember;
	if (resolve_symbol(UnresolvedMember, dot, ResolvedMember)) {
	  INT_FATAL(dot, "Major error resolving MemberAccess in ResolveSymbols");
	}
	dot->member = ResolvedMember;
      }
    }
  }
}
