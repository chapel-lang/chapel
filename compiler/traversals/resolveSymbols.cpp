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
    UnresolvedSymbol* UnresolvedMember;
    Symbol* ResolvedMember;

    if (UnresolvedMember = dynamic_cast<UnresolvedSymbol*>(dot->member)) {
      //      printf("hi\n");
    }
    if (RunAnalysis::runCount) {
      if (UnresolvedMember = dynamic_cast<UnresolvedSymbol*>(dot->member)) {
	if (resolve_symbol(UnresolvedMember, dot, ResolvedMember)) {
	  INT_FATAL(dot, "Major error resolving MemberAccess in ResolveSymbols");
	}
	dot->member = ResolvedMember;
      }
    }
  }
}
