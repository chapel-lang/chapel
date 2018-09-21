/*
 * Copyright 2004-2018 Cray Inc.
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

#include "lifetime.h"

#include "AstVisitorTraverse.h"
#include "bb.h"
#include "bitVec.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "loopDetails.h"
#include "UnmanagedClassType.h"
#include "stlUtil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

// These enable debug facilities
static const char* debugNilsForFn = "";
static const int debugNilsForId = 0;

typedef enum {
  UNKNOWN_NIL = 0,
  IS_NIL = 1,
  SET_MAYBE_NIL = 2,
  NOT_NIL = 3
} classify_nil_t;


static bool isCheckedClassMethodCall(CallExpr* call) {
  FnSymbol* fn     = call->resolvedOrVirtualFunction();
  bool      retval = false;

  if (fn && fn->isMethod() && fn->_this) {
    if (0 == strcmp(fn->name, "borrow")) {
      // Ignore .borrow()
      // (at least until we decide that borrow-from-empty-owned
      //  is a problem, rather than use of the resulting nil)
      retval = false;

    } else if (AggregateType* ct = toAggregateType(fn->_this->typeInfo())) {
      if (fn->numFormals()             >  0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (isClassLike(ct)) {
          retval = true;
        }
      }
    }
  }

  return retval;
}
/*
static void printDataFlowSet(const char* name, BitVec* set,
                             std::vector<Symbol*>& idxToSym)
{
  if (set->any()) {
    printf("dataflow set %s\n", name);
    for(size_t i = 0; i < idxToSym.size(); i++ ) {
      Symbol* sym = idxToSym[i];
      if (sym && set->get(i)) {
        printf("%i %s\n", sym->id, sym->name);
      }
    }
  }
}
*/

static void checkForNilDereferencesInCall(
    CallExpr* call,
    std::map<Symbol*, int>& symToIdx,
    std::vector<Symbol*>& idxToSym,
    std::vector<classify_nil_t>& bbStatus) {

  if (call->isPrimitive(PRIM_GET_MEMBER)       ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_SET_MEMBER)       ||
      call->isPrimitive(PRIM_GETCID)           ||
      call->isPrimitive(PRIM_TESTCID)          ||
      isCheckedClassMethodCall(call)) {

    SymExpr* thisSe = toSymExpr(call->get(1));
    Symbol* thisSym = thisSe->symbol();
    if (symToIdx.count(thisSym)) {
      int idx = symToIdx[thisSym];
      // Raise an error if it was definately nil
      if (bbStatus[idx] == IS_NIL) {
        USR_FATAL_CONT(call, "use of nil variable");
      }
    }
  }
}

static void printStatus(const char* prefix,
                        std::vector<classify_nil_t>& status,
                        std::vector<Symbol*>& idxToSym)
{
  for (size_t i = 0; i < idxToSym.size(); i++ ) {
    Symbol* sym = idxToSym[i];
    if (sym) {
      if (status[i] == IS_NIL) {
        printf("%s nil: %i %s\n", prefix, sym->id, sym->name);
      } else if (status[i] == NOT_NIL) {
        printf("%s not: %i %s\n", prefix, sym->id, sym->name);
      } else if (status[i] == SET_MAYBE_NIL) {
        printf("%s set: %i %s\n", prefix, sym->id, sym->name);
      }
    }
  }
}

static bool isRecordInitOrReturn(CallExpr* call, Symbol*& lhs, CallExpr*& initOrCtor) {

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
      if (rhsCallExpr->resolvedOrVirtualFunction()) {
        if (AggregateType* at = toAggregateType(rhsCallExpr->typeInfo())) {
          if (isRecord(at)) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            lhs = se->symbol();
            initOrCtor = rhsCallExpr;
            return true;
          }
        }
      }
    }
  }

  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (calledFn->isMethod() && calledFn->name == astrInit) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      Symbol* sym = se->symbol();
      if (isRecord(sym->type)) {
        lhs = sym;
        initOrCtor = call;
        return true;
      }
    } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
      for_formals_actuals(formal, actual, call) {
        if (formal->hasFlag(FLAG_RETARG)) {
          if (isRecord(formal->getValType())) {
            SymExpr* se = toSymExpr(actual);
            INT_ASSERT(se);
            Symbol* sym = se->symbol();
            lhs = sym;
            initOrCtor = call;
            return true;
          }
        }
      }
    }
  }

  lhs = NULL;
  initOrCtor = NULL;
  return false;
}

static void findNilDereferencesInBasicBlock(
    FnSymbol* fn,
    std::map<Symbol*, int>& symToIdx,
    std::vector<Symbol*>& idxToSym,
    size_t nvars,
    BasicBlock* bb,
    BitVec* GEN,
    BitVec* KILL,
    BitVec* IN,
    BitVec* OUT,
    bool raiseErrors) {

  GEN->reset();
  KILL->reset();


  std::vector<classify_nil_t> bbStatus(nvars);
  std::vector<classify_nil_t> status(nvars);

  for (size_t i = 0; i < nvars; i++ ) {
    bbStatus[i] = IN->get(i) ? IS_NIL : UNKNOWN_NIL;
  }

  bool gaveUp = false;

  for_vector(Expr, expr, bb->exprs) {

    for (size_t i = 0; i < nvars; i++ ) {
      status[i] = UNKNOWN_NIL;
    }

    bool trace = 0 == strcmp(fn->name, debugNilsForFn);

    if (trace) {
      nprint_view(expr);

      printStatus("bb", bbStatus, idxToSym);
    }

    if (expr->id == debugNilsForId)
      gdbShouldBreakHere();

    if (isDefExpr(expr)) {
      // Ignore DefExprs
      // instead, find the defaultOf or whatever initializes them.
    } else if (CallExpr* call = toCallExpr(expr)) {
      Symbol* lhsSym = NULL;
      Expr* rhsExpr = NULL;
      bool moveLike = (call->isPrimitive(PRIM_MOVE) ||
                       call->isPrimitive(PRIM_ASSIGN) ||
                       call->isNamedAstr(astrSequals));

      if (moveLike) {
        SymExpr* lhsSe = toSymExpr(call->get(1));
        lhsSym = lhsSe->symbol();
        rhsExpr = call->get(2);
      }

      {
        Symbol* initSym = NULL;
        CallExpr* initCall = NULL;
        if (isRecordInitOrReturn(call, initSym, initCall)) {
          lhsSym = initSym;
          rhsExpr = initCall;
          moveLike = true;
        }
      }

      if (moveLike) {
        // lhsSym and rhsExpr set above
        if (lhsSym->isRef()) {
          // TODO -- may-alias sets?
          // var x: MyClass; ref r = x; r = new MyClass(); x.method();
          // TODO: call returning ref
          // TODO: alias case above but ref r = returnsRefArg(x);

          // For now just kill everything (disable errors)
          gaveUp = true;
          gdbShouldBreakHere();

        } else {
          // gather set of possibly aliasing variables from RHS
          if (rhsExpr->typeInfo() == dtNil) {
            if (symToIdx.count(lhsSym))
              status[symToIdx[lhsSym]] = IS_NIL;

          } else if (CallExpr* rhsCall = toCallExpr(rhsExpr)) {
            if (raiseErrors)
              checkForNilDereferencesInCall(rhsCall,
                                            symToIdx,
                                            idxToSym,
                                            bbStatus);

            FnSymbol* calledFn = rhsCall->resolvedOrVirtualFunction();
            if (calledFn == NULL) {
              if (rhsCall->isPrimitive(PRIM_DEREF)) {
                SymExpr* fromSe = toSymExpr(rhsCall->get(1));
                Symbol* fromSym = fromSe->symbol();
                if (symToIdx.count(lhsSym) && symToIdx.count(fromSym))
                  status[symToIdx[lhsSym]] = bbStatus[symToIdx[fromSym]];

              } else if (rhsCall->isPrimitive(PRIM_CAST)) {
                SymExpr* fromSe = toSymExpr(rhsCall->get(2));
                Symbol* fromSym = fromSe->symbol();
                if (symToIdx.count(lhsSym) && symToIdx.count(fromSym))
                  status[symToIdx[lhsSym]] = bbStatus[symToIdx[fromSym]];

              } else {
                gaveUp = true;
                gdbShouldBreakHere();
              }
            } else if (calledFn->name == astr_defaultOf) {
              if (symToIdx.count(lhsSym))
                status[symToIdx[lhsSym]] = IS_NIL;

            } else if (calledFn->name == astrNew) {
              // assume new returns non-nil (we halt otherwise)
              if (symToIdx.count(lhsSym))
                status[symToIdx[lhsSym]] = NOT_NIL;

            } else if (0 == strcmp(calledFn->name, "borrow")) {
              SymExpr* thisSe = toSymExpr(rhsCall->get(1));
              Symbol* thisSym = thisSe->symbol();
              if (symToIdx.count(lhsSym) && symToIdx.count(thisSym))
                status[symToIdx[lhsSym]] = bbStatus[symToIdx[thisSym]];

            } else {
              // Think about the formals and the actuals?
              // check formals for FLAG_NILIFIES_ARG ?
              gaveUp = true;
              gdbShouldBreakHere();
            }

            // TODO -- check for method calls with nil this
          } else if (SymExpr* se = toSymExpr(rhsExpr)) {
            Symbol* rhsSym = se->symbol();
            if (symToIdx.count(lhsSym) && symToIdx.count(rhsSym))
                status[symToIdx[lhsSym]] = bbStatus[symToIdx[rhsSym]];

          } else {
            INT_FATAL("unhandled case");
          }
        }
      } else {
        // Not PRIM_MOVE, PRIM_ASSIGN, or =

        FnSymbol* calledFn = call->resolvedOrVirtualFunction();
        // handle owned.init(pointer)
        if (calledFn && calledFn->name == astrInit) {
          if (call->numActuals() == 2) {
            Symbol* initedSym = toSymExpr(call->get(1))->symbol();
            Symbol* fromSym = toSymExpr(call->get(2))->symbol();
            if (initedSym->type->symbol->hasFlag(FLAG_MANAGED_POINTER)) {
              if (symToIdx.count(initedSym) && symToIdx.count(fromSym))
                status[symToIdx[initedSym]] = bbStatus[symToIdx[fromSym]];
            }
          }
        } else if (raiseErrors) {
          // check for calls to nil
          checkForNilDereferencesInCall(call,
                                        symToIdx,
                                        idxToSym,
                                        bbStatus);
        }
      }
    }

    if (gaveUp) {
      for (size_t i = 0; i < nvars; i++ ) {
        status[i] = SET_MAYBE_NIL;
      }
    }

    if (trace) {
      if (gaveUp)
        printf("(gave up)\n");
      printStatus("+- ", status, idxToSym);
    }


    // TODO: Consider possible aliases for symbols in toGen / toKill
    // May-alias information
    //   - propagate to may-aliases for toKill but not toGen
    //     (since if it didn't alias, we don't know it's set to nil)
    // Must-alias information
    //   - propagate to must-aliases for toGen and toKill

    for (size_t i = 0; i < nvars; i++ ) {
      if (status[i] != UNKNOWN_NIL) {
        bbStatus[i] = status[i];
      }
    }
  }

  for (size_t i = 0; i < nvars; i++ ) {
    if (status[i] == IS_NIL)
      GEN->set(i);
    else if (status[i] == SET_MAYBE_NIL)
      KILL->set(i);
    else if (status[i] == NOT_NIL)
      KILL->set(i);
  }
}

// TODO -- move this (and the version in copyPropagation) to bb.h
static void makeDataFlowSet(std::vector<BitVec*>& set,
			    FnSymbol* fn,
			    size_t size) {
  size_t nbbs = fn->basicBlocks->size();

  // Create a BitVec of length size for each block.
  for (size_t i = 0; i < nbbs; ++i)
    set.push_back(new BitVec(size));
}

static void destroyDataFlowSet(std::vector<BitVec*> set)
{
  for_vector(BitVec, vec, set)
    delete vec, vec = 0;
}


// TODO -- move this (and the version in copyPropagation) to bb.h
static void setupInDataFlowSet(std::vector<BitVec*>& IN, FnSymbol* fn)
{
  size_t i = 0;
  for_vector(BasicBlock, bb, *fn->basicBlocks)
  {
    if (bb->ins.size() == 0)
      // This block has no predecessors, so set its initial IN set to zeroes.
      IN[i]->reset();
    else
      // This block has a predecessor, so set its initial IN set to all ones.
      IN[i]->set();

    ++i;
  }
}

void findNilDereferences(FnSymbol* fn) {

  bool debugging = 0 == strcmp(fn->name, debugNilsForFn) ||
                   fn->id == debugNilsForId;

  if (debugging) {
    printf("Visiting function %s id %i\n", fn->name, fn->id);
    nprint_view(fn);
    gdbShouldBreakHere();
  }


  // Let's do some data-flow analysis!

  // GEN(x) means that variable x stores nil
  // KILL(x) means that variable x might no longer store nil

  BasicBlock::buildBasicBlocks(fn);

  // Gather the variables to consider (variables of class type,
  // including borrows, owned, unmanaged, etc)
  std::vector<Symbol*> idxToSym;
  std::map<Symbol*, int> symToIdx;

  {
    int index = 0;
    std::vector<SymExpr*> symExprs;
    collectSymExprs(fn, symExprs);
    for_vector(SymExpr, se, symExprs) {
      Symbol* sym = se->symbol();
      if (isArgSymbol(sym) || isVarSymbol(sym)) {
        if (symToIdx.count(sym) == 0) {
          // Consider adding it to the map.
          // Is it of class (ish) type?
          TypeSymbol* ts = sym->getValType()->symbol;
          if (isClassLike(ts->type) ||
              ts->hasFlag(FLAG_MANAGED_POINTER)) {
            symToIdx.insert(std::pair<Symbol*, int>(sym, index));
            idxToSym.push_back(sym);
            INT_ASSERT((int)(idxToSym.size()-1) == index);
            index++;
            if (debugging) {
              printf("Tracking symbol %i %s\n", sym->id, sym->name);
            }
          }
        }
      }
    }
  }

  std::vector<BitVec*> GEN;
  std::vector<BitVec*> KILL;
  std::vector<BitVec*> IN;
  std::vector<BitVec*> OUT;

  size_t nvars = idxToSym.size();
  makeDataFlowSet(GEN, fn, nvars);
  makeDataFlowSet(KILL,fn, nvars);
  makeDataFlowSet(IN,  fn, nvars);
  makeDataFlowSet(OUT, fn, nvars);

  setupInDataFlowSet(IN, fn);

  size_t nbbs = fn->basicBlocks->size();
  for (size_t i = 0; i < nbbs; i++) {
    findNilDereferencesInBasicBlock(fn, symToIdx, idxToSym, nvars,
                                    (*fn->basicBlocks)[i],
                                    GEN[i], KILL[i], IN[i], OUT[i],
                                    /*raise errors?*/ false);
  }

  // intersect or union?
  // if something is definately nil in one block that flows to this one,
  // but maye not nil in another block, the analysis should assume it's
  // not nil.
  BasicBlock::forwardFlowAnalysis(fn, GEN, KILL, IN, OUT, /*intersect?*/true);

  for (size_t i = 0; i < nbbs; i++) {
    findNilDereferencesInBasicBlock(fn, symToIdx, idxToSym, nvars,
                                    (*fn->basicBlocks)[i],
                                    GEN[i], KILL[i], IN[i], OUT[i],
                                    /*raise errors?*/ true);
  }

  destroyDataFlowSet(GEN);
  destroyDataFlowSet(KILL);
  destroyDataFlowSet(IN);
  destroyDataFlowSet(OUT);
}
