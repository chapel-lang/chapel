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

// copyPropagation.cpp
//
#include "optimizations.h"

#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "view.h"

//#############################################################################
//# COPY PROPAGATION
//#
//# In general, we want to replace:
//#  (move y x)
//#  ("foo" ... y ... )
//# with:
//#  (move y x)
//#  ("foo" ... x ... )
//# That is, cut out the middle man.  If it then happens that y is unused, its
//# declaration and the move that defines it can be removed (by
//# deadVariableElimination).
//#
//# Clearly, we can only make the substitution if neither x nor y is reassigned
//# in the interim.  If x changes:
//#  (move y x)
//#  (move x norp)
//#  ("foo" ... x ... ) // Wrong! We need to use the "old" x (i.e. y).
//# If y changes:
//#  (move y x)
//#  (move y bink)
//#  ("foo" ... x ... ) // Wrong! We need to use the "new" y.
//#
//# We also need to worry about either variable being updated through a
//# reference.
//#  (move rx (addr_of x))
//#  (move ry (addr_of y))
//#  (move y x)
//#  ("foo" ... x ... ) // OK
//# If x is changed through any of its references:
//#  (move y x)
//#  (move (deref rx) norp)
//#  ("foo" ... x ... ) // Wrong! We need to use the "old" x.
//# If y is changed through any of its references:
//#  (move y x)
//#  (move (deref ry) bink)
//#  ("foo" ... x ... ) // Wrong! We need to use the "new" y.
//#
//# Note that (deref rx) can be treated like another y:
//#  ("foo" ... (deref rx) ...)
//# becomes:
//#  ("foo" ... x ...)
//# As long as rx is a valid reference of x.
//#
//# For simplicity, this pass avoids propagating any variables that may have
//# references to them.
//#
//#############################################################################

//#############################################################################
//# Implementation
//#
//# Note that the algorithm straight out of Muchnick does not handle
//# references.  We assume that deref temporaries are used, so
//#  (move z (deref rx))
//# makes z another alias for x.
//#
//# The standard Muchnick algorithm just keeps track of available copy pairs
//# (lhs, rhs).  Since a variable (lhs) can store at most one value, these
//# pairs can be stored in a map.  But when it comes to invalidating pairs when
//# the rhs changes, a search would be required.  In the present
//# implementation, another map stores (rhs, [lhs, lhs, ...]) -- the second
//# element of each pair being a list of lhs keys to invalidate when a rhs is
//# overwritten.
//#
//# I think the set of available copy pairs should be reasonably small, so we
//# can dispense with the reverse map and just traverse the whole set when
//# looking for pairs to invalidate.  If this map size grows to 100 or more,
//# then some more efficient index is needed.
//#
//#############################################################################

//#############################################################################
//# CAVEAT:
//#
//# Reference tracking currently does not traverse record field assignments, so
//# given:
//#  (move rx (addr_of x))
//#  (.= record field rx)
//#  (move rx2 (.v record field))
//#  (move x 3)                    // Creates pair (x, 3)
//#  (move (deref rx2) 7)          // Should kill (x, 3) but doesn't
//# the current implementation does not discover that x has been overwritten
//# through rx2.  This can lead to the generation of incorrect code.
//#
//# Because scalar replacement unwraps records, it eliminates many (if not all)
//# of the cases that could cause this error to occur.  But the fact that some
//# test cases fail (e.g. test_nested_var_iterator3) when scalar propagation is
//# turned off means this is a known weakness that should be addressed.
//#############################################################################


static size_t s_repl_count; ///< The number of pairs replaced by GCP this pass.

//#############################################################################
//# LOCAL COPY PROPAGATION
//#############################################################################

#include <map>
#include <set>

// AvailableMap: lhs -> rhs
// The relationship derived from (move lhs rhs) -- that lhs becomes an alias
// for rhs.
// Substituting the value for the key uses the original value (i.e. symbol) in
// place of the alias.
typedef std::map<Symbol*, Symbol*> AvailableMap;
typedef AvailableMap::value_type AvailableMapElem;
typedef std::pair<Symbol*, Symbol*> AvailablePair;

// ReverseAvailableMap: rhs --> lhs*
// The reverse of the available map, used to accelerate the removal of pairs
// invalidated because the value of the RHS has changed.
typedef std::map<Symbol*, std::vector<Symbol*> > ReverseAvailableMap;
typedef ReverseAvailableMap::mapped_type ReverseMapList;


#if DEBUG_CP
// Set nonzero to enable verbose output.
static unsigned debug = 0;

#define DEBUG_COPYPROP(...) if (debug > 0) printf(__VA_ARGS__)

#else

#define DEBUG_COPYPROP(...)

#endif


// This routine returns true if the value of the given symbol may have changed
// due to execution of the containing expression.
// If the symbol is a reference, this means that the address to which the
// symbol points will be changed, not the value contained in that address.  See
// isRefUse() for that case.
// To be conservative, the routine should return true by default and then
// select the cases where we are sure nothing has changed.
static bool needsKilling(SymExpr* se, std::set<Symbol*>& liveRefs)
{
  INT_ASSERT(se->isRef() == false);
  if (toGotoStmt(se->parentExpr)) {
    return false;
  }

  if (toCondStmt(se->parentExpr)) {
    return false;
  }

  if (toBlockStmt(se->parentExpr)) {
    return false;
  }

  if (isDefExpr(se->parentExpr)) {
    return false;
  }

  CallExpr* call = toCallExpr(se->parentExpr);

  if (FnSymbol* fn = call->resolvedFunction())
  {
    // Skip the "base" symbol.
    if (se->symbol() == fn)
    {
      return false;
    }

    ArgSymbol* arg = actual_to_formal(se);

    if (arg->intent == INTENT_OUT   ||
        arg->intent == INTENT_INOUT ||
        arg->intent == INTENT_REF   ||
        arg->hasFlag(FLAG_ARG_THIS)) // Todo: replace with arg intent check?
    {
      liveRefs.insert(se->symbol());
      return true;
    }

    if (isRecordWrappedType(arg->type))
    {
      return true;
    }

    return false;
  }
  else
  {
    const bool isFirstActual = call->get(1) == se;
    if ((call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
        && isFirstActual)
    {
      return true;
    }

    if (isOpEqualPrim(call) && isFirstActual)
    {
      return true;
    }

    if (call->isPrimitive(PRIM_SET_MEMBER) && isFirstActual)
    {
      return true;
    }

    if (call->isPrimitive(PRIM_ARRAY_SET) ||
        call->isPrimitive(PRIM_ARRAY_SET_FIRST))
    {
      if (isFirstActual)
      {
        return true;
      }

      return false;
    }

    if (call->isPrimitive(PRIM_GET_MEMBER))
    {
      // This creates an alias to a portion of the first arg.
      // We could track this as a reference and invalidate a pair containing
      // this symbol when the ref is dereferenced.  But for now, we want to
      // preserve the mapping ref = &value in the RefMap, so having a (ref,
      // value) pair also possibly mean ref = &(value.subfield) does not quite
      // fit.
      // We could keep the ability to do (deref ref) <- value substitution by
      // keeping a separate map for "true" references, or by performing those
      // substitutions in a separate pass.
      // For now, we treat subfield extraction as evidence of a future change
      // to the symbol itself, and use that fact to remove it from
      // consideration in copy propagation.
      if (isFirstActual)
      {
        // We select just the case where the referent is passed by value,
        // because in the other case, the address of the object is not
        // returned, so that means that the address (i.e. the value of the
        // reference variable) does not change.
        return true;
      }

      return false;
    }

    if (call->isPrimitive(PRIM_ADDR_OF) ||
        call->isPrimitive(PRIM_SET_REFERENCE)) {
      liveRefs.insert(se->symbol());
      return true;
    }

    return false;
  }

  INT_ASSERT(0); // Should never get here.

  return true;
}


// Returns true if the symbol is read in the containing expression,
// false otherwise.  If the operand is used as an address,
// that does not count as a 'read', so false is returned in that case.
static bool isUse(SymExpr* se)
{
  if (toGotoStmt(se->parentExpr))
    return false;

  if (toCondStmt(se->parentExpr))
    return true;

  if (toBlockStmt(se->parentExpr))
    return true;

  if (isDefExpr(se->parentExpr))
    return false;

  CallExpr* call = toCallExpr(se->parentExpr);

  if (FnSymbol* fn = call->resolvedFunction())
  {
    // Skip the "base" symbol.
    if (se->symbol() == fn)
      return false;

    // A "normal" call.
    ArgSymbol* arg = actual_to_formal(se);

    if (arg->intent == INTENT_OUT ||
        (arg->intent & INTENT_FLAG_REF))
      return false;
  }

  else
  {
    INT_ASSERT(call->primitive);
    const bool isFirstActual = call->get(1) == se;

    switch(call->primitive->tag)
    {
     default:
      return true;

     case PRIM_MOVE:
     case PRIM_ASSIGN:
     case PRIM_ADD_ASSIGN:
     case PRIM_SUBTRACT_ASSIGN:
     case PRIM_MULT_ASSIGN:
     case PRIM_DIV_ASSIGN:
     case PRIM_MOD_ASSIGN:
     case PRIM_LSH_ASSIGN:
     case PRIM_RSH_ASSIGN:
     case PRIM_AND_ASSIGN:
     case PRIM_OR_ASSIGN:
     case PRIM_XOR_ASSIGN:
      if (isFirstActual)
      {
        return false;
      }
      return true;

     case PRIM_ADDR_OF:
     case PRIM_SET_REFERENCE:
      return false; // See Note #2.

     case PRIM_PRIVATE_BROADCAST:
      // The operand is used by name (it must be a manifest constant).
      // Thus it acts more like an address than a value.
      return false;

     case PRIM_CHPL_COMM_GET:
     case PRIM_CHPL_COMM_PUT:
     case PRIM_CHPL_COMM_ARRAY_GET:
     case PRIM_CHPL_COMM_ARRAY_PUT:
     case PRIM_CHPL_COMM_GET_STRD:
     case PRIM_CHPL_COMM_PUT_STRD:
      // ('comm_get/put' locAddr locale widePtr len)
      // The first and third operands are treated as addresses.
      // The second and fourth are values
      if (se == call->get(2) || se == call->get(4))
      {
        return true;
      }
      return false;

     case PRIM_CHPL_COMM_REMOTE_PREFETCH:
      // comm prefetch locale widePtr len
      // second argument is an address
      // first and third are values.
      if (isFirstActual || se == call->get(3))
      {
        return true;
      }
      return false;

     case PRIM_SET_MEMBER:
      // The first operand works like a reference, and the second is a field
      // name.  Only the third is a replaceable use.
      if (se == call->get(3))
      {
        return true;
      }
      return false;

     case PRIM_GET_MEMBER:
     case PRIM_GET_MEMBER_VALUE:
      if (isFirstActual)
      {
        return false;
      }
      return true;

     case PRIM_ARRAY_SET:
     case PRIM_ARRAY_SET_FIRST:
     case PRIM_ARRAY_GET:
     case PRIM_ARRAY_GET_VALUE:
      // The first operand is treated like a reference.
      if (isFirstActual)
      {
        return false;
      }
      return true;

     case PRIM_SET_UNION_ID:
      // The first operand is treated like a reference.
      if (isFirstActual)
      {
        return false;
      }

      return true;
    }
  }

  return true;
}

// Perform updates that effect the copyPropagation transformation.
static void propagateCopies(std::vector<SymExpr*>& symExprs,
                            AvailableMap& available)
{
  // Scan the SymExprs in the current block, looking for ones that can be replaced.
  for_vector(SymExpr, se, symExprs)
  {
    if (se->isRef()) continue;

    // Replace an alias with its definition, using the current set of
    // available pairs.
    if (isUse(se))
    {
      // See if there is an (alias,def) pair.
      AvailableMap::iterator alias_def_pair = available.find(se->symbol());
      // If so, replace the alias with its definition.
      if (alias_def_pair != available.end())
      {
        DEBUG_COPYPROP("Replacing %s[%d] with %s[%d]\n",
               alias_def_pair->first->name, alias_def_pair->first->id,
               alias_def_pair->second->name, alias_def_pair->second->id);

        INT_ASSERT(alias_def_pair->first != alias_def_pair->second);
        se->setSymbol(alias_def_pair->second);
        ++s_repl_count;
      }
    }
  }
}


static void
removeAvailable(AvailableMap& available, ReverseAvailableMap& ravailable,
                Symbol* sym)
{
  std::vector<Symbol*> to_kill;

  // Remove the pair (sym, ?).
  AvailableMap::iterator ami = available.find(sym);
  if (ami != available.end())
  {
    DEBUG_COPYPROP("Removing (%s[%d], %s[%d])\n",
      ami->first->name, ami->first->id, ami->second->name, ami->second->id);
    available.erase(ami);
  }

  // Look up the pairs whose RHSs match sym.
  ReverseAvailableMap::iterator rami = ravailable.find(sym);
  if (rami != ravailable.end())
  {
    // Traverse the list of LHSs stored in the reverse map, and remove them.
    ReverseMapList& rml = rami->second;
    for (ReverseMapList::iterator i = rml.begin(); i != rml.end(); ++i)
    {
      AvailableMap::iterator ami = available.find(*i);
      if (ami != available.end())
      {
        DEBUG_COPYPROP("Removing (%s[%d], %s[%d])\n",
          ami->first->name, ami->first->id, ami->second->name, ami->second->id);
        available.erase(ami);
      }
    }
    ravailable.erase(rami);
  }
}


// Invalidate available copies based on defs
// Also if a reference to a variable is taken since the reference
// can be assigned.
static void removeKilledSymbols(std::vector<SymExpr*>& symExprs,
                                AvailableMap& available,
                                ReverseAvailableMap& ravailable,
                                std::set<Symbol*>& liveRefs)
{
  for_vector(SymExpr, se, symExprs)
  {
    // If se was in a (deref se) replaced by the value se points to, then se
    // has been removed from the tree.
    // At present, we assume the value is "just" a value, that reading it is
    // not a definition.  So here it can be ignored.
    if (! se->parentExpr)
      continue;
    if (se->isRef()) continue;

    if (needsKilling(se, liveRefs))
      removeAvailable(available, ravailable, se->symbol());
  }
}


// Returns true if the given symbol may be volatile, false otherwise.
static bool maybeVolatile(SymExpr* se)
{
  // Compile-time constants (immediates) have immutable values.
  // Expression folding should have turned all params into immediates before
  // this pass.
  if (isVarSymbol(se->symbol()) && toVarSymbol(se->symbol())->immediate)
    return false;

  // If the symbol is declared as "concurrently accessed" then it is definitely
  // volatile.
  if (se->symbol()->hasFlag(FLAG_CONCURRENTLY_ACCESSED))
    return true;

  // If the symbol is not defined in this function, then it is in a shared
  // (module or global) scope (and not const).  We assume that it can be
  // accessed concurrently by other threads, so it is volatile.
  // See Note #1.

  // The function containing the SymExpr referencing the variable.
  Symbol* fn = se->parentSymbol; 
  // Where the variable is defined.
  Symbol* defScope = se->symbol()->defPoint->parentSymbol;
  if (defScope != fn)
    return true;

  if (se->symbol()->type->symbol->hasFlag(FLAG_COPY_MUTATES))
    return true;

  return false;
}

// Insert pairs into available copies map
static void extractCopies(Expr* expr,
                          AvailableMap& available,
                          ReverseAvailableMap& ravailable,
                          std::set<Symbol*>& liveRefs)
{
  // We're only interested in call expressions.
  if (CallExpr* call = toCallExpr(expr))
  {
    // Only the move primitive creates an available pair.
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhe = toSymExpr(call->get(1)); // Left-Hand Expression
      Symbol* lhs = lhe->symbol(); // Left-Hand Symbol

      if (SymExpr* rhe = toSymExpr(call->get(2))) // Right-Hand Expression
      {
        Symbol* rhs = rhe->symbol(); // Right-Hand Symbol

        // We would like assume there are no trivial assignments in the tree.
        // INT_ASSERT(lhs != rhs);
        // But we cannot.  "Old-style" assignment (at least) leaves them in, so
        // for now we just ignore these.
        if (lhs == rhs)
          return; // Do not save trivial pairs.

        // Don't attempt to do anything with references.
        if (lhs->isRef() || rhs->isRef()) return;

        // We can't make substitutions if the lhs or rhs may change at any
        // time.
        if (maybeVolatile(lhe) || maybeVolatile(rhe))
          return; // Not a reliable pair.

        if (liveRefs.find(lhs) == liveRefs.end() &&
            liveRefs.find(rhs) == liveRefs.end()) {
          // Create the pair lhs <- rhs.
          DEBUG_COPYPROP("Creating pair (%s[%d], %s[%d])\n",
                 lhs->name, lhs->id, rhs->name, rhs->id);
          available.insert(AvailableMapElem(lhs, rhs));
          ravailable[rhs].push_back(lhs);
        }
      }
    }
  }
}


// In my version, I compute defs and uses on the fly.
// This version expands upon the standard implementation outlined in Muchnick,
// to handle references and symbols that are "volatile" in the C++ sense:
// "volatile is a hint ... [that] the value of the object might be changed by
// means undetectable by an implementation."
// Symbols that are not local to the current function (e.g. globals) are
// assumed to be volatile, as are symbols marked explicitly with the
// "concurrently accessed" flag.  It should be possible to determine statically
// if a variable can be accessed concurrently, so we assume that a separate
// pass has already marked such symbols.
static void
localCopyPropagationCore(BasicBlock*          bb,
                         AvailableMap&        available,
                         ReverseAvailableMap& ravailable,
                         std::set<Symbol*>& liveRefs)
{
  for_vector(Expr, expr, bb->exprs)
  {
#if DEBUG_CP
    if (debug > 0)
      nprint_view(expr);
#endif

    std::vector<SymExpr*> symExprs;

    // TODO: do not collect references or symbols that are not LcnSymbols
    collectSymExprs(expr, symExprs);

    propagateCopies(symExprs, available);

    removeKilledSymbols(symExprs, available, ravailable, liveRefs);

    extractCopies(expr, available, ravailable, liveRefs);
  }
}


//
// Apply local copy propagation to basic blocks of function
//
size_t localCopyPropagation(FnSymbol* fn)
{
  BasicBlock::buildBasicBlocks(fn);
  std::set<Symbol*> liveRefs;

  s_repl_count     = 0;

  for_vector(BasicBlock, bb1, *fn->basicBlocks)
  {
    AvailableMap available;
    ReverseAvailableMap ravailable;
    localCopyPropagationCore(bb1, available, ravailable, liveRefs);
  }

  return s_repl_count;
}


//#############################################################################
//# GLOBAL COPY PROPAGATION
//#############################################################################


static void createPairSet(std::vector<BitVec*>& set,
                          size_t                nbbs,
                          size_t                size)
{
  // Create a BitVec of length size for each block.
  for (size_t i = 0; i < nbbs; ++i)
    set.push_back(new BitVec(size));
}


static void destroyPairSet(std::vector<BitVec*> set)
{
  for_vector(BitVec, vec, set)
    delete vec, vec = 0;
}


// Here, we run local copy propagation again, to extract the available
// pairs that are live at the end of each BB.  These are concatenated into
// one long vector: availablePairs.
// The ending index for each block is stored in ends[i].
static void extractAvailablePairs(FnSymbol* fn,
                                  std::vector<AvailablePair>& availablePairs, 
                                  std::vector<size_t>& ends)
{
  std::set<Symbol*> liveRefs;
  for_vector(BasicBlock, bb1, *fn->basicBlocks)
  {
    // Run local copy propagation to extract live pairs at the end of each block.
    AvailableMap available;
    ReverseAvailableMap ravailable;
    localCopyPropagationCore(bb1, available, ravailable, liveRefs);

    // Record those live pairs in successive elements in availablePairs.
    for (AvailableMap::iterator i = available.begin();
         i != available.end();
         ++i)
    {
      AvailablePair pair = AvailablePair(i->first, i->second);
      availablePairs.push_back(pair);
    }
    ends.push_back(availablePairs.size());
  }
}


// Compute the bit-vector of expressions that are killed in each block.
// This is done by traversing the expressions in block i and then noting
// which of the pairs in the whole set of available pairs will be killed by
// them.
// Note that due to the possibility of loops, an earlier block can kill a pair
// that is defined later.
static void computeKillSets(FnSymbol* fn,
                            std::vector<AvailablePair>& availablePairs,
                            std::vector<BitVec*>& KILL,
                            std::set<Symbol*>& liveRefs)
{
  size_t nbbs = fn->basicBlocks->size();
  for (size_t i = 0; i < nbbs; ++i)
  {
    BasicBlock* bb2 = (*fn->basicBlocks)[i];

    // Collect up the set of symbols killed in this block in killSet.
    std::set<Symbol*> killSet;
    for_vector(Expr, expr, bb2->exprs)
    {
      std::vector<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);

      for_vector(SymExpr, se, symExprs)
      {
        if (se->isRef()) continue;
        // Invalidate a symbol if it is redefined.
        if (needsKilling(se, liveRefs))
          killSet.insert(se->symbol());
      }
    }

    // Use killSet to initialize the KILL set for this block.
    // It's OK if we include the pairs from this block in KILL[i] because we
    // put them back when we add in the COPY set.
    for (size_t j = 0; j < KILL[i]->size(); ++j)
      if (killSet.find(availablePairs[j].first) != killSet.end() ||
          killSet.find(availablePairs[j].second) != killSet.end())
        KILL[i]->set(j);
  }
}


// The bit vector in COPY[i] is set to represent the available pairs generated
// in basicBlock[i].
// Since pairs are inserted sequentially into availablePairs, this will be a
// run or zero or more contiguous bits for each block.
// ends[i] stores the next available index after block i, so the run of bits
// is the range ends[i-1]..ends[i] - 1.
static void initCopySets(std::vector<BitVec*>& COPY, std::vector<size_t>& ends,
                         size_t nbbs)
{
  size_t j = 0;
  for(size_t i = 0; i < nbbs; ++i) 
  {
    // Initialize each copy set: Just set the string of bits corresponding to
    // the pairs generated in block i.
    while (j < ends[i])
      COPY[i]->set(j++);
  }
}


// In the textbook algorithm, the IN set for the entry block is set to all
// zeroes, and the IN set for all others is set to all ones.
// In other words, we should not have to look at the function's basic blocks to
// perform this initialization.
// However, This is based on the assumption that only the entry block has no
// predecessors.  Due to how we currently perform extract basic blocks and
// perform inlining, it is not always true in our AST.
// When these are corrected and the test becomes true, then we can drop back
// to the simpler form given here:
#ifdef INLINING_DOES_NOT_LEAVE_INTERNAL_BASIC_BLOCKS_WITHOUT_PREDECESSORS
static void initInSets(std::vector<BitVec*>& IN, FnSymbol* fn)
{
  size_t nbbs = fn->basicBlocks->size();

  // Note that we start with i = 1, so that IN[0] is left as all zeroes.
  for (size_t i = 1; i < nbbs; i++)
    IN[i]->set();
}
#else
static void initInSets(std::vector<BitVec*>& IN, FnSymbol* fn)
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
#endif


//
// Apply global copy propagation to basic blocks of function
//
// The set of all available pairs in the function is represented by the vector
// availablePairs.  The sets COPY, KILL, IN, and OUT are represented by bit
// vectors.  A '1' bit at a given position in one of these sets means that the
// pair at the corresponding location is availablePairs is live.
//
// COPY[i] -- The set of pairs that are live at the end of basic block i.
// KILL[i] -- The set of pairs that are killed by block i.
// IN[i] -- The set of pairs that are live at the beginning of block i.
// OUT[i] -- The set of pairs that are live at the end of block i.
//
// I removed the test for nbbs <= 1, to make global propagation perform local
// copy propagation as well.  That means that if global copy propagation is
// run, then running local copy propagation either immediately before or
// immediately after it would be redundant.
//
size_t globalCopyPropagation(FnSymbol* fn) {
  BasicBlock::buildBasicBlocks(fn);
  std::set<Symbol*> liveRefs;

  size_t                     nbbs = fn->basicBlocks->size();

  std::vector<AvailablePair> availablePairs;
  std::vector<size_t>        ends;

  extractAvailablePairs(fn, availablePairs, ends);

  size_t size = availablePairs.size();

#if DEBUG_CP
  if (debug > 0)
  {
    printf("\n");
    list_view(fn);

    BasicBlock::printBasicBlocks(fn);
  }
#endif

  std::vector<BitVec*> COPY;
  std::vector<BitVec*> KILL;
  std::vector<BitVec*> IN;
  std::vector<BitVec*> OUT;

  createPairSet(COPY, nbbs, size);
  createPairSet(KILL, nbbs, size);
  createPairSet(IN,   nbbs, size);
  createPairSet(OUT,  nbbs, size);

  initCopySets(COPY, ends, nbbs);

  computeKillSets(fn, availablePairs, KILL, liveRefs);

  initInSets(IN, fn);

#if DEBUG_CP
  if (debug > 0)
  {
    printf("COPY:\n"); BasicBlock::printBitVectorSets(COPY);
    printf("KILL:\n"); BasicBlock::printBitVectorSets(KILL);
  }
#endif

  BasicBlock::forwardFlowAnalysis(fn, COPY, KILL, IN, OUT, true);

#if DEBUG_CP
  if (debug > 0)
  {
    printf("IN:\n");  BasicBlock::printBitVectorSets(IN);
    printf("OUT:\n"); BasicBlock::printBitVectorSets(OUT);
  }
#endif

  // This is the main loop.
  // Use the set IN[i] to initialize the available pairs at the top of a
  // block.  Then call local copy propagation to perform legal substitutions
  // within a block.
  // The local copy propagation algorithm will invalidate pairs when their LHS
  // or RHS is overwritten, so the available set remains correct as the block
  // is traversed.  As a check, the contents of the available set following
  // traversal should match the precomputed OUT[i] for the same block.
  s_repl_count =  0; // Zero out counters.
  for (size_t i = 0; i < nbbs; i++) {
    BasicBlock* bb = (*fn->basicBlocks)[i];
    AvailableMap available;
    ReverseAvailableMap ravailable;

    for (size_t j = 0; j < availablePairs.size(); ++j)
    {
      if (IN[i]->get(j))
      {
        AvailablePair& ap = availablePairs[j];
        // Two available pairs at the start of a basic block should not have
        // the same LHS, because one should kill the other.
        // Also, this makes arbitrary the choice of which one survives.
        INT_ASSERT(available.find(ap.first) == available.end());
        available.insert(ap);
        ravailable[ap.second].push_back(ap.first);
      }
    }

    if (available.size() > 0)
    {
      localCopyPropagationCore(bb, available, ravailable, liveRefs);
      // Here, as a check, we could subtract from available the pairs
      // corresponding to the true bits in OUT[i].  The expectation is that all
      // and only those pairs remain.
    }
  }

  destroyPairSet(COPY);
  destroyPairSet(KILL);
  destroyPairSet(IN);
  destroyPairSet(OUT);

  return s_repl_count;
}

void copyPropagation(void) {
  if (!fNoCopyPropagation) {
    forv_Vec(FnSymbol, fn, gFnSymbols)
    {
      // BHARSH INIT TODO: Can this be eliminated now that tuples no longer use
      // constructors?
      //
      // This test is necessary because extern function stubs may contain
      // _construct__tuple calls that are unresolved.
      if (fn->hasFlag(FLAG_EXTERN))
        continue;

      localCopyPropagation(fn);
      if (!fNoDeadCodeElimination)
        deadVariableElimination(fn);

      // Iterate GCP with dead code elimination.
      while (globalCopyPropagation(fn) > 0)
      {
        if (!fNoDeadCodeElimination)
          deadVariableElimination(fn);
      }
    }
  }
}


//#############################################################################
//# NOTES:
//#
//# Note #1: Maybe require volatile variables to be marked explicitly with the
//# "concurrently accessed" flag.
//# In a multithreaded programming model especially, it may be unduly
//# pessimistic to prevent copy propagation merely because a variable is
//# shared.  Copy propagation assumes that the value has not changed, which is
//# tantamount to delaying recognition of an external change.  In many
//# routines, this delay is acceptable and may result in simpler code.  
//# One place where the delay would not be acceptable is in a tight loop that
//# is waiting for the named variable to change.  In that case, CP might
//# replace the variable with a constant, and execution will get stuck in an
//# infinite loop.
//#
//# Note #2: Prohibit copy replacement in an 'addr_of' primitive.
//# Supposing we have
//#  (move y x)
//#  (move ry (addr_of y))
//#  (foo ry) // Alters y
//#  (bar x) // Expects unchanged x.
//# it is probably safest to avoid substituting x for y in the addr_of.
//# The copy of x into y may be intentional, because one or the other is about
//# to be changed.  Allowing the substitution collapses the two variables onto
//# one.
//#

