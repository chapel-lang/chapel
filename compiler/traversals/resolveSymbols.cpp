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

  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (typeid(*paren) == typeid(ParenOpExpr)) {

      // Don't resolve defaultVal in Type REMOVE
      if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(paren->stmt)) {
        if (DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList)) {
          if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
            if (type_sym->type->defaultVal == paren) {
              return;
            }
          }
        }
      }

      if (Variable* var = dynamic_cast<Variable*>(paren->baseExpr)) {
        if (UnresolvedSymbol* call = 
            dynamic_cast<UnresolvedSymbol*>(var->var)) {
          if (strcmp(call->name, "__primitive")) { /** can't resolve that **/

            if (analyzeAST) {
              Vec<FnSymbol*> fns;
              call_info(paren, fns);
              if (fns.n != 1) {
                INT_FATAL(expr, "Trouble resolving function call");
              } else {
                FnCall* fn = new FnCall(new Variable(fns.e[0]),
                                        paren->argList->copyList());
                expr->replace(fn);
              }
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
                    // Try to resolve overloaded functions
                    // This is just a stopgap for developing
                    // It is not accurate, but good enough for some things
                    FnSymbol* tmp = fn;
                    FnSymbol* candidate = NULL;
                    while (tmp) {
                      Symbol* formals = tmp->formals;
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
                        if (candidate) {
                          INT_FATAL(expr, 
                                    "Unable to resolve overloaded"
                                    "function without analysis");
                        }
                        candidate = tmp;
                      }
                      tmp = tmp->overload;
                    }
                    if (!candidate) {
                          INT_FATAL(expr, 
                                    "Unable to resolve overloaded"
                                    "function without analysis");
                    } else {
                      FnCall* fn_call = new FnCall(new Variable(candidate),
                                                   paren->argList->copyList());
                      expr->replace(fn_call);

                    }
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
        } else {
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
      }
    }
  }
}

