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
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static void doGatherCandidates(CallInfo&                  info,
                               Vec<FnSymbol*>&            visibleFns,
                               bool                       compilerGenerated,
                               Vec<ResolutionCandidate*>& candidates);

static void filterCandidate(CallInfo&                  info,
                            FnSymbol*                  fn,
                            Vec<ResolutionCandidate*>& candidates);

static void filterCandidate(CallInfo&                  info,
                            ResolutionCandidate*       currCandidate,
                            Vec<ResolutionCandidate*>& candidates);

static void filterConcrete(CallInfo&                  info,
                           ResolutionCandidate*       currCandidate,
                           Vec<ResolutionCandidate*>& candidates);

static void resolveTypeConstructor(CallInfo& info,
                                   FnSymbol* fn);

static void filterGeneric(CallInfo&                  info,
                          ResolutionCandidate*       currCandidate,
                          Vec<ResolutionCandidate*>& candidates);

static int  varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs);

static bool isVarargSizeExpr (SymExpr* se, CallExpr* parent);

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
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void handleSymExprInExpandVarArgs(FnSymbol*  workingFn,
                                         ArgSymbol* formal,
                                         SymExpr*   sym);

static bool needVarArgTupleAsWhole(BlockStmt* ast,
                                   int        numArgs,
                                   ArgSymbol* formal);

FnSymbol* expandIfVarArgs(FnSymbol* origFn, CallInfo& info) {
  int       numActuals     = info.actuals.n;
  bool      genericArgSeen = false;
  FnSymbol* workingFn      = origFn;

  SymbolMap substitutions;

  static Map<FnSymbol*,Vec<FnSymbol*>*> cache;

  // check for cached stamped out function
  if (Vec<FnSymbol*>* cfns = cache.get(origFn)) {
    forv_Vec(FnSymbol, cfn, *cfns) {
      if (cfn->numFormals() == numActuals) return cfn;
    }
  }

  for_formals(formal, origFn) {

    if (workingFn != origFn) {
      formal = toArgSymbol(substitutions.get(formal));
    }

    if (!genericArgSeen &&
        formal->variableExpr &&
        !isDefExpr(formal->variableExpr->body.tail)) {
      resolveBlockStmt(formal->variableExpr);
    }

    /*
     * Set genericArgSeen to true if a generic argument appears before the
     * argument with the variable expression.
     */

    // INT_ASSERT(arg->type);
    // Adding 'ref' intent to the "ret" arg of
    //  inline proc =(ref ret:syserr, x:syserr) { __primitive("=", ret, x); }
    // in SysBasic.chpl:150 causes a segfault.
    // The addition of the arg->type test in the following conditional is a
    // workaround.
    // A better approach would be to add a check that each formal of a
    // function has a type (if that can be expected) and then fix the
    // fault where it occurs.
    if (formal->type && formal->type->symbol->hasFlag(FLAG_GENERIC)) {
      genericArgSeen = true;
    }

    if (!formal->variableExpr) {
      continue;
    }

    // Handle unspecified variable number of arguments.
    if (DefExpr* def = toDefExpr(formal->variableExpr->body.tail)) {
      // This assumes a single set of varargs.
      int numCopies = numActuals - workingFn->numFormals() + 1;
      if (numCopies <= 0) {
        if (workingFn != origFn) delete workingFn;
        return NULL;
      }

      if (workingFn == origFn) {
        workingFn = origFn->copy(&substitutions);
        INT_ASSERT(! workingFn->hasFlag(FLAG_RESOLVED));
        workingFn->addFlag(FLAG_INVISIBLE_FN);

        origFn->defPoint->insertBefore(new DefExpr(workingFn));

        formal = static_cast<ArgSymbol*>(substitutions.get(formal));
      }

      // newSym queries the number of varargs. Replace it with int literal.
      Symbol*  newSym     = substitutions.get(def->sym);
      SymExpr* newSymExpr = new SymExpr(new_IntSymbol(numCopies));
      newSymExpr->astloc = newSym->astloc;
      newSym->defPoint->replace(newSymExpr);

      subSymbol(workingFn, newSym, new_IntSymbol(numCopies));

      handleSymExprInExpandVarArgs(workingFn, formal, newSymExpr);
      genericArgSeen = false;

    } else if (SymExpr* sym = toSymExpr(formal->variableExpr->body.tail)) {

      handleSymExprInExpandVarArgs(workingFn, formal, sym);

    } else if (!workingFn->hasFlag(FLAG_GENERIC)) {
      INT_FATAL("bad variableExpr");
    }
  }

  Vec<FnSymbol*>* cfns = cache.get(origFn);
  if (cfns == NULL) {
    cfns = new Vec<FnSymbol*>();
  }
  cfns->add(workingFn);
  cache.put(origFn, cfns);

  return workingFn;
}


/** Common code for multiple paths through expandVarArgs.
 *
 * This code handles the case where the number of varargs are known at compile
 * time.  It inserts the necessary code to copy the values into and out of the
 * varargs tuple.
 */
static void handleSymExprInExpandVarArgs(FnSymbol*  workingFn,
                                         ArgSymbol* formal,
                                         SymExpr*   sym) {
  // Handle specified number of variable arguments.
  // sym->symbol() is not a VarSymbol e.g. in: proc f(param n, v...n)
  if (VarSymbol* nVar = toVarSymbol(sym->symbol())) {
    if (nVar->type == dtInt[INT_SIZE_DEFAULT] && nVar->immediate) {
      workingFn->addFlag(FLAG_EXPANDED_VARARGS);

      SET_LINENO(formal);
      VarSymbol* var       = new VarSymbol(formal->name);
      int        n         = nVar->immediate->int_value();
      CallExpr*  tupleCall = NULL;

      // MPF 2016-10-02
      // This code used to be run for _build_tuple
      // and _build_tuple_always_allow_ref but now
      // only runs for var-args functions.

      std::vector<ArgSymbol*> varargFormals(n);
      if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true)
        tupleCall = new CallExpr("_type_construct__tuple");
      else {
        tupleCall = new CallExpr("_construct__tuple");
        // _construct__tuple now calls initCopy, so var
        // needs to be auto-destroyed.
        var->addFlag(FLAG_INSERT_AUTO_DESTROY);
      }

      tupleCall->insertAtTail(new_IntSymbol(n));

      for (int i = 0; i < n; i++) {
        DefExpr*   newArgDef = formal->defPoint->copy();
        ArgSymbol* newFormal = toArgSymbol(newArgDef->sym);

        newFormal->variableExpr = NULL;
        newFormal->name         = astr("_e", istr(i), "_", formal->name);
        newFormal->cname        = astr("_e", istr(i), "_", formal->cname);

        tupleCall->insertAtTail(new SymExpr(newFormal));
        formal->defPoint->insertBefore(newArgDef);
        varargFormals[i] = newFormal;
      }

      bool needTupleInBody = true; //avoid "may be used uninitialized" warning

      // Replace mappings to the old formal with mappings to the new variable.
      if (PartialCopyData* pci = getPartialCopyInfo(workingFn)) {
        bool gotFormal = false; // for assertion only
        for (int index = pci->partialCopyMap.n; --index >= 0;) {
          SymbolMapElem& mapElem = pci->partialCopyMap.v[index];

          if (mapElem.value == formal) {
            gotFormal = true;
            needTupleInBody =
              needVarArgTupleAsWhole(pci->partialCopySource->body, n,
                                     toArgSymbol(mapElem.key));

            if (needTupleInBody) {
              mapElem.value = var;
            } else {
              // We will rely on mapElem.value==formal to replace it away
              // in finalizeCopy().  This assumes a single set of varargs.
              pci->varargOldFormal = formal;
              pci->varargNewFormals = varargFormals;
            }

            break;
          }
        }
        // If !gotFormal, still need to compute needTupleInBody.
        // What to pass for 'formal' argument? Or maybe doesn't matter?
        INT_ASSERT(gotFormal);
      } else {
        needTupleInBody = needVarArgTupleAsWhole(workingFn->body, n, formal);
      }

      if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        var->addFlag(FLAG_TYPE_VARIABLE);
      }

      // This is needed regardless of needTupleInBody...
      if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
        // ... however, we need temporaries even if !needTupleInBody.
        // Creating one temporary per formal is left as future work.
        // A challenge is whether varargFormals should contain these
        // per-formal temporaries instead of the formals, in this case.
        needTupleInBody = true;

        int i = 0;

        for_actuals(actual, tupleCall) {
          // Skip the tuple count
          if (i > 0) {
            VarSymbol* tmp    = newTemp("_varargs_tmp_");

            CallExpr*  elem   = new CallExpr(var, new_IntSymbol(i));
            CallExpr*  move   = new CallExpr(PRIM_MOVE, tmp,            elem);

            CallExpr*  assign = new CallExpr("=",       actual->copy(), tmp);

            workingFn->insertIntoEpilogue(new DefExpr(tmp));
            workingFn->insertIntoEpilogue(move);
            workingFn->insertIntoEpilogue(assign);
          }

          i++;
        }
      }

      if (needTupleInBody) {

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

        if (isString(formal->type) && formal->intent == INTENT_BLANK) {
          DefExpr* varDefn = new DefExpr(var);
          Expr*    tail    = NULL;

          // Insert the new locals
          for (int i = 0; i < n; i++) {
            const char* localName = astr("_e", istr(i + n), "_", formal->name);
            VarSymbol*  local     = newTemp(localName, formal->type);
            DefExpr*    defn      = new DefExpr(local);

            if (tail == NULL)
              workingFn->insertAtHead(defn);
            else
              tail->insertAfter(defn);

            tail = defn;
          }

          // Insert the definition for the tuple
          tail->insertAfter(varDefn);
          tail = varDefn;

          // Insert the copies from the blank-intent strings to the locals
          // Update the arguments to the tuple call
          for (int i = 1; i <= n; i++) {
            DefExpr*  localDefn = toDefExpr(workingFn->body->body.get(i));
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
          workingFn->insertAtHead(new CallExpr(PRIM_MOVE, var, tupleCall));
          workingFn->insertAtHead(new DefExpr(var));
        }

        if (PartialCopyData* pci = getPartialCopyInfo(workingFn)) {
          // If this is a partial copy,
          // store the mapping for substitution later.
          pci->partialCopyMap.put(formal, var);

        } else {
          // Otherwise, do the substitution now.
          subSymbol(workingFn->body, formal, var);
        }

      } else {
        // !needTupleInBody
        substituteVarargTupleRefs(workingFn->body, n, formal, varargFormals);
      }

      if (workingFn->where) {
        if (needVarArgTupleAsWhole(workingFn->where, n, formal)) {
          VarSymbol* var  = new VarSymbol(formal->name);
          CallExpr*  move = new CallExpr(PRIM_MOVE, var, tupleCall->copy());

          if (formal->hasFlag(FLAG_TYPE_VARIABLE)) {
            var->addFlag(FLAG_TYPE_VARIABLE);
          }

          workingFn->where->insertAtHead(move);
          workingFn->where->insertAtHead(new DefExpr(var));

          subSymbol(workingFn->where, formal, var);
        } else {
          // !needVarArgTupleAsWhole()
          substituteVarargTupleRefs(workingFn->where,
                                    n,
                                    formal,
                                    varargFormals);
        }
      }

      formal->defPoint->remove();

    } else {
      // Just documenting the current status.
      INT_FATAL(formal, "unexpected non-VarSymbol");
    }
  }
}

//
// Does 'ast' contain use(s) of the vararg tuple 'formal'
// that require 'formal' as a whole tuple?
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
static bool needVarArgTupleAsWhole(BlockStmt* ast,
                                   int        numArgs,
                                   ArgSymbol* formal) {
  std::vector<SymExpr*> symExprs;

  collectSymExprs(ast, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        if (parent->isPrimitive(PRIM_TUPLE_EXPAND)) {
          // (...formal) -- does not require
          continue;
        }

        if (varargAccessIndex(se, parent, numArgs) > 0) {
          // formal(i) -- does not require
          continue;
        }

        if (isVarargSizeExpr(se, parent)) {
          // formal.size -- does not require
          continue;
        }
      }
      // Something else ==> may require.
      return true;
    }
  }

  // The 'formal' is used only in "does not require" cases.
  return false;
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

//
// Replace, in 'ast', uses of the vararg tuple 'formal'
// with references to individual formals.
//
// needVarArgTupleAsWhole() and substituteVarargTupleRefs() should handle
// the exact same set of SymExprs.
//
void substituteVarargTupleRefs(BlockStmt*               ast,
                               int                      numArgs,
                               ArgSymbol*               formal,
                               std::vector<ArgSymbol*>& varargFormals) {
  std::vector<SymExpr*> symExprs;
  SymbolMap             tempReps;

  collectSymExprs(ast, symExprs);

  for_vector(SymExpr, se, symExprs) {
    if (se->symbol() == formal) {
      if (CallExpr* parent = toCallExpr(se->parentExpr)) {
        SET_LINENO(parent);

        if (parent->isPrimitive(PRIM_TUPLE_EXPAND)) {
          // Replace 'parent' with a sequence of individual args.
          for_vector(ArgSymbol, arg, varargFormals)
            parent->insertBefore(new SymExpr(arg));
          parent->remove();
          continue;
        }

        if (int idxNum = varargAccessIndex(se, parent, numArgs)) {
          INT_ASSERT(1 <= idxNum && idxNum <= numArgs);
          ArgSymbol* replFormal = varargFormals[idxNum-1];
          replaceVarargAccess(parent, se, replFormal, tempReps);
          continue;
        }

        if (isVarargSizeExpr(se, parent)) {
          parent->replace(new SymExpr(new_IntSymbol(numArgs)));
          continue;
        }

      }
      // Cases not "continue"-ed above should have been ruled out
      // by needVarArgTupleAsWhole().
      INT_ASSERT(false);

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
