#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"
#include "../passes/runAnalysis.h"

/** Assumption: Analysis has run **/

void ResolveSymbols::postProcessExpr(Expr* expr) {

  if (MemberAccess* dot = dynamic_cast<MemberAccess*>(expr)) {
    if (UnresolvedSymbol* UnresolvedMember =
	dynamic_cast<UnresolvedSymbol*>(dot->member)) {
      if (RunAnalysis::runCount > 0) {
	Type* type;
	int offset;
	resolve_member_access(dot, &offset, &type);
	if (ClassType* class_type = dynamic_cast<ClassType*>(type)) {
	  dot->member = 
	    Symboltable::lookupInScope(UnresolvedMember->name,
				       class_type->classScope);
	    /** Can I use offset here? **
	    class_type->fields.v[offset]; offset = 0, 8, ...?
	    Is this the offset in the generated type
	    I guess I use lookupInScope here?
	    */
	}

	    /*  OLD API
	Symbol* ResolvedMember;
	if (resolve_symbol(UnresolvedMember, dot, ResolvedMember)) {
	  INT_FATAL(dot, "Major error resolving MemberAccess in ResolveSymbols");
	}
	dot->member = ResolvedMember;
	    */
      }
      else {
	if (Variable* var = dynamic_cast<Variable*>(dot->base)) {
	  if (ClassType* class_type = dynamic_cast<ClassType*>(var->var->type)) {
	    Symbol* sym = Symboltable::lookupInScope(UnresolvedMember->name,
						     class_type->classScope);
	    if (sym) {
	      dot->member = sym;
	    }
	    else {
	      INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	    }
	  }
	  else {
	    INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	  }
	}
	else {
	  INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	}
      }
    }
  }

  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (typeid(*paren) == typeid(ParenOpExpr)) {
      if (Variable* var = dynamic_cast<Variable*>(paren->baseExpr)) {
	if (UnresolvedSymbol* call = 
	    dynamic_cast<UnresolvedSymbol*>(var->var)) {
	  if (strcmp(call->name, "__primitive")) { /** can't resolve that **/

	    if (RunAnalysis::runCount > 0) {
	      Vec<FnSymbol*> fns;
	      call_info(paren, fns);
	      if (fns.n != 1) {
		INT_FATAL(expr, "Trouble resolving function call");
	      }
	      FnCall* fn = new FnCall(new Variable(fns.e[0]),
				      paren->argList->copyList());
	      expr->replace(fn);
	    }
	    else {
	      if (Symbol* sym = Symboltable::lookup(call->name)) {
		if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
		  if (!fn->overload) {
		    FnCall* fn_call = new FnCall(new Variable(fn),
					    paren->argList->copyList());
		    expr->replace(fn_call);
		  }
		  else {
		    INT_FATAL(expr, 
			      "Unable to resolve overloaded"
			      "function without analysis");
		  }
		}
		else {
		  INT_FATAL(expr,
			    "Unable to resolve 'strange' function without analysis");
		}
	      }
	      else {
		INT_FATAL(expr,
			  "Unable to resolve unknownfunction without analysis");
	      }
	    }
	  }
	}
      }
    }
  }
}

