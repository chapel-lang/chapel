#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


//#define ANALYSIS_MATCH


/** Assumption: Analysis has run **/

void ResolveSymbols::postProcessExpr(Expr* expr) {

  if (Variable* var_expr = dynamic_cast<Variable*>(expr)) {
    if (var_expr->var->type == dtUnknown) {
      var_expr->var->type = type_info(var_expr->var);
    }
  }

  if (MemberAccess* dot = dynamic_cast<MemberAccess*>(expr)) {
    if (UnresolvedSymbol* UnresolvedMember =
	dynamic_cast<UnresolvedSymbol*>(dot->member)) {
      if (analyzeAST) {
	if (ClassType* class_type = dynamic_cast<ClassType*>(dot->base->typeInfo())) {
	  dot->member = 
	    Symboltable::lookupInScope(UnresolvedMember->name,
				       class_type->classScope);
	}
      } else {
	if (Variable* var = dynamic_cast<Variable*>(dot->base)) {
	  if (ClassType* class_type = dynamic_cast<ClassType*>(var->var->type)) {
	    Symbol* sym = Symboltable::lookupInScope(UnresolvedMember->name,
						     class_type->classScope);
	    if (sym) {
	      dot->member = sym;
	    } else {
	      INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	    }
	  } else {
	    INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	  }
	} else if (MemberAccess* outer = dynamic_cast<MemberAccess*>(dot->base)) {
	  if (ClassType* class_type = dynamic_cast<ClassType*>(outer->member->type)) {
	    Symbol* sym = Symboltable::lookupInScope(UnresolvedMember->name,
						     class_type->classScope);
	    if (sym) {
	      dot->member = sym;
	    } else {
	      INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	    }
	  } else {
	    INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
	  }
	} else {
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

	    if (analyzeAST) {
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
#ifdef ANALYSIS_MATCH
	else {
	  if (strcmp(var->var->name, "__primitive")) { /** can't resolve that **/
	    if (analyzeAST) {
	      Vec<FnSymbol*> fns;
	      call_info(paren, fns);
	      if (fns.n != 1) {
		INT_FATAL(expr, "Trouble resolving function call");
	      }
	      if (fns.e[0] != var->var) {
		INT_WARNING(var, "Analysis function call mismatch, using analysis function");
		FnCall* fn = new FnCall(new Variable(fns.e[0]),
					paren->argList->copyList());
		expr->replace(fn);
	      }
	    }
	  }
	}
#endif
      }
    }
  }
}

