#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"

/** Assumption: Analysis has run **/

void ResolveSymbols::postProcessExpr(Expr* expr) {

  if (Variable* var_expr = dynamic_cast<Variable*>(expr)) {
    if (var_expr->var->type == dtUnknown) {
      var_expr->var->type = type_info(var_expr->var);
    }
  }

  if (MemberAccess* dot = dynamic_cast<MemberAccess*>(expr)) {

    /** Resolve methods with arguments at ParenOpExpr */
//     if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr->parent)) {
//       if (paren_op->baseExpr == expr) {
//         return;
//       }
//     }

    if (UnresolvedSymbol* UnresolvedMember =
        dynamic_cast<UnresolvedSymbol*>(dot->member)) {
      if (analyzeAST) {
        if (StructuralType* class_type = dynamic_cast<StructuralType*>(dot->base->typeInfo())) {
          dot->member = 
            Symboltable::lookupInScope(UnresolvedMember->name,
                                       class_type->structScope);
        }
      } else {
        Expr* base = dot->base;
        while (ArrayRef* array_ref = dynamic_cast<ArrayRef*>(base)) {
          base = array_ref->baseExpr;
        }
        if (Variable* var = dynamic_cast<Variable*>(base)) {
          Type* type = var->var->type;
          while (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
            type = array_type->elementType;
          }
          if (StructuralType* class_type = dynamic_cast<StructuralType*>(type)) {
            Symbol* sym = Symboltable::lookupInScope(UnresolvedMember->name,
                                                     class_type->structScope);
            if (sym) {
              dot->member = sym;
            } else {
              INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
            }
          } else {
            INT_FATAL(expr, "No Analysis Resolution Failure (NARF)");
          }
        } else if (MemberAccess* outer = dynamic_cast<MemberAccess*>(base)) {
          if (StructuralType* class_type = dynamic_cast<StructuralType*>(outer->member->type)) {
            Symbol* sym = Symboltable::lookupInScope(UnresolvedMember->name,
                                                     class_type->structScope);
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

  if (typeid(*expr) == typeid(ParenOpExpr)) {
    ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr);
    if (Variable* var = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (UnresolvedSymbol* call = dynamic_cast<UnresolvedSymbol*>(var->var)) {
        if (strcmp(call->name, "__primitive")) { /** can't resolve that **/
          
          if (analyzeAST) {
            Vec<FnSymbol*> fns;
            call_info(paren, fns);
            if (fns.n != 1) {
              INT_FATAL(expr, "Unable to resolve function");
            } else {
              FnCall* fn = new FnCall(new Variable(fns.e[0]),
                                      paren->argList->copyList());
              expr->replace(fn);
            }
          } else {
            SymScope* scope = Symboltable::getCurrentScope();
            Vec<FnSymbol*> fns;
            for (int i = 0; i < scope->visibleFunctions.n; i++) {
              Vec<FnSymbol*>* fs = scope->visibleFunctions.v[i].value;
              if (fs) {
                forv_Vec(FnSymbol, fn, *fs) {
                  if (fn && !strcmp(fn->name, call->name)) {
                    fns.add(fn);
                  }
                }
              }
            }
            FnSymbol* candidate = NULL;
            forv_Vec(FnSymbol*, fn, fns) {
              Symbol* formals = fn->formals;
              Expr* actuals = paren->argList;
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
                  INT_FATAL(expr, "Unable to resolve function");
                }
                candidate = fn;
              }
            }
            if (!candidate) {
              INT_FATAL(expr, "Unable to resolve function");
            }
            Expr* args = paren->argList->copyList();
            Expr* fn_call = new FnCall(new Variable(candidate), args);
            expr->replace(fn_call);
          }
        }
      } else {
        if (strcmp(var->var->name, "__primitive")) { /** can't resolve that **/
          if (analyzeAST) {
            Vec<FnSymbol*> fns;
            call_info(paren, fns);
            if (fns.n != 1) {
              INT_FATAL(expr, "Trouble resolving function call");
            }
            if (fns.e[0] != var->var) {
              INT_WARNING(var, "Analysis function call mismatch");
              FnCall* fn = new FnCall(new Variable(fns.e[0]),
                                      paren->argList->copyList());
              expr->replace(fn);
            }
          }
        }
      }
    }
  }
}

