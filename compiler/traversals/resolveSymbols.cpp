#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


static Expr* copy_argument_list(ParenOpExpr* expr) {
  Expr* args = NULL;
  MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr);
  if (member_access) {
    args = member_access->base->copy();
  }
  args = appendLink(args, expr->argList->copyList());
  return args;
}


static void call_info_noanalysis(ParenOpExpr* expr, Vec<FnSymbol*>& fns) {
  char* name;
  if (Variable* variable = dynamic_cast<Variable*>(expr->baseExpr)) {
    name = variable->var->name;
  } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr)) {
    name = member_access->member->name;
  } else {
    INT_FATAL(expr, "Unable to resolve function without analysis");
  }
  SymScope* scope = Symboltable::getCurrentScope();
  Vec<FnSymbol*> candidates;
  for (int i = 0; i < scope->visibleFunctions.n; i++) {
    Vec<FnSymbol*>* fs = scope->visibleFunctions.v[i].value;
    if (fs) {
      forv_Vec(FnSymbol, fn, *fs) {
        if (fn && !strcmp(fn->name, name)) {
          candidates.add(fn);
        }
      }
    }
  }
  FnSymbol* candidate = NULL;
  forv_Vec(FnSymbol*, fn, candidates) {
    Symbol* formals = fn->formals;
    Expr* actuals = copy_argument_list(expr);
    bool match = true;
    while (actuals && formals) {
      if (actuals->typeInfo() != formals->type) {
        match = false;
        break;
      }
      actuals = nextLink(Expr, actuals);
      formals = nextLink(Symbol, formals);
    }
    if (actuals || formals) {
      match = false;
    }
    if (match) {
      if (candidate != NULL && candidate != fn) {
        if (strncmp("_construct", fn->name, 10)) {
          INT_FATAL(expr, "Unable to resolve function");
        }
      }
      candidate = fn;
    }
  }
  if (!candidate) {
    INT_FATAL(expr, "Unable to resolve function");
  }
  fns.add(candidate);
}


ResolveSymbols::ResolveSymbols() {
  whichModules = MODULES_COMMON_AND_USER;
}


void ResolveSymbols::postProcessExpr(Expr* expr) {
  if (typeid(*expr) == typeid(ParenOpExpr)) {
    ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr);
    Vec<FnSymbol*> fns;

    if (analyzeAST) {
      call_info(paren, fns);
      if (fns.n == 0) {
        call_info(paren->baseExpr, fns);
      }
      if (fns.n != 1) {
        INT_FATAL(expr, "Unable to resolve function");
      }
    } else {
      call_info_noanalysis(paren, fns);
      if (fns.n != 1) {
        INT_FATAL(expr, "Unable to resolve function without analysis");
      }
    }

    Expr* arguments = copy_argument_list(paren);
    Expr* function = new Variable(fns.e[0]);
    expr->replace(new FnCall(function, arguments));
  }

  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {

    /***
     *** Resolve methods with arguments at ParenOpExpr
     ***/
    if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }

    if (dynamic_cast<UnresolvedSymbol*>(member_access->member)) {
      if (analyzeAST) {
        /*** Need to use call_info here once getters are working ***/
      }

      StructuralType* struct_scope =
        dynamic_cast<StructuralType*>(member_access->base->typeInfo());
      if (struct_scope) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);

        if (dynamic_cast<FnSymbol*>(member_access->member)) {
          Expr* arguments = member_access->base->copy();
          Expr* function = new Variable(member_access->member);
          expr->replace(new FnCall(function, arguments));
        }
      } else {
        INT_FATAL(expr, "Cannot resolve MemberAccess");
      }
    }
  }

  if (useNewConstructor) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
      if (analyzeAST) {
        Vec<FnSymbol*> fns;
        call_info(defExpr, fns);
        if (fns.n == 1) {
          defExpr->sym->type->defaultConstructor = fns.v[0];
        }
      }
    }
  }
}
