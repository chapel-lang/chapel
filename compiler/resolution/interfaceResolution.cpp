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


static bool resolveImplementsStmt(ImplementsStmt* istm,
                                  bool nested, //used only for debugging output
                                  bool reportErrors);

static void resolveIStmActuals(FnSymbol* wrapFn, ImplementsStmt* istm);


/*********** resolveInterfaceSymbol ***********/

// An associated type. Nothing to do.
static void resolveISymAssocType(InterfaceSymbol* isym, TypeSymbol* at) {
  cgprint("  assoc type  %s\n", symstring(at));
  INT_ASSERT(isConstrainedTypeSymbol(at, CT_IFC_ASSOC_TYPE));
}

// This is really a constraint, even though syntactically it looks like
// an ImplementsStmt.
static void resolveISymAssocConstraint(InterfaceSymbol* isym,
                                       FnSymbol* wrapFn) {
  bool isSuccess;
  ImplementsStmt* istm = implementsStmtForWrapperFn(wrapFn, isSuccess);
  INT_ASSERT(isSuccess && istm->implBody->body.length == 0);
  cgprintAssocConstraint(istm->iConstraint);

  resolveIStmActuals(wrapFn, istm);
}

// A function required by the interface.
static void resolveISymRequiredFun(InterfaceSymbol* isym, FnSymbol* fn) {
  cgprint("  required fn %s%s\n", symstring(fn),
    fn->hasFlag(FLAG_NO_FN_BODY) ? "" : "  with a default implementation");

  if (fn->hasFlag(FLAG_NO_FN_BODY))
    isym->requiredFns.put(fn, gDummyWitness);
  else
    isym->requiredFns.put(fn, fn); // with a default implementation

  if (fn->where != NULL)
    USR_FATAL_CONT(fn->where, "the interface function %s%s", fn->name,
          " contains a where clause, which is currently not supported");

  resolveFunction(fn);
}

void resolveInterfaceSymbol(InterfaceSymbol* isym) {
  cgprint("resolving interface declaration %s  %s\n",
          symstring(isym), debugLoc(isym));

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
    SymbolMap reps;
    SymbolMap fml2act;
    buildAndCheckFormals2Actuals(isym, icon, "interface constraint", fml2act);

    // Create reps for isym's associated types.
    // Do this before instantiating the required functions
    // because those can reference associated types.
    for (auto& elem: isym->associatedTypes) {
      ConstrainedType* required = elem.second;
      TypeSymbol*  instantiated = ConstrainedType::build(elem.first,
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

// Computes and stores the associated type for this implementations
// that corresponds to the associated type 'ifcAT' in the interface, ex.
//
//   interface IFC(Self) { type AssocType; }
//   record R { proc AssocType type return int; }
//   R implements IFC;
//   ===> AssocType -> int
//
// Returns whether this was successful.
static bool resolveOneAssocType(InterfaceSymbol* isym, ImplementsStmt* istm,
                                SymbolMap& fml2act, BlockStmt* holder,
                                const char* indent, bool reportErrors,
                                ConstrainedType* ifcAT) {
  INT_ASSERT(holder->body.empty());
  INT_ASSERT(ifcAT->ctUse == CT_IFC_ASSOC_TYPE);
  cgprint("%s  assoc type  %s\n", indent, symstring(ifcAT->symbol));

  // To find the corresponding associated type, create a call and resolve it.
  // It is a method call, the receiver's type is the first actual of istm.
  Symbol* ifcFormal = toDefExpr(isym->ifcFormals.head)->sym;
  Type* recvType = fml2act.get(ifcFormal)->typeInfo();

  VarSymbol* recv = newTemp("atype_tmp", recvType);
  CallExpr* call = new CallExpr(ifcAT->symbol->name, gMethodToken, recv);
  holder->insertAtTail(call);
  FnSymbol* target = tryResolveCall(call);
  Type* gcfunAT = NULL;
  if (target != NULL) {
    if (target->retTag == RET_TYPE) {
      resolveFunction(target); // aborts if there are errors
      gcfunAT = target->retType;
      cgprint("%s           -> %s  %s\n", indent,
              symstring(gcfunAT->symbol), gcfunAT->symbol->getModule()
              ->modTag == MOD_INTERNAL ? "" : debugLoc(gcfunAT));
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
  if (gcfunAT != NULL) {
    fml2act.put(ifcAT->symbol, gcfunAT->symbol);
    istm->witnesses.put(ifcAT->symbol, gcfunAT->symbol);
    if (gcfunAT->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE))
      USR_FATAL_CONT(istm, "the associated type %s is instantiated with a"
                     " runtime type '%s', which is currently not implemented",
                     ifcAT->symbol->name, toString(gcfunAT));
  }

  return gcfunAT != NULL;
}

static bool resolveAssociatedTypes(InterfaceSymbol* isym, ImplementsStmt* istm,
                                   SymbolMap& fml2act, BlockStmt* holder,
                                   bool nested, const char* indent,
                                   bool reportErrors) {
  bool atSuccess = true;

  for (auto& elem: isym->associatedTypes)
    atSuccess &= resolveOneAssocType(isym, istm, fml2act, holder,
                                     indent, reportErrors, elem.second);

  return atSuccess;
}

static bool checkAssocConstraints(InterfaceSymbol* isym, ImplementsStmt* istm,
                                  SymbolMap& fml2act, BlockStmt* holder,
                                  //bool nested, const char* indent,
                                  CallExpr* callsite, bool reportErrors) {
  bool acSuccess = true;

  for (ImplementsStmt* assocIS: isym->associatedConstraints) {
    cgprintAssocConstraint(assocIS->iConstraint);
    ImplementsStmt* instantiated = toImplementsStmt(assocIS->copy(&fml2act));
    holder->insertAtTail(instantiated);
    ImplementsStmt* resolved = constraintIsSatisfiedAtCallSite(callsite,
                                       instantiated->iConstraint, fml2act);
    //CG TODO: incorporate resolved->witnesses
    cleanupHolder(holder);

    if (resolved == NULL) {
      if (! reportErrors) return false;
      USR_FATAL_CONT(istm, "when checking this implements statement");
      USR_PRINT(assocIS, "this associated constraint is not satisfied");
      acSuccess = false;
    }
  }

  return acSuccess;
}


// Returns whether a witness has been established successfully.
static bool resolveOneWitness(InterfaceSymbol* isym, ImplementsStmt* istm,
                              SymbolMap& fml2act, BlockStmt* holder,
                              const char* indent, bool reportErrors,
                              FnSymbol* reqFn, Symbol*& implRef) {
  INT_ASSERT(holder->body.empty());
  cgprint("%s  required fn %s\n", indent, symstring(reqFn));

  // Create a call to the required function inside 'holder'.
  CallExpr* call = new CallExpr(reqFn->name);
  for_formals(formal, reqFn)
    call->insertAtTail(formal->copy(&fml2act));
  holder->insertAtTail(call);

  // Resolve the call and see if we got a witness.
  FnSymbol* target = tryResolveCall(call);
  if (target != NULL) {
    cgprint("%s           -> %s  %s\n", indent,
            symstring(target), debugLoc(target));
    resolveFunction(target); // aborts if there are errors

    call->remove();
    if (holder->body.tail != NULL) {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "when checking this implements statement");
        USR_PRINT(target, "this implementation of the required function %s"
          " requires implicit conversion(s), which is currently disallowed",
          reqFn->name);
        USR_PRINT(reqFn, "the required function %s in the interface %s"
                         " is declared here", reqFn->name, isym->name);
      }
      target = NULL;
      cleanupHolder(holder);
    }
  } else {
    if (implRef == gDummyWitness) {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "when checking this implements statement");
        USR_PRINT(istm, "the required function %s is not implemented",
                  reqFn->name);
        USR_PRINT(reqFn, "the required function %s in the interface %s"
                  " is declared here", reqFn->name, isym->name);
      }
    } else {
      SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
      target = toFnSymbol(implRef->copy(&fml2actDup));
      holder->insertBefore(new DefExpr(target));
      INT_ASSERT(target->isResolved()); // the dflt impl was resolved with isym
      cgprint("%s      dflt -> %s  %s\n", indent,
              symstring(target), debugLoc(target));
    }
    call->remove();
  }

  implRef = target; // aka istm->witnesses[indx].put(reqFn, target)
  return target != NULL;
}

static bool resolveWitnesses(InterfaceSymbol* isym, ImplementsStmt* istm,
                             SymbolMap& fml2act, BlockStmt* holder,
                             const char* indent, bool reportErrors) {
  bool witSuccess = true;

  form_Map(SymbolMapElem, wit, istm->witnesses) {
    if (FnSymbol* reqFn = toFnSymbol(wit->key))
      witSuccess &= resolveOneWitness(isym, istm, fml2act, holder,
                                   indent, reportErrors, reqFn, wit->value);
    else
      INT_ASSERT(isConstrainedTypeSymbol(wit->key, CT_IFC_ASSOC_TYPE));
  }

  return witSuccess;
}


//
// Ensures this ImplementsStmt indeed implements the interface, ex.
// * determine each associated type
// * verify the associated constraints are satisfied
// * determine each witness, default or otherwise
// * resolve each witness
// Returns true if so, false otherwise.
//
static bool resolveImplementsStmt(ImplementsStmt* istm,
                                  bool nested, //used only for debugging output
                                  bool reportErrors) {
  if (istm->id == breakOnResolveID) gdbShouldBreakHere();
  IfcConstraint* icon = istm->iConstraint;
  InterfaceSymbol* isym = icon->ifcSymbol();
  FnSymbol* wrapFn = wrapperFnForImplementsStmt(istm);

  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  if (wrapFn->isResolved()) {
    // no need to resolve again
    bool isSuccess;
    implementsStmtForWrapperFn(wrapFn, isSuccess);
    // if isSuccess can legitimately be false, return it and remove the assert
    INT_ASSERT(isSuccess);
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

  resolveIStmActuals(wrapFn, istm);

  SET_LINENO(istm->implBody);
  BlockStmt* holder = new BlockStmt();
  istm->implBody->insertAtTail(holder);

  INT_ASSERT(istm->witnesses.n == 0);      // we have not filled it yet
  istm->witnesses.copy(isym->requiredFns); // start with the defaults impls

  SymbolMap fml2act; // isym formal -> istm actual
  buildAndCheckFormals2Actuals(isym, icon, "implements statement", fml2act);

  // check each category of istm contents
  bool rSuccess = true;

  if (rSuccess)
    rSuccess = resolveAssociatedTypes(isym, istm, fml2act, holder,
                                      nested, indent, reportErrors);
  if (rSuccess)
    rSuccess = checkAssocConstraints(isym, istm, fml2act, holder,
                                     callsite, reportErrors);

  if (rSuccess)
    rSuccess = resolveWitnesses(isym, istm, fml2act, holder,
                                indent, reportErrors);

  cgprint(nested ? "    ...done\n" : "\n");
  holder->remove();
  wrapFn->addFlag(FLAG_RESOLVED);
  CallExpr* popped = callStack.pop();
  INT_ASSERT(popped == callsite);
  callsite->remove();  

  if (!rSuccess) {
    if (reportErrors) USR_STOP();
    markImplStmtWrapFnAsFailure(wrapFn);
  }

  return rSuccess;
}

void resolveImplementsStmt(ImplementsStmt* istm) {
  resolveImplementsStmt(istm, false, true);
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

// Returns the istm within 'wrapFn' if it matches the constraint
// represented by 'call2wf', i.e. if their arguments are pair-wise equal.
// Otherwise returns NULL.
// isSuccess=false when 'wrapFn' is a result of a failed attempt to infer.
static ImplementsStmt* matchingImplStm(FnSymbol* wrapFn,
                                       CallExpr* call2wf,
                                       bool& isSuccess) {
  ImplementsStmt* istm = implementsStmtForWrapperFn(wrapFn, isSuccess);

  for (Expr *consArg = call2wf->argList.head,
            *implArg = istm->iConstraint->consActuals.head;
       consArg != NULL;
       consArg = consArg->next, implArg = implArg->next)

    if (toSymExpr(consArg)->symbol()->type !=
        toSymExpr(implArg)->symbol()->type)
      // istm does not match
      return NULL;

  return istm;
}

static void pickMatchingImplementsStmt(Vec<FnSymbol*>&  visibleFns,
                                       CallExpr*        call2wf,
                                       ImplementsStmt*& firstSuccess,
                                       FnSymbol*&       firstFailure) {
  forv_Vec(FnSymbol, wrapFn, visibleFns) {
    bool isSuccess;
    if (ImplementsStmt* match = matchingImplStm(wrapFn, call2wf, isSuccess)) {
      if (isSuccess) {
        resolveImplementsStmt(match, true, true);
        firstSuccess = match;
        return;
      } else {
        if (firstFailure == NULL)
          firstFailure = wrapFn;
      }
    }
  }
}

static void cgprintCheckedConstraint(InterfaceSymbol* isym,
                                     IfcConstraint* constraint,
                                     CallExpr* callsite,
                                     ImplementsStmt *bestIstm) {
#ifdef PRINT_CG
  cgprint("checked constraint for %s  %s\n",
          symstring(isym), debugLoc(constraint));

  if (UnresolvedSymExpr* use = toUnresolvedSymExpr(callsite->baseExpr))
    cgprint("  for call to %s  at %s\n", use->unresolved, debugLoc(callsite));
  else {
    FnSymbol* wrapFn = toFnSymbol(toSymExpr(callsite->baseExpr)->symbol());
    if (wrapFn != NULL) {
      // This is the only use case.
      INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
      cgprint("  when checking the constraint at %s\n", debugLoc(wrapFn));
    }
  }

  if (bestIstm)
    cgprint("  satisfied with implements stmt%s  %s\n\n",
            idstring("", bestIstm), debugLoc(bestIstm));
  else
    cgprint("  not satisfied\n\n");
#endif
}

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

// Follows ImplementsStmt::build().
// Clears 'call2wf'.
static ImplementsStmt* buildInferredImplStmt(InterfaceSymbol* isym,
                                             CallExpr* call2wf) {
  IfcConstraint* icon = new IfcConstraint(new SymExpr(isym));
  for_alist(actual, call2wf->argList)
    icon->consActuals.insertAtTail(actual->remove());

 return new ImplementsStmt(icon, new BlockStmt());
}

static ImplementsStmt* checkInferredImplStmt(CallExpr* callsite,
                                             InterfaceSymbol* isym,
                                             CallExpr* call2wf,
                                             FnSymbol* failureWrapFn) {
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

  ImplementsStmt* istm = buildInferredImplStmt(isym, call2wf);
  anchor->insertBefore(istm);
  wrapOneImplementsStatement(istm);
  bool success = resolveImplementsStmt(istm, true, false);

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
ImplementsStmt* constraintIsSatisfiedAtCallSite(CallExpr* callsite,
                                                IfcConstraint* constraint,
                                                SymbolMap& substitutions) {
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

  ImplementsStmt *firstSuccess = NULL,
                 *bestIstm = NULL;
  FnSymbol* firstFailure = NULL;

  pickMatchingImplementsStmt(visibleFns, call2wf,
                             firstSuccess, firstFailure); // it sets these

  if (firstSuccess != NULL)
    // yippee, found a satisfying istm
    bestIstm = firstSuccess;

  else if (fInferImplementsStmts)
    // We found either a negative outcome of an earlier attempt to infer
    // in 'firstFailure', or nothing at all.
    bestIstm = checkInferredImplStmt(callsite, isym, call2wf,  firstFailure);

  call2wf = NULL; // call2wf is cleared in checkInferredImplStmt

  cgprintCheckedConstraint(isym, constraint, callsite, bestIstm);

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
      bool isSuccess;
      ImplementsStmt* istm = implementsStmtForWrapperFn(fn, isSuccess);
      INT_ASSERT(isSuccess); // remove this if there is a legitimate case
      resolveImplementsStmt(istm);
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
