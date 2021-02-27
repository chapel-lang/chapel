/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "resolution.h"

#include "callInfo.h"
#include "driver.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "visibleFunctions.h"
#include "view.h" // for debugging


/*********** debugging support ***********/

// uncomment the following to turn on tracking printouts
//#define PRINT_CG 1
#ifdef PRINT_CG
#define cgprint(...) fprintf(stderr, __VA_ARGS__)

#define nameBuffSize 256
static char nameBuff[nameBuffSize];
static char idBuff[32];

// show 'sym' as name[id] upon --devel, otherwise 'id'
static const char* symstring(Symbol* sym) {
  if (!sym)
    return "<no symbol provided>";
  if (developer)
    snprintf(nameBuff, nameBuffSize, "%s[%d]", sym->name, sym->id);
  else
    snprintf(nameBuff, nameBuffSize, "'%s'", sym->name);
  return nameBuff;
}

// show [id] upon --devel, empty string otherwise
static const char* idstring(const char* prefix, BaseAST* ast) {
  if (!ast)
    return "<no node provided>";
  if (developer)
    sprintf(idBuff, "%s [%d]", prefix, ast->id);
  else
    idBuff[0] = '\0'; // gcc 4.8 does not allow sprintf(idBuff, "");
  return idBuff;
}

static void cgprintAssocConstraint(IfcConstraint* icon) {
  cgprint("  assoc cons  %s(%s%s) %s\n",
          icon->ifcSymbol()->name,
          toSymExpr(icon->consActuals.head)->symbol()->name,
          icon->consActuals.length > 1 ? ",..." : "", idstring("", icon));
}

#else
#define cgprint(...)
#define cgprintAssocConstraint(...)
#endif


/*********** stand-in types ***********/

static ConstrainedType* dtGenericStandin = nullptr;
static VarSymbol*       gGenericParam    = nullptr;
static std::set<Type*>  standinInstantiations;

void createGenericStandins() {
  cgprint("\n");
  dtGenericStandin = ConstrainedType::buildType("GenericStandinType",
                                                CT_GENERIC_STANDIN);
  // We need a defPoint for verify / isAlive.
  // However we do not want it inTree() to avoid dealing with it later.
  new DefExpr(dtGenericStandin->symbol);

  gGenericParam = new VarSymbol("GenericStandinParam");
  gGenericParam->immediate = new Immediate;  // also a stand-in
  new DefExpr(gGenericParam);
}

// Remove generic types instantiated with dtGenericStandin / gGenericParam
// to avoid compiler-generated init() et al. on these.
void cleanupGenericStandins() {
  for (Type* instType: standinInstantiations) {
    cgprint("cleanup  %s[%d]\n", instType->symbol->name, instType->id);
    instType->symbol->defPoint->remove();
    if (instType->refType != nullptr) {
      cgprint("  also ref [%d]\n", instType->refType->id);
      instType->refType->symbol->defPoint->remove();
    }
  }

  // dtGenericStandin->defPoint is not inTree. However,
  // if dtGenericStandin->refType gets created, that one will be inTree.
  INT_ASSERT(! dtGenericStandin->symbol->defPoint->inTree());
  if (Type* gsRef = dtGenericStandin->refType) {
    cgprint("cleanup  dtGenericStandin->refType [%d]\n", gsRef->id);
    gsRef->symbol->defPoint->remove();
  }

  cgprint("\n");
}

static void addArgForGenericField(CallExpr* genCall, Symbol* gf) {
  // CG TODO: cater to restricted generics, ex. 'var gf: MyGenericRec;'
  //
  // Also, consider a separate "generic standin" for each generic field.
  // The same set of standins could be reused across required functions.
  // However, if we create a fresh standin for each generic field,
  // we lose the potential benefit of genericsCache.

  Symbol* instSym = nullptr;

  if (gf->hasFlag(FLAG_TYPE_VARIABLE))                 // 'type' field
    instSym = dtGenericStandin->symbol;
  else if (isVarSymbol(gf) && gf->hasFlag(FLAG_PARAM)) // 'param' field
    instSym = gGenericParam;
  else if (isVarSymbol(gf))                            // 'var' field
    instSym = dtGenericStandin->symbol;
  else {
    // how to handle this case?
    Symbol* baseType = toSymExpr(genCall->baseExpr)->symbol();
    USR_FATAL_CONT(genCall, "generic types with fields like %s.%s"
                            " are currently not supported",
                            baseType->name, gf->name);
    USR_PRINT(gf, "the field %s.%s is declared here",
                            baseType->name, gf->name);
    USR_STOP();
  }

  genCall->insertAtTail(new NamedExpr(gf->name, new SymExpr(instSym)));
}

static bool applyStandinsToGenerics(BlockStmt* holder, SymbolMap& fml2act) {
  bool gotGenerics = false;
  form_Map(SymbolMapElem, elem, fml2act) {
    Type* targetOrig = elem->value->type;
    Type* targetVal  = targetOrig->getValType();
    // CG TODO: also handle DecoratedClassType
    if (AggregateType* at = toAggregateType(targetVal)) {
      if (! at->isGeneric()) continue;
      gotGenerics = true;

      CallExpr* genCall = new CallExpr(at->symbol); // for at->generateType()
      holder->insertAtHead(genCall);
      for (Symbol* gf: at->genericFields)
        addArgForGenericField(genCall, gf);

      AggregateType* instType = at->generateType(genCall, "<internal error>");
      standinInstantiations.insert(instType);

      if (targetVal != targetOrig && instType->refType != nullptr)
        instType = instType->refType;

      elem->value = instType->symbol; // aka fml2act.put(elem->key, instType);
      genCall->remove();
    }
  }

  return gotGenerics;
}




/*********** resolveInterfaceSymbol ***********/

static void resolveIStmActuals(FnSymbol* wrapFn, ImplementsStmt* istm);

// An associated type. Nothing to do.
static void resolveISymAssocType(InterfaceSymbol* isym, TypeSymbol* at) {
  cgprint("  assoc type  %s\n", symstring(at));
  INT_ASSERT(isConstrainedTypeSymbol(at, CT_IFC_ASSOC_TYPE));
}

// This is really a constraint, even though syntactically it looks like
// an ImplementsStmt.
static void resolveISymAssocConstraint(InterfaceSymbol* isym,
                                       FnSymbol* wrapFn) {
  IstmAndSuccess iss = implementsStmtForWrapperFn(wrapFn);

  INT_ASSERT(iss.isSuccess);
  INT_ASSERT(iss.istm->implBody->body.length == 0);
  cgprintAssocConstraint(iss.istm->iConstraint);

  resolveIStmActuals(wrapFn, iss.istm);
}

// A function required by the interface.
static void resolveISymRequiredFun(InterfaceSymbol* isym, FnSymbol* fn) {
  cgprint("  required fn %s%s\n", symstring(fn),
    fn->hasFlag(FLAG_NO_FN_BODY) ? "" : "  with a default implementation");

  if (fn->hasFlag(FLAG_NO_FN_BODY))
    isym->requiredFns.put(fn, gDummyWitness);
  else
    isym->requiredFns.put(fn, fn); // with a default implementation

  if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE)
    USR_FATAL_CONT(fn, "interface functions with 'param' or 'type'"
                   " return intent are currently not allowed");

  if (fn->where != NULL)
    USR_FATAL_CONT(fn->where, "the interface function %s%s", fn->name,
          " contains a where clause, which is currently not supported");

  if (fn->retExprType != NULL && fn->retType == dtUnknown)
    resolveSpecifiedReturnType(fn);  // like in isApplicable()

  resolveFunction(fn);
}

void resolveInterfaceSymbol(InterfaceSymbol* isym) {
  if (isym->hasFlag(FLAG_RESOLVED)) return;
  isym->addFlag(FLAG_RESOLVED);

  cgprint("resolving interface declaration %s  %s\n",
          symstring(isym), debugLoc(isym));
  if (isym->id == breakOnResolveID) gdbShouldBreakHere();

  for_alist(stmt, isym->ifcBody->body) {
   if (FnSymbol* fn = toFnSymbol(toDefExpr(stmt)->sym))
     if (fn->hasFlag(FLAG_IMPLEMENTS_WRAPPER))
       resolveISymAssocConstraint(isym, fn);
     else
       resolveISymRequiredFun(isym, fn);
   else if (TypeSymbol* at = toTypeSymbol(toDefExpr(stmt)->sym))
    resolveISymAssocType(isym, at);
   else
    INT_FATAL(stmt, "should have ruled this out earlier");
  }
  cgprint("\n");
}


/*********** resolveConstrainedGenericFun ***********/

static void buildAndCheckFormals2Actuals(InterfaceSymbol* isym,
                                         IfcConstraint* icon,
                                         const char* context,
                                         SymbolMap& fml2act) {
  if (isym->numFormals() != icon->numActuals()) {
    USR_FATAL(icon, "the number of actuals in the %s (%d)"
      " does not match the number of the interface formals (%d)",
      context, icon->numActuals(), isym->numFormals());
  }

  for (Expr *curFml = isym->ifcFormals.head, *curAct = icon->consActuals.head;
       curFml != NULL;
       curFml = curFml->next, curAct = curAct->next)
   fml2act.put(toDefExpr(curFml)->sym, toSymExpr(curAct)->symbol());
}

static FnSymbol* instantiateRequiredFn(FnSymbol* required, SymbolMap& fml2act)
{
  // Can we instead use instantiateSignature(required, fml2act, NULL) ?

  SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
  FnSymbol* instantiated = required->copy(&fml2actDup);
  instantiated->addFlag(FLAG_RESOLVED); // this is white lie
  for_formals(formal, instantiated) {
    TypeSymbol* fml = formal->type->symbol;
    if (Symbol* act = fml2actDup.get(fml))  // is this needed?
      formal->type = toTypeSymbol(act)->type;
  }
  return instantiated;
}

//
// Implements visibility of interface functions within a CG function.
// Ex. worker() should be visible within fun():
//
//   interface IFC(T) { proc worker(arg:T); }
//   proc fun(x) where implements IFC(x.type) { worker(x); }
//
static void createRepsForIfcSymbols(FnSymbol* fn, InterfaceInfo* ifcInfo) {
  INT_ASSERT(ifcInfo->repsForIfcSymbols.empty()); // first time resolving 'fn'

  // we could resize this up front (does not work with gcc 4.8)
  //ifcInfo->repsForIfcSymbols.resize(ifcInfo->interfaceConstraints.length);

  // For each required function of each interface, make an instantiation of
  // its signature visible to the function body.
  for_alist(iconExpr, ifcInfo->interfaceConstraints) {
    IfcConstraint* icon = toIfcConstraint(iconExpr);
    InterfaceSymbol* isym = icon->ifcSymbol();
    resolveInterfaceSymbol(isym);

    SymbolMap reps;
    SymbolMap fml2act;
    buildAndCheckFormals2Actuals(isym, icon, "interface constraint", fml2act);

    // Create reps for isym's associated types.
    // Do this before instantiating the required functions
    // because those can reference associated types.
    for (auto& elem: isym->associatedTypes) {
      ConstrainedType* required = elem.second;
      TypeSymbol*  instantiated = ConstrainedType::buildSym(elem.first,
                                                    CT_CGFUN_ASSOC_TYPE);
      ifcInfo->constrainedTypes.insertAtTail(new DefExpr(instantiated));
      INT_ASSERT(instantiated->inTree()); //CG TODO: this assert is not needed
      reps.put(required->symbol, instantiated);
      fml2act.put(required->symbol, instantiated);
    }

    form_Map(SymbolMapElem, elem, isym->requiredFns) {
      FnSymbol* required = toFnSymbol(elem->key);
      FnSymbol* instantiated = instantiateRequiredFn(required, fml2act);
      // We may also want to make 'instantiated' visible in fn->where.
      fn->body->insertAtHead(new DefExpr(instantiated));
      reps.put(required, instantiated);
    }

    ifcInfo->repsForIfcSymbols.push_back(reps);
  }
}

// If 'fn' is a CG function and has not been resolved yet, resolve it.
void resolveConstrainedGenericFun(FnSymbol* fn) {
  if (fn->isResolved()) return;
  InterfaceInfo* ifcInfo = fn->interfaceInfo;
  if (ifcInfo == NULL) return;  // not a CG

  cgprint("resolving CG function early %s  %s\n", symstring(fn), debugLoc(fn));
  if (fn->id == breakOnResolveID) gdbShouldBreakHere();

  createRepsForIfcSymbols(fn, ifcInfo);

  // Seems like we do not need fn->setGeneric(false) for resolveFunction()
  // NB we want 'fn' to be generic later, when checking isApplicable()
  resolveSignature(fn);
  resolveFunction(fn);

  for_alist(expr, ifcInfo->constrainedTypes)
    if (Symbol* s = toDefExpr(expr)->sym) {
      if (isConstrainedTypeSymbol(s, CT_CGFUN_FORMAL))
        s->addFlag(FLAG_GENERIC);
      else
        // keep these "concrete"
        INT_ASSERT(isConstrainedTypeSymbol(s, CT_CGFUN_ASSOC_TYPE));
    }

  // Remove the reps created in createRepsForIfcSymbols(),
  // which were needed during resolveFunction(fn).
  // This way they do not show in instantiations of 'fn'.
  // No need to clean 'fn' -- it will be pruned as it is considered generic.
  for (SymbolMap& reps: ifcInfo->repsForIfcSymbols) {
    form_Map(SymbolMapElem, elem, reps) {
      if (FnSymbol* instantiated = toFnSymbol(elem->value)) {
        INT_ASSERT(instantiated->defPoint->parentExpr == fn->body);
        instantiated->defPoint->remove();
      }
    }
  }
  cgprint("\n");
}


/*********** resolveImplementsStmt ***********/

static void resolveIStmActuals(FnSymbol* wrapFn, ImplementsStmt* istm) {
  // ideally, removing and re-inserting 'istm' would not be needed
  Expr* anchor = istm->next;
  istm->remove();
  resolveFunction(wrapFn);  // resolve the actuals of istm
  anchor->insertBefore(istm);
}

static void cleanupHolder(BlockStmt* holder) {
  do { holder->body.tail->remove(); } while (holder->body.tail != NULL);
}

static void handleGenericImplementation(FnSymbol*& target,
                                        const char* indent) {
  // The required function is implemented with a generic, of which 'target'
  // is an instantiation. Store the original generic in istm->witnesses
  // and resolve it when checking whether a constraint is satisfied.
  target = target->instantiatedFrom; // CG TODO: does this cover all cases?
  INT_ASSERT(target != nullptr);
  cgprint("%s   generic -> %s  %s\n", indent,
            symstring(target), debugLoc(target));
}

static bool hasStandinArgs(CallExpr* call) {
  for_actuals(actual, call)
    if (SymExpr* ase = toSymExpr(actual))
      if (standinInstantiations.count(ase->symbol()->type))
        return true;
  return false;
}

// resolveAssociatedTypes() and helpers

// Computes and stores the associated type for this implementations
// that corresponds to the associated type 'ifcAT' in the interface, ex.
//
//   interface IFC(Self) { type AssocType; }
//   record R { proc AssocType type return int; }
//   R implements IFC;
//   ===> AssocType -> int
//
// Returns whether this was successful.
static bool resolveOneAssocType(InterfaceSymbol* isym,  ImplementsStmt*   istm,
                                SymbolMap&    fml2act,  BlockStmt*      holder,
                                const char*    indent,  ConstrainedType* ifcAT,
                                bool      gotGenerics,  bool    reportErrors) {
  INT_ASSERT(holder->body.empty());
  INT_ASSERT(ifcAT->ctUse == CT_IFC_ASSOC_TYPE);
  cgprint("%s  assoc type  %s\n", indent, symstring(ifcAT->symbol));

  // To find the corresponding associated type, create a call and resolve it.
  // It is a method call, the receiver's type is the first actual of istm.
  Symbol* ifcFormal = toDefExpr(isym->ifcFormals.head)->sym;
  Type* recvType = fml2act.get(ifcFormal)->typeInfo();

  VarSymbol*  recv = newTemp("atype_tmp", recvType);
  CallExpr*   call = new CallExpr(ifcAT->symbol->name, gMethodToken, recv);

  holder->insertAtTail(call);
  FnSymbol* target = tryResolveCall(call);
  Type* implAT = nullptr;
  if (target != nullptr) {
    if (target->retTag == RET_TYPE) {
      bool standinArgs = gotGenerics && hasStandinArgs(call);
      if (standinArgs) {
        // Only simple cases will resolve with standin args.
        FnSymbol* resolvedTarget = tryResolveFunction(target);
        if (resolvedTarget == nullptr) {
          USR_FATAL_CONT(istm, "when checking this implements statement");
          USR_PRINT(istm, "current limitations for generic implements-"
                    "statements may have cause the following error");
          USR_PRINT(target, "could not resolve this implementation"
                    " of the associated type %s", ifcAT->symbol->name);
          USR_PRINT(ifcAT->symbol, "the associated type %s in the interface %s"
                    " is declared here", ifcAT->symbol->name, isym->name);
          USR_STOP();
        } else
          INT_ASSERT(resolvedTarget == target);
        
        implAT = target->retType;
        cgprint("%s   generic -> %s\n", indent, symstring(implAT->symbol));

      } else {
        // A concrete implementing type given by 'target'.
        resolveFunction(target); // aborts if there are errors
        implAT = target->retType;
        cgprint("%s           -> %s  %s\n", indent,
                symstring(implAT->symbol), implAT->symbol->getModule()
                ->modTag == MOD_INTERNAL ? "" : debugLoc(implAT));
      }
    } else {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "when checking this implements statement");
        USR_PRINT(target, "this implementation of the associated type %s"
                  " is not or does not provide a type", ifcAT->symbol->name);
        USR_PRINT(ifcAT->symbol, "the associated type %s in the interface %s"
                  " is declared here", ifcAT->symbol->name, isym->name);
      }
    }
  } else {
    if (reportErrors) {
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(istm, "the associated type %s is not implemented",
                ifcAT->symbol->name);
      USR_PRINT(ifcAT->symbol, "the associated type %s in the interface %s"
                " is declared here", ifcAT->symbol->name, isym->name);
    }
  }

  cleanupHolder(holder);
  if (implAT != nullptr) {
    fml2act.put(ifcAT->symbol, implAT->symbol);
    istm->witnesses.put(ifcAT->symbol, implAT->symbol);
    if (implAT->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
      USR_FATAL_CONT(istm, "the associated type %s is instantiated with a"
                     " runtime type '%s', which is currently not implemented",
                     ifcAT->symbol->name, toString(implAT));
  }

  return implAT != nullptr;
}

static bool resolveAssociatedTypes(InterfaceSymbol* isym, ImplementsStmt* istm,
                                   SymbolMap&    fml2act, BlockStmt*    holder,
                                   bool           nested, const char*   indent,
                                   bool      gotGenerics, bool  reportErrors) {
  bool atSuccess = true;

  for (auto& elem: isym->associatedTypes)
    atSuccess &= resolveOneAssocType(isym, istm, fml2act, holder, indent,
                                     elem.second, gotGenerics, reportErrors);

  return atSuccess;
}

// checkAssocConstraints()

static bool checkAssocConstraints(InterfaceSymbol* isym,  ImplementsStmt* istm,
                                  SymbolMap&    fml2act,  BlockStmt*    holder,
                                  //bool nested, const char* indent,
                                  CallExpr*    callsite,
                                  bool      gotGenerics,  bool  reportErrors) {
  bool acSuccess = true;

  for (IfcConstraint* assocCons: isym->associatedConstraints) {
    cgprintAssocConstraint(assocCons);
    IfcConstraint* instantiated = toIfcConstraint(assocCons->copy(&fml2act));
    holder->insertAtTail(instantiated);
    ImplementsStmt* resolved = constraintIsSatisfiedAtCallSite(callsite,
                                                   instantiated, fml2act);
    //CG TODO: incorporate resolved->witnesses
    cleanupHolder(holder);

    if (resolved != NULL) {
      auto insertVal = std::make_pair(assocCons, resolved);
      auto insertResult = istm->aconsWitnesses.insert(insertVal);
      INT_ASSERT(insertResult.second); // no prior mapping for 'assocCon'

    } else {
      if (! reportErrors) return false;
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(assocCons, "this associated constraint is not satisfied");
      acSuccess = false;
    }
  }

  return acSuccess;
}

// resolveRequiredFns() and helpers

static bool checkNonemptyHolder(InterfaceSymbol* isym,  ImplementsStmt* istm,
                                BlockStmt*     holder,  FnSymbol*     target,
                                FnSymbol*       reqFn,  bool  reportErrors) {
  if (holder->body.tail != NULL) {
    if (reportErrors) {
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(target, "this implementation of the required function %s"
        " requires implicit conversion(s), which is currently disallowed",
        reqFn->name);
      USR_PRINT(reqFn, "the required function %s in the interface %s"
                       " is declared here", reqFn->name, isym->name);
    }
    cleanupHolder(holder);
    return false;
  }
  return true;
}

static bool checkReturnType(InterfaceSymbol* isym,  ImplementsStmt* istm,
                            FnSymbol*      target,  SymbolMap&   fml2act,
                            FnSymbol*       reqFn,  bool  reportErrors) {
  Type* implType = target->retType->getValType();
  Type* reqType = reqFn->retType->getValType();
  if (Symbol* actualType = fml2act.get(reqType->symbol))
    reqType = actualType->type->getValType();

  if (reqType != implType) {
    if (reportErrors) {
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(target, "this implementation of the required function %s",
                        reqFn->name);
      if (implType == dtUnknown)
        USR_PRINT(target, "needs to specify its return type");
      else {
        USR_PRINT(target, "has return type '%s'", toString(implType));
        USR_PRINT(reqFn, "which does not match the expected return type '%s'",
                          toString(reqType));
      }
      USR_PRINT(reqFn, "the required function %s in the interface %s"
                       " is declared here", reqFn->name, isym->name);
    }
    return false;
  }
  return true;
}

static bool checkReturnIntent(InterfaceSymbol* isym,  ImplementsStmt* istm,
                              FnSymbol*      target,  FnSymbol*      reqFn,
                              bool   reportErrors) {
  if (target->retTag != reqFn->retTag) {
    if (reportErrors) {
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(target, "this implementation of the required function %s",
                         reqFn->name);
      USR_PRINT(target, "has return intent '%s'",
                        retTagDescrString(target->retTag));
      USR_PRINT(reqFn, "which does not match the required intent '%s'",
                        retTagDescrString(reqFn->retTag));
      USR_PRINT(reqFn, "the required function %s in the interface %s"
                       " is declared here", reqFn->name, isym->name);
    }
    return false;
  }
  return true;
}

// Returns whether an implementation of 'reqFn has been established
// successfully. If so, store it in 'implRef'.
static bool resolveOneRequiredFn(InterfaceSymbol* isym,  ImplementsStmt*  istm,
                                 SymbolMap&    fml2act,  bool      gotGenerics,
                                 std::vector<FnSymbol*>  &instantiatedDefaults,
                                 BlockStmt*     holder,
                                 const char*    indent,  bool     reportErrors,
                                 FnSymbol*       reqFn,  Symbol*      &implRef)
{
  INT_ASSERT(holder->body.empty());
  cgprint("%s  required fn %s\n", indent, symstring(reqFn));

  // Create a call to the required function inside 'holder'.
  CallExpr* call = new CallExpr(reqFn->name);
  for_formals(formal, reqFn)
    call->insertAtTail(formal->copy(&fml2act));
  bool standinArgs = gotGenerics && hasStandinArgs(call);

  // Resolve the call and see if we got an implementation.
  holder->insertAtTail(call);
  callStack.add(call);
  FnSymbol* target = tryResolveCall(call);

  if (target != NULL) {
    cgprint("%s           -> %s  %s\n", indent,
            symstring(target), debugLoc(target));

    INT_ASSERT(! target->isGeneric());
    bool genericTarget = target->instantiatedFrom ? standinArgs : false;

    if (genericTarget) {
      // Need the return type for checkReturnType() below.
      // Will resolve the body when instantiating this istm.
      if (target->retExprType != NULL && target->retType == dtUnknown)
        resolveSpecifiedReturnType(target);  // like in isApplicable()
    } else
      resolveFunction(target); // aborts if there are errors

    call->remove(); // need 'call' inTree if resolveFunction() has errors

    if (!( checkNonemptyHolder(isym, istm, holder, target, reqFn, reportErrors)
           & checkReturnType(isym, istm, target, fml2act, reqFn, reportErrors)
           & checkReturnIntent(isym, istm, target, reqFn, reportErrors) ))
      target = NULL;  // if one or more of the above checks fail
    else if (genericTarget)
      handleGenericImplementation(target, indent);

  } else {
    // the call did not resolve
    call->remove();
    if (implRef == gDummyWitness) {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "when checking this implements statement");
        USR_PRINT(istm, "the required function %s is not implemented",
                  reqFn->name);
        USR_PRINT(reqFn, "the required function %s in the interface %s"
                  " is declared here", reqFn->name, isym->name);
      }
    } else if (standinArgs) {
      // todo: fall back on the default implementation in the generic case
      if (reportErrors) {
        USR_FATAL_CONT(istm, "when checking this implements statement");
        USR_PRINT(istm, "default implementations are currently not supported in generic implements statements");
        USR_PRINT(reqFn, "the required function %s in the interface %s"
                  " is declared here", reqFn->name, isym->name);
      }
    } else {
      // fall back on the default implementation
      SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
      target = toFnSymbol(implRef->copy(&fml2actDup));
      holder->insertBefore(new DefExpr(target));
      instantiatedDefaults.push_back(target);
      INT_ASSERT(target->isResolved()); // the dflt impl was resolved with isym
      cgprint("%s      dflt -> %s  %s\n", indent,
              symstring(target), debugLoc(target));
    }
  }
  CallExpr* popped = callStack.pop();
  INT_ASSERT(popped == call);

  implRef = target; // aka istm->witnesses.put(reqFn, target)
  return target != NULL;
}

static bool resolveRequiredFns(InterfaceSymbol* isym,  ImplementsStmt* istm,
                               SymbolMap&    fml2act,  BlockStmt*    holder,
                               const char*    indent,
                               bool      gotGenerics,  bool  reportErrors) {
  bool rfSuccess = true;
  std::vector<FnSymbol*> instantiatedDefaults;

  form_Map(SymbolMapElem, wit, istm->witnesses) {
    if (FnSymbol* reqFn = toFnSymbol(wit->key))
      rfSuccess &= resolveOneRequiredFn(isym, istm, fml2act, gotGenerics,
                                        instantiatedDefaults, holder, indent,
                                        reportErrors, reqFn, wit->value);
    else
      INT_ASSERT(isConstrainedTypeSymbol(wit->key, CT_IFC_ASSOC_TYPE));
  }

  if (rfSuccess) {
    for(FnSymbol* iDflt: instantiatedDefaults)
      // Update references to now-instantiated required functions
      // within default implementations.
      update_symbols(iDflt, &(istm->witnesses));
  }

  return rfSuccess;
}


//
// Ensures this ImplementsStmt indeed implements the interface, ex.
// * determine each associated type
// * verify the associated constraints are satisfied
// * determine each witness, default or otherwise
// * resolve each witness
// Returns true if so, false otherwise.
//
static bool resolveImplementsStmt(FnSymbol* wrapFn, ImplementsStmt* istm,
                                  bool nested, //used only for debugging output
                                  bool reportErrors) {
  if (istm->id == breakOnResolveID) gdbShouldBreakHere();

  IfcConstraint* icon = istm->iConstraint;
  InterfaceSymbol* isym = icon->ifcSymbol();

  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  if (wrapFn->isResolved()) {
    // We can get here because we have already resolved 'istm'.
    // If so, no need to resolve it again.
    //
    // We can also get here while resolving 'istm' recursively,
    // because wrapFn gets marked resolved before checkAssocConstraints().
    // If so, we return successful implementation, i.e. we break recursion
    // by assuming success.

    IstmAndSuccess iss = implementsStmtForWrapperFn(wrapFn);
    // if isSuccess can legitimately be false, return it and remove the assert
    INT_ASSERT(iss.isSuccess);
    return true;
  }

  const char* indent = nested ? "  " : ""; //used only for debugging output
  if (nested) cgprint("    resolving it...\n");
  else cgprint("resolving implements statement%s for ifc %s  %s\n",
               idstring("", istm), symstring(isym), debugLoc(istm));

  // This CallExpr will represent the checking of 'istm'.
  CallExpr* callsite = new CallExpr(wrapFn);
  wrapFn->defPoint->insertBefore(callsite);
  callStack.add(callsite);

  resolveIStmActuals(wrapFn, istm);  // marks wrapFn with FLAG_RESOLVED

  SET_LINENO(istm->implBody);
  BlockStmt* holder = new BlockStmt();
  istm->implBody->insertAtTail(holder);

  INT_ASSERT(istm->witnesses.n == 0);      // we have not filled it yet
  istm->witnesses.copy(isym->requiredFns); // start with the defaults impls

  SymbolMap fml2act; // isym formal -> istm actual
  buildAndCheckFormals2Actuals(isym, icon, "implements statement", fml2act);
  // need to stash away the original fml2act?
  bool gotGenerics = applyStandinsToGenerics(holder, fml2act);

  // check each category of istm contents

  bool success =
     resolveAssociatedTypes(isym, istm, fml2act, holder, nested, indent,
                                gotGenerics, reportErrors)
     && checkAssocConstraints(isym, istm, fml2act, holder, callsite,
                                gotGenerics, reportErrors)
     && resolveRequiredFns(isym, istm, fml2act, holder, indent,
                                gotGenerics, reportErrors);

  cgprint(nested ? "    ...done\n" : "\n");
  holder->remove();
  INT_ASSERT(wrapFn->hasFlag(FLAG_RESOLVED));
  CallExpr* popped = callStack.pop();
  INT_ASSERT(popped == callsite);
  callsite->remove();  

  if (!success) {
    if (reportErrors) USR_STOP();
    markImplStmtWrapFnAsFailure(wrapFn);
  }

  return success;
}

void resolveImplementsStmt(ImplementsStmt* istm) {
  FnSymbol* wrapFn = wrapperFnForImplementsStmt(istm);
  resolveImplementsStmt(wrapFn, istm, false, true);
}


/*********** constraintIsSatisfiedAtCallSite ***********/

// Build a call to the wrapper function.
static CallExpr* buildCall2wf(InterfaceSymbol* isym, IfcConstraint* constraint,
                              SymbolMap& substitutions) {
  CallExpr* call2wf = new CallExpr(implementsStmtWrapperName(isym));
  for_alist(act, constraint->consActuals)
    call2wf->insertAtTail(act->copy(&substitutions));
  return call2wf;
}

static void gatherVisibleWrapperFns(CallExpr* callsite,
                                    CallExpr* call2wf,
                                    Vec<FnSymbol*>& visibleFns) {
  callsite->insertBefore(call2wf);

  CallInfo info;
  bool infoOK = info.isWellFormed(call2wf);
  INT_ASSERT(infoOK);

  findVisibleFunctionsAllPOIs(info, visibleFns);

  call2wf->remove();
}

// Follows ImplementsStmt::build().
// Clears the arglist of 'call2wf'.
static ImplementsStmt* buildInferredImplStmt(InterfaceSymbol* isym,
                                             CallExpr* call2wf) {
  IfcConstraint* icon = new IfcConstraint(new SymExpr(isym));
  for_alist(actual, call2wf->argList)
    icon->consActuals.insertAtTail(actual->remove());

 return new ImplementsStmt(icon, new BlockStmt());
}

static bool isGenericMatch(Type* consT, Type* implT) {
  if (AggregateType* consAT = toAggregateType(consT))
    if (AggregateType* implAT = toAggregateType(implT))
      do {
        consAT = consAT->instantiatedFrom;
        if (consAT == implAT)
          return true;
      }
      while (consAT != NULL);

  return false;
}

class MatchResult { public:
  ImplementsStmt* istm;
  bool            isSuccess;
  bool            isConcrete; 
  MatchResult(ImplementsStmt* is, bool ss, bool con):
    istm(is), isSuccess(ss), isConcrete(con) { }
};

//
// Returns a MatchResult tuple with the following components:
// * istm: the istm within wrapFn, if it matches the constraint
//   represented by 'call2wf', NULL otherwise
// If istm is non-NULL:
// * isSuccess: true, unless wrapFn is marked as failure
// * isConcrete: true when the arguments of the constraint
//   and the istm pare pair-wise equal; otherwise a generic match
//   can occur.
//
static MatchResult matchingImplStm(FnSymbol* wrapFn, CallExpr* call2wf) {
  
  IstmAndSuccess iss = implementsStmtForWrapperFn(wrapFn);
  if (iss.isSuccess) // do we need this restriction?
    resolveImplementsStmt(wrapFn, iss.istm, false, true);

  bool isConcrete = true;

  for (Expr *consArg = call2wf->argList.head,
            *implArg = iss.istm->iConstraint->consActuals.head;
       consArg != NULL;
       consArg = consArg->next, implArg = implArg->next)
  {
    Type* consT = toSymExpr(consArg)->symbol()->type;
    Type* implT = toSymExpr(implArg)->symbol()->type;

    if (consT == implT)
      continue; // so far it is a match

    if (isGenericMatch(consT, implT)) {
      isConcrete = false;
      continue;
    }

    // the two types are different, so istm does not match at all
    return MatchResult(nullptr, false, false);
  }

  return MatchResult(iss.istm, iss.isSuccess, isConcrete);
}

class FirstPick { public:
  ImplementsStmt* conSuccess; // concrete
  ImplementsStmt* genSuccess; // generic
  FnSymbol*       failure;    // wrapFn
  FirstPick(ImplementsStmt* con, ImplementsStmt* gen, FnSymbol* wfn):
    conSuccess(con), genSuccess(gen), failure(wfn) { }
};

// Note:
//  * A generic istm is always successful:
//   - It is never created implicitly, so it is always user-provided.
//   - It is resolved before being used; any errors would abort compilation.
//  * A failure wrapFn / istm is always concrete:
//   - It cannot be written by the user, so it is always created implicitly.
//   - We create an istm implicitly only when checking a concrete constraint.

static FirstPick pickMatchingImplementsStmts(Vec<FnSymbol*> &visibleFns,
                                             CallExpr*       call2wf) {
  ImplementsStmt* firstGenSuccess = nullptr;
  FnSymbol*       firstFailure    = nullptr;

  forv_Vec(FnSymbol, wrapFn, visibleFns) {
    MatchResult match = matchingImplStm(wrapFn, call2wf);
    if (match.istm != nullptr) {
      // got a match, what kind?
      if (match.isSuccess) {
        if (match.isConcrete)
          // got the first successfull concrete istm
          return FirstPick(match.istm, nullptr, nullptr);
        else
          if (firstGenSuccess == nullptr)
            firstGenSuccess = match.istm;
      } else {
        if (firstFailure == nullptr)
          firstFailure = wrapFn;
      }
    }
  }

  return FirstPick(nullptr, firstGenSuccess, firstFailure);
}

//
// We found a generic istm, which we have checked/resolved successfully
// (as much as that is doable for a generic istm).
// Now we need to apply it to satisfy a concrete constraint.
//
// Implementation-wise, instead of properly instantiating this generic istm,
// which would require more implementation complexity, we pretend that
// the user wrote a corresponding concrete istm, and resolve that.
//
static ImplementsStmt* useGenericImplementsStmt(CallExpr*        callsite,
                                                InterfaceSymbol* isym,
                                                CallExpr*        call2wf,
                                                ImplementsStmt*  genIstm) {
  // follow parts of checkInferredImplStmt()
  SET_LINENO(genIstm);
  ImplementsStmt* conIstm = buildInferredImplStmt(isym, call2wf);
  genIstm->insertAfter(conIstm);
  cgprint("instantiated generic implements statement%s",
    idstring("", genIstm)); cgprint("%s\n", idstring("  ->", conIstm));

  FnSymbol* wrapFn = wrapOneImplementsStatement(conIstm);
  bool success = resolveImplementsStmt(wrapFn, conIstm, false, true);

  return success ? conIstm : nullptr;
}

#ifdef PRINT_CG
static void cgprintCheckedConstraint(InterfaceSymbol* isym,
                                     IfcConstraint*   constraint,
                                     CallExpr*        callsite,
                                     ImplementsStmt*  bestIstm,
                                     bool             maybeUserDefined) {
  cgprint("checked constraint for %s  %s\n",
          symstring(isym), debugLoc(constraint));

  if (UnresolvedSymExpr* use = toUnresolvedSymExpr(callsite->baseExpr))
    cgprint("  for call [%d]  to %s  at %s\n", callsite->id,
            use->unresolved, debugLoc(callsite));
  else {
    FnSymbol* wrapFn = toFnSymbol(toSymExpr(callsite->baseExpr)->symbol());
    if (wrapFn != NULL) {
      // This is the only use case.
      INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
      cgprint("  when checking the constraint at %s\n", debugLoc(wrapFn));
    }
  }

  if (bestIstm)
    cgprint("  satisfied with %simplements stmt%s  %s\n\n",
            maybeUserDefined ? "" : "inferred ",
            idstring("", bestIstm), debugLoc(bestIstm));
  else
    cgprint("  not satisfied\n\n");
}
#else
#define cgprintCheckedConstraint(...)
#endif

// Traverse lexical scopes starting with callsite's.
// If the scope of 'wrapFn' is found first, return NULL.
// Otherwise return the parent of 'callsite' within the earliest
// enclosing scope that contains visible functions or a POI.
static Expr* closestInterestingScopeAnchor(CallExpr* callsite,
                                           FnSymbol* wrapFn) {
  Expr*     currAnchor = callsite->getStmtExpr();
  BlockStmt* wrapBlock = NULL;
  if (wrapFn != NULL) {
    wrapBlock = toBlockStmt(wrapFn->defPoint->parentExpr);
    INT_ASSERT(wrapBlock != NULL);
  }

  while (true) {
    BlockStmt* currBlock = toBlockStmt(currAnchor->parentExpr);

    if (wrapBlock != NULL && wrapBlock == currBlock)
      return NULL;

    if (currBlock == NULL) {
      INT_ASSERT(currAnchor->parentExpr == NULL);

      // Check for a POI, like in getInstantiationPoint().
      Symbol* parentSym = currAnchor->parentSymbol;
      if (FnSymbol* fn = toFnSymbol(parentSym)) {
        if (BlockStmt* instantiationPt = fn->instantiationPoint())
          return instantiationPt;
      } else if (TypeSymbol* ts = toTypeSymbol(parentSym)) {
        if (BlockStmt* instantiationPt = ts->instantiationPoint)
          return instantiationPt;
      }

      currAnchor = parentSym->defPoint;
      continue;
    }

    // This guarantees termination of this loop:
    // once we get to module scope, it has at least the init function.
    if (scopeDefinesVisibleFunctions(currBlock))
      return currAnchor;

    currAnchor = currAnchor->parentExpr;
    continue;
  }
}

// 'failureWrapFn' is the wrapFn for a negative outcome of an earlier attempt
// to infer this constraint, if found, otherwise NULL.
static ImplementsStmt* checkInferredImplStmt(CallExpr* callsite,
                                             InterfaceSymbol* isym,
                                             CallExpr* call2wf,
                                             FnSymbol* failureWrapFn) {
  cgprint("checking for inferred implements statement for ifc %s  %s\n",
          symstring(isym), debugLoc(callsite));

  if (isym->requiredFns.n == 0)
    // Do not infer for an empty interface.
    return NULL;

  Expr* anchor = closestInterestingScopeAnchor(callsite, failureWrapFn);

  if (anchor == NULL)
    // Closer scopes do not define any functions, so the inference outcome,
    // if we tried to infer, would be the same, i.e. negative.
    return NULL;

  // Try to infer as if we are in anchor's scope.
  // Retain the outcome - positive or negative - next to 'anchor'.

  SET_LINENO(callsite);
  ImplementsStmt* istm = buildInferredImplStmt(isym, call2wf);
  anchor->insertBefore(istm);
  FnSymbol* wrapFn = wrapOneImplementsStatement(istm);
  bool success = resolveImplementsStmt(wrapFn, istm, false, false);

  return success ? istm : NULL;
}

/*
constraintIsSatisfiedAtCallSite() checks if 'constraint' is satisfied.
Return the corresponding ImplementsStmt if yes, NULL if no.

Semantics:
- When there is/are matching visible ImplementsStmt(s), use the closest one.
- Otherwise, infer it:
 * Create an ImplementsStmt and check if it is correct,
   see resolveImplementsStmt().
 * Exception: do not infer for an empty interface (has no required functions).

Efficiency:
- To reduce the number of ImplementsStmts created to infer the same constraint
  multiple times:
 * Store the created ImplementsStmt in the AST
   just like a user-written one, i.e. in a wrapper function.
   This way it will be found and reused in subsequent lookups.
 * Do this even if we found that it is not correct.
   Mark the wrapper function with PRIM_ERROR to indicate this.
   This caches the negative outcome of an attempt to infer.
 * Place the created ImplementsStmt as far out as possible
   so it can be reused in more cases.
   Ex. place it in the innermost scope that defines any functions or has POI.
*/
ImplementsStmt* constraintIsSatisfiedAtCallSite(CallExpr*      callsite,
                                                IfcConstraint* constraint,
                                                SymbolMap&     substitutions) {
  if (callsite->id == breakOnResolveID  ||
      constraint->id == breakOnResolveID ) gdbShouldBreakHere();

  InterfaceSymbol* isym = constraint->ifcSymbol();

  // 'call2wf' represents 'constraint' throughout this function
  // because it contains proiperly instantiated arguments.
  CallExpr* call2wf = buildCall2wf(isym, constraint, substitutions);

  // An earlier version resolved 'call2wf' completely.
  // That required figuring out whether the wrapper functions
  // were concrete or generic, also needed to avoid instantiating them.
  // An even earlier version checked the wrapper function's where-clause.
  // Both approaches increased code complexity. Instead, just gather
  // all visible functions and manually check their applicability.
  //
  // If semantically allowed, see #16731, we could optimize by breaking out
  // from gatherVisibleWrapperFns once a successful match is found.

  Vec<FnSymbol*> visibleFns;
  gatherVisibleWrapperFns(callsite, call2wf, visibleFns);

  FirstPick pick = pickMatchingImplementsStmts(visibleFns, call2wf);
  ImplementsStmt* bestIstm = NULL;

  if (ImplementsStmt* conSuccess = pick.conSuccess)
    // yippee, found a satisfying concrete istm
    bestIstm = conSuccess;
  
  else if (ImplementsStmt* genSuccess = pick.genSuccess)
    // instantiate a generic istm
    bestIstm = useGenericImplementsStmt(callsite, isym, call2wf, genSuccess);

  else if (fInferImplementsStmts)
    bestIstm = checkInferredImplStmt(callsite, isym, call2wf, pick.failure);

  call2wf = nullptr; // call2wf may now be useless

  cgprintCheckedConstraint(isym, constraint, callsite, bestIstm,
        pick.conSuccess != nullptr || pick.genSuccess != nullptr);

  // It is resolved, if non-null.
  return bestIstm;
}


/*********** other ***********/

//
// This populates 'substitutions' with the mapping from each associated type
// in 'fn' for indx-th constraint to the A.T.'s instantiation.
// 
void copyIfcRepsToSubstitutions(FnSymbol* fn, int indx, ImplementsStmt* istm,
                                SymbolMap& substitutions) {
  form_Map(SymbolMapElem, elem,
           fn->interfaceInfo->repsForIfcSymbols[indx])
  {
    Symbol* required = elem->key;
    Symbol* usedInFn = elem->value;
    Symbol* implem   = istm->witnesses.get(required);
    if (isFnSymbol(required)) {
      INT_ASSERT(isFnSymbol(usedInFn));
      INT_ASSERT(isFnSymbol(implem));
    } else {
      INT_ASSERT(isConstrainedTypeSymbol(required, CT_IFC_ASSOC_TYPE));
      INT_ASSERT(isConstrainedTypeSymbol(usedInFn, CT_CGFUN_ASSOC_TYPE));
    }
    substitutions.put(usedInFn, implem);
  }

  for_formals(formal, fn)
    // CG TODO: also handle a nested AT, ex. [1..3] AT
    if (Symbol* sym = substitutions.get(formal->type->symbol))
      substitutions.put(formal, sym);
}

//
// The substitutions from repsForIfcSymbols to their implementations,
// which we worked so hard to compute, are dropped on the floor
// by instantiateSignature() -> partialCopy().
// Recover them so they take effect later upon finalizeCopy().
//
void adjustForCGinstantiation(FnSymbol* fn, SymbolMap& substitutions) {
  PartialCopyData* pci = getPartialCopyData(fn);
  if (! pci)
    return; // already finalizeCopy-ed
  if (! pci->partialCopySource->isConstrainedGeneric())
    return; // works fine as-is

  // This is how we copy a CG function.
  INT_ASSERT(fn->interfaceInfo == NULL);
  INT_ASSERT(! fn->hasFlag(FLAG_GENERIC));

  SymbolMap& pciMap = pci->partialCopyMap;

  form_Map(SymbolMapElem, elem, substitutions) {
    if (isArgSymbol(elem->key)) {
      // The formals are already mapped.
      // NB 'substitutions' map formals to their types.
      Symbol* alreadyMapped = pciMap.get(elem->key);
      INT_ASSERT(alreadyMapped->type->symbol == elem->value);
    } else {
      pciMap.put(elem->key, elem->value);
    }
  }
}

void resolveConstrainedGenericSymbol(Symbol* sym, bool mustBeCG) {
  if (FnSymbol* fn = toFnSymbol(sym)) {
    if (fn->hasFlag(FLAG_IMPLEMENTS_WRAPPER)) {
      IstmAndSuccess iss = implementsStmtForWrapperFn(fn);
      INT_ASSERT(iss.isSuccess); // remove this if there is a legitimate case
      resolveImplementsStmt(fn, iss.istm, false, true);
      return;
    }
    if (fn->isConstrainedGeneric()) {
      resolveConstrainedGenericFun(fn);
      return;
    }
  } else if (InterfaceSymbol* isym = toInterfaceSymbol(sym)) {
    resolveInterfaceSymbol(isym);
    return;
  }

  // Not an expected case. Ignore unless mustBeCG.
  INT_ASSERT(!mustBeCG);
}

static void issueAssocTypeAmbiguousError(IfcConstraint*   assocCons1,
                                         IfcConstraint*   assocCons2,
                                         CallExpr*        call,
                                         const char*      atName) {
  USR_FATAL_CONT(call, "the associated type '%s' is ambiguous", atName);
  USR_PRINT(assocCons1, "it can come from interface '%s' in this constraint",
                        assocCons1->ifcSymbol()->name);
  USR_PRINT(assocCons2, "or from interface '%s' in this constraint",
                        assocCons2->ifcSymbol()->name);
  USR_STOP();
}

// Convert the call 'CT.assocType' to the corresponding entry
// from 'repsForIfcSymbols'.
Expr* resolveCallToAssociatedType(CallExpr* call, ConstrainedType* recv) {
  if (recv->ctUse != CT_CGFUN_FORMAL)
    USR_FATAL(call, "this use of a constrained type '%s'"
              " is at present not supported", recv->symbol->name);
  FnSymbol* parentFn = call->getFunction();
  INT_ASSERT(recv->symbol->defPoint->parentSymbol == parentFn);
  InterfaceInfo* ifcInfo = parentFn->interfaceInfo;
  const char* atName = toUnresolvedSymExpr(call->baseExpr)->unresolved;

  ConstrainedType* assocType = NULL;
  IfcConstraint*   assocCon  = NULL;
  int indx = -1;

  // Find the constraint(s) where 'recv' is an implementer.
  // This search below succeeds whether 'recv' is the single actual
  // to an interface constraint or one of the several actuals.
  for_alist(iconExpr, ifcInfo->interfaceConstraints) {
    IfcConstraint* icon = toIfcConstraint(iconExpr);
    indx++;
    bool found = false;
    for_alist(arg, icon->consActuals)
      if (arg->typeInfo() == recv)
        { found = true; break; }  // yes, it is an implementer
    if (!found)
      continue;

    // See if the corresponding interface has an assoc type named atName.
    InterfaceSymbol* isym = icon->ifcSymbol();
    auto assocTypeIT = isym->associatedTypes.find(atName);
    if (assocTypeIT == isym->associatedTypes.end())
      continue;

    // Ensure it is unambiguous.
    if (assocCon != NULL)
      issueAssocTypeAmbiguousError(assocCon, icon, call, atName);
    assocCon = icon;

    // Fetch the corresponding ConstrainedType from repsForIfcSymbols.
    // We could have lazy creation of these ConstrainedTypes here, i.e.
    // create one if it is not already in repsForIfcSymbols. However,
    // we will use repsForIfcSymbols in a copy() later, which does not
    // give us an option for lazy creation.
    Symbol* atRep = ifcInfo->repsForIfcSymbols[indx].get(
                               assocTypeIT->second->symbol);
    assocType = toConstrainedType(toTypeSymbol(atRep)->type);
    INT_ASSERT(assocType->ctUse == CT_CGFUN_ASSOC_TYPE);
  }

  if (assocType == NULL)
    // This was not a reference to an associated type
    return call;

  SymExpr* result = new SymExpr(assocType->symbol);
  call->replace(result);
  return result;
}
