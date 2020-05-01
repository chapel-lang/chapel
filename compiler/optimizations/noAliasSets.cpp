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

#include "passes.h"

#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "CForLoop.h"
#include "dominator.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "ParamForLoop.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "optimizations.h"
#include "timer.h"
#include "WhileStmt.h"

#include <algorithm>
#include <set>
#include <queue>
#include <utility>


/* This file includes analysis that determines non-trivial
   no-alias relationships. It is particularly focused on:
     * reference arguments
     * arrays

   The work here consists of two basic parts. First, there is an
   interprocedural reference alias analysis that follows
   an algorithm described in
   Allen & Kennedy "Optimizing Compilers for Modern Architectures".
   This analysis determines when reference arguments may not alias
   each other. It concludes by storing the result in PRIM_NO_ALIAS_SET
   calls at the start of each function where it determined something.

   Second, there is a per-function portion that starts from the above
   PRIM_NO_ALIAS_SET calls and propagates these, in the case of an
   array. If two arrays come from separate (value) variables or from
   references don't alias, this portion infers that the following also
   don't alias each other (for different arrays):
     - access to array header fields
     - result of dsiAccess
     - result of PRIM_ARRAY_GET
   These properties are not encoded otherwise (e.g. in TBAA metadata)
   and are specific to Chapel. It's unlikely that generic alias analysis
   would determine these not to alias.

   This second portion adds PRIM_NO_ALIAS_SET for local value variables
   and PRIM_COPIES_NO_ALIAS_SET calls to indicate when a value has the
   same no-alias annotations as another value.

   Initially these primitives are only used in --llvm code generation
   in order to generate !alias.scope and !noalias metadata.
 */

#define MAX_ANALYZED_FORMALS_PER_FUNCTION 100


static
void addNoAliasSetForFormal(ArgSymbol* arg,
                            std::vector<ArgSymbol*> notAliasingThese) {
  SET_LINENO(arg);

  CallExpr* c = new CallExpr(PRIM_NO_ALIAS_SET, new SymExpr(arg));

  for_vector(ArgSymbol, other, notAliasingThese) {
    c->insertAtTail(new SymExpr(other));
  }

  FnSymbol* fn = arg->getFunction();
  fn->body->insertAtHead(c);
}

static
bool isNonAliasingArrayImplType(Type* t) {
  // Array views are marked with this flag because they
  // can alias other arrays.
  if (t->symbol->hasFlag(FLAG_ALIASING_ARRAY))
    return false;

  // Non-array view array classes
  if (isArrayImplType(t))
    return true;

  return false;
}

static
bool isNonAliasingArrayType(Type* t) {
  if (AggregateType* at = toAggregateType(t)) {
    if (isRecordWrappedType(at)) {
      Symbol* instanceField = at->getField("_instance", false);

      if (instanceField)
        return isNonAliasingArrayImplType(instanceField->type);
    }
  }

  return false;
}

static
bool isRuntimeType(Symbol* sym) {
  if (sym->hasFlag(FLAG_TEMP) && 0 == strcmp("_runtime_type_tmp_", sym->name))
    return true;

  return false;
}

static
bool shouldAddNoAliasSetForVariable(Symbol* var) {
  if (!var->isRef() &&
      isNonAliasingArrayType(var->type) &&
      !isRuntimeType(var))
    return true;

  return false;
}

static
bool isUsedInArrayGet(Symbol* sym) {
  for_SymbolSymExprs(se, sym) {
    if (CallExpr* call = toCallExpr(se->parentExpr))
      if (call->isPrimitive(PRIM_ARRAY_GET) ||
          call->isPrimitive(PRIM_ARRAY_GET_VALUE))
        if (se == call->get(1))
          return true;
  }

  return false;
}

static
void reportAliases(std::map<Symbol*, CallExpr*> &noAliasCallsForSymbol) {
  std::map<Symbol*, CallExpr*>::iterator it;
  std::map<Symbol*, CallExpr*>::iterator it2;

  if (developer) {
    for (it = noAliasCallsForSymbol.begin();
         it != noAliasCallsForSymbol.end();
         ++it) {
      Symbol* sym = it->first;
      CallExpr* call = it->second;
      while (call->isPrimitive(PRIM_COPIES_NO_ALIAS_SET)) {
        Symbol* fromSym = toSymExpr(call->get(2))->symbol();
        call = noAliasCallsForSymbol[fromSym];
        INT_ASSERT(call);
      }

      Symbol* scope = toSymExpr(call->get(1))->symbol();

      printf(" %s (%i) scope %s (%i)\n",
             sym->name, sym->id, scope->name, scope->id);

      bool first = true;
      for_actuals(actual, call) {
        if (!first) {
          Symbol* noAliasSym = toSymExpr(actual)->symbol();
          printf("   noalias scope %s (%i)\n",
                 noAliasSym->name, noAliasSym->id);
        }
        first = false;
      }
    }
  }

  // Print out doesn't-alias pairs
  for (it = noAliasCallsForSymbol.begin();
       it != noAliasCallsForSymbol.end();
       ++it) {
    Symbol* sym = it->first;
    CallExpr* call = it->second;

    while (call->isPrimitive(PRIM_COPIES_NO_ALIAS_SET)) {
      Symbol* fromSym = toSymExpr(call->get(2))->symbol();
      call = noAliasCallsForSymbol[fromSym];
      INT_ASSERT(call);
    }

    for (it2 = noAliasCallsForSymbol.begin();
         it2 != noAliasCallsForSymbol.end();
         ++it2) {
      Symbol* otherSym = it2->first;
      CallExpr* otherCall = it2->second;

      while (otherCall->isPrimitive(PRIM_COPIES_NO_ALIAS_SET)) {
        Symbol* fromSym = toSymExpr(otherCall->get(2))->symbol();
        otherCall = noAliasCallsForSymbol[fromSym];
        INT_ASSERT(otherCall);
      }

      // Only consider each pair once, don't consider same pair
      if (sym != otherSym && sym->id < otherSym->id) {
        // Can they alias?
        // Is one scope in the other's no-alias list?
        Symbol* scope = toSymExpr(call->get(1))->symbol();
        Symbol* otherScope = toSymExpr(otherCall->get(1))->symbol();

        bool symTemp = (sym->hasFlag(FLAG_TEMP) && !isArgSymbol(sym));
        bool otherTemp = (otherSym->hasFlag(FLAG_TEMP) &&
                          !isArgSymbol(otherSym));

        bool symInArrayGet = isUsedInArrayGet(sym);
        bool otherInArrayGet = isUsedInArrayGet(otherSym);

        bool sameScope = (scope == otherScope);

        // Is one in the other's no-alias scope list?
        bool otherScopeInCall = false;
        bool scopeInOtherCall = false;

        bool first;
        first = true;
        for_actuals(actual, call) {
          if (!first) {
            Symbol* noAliasSym = toSymExpr(actual)->symbol();
            if (otherScope == noAliasSym)
              otherScopeInCall = true;
          }
          first = false;
        }
        first = true;
        for_actuals(actual, otherCall) {
          if (!first) {
            Symbol* noAliasSym = toSymExpr(actual)->symbol();
            if (scope == noAliasSym)
              scopeInOtherCall = true;
          }
          first = false;
        }

        // shouldn't no-alias when containing same scope
        if (sameScope)
          INT_ASSERT(!otherScopeInCall && !scopeInOtherCall);

        // analysis should be symmetric
        INT_ASSERT(otherScopeInCall == scopeInOtherCall);

        // symTemp otherSymTemp
        if (otherScopeInCall || scopeInOtherCall) {
          const char* symName = NULL;
          const char* otherSymName = NULL;

          if (developer) {
            symName = sym->name;
            otherSymName = otherSym->name;
          } else {
            if (sym->hasFlag(FLAG_RETARG))
              symName = "return argument";
            else if (!symTemp)
              symName = sym->name;

            if (otherSym->hasFlag(FLAG_RETARG))
              otherSymName = "return argument";
            else if (!otherTemp)
              otherSymName = otherSym->name;

            if (symName == NULL && otherSymName == NULL)
              if (symInArrayGet && otherInArrayGet)
                symName = otherSymName = "<array get pointer>";
          }

          if (symName && otherSymName) {
            if (sym->isRef() && otherSym->isRef())
              printf("  %s no ref alias %s\n", symName, otherSymName);
            else
              printf("  %s no alias %s\n", symName, otherSymName);
          }
        }
      }
    }
  }
}


static
void addNoAliasSetsInFn(FnSymbol* fn) {
  std::map<Symbol*, CallExpr*> noAliasCallsForSymbol;
  CallExpr* lastNoAliasCall = NULL;
  CallExpr* noop = NULL;

  // Look for aliasing information about any array formals
  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);
  for_vector(CallExpr, call, calls) {
    if (call->isPrimitive(PRIM_NO_ALIAS_SET)) {
      SymExpr* se = toSymExpr(call->get(1));
      if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
        noAliasCallsForSymbol[arg] = call;
        lastNoAliasCall = call;
      }
    }
  }

  if (lastNoAliasCall == NULL) {
    SET_LINENO(fn);
    noop = new CallExpr(PRIM_NOOP);
    lastNoAliasCall = noop;
    fn->body->insertAtHead(lastNoAliasCall);
  }

  std::vector<VarSymbol*> localArrayVariables;
  // Find local array variable declarations
  // Don't consider:
  //   * calls to functions with FLAG_RETURNS_ALIASING_ARRAY
  //     (but these should already handled by ruling out array views)
  //   * array temporaries that are just PRIM_MOVE/PRIM_ASSIGN'd to
  //     create them.
  {
    std::vector<DefExpr*> defs;
    collectDefExprs(fn, defs);
    for_vector(DefExpr, def, defs) {
      if (VarSymbol* var = toVarSymbol(def->sym)) {
        if (shouldAddNoAliasSetForVariable(var)) {

          // What created this array? Is it a PRIM_MOVE/PRIM_ASSIGN?
          // That pattern indicates compiler implementation oddities
          // rather than a new array.

          bool createdByMove = false;
          for_SymbolDefs(defSe, var) {
            if (CallExpr* call = toCallExpr(defSe->parentExpr)) {
              if (call->isPrimitive(PRIM_MOVE) ||
                  call->isPrimitive(PRIM_ASSIGN)) {
                if (defSe == call->get(1))
                  createdByMove = true;
              } else {
                FnSymbol* fn = call->resolvedOrVirtualFunction();
                if (fn && fn->hasFlag(FLAG_RETURNS_ALIASING_ARRAY))
                  INT_FATAL("Aliasing arrays should have different type");
                  // Should have been ruled out by
                  // shouldAddNoAliasSetForVariable (check for array views)
              }
            }
          }

          if (createdByMove) {
            // These don't count.
            // Real array initializations happen by _retArg
          } else {
            // Add a no-aliasing call
            localArrayVariables.push_back(var);
          }
        }
      }
    }
  }

  // Add PRIM_NO_ALIAS_SET for local array vars, populate map
  for_vector(VarSymbol, var, localArrayVariables) {
    DefExpr* def = var->defPoint;
    SET_LINENO(def);
    CallExpr* c = new CallExpr(PRIM_NO_ALIAS_SET, new SymExpr(var));
    lastNoAliasCall->insertAfter(c);
    noAliasCallsForSymbol[var] = c;
    lastNoAliasCall = c;
  }

  // Now fill out the no-alias sets. Local array variables can't
  // alias any other local array variable or argument
  // unless one of the types is an array view.
  for_vector(VarSymbol, var, localArrayVariables) {
    // Enhance the no-aliasing call
    DefExpr* def = var->defPoint;
    SET_LINENO(def);
    CallExpr* c = noAliasCallsForSymbol[var];
    INT_ASSERT(c);
    // For every other thing in the map, add an entry
    //  - includes local array value variables
    //  - includes array arguments
    std::map<Symbol*, CallExpr*>::iterator it;
    for (it = noAliasCallsForSymbol.begin();
        it != noAliasCallsForSymbol.end();
        ++it) {
      Symbol* otherSym = it->first;
      if (otherSym != var) {
        if (ArgSymbol* arg = toArgSymbol(otherSym)) {
          bool isReturning = arg->originalIntent == INTENT_OUT ||
                             arg->intent == INTENT_OUT ||
                             arg->hasFlag(FLAG_RETARG);

          if (isReturning) {
            // Don't consider this arg
            //  (returning an array could cause aliasing the ret arg)
          } else {
            // Add to both the local variable and to the array
            // (since the above outer loop only considers local variables)
            c->insertAtTail(new SymExpr(otherSym));
            CallExpr* otherC = noAliasCallsForSymbol[otherSym];
            INT_ASSERT(otherC);
            otherC->insertAtTail(new SymExpr(var));
          }
        } else {
          // Other is a local variable
          // Will add the symmetric case when processing it.
          c->insertAtTail(new SymExpr(otherSym));
        }
      }
    }
  }

  // Now look for calls that return references to array elements
  // We'll mark these as having the same aliasing scope as the
  // parent array in noAliasCallsForSymbol.
  bool changed;
  do {
    changed = false;
    for_vector(CallExpr, call, calls) {
      // Code below sets these to propagate alias scopes
      Symbol* fromSym = NULL;
      Symbol* toSym = NULL;

      // Check for cases where propagation is needed
      if (call->isPrimitive(PRIM_MOVE) ||
          call->isPrimitive(PRIM_ASSIGN)) {
        Symbol* lhs = toSymExpr(call->get(1))->symbol();
        if (lhs->hasFlag(FLAG_RETARG)) {
          // Don't propagate to _retArg
        } else if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
          // Check for getting the _instance field or data field
          if (rhsCall->isPrimitive(PRIM_GET_MEMBER) ||
              rhsCall->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            Symbol* field = toSymExpr(rhsCall->get(2))->symbol();
            if (field->hasFlag(FLAG_ALIAS_SCOPE_FROM_THIS)) {
              fromSym = toSymExpr(rhsCall->get(1))->symbol();
              toSym = lhs;
            }

          // Check for getting reference to an element
          } else if (rhsCall->isPrimitive(PRIM_ARRAY_GET)) {
            if (lhs->isRef() && call->isPrimitive(PRIM_MOVE)) {
              fromSym = toSymExpr(rhsCall->get(1))->symbol();
              toSym = lhs;
            }
          } else if (FnSymbol* rhsFn = rhsCall->resolvedOrVirtualFunction()) {
            if (rhsFn->hasFlag(FLAG_ALIAS_SCOPE_FROM_THIS)) {
              // Find this argument
              for_formals_actuals(formal, actual, rhsCall) {
                if (formal == fn->_this) {
                  fromSym = toSymExpr(actual)->symbol();
                  toSym = lhs;
                }
              }
            }
          }
          // TODO: check for chpl_privateObjects[objectPid]
          //  -- this should propagate from the _array record
          //     from which we got the id, if present
        } else {
          // Check for copy the result of ._instance
          if (isNonAliasingArrayImplType(lhs->type)) {
            Symbol* rhs = toSymExpr(call->get(2))->symbol();
            fromSym = rhs;
            toSym = lhs;

          // Check for PRIM_MOVE/ASSIGN of array records added by opts
          } else if (isNonAliasingArrayType(lhs->type)) {
            Symbol* rhs = toSymExpr(call->get(2))->symbol();
            fromSym = rhs;
            toSym = lhs;

          }
        }
      }


      if (fromSym != NULL) {
        if (noAliasCallsForSymbol.count(fromSym)) {
          if (noAliasCallsForSymbol.count(toSym) == 0) {
            SET_LINENO(call);
            // Propagate alias scope from fromSym to toSym

            // First, collapse any copies
            CallExpr* found = noAliasCallsForSymbol[fromSym];
            INT_ASSERT(found);
            while (found->isPrimitive(PRIM_COPIES_NO_ALIAS_SET)) {
              fromSym = toSymExpr(found->get(2))->symbol();
              found = noAliasCallsForSymbol[fromSym];
              INT_ASSERT(found);
            }

            CallExpr* c = new CallExpr(PRIM_COPIES_NO_ALIAS_SET,
                                       new SymExpr(toSym),
                                       new SymExpr(fromSym));
            lastNoAliasCall->insertAfter(c);
            noAliasCallsForSymbol[toSym] = c;

            changed = true;
          }
        }
      }
    }
  } while (changed);

  if (fReportAliases) {
    if (fn->getModule()->modTag == MOD_USER) {
      // Don't produce output for compiler-generated functions
      if (developer ||
          (!fn->hasFlag(FLAG_MODULE_INIT) &&
           !fn->hasFlag(FLAG_GEN_MAIN_FUNC))) {
        printf("noAliasSets: no-aliases for function %s:\n", fn->name);
        reportAliases(noAliasCallsForSymbol);
      }
    }
  }

  if (noop)
    noop->remove();
}


static
bool isAddrTaken(Symbol* var) {
  // Only handles values
  INT_ASSERT(!var->isRef());

  for_SymbolSymExprs(se, var) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call->isPrimitive(PRIM_SET_REFERENCE) ||
          call->isPrimitive(PRIM_ADDR_OF)) {
        return true;
      } else if (call->isResolved()) {
        // Return true for non-ref passed by ref
        for_formals_actuals(formal, actual, call) {
          if (actual == se) {
            if (formal->intent & INTENT_FLAG_REF)
              return true;
          }
        }
      }
    }
  }
  return false;
}

static
bool isRefFormal(ArgSymbol* formal) {
  return (formal->intent & INTENT_FLAG_REF);
}

static
bool fnHasRefFormal(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (isRefFormal(formal))
      return true;
  }

  return false;
}

static
BitVec makeBitVec(int size) {
  BitVec ret(size);
  return ret;
}

// Sets the bit at index in the bit vector for sym.
// Like
//   map[sym].set(index)
// but creates the BitVec if it doesn't exist yet, with size bitVecSize
//
// returns true if it changed something
static
bool addAlias(std::map<Symbol*, BitVec> &map,
              int bitVecSize,
              Symbol* sym,
              int index) {

  bool changed = false;
  std::map<Symbol*, BitVec>::iterator it = map.find(sym);

  INT_ASSERT(index < bitVecSize);

  if (it == map.end()) {
    it = map.insert(std::make_pair(sym, makeBitVec(bitVecSize))).first;
    changed = true;
  }

  BitVec &bits = it->second;
  if (bits.get(index) == false) {
    bits.set(index);
    changed = true;
  }

  return changed;
}

static
bool addAliases(std::map<Symbol*, BitVec> &map,
                int bitVecSize,
                Symbol* sym,
                BitVec& from) {

  bool changed = false;
  std::map<Symbol*, BitVec>::iterator it = map.find(sym);

  if (it == map.end()) {
    it = map.insert(std::make_pair(sym, makeBitVec(bitVecSize))).first;
    changed = true;
  }

  BitVec &bits = it->second;
  BitVec toSet = from - bits;
  if (toSet.any()) {
    bits += toSet;
    changed = true;
  }

  return changed;
}


void computeNoAliasSets() {

  if (fNoInterproceduralAliasAnalysis)
    return;

  // TODO: retArg and in --
  //  these never alias any other arguments.

  // See Allen & Kennedy "Optimizing Compilers for Modern Architectures" p 571

  // These are the main results of this function

  // map from ArgSymbol -> BitVecs of size nAddrTakenGlobals
  std::map<Symbol*, BitVec> formalsAliasingGlobals;

  // set ArgSymbol where we gave up on analysis
  std::set<ArgSymbol*> formalsAliasingAnything;

  // map from FnSymbol -> BitVec of size nFormalPairs,
  //   storing pairs of arguments that can alias
  std::map<Symbol*, BitVec> fpairs;


  // reused a few times in this function
  std::vector<CallExpr*> calls;

  // First, compute global variables that have their address taken.
  // The analysis will establish when these can alias ref arguments.
  std::map<Symbol*, int> addrTakenGlobalsToIds;

  {
    int id = 1;
    forv_Vec(VarSymbol, var, gVarSymbols) {
      if (isGlobal(var) &&
          !var->isRef() &&
          isAddrTaken(var)) {
        if (addrTakenGlobalsToIds.count(var) == 0) {
          addrTakenGlobalsToIds[var] = id;
          id++;
        }
      }
    }
  }

  // Now compute the global alias sets for procedure arguments
  size_t nAddrTakenGlobals = addrTakenGlobalsToIds.size();
  nAddrTakenGlobals++; // add 1 since we count from 1

  // Compute the starting point for the sets,
  // don't worry about transitivity/propagating yet.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fnHasRefFormal(fn)) {
      if (fn->hasFlag(FLAG_EXPORT)) {
        // Assume exported functions can have formals aliasing each other
        for_formals(fnFormal, fn) {
          if (isRefFormal(fnFormal)) {
            formalsAliasingAnything.insert(fnFormal);
          }
        }
      } else {
        // Consider call sites to the function to propagate alias information
        for_SymbolSymExprs(se, fn) {
          // use of SymExpr(fn) is a call
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            // it's a call to fn
            if (fn == call->resolvedOrVirtualFunction()) {
              // Consider the actuals and formals and update.
              for_formals_actuals(fnFormal, actual, call) {
                // fnFormal is a formal from fn, which we're investigating
                if (isRefFormal(fnFormal)) {
                  // What is the actual?
                  SymExpr* actualSe = toSymExpr(actual);
                  if (VarSymbol* fromVar = toVarSymbol(actualSe->symbol())) {
                    if (fromVar->isRef()) {
                      // Give up
                      formalsAliasingAnything.insert(fnFormal);
                    } else if (isGlobal(fromVar) &&
                               addrTakenGlobalsToIds.count(fromVar) > 0) {
                      int globalId = addrTakenGlobalsToIds[fromVar];
                      // add the global to the set
                      addAlias(formalsAliasingGlobals,
                               nAddrTakenGlobals,
                               fnFormal, globalId);
                      // local *value* variables don't add to alias sets
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // Construct the binding graph, which has nodes that are formal parameters,
  // where there is an edge between f1 of p and f2 of q if
  // f1 is passed to f2 in a call q(...) inside of the body of p.
  // See Kennedy p 564

  std::map<ArgSymbol*, std::set<ArgSymbol*> > bindingGraph;

  forv_Vec(FnSymbol, p, gFnSymbols) {
    if (fnHasRefFormal(p)) {
      calls.clear();
      collectFnCalls(p, calls);

      for_vector(CallExpr, call, calls) {
        FnSymbol* q = call->resolvedOrVirtualFunction();
        INT_ASSERT(q);

        for_formals_actuals(qFormal, actual, call) {
          if (isRefFormal(qFormal)) {
            SymExpr* actualSe = toSymExpr(actual);
            if (ArgSymbol* fFormal = toArgSymbol(actualSe->symbol())) {
              if (isRefFormal(fFormal)) {
                bindingGraph[fFormal].insert(qFormal);
              }
            }
          }
        }
      }
    }
  }

  // Now use the binding graph to compute the transitive closure
  // This is inspired by the algorithm in Kennedy but uses a simpler
  // fixed-point strategy. If it becomes a performance issue looking
  // back at that book should allow a more efficient version.
  // The algorithm in the book (which the code below attempts to simplify):
  //    for each formal parameter f, clear A[f]
  //    for each call site s
  //      for each global g mapped to parameter f at s
  //        A[f] = A[f] union {g}
  //    replace every cycle in the binding graph with a single node,
  //      reducing the graph to a directed, acyclic form
  //    for each f in the reduced graph in topological order do
  //      for each formal f0 where (f0, f) is in the binding graph
  //        A[f] = A[f] union A[f0]
  //    for each cycle in the original binding graph
  //      let C be the reduced binding graph node for the cycle
  //      for each f in C
  //        A[f] = A[C]

  bool changed;
  bool lastiter;
  int niters = 1;
  int maxiters = 3;
  do {
    lastiter = niters == maxiters;
    changed = false;
    std::map<ArgSymbol*, std::set<ArgSymbol*> >::const_iterator it;

    for (it = bindingGraph.begin(); it != bindingGraph.end(); ++it) {
      ArgSymbol* f1 = it->first;
      const std::set<ArgSymbol*> &toSet = it->second;
      for_set(ArgSymbol, f2, toSet) {
        // Propagate alias information from f1 to f2

        // propagate aliases anything
        if (formalsAliasingAnything.count(f1)) {
          if (formalsAliasingAnything.count(f2) == 0) {
            formalsAliasingAnything.insert(f2);
            changed = true;
          }
        }

        // propagate aliases to globals
        {
          bool newGlobals = false;
          std::map<Symbol*, BitVec>::iterator it =
            formalsAliasingGlobals.find(f1);
          if (it != formalsAliasingGlobals.end()) {
            BitVec &fromBits = it->second;
            newGlobals = addAliases(formalsAliasingGlobals,
                                    nAddrTakenGlobals,
                                    f2, fromBits);
            if (newGlobals) {
              changed = true;
              if (lastiter) {
                formalsAliasingAnything.insert(f2);
              }
            }
          }
        }
      }
    }
    niters++;
  } while (changed && !lastiter);

  // Compute the maximum number of formals
  int maxFormals = 1;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    int nFormals = fn->numFormals();
    if (nFormals > maxFormals) {
      // If there are too many formals, don't do anything
      if (nFormals >= MAX_ANALYZED_FORMALS_PER_FUNCTION) {
        int i = 1;
        bool anyRefs = false;
        for_formals(fnFormal, fn) {
          if (i >= MAX_ANALYZED_FORMALS_PER_FUNCTION &&
              isRefFormal(fnFormal)) {
            anyRefs = true;
            formalsAliasingAnything.insert(fnFormal);
          }
          i++;
        }
        if (developer && anyRefs) {
          USR_WARN(fn, "Optimization inhibited due to many formal parameters");
        }
        maxFormals = MAX_ANALYZED_FORMALS_PER_FUNCTION;
      } else {
        maxFormals = nFormals;
      }
    }
  }
  maxFormals++; // Add 1 so we can count formals from 1

  if (maxFormals > 500) {
    USR_WARN("Too many formal parameters! Optimization is inhibited");
    maxFormals = 500;
    return;
  }

  // Now compute the set of formals aliasing other formals
  // This follows Figure 11.8 in Allen & Kennedy
  int nFormalPairs = maxFormals * maxFormals;
  std::queue<std::pair<ArgSymbol*, ArgSymbol*> > worklist;

  // for each alias introduction site, e.g. f(X, X),
  // insert the resulting formal pair in fpairs and worklist
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fnHasRefFormal(fn)) {
      // Consider call sites to the function to propagate alias information
      for_SymbolSymExprs(se, fn) {
        // use of SymExpr(fn) is a call
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          // it's a call to fn
          if (fn == call->resolvedOrVirtualFunction()) {
            // Consider the actuals and formals and update
            // Look for f(X, X)
            int formalIdx1 = 1;
            for_formals_actuals(fnFormal1, actual, call) {
              // fnFormal is a formal from fn, which we're investigating
              SymExpr* actualSe = toSymExpr(actual);
              Symbol* actualSym = actualSe->symbol();
              VarSymbol* actualVar = toVarSymbol(actualSym);
              if (actualVar && actualVar->isRef()) {
                // don't worry about ref variables; covered above
              } else if (isRefFormal(fnFormal1)) {
                // Find cases where the same argument is passed
                // This could be implemented in a different way
                // but the nested loops is much clearer
                int formalIdx2 = 1;
                for_formals_actuals(fnFormal2, actual2, call) {
                  SymExpr* actual2Se = toSymExpr(actual2);
                  Symbol* actual2Sym = actual2Se->symbol();
                  if (formalIdx1 < formalIdx2 &&
                      actualSym == actual2Sym) {
                    // The same actual was passed in positions
                    // formalIdx1 and formalIdx2

                    if (formalIdx1 < maxFormals && formalIdx2 < maxFormals) {
                      // add the pair to fpairs
                      addAlias(fpairs, nFormalPairs,
                               fn, maxFormals*formalIdx1 + formalIdx2);
                    } else {
                      INT_ASSERT(maxFormals >=
                          MAX_ANALYZED_FORMALS_PER_FUNCTION);
                    }

                    // add the pair to the worklist
                    worklist.push(std::make_pair(fnFormal1, fnFormal2));
                  }
                  formalIdx2++;
                }
              }
              formalIdx1++;
            }
          }
        }
      }
    }
  }

  // while worklist is not empty
  //   remove element from worklist, arguments f1 and f2 to procedure p
  //   for each call to q in that procedure passing both arguments f1, f2
  //     suppose q's corresponding formals are at positions f3, f4
  //     if (f3,f4) is not in fpairs[q]
  //       add (f3,f4) to fpairs[q]
  //       add (f3,f4) to the worklist

  while (!worklist.empty()) {
    std::pair<ArgSymbol*, ArgSymbol*> elt = worklist.front();
    worklist.pop();
    ArgSymbol* f1 = elt.first;
    ArgSymbol* f2 = elt.second;
    FnSymbol* p = f1->getFunction();
    INT_ASSERT(f2->getFunction() == p);
    INT_ASSERT(f1 != f2);

    calls.clear();
    collectFnCalls(p, calls);

    for_vector(CallExpr, call, calls) {
      FnSymbol* q = call->resolvedOrVirtualFunction();
      INT_ASSERT(q);
      ArgSymbol* f3 = NULL;
      int f3Idx = 0;
      ArgSymbol* f4 = NULL;
      int f4Idx = 0;

      // Figure out what f1 is passed to, and what f2 is passed to
      int formalIdx = 1;
      for_formals_actuals(qFormal, actual, call) {
        if (isRefFormal(qFormal)) {
          SymExpr* actualSe = toSymExpr(actual);
          if (ArgSymbol* actualArg = toArgSymbol(actualSe->symbol())) {
            if (actualArg == f1) {
              f3 = qFormal;
              f3Idx = formalIdx;
            }
            if (actualArg == f2) {
              f4 = qFormal;
              f4Idx = formalIdx;
            }
          }
        }
        formalIdx++;
      }
      if (f3 != NULL && f4 != NULL) {
        // compute normalized index pair (idx3 < idx4)
        int idx3 = f3Idx < f4Idx ? f3Idx : f4Idx;
        int idx4 = f3Idx < f4Idx ? f4Idx : f3Idx;
        INT_ASSERT(idx3 < idx4);

        // add the pair to fpairs
        bool added = addAlias(fpairs, nFormalPairs,
                              q, maxFormals*idx3 + idx4);
        if (added) {
          worklist.push(std::make_pair(f3, f4));
        }
      }
    }
  }

  // OK, now we have computed answers to lots of aliasing questions!

  // in function p, we should consider formal f1 potentially aliasing
  // formal f2 if:
  //   - f1 or f2 is in formalsAliasingAnything
  //   - (f1,f2) is in fpairs[p]
  //   - formalsAliasingGlobals[f1] intersect formalsAliasingGlobals[f2] != {}

  std::vector<ArgSymbol*> noAliasArgs;

  forv_Vec(FnSymbol, p, gFnSymbols) {
    if (fnHasRefFormal(p)) {
      // Are the formals independent? Do they alias each other?
      int formalIdx1 = 1;
      for_formals(formal1, p) {
        if (isRefFormal(formal1)) {
          if (formalsAliasingAnything.count(formal1)) {
            // Don't emit any alias sets for this one
          } else {
            noAliasArgs.clear();
            int formalIdx2 = 1;
            for_formals(formal2, p) {
              if (formalIdx1 != formalIdx2 && isRefFormal(formal2)) {
                // normalize the pair to idx1 < idx2
                int idx1 = formalIdx1 < formalIdx2 ? formalIdx1 : formalIdx2;
                int idx2 = formalIdx1 < formalIdx2 ? formalIdx2 : formalIdx1;
                INT_ASSERT(idx1 < idx2);
                bool pairCanAlias = false;

                // f2 is in formalsAliasingAnything
                if (formalsAliasingAnything.count(formal2))
                  pairCanAlias = true;

                // Does (idx1,idx2) appear in fpairs(p) ?
                std::map<Symbol*, BitVec>::iterator it = fpairs.find(p);
                if (it != fpairs.end()) {
                  BitVec &bits = it->second;
                  if (bits.get(maxFormals*idx1 + idx2))
                    pairCanAlias = true;
                }
                // Is formalsAliasingGlobals non-intersecting?
                std::map<Symbol*, BitVec>::iterator it2;
                it = formalsAliasingGlobals.find(formal1);
                it2 = formalsAliasingGlobals.find(formal2);
                if (it != formalsAliasingGlobals.end() &&
                    it2 != formalsAliasingGlobals.end()) {
                  BitVec &bits1 = it->second;
                  BitVec &bits2 = it2->second;
                  BitVec intersect = bits1 & bits2;
                  if (intersect.any())
                    pairCanAlias = true;
                }

                if (pairCanAlias == false) {
                  noAliasArgs.push_back(formal2);
                }
              }
              formalIdx2++;
            }
            addNoAliasSetForFormal(formal1, noAliasArgs);
          }
        }
        formalIdx1++;
      }
    }
  }


  // Lastly, construct alias sets for local array variables.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    addNoAliasSetsInFn(fn);  // map from ArgSymbol -> BitVecs of size nAddrTakenGlobals
  }
}
