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

#else
#define cgprint(...)
#endif


static void buildAndCheckFormals2Actuals(InterfaceSymbol* isym,
                                         IfcConstraint* icon,
                                         const char* context,
                                         SymbolMap& result) {
  if (isym->numFormals() != icon->numActuals()) {
    USR_FATAL(icon, "the number of actuals in the %s (%d)"
      " does not match the number of the interface formals (%d)",
      context, icon->numActuals(), isym->numFormals());
  }

  for (Expr *curFml = isym->ifcFormals.head, *curAct = icon->consActuals.head;
       curFml != NULL;
       curFml = curFml->next, curAct = curAct->next)
   result.put(toDefExpr(curFml)->sym, toSymExpr(curAct)->symbol());
}


/*********** resolveInterfaceSymbol ***********/

void resolveInterfaceSymbol(InterfaceSymbol* isym) {
  cgprint("resolving interface declaration %s  %s\n",
          symstring(isym), debugLoc(isym));

  for_alist(stmt, isym->ifcBody->body) {
    // A function required by the interface.
    FnSymbol* fn = toFnSymbol(toDefExpr(stmt)->sym);
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
  cgprint("\n");
}


/*********** InterfaceFunctionMap ***********/
//
// Implements visibility of interface functions within a CG function.
// Ex. worker() should be visible within fun():
//
//   interface IFC(T) { proc worker(arg:T); }
//   proc fun(x) where implements IFC(x.type) { worker(x); }
//

static FnSymbol* instantiateRequiredFn(FnSymbol* required, SymbolMap& fml2act)
{
  // Can we instead use instantiateSignature(required, fml2act, NULL) ?

  SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
  FnSymbol* instantiated = required->copy(&fml2actDup);
  instantiated->addFlag(FLAG_RESOLVED); // this is white lie
  for_formals(formal, instantiated) {
    TypeSymbol* fml = formal->type->symbol;
    if (Symbol* act = fml2act.get(fml))  // is this needed?
      formal->type = toTypeSymbol(act)->type;
  }
  return instantiated;
}


static void createRepsForRequiredFns(FnSymbol* fn, InterfaceInfo* ifcInfo) {
  INT_ASSERT(ifcInfo->repsForRequiredFns.empty()); // first time resolving 'fn'

  // we could resize this up front (does not work with gcc 4.8)
  //ifcInfo->repsForRequiredFns.resize(ifcInfo->interfaceConstraints.length);

  // For each required function of each interface, make an instantiation of
  // its signature visible to the function body.
  for_alist(iconExpr, ifcInfo->interfaceConstraints) {
    IfcConstraint* icon = toIfcConstraint(iconExpr);
    InterfaceSymbol* isym = icon->ifcSymbol();
    SymbolMap reps;
    SymbolMap fml2act;
    buildAndCheckFormals2Actuals(isym, icon, "interface constraint", fml2act);

    form_Map(SymbolMapElem, elem, isym->requiredFns) {
      FnSymbol* required = toFnSymbol(elem->key);
      FnSymbol* instantiated = instantiateRequiredFn(required, fml2act);
      // We may also want to make 'instantiated' visible in fn->where.
      fn->body->insertAtHead(new DefExpr(instantiated));
      reps.put(required, instantiated);
    }

    ifcInfo->repsForRequiredFns.push_back(reps);
  }
}


/*********** resolveConstrainedGenericFun ***********/

// If 'fn' is a CG function and has not been resolved yet, resolve it.
void resolveConstrainedGenericFun(FnSymbol* fn) {
  if (fn->isResolved()) return;
  InterfaceInfo* ifcInfo = fn->interfaceInfo;
  if (ifcInfo == NULL) return;  // not a CG
  cgprint("resolving CG function early %s  %s\n", symstring(fn), debugLoc(fn));

  createRepsForRequiredFns(fn, ifcInfo);

  // Seems like we do not need fn->setGeneric(false) for resolveFunction()
  // NB we want 'fn' to be generic later, when checking isApplicable()
  resolveFunction(fn);

  for_alist(expr, ifcInfo->constrainedTypes)
    toDefExpr(expr)->sym->addFlag(FLAG_GENERIC);

  // Remove the reps created in createRepsForRequiredFns()
  // that were needed during resolveFunction(fn).
  // This way they do not show in instantiations of 'fn'.
  // 'fn' itself will be pruned as it is considered generic.
  for (int indx = 0; indx < (int)ifcInfo->repsForRequiredFns.size(); indx++) {
    form_Map(SymbolMapElem, elem, ifcInfo->repsForRequiredFns[indx]) {
      FnSymbol* instantiated = toFnSymbol(elem->value);
      INT_ASSERT(instantiated->defPoint->parentExpr == fn->body);
      instantiated->defPoint->remove();
    }
  }
  cgprint("\n");
}


/*********** resolveImplementsStmt ***********/

// Returns whether a witness has been established successfully.
static bool resolveWitness(InterfaceSymbol* isym, ImplementsStmt* istm,
                           SymbolMap& fml2act, BlockStmt* holder,
                           //CG TODO: 'indent' is used only for debug. output
                           const char* indent,
                           bool reportErrors,
                           FnSymbol* reqFn, Symbol*& implRef) {
  INT_ASSERT(holder->body.empty());
  CallExpr* call = new CallExpr(reqFn->name);
  for_formals(formal, reqFn)
    call->insertAtTail(formal->copy(&fml2act));
  holder->insertAtTail(call);
  FnSymbol* target = tryResolveCall(call);
  if (target != NULL) {
    cgprint("%s  witness %s  %s\n", indent,
            symstring(target), debugLoc(target));
    resolveFunction(target); // aborts if there are errors

    call->remove();
    if (holder->body.tail != NULL) {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "the implementation for the required function %s"
          " requires implicit conversion(s), which is currently disallowed",
          reqFn->name);
        USR_PRINT(target, "the implementation is here");
        USR_PRINT(reqFn, "the required function in the interface %s is here",
                         isym->name);
      }
      target = NULL;
      do { holder->body.tail->remove(); } while (holder->body.tail != NULL);
    }
  } else {
    if (implRef == gDummyWitness) {
      if (reportErrors) {
        USR_FATAL_CONT(istm, "the required function %s is not implemented",
                              reqFn->name);
        USR_PRINT(reqFn, "the required function in the interface %s is here",
                         isym->name);
      }
    } else {
      SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
      target = toFnSymbol(implRef->copy(&fml2actDup));
      holder->insertBefore(new DefExpr(target));
      INT_ASSERT(target->isResolved()); // the dflt impl was resolved with isym
      cgprint("%s  default %s  %s\n", indent,
              symstring(target), debugLoc(target));
    }
    call->remove();
  }

  implRef = target; // aka istm->witnesses[indx].put(reqFn, target)
  return target != NULL;
}

//
// Ensures this ImplementsStmt indeed implements the interface, ex.
// * determine each witness, default or otherwise
// * resolve each witness
// Returns false otherwise.
//
// CG TODO: 'nested' is used only for debugging output
static bool resolveImplementsStmt(ImplementsStmt* istm, bool nested,
                                  bool reportErrors) {
  if (istm->id == breakOnResolveID) gdbShouldBreakHere();
  IfcConstraint* icon = istm->iConstraint;
  InterfaceSymbol* isym = icon->ifcSymbol();
  FnSymbol* wrapFn = wrapperFnForImplementsStmt(istm);
  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  if (wrapFn->isResolved()) {
    INT_ASSERT(reportErrors);
    return true; // no need to resolve again; we must have succeeded before.
  }

  if (nested) cgprint("    resolving it...\n");
  else cgprint("resolving implements statement%s for ifc %s  %s\n",
               idstring("", istm), symstring(isym), debugLoc(istm));

  // ideally, removing and re-inserting 'istm' would not be needed
  Expr* anchor = istm->next;
  istm->remove();
  resolveFunction(wrapFn);
  anchor->insertBefore(istm);

  SymbolMap fml2act; // isym formal -> istm actual
  buildAndCheckFormals2Actuals(isym, icon, "implements statement", fml2act);

  INT_ASSERT(istm->witnesses.n == 0);      // we have not filled it yet
  istm->witnesses.copy(isym->requiredFns); // start with the defaults impls
  SET_LINENO(istm->implBody);
  BlockStmt* holder = new BlockStmt();
  istm->implBody->insertAtTail(holder);
  bool witSuccess = true;
  form_Map(SymbolMapElem, wit, istm->witnesses)
    witSuccess &= resolveWitness(isym, istm, fml2act, holder,
                                 nested ? "  " : "", reportErrors,
                                 toFnSymbol(wit->key), wit->value);
  holder->remove();
  wrapFn->addFlag(FLAG_RESOLVED);
  if (reportErrors && ! witSuccess) USR_STOP();

  cgprint(nested ? "    ...done\n" : "\n");
  return witSuccess;
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
        firstSuccess = match;
        return;
      } else {
        if (firstFailure == NULL)
          firstFailure = wrapFn;
      }
    }
  }
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
  // Place the outcome - positive or negative - next to 'anchor'.

  ImplementsStmt* istm = buildInferredImplStmt(isym, call2wf);
  anchor->insertBefore(istm);
  FnSymbol* wrapFn = wrapOneImplementsStatement(istm);
  bool success = resolveImplementsStmt(istm, true, false);
  if (success) {
    return istm;
  } else {
    markImplStmtWrapFnAsFailure(wrapFn);
    return NULL;
  }
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
  // because it contains properly instantiated arguments.
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

  cgprint("checked constraint for %s  %s\n",
          symstring(isym), debugLoc(constraint));
  cgprint("  for call to %s  at %s\n",
          toUnresolvedSymExpr(callsite->baseExpr)->unresolved,
          debugLoc(callsite));
  if (bestIstm) cgprint("  satisfied with implements stmt%s  %s\n\n",
                         idstring("", bestIstm), debugLoc(bestIstm));
  else          cgprint("  not satisfied\n\n");

  return bestIstm;
}


/*********** other ***********/

//
// 'fn' was created by FnSymbol::partialCopy() from an early-resolved
// CG function, specializing it for the given witness(es).
// We need to adjust the outcome. For example, set up the substitutions
// from the FnSymbols representing interface functions within the
// early-resolved CG function to the FnSymbols of the corresponding
// witnesses. These substitutions will be applied in FnSymbol::finalizeCopy().
//
void cleanupInstantiatedCGfun(FnSymbol* fn,
                              std::vector<ImplementsStmt*>& witnesses)
{
  if (fatalErrorsEncountered())
    return; // got errors, do not proceed

  cgprint("considering CG candidate %s%s  %s\n", symstring(fn),
          idstring("  from", fn->instantiatedFrom), debugLoc(fn));
#ifdef PRINT_CG
  cgprint("  witness(es):\n");
  for_vector(ImplementsStmt, istm, witnesses) {
    IfcConstraint* icon = istm->iConstraint;
    InterfaceSymbol* isym = icon->ifcSymbol();
    cgprint("  implements stmt%s for ifc %s  %s\n",
            idstring("", istm), symstring(isym), debugLoc(istm));
  }
#endif

  PartialCopyData* pcd = getPartialCopyData(fn);
  INT_ASSERT((pcd == NULL) == (fn->interfaceInfo == NULL));
  if (pcd == NULL)
    return; // this function has been cleaned up already

  SymbolMap& substitutions = pcd->partialCopyMap;

  // Resolve ImplementsStmts that haven't been already.
  // Extend 'substitutions'.
  for (int indx = 0; indx < (int)witnesses.size(); indx++) {
    ImplementsStmt* istm = witnesses[indx];
    resolveImplementsStmt(istm, true, true);
    SymbolMap& reps = fn->instantiatedFrom->interfaceInfo
                       ->repsForRequiredFns[indx];
    form_Map(SymbolMapElem, elem, reps) {
      Symbol* required = elem->key;
      Symbol* usedInFn = elem->value;
      Symbol* implem   = istm->witnesses.get(required);
      INT_ASSERT(isFnSymbol(required) && isFnSymbol(usedInFn) &&
                 isFnSymbol(implem));
      substitutions.put(usedInFn, implem);
    }
  }

  // remove fn->interfaceInfo

  INT_ASSERT(fn->interfaceInfo->repsForRequiredFns.empty());
  INT_ASSERT(! fn->interfaceInfo->interfaceConstraints.empty());
  for_alist(expr, fn->interfaceInfo->constrainedTypes)
    expr->remove();
  for_alist(expr, fn->interfaceInfo->interfaceConstraints)
    expr->remove();
  delete fn->interfaceInfo;
  fn->interfaceInfo = NULL;

  // 'fn' is concrete now

  fn->setGeneric(false);

  cgprint("\n");
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
