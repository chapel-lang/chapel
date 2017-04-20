/*
 * Copyright 2004-2017 Cray Inc.
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

#include "visibleCandidates.h"

#include "astutil.h"
#include "callInfo.h"
#include "expr.h"
#include "PartialCopyData.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static void      doGatherCandidates(CallInfo&                  info,
                                    Vec<FnSymbol*>&            visibleFns,
                                    bool                       generated,
                                    Vec<ResolutionCandidate*>& candidates);

static void      filterCandidate(CallInfo&                  info,
                                 FnSymbol*                  fn,
                                 Vec<ResolutionCandidate*>& candidates);

static void      filterCandidate(CallInfo&                  info,
                                 ResolutionCandidate*       currCandidate,
                                 Vec<ResolutionCandidate*>& candidates);

static void      filterConcrete(CallInfo&                  info,
                                ResolutionCandidate*       currCandidate,
                                Vec<ResolutionCandidate*>& candidates);

static void      resolveTypeConstructor(CallInfo& info,
                                        FnSymbol* fn);

static void      filterGeneric(CallInfo&                  info,
                               ResolutionCandidate*       currCandidate,
                               Vec<ResolutionCandidate*>& candidates);

static int       varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs);

static bool      isVarargSizeExpr (SymExpr* se, CallExpr* parent);

static FnSymbol* expandVarArgs(FnSymbol* origFn, CallInfo& info);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void findVisibleCandidates(CallInfo&                  info,
                           Vec<FnSymbol*>&            visibleFns,
                           Vec<ResolutionCandidate*>& candidates) {
  // Search user-defined (i.e. non-compiler-generated) functions first.
  doGatherCandidates(info, visibleFns, false, candidates);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    doGatherCandidates(info, visibleFns, true, candidates);
  }
}


static void doGatherCandidates(CallInfo&                  info,
                               Vec<FnSymbol*>&            visibleFns,
                               bool                       compilerGenerated,
                               Vec<ResolutionCandidate*>& candidates) {
  forv_Vec(FnSymbol, fn, visibleFns) {
    // Consider either the user-defined functions or the compiler-generated
    // functions based on the input 'compilerGenerated'.
    if (fn->hasFlag(FLAG_COMPILER_GENERATED) == compilerGenerated) {

      // Consider
      //
      //   c1.foo(10, 20);
      //
      // where foo() is a simple method on some class/record
      //
      // Normalize currently converts this to
      //
      //   #<Call     #<Call "foo" _mt c1>    10    20>
      //
      // Resolution performs a post-order traversal of this expression
      // and so the inner call is visited before the outer call.
      //
      // In this context, the inner "call" is effectively a field access
      // rather than a true function call.  Normalize sets the methodTag
      // property to true to indicate this, and this form of call can only
      // be matched to parentheses-less methods and type constructors.
      //
      // Later steps will convert the outer call to become
      //
      //   #<Call "foo" _mt c1  10    20>
      //
      // This outer call has methodTag set to false and this call
      // should be filtered against the available visibleFunctions.
      //

      if (info.call->methodTag == false) {
        filterCandidate(info, fn, candidates);

      } else {
        if (fn->hasFlag(FLAG_NO_PARENS)        == true ||
            fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
          filterCandidate(info, fn, candidates);
        }
      }
    }
  }
}


/** Tests to see if a function is a candidate for resolving a specific call.
 *  If it is a candidate, we add it to the candidate lists.
 *
 * \param info          The CallInfo object for the call site.
 * \param currCandidate The current candidate to consider.
 * \param candidates    The list to add possible candidates to.
 */

static void filterCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates) {
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn);

  if (fExplainVerbose &&
      ((explainCallLine && explainCallMatch(info.call)) ||
       info.call->id == explainCallID)) {
    USR_PRINT(fn, "Considering function: %s", toString(fn));

    if (info.call->id == breakOnResolveID) {
      gdbShouldBreakHere();
    }
  }

  filterCandidate(info, currCandidate, candidates);

  if (candidates.tail() != currCandidate) {
    delete currCandidate;
  }
}


/** Tests to see if a function is a candidate for resolving a specific call.
 *  If it is a candidate, we add it to the candidate lists.
 *
 * This version of filterCandidate is called by other versions of
 * filterCandidate, and shouldn't be called outside this family of functions.
 *
 * \param info          The CallInfo object for the call site.
 * \param currCandidate The current candidate to consider.
 * \param candidates    The list to add possible candidates to.
 */
static void filterCandidate(CallInfo&                  info,
                            ResolutionCandidate*       currCandidate,
                            Vec<ResolutionCandidate*>& candidates) {
  if (currCandidate->fn->hasFlag(FLAG_GENERIC) == false) {
    filterConcrete(info, currCandidate, candidates);
  } else {
    filterGeneric (info, currCandidate, candidates);
  }
}


/** Candidate filtering logic specific to concrete functions.
 *
 * \param info          The CallInfo object for the call site.
 * \param currCandidate The current candidate to consider.
 * \param candidates    The list to add possible candidates to.
 */
static void filterConcrete(CallInfo&                  info,
                           ResolutionCandidate*       currCandidate,
                           Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn != NULL) {
    resolveTypedefedArgTypes(currCandidate->fn);

    if (currCandidate->computeAlignment(info) == true) {
      // Ensure that type constructor is resolved before other constructors.
      if (currCandidate->fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
        resolveTypeConstructor(info, currCandidate->fn);
      }

      if (checkResolveFormalsWhereClauses(currCandidate) == true) {
        candidates.add(currCandidate);
      }
    }
  }
}

void resolveTypedefedArgTypes(FnSymbol* fn) {
  for_formals(formal, fn) {
    INT_ASSERT(formal->type); // Should be *something*.

    if (formal->type == dtUnknown) {
      if (BlockStmt* block = formal->typeExpr) {
        if (SymExpr* se = toSymExpr(block->body.first())) {
          if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) == true) {
            Type* type = resolveTypeAlias(se);

            INT_ASSERT(type);

            formal->type = type;
          }
        }
      }
    }
  }
}

static void resolveTypeConstructor(CallInfo& info, FnSymbol* fn) {
  SET_LINENO(fn);

  // Ignore tuple constructors; they were generated
  // with their type constructors.
  if (fn->hasFlag(FLAG_PARTIAL_TUPLE) == false) {
    CallExpr* typeConstructorCall = new CallExpr(astr("_type", fn->name));

    for_formals(formal, fn) {
      if (!formal->hasFlag(FLAG_IS_MEME)) {
        if (fn->_this->type->symbol->hasFlag(FLAG_TUPLE)) {
          if (formal->instantiatedFrom) {
            typeConstructorCall->insertAtTail(formal->type->symbol);
          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            typeConstructorCall->insertAtTail(paramMap.get(formal));
          }

        } else {
          if (strcmp(formal->name, "outer") == 0 ||
              formal->type                  == dtMethodToken) {
            typeConstructorCall->insertAtTail(formal);

          } else if (formal->instantiatedFrom) {
            SymExpr*   se = new SymExpr(formal->type->symbol);
            NamedExpr* ne = new NamedExpr(formal->name, se);

            typeConstructorCall->insertAtTail(ne);

          } else if (formal->hasFlag(FLAG_INSTANTIATED_PARAM)) {
            SymExpr*   se = new SymExpr(paramMap.get(formal));
            NamedExpr* ne = new NamedExpr(formal->name, se);

            typeConstructorCall->insertAtTail(ne);
          }
        }
      }
    }

    info.call->insertBefore(typeConstructorCall);

    // If instead we call resolveCallAndCallee(typeConstructorCall)
    // then the line number reported in an error would change
    // e.g.: domains/deitz/test_generic_class_of_sparse_domain
    // or:   classes/diten/multipledestructor
    resolveCall(typeConstructorCall);

    INT_ASSERT(typeConstructorCall->isResolved());

    resolveFns(typeConstructorCall->resolvedFunction());

    fn->_this->type = typeConstructorCall->isResolved()->retType;

    typeConstructorCall->remove();
  }
}


/** Candidate filtering logic specific to generic functions.
 *
 * \param info          The CallInfo object for the call site.
 * \param currCandidate The current candidate to consider.
 * \param candidates    The list to add possible candidates to.
 */
static void filterGeneric(CallInfo&                  info,
                          ResolutionCandidate*       currCandidate,
                          Vec<ResolutionCandidate*>& candidates) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (currCandidate->fn                     != NULL &&
      currCandidate->computeAlignment(info) == true &&
      checkGenericFormals(currCandidate)    == true) {
    // Compute the param/type substitutions for generic arguments.
    currCandidate->computeSubstitutions();

    if (currCandidate->substitutions.n > 0) {
      /*
       * Instantiate enough of the generic to get through the rest of the
       * filtering and disambiguation processes.
       */
      currCandidate->fn = instantiateSignature(currCandidate->fn,
                                               currCandidate->substitutions,
                                               info.call);

      if (currCandidate->fn != NULL) {
        filterCandidate(info, currCandidate, candidates);
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Maintain a cache from a vargArgs function to the function with the required *
* number of formals.                                                          *
*                                                                             *
************************************** | *************************************/

typedef std::map<FnSymbol*, std::vector<FnSymbol*>*> ExpandVarArgsMap;

static ExpandVarArgsMap sCache;

static FnSymbol* cacheLookup(FnSymbol* fn, int numActuals) {
  ExpandVarArgsMap::iterator it     = sCache.find(fn);
  FnSymbol*                  retval = NULL;

  if (it != sCache.end()) {
    std::vector<FnSymbol*>* fns = it->second;

    for (size_t i = 0; i < (*fns).size() && retval == NULL; i++) {
      if ((*fns)[i]->numFormals() == numActuals) {
        retval = (*fns)[i];
      }
    }
  }

  return retval;
}

static void cacheExtend(FnSymbol* fn, FnSymbol* expansion) {
  ExpandVarArgsMap::iterator it = sCache.find(fn);

  if (it != sCache.end()) {
    it->second->push_back(expansion);

  } else {
    std::vector<FnSymbol*>* fns = new std::vector<FnSymbol*>();

    fns->push_back(expansion);

    sCache[fn] = fns;
  }
}

/************************************* | **************************************
*                                                                             *
* If the function accepts a variable number of args, map it to a function     *
* with the necessary number of formals.                                       *
*                                                                             *
* 2017/04/05 There are several points at which this implementation assumes    *
* there is only one set of varargs.                                           *
*                                                                             *
************************************** | *************************************/

static bool hasVariableArgs(FnSymbol* fn);

FnSymbol* expandIfVarArgs(FnSymbol* fn, CallInfo& info) {
  FnSymbol* retval = fn;

  if (hasVariableArgs(fn) == true) {
    retval = cacheLookup(fn, info.actuals.n);

    // No substitution found
    if (retval == NULL) {
      retval = expandVarArgs(fn, info);

      if (retval != NULL) {
        cacheExtend(fn, retval);
      }
    }
  }

  return retval;
}

static bool hasVariableArgs(FnSymbol* fn) {
  bool retval = false;

  for_formals(formal, fn) {
    if (formal->variableExpr != NULL) {
      retval = true;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* The function is known to have at least one var-args set.                    *
*                                                                             *
* Portions of the current implementation assume that there is exactly one     *
* var-args set.  Handling multiple sets is complex for both the user and      *
* the implementation.                                                         *
*                                                                             *
* This constraint is enforced by expandVarArgs() and is then implicitly       *
* assumed by the helper functions.                                            *
*                                                                             *
************************************** | *************************************/

typedef std::vector<ArgSymbol*> Formals;

static void       expandVarArgsFixed (FnSymbol*  fn, CallInfo& info);

static int        varArgsCount(ArgSymbol* formal, VarSymbol* nVar);

static FnSymbol*  expandVarArgsQuery (FnSymbol*  fn, CallInfo& info);

static void       expandVarArgsFormal(FnSymbol*  fn,
                                      ArgSymbol* formal,
                                      int        n);

static Formals    insertFormalsForVarArg(ArgSymbol* varArg, int n);

static void       expandVarArgsWhere(FnSymbol*      fn,
                                     ArgSymbol*     formal,
                                     const Formals& varargs);

static void       expandVarArgsBody(FnSymbol*      fn,
                                    ArgSymbol*     formal,
                                    const Formals& varargs);

static bool       handleCopyData(PartialCopyData* pci,
                                 VarSymbol*       var,
                                 ArgSymbol*       formal,
                                 const Formals&   varargs);

static VarSymbol* buildTupleVariable(ArgSymbol* formal);

static CallExpr*  buildTupleCall(ArgSymbol* formal, const Formals& formals);

static bool       needVarArgTupleAsWhole(BlockStmt* block,
                                         int        numArgs,
                                         ArgSymbol* formal);

static void       substituteVarargTupleRefs(BlockStmt*     block,
                                            ArgSymbol*     formal,
                                            const Formals& varargs);

static FnSymbol* expandVarArgs(FnSymbol* fn, CallInfo& info) {
  int       numVarArgs      = 0;
  bool      isQueryVariable = false;
  FnSymbol* retval          = NULL;

  for_formals(formal, fn) {
    if (formal->variableExpr != NULL) {
      if (isDefExpr(formal->variableExpr->body.tail) == true) {
        isQueryVariable = true;
      }

      numVarArgs = numVarArgs + 1;
    }
  }

  if (numVarArgs == 0) {
    INT_ASSERT(false);

  } else if (numVarArgs == 1) {

    if (isQueryVariable == false) {
      expandVarArgsFixed(fn, info);

      retval = fn;

    } else {
      retval = expandVarArgsQuery(fn, info);
    }

  } else {
    USR_FATAL(fn, "No support for a function with multiple vararg sets");
  }

  return retval;
}

// No query variable e.g. proc foo(x, y, z ... 5)
//                   or   proc foo(x, y, z ... paramValue)
static void expandVarArgsFixed(FnSymbol* fn, CallInfo& info) {
  bool genericArgSeen = false;

  for_formals(formal, fn) {
    if (BlockStmt* block = formal->variableExpr) {
      if (genericArgSeen == false) {
        resolveBlockStmt(block);
      }

      if (SymExpr* sym = toSymExpr(block->body.tail)) {
        if (VarSymbol* nVar = toVarSymbol(sym->symbol())) {
          expandVarArgsFormal(fn, formal, varArgsCount(formal, nVar));
        }

      } else if (fn->hasFlag(FLAG_GENERIC) == false) {
        INT_FATAL("bad variableExpr");
      }

      // It is certain that there is just one var-arg set to handle
      break;

    } else {
      if (formal->type && formal->type->symbol->hasFlag(FLAG_GENERIC)) {
        genericArgSeen = true;
      }
    }
  }
}

static int varArgsCount(ArgSymbol* formal, VarSymbol* nVar) {
  int retval = 0;

  if (nVar->type == dtInt[INT_SIZE_DEFAULT] && nVar->immediate != NULL) {
    retval = nVar->immediate->int_value();

  } else {
    INT_FATAL(formal, "unexpected non-VarSymbol");
  }

  return retval;
}

// A  query variable e.g. proc foo(x, y, z ... ?N)
static FnSymbol* expandVarArgsQuery(FnSymbol* fn, CallInfo& info) {
  FnSymbol* retval = NULL;

  for_formals(formal, fn) {
    if (BlockStmt* block = formal->variableExpr) {
      int numCopies = info.actuals.n - fn->numFormals() + 1;

      if (numCopies > 0) {
        SymbolMap substitutions;

        retval = fn->copy(&substitutions);
        retval->addFlag(FLAG_INVISIBLE_FN);

        fn->defPoint->insertBefore(new DefExpr(retval));

        // newSym queries the number of varargs. Replace it with int literal.
        Symbol*    defSym     = toDefExpr(block->body.tail)->sym;
        Symbol*    newSym     = substitutions.get(defSym);
        VarSymbol* nVar       = new_IntSymbol(numCopies);
        SymExpr*   newSymExpr = new SymExpr(nVar);

        newSymExpr->astloc = newSym->astloc;

        newSym->defPoint->replace(newSymExpr);

        subSymbol(retval, newSym, nVar);

        formal = toArgSymbol(substitutions.get(formal));

        expandVarArgsFormal(retval, formal, numCopies);
      }

      // It is certain that there is just one var-arg set to handle
      break;
    }
  }

  return retval;
}

static void expandVarArgsFormal(FnSymbol*  fn, ArgSymbol* formal, int n) {
  SET_LINENO(formal);

  Formals formals = insertFormalsForVarArg(formal, n);

  fn->addFlag(FLAG_EXPANDED_VARARGS);

  if (fn->where != NULL) {
    expandVarArgsWhere(fn, formal, formals);
  }

  expandVarArgsBody(fn, formal, formals);

  formal->defPoint->remove();
}

static Formals insertFormalsForVarArg(ArgSymbol* varArg, int n) {
  Formals retval(n);

  for (int i = 0; i < n; i++) {
    DefExpr*   newArgDef = varArg->defPoint->copy();
    ArgSymbol* newFormal = toArgSymbol(newArgDef->sym);

    newFormal->variableExpr = NULL;
    newFormal->name         = astr("_e", istr(i), "_", varArg->name);
    newFormal->cname        = astr("_e", istr(i), "_", varArg->cname);

    varArg->defPoint->insertBefore(newArgDef);

    retval[i] = newFormal;
  }

  return retval;
}

static void expandVarArgsWhere(FnSymbol*      fn,
                               ArgSymbol*     formal,
                               const Formals& varargs) {
  int n = static_cast<int>(varargs.size());

  if (needVarArgTupleAsWhole(fn->where, n, formal) == true) {
    VarSymbol* var        = buildTupleVariable(formal);
    CallExpr*  buildTuple = buildTupleCall(formal, varargs);

    fn->where->insertAtHead(new CallExpr(PRIM_MOVE, var, buildTuple));
    fn->where->insertAtHead(new DefExpr(var));

    subSymbol(fn->where, formal, var);
  } else {
    substituteVarargTupleRefs(fn->where, formal, varargs);
  }
}

static void expandVarArgsBody(FnSymbol*      fn,
                              ArgSymbol*     formal,
                              const Formals& formals) {
  int        n         = static_cast<int>(formals.size());
  VarSymbol* var       = buildTupleVariable(formal);
  bool       needTuple = true;

  if (PartialCopyData* pci = getPartialCopyData(fn)) {
    needTuple = handleCopyData(pci, var, formal, formals);

  } else {
    needTuple = needVarArgTupleAsWhole(fn->body, n, formal);
  }

  if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
    needTuple = true;
  }

  if (needTuple == true) {
    CallExpr*  tupleCall = buildTupleCall(formal, formals);

    // Noakes 2016/02/01.
    // Enable special handling for strings with blank intent

    // If the original formal is a string with blank intent then the
    // new formals appear to be strings with blank intent.  However
    // the resolver is about to update these to be const ref intent.
    //
    // Currently this will cause the tuple to appear to be a tuple
    // ref(String) which is not supported.
    //
    // Insert local copies that can be used for the tuple

    if (isString(formal->type) == true && formal->intent == INTENT_BLANK) {
      DefExpr* varDefn = new DefExpr(var);
      Expr*    tail    = NULL;

      // Insert the new locals
      for (int i = 0; i < n; i++) {
        const char* localName = astr("_e", istr(i + n), "_", formal->name);
        VarSymbol*  local     = newTemp(localName, formal->type);
        DefExpr*    defn      = new DefExpr(local);

        if (tail == NULL) {
          fn->insertAtHead(defn);
        } else {
          tail->insertAfter(defn);
        }

        tail = defn;
      }

      // Insert the definition for the tuple
      tail->insertAfter(varDefn);
      tail = varDefn;

      // Insert the copies from the blank-intent strings to the locals
      // Update the arguments to the tuple call
      for (int i = 1; i <= n; i++) {
        DefExpr*  localDefn = toDefExpr(fn->body->body.get(i));
        Symbol*   local     = localDefn->sym;
        SymExpr*  localExpr = new SymExpr(local);

        SymExpr*  tupleArg  = toSymExpr(tupleCall->get(i + 1));
        SymExpr*  copyArg   = tupleArg->copy();

        CallExpr* moveExpr  = new CallExpr(PRIM_MOVE, localExpr, copyArg);

        tupleArg->setSymbol(local);

        tail->insertAfter(moveExpr);
        tail = moveExpr;
      }

      tail->insertAfter(new CallExpr(PRIM_MOVE, var, tupleCall));

    } else {
      fn->insertAtHead(new CallExpr(PRIM_MOVE, var, tupleCall));
      fn->insertAtHead(new DefExpr(var));
    }

    if (PartialCopyData* pci = getPartialCopyData(fn)) {
      // If this is a partial copy,
      // store the mapping for substitution later.
      pci->partialCopyMap.put(formal, var);

    } else {
      // Otherwise, do the substitution now.
      subSymbol(fn->body, formal, var);
    }

    // Insert epilogue temps
    if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
      int i = 0;

      for_actuals(actual, tupleCall) {
        // Skip the tuple count
        if (i > 0) {
          VarSymbol* tmp    = newTemp("_varargs_tmp_");

          CallExpr*  elem   = new CallExpr(var, new_IntSymbol(i));
          CallExpr*  move   = new CallExpr(PRIM_MOVE, tmp,            elem);

          CallExpr*  assign = new CallExpr("=",       actual->copy(), tmp);

          fn->insertIntoEpilogue(new DefExpr(tmp));
          fn->insertIntoEpilogue(move);
          fn->insertIntoEpilogue(assign);
        }

        i++;
      }
    }

  } else {
    substituteVarargTupleRefs(fn->body, formal, formals);
  }
}

static bool handleCopyData(PartialCopyData* pci,
                           VarSymbol*       var,
                           ArgSymbol*       formal,
                           const Formals&   varargs) {
  int  n         = static_cast<int>(varargs.size());
  int  index     = pci->partialCopyMap.n - 1;
  bool gotFormal = false;
  bool retval    = false;

  while (index >= 0 && gotFormal == false) {
    SymbolMapElem& mapElem = pci->partialCopyMap.v[index];

    if (mapElem.value == formal) {
      retval = needVarArgTupleAsWhole(pci->partialCopySource->body,
                                      n,
                                      toArgSymbol(mapElem.key));

      if (retval == true) {
        mapElem.value = var;
      } else {
        // Rely on mapElem.value==formal to replace it away
        // in finalizeCopy().  This assumes a single set of varargs.
        pci->varargOldFormal  = formal;
        pci->varargNewFormals = varargs;
      }

      gotFormal = true;
    } else {
      index     = index - 1;
    }
  }

  INT_ASSERT(gotFormal == true);

  return retval;
}

static VarSymbol* buildTupleVariable(ArgSymbol* formal) {
  VarSymbol* retval = new VarSymbol(formal->name);

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    retval->addFlag(FLAG_TYPE_VARIABLE);
  } else {
    retval->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  return retval;
}

static CallExpr* buildTupleCall(ArgSymbol* formal, const Formals& formals) {
  int       n      = static_cast<int>(formals.size());
  CallExpr* retval = NULL;

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    retval = new CallExpr("_type_construct__tuple");
  } else {
    retval = new CallExpr("_construct__tuple");
  }

  retval ->insertAtTail(new_IntSymbol(n));

  for (int i = 0; i < n; i++) {
    retval->insertAtTail(formals[i]);
  }

  return retval;
}

//
// Does 'block' contain use(s) of the vararg tuple 'formal'
// that require 'formal' as a whole tuple?
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
static bool needVarArgTupleAsWhole(BlockStmt* block,
                                   int        numArgs,
                                   ArgSymbol* formal) {
  std::vector<SymExpr*> symExprs;
  bool                  retval = false;

  collectSymExprs(block, symExprs);

  for (size_t i = 0; i < symExprs.size() && retval == false; i++) {
    SymExpr* se = symExprs[i];

    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        if (parent->isPrimitive(PRIM_TUPLE_EXPAND) == false &&
            varargAccessIndex(se, parent, numArgs) == 0     &&
            isVarargSizeExpr (se, parent)          == false) {
          retval =  true;
        }

      } else {
        retval = true;
      }
    }
  }

  // The 'formal' is used only in "does not require" cases.
  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void replaceVarargAccess(CallExpr*  parent,
                                SymExpr*   se,
                                ArgSymbol* replFormal,
                                SymbolMap& tempReps);

void substituteVarargTupleRefs(FnSymbol* fn, const PartialCopyData* pci) {
  substituteVarargTupleRefs(fn->body,
                            pci->varargOldFormal,
                            pci->varargNewFormals);
}

//
// Replace, in 'ast', uses of the vararg tuple 'formal'
// with references to individual formals.
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
static void substituteVarargTupleRefs(BlockStmt*     block,
                                      ArgSymbol*     formal,
                                      const Formals& varargs) {
  std::vector<SymExpr*> symExprs;
  SymbolMap             tempReps;
  int                   numArgs    = static_cast<int>(varargs.size());

  collectSymExprs(block, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        SET_LINENO(parent);

        if (parent->isPrimitive(PRIM_TUPLE_EXPAND) == true) {
          // Replace 'parent' with a sequence of individual args.
          for_vector(ArgSymbol, arg, varargs) {
            parent->insertBefore(new SymExpr(arg));
          }

          parent->remove();

        } else if (int idxNum = varargAccessIndex(se, parent, numArgs)) {
          INT_ASSERT(1 <= idxNum && idxNum <= numArgs);

          ArgSymbol* replFormal = varargs[idxNum - 1];

          replaceVarargAccess(parent, se, replFormal, tempReps);

        } else if (isVarargSizeExpr(se, parent) == true) {
          parent->replace(new SymExpr(new_IntSymbol(numArgs)));

        } else {
          INT_ASSERT(false);
        }

      } else {
        INT_ASSERT(false);
      }

    } else if (Symbol* replmt = tempReps.get(se->symbol())) {
      SET_LINENO(se);

      se->replace(new SymExpr(replmt));
    }
  }
}


//
// 'parent' corresponds to tuple(i); replace it with 'replFormal'.
//
// If 'parent' is the source of a prim_move into a temp 'dest',
// then eliminate 'dest' altogether, replacing its uses with 'replFormal'.
//
// Q: Why not just keep move(dest,replFormal), which is simpler?
// A: because 'move' does not preserv replFormal's ref-ness and const-ness,
// so we will get errors when using dest on the l.h.s. and miss errors
// when dest or deref(dest) is modified and it shouldn't.
// Note that we are here before the code is resolved, so we do not
// necessarily know whether 'replFormal' is a const and/or a ref.
// To keep 'dest' around, instead of move we probably need to add
// the AST equivalent of Chapel's "ref dest = replFormal;".
//
static void replaceVarargAccess(CallExpr*  parent,
                                SymExpr*   se,
                                ArgSymbol* replFormal,
                                SymbolMap& tempReps) {
  if (CallExpr* move = toCallExpr(parent->parentExpr)) {
    if (move->isPrimitive(PRIM_MOVE)) {
      Symbol* dest = toSymExpr(move->get(1))->symbol();

      if (dest->hasFlag(FLAG_TEMP)) {
        // Remove the def of 'dest'.
        dest->defPoint->remove();
        move->remove();
        // Uses of 'dest' will come later. We replace them with 'replFormal'
        // in substituteVarargTupleRefs() using 'tempReps'.
        tempReps.put(dest, replFormal);
        return;
      }
    }
  }

  // Otherwise, just replace 'parent'.
  parent->replace(new SymExpr(replFormal));
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Verifies the usage of this candidate function
bool checkResolveFormalsWhereClauses(ResolutionCandidate* currCandidate) {
  /*
   * A derived generic type will use the type of its parent,
   * and expects this to be instantiated before it is.
   */
  resolveFormals(currCandidate->fn);

  int coindex = -1;

  for_formals(formal, currCandidate->fn) {
    if (Symbol* actual = currCandidate->formalIdxToActual.v[++coindex]) {
      if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        return false;
      }

      bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) || formal->intent == INTENT_PARAM;
      if (!canDispatch(actual->type, actual, formal->type, currCandidate->fn, NULL, formalIsParam)) {
        return false;
      }
    }
  }

  //
  // Evaluate where clauses
  //
  if (!evaluateWhereClause(currCandidate->fn)) {
    return false;
  }

  return true;
}


// Verify that the generic formals are matched correctly
bool checkGenericFormals(ResolutionCandidate* currCandidate) {
  /*
   * Early rejection of generic functions.
   */
  int coindex = 0;

  for_formals(formal, currCandidate->fn) {
    if (formal->type != dtUnknown) {
      if (Symbol* actual = currCandidate->formalIdxToActual.v[coindex]) {
        if (actual->hasFlag(FLAG_TYPE_VARIABLE) != formal->hasFlag(FLAG_TYPE_VARIABLE)) {
          return false;
        }

        if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
          Type* vt  = actual->getValType();
          Type* st  = actual->type->scalarPromotionType;
          Type* svt = (vt) ? vt->scalarPromotionType : NULL;

          if (!canInstantiate(actual->type, formal->type) &&

              (!vt  || !canInstantiate(vt,  formal->type)) &&
              (!st  || !canInstantiate(st,  formal->type)) &&
              (!svt || !canInstantiate(svt, formal->type))) {

            return false;

          }
        } else {
          bool formalIsParam = formal->hasFlag(FLAG_INSTANTIATED_PARAM) ||
                               formal->intent == INTENT_PARAM;

          if (!canDispatch(actual->type,
                           actual,
                           formal->type,
                           currCandidate->fn,
                           NULL,
                           formalIsParam)) {
            return false;
          }
        }
      }
    }

    ++coindex;
  }

  return true;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// Assuming 'parent' is an indexing expression into the tuple 'se',
// return the index, between 1 and numArgs. Otherwise return 0.
//
static int varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs) {
  if (se == parent->baseExpr) {
    if(parent->numActuals() == 1) {
      VarSymbol* idxVar = toVarSymbol(toSymExpr(parent->get(1))->symbol());
      if (idxVar && idxVar->immediate &&
          idxVar->immediate->const_kind == NUM_KIND_INT) {
        int idxNum = idxVar->immediate->int_value();
        if (1 <= idxNum && idxNum <= numArgs)
          return idxNum;
        // report an "index out of bounds" error otherwise?
      }
    }
  }

  return 0;
}


//
// Is 'parent' the call 'se'.size?
//
static bool isVarargSizeExpr(SymExpr* se, CallExpr* parent) {
  if (parent->isNamed("size")) {
    // Is 'parent' a method call?
    if (parent->numActuals() == 2) {
      if (SymExpr* firstArg = toSymExpr(parent->get(1))) {
        if (firstArg->symbol()->type == dtMethodToken) {
          // If this fails, ensure that isVarargSizeExpr still works correctly.
          INT_ASSERT(parent->get(2) == se);
          return true;
        }
      }
    }
  }

  return false;
}
