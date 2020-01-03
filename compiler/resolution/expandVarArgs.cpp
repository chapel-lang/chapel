/*
 * Copyright 2004-2020 Cray Inc.
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

#include "expandVarArgs.h"

#include "alist.h"
#include "astutil.h"
#include "callInfo.h"
#include "expr.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

typedef std::vector<ArgSymbol*> Formals;

static FnSymbol* expandVarArgs(FnSymbol* fn, CallInfo& info);
static void      expandVarArgsFormal(FnSymbol*  fn, ArgSymbol* varArg, int n);

static void      substituteVarargTupleRefs(BlockStmt*     block,
                                           ArgSymbol*     varArg,
                                           const Formals& varargs);

static int       varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs);

static bool      isVarargSizeExpr (SymExpr* se, CallExpr* parent);

static FnSymbol* cacheLookup(FnSymbol* fn, int       numActuals);
static void      cacheExtend(FnSymbol* fn, FnSymbol* expansion);

/************************************* | **************************************
*                                                                             *
* Consider a function that accepts a variable number of formals e.g.          *
*                                                                             *
*    proc myWriteln(x ...?k) {                                                *
*      for param i in 1 .. k do writeln(x(i));                                *
*    }                                                                        *
*                                                                             *
*    proc sum(x : int ... N) return x(1) + x(2) + . + x(N);                   *
*                                                                             *
* where N is a param.                                                         *
*                                                                             *
*                                                                             *
* Within the body of the function the vararg formal is referenced as if       *
* it is a tuple.  This suggests two broad implementation strategies:          *
*                                                                             *
*    1) Implement the formal as a tuple and build the required tuple at       *
*       the call site.                                                        *
*                                                                             *
*    2) Instantiate the function with individual formals based on existing    *
*       call sites.                                                           *
*                                                                             *
* The current implementation follows the second strategy.                     *
*                                                                             *
*                                                                             *
* An accommodation must be made for the uses of the varArg within the body.   *
* Yet again there are two strategies:                                         *
*                                                                             *
*    1) Construct the required tuple within the body of the procedure         *
*                                                                             *
*    2) Convert the references to the tuple to references to the formals      *
*                                                                             *
* The current implementation selects between these strategies based on the    *
* way in which the varArg is used.                                            *
*                                                                             *
*                                                                             *
*                                                                             *
* 2017/04/05 There are several points at which this implementation assumes    *
* there is only one set of varargs. This has historically been implicit but   *
* is now explicit.                                                            *
*                                                                             *
* It is possible that this limit may be lifted at some point based on needs   *
* and priorities.                                                             *
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

static void      expandVarArgsFixed(FnSymbol* fn, CallInfo& info);
static FnSymbol* expandVarArgsQuery(FnSymbol* fn, CallInfo& info);
static int       varArgsCount(ArgSymbol* formal, VarSymbol* nVar);

static FnSymbol* expandVarArgs(FnSymbol* fn, CallInfo& info) {
  int       numVarArgs      = 0;
  bool      isQueryVariable = false;
  FnSymbol* retval          = NULL;

  for_formals(formal, fn) {
    if (formal->variableExpr != NULL) {
      if (isDefExpr(formal->variableExpr->body.tail) == true) {
        isQueryVariable = true;
      } else if (SymExpr* se = toSymExpr(formal->variableExpr->body.tail)) {
        if (se->symbol() == gUninstantiated) {
          isQueryVariable = true;
        }
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

      } else if (! fn->isGeneric()) {
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
        retval->instantiatedFrom = fn;

        fn->defPoint->insertBefore(new DefExpr(retval));

        // newSym queries the number of varargs. Replace it with int literal.
        if (DefExpr* def = toDefExpr(block->body.tail)) {
          Symbol*    defSym     = def->sym;
          Symbol*    newSym     = substitutions.get(defSym);
          VarSymbol* nVar       = new_IntSymbol(numCopies);
          SymExpr*   newSymExpr = new SymExpr(nVar);

          newSymExpr->astloc = newSym->astloc;

          newSym->defPoint->replace(newSymExpr);

          subSymbol(retval, newSym, nVar);
        }

        formal = toArgSymbol(substitutions.get(formal));

        expandVarArgsFormal(retval, formal, numCopies);
      }

      // It is certain that there is just one var-arg set to handle
      break;
    }
  }

  return retval;
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static Formals    insertFormalsForVarArg(ArgSymbol* varArg, int n);

static void       expandVarArgsWhere(FnSymbol*      fn,
                                     ArgSymbol*     formal,
                                     const Formals& varargs);

static void       expandVarArgsLifetimeConstraints(FnSymbol* fn,
                                     ArgSymbol*     formal,
                                     const Formals& varargs);

static void       expandVarArgsBody(FnSymbol*      fn,
                                    ArgSymbol*     formal,
                                    const Formals& varargs);

static bool       handleCopyData(PartialCopyData* pci,
                                 VarSymbol*       var,
                                 ArgSymbol*       formal,
                                 const Formals&   varargs);

static bool       needVarArgTupleAsWhole(BlockStmt* block,
                                         int        numArgs,
                                         ArgSymbol* formal);

static CallExpr*  expandVarArgString(FnSymbol*      fn,
                                     VarSymbol*     var,
                                     ArgSymbol*     formal,
                                     const Formals& formals);

static VarSymbol* buildTupleVariable(ArgSymbol* formal);

static CallExpr*  buildTupleCall(ArgSymbol* formal, const Formals& formals);

static void       insertEpilogueTemps(FnSymbol*  fn,
                                      VarSymbol* var,
                                      CallExpr*  tupleCall);

static void expandVarArgsFormal(FnSymbol* fn, ArgSymbol* formal, int n) {
  SET_LINENO(formal);

  Formals formals = insertFormalsForVarArg(formal, n);

  fn->addFlag(FLAG_EXPANDED_VARARGS);

  if (fn->where != NULL) {
    expandVarArgsWhere(fn, formal, formals);
  }

  expandVarArgsLifetimeConstraints(fn, formal, formals);

  expandVarArgsBody(fn, formal, formals);

  formal->defPoint->remove();
}

// Insert n formals that will replace the origin varArg
// Return the set of new formals
static Formals insertFormalsForVarArg(ArgSymbol* varArg, int n) {
  Formals retval(n);

  for (int i = 0; i < n; i++) {
    DefExpr*   newArgDef = varArg->defPoint->copy();
    ArgSymbol* newFormal = toArgSymbol(newArgDef->sym);

    // Please update FnSymbol::substitutionsToString if this changes
    newFormal->addFlag(FLAG_EXPANDED_VARARGS);
    newFormal->variableExpr = NULL;
    newFormal->name         = astr("_e", istr(i + 1), "_", varArg->name);
    newFormal->cname        = astr("_e", istr(i + 1), "_", varArg->cname);

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

//
// Replace all constraints like "something < varargs"
// with "something < vararg1, ..., something < varargN"
//
static void expandVarArgsLifetimeConstraints(FnSymbol* fn,
                                             ArgSymbol* formal,
                                             const Formals& varargs) {
  if (! fn->lifetimeConstraints) return; // nothing to do

  std::vector<SymExpr*> symExprs;
  collectSymExprsFor(fn->lifetimeConstraints, formal, symExprs);

  for_vector(SymExpr, se, symExprs)
    if (CallExpr* ltof = toCallExpr(se->parentExpr))
      if (ltof->isPrimitive(PRIM_LIFETIME_OF))
        if (CallExpr* constraint = toCallExpr(ltof->parentExpr))
          {
            // Replace 'constraint' with a copy for each of 'varargs'.
            CallExpr* replAll = NULL;
            for_vector(ArgSymbol, newarg, varargs) {
              SymbolMap map;
              map.put(formal, newarg);
              CallExpr* repl1 = constraint->copy(&map);
              if (replAll)
                replAll = new CallExpr(",", replAll, repl1);
              else
                replAll = repl1;
            }
            constraint->replace(replAll);
          }

  // Are there any references to 'formal' still remaining?
  // If so, complain, because it will be removed from tree.
  symExprs.clear();
  collectSymExprsFor(fn->lifetimeConstraints, formal, symExprs);
  for_vector(SymExpr, se, symExprs)
    USR_FATAL_CONT(se, "this use of the varargs formal %s is currently unsupported", formal->name);
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
    CallExpr* tupleCall = NULL;

    if (isString(formal->type) == true && formal->intent == INTENT_BLANK) {
      tupleCall = expandVarArgString(fn, var, formal, formals);

    } else {
      tupleCall = buildTupleCall(formal, formals);

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

    if (formal->intent == INTENT_OUT || formal->intent == INTENT_INOUT) {
      insertEpilogueTemps(fn, var, tupleCall);
    }

  } else {
    substituteVarargTupleRefs(fn->body, formal, formals);
  }
}

static bool handleCopyData(PartialCopyData* pci,
                           VarSymbol*       var,
                           ArgSymbol*       formal,
                           const Formals&   varargs) {
  int  maxIndex  = pci->partialCopyMap.n - 1;
  bool gotFormal = false;
  bool retval    = false;

  for (int index = maxIndex; index >= 0 && gotFormal == false; index--) {
    SymbolMapElem& mapElem = pci->partialCopyMap.v[index];

    if (mapElem.value == formal) {
      int n = static_cast<int>(varargs.size());

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
    }
  }

  INT_ASSERT(gotFormal == true);

  return retval;
}

//
// Does 'block' contain use(s) of the vararg tuple 'formal'
// that require 'formal' as a whole tuple?
//
static bool needVarArgTupleAsWhole(BlockStmt* block,
                                   int        numArgs,
                                   ArgSymbol* formal) {
  std::vector<SymExpr*> symExprs;
  bool                  retval = false;

  collectSymExprsFor(block, formal, symExprs);

  for (size_t i = 0; i < symExprs.size() && retval == false; i++) {
      SymExpr* se = symExprs[i];

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

  return retval;
}

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

static CallExpr* expandVarArgString(FnSymbol*      fn,
                                    VarSymbol*     var,
                                    ArgSymbol*     formal,
                                    const Formals& varargs) {
  int       n      = static_cast<int>(varargs.size());
  DefExpr*  defn   = new DefExpr(var);
  Expr*     tail   = NULL;
  CallExpr* retval = NULL;

  if (formal->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    retval = new CallExpr(dtTuple->symbol);

    retval->insertAtTail(new_IntSymbol(n));
  } else {
    retval = new CallExpr("_build_tuple");
  }

  // Create a local for every blank string
  for (int i = 0; i < n; i++) {
    const char* localName = astr("_e", istr(i + 1 + n), "_", formal->name);
    VarSymbol*  local     = newTemp(localName, formal->type);
    DefExpr*    localDefn = new DefExpr(local);
    CallExpr*   move      = new CallExpr(PRIM_MOVE, local, varargs[i]);

    // Collect the local in to the tuple
    retval->insertAtTail(local);

    // Define the local and initialize the value
    if (tail == NULL) {
      fn->insertAtHead(localDefn);
    } else {
      tail->insertAfter(localDefn);
    }

    localDefn->insertAfter(move);

    tail = move;
  }

  tail->insertAfter(defn);
  defn->insertAfter(new CallExpr(PRIM_MOVE, var, retval));

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
    retval = new CallExpr(dtTuple->symbol);
  } else {
    retval = new CallExpr(tupleInitName);
  }

  retval->insertAtTail(new_IntSymbol(n));

  for (int i = 0; i < n; i++) {
    retval->insertAtTail(formals[i]);
  }

  return retval;
}

static void insertEpilogueTemps(FnSymbol*  fn,
                                VarSymbol* var,
                                CallExpr*  tupleCall) {
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
  bool movedTemp = false;

  if (CallExpr* move = toCallExpr(parent->parentExpr)) {
    if (move->isPrimitive(PRIM_MOVE)) {
      Symbol* dest = toSymExpr(move->get(1))->symbol();

      if (dest->hasFlag(FLAG_TEMP) == true) {
        // Remove the def of 'dest'.
        dest->defPoint->remove();
        move->remove();

        // Uses of 'dest' will come later.
        // Replace them with 'replFormal' in substituteVarargTupleRefs()
        // using 'tempReps'.
        tempReps.put(dest, replFormal);

        movedTemp = true;
      }
    }
  }

  // Otherwise, just replace 'parent'.
  if (movedTemp == false) {
    parent->replace(new SymExpr(replFormal));
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static int varargAccessIndex(SymExpr* se, CallExpr* parent, int numArgs) {
  if (se == parent->baseExpr && parent->numActuals() == 1) {
    VarSymbol* idxVar = toVarSymbol(toSymExpr(parent->get(1))->symbol());

    if (idxVar                        != NULL &&
        idxVar->immediate             != NULL &&
        idxVar->immediate->const_kind == NUM_KIND_INT) {
      int idxNum = idxVar->immediate->int_value();

      if (1 <= idxNum && idxNum <= numArgs) {
        return idxNum;
      }
    }
  }

  return 0;
}


// Is 'parent' the method call 'se'.size?
static bool isVarargSizeExpr(SymExpr* se, CallExpr* parent) {
  bool retval = false;

  if (parent->isNamed("size") == true) {
    // Is 'parent' a method call?
    if (parent->numActuals() == 2) {
      if (SymExpr* firstArg = toSymExpr(parent->get(1))) {
        if (firstArg->symbol()->type == dtMethodToken) {
          INT_ASSERT(parent->get(2) == se);

          retval = true;
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Maintain a cache from a varArgs function to the function with the required  *
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
