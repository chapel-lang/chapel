/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
      USR_FATAL_CONT(istm, "the implementation for the required function %s"
        " requires implicit conversion(s), which is currently disallowed",
        reqFn->name);
      USR_PRINT(target, "the implementation is here");
      USR_PRINT(reqFn, "the required function in the interface %s is here",
                       isym->name);
      target = NULL;
      do { holder->body.tail->remove(); } while (holder->body.tail != NULL);
    }
  } else {
    if (implRef == gDummyWitness) {
      USR_FATAL_CONT(istm, "the required function %s is not implemented",
                            reqFn->name);
      USR_PRINT(reqFn, "the required function in the interface %s is here",
                       isym->name);
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
// Ensure this ImplementsStmt indeed implements the interface, ex.
// * determine each witness, default or otherwise
// * resolve each witness
//
// CG TODO: 'nested' is used only for debugging output
static void resolveImplementsStmt(ImplementsStmt* istm, bool nested) {
  IfcConstraint* icon = istm->iConstraint;
  InterfaceSymbol* isym = icon->ifcSymbol();
  FnSymbol* wrapFn = wrapperFnForImplementsStmt(istm);
  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  if (wrapFn->isResolved())
    return; // no need to resolve again

  if (nested) cgprint("    resolving it...\n");
  else cgprint("resolving implements statement%s for ifc %s  %s\n",
               idstring("", istm), symstring(isym), debugLoc(istm));

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
                                 nested ? "  " : "",
                                 toFnSymbol(wit->key), wit->value);
  holder->remove();
  wrapFn->addFlag(FLAG_RESOLVED);
  if (! witSuccess) USR_STOP();

  cgprint(nested ? "    ...done\n" : "\n");
}

void resolveImplementsStmt(ImplementsStmt* istm) {
  resolveImplementsStmt(istm, false);
}


/*********** constraintIsSatisfiedAtCallSite ***********/

static void gatherVisibleWrapperFns(CallExpr* callsite, CallExpr* call2wf,
                                    Vec<FnSymbol*>& visibleFns) {
  callsite->insertBefore(call2wf);
  CallInfo info;
  bool infoOK = info.isWellFormed(call2wf);
  INT_ASSERT(infoOK);

  findVisibleFunctionsAllPOIs(info, visibleFns);
  call2wf->remove();
}

static ImplementsStmt* bestVisibleImplementsStmt(CallExpr* call2wf,
                                                 Vec<FnSymbol*>& visibleFns) {
  ImplementsStmt* bestIstm = NULL;
  forv_Vec(FnSymbol, wrapFn, visibleFns) {
    ImplementsStmt* istm = implementsStmtForWrapperFn(wrapFn);
    bool gotNonMatch = false;

    // 'istm' matches the constraint if the constraint arguments
    // equal the implements arguments.
    for (Expr *consArg = call2wf->argList.head,
              *implArg = istm->iConstraint->consActuals.head;
         consArg != NULL;
         consArg = consArg->next, implArg = implArg->next)
    {
      if (toSymExpr(consArg)->symbol()->type !=
          toSymExpr(implArg)->symbol()->type) {
        gotNonMatch = true; break;
      }
    }

    if (gotNonMatch)
      continue;

    // So this istm matches. Have we seen another match before?
    if (bestIstm == NULL)
      bestIstm = istm;
    else
      // got an ambiguity, meaning that the constraint is not satisfied
      // CG TODO: issue a warning or an error?
      return NULL;
  }

  return bestIstm;
}

ImplementsStmt* constraintIsSatisfiedAtCallSite(CallExpr* callsite,
                                                IfcConstraint* constraint,
                                                SymbolMap& substitutions) {
  // build a call to the wrapper function
  InterfaceSymbol* isym = constraint->ifcSymbol();
  CallExpr* call2wf = new CallExpr(implementsStmtWrapperName(isym));
  for_alist(act, constraint->consActuals)
    call2wf->insertAtTail(act->copy(&substitutions));

  // An earlier version resolved the call completely.
  // That required figuring out whether the wrapper functions
  // were concrete or generic, also needed to avoid instantiating them.
  // An even earlier version checked the wrapper function's where-clause.
  // Instead, just gather all visible functions and manually check
  // their applicability.

  Vec<FnSymbol*> visibleFns;
  gatherVisibleWrapperFns(callsite, call2wf, visibleFns);

  ImplementsStmt* bestIstm = bestVisibleImplementsStmt(call2wf, visibleFns);

  cgprint("checking constraint for %s  %s\n",
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

  SymbolMap& substitutions = getPartialCopyData(fn)->partialCopyMap;

  // Resolve ImplementsStmts that haven't been already.
  // Extend 'substitutions'.
  for (int indx = 0; indx < (int)witnesses.size(); indx++) {
    ImplementsStmt* istm = witnesses[indx];
    resolveImplementsStmt(istm, true);
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

  // other cleanups

  fn->setGeneric(false);

  INT_ASSERT(! fn->interfaceInfo->interfaceConstraints.empty());
  for_alist(expr, fn->interfaceInfo->interfaceConstraints)
    expr->remove();

  cgprint("\n");
}

void resolveConstrainedGenericSymbol(Symbol* sym, bool mustBeCG) {
  if (FnSymbol* fn = toFnSymbol(sym)) {
    if (fn->hasFlag(FLAG_IMPLEMENTS_WRAPPER)) {
      ImplementsStmt* istm = implementsStmtForWrapperFn(fn);
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
