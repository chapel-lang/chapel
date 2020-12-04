/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "PartialCopyData.h"
#include "ResolutionCandidate.h"
#include "resolveFunction.h"
#include "stmt.h"

static void buildAndCheckFormals2Actuals(InterfaceSymbol* isym,
                                         ImplementsExpr* ie,
                                         const char* context,
                                         SymbolMap& result) {
  if (isym->numFormals() != ie->numActuals()) {
    USR_FATAL(ie, "the number of actuals in the %s (%d)"
      " does not match the number of the interface formals (%d)",
      context, ie->numActuals(), isym->numFormals());
  }

  for (Expr *curFml = isym->ifcFormals.head, *curAct = ie->implActuals.head;
       curFml != NULL;
       curFml = curFml->next, curAct = curAct->next)
   result.put(toDefExpr(curFml)->sym, toSymExpr(curAct)->symbol());
}

/*********** resolveInterfaceSymbol ***********/

void resolveInterfaceSymbol(InterfaceSymbol* isym) {
  for_alist(stmt, isym->ifcBody->body) {
    // A function required by the interface.
    FnSymbol* fn = toFnSymbol(toDefExpr(stmt)->sym);
    if (fn->hasFlag(FLAG_NO_FN_BODY))
      isym->ifcSymbols.put(fn, gDummyWitness);
    else
      isym->ifcSymbols.put(fn, fn); // with the default implementation

    if (fn->where != NULL)
      USR_FATAL_CONT(fn->where, "the interface function %s%s", fn->name,
            " contains a where clause, which is currently not supported");

    resolveFunction(fn);
  }
}

/*********** resolveConstrainedGenericFun ***********/

static FnSymbol* instantiateRequiredFn(FnSymbol* required, SymbolMap& fml2act)
{
  // can we instead use instantiateSignature(required, fml2act, NULL) ?
  SymbolMap fml2actDup = fml2act; // avoid fml2act updates by copy()
  FnSymbol* instantiated = required->copy(&fml2actDup);
  instantiated->addFlag(FLAG_RESOLVED); // white lie
  for_formals(formal, instantiated) {
    TypeSymbol* fml = formal->type->symbol;
    if (Symbol* act = fml2act.get(fml))  //taken care by above copy() ?
      formal->type = toTypeSymbol(act)->type;
  }
  return instantiated;
}


static void populateInterfaceFunctionMap(FnSymbol* fn) {
  INT_ASSERT(fn->interface2actuals.empty()); // first time resolving this fn
  fn->interface2actuals.resize(fn->interfaceConstraints.length);
  int indx = 0;

  // For each required function of each interface, make an instantiation of
  // its signature visible to the function body.
  for_alist(ieExpr, fn->interfaceConstraints) {
    ImplementsExpr* ie = toImplementsExpr(ieExpr);
    InterfaceSymbol* isym = ie->implInterfaceSym();
    SymbolMap& i2a = fn->interface2actuals[indx++];
    SymbolMap fml2act;
    buildAndCheckFormals2Actuals(isym, ie, "interface constraint", fml2act);
    form_Map(SymbolMapElem, elem, isym->ifcSymbols) {
      FnSymbol* required = toFnSymbol(elem->key);
      FnSymbol* instantiated = instantiateRequiredFn(required, fml2act);
      fn->body->insertAtHead(new DefExpr(instantiated));
      i2a.put(required, instantiated);
    }
  }
}

// If it a CG function and has not been resolved yet, resolve it.
void resolveConstrainedGenericFun(FnSymbol* fn) {
  if (fn->isResolved()) return;
  if (! fn->isConstrainedGeneric()) return;

  populateInterfaceFunctionMap(fn);

  // do we need fn->setGeneric(false) for resolveFunction()?
  // note that we want 'fn' to be generic when checking isApplicable()
  resolveFunction(fn);

  for_alist(expr, fn->constrainedTypes) {
    TypeSymbol* ts = toTypeSymbol(toDefExpr(expr)->sym);
    ts->addFlag(FLAG_GENERIC); // for isApplicableGeneric()
  }

  for (int indx = 0; indx < (int)fn->interface2actuals.size(); indx++) {
    form_Map(SymbolMapElem, elem, fn->interface2actuals[indx]) {
      FnSymbol* instantiated = toFnSymbol(elem->value);
      INT_ASSERT(instantiated->defPoint->parentExpr == fn->body);
      instantiated->defPoint->remove();
    }
  }
}

/*********** resolveImplementsStmt ***********/

// Returns whether a witness has been established successfully.
static bool resolveWitness(InterfaceSymbol* isym, ImplementsStmt* istm,
                           SymbolMap& fml2act, BlockStmt* holder,
                           FnSymbol* reqFn, Symbol*& implRef) {
  INT_ASSERT(holder->body.empty());
  CallExpr* call = new CallExpr(reqFn->name);
  for_formals(formal, reqFn)
    call->insertAtTail(formal->copy(&fml2act));
  holder->insertAtTail(call);
  FnSymbol* target = tryResolveCall(call);
  if (target != NULL) {
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
    }
    call->remove();
  }

  implRef = target; // aka istm->witnesses[i].put(reqFn, target)
  return target != NULL;
}

/*
Ensure it indeed implements the interface.
Build witnesses.
Resolve function signatures and function bodies.
*/
void resolveImplementsStmt(ImplementsStmt* istm) {
  ImplementsExpr* ie = istm->implExpr;
  InterfaceSymbol* isym = ie->implInterfaceSym();
  FnSymbol* wrapFn = toFnSymbol(istm->parentSymbol);
  INT_ASSERT(wrapFn->hasFlag(FLAG_IMPLEMENTS_WRAPPER));
  if (wrapFn->isResolved())
    return; // no need to resolve again

  SymbolMap fml2act; // isym formal -> istm actual

  buildAndCheckFormals2Actuals(isym, ie, "implements statement", fml2act);

  INT_ASSERT(istm->witnesses.n == 0);     // we have not filled it yet
  istm->witnesses.copy(isym->ifcSymbols); // start with the defaults impls

  SET_LINENO(istm->implBody);
  BlockStmt* holder = new BlockStmt();
  istm->implBody->insertAtTail(holder);
  bool witSuccess = true;
  form_Map(SymbolMapElem, wit, istm->witnesses)
    witSuccess &= resolveWitness(isym, istm, fml2act, holder,
                                 toFnSymbol(wit->key), wit->value);
  holder->remove();
  wrapFn->addFlag(FLAG_RESOLVED);
  if (! witSuccess) USR_STOP();
}

/*********** checkInterfaceConstraints ***********/

ImplementsStmt* constraintIsSatisfiedAtCallSite(CallExpr* callsite,
                                                ImplementsExpr* constraint,
                                                SymbolMap& substitutions) {
  // build a call to the wrapper function
  InterfaceSymbol* isym = constraint->implInterfaceSym();
  CallExpr* call2wf = new CallExpr(ImplementsStmt::wrapperName(isym));
  for_alist(act, constraint->implActuals)
    call2wf->insertAtTail(act->copy(&substitutions));

  // resolve this call
  BlockStmt* holder = new BlockStmt();
  callsite->insertBefore(holder);
  holder->insertAtTail(call2wf);
  FnSymbol* wrapFn = tryResolveCall(call2wf);

  // cleanup
  call2wf->remove();
  holder->remove();
  if (! holder->body.empty() && wrapFn != NULL) {
    USR_FATAL_CONT(constraint, "using an implements declaration"
                 " for this constraint with the interface '%s'"
                 " is not supported because it required argument conversions",
                 isym->name);
    USR_PRINT(wrapFn, "the implements declaration is here");
    USR_PRINT(callsite, "the call to the constrained function is here");
  }
  
  // extract the witness
  if (wrapFn != NULL)
    return ImplementsStmt::extractImplementsStmtFromWrapper(wrapFn);
  else
    return NULL;
}

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
  SymbolMap& substitutions = getPartialCopyData(fn)->partialCopyMap;

  // Resolve ImplementsStmts that haven't been already.
  // Extend 'substitutions'.
  for (int indx = 0; indx < (int)witnesses.size(); indx++) {
    ImplementsStmt* istm = witnesses[indx];
    resolveImplementsStmt(istm);
    SymbolMap& i2impl = istm->witnesses;
    SymbolMap& i2fn   = fn->instantiatedFrom->interface2actuals[indx];
    form_Map(SymbolMapElem, i2fnElem, i2fn) {
      Symbol* required = i2fnElem->key;
      Symbol* usedInFn = i2fnElem->value;
      Symbol* implem   = i2impl.get(required);
      INT_ASSERT(isFnSymbol(required) && isFnSymbol(usedInFn) &&
                 isFnSymbol(implem));
      substitutions.put(usedInFn, implem);
    }
  }

  // other cleanups
  fn->setGeneric(false); // fn->clearGeneric() ?

  for_alist(expr, fn->interfaceConstraints)
    expr->remove();
}
