#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "earlyTypeCheck.h"
#include "congruenceClosure.h"
#include "typeCheck.h"
#include "callInfo.h"

//FIXME: Convert these over to the Chapel structures
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <algorithm>

//Predeclare functions
static BaseAST *typeCheckExpr(BaseAST *currentExpr, BaseAST *expectedReturnTypeExpr, Map<FnSymbol*, ArgSymbol*>&);
static void handleWhereClauseExpr(BaseAST *ast);
static BaseAST *typeCheckFn(FnSymbol *fn);
static BaseAST *checkInterfaceImplementations(BaseAST *s);
static Symbol* mapArguments(CallExpr*, FnSymbol*, CallExpr*);
static BaseAST* checkFunctionCall(CallExpr*, Map<FnSymbol*, ArgSymbol*>&);
static void addAdaptationToWitness(BaseAST*, ClassType *witness, FnSymbol *requiredFn, FnSymbol *actualFn);
static void addToImplementsSymbolTable(CallExpr* ce, BaseAST* interface, BaseAST* implementingType, Symbol *implementingWitness);

static void getMatchingFunctionsInInterfaces(const char* name, Vec<FnSymbol*>& visibleFns,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces);

static void addToImplementsSymbolTable(CallExpr* ce, BaseAST* interface, BaseAST* implementingType, Symbol *implementingWitness);
static void getWitnessesInWhereClause(Vec<CallExpr*>& implementsClauses, BaseAST *whereClause);
static void getFunctionsInWhereClause(Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces,
    Vec<ArgSymbol*>& witnessObjects, BaseAST *whereClause);

CongruenceClosure cclosure;

/*
 * FIXME: With CallInfo, respect isResolved for a speed improvement
 */

/*
 * FIXME: For performance reasons, this should be shared with function resolution
 */
static VisibleFunctionManager vFunManager;
static Vec<FnSymbol*> visitedFns;

/*
 * Adapted code for SymbolTable required for scoped implements clauses
 * Code taken from scopeResolve.cpp
 * Code is modified - structure of SymbolTable
 */

typedef Map<BaseAST*, Symbol*> ImplementingTypeList;
typedef Map<BaseAST*, ImplementingTypeList*> ImplementedInterfaceList;
typedef Map<BaseAST*, ImplementedInterfaceList*> ImplementsSymbolTable;

static ImplementsSymbolTable implementsSymbolTable;

static void
addToImplementsSymbolTable(CallExpr* ce, BaseAST* interface, BaseAST* implementingType,
    Symbol *implementingWitness){
  BaseAST* scope = getScope(ce);
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    interfaceEntry = new ImplementedInterfaceList();
    implementsSymbolTable.put(scope,interfaceEntry);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface);
  if(!typeEntry) {
    typeEntry = new ImplementingTypeList();
    interfaceEntry->put(interface_node,typeEntry);
  }
  typeEntry->put(implementingType_node,implementingWitness);
}

static bool
lookupImplementsSymbolTable(BaseAST* ce, BaseAST* interface, BaseAST* implementingType) {
  BaseAST* scope = getScope(ce);
  if(!scope)
    return false;
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface_node);
  if(!typeEntry)
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  Symbol* witness = typeEntry->get(implementingType_node);
  if(!witness)
    return lookupImplementsSymbolTable(scope,interface,implementingType);
  return true;
}

static Symbol*
lookupImplementsWitnessInSymbolTable(BaseAST* ce, BaseAST* interface, BaseAST* implementingType) {
  BaseAST* scope = getScope(ce);
  if(!scope)
    return NULL;
  BaseAST* interface_node = cclosure.get_representative_ast(interface);
  BaseAST* implementingType_node = cclosure.get_representative_ast(implementingType);
  ImplementedInterfaceList* interfaceEntry = implementsSymbolTable.get(scope);
  if(!interfaceEntry) {
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  }
  ImplementingTypeList* typeEntry = interfaceEntry->get(interface_node);
  if(!typeEntry)
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  Symbol* witness = typeEntry->get(implementingType_node);
  if(!witness)
    return lookupImplementsWitnessInSymbolTable(scope,interface,implementingType);
  return witness;
}

typedef MapElem<FnSymbol *, ArgSymbol *> DictElem;

static void getMatchingFunctionsInInterfaces(const char* name, Vec<FnSymbol*>& visibleFns,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces) {

  form_Map(DictElem, elem, fnsInInterfaces) {
    if (!strcmp(elem->key->name, name)) {
      visibleFns.add(elem->key);
    }
  }
}

// Typechecks the given ast node with the expected return (in case a return
// is encountered)
static BaseAST *typeCheckExpr(BaseAST *currentExpr, BaseAST *expectedReturnTypeExpr,
    Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces) {

  if (SymExpr *se_actual = toSymExpr(currentExpr)) {

    if (se_actual->var->type && se_actual->var->type != dtUnknown)
      return se_actual->var->type;

    ArgSymbol *argSym;
    DefExpr *defPt;
    if ((argSym = toArgSymbol(se_actual->var)) && argSym->typeExpr) {
      return cclosure.get_representative_ast(argSym->typeExpr);
    } else if ((defPt = se_actual->var->defPoint)) {
      return typeCheckExpr(defPt, expectedReturnTypeExpr, fnsInInterfaces);
    }
  } else if (CallExpr* call = toCallExpr(currentExpr)) {
    Vec<Expr*> checked_arg_exprs;

    if (call->primitive && (call->primitive->tag == PRIM_RETURN)) {
      BaseAST *e_typeAST = typeCheckExpr(call->argList.head,
          expectedReturnTypeExpr, fnsInInterfaces);

      if (!cclosure.is_equal(e_typeAST, expectedReturnTypeExpr)) {
        INT_FATAL("Mismatched type expressions in return\n");
      } else {
        return e_typeAST;
      }
    } else if (UnresolvedSymExpr *use = toUnresolvedSymExpr(call->baseExpr)) {
      if (!strcmp(use->unresolved, "implements")) {
        checkInterfaceImplementations(call);
        return NULL;
      }
    }
    BaseAST* retExpr = checkFunctionCall(call,fnsInInterfaces);
    if(retExpr == dtUnknown) {
      USR_FATAL(call, "No matching functions at call");
    } else {
      return retExpr;
    }
  } else if (isImplementsStmt(currentExpr)) {
    checkInterfaceImplementations(currentExpr);
  } else if (UnresolvedSymExpr *use = toUnresolvedSymExpr(currentExpr)) {
    INT_FATAL("UNIMPLEMENTED: Unresolved SymExpr: %s\n", use->unresolved);
  } else if (DefExpr *de = toDefExpr(currentExpr)) {
    if (de->exprType && de->init) {
      if (!cclosure.is_equal(de->exprType, de->init)) {
        INT_FATAL(
            "Declared variable type does not match initialization expression\n");
      }
    } else if (de->exprType) {
      return de->exprType;
    } else if (de->init) {
      return de->init;
    } else {
      INT_FATAL("Generic variable without initializing expression\n");
    }
  } else if (NamedExpr *ne = toNamedExpr(currentExpr)) {
    return typeCheckExpr(ne->actual, expectedReturnTypeExpr, fnsInInterfaces);
  } else if (BlockStmt *block = toBlockStmt(currentExpr)) {
    BaseAST *ret = NULL;
    for_alist(e, block->body) {
      ret = typeCheckExpr(e, expectedReturnTypeExpr, fnsInInterfaces);
    }
    return ret;
  } else if (CondStmt *cond = toCondStmt(currentExpr)) {
    //INT_FATAL("UNIMPLEMENTED: ContStmt\n");
    if (CallExpr *cond_test = toCallExpr(cond->condExpr)) {
      BaseAST *conditional = typeCheckExpr(cond_test->argList.head,
          expectedReturnTypeExpr, fnsInInterfaces);
      if (cclosure.is_equal(conditional, dtBool)) {
        typeCheckExpr(cond->thenStmt, expectedReturnTypeExpr, fnsInInterfaces);
        typeCheckExpr(cond->elseStmt, expectedReturnTypeExpr, fnsInInterfaces);
      }
      else {
        INT_FATAL("Expected boolean expression in if condition");
      }
    }
    else {
      INT_FATAL("Expected boolean expression in if condition");
    }
  } else if (GotoStmt *gotoStmt = toGotoStmt(currentExpr)) {
    typeCheckExpr(gotoStmt->label, expectedReturnTypeExpr, fnsInInterfaces);
  } else {
    if (currentExpr) {
      INT_FATAL("UNIMPLEMENTED: <expr type unknown: %i %i %i %i>\n",
          currentExpr->astTag, E_Expr, E_Symbol, E_Type);
    } else {
      INT_FATAL("UNIMPLEMENTED: <expr is NULL>\n");
    }
  }
  return NULL;
}

static void getWitnessesInWhereClause(Vec<CallExpr*>& implementsClauses, BaseAST *whereClause) {
  if (CallExpr *ce = toCallExpr(whereClause)) {
    if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(callsymexpr->unresolved, "implements")) {
        //First find the interface that was implemented and open it up
        BaseAST *interface_implemented = ce->argList.get(2);
        if (SymExpr *se = toSymExpr(interface_implemented)) {
          // Is a symexpr
          if (isInterfaceSymbol(se->var)) {
            implementsClauses.add(ce);
          }
          else {
            INT_FATAL("Implementation phrase with non-interface symbol");
          }
        }
        else {
          // Isn't a symexpr, something is wrong
        }
      } else if (!strcmp(callsymexpr->unresolved, "_build_tuple")) {
        //We have multiple constraints, handle them
        for_alist(arg, ce->argList) {
          handleWhereClauseExpr(arg);
        }
      }
    }
    else {
      //printf("Not unresolved\n");
    }
  }
  else if (BlockStmt *block = toBlockStmt(whereClause)) {
    for_alist(expr, block->body) {
      getWitnessesInWhereClause(implementsClauses, expr);
    }
  }
}

/*
static void
getFunctionsInWhereClause(const char* name, Vec<FnSymbol*>& visibleFns,
    BaseAST *whereClause) {
*/

static void
getFunctionsInWhereClause(Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces,
    Vec<ArgSymbol*>& witnessObjects, BaseAST *whereClause) {

  Vec<CallExpr*> implementsClauses;
  getWitnessesInWhereClause(implementsClauses, whereClause);

  forv_Vec(CallExpr, ce, implementsClauses) {
    //First find the interface that was implemented and open it up
    SymExpr *implementing_type = toSymExpr(ce->argList.get(1));
    BaseAST *interface_implemented = ce->argList.get(2);
    if (SymExpr *se = toSymExpr(interface_implemented)) {
      // Is a symexpr
      if (InterfaceSymbol *is = toInterfaceSymbol(se->var)) {

        // Create an symbol that will represent this dictionary
        ArgSymbol *dict = new ArgSymbol(INTENT_BLANK,
            astr(implementing_type->var->name, "_impl_", se->var->name), dtAny);
        dict->addFlag(FLAG_GENERIC);

        witnessObjects.add(dict);

        addToImplementsSymbolTable(ce, is, se, dict);

        forv_Vec(FnSymbol, fn, is->functionSignatures) {
          //For now, copy the function prototype and replace the types
          //with the ones we know
          FnSymbol *fn_copy = fn->copy();
          fn_copy->addFlag(FLAG_INVISIBLE_FN);

          //replace formal types
          for_formals(arg, fn_copy) {
            //list_view(arg);
            if (BlockStmt *bs = toBlockStmt(arg->typeExpr)) {
              if (UnresolvedSymExpr *use = toUnresolvedSymExpr(bs->body.head)) {
                if (!strcmp(use->unresolved, "self")) {
                  bs->body.head = ce->argList.get(1)->copy();
                }
              }
            }
          }
          //replace return type
          if (BlockStmt *bs = toBlockStmt(fn_copy->retExprType)) {
            if (UnresolvedSymExpr *use = toUnresolvedSymExpr(bs->body.head)) {
              if (!strcmp(use->unresolved, "self")) {
                bs->body.head = ce->argList.get(1)->copy();
              }
            }
          }

          fnsInInterfaces.put(fn_copy, dict);
          //list_view(fn_copy);
        }
      }
      else {
        INT_FATAL("Implementation phrase with non-interface symbol");
      }
    }
  }
}

static void handleWhereClauseExpr(BaseAST *ast) {
  if (CallExpr *ce = toCallExpr(ast)) {
    if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(callsymexpr->unresolved, "==")) {
        //Equality constraint, let's add it
        BaseAST *arg1 = ce->argList.get(1);
        BaseAST *arg2 = ce->argList.get(2);
        cclosure.equate(arg1, arg2);
      } else if (!strcmp(callsymexpr->unresolved, "_build_tuple")) {
        //We have multiple constraints, handle them
        for_alist(arg, ce->argList) {
          handleWhereClauseExpr(arg);
        }
      }
    } else {
      //printf("Not unresolved\n");
    }
  }
}

static BaseAST *typeCheckFn(FnSymbol *fn) {
  // Look through our formal arguments
  for_formals(formal, fn) {
    if (!formal->typeExpr && (!formal->flags.test(FLAG_TYPE_VARIABLE))) {
      INT_FATAL("Formal is missing explicit type annotation\n");
    }
  }
  if (!fn->retExprType) {
    INT_FATAL("function missing explicit return type annotation\n");
  }

  if (fn->where) {
    for_alist(expr, fn->where->body) {
      handleWhereClauseExpr(expr);
    }
  }

  Map<FnSymbol*, ArgSymbol*> fnsInInterfaces;
  Vec<ArgSymbol*> witnessObjects;
  getFunctionsInWhereClause(fnsInInterfaces, witnessObjects, fn->where);

  forv_Vec(ArgSymbol, arg, witnessObjects) {
    fn->insertFormalAtTail(arg);
  }

  // Now, look through the function body
  BaseAST *tmp = typeCheckExpr(fn->body, fn->retExprType, fnsInInterfaces);

  //FIXME: This is a workaround until we have a clean-up pass
  //fn->where->remove();

  if (witnessObjects.length() > 0) {
    fn->addFlag(FLAG_ALLOW_REF);
    fn->addFlag(FLAG_GENERIC);
    fn->removeFlag(FLAG_CHECKED);
  }

  return tmp;
}

static Symbol* mapArguments(CallExpr* where, FnSymbol* visibleFn, CallExpr* call) {
  BaseAST *arg1 = where->argList.get(1);
  BaseAST *arg2 = where->argList.get(2);
  SymExpr *s_arg1 = toSymExpr(arg1);
  Expr *e_actual = call->argList.head;
  ArgSymbol *s_formal =
      ((visibleFn)->formals.head) ?
          toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
  for (;
      e_actual;
      e_actual = e_actual->next, s_formal =
          (s_formal && s_formal->defPoint->next) ?
              toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) : NULL) {
    if (s_formal->id == s_arg1->var->id) {
      if(Symbol *witness = lookupImplementsWitnessInSymbolTable(call, arg2, e_actual)) {
        return witness;
      } else {

        return NULL;
      }

    }
  }
  return NULL;
}

static BaseAST* checkFunctionCall(CallExpr* call, Map<FnSymbol*, ArgSymbol*>& fnsInInterfaces) {
  if(call->primitive) {
    USR_FATAL(call,"Unimplemented: Primitive operators\n");
  }
  BaseAST* retExpr;
  //First, check to see if it should be a primitive but it hasn't been
  //resolved yet
  if (UnresolvedSymExpr *use = toUnresolvedSymExpr(call->baseExpr)) {
    if (PrimitiveOp *op = primitives_map.get(use->unresolved)) {
      if (call->argList.length == 0) {
        return op->returnInfo(call, NULL, NULL);
      } else if (call->argList.length == 1) {
        return op->returnInfo(call,
            cclosure.get_representative_ast(call->argList.get(1)), NULL);
      } else {
        return op->returnInfo(call,
            cclosure.get_representative_ast(call->argList.get(1)),
            cclosure.get_representative_ast(call->argList.get(2)));
      }
    }
  }

  CallInfo info(call, false);
  Vec<FnSymbol*> visibleFns;

  vFunManager.buildVisibleFunctionMap();

  if (!call->isResolved()) {
    // Also include the functions that come into scope because of
    // implements statements in the where clause
    //getFunctionsInWhereClause(info.name, visibleFns, whereClause);
    getMatchingFunctionsInInterfaces(info.name, visibleFns, fnsInInterfaces);

    if (!info.scope) {
      Vec<BlockStmt*> visited;
      vFunManager.getVisibleFunctions(vFunManager.getVisibilityBlock(call), info.name, visibleFns,
          visited);
    } else {
      Vec<FnSymbol*> fns;
      Vec<BlockStmt*> visited;
      vFunManager.getVisibleFunctions(info.scope, info.name, fns, visited);
      visibleFns.append(fns);
    }
  } else {
    visibleFns.add(call->isResolved());
  }

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    bool mismatch = false;
    Expr *e_actual = call->argList.head;
    ArgSymbol *s_formal =
        ((visibleFn)->formals.head) ?
            toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
    for (;
        e_actual;
        e_actual = e_actual->next, s_formal =
            (s_formal && s_formal->defPoint->next) ?
                toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
                NULL) {
      if (!s_formal) {
        mismatch = true;
        break;
      }
      if (s_formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (SymExpr *se_actual = toSymExpr(e_actual)) {
          if (!isTypeSymbol(se_actual->var)) {
            mismatch = true;
            break;
          } else {
            cclosure.equate(s_formal,se_actual->var);
          }
        } else {
          mismatch = true;
          break;
        }
      } else if (!cclosure.is_equal(e_actual, s_formal->typeExpr)) {
        mismatch = true;
        break;
      }
    }

    if (!mismatch) {
      if (visibleFn->where) {
        for_alist(expr,visibleFn->where->body) {
          if (CallExpr* where_expr = toCallExpr(expr)) {
            if (UnresolvedSymExpr *callsymexpr = toUnresolvedSymExpr(where_expr->baseExpr)) {
              if ((!strcmp(callsymexpr->unresolved, "_build_tuple"))
                  || (!strcmp(callsymexpr->unresolved, "PRIM_ACTUALS_LIST"))) {
                for_alist(wl, where_expr->argList) {
                  if (CallExpr *where_ce = toCallExpr(wl)) {
                    if (UnresolvedSymExpr *use = toUnresolvedSymExpr(where_ce->baseExpr)) {
                      if (!strcmp(use->unresolved, "implements")) {
                        //printf("where with implements found\n");
                        if(Symbol *witness = mapArguments(where_ce, visibleFn, call)){
                          retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
                          cclosure.make_parent_null((visibleFn->retExprType)->body.head);

                          call->insertAtTail(new SymExpr(witness));
                          if (ArgSymbol *obj = fnsInInterfaces.get(visibleFn)) {
                            // Convert call to method call on interface dictionary
                            call->baseExpr->replace(new CallExpr(".", new SymExpr(obj),
                                new_StringSymbol(info.name)));
                          }

                          return retExpr;
                        }
                        else {
                          cclosure.make_parent_null((visibleFn->retExprType)->body.head);

                          return dtUnknown;
                        }
                      }
                    }
                  }
                }
              } else if (!strcmp(callsymexpr->unresolved, "implements")) {
                //printf("where with implements found\n");
                if(Symbol *witness = mapArguments(where_expr, visibleFn, call)){
                  retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
                  cclosure.make_parent_null((visibleFn->retExprType)->body.head);

                  call->insertAtTail(new SymExpr(witness));
                  if (ArgSymbol *obj = fnsInInterfaces.get(visibleFn)) {
                    // Convert call to method call on interface dictionary
                    call->baseExpr->replace(new CallExpr(".", new SymExpr(obj),
                        new_StringSymbol(info.name)));
                  }

                  return retExpr;
                }
                else {
                  cclosure.make_parent_null((visibleFn->retExprType)->body.head);
                  return dtUnknown;
                }
              }
            }
          }
        }
      }
      else{
        retExpr = cclosure.get_representative_ast((visibleFn->retExprType)->body.head);
        cclosure.make_parent_null((visibleFn->retExprType)->body.head);
        if (ArgSymbol *obj = fnsInInterfaces.get(visibleFn)) {
          // Convert call to method call on interface dictionary
          call->baseExpr->replace(new CallExpr(".", new SymExpr(obj),
              new_StringSymbol(info.name)));
        }

        return retExpr;
      }
    }
  }

  return dtUnknown;
}


/*
  Creates the parent class which will represent the function's type.  Children of the parent class will capture different functions which
  happen to share the same function type.  By using the parent class we can assign new values onto variable that match the function type
  but may currently be pointing at a different function.
*/
static ClassType* createAndInsertInstanceWitnessClass(BaseAST *witness, const char *name) {
  ClassType *parent = new ClassType(CLASS_CLASS);
  TypeSymbol *parent_ts = new TypeSymbol(name, parent);

  //Adding support for implements stmt
  if(ImplementsStmt* s = toImplementsStmt(witness)) {
    s->insertBefore(new DefExpr(parent_ts));
  } else if (CallExpr* ce = toCallExpr(witness)) {
    ce->insertBefore(new DefExpr(parent_ts));
  }

  parent->dispatchParents.add(dtObject);
  dtObject->dispatchChildren.add(parent);
  VarSymbol* parent_super = new VarSymbol("super", dtObject);
  parent_super->addFlag(FLAG_SUPER_CLASS);
  parent->fields.insertAtHead(new DefExpr(parent_super));
  build_constructor(parent);
  build_type_constructor(parent);

  return parent;
}

static void addAdaptationToWitness(BaseAST *insertBefore, ClassType *witness, FnSymbol *requiredFn, FnSymbol *actualFn) {
  FnSymbol* adapted_method = new FnSymbol(requiredFn->name);
  adapted_method->addFlag(FLAG_INLINE);
  adapted_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  ArgSymbol* thisWitnessSymbol = new ArgSymbol(INTENT_BLANK, "this", witness);
  thisWitnessSymbol->addFlag(FLAG_ARG_THIS);
  adapted_method->insertFormalAtTail(thisWitnessSymbol);
  adapted_method->_this = thisWitnessSymbol;

  CallExpr *ce = new CallExpr(actualFn);

  for_formals(formal, requiredFn) {
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, formal->name, dtAny);
    adapted_method->insertFormalAtTail(arg);
    ce->insertAtTail(new SymExpr(arg));
  }

  if (actualFn->retType == dtVoid) {
    adapted_method->insertAtTail(ce);
  }
  else {
    VarSymbol *tmp = newTemp("_return_tmp_");
    adapted_method->insertAtTail(new DefExpr(tmp));
    adapted_method->insertAtTail(new CallExpr(PRIM_MOVE, tmp, ce));

    adapted_method->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if(ImplementsStmt* s = toImplementsStmt(insertBefore)) {
    s->insertBefore(new DefExpr(adapted_method));
  } else if (CallExpr* c = toCallExpr(insertBefore)) {
    c->insertBefore(new DefExpr(adapted_method));
  }

  normalize(adapted_method);

  witness->methods.add(adapted_method);
}

static BaseAST* checkInterfaceImplementations(BaseAST *s) {
  BaseAST * returnExpr;

  if(BlockStmt* block = toBlockStmt(s)) {
    for_alist(stmt, block->body) {
      checkInterfaceImplementations(stmt);
    }
  }

  else if (DefExpr *de = toDefExpr(s)) {
      if (de->sym && isFnSymbol(de->sym)) {
        FnSymbol *fn = toFnSymbol(de->sym);
        if (!fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED))
          checkInterfaceImplementations(fn->body);
      }
  }
  else if (isCallExpr(s) || isImplementsStmt(s)) {
    CallExpr *ce;
    if(isCallExpr(s)) {
      ce = toCallExpr(s);
    }
    if(isImplementsStmt(s)) {
      ImplementsStmt* istmt = toImplementsStmt(s);
      ce = toCallExpr(istmt->implementsClause);
    }
    if (UnresolvedSymExpr *use = toUnresolvedSymExpr(ce->baseExpr)) {
      if (!strcmp(use->unresolved, "implements")) {
        ClassType* witness = createAndInsertInstanceWitnessClass(s, "witness");
        if (SymExpr *se = toSymExpr(ce->argList.tail)) {
          //if (SymExpr *implementing_type = toSymExpr(ce->argList.head)) {
          if (isSymExpr(ce->argList.head)) {
            if (InterfaceSymbol *is = toInterfaceSymbol(se->var)) {
              forv_Vec (FnSymbol, fn, is->functionSignatures) {
                  CallInfo info(fn, cclosure);

                if (PrimitiveOp *op = primitives_map.get(fn->cname)) {
                  if (fn->formals.length == 0) {
                    INT_FATAL("Unimplemented Operator: 0 args\n");
                    //if (cclosure.is_equal(e_actual, s_formal->typeExpr))
                    //return op->returnInfo(call, NULL, NULL);
                  } else if (fn->formals.length == 1) {
                    INT_FATAL("Unimplemented Operator: 1 arg\n");
                    //return op->returnInfo(call, cclosure.get_representative_ast(call->argList.get(1)), NULL);
                  } else {
                    //printf("Operator: 2 args\n");
                    //FIXME: I don't like passing NULL here, we need to be more flexible
                    //with what operators need

                    //FIXME: Handle dtUnknown

                    BaseAST *arg1Expr;
                    BaseAST *arg2Expr;

                    bool isArg1Self = false;
                    if (DefExpr *de1 = toDefExpr(fn->formals.get(1))) {
                      if (ArgSymbol *as1 = toArgSymbol(de1->sym)) {
                        if (UnresolvedSymExpr *use1 =
                        toUnresolvedSymExpr(as1->typeExpr->body.head)) {

                          if (!strcmp(use1->unresolved, "self"))
                            isArg1Self = true;
                        }
                      }
                    }
                    if (isArg1Self) {
                      arg1Expr = cclosure.get_representative_ast(
                          ce->argList.head);
                    } else {
                      arg1Expr = cclosure.get_representative_ast(
                          fn->formals.get(1));
                    }

                    bool isArg2Self = false;
                    if (DefExpr *de2 = toDefExpr(fn->formals.get(2))) {
                      if (ArgSymbol *as2 = toArgSymbol(de2->sym)) {
                        if (UnresolvedSymExpr *use2 =
                        toUnresolvedSymExpr(as2->typeExpr->body.head)) {

                          if (!strcmp(use2->unresolved, "self"))
                            isArg2Self = true;
                        }
                      }
                    }
                    if (isArg2Self) {
                      arg2Expr = cclosure.get_representative_ast(
                          ce->argList.head);
                    } else {
                      arg2Expr = cclosure.get_representative_ast(
                          fn->formals.get(2));
                    }

                    Type *retType = op->returnInfo(NULL, arg1Expr,
                        arg2Expr);

                    if (retType != dtUnknown) {
                      //FIXME: Can't assume 'self' is always unresolved
                      BlockStmt *retBlock = toBlockStmt(fn->retExprType);
                      if (retBlock) {
                        if (UnresolvedSymExpr *use3 =
                        toUnresolvedSymExpr(retBlock->body.head)) {
                          if (!strcmp(use3->unresolved, "self")) {
                            if (!cclosure.is_equal(ce->argList.head,
                                retType)) {
                              INT_FATAL(
                                  "Operator which satisfies interface requirement has mismatched type");
                            } else {
                              continue;
                            }
                          } else {
                            INT_FATAL("Return type of operator unknown");
                          }
                        }
                      }
                      if (!cclosure.is_equal(fn->retExprType, retType)) {
                        INT_FATAL(
                            "Operator which satisfies interface requirement has mismatched type");
                      } else {
                        continue;
                      }
                    }
                    //return op->returnInfo(call, cclosure.get_representative_ast(call->argList.get(1)),
                    //		cclosure.get_representative_ast(call->argList.get(2)));
                  }
                }

                vFunManager.buildVisibleFunctionMap();

                Vec<FnSymbol*> visibleFns; // visible functions

                Vec<BlockStmt*> visited;

                //First, add the visible functions

                if (ImplementsStmt* istmt = toImplementsStmt(s)) {   //the scope for visible functions for implements statement is its block_stmt
                  const char* function_name = info.name;
                  for_alist(stmt, istmt->statements->body) {
                    if(CallExpr *ce1 = toCallExpr(stmt)) {
                      if (UnresolvedSymExpr *use1 = toUnresolvedSymExpr(ce1->baseExpr)) {
                        if (!strcmp(use1->unresolved, "=")) {
                          Expr* lhs = ce1->argList.head;
                          Expr* rhs = ce1->argList.tail;
                          if(CallExpr* lhs_ce = toCallExpr(lhs)) {
                            Expr* lhs_fn = lhs_ce->argList.head;
                            if(UnresolvedSymExpr* us_lhs = toUnresolvedSymExpr(lhs_fn)) {
                              if(!strcmp(us_lhs->unresolved,info.name)) {
                                if(CallExpr* rhs_ce = toCallExpr(rhs)) {
                                  Expr* rhs_fn = rhs_ce->argList.head;
                                  if(UnresolvedSymExpr* us_rhs = toUnresolvedSymExpr(rhs_fn)) {
                                    function_name = us_rhs->unresolved;
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                  vFunManager.getVisibleFunctions(istmt->statements, function_name,
                    visibleFns, visited);
                } else {
                  vFunManager.getVisibleFunctions(vFunManager.getVisibilityBlock(ce), info.name,
                      visibleFns, visited);
                }

                bool found_match = false;
                forv_Vec(FnSymbol, visibleFn, visibleFns) {
                  bool mismatch = false;
                  Expr *e_actual = ce->argList.head;
                  ArgSymbol *s_formal =
                      ((visibleFn)->formals.head) ?
                          toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) :
                          NULL;
                  for (;
                      e_actual;
                      e_actual = e_actual->next, s_formal =
                          (s_formal && s_formal->defPoint->next) ?
                              toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) :
                              NULL) {

                    SymExpr *actual_sym = toSymExpr(e_actual);
                    if (!actual_sym || !isInterfaceSymbol(actual_sym->var)) {
                      if (!s_formal) {
                        mismatch = true;
                        break;
                      }
                      if (actual_sym
                          && !strcmp(actual_sym->var->cname, "self")) {
                        if (!cclosure.is_equal(e_actual,
                            s_formal->typeExpr)) {
                          mismatch = true;
                          break;
                        }
                      } else {
                        if (!cclosure.is_equal(ce->argList.head,
                            s_formal->typeExpr)) {
                          mismatch = true;
                          break;
                        }
                      }
                    }
                  }
                  if (!mismatch) {
                    //Next check return type.
                    Expr *retExpr;

                    if (BlockStmt *blockStmt = toBlockStmt(fn->retExprType))
                      retExpr = blockStmt->body.head;
                    else
                      retExpr = fn->retExprType;

                    SymExpr *retSymExpr = toSymExpr(retExpr);
                    UnresolvedSymExpr *retUnSymExpr =
                        toUnresolvedSymExpr(retExpr);
                    if (retSymExpr
                        && !strcmp(retSymExpr->var->cname, "self")) {
                      if (cclosure.is_equal(ce->argList.head,
                          visibleFn->retExprType)) {

                        //printf("Adding adaptation to witness\n");
                        addAdaptationToWitness(s, witness, fn, visibleFn);
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    } else if (retUnSymExpr
                        && !strcmp(retUnSymExpr->unresolved, "self")) {
                      if (cclosure.is_equal(ce->argList.head,
                          visibleFn->retExprType)) {

                        //printf("Adding adaptation to witness\n");
                        addAdaptationToWitness(s, witness, fn, visibleFn);
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    } else {
                      if (cclosure.is_equal(fn->retExprType,
                          visibleFn->retExprType)) {

                        //printf("Adding adaptation to witness\n");
                        addAdaptationToWitness(s, witness, fn, visibleFn);
                        found_match = true;
                        break;
                      } else {
                        mismatch = true;
                      }
                    }
                  }
                }
                if (!found_match) {
                  INT_FATAL("No matching functions at call");
                }
              }
            }
            //cclosure.add_implements_witness(ce->argList.head,
                //ce->argList.tail);
            if (ImplementsStmt* istmt = toImplementsStmt(s)) {
              //printf("Inside stmt");
              VarSymbol *tmp = new VarSymbol("witness");
              istmt->insertBefore(new DefExpr(tmp));
              istmt->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                 new CallExpr(witness->defaultConstructor->name)));
              addToImplementsSymbolTable(ce,ce->argList.tail,
                  ce->argList.head, tmp);
              istmt->remove();
            } else {
              //printf("Inside clause");
              VarSymbol *tmp = new VarSymbol("witness");
              ce->insertBefore(new DefExpr(tmp));
              ce->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                 new CallExpr(witness->defaultConstructor->name)));

              addToImplementsSymbolTable(ce,ce->argList.tail,
                  ce->argList.head, tmp);
              ce->remove();
            }
          } else {
            INT_FATAL("Implementing type not found\n");
          }
        }
      }
      else if (!strcmp(use->unresolved, "writeln")) {
          //printf("Inside Writeln\n");
          checkInterfaceImplementations(ce->argList.head);
      }
      else if (!ce->primitive) {
        Map<FnSymbol*, ArgSymbol*> fnInInterfaces;
        returnExpr = checkFunctionCall(ce, fnInInterfaces);
        //printf("Return Type: %d\n",returnExpr->id);
        //list_view(returnExpr);
      }
    }
  }
  else if(BlockStmt* bs = toBlockStmt(s)) {
    returnExpr = checkInterfaceImplementations(bs);
  }

  return returnExpr;
}

void earlyTypeCheck(void) {
  checkInterfaceImplementations(
      userModules.v[0]->block);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED)) {
      typeCheckFn(fn);
    }
  }

  forv_Vec(InterfaceSymbol, is, gInterfaceSymbols) {
    is->defPoint->remove();
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED)) {
      fn->where->remove();
    }
  }

  //Hackish workaround to stop early when we're early type-checking until we
  //tie into the rest of the passes
  //INT_FATAL("SUCCESS");
}
