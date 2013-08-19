#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"

//#define DEBUG_CP

static bool isCandidateForCopyPropagation(FnSymbol* fn, VarSymbol* var) {
  // Note: iterator yield symbols are distinct from the return symbol
  // and so are not ruled out by the first test below.
  return
    var != fn->getReturnSymbol() &&
    var->type->refType && // hilde sez: This makes no sense!
    // It appears that this test is intended to ensure that the selected var has value
    // semantics, but there can be refs of refs, so it doesn't really.
    // TODO: Try removing the above line or replacing the test with what is really meant.
    !var->hasFlag(FLAG_CONCURRENTLY_ACCESSED);
}

static bool invalidateCopies(SymExpr* se, Vec<SymExpr*>& defSet, Vec<SymExpr*>& useSet) {
  if (defSet.set_in(se))
    return true;
  if (useSet.set_in(se)) {
    if (CallExpr* parent = toCallExpr(se->parentExpr)) {
      if (parent->isPrimitive(PRIM_ADDR_OF))
        return true;
      if (isRecord(se->var->type)) {
        if (parent->isPrimitive(PRIM_GET_MEMBER))
          return true;
        if (parent->isPrimitive(PRIM_SET_MEMBER) && parent->get(1) == se)
          return true;
      }
      if (is_complex_type(se->var->type)) {
        if (parent->isPrimitive(PRIM_GET_REAL) ||
            parent->isPrimitive(PRIM_GET_IMAG))
          return true;
      }
    }
  }
  return false;
}

//
// The keys in the ReverseAvailableMap are the values in the
// AvailableMap.  Its values are the keys in the AvailableMap so there
// may be more than one.
//
typedef Map<Symbol*,Symbol*> AvailableMap;
typedef MapElem<Symbol*,Symbol*> AvailableMapElem;
typedef Map<Symbol*,Vec<Symbol*>*> ReverseAvailableMap;


static void
makeAvailable(AvailableMap& available,
              ReverseAvailableMap& reverseAvailable,
              Symbol* key,
              Symbol* value) {
  Vec<Symbol*>* keys = reverseAvailable.get(value);
  if (!keys)
    keys = new Vec<Symbol*>();
  keys->add(key);
  reverseAvailable.put(value, keys);
  available.put(key, value);
}


static void
removeAvailable(AvailableMap& available,
                ReverseAvailableMap& reverseAvailable,
                Symbol* sym) {
  Vec<Symbol*>* keys = reverseAvailable.get(sym);
  if (keys) {
    forv_Vec(Symbol, key, *keys) {
      if (sym == available.get(key))
        available.put(key, NULL);
    }
  }
  available.put(sym, NULL);
}


static void
freeReverseAvailable(ReverseAvailableMap& reverseAvailable) {
  Vec<Symbol*> values;
  reverseAvailable.get_keys(values);
  forv_Vec(Symbol, value, values) {
    delete reverseAvailable.get(value);
  }
}


static void
localCopyPropagationCore(BasicBlock* bb,
                         AvailableMap& available,
                         ReverseAvailableMap& reverseAvailable,
                         Vec<SymExpr*>& useSet,
                         Vec<SymExpr*>& defSet) {
  for_vector(Expr, expr, bb->exprs) {
    Vec<SymExpr*> symExprs;
    collectSymExprs(expr, symExprs);

    //
    // replace uses with available copies
    //
    forv_Vec(SymExpr, se, symExprs) {
      if (useSet.set_in(se)) {
        if (Symbol* sym = available.get(se->var)) {
          if (!invalidateCopies(se, defSet, useSet))
            se->var = sym;
        }
      }
    }

    //
    // invalidate available copies based on defs
    //  also if a reference to a variable is taken since the reference
    //  can be assigned
    //
    forv_Vec(SymExpr, se, symExprs) {
      if (invalidateCopies(se, defSet, useSet)) {
        removeAvailable(available, reverseAvailable, se->var);
      }
    }

    //
    // insert pairs into available copies map
    //
    if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* rhs = toSymExpr(call->get(2)))
          if (SymExpr* lhs = toSymExpr(call->get(1)))
            if (lhs->var != rhs->var)
              if (defSet.set_in(lhs))
                if (useSet.set_in(rhs) || rhs->var->isConstant() || rhs->var->isImmediate())
                  makeAvailable(available, reverseAvailable, lhs->var, rhs->var);
  }
}


//
// Apply local copy propagation to basic blocks of function
//
void localCopyPropagation(FnSymbol* fn) {
  buildBasicBlocks(fn);

  //
  // locals: a vector of local variables in function fn that are
  // candidates for copy propagation
  //
  Vec<Symbol*> locals;
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    for_vector(Expr, expr, bb->exprs) {
      if (DefExpr* def = toDefExpr(expr))
        if (VarSymbol* var = toVarSymbol(def->sym))
          if (isCandidateForCopyPropagation(fn, var))
            locals.add(def->sym);
    }
  }

  //
  // defSet: a set of SymExprs that are defs of local variables
  // useSet: a set of SymExprs that are uses of local variables
  //
  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  for_vector(BasicBlock, bb1, *fn->basicBlocks) {
    AvailableMap available;
    ReverseAvailableMap reverseAvailable;
    localCopyPropagationCore(bb1, available, reverseAvailable, useSet, defSet);
    freeReverseAvailable(reverseAvailable);
  }
}


//
// Apply global copy propagation to basic blocks of function
//
void globalCopyPropagation(FnSymbol* fn) {
  buildBasicBlocks(fn);

  // global copy propagation will have no effect
  if (fn->basicBlocks->size() <= 1)
    return;

  //
  // locals: a vector of local variables in function fn that are
  // candidates for copy propagation
  //
  Vec<Symbol*> locals;
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    for_vector(Expr, expr, bb->exprs) {
      if (DefExpr* def = toDefExpr(expr))
        if (VarSymbol* var = toVarSymbol(def->sym))
          if (isCandidateForCopyPropagation(fn, var))
            locals.add(def->sym);
    }
  }

  //
  // defSet: a set of SymExprs that are defs of local variables
  // useSet: a set of SymExprs that are uses of local variables
  //
  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  //
  // spsLHS: left-hand side of copy
  // spsRHS: right-hand side of copy
  // spsN: basic block locations; basic block i contains the copies from
  //     spsN(i-1) to spsN(i)
  //
  // Compute sparse versions of these vectors; convert to dense and
  // drop sps; these are sparse to zero out invalidated copies
  //
  Vec<int> spsN;
  Vec<SymExpr*> spsLHS;
  Vec<SymExpr*> spsRHS;
  int start = 0;
  for_vector(BasicBlock, bb1, *fn->basicBlocks) {
    for_vector(Expr, expr, bb1->exprs) {

      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);

      //
      // invalidate available copies based on defs
      //
      forv_Vec(SymExpr, se, symExprs) {
        if (invalidateCopies(se, defSet, useSet)) {
          for (int i = start; i < spsLHS.n; i++) {
            if (spsLHS.v[i]) {
              if (spsLHS.v[i]->var == se->var || spsRHS.v[i]->var == se->var) {
                spsLHS.v[i] = 0;
                spsRHS.v[i] = 0;
              }
            }
          }
        }
      }

      //
      // insert pairs into available copies map
      //
      if (CallExpr* call = toCallExpr(expr))
        if (call->isPrimitive(PRIM_MOVE))
          if (SymExpr* lhs = toSymExpr(call->get(1)))
            if (SymExpr* rhs = toSymExpr(call->get(2)))
              if (lhs->var != rhs->var &&
                  defSet.set_in(lhs) &&
                  (useSet.set_in(rhs) || rhs->var->isConstant() || rhs->var->isImmediate())) {
                spsLHS.add(lhs);
                spsRHS.add(rhs);
              }
    }
    spsN.add(spsLHS.n);
    start = spsLHS.n;
  }

#ifdef DEBUG_CP
  printf("\n");
  list_view(fn);

  printBasicBlocks(fn);
#endif

  //
  // Turn sparse spsLHS, spsRHS, spsN into dense LHS, RHS, N
  //
  Vec<int> N;
  Vec<SymExpr*> LHS;
  Vec<SymExpr*> RHS;

  size_t nbbs = fn->basicBlocks->size();
  int j = 0;
  for(size_t i = 0; i < nbbs; ++i) {
#ifdef DEBUG_CP
    printf("%d:\n", i);
#endif
    while (j < spsN.v[i]) {
      if (spsLHS.v[j]) {
#ifdef DEBUG_CP
        list_view(spsLHS.v[j]->parentExpr);
#endif
        LHS.add(spsLHS.v[j]);
        RHS.add(spsRHS.v[j]);
      }
      j++;
    }
    N.add(LHS.n);
  }
#ifdef DEBUG_CP
  printf("\n");
#endif

  std::vector<BitVec*> COPY;
  std::vector<BitVec*> KILL;
  std::vector<BitVec*> IN;
  std::vector<BitVec*> OUT;
  j = 0;
  for(size_t i = 0; i < nbbs; ++i) {
    BitVec* copy = new BitVec(N.v[nbbs-1]);
    BitVec* kill = new BitVec(N.v[nbbs-1]);
    BitVec* in = new BitVec(N.v[nbbs-1]);
    BitVec* out = new BitVec(N.v[nbbs-1]);
    while (j < N.v[i]) {
      copy->set(j);
      j++;
    }
    COPY.push_back(copy);
    KILL.push_back(kill);
    IN.push_back(in);
    OUT.push_back(out);
  }

#ifdef DEBUG_CP
  printf("COPY:\n"); printBitVectorSets(COPY);
#endif

  //
  // compute kill set
  //
  size_t i = 0;
  start = 0;
  for_vector(BasicBlock, bb2, *fn->basicBlocks) {
    int stop = N.v[i];

    Vec<Symbol*> killSet;
    for_vector(Expr, expr, bb2->exprs) {

      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);

      //
      // invalidate available copies based on defs
      //
      forv_Vec(SymExpr, se, symExprs) {
        if (invalidateCopies(se, defSet, useSet)) {
          if (!killSet.set_in(se->var)) {
            killSet.set_add(se->var);
          }
        }
      }
    }
    for (int j = 0; j < start; j++) {
      if (killSet.set_in(LHS.v[j]->var) || killSet.set_in(RHS.v[j]->var)) {
        KILL[i]->set(j);
      }
    }
    for (int j = stop; j < LHS.n; j++) {
      if (killSet.set_in(LHS.v[j]->var) || killSet.set_in(RHS.v[j]->var)) {
        KILL[i]->set(j);
      }
    }
    start = stop;
    i++;
  }

#ifdef DEBUG_CP
  printf("KILL:\n"); printBitVectorSets(KILL);
#endif

  // initialize IN set
  for (size_t i = 1; i < nbbs; i++) {
    for (int j = 0; j < LHS.n; j++) {
      IN[i]->set(j);
    }
  }

#ifdef DEBUG_CP
  printf("IN:\n"); printBitVectorSets(IN);
#endif

  forwardFlowAnalysis(fn, COPY, KILL, IN, OUT, true);

  for (size_t i = 0; i < nbbs; i++) {
    BasicBlock* bb = (*fn->basicBlocks)[i];
    AvailableMap available;
    ReverseAvailableMap reverseAvailable;
    bool proceed = false;
    for (int j = 0; j < LHS.n; j++) {
      if (IN[i]->get(j)) {
        makeAvailable(available, reverseAvailable, LHS.v[j]->var, RHS.v[j]->var);
        proceed = true;
      }
    }
    if (proceed)
      localCopyPropagationCore(bb, available, reverseAvailable, useSet, defSet);
    freeReverseAvailable(reverseAvailable);
  }

  for_vector(BitVec, copy, COPY)
    delete copy, copy = 0;

  for_vector(BitVec, kill, KILL)
    delete kill, kill = 0;

  for_vector(BitVec, in, IN)
    delete in, in = 0;

  for_vector(BitVec, out, OUT)
    delete out, out = 0;
}


// If there is exactly one definition of var by something of reference type, 
// then return the call that defines it.
// Otherwise, return NULL.
static CallExpr*
findRefDef(Map<Symbol*,Vec<SymExpr*>*>& defMap, Symbol* var) {
  CallExpr* ret = NULL;
  for_defs(def, defMap, var) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (isReferenceType(call->get(2)->typeInfo())) {
          if (ret)
            return NULL;
          else
            ret = call;
        }
      }
    }
  }
  return ret;
}


void
eliminateSingleAssignmentReference(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                   Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                   Symbol* var) {
  if (CallExpr* move = findRefDef(defMap, var)) {
    if (CallExpr* rhs = toCallExpr(move->get(2))) {
      if (rhs->isPrimitive(PRIM_ADDR_OF)) {
        bool stillAlive = false;
        for_uses(se, useMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent && parent->isPrimitive(PRIM_DEREF)) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            parent->replace(se);
            addUse(useMap, se);
          } else if (parent &&
                     (parent->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                      parent->isPrimitive(PRIM_GET_MEMBER) ||
                      parent->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                      parent->isPrimitive(PRIM_GET_MEMBER))) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            parent->get(1)->replace(se);
            addUse(useMap, se);
          } else if (parent && parent->isPrimitive(PRIM_MOVE)) {
            CallExpr* rhsCopy = rhs->copy();
            parent->get(2)->replace(rhsCopy);
            SymExpr* se = toSymExpr(rhsCopy->get(1));
            INT_ASSERT(se);
            addUse(useMap, se);
          } else
            stillAlive = true;
        }
        for_defs(se, defMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent == move)
            continue;
          if (parent && parent->isPrimitive(PRIM_MOVE)) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            parent->get(1)->replace(se);
            addDef(defMap, se);
          } else
            stillAlive = true;
        }
        if (!stillAlive) {
          var->defPoint->remove();
          defMap.get(var)->v[0]->getStmtExpr()->remove();
        }
      } else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                 rhs->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        bool stillAlive = false;
        bool isSvec = rhs->isPrimitive(PRIM_GET_SVEC_MEMBER);
        for_uses(se, useMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent && parent->isPrimitive(PRIM_DEREF)) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            if (!isSvec)
              parent->replace(new CallExpr(PRIM_GET_MEMBER_VALUE,
                                           se,
                                           rhs->get(2)->copy()));
            else
              parent->replace(new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                                           se,
                                           rhs->get(2)->copy()));
            addUse(useMap, se);
          } else if (parent && parent->isPrimitive(PRIM_MOVE)) {
            CallExpr* rhsCopy = rhs->copy();
            parent->get(2)->replace(rhsCopy);
            SymExpr* se = toSymExpr(rhsCopy->get(1));
            INT_ASSERT(se);
            addUse(useMap, se);
          } else
            stillAlive = true;
        }
        for_defs(se, defMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent == move)
            continue;
          if (parent && parent->isPrimitive(PRIM_MOVE)) {
            if (SymExpr* rtmp = toSymExpr(parent->get(2))) {
              SymExpr* se = toSymExpr(rhs->get(1)->copy());
              INT_ASSERT(se);
              if (!isSvec)
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             rtmp->remove()));
              else
                parent->replace(new CallExpr(PRIM_SET_SVEC_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             rtmp->remove()));
              addUse(useMap, se);
            } else {
              VarSymbol* tmp = newTemp(parent->get(2)->typeInfo());
              parent->getStmtExpr()->insertBefore(new DefExpr(tmp));
              parent->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, parent->get(2)->remove()));
              SymExpr* se = toSymExpr(rhs->get(1)->copy());
              INT_ASSERT(se);
              if (!isSvec)
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             tmp));
              else
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             tmp));
              addUse(useMap, se);
            }
          } else
            stillAlive = true;
        }
        if (!stillAlive) {
          var->defPoint->remove();
          defMap.get(var)->v[0]->getStmtExpr()->remove();
        }
      }
    }
  }
}


void singleAssignmentRefPropagation(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);

  Vec<Symbol*> refSet;
  Vec<Symbol*> refVec;
  Vec<SymExpr*> symExprs;
  // Walk the asts in this function, and build lists of reference variables and sym exprs.
  forv_Vec(BaseAST, ast, asts) {
    if (VarSymbol* var = toVarSymbol(ast)) {
      if (isReferenceType(var->type)) {
        refVec.add(var);
        refSet.set_add(var);
      }
    } else if (SymExpr* se = toSymExpr(ast)) {
      symExprs.add(se);
    }
  }

  // Build def/use maps across all symexprs in the function, 
  // but restricted to only the ref variables therein.
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(refSet, symExprs, defMap, useMap);

  // Walk the list of reference vars
  forv_Vec(Symbol, var, refVec) {
    if (var) {
      // Get the move that defines this reference
      if (CallExpr* move = findRefDef(defMap, var)) {
        if (SymExpr* rhs = toSymExpr(move->get(2))) {
          // If it is defined from another reference, these two are mutual aliases.
          if (isReferenceType(rhs->var->type)) {
            // Replace each use of the new name with the old name.
            for_uses(se, useMap, var) {
              if (se->parentExpr) {
                SET_LINENO(se);
                SymExpr* rhsCopy = rhs->copy();
                se->replace(rhsCopy);
                addUse(useMap, rhsCopy);
              }
            }
            // Other than the original definition, replace
            // definitions of the new name with defs of the old.
            for_defs(se, defMap, var) {
              CallExpr* parent = toCallExpr(se->parentExpr);
              if (parent && parent != move) {
                SET_LINENO(se);
                SymExpr* rhsCopy = rhs->copy();
                se->replace(rhsCopy);
                addDef(defMap, rhsCopy);
              }
            }
          }
        }
      }
    }
  }

  forv_Vec(Symbol, var, refVec) { // ack! note: order matters
    if (var) {
      eliminateSingleAssignmentReference(defMap, useMap, var);
    }
  }

  freeDefUseMaps(defMap, useMap);
}


void copyPropagation(void) {
  if (!fNoCopyPropagation) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      localCopyPropagation(fn);
      if (!fNoDeadCodeElimination) {
        deadVariableElimination(fn);
      }
      globalCopyPropagation(fn);
    }
  }
}


void refPropagation() {
  if (!fNoCopyPropagation) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      singleAssignmentRefPropagation(fn);
    }
  }
}
