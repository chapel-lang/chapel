/*
 * Copyright 2004-2015 Cray Inc.
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

//# OwnershipFlowManager.cpp -*- C++ -*-
//#########################################################################
//#

#include "OwnershipFlowManager.h"


#include "astutil.h"
#include "stmt.h"
#include "expr.h"

#include "resolution.h" // for autoCopyMap and autoDestroyMap.
#include "passes.h" // for insertReferenceTemps

// TODO: This should be moved to symbol.cpp
bool fWarnOwnership = false;


//######################### Alias list utilities #########################
// Optimization note: When optimization is turned on, GCC is probably smart
// enough to inline and coalesce adjacent loops (see e.g. the adjacent calls to
// resetAliasList and setAliasList at the end of processConsumer), even without
// the "inline" hint.  If so, inlining these by hand would be a waste of time.

static inline void
setAliasList(BitVec* bits, SymbolVector& aliasList,
             OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  for_vector(Symbol, alias, aliasList)
  {
    size_t index = symbolIndex[alias];
    bits->set(index);
  }
}


static inline void
resetAliasList(BitVec* bits, SymbolVector& aliasList,
               OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  for_vector(Symbol, alias, aliasList)
  {
    size_t index = symbolIndex[alias];
    bits->reset(index);
  }
}


//########################################################################
//# Scope utilities
//#
//# TODO: See if these are defined elsewhere.
//#

static BlockStmt* getScopeBlock(Expr* expr)
{
  // If this is a DefExpr and the symbol is a formal, then we use the body of
  // the function as the scope block.
  if (DefExpr* def = toDefExpr(expr))
  {
    if (isArgSymbol(def->sym))
      return toFnSymbol(def->parentSymbol)->body;

    // Fall through.
    // That is, we treat DefExprs containing VarSymbols like any other
    // expression and search for scopes using the loop below.
  }

  // Traverse all scopes containing this expression, up to the root.
  while (expr)
  {
    BlockStmt* block = toBlockStmt(expr);
    if (block && ! (block->blockTag & BLOCK_SCOPELESS))
      // This is a scoped block containing the Expr, so this is what we want.
      return block;

    expr = expr->parentExpr;
  }

  INT_FATAL(expr, "Expression is not associated with any scope.");
  return NULL;
}


// Returns true if expr is a parent expr of other; false otherwise.
// The comparison is not strict (an Expr be its own parent);
// use equality to disambiguate.
static bool isParentExpr(Expr* expr, Expr* other)
{
  while (other)
  {
    if (other == expr)
      return true;

    other = other->parentExpr;
  }
  return false;
}


// Scans the body of the given function and inserts all of the variable and
// argument symbols defined in it into the vector of symbols.  Bits in the flow
// analysis bit-vectors correspond to the entries in this vector.
// Also constructs an index map, to make it easier to find the index of a
// symbol in the vector.  (Otherwise, a linear search is required.)
// The alias map can also be populated at the same time.  
void
OwnershipFlowManager::extractSymbols()
{
  DefExprVector defExprs;
  collectDefExprs(_fn, defExprs);

  for_vector(DefExpr, def, defExprs)
  {
    Symbol* sym = def->sym;

    // We are interested only in arguments and variables.
    if (! (toArgSymbol(sym) || toVarSymbol(sym)))
      continue;

    Type* type = sym->type;

    // TODO: Extern record types also do not have constructors and
    // destructors.  To treat them uniformly, we would have to enforce that
    // extern types supply constructors and destructors and/or supply them
    // internally.
    TypeSymbol* ts = type->symbol;
    if (ts->hasFlag(FLAG_EXTERN))
      continue;

    // We are concerned only with record types.
    // TODO: This is too bad, because it would be nice to be able to treat all
    // value types uniformly.  But for that to work, arguments of fundamental
    // type must be constructed by having their values piped through a copy
    // constructor.  Currently, that is not the case.

    // We are interested only in records passed by value.  Records passed by
    // ref appear be a class in the current AST because _ref(T) is a class type.
    AggregateType* at = toAggregateType(type);
    if (at == NULL)
      // Not an aggregate type, so not a record.
      continue;
    if (!at->isRecord())
      // Not a record.
      continue;

    symbolIndex.insert(SymbolIndexElement(sym, symbols.size()));
    symbols.push_back(sym);

    // We expect the symbolIndex to return the index of that symbol in the
    // symbols vector.
    INT_ASSERT(symbols[symbolIndex[sym]] == sym);

    // Initialize each entry in the alias map with a list of symbols
    // containing just the symbol itself.
    aliases.insert(sym);
  }

  nsyms = symbols.size();
}


static void createAlias(SymExpr* se, AliasVectorMap& aliases)
{
  CallExpr* call = toCallExpr(se->parentExpr);

  SymExpr* lhs = toSymExpr(call->get(1));
  SymExpr* rhs = toSymExpr(call->get(2));

  // Workaround (See Note #2).

  Symbol* lsym = lhs->var;
  Symbol* rsym = rhs->var;

  // All members of an alias clique must belong to the same scope.  Otherwise,
  // the destruction of one in an inner scope may cause the the one still
  // accessible in an outer scope to become corrupted (read-after-free).
  BlockStmt* lscope = getScopeBlock(lsym->defPoint);
  BlockStmt* rscope = getScopeBlock(rsym->defPoint);
  if (lscope != rscope)
    return;

  // Merge aliases whether or not they are live.
  aliases.merge(rsym, lsym);
}


void 
OwnershipFlowManager::populateStmtAliases(OwnershipFlowManager::SymExprVector& symExprs)
{
  for_vector(SymExpr, se, symExprs)
  {
    // We are only interested in local symbols, so if this one does not appear
    // in our map, move on.
    Symbol* sym = se->var;
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    // We treat the return value variable specially.  It cannot participate in
    // alias cliques because it can be assigned more than once and that causes
    // two unrelated cliques to be joined.
    if (sym == toFnSymbol(se->parentSymbol)->getReturnSymbol())
      continue;

    // Pass ownership to this symbol if it is the result of a bitwise copy.
    if (bitwiseCopyArg(se) == 1)
      // It suffices to create the alias when the se is the left operand (1),
      // though it probably does no harm to do this symmetrically.
      createAlias(se, aliases);
  }
}


void 
OwnershipFlowManager::populateAliases()
{
  for_vector(BasicBlock, bb, *basicBlocks)
  {
    for_vector(Expr, expr, bb->exprs)
    {
      SymExprVector symExprs;
      collectSymExprs(expr, symExprs);
      populateStmtAliases(symExprs);
    }
  }
}


//
// computeTransitions
//


// Track when a local symbol gains ownership of an object.
static void processCreator(SymExpr* se, 
                           BitVec* prod, BitVec* live,
                           const AliasVectorMap& aliases,
                           OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  // Any function returning a value is considered to be a constructor.

  Symbol* sym = se->var;

  // Ignore symbols that are not in our set-of-interest.
  if (symbolIndex.find(sym) == symbolIndex.end())
    return;

  size_t index = symbolIndex[sym];

  // We expect that each symbol gets constructed only once, so if we are
  // about to set a bit in the prod set, it cannot already be true.
  // If this assumption turns out to be false, it means we are reusing
  // symbols.  That case can be accommodated, but it means we should
  // insert a destructor call ahead of the symbol's reinitialization.
  if (prod->get(index))
    if (fWarnOwnership)
      USR_WARN(sym, "Reinitialization of sym");

  // When one member of an alias set is defined, we treat them all as being
  // defined.
  // Otherwise, backward flow will lead to premature deletion:
  //  1:  0  >  5 
  // move( call_tmp call( fn newAlias arg this ) ) 
  // move( ret call_tmp ) 
  // goto 806806   _end_reindex 
  //  4:  2 3  >  5 
  // move( call_tmp call( fn _newArray x ) ) 
  // move( ret call_tmp ) 
  //  5:  4 1  >  
  // def _end_reindex 
  // return( ret ) 
  // In blocks 1 and 4, the respective call_tmp becomes live and is aliased
  // with ret.  But only the first call_tmp is live at the end of block 1 and
  // only the second call_tmp is live at the end of block 4: they will not flow
  // through to block 5.  After flow analysis (unless we treat the members of a
  // clique equivalently), each call_tmp will be destroyed within its
  // respective block, making ret unowned on exit -- an error.
  // TODO: I think we can represent all members of an alias set with one bit in
  // the flow sets -- either all are owned or none are.  This will save time
  // and space.
  SymbolVector* aliasList = aliases.at(sym);
  setAliasList(prod, *aliasList, symbolIndex);
  setAliasList(live, *aliasList, symbolIndex);
}


static void processBitwiseCopy(SymExpr* se,
                               BitVec* prod, BitVec* live, BitVec* cons,
                               const AliasVectorMap& aliases,
                               OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  CallExpr* call = toCallExpr(se->parentExpr);

  SymExpr* lhs = toSymExpr(call->get(1));
  SymExpr* rhs = toSymExpr(call->get(2));

  Symbol* lsym = lhs->var;
  Symbol* rsym = rhs->var;

  size_t lindex = symbolIndex[lsym];
  // The rhs must contain a local symbol, otherwise we ignore it.
  if (symbolIndex.find(rsym) == symbolIndex.end())
    // The lhs will be considered unowned, which is correct because it
    // is a bitwise copy of the value of a global.
    return;
  size_t rindex = symbolIndex[rsym];

  SymbolVector* laliasList = aliases.at(lsym);
  SymbolVector* raliasList = aliases.at(rsym);

  // Ownership is shared if the symbols are in the same alias class.
  // Ownership is transferred to the lhs if the lhs is in an outer scope.
  // Ownership is not transferred nor shared if the lhs is in an inner scope.

  // If the two alias lists are identical, then the two symbols are in the
  // same alias clique.  In that case, ownership is shared.
  if (laliasList == raliasList)
  {
    // We set and reset the live bits for the members of an alias set in concert,
    // so all we do here is assert that that has been done correctly.
    INT_ASSERT(live->get(lindex) == live->get(rindex));
  }
  else
  {
    // Otherwise, the symbols are in different cliques.
    BlockStmt* lscope = getScopeBlock(lsym->defPoint);
    BlockStmt* rscope = getScopeBlock(rsym->defPoint);
    if (isParentExpr(lscope, rscope))
    {
      bool owned = live->get(rindex);

      // lsym is defined in an outer scope, so we can transfer ownership from
      // rsym's clique to lsym's clique.
      // When that happens, we clear the live bit and set
      // the cons bit for each member of the RHS clique, 
      // and then set the live and prod bits for each member of the LHS clique.
      resetAliasList(live, *raliasList, symbolIndex);
      setAliasList(cons, *raliasList, symbolIndex);

      // Now set the produce bit for each member of the lhs clique, but only if the RHS
      // was owned.
      if (owned)
      {
        setAliasList(prod, *laliasList, symbolIndex);
        setAliasList(live, *laliasList, symbolIndex);
      }
    }
  }
}


// If thsi call uses the given symbol, then add that symbol and all its aliases
// to the use set.
static void processUser(SymExpr* se, BitVec* use,
                        const AliasVectorMap& aliases,
                        OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  // All members of an alias clique point to the same SymbolVector, so we only
  // need to look up one arbitrarily and then run the list.
  Symbol* sym = se->var;
  SymbolVector* aliasList = aliases.at(sym);
  setAliasList(use, *aliasList, symbolIndex);
}


// If this call acts like a destructor, then add the symbols it affects to the
// cons set and remove them from the prod set.
static void processConsumer(SymExpr* se, 
                            BitVec* live, BitVec* cons,
                            const AliasVectorMap& aliases,
                            OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  // All members of an alias clique point to the same SymbolVector, so we only
  // need to look up one arbitrarily and then run the list.
  Symbol* sym = se->var;
  SymbolVector* aliasList = aliases.at(sym);
  resetAliasList(live, *aliasList, symbolIndex);
  setAliasList(cons, *aliasList, symbolIndex);
}


void
OwnershipFlowManager::computeTransitions(SymExprVector& symExprs,
                                         BitVec* prod, BitVec* live,
                                         BitVec* use, BitVec* cons)
{
  for_vector(SymExpr, se, symExprs)
  {
    // We are only interested in local symbols, so if this one does not appear
    // in our map, move on.
    Symbol* sym = se->var;
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    if (isCreated(se))
      processCreator(se, prod, live, aliases, symbolIndex);

    if (bitwiseCopyArg(se) == 1)
    {
      processBitwiseCopy(se, prod, live, cons, aliases, symbolIndex);

      // When the RVV is on the LHS of an assignment, it is always considered
      // to be owned.
      FnSymbol* fn = toFnSymbol(se->parentSymbol);
      if (sym == fn->getReturnSymbol())
      {
        SymbolVector* laliasList = aliases.at(sym);
        setAliasList(prod, *laliasList, symbolIndex);
        setAliasList(live, *laliasList, symbolIndex);
      }
    }

    if (isUsed(se))
      processUser(se, use, aliases, symbolIndex);

    if (isConsumed(se))
      processConsumer(se, live, cons, aliases, symbolIndex);
  }
}


// Look for expressions that create, destroy or transfer ownership of a record
// object.
// Right now, we can get away with just looking for MOVE or ASSIGN primitives
// to determine where ownership is created or transferred.  When constructors
// become methods, we'll key off the CONSTRUCTOR flag and modify the state of
// the first (receiver) argument.
void
OwnershipFlowManager::computeTransitions(BasicBlock& bb,
                                         BitVec* prod, BitVec* live,
                                         BitVec* use, BitVec* cons)
{
  for_vector(Expr, expr, bb.exprs)
  {
    // Are all expressions in this list mutually exclusive?
    // Can we speed things up by processing only statement expressions?

    OwnershipFlowManager::SymExprVector symExprs;
    collectSymExprs(expr, symExprs);

    computeTransitions(symExprs, prod, live, use, cons);
  }
}


void
OwnershipFlowManager::computeTransitions()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    // On the first pass, we compute live and then throw it away.
    BitVec* live = new BitVec(PROD[i]->size());
    computeTransitions(*(*basicBlocks)[i], PROD[i], live, USE[i], CONS[i]);
    delete live;
  }
}


//////////////////////////////////////////////////////////////////////////
// computeExits
//
// For each symbol in our symbol set, find the basic block whose end coincides
// with the end-of-scope for that symbol.
// We do this by creating a map from blocks to basic block IDs: scope -> bbID.
// We traverse the statements in each basic block and for each block
// expression containing that statement, the corresponding index is updated to
// the index of the current basic block.  When this pass is complete, each
// element of the map will contain the ID of the final BB the lies entirely
// within the scope it defines.
// Then, for each symbol in our symbol set,
//   Find the block (scope) in which it is declared.
//   Look up the bbID for the final bb in that scope.
//   Set the corresponding bit: EXIT[bbID]->set(symbolID)
static void
computeScopeToLastBBIDMap(FnSymbol* fn,
                          std::map<BlockStmt*, size_t>&scopeToLastBBIDMap)
{
  size_t nbbs = fn->basicBlocks->size();
  BasicBlock::BasicBlockVector& bbs = *fn->basicBlocks;
  for (size_t i = 0; i < nbbs; ++i)
  {
    // Walk the expressions in this block.
    std::vector<Expr*>& exprs = bbs[i]->exprs;
    for_vector(Expr, expr, exprs)
    {
      // Now, for each scope up the chain, mark that this basic block (i) is the
      // last one in it.  In loops, the last block belonging to the outer scope
      // cannot be one that is executed repeatedly.  Otherwise, variables
      // belonging to an outer scope will be freed repeatedly, which is not what
      // we want.
      // If blocks were properly nested, we would not have to do this, but since
      // the end of several blocks may lie between two adjacent statements, we
      // have to go up the chain and mark them all.
      while ((expr = expr->parentExpr))
      {
        if (BlockStmt* block = toBlockStmt(expr))
        {
          if (! (block->blockTag & BLOCK_SCOPELESS))
            scopeToLastBBIDMap[block] = i;
          if (isRepeatedInLoop(block))
            break;
        }
      }
    }
  }
}


// We need to find the DefExprs corresponding to each symbol on our list, so
// just traverse all DefExprs in the function and then look up the ones we're
// interested in.
void
OwnershipFlowManager::computeExits(std::map<BlockStmt*, size_t>& scopeToLastBBIDMap)
{
  std::vector<DefExpr*> defExprs;
  collectDefExprs(_fn, defExprs);
  for_vector(DefExpr, def, defExprs)
  {
    Symbol* sym = def->sym;
    OwnershipFlowManager::SymbolIndexMap::iterator simelt = symbolIndex.find(sym);

    // We only care about symbols in our selected set.
    if (simelt == symbolIndex.end())
      continue;

    // Get the symbol index out of the symbolIndexMap element.
    size_t symID = simelt->second;

    // Look for the closest scope block enclosing the DefExpr.
    // Is there a utility for this?
    BlockStmt* block = getScopeBlock(def);

    // Workaround: Some passes leave shrapnel in the tree, including DefExprs
    // for symbols that are never accessed.  We would normally expect to find a
    // last basic block for every DefExpr, but if the DefExpr appears in a
    // block that contains no executable statements, then because basic block
    // analysis harvests only statements (not DefExprs), that block never has a
    // last BBID established for it.
    // The workaround can be removed if we make sure that DefExprs for symbols
    // that are never actually accessed are removed from the treee before we
    // reach this point in the translation.
    std::map<BlockStmt*, size_t>::iterator item = scopeToLastBBIDMap.find(block);
    if (item != scopeToLastBBIDMap.end())
    {
      size_t bbID = item->second;
      EXIT[bbID]->set(symID);
    }
  }
}


void
OwnershipFlowManager::computeExits()
{
  std::map<BlockStmt*, size_t> scopeToLastBBIDMap;

  computeScopeToLastBBIDMap(_fn, scopeToLastBBIDMap);

  computeExits(scopeToLastBBIDMap);
}


// We use backward flow to determine the last block in each execution path in
// which a given symbol is used.
// We only care about the last use: we assume that either variables are not
// redefined (single-assignment) or an owned variable is properly consumed
// where it is redefined.
//
// First, we use IN and OUT as temporary sets to flow future use backwards.  If
// a use in one block sets the corresponding OUT bit in a preceding block, that
// means the preceding block is not the last use.  All OUT sets are initially
// empty.  Then we compute:
//  IN[i] = OUT[i] + USE[i]
//  OUT[i] += IN[j], for every j a successor of block i.
//  and iterate.
// Next, we cull bits from the USE set in blocks where there is a correponding
// OUT bit:
//  USE[i] -= OUT[i]
//
void 
OwnershipFlowManager::backwardFlowUse()
{
  // Assume that IN and OUT are both empty.

  bool changed;
  do {
    changed = false;

    for (size_t i = nbbs; i--; )
    {
      // Compute the OUT set for block i.
      // A true bit means that the symbol is used on all paths out of that block.
      if ((*basicBlocks)[i]->outs.size() == 0)
      {
        // This is a terminal block, so the symbol is not used in any successor
        // block (because there are none).
        OUT[i]->clear();
      }
      else
      {
        // Otherwise, OUT is the union of the IN sets in all successor blocks.
        // (If the last use of a symbol is in one or more successor branches,
        // it must flow through this block.)
        // In this computation, we ignore back edges, because that would move
        // the last use of a symbol defined within the loop past the end of the
        // loop (which is not what we want).
        BitVec& out = *OUT[i];
        out.clear();
        for_vector(BasicBlock, succ, (*basicBlocks)[i]->outs)
          // TODO: bb IDs are stored as ints (not uints). Change that.
          if (succ->id > (int) i) // Ignore self- and back-edges.
            out |= *IN[succ->id];
      }

      // Within a block, a bit in IN transitions to true if the symbol is used
      // within that block.
      BitVec new_in = *OUT[i] + *USE[i];
      if (new_in != *IN[i])
      {
        *IN[i] = new_in;
        changed = true;
      }
    }
  } while (changed);

  // Now recompute IN as the union of the OUTs of its predecessors.
  // The last use of a symbol (on a given branch) is where this IN is true and
  // OUT for the same block is false.
  for (size_t i = 0; i < nbbs; ++i)
  {
    if ((*basicBlocks)[i]->ins.size() == 0)
    {
      IN[i]->clear();
    }
    else
    {
      BitVec& in = *IN[i];
      in.clear();
      for_vector(BasicBlock, pred, (*basicBlocks)[i]->ins)
        in |= *OUT[pred->id];
    }

    *USE[i] = *IN[i] - *OUT[i];
  }
}


void
OwnershipFlowManager::forwardFlowOwnership()
{
  bool changed;
  do {
    changed = false;

    for (size_t i = 0; i < nbbs; ++i)
    {
      if ((*basicBlocks)[i]->ins.size() == 0)
      {
        // This is an initial block because it has no predecessors.
        // Its IN set must be empty.
        IN[i]->clear();
      }
      else
      {
        // Otherwise, the in set is the intersection of the OUTs of its
        // predecessors.
        // We ignore back-edges, so that ownership can flow through loop constructs.
        BitVec* in = IN[i];
        in->set();
        for_vector(BasicBlock, pred, (*basicBlocks)[i]->ins)
          // Ignore back-edges.  We assume that the entry point of a loop
          // always has a lower-numbered ID than the exit.
          if (pred->id < (int) i)
            *in &= *OUT[pred->id];
      }

      // This means: ownership reaches the end of a block if it enters or is
      // produced in the block and it is not consumed within the block.  Also,
      // if this is the last block in which the variable of interest is in
      // scope, an autoDestroy will be inserted, so in that case ownership does
      // not survive to the end of the block.
      // There is an implicit assumption here that if a producer and consumer
      // of the same variable appear within the same block, then the producer
      // precedes the consumer.  But then, the other ordering makes no sense.
      BitVec new_out = *IN[i] + *PROD[i] - *CONS[i] - *EXIT[i];
      if (new_out != *OUT[i])
      {
#if 0
        // We don't have to propagate aliases in forward flow, because
        // the bits in an alias clique are always set or reset in concert.  If
        // that assumption breaks down (because the membership in a clique
        // changes from block to block), then we either need to maintain
        // separate alias lists for each block or just re-run the block-wise
        // analysis -- whichever appears to be cheaper.
        BitVec* added = new_out - *OUT[i];
        propagateAliases(added, new_out, aliases, symbolIndex);
#endif
        *OUT[i] = new_out;
        changed = true;
      }
    }
  } while (changed);

  // We set the OUT set of any terminal block to all zeroes, to force cleanup
  // of any variables that are unused at that point. 
  // TODO: Try disabling this and see if it is needed after reworking the USE
  // computation.
#if 1
  for (size_t i = 0; i < nbbs; ++i)
  {
    if ((*basicBlocks)[i]->outs.size() == 0)
    {
      OUT[i]->clear();
    }
  }
#endif
}


// Backward flow is no longer needed.  The computation of the last-use takes
// care of determining the point at which autoDestroy calls need to be
// inserted.  Those calls are consumers, so ownership of a given symbol does
// not propagate out of a block that contains a corresponding USE bit.
// Owned symbols are eagerly destroyed in a block where their USE bit is true.
//
// In backward flow, we adjust the out set so it is the intersection of the IN
// sets of its successors.  If the block is a terminal block (a block with no
// successors), however, we set its OUT to all zeroes.  This forces ownership to
// be driven to zero before the function is exited.
// This analysis will work correctly even if the function has multiple exits.
//
// Backward flow through the blocks allows the destruction of temporaries to
// flow backward to the end of the block in which they are last used.  
void
OwnershipFlowManager::backwardFlowOwnership()
{
  bool changed;
  do {
    changed = false;

    for (size_t i = nbbs; i-- > 0; )
    {
      if ((*basicBlocks)[i]->outs.size() == 0)
      {
        // This is a terminal block because it has no successors.
        // Its OUT set must be empty.
        OUT[i]->clear();
      }
      else
      {
        BitVec* out = OUT[i];
        for_vector(BasicBlock, succ, (*basicBlocks)[i]->outs)
          *out &= *IN[succ->id];
      }
      BitVec new_in = *OUT[i] + *CONS[i] - *PROD[i];
      if (new_in != *IN[i])
      {
        *IN[i] = new_in;
        changed = true;
      }
    }
  } while (changed);
}


static bool isRetVarInReturn(SymExpr* se)
{
  if (CallExpr* call = toCallExpr(se->parentExpr))
    if (call->isPrimitive(PRIM_RETURN))
      // We just assume that that call->get(1) == se.
      // What else could it be?
      if (FnSymbol* fn = toFnSymbol(call->parentSymbol))
        if (fn->hasFlag(FLAG_CONSTRUCTOR) ||
            fn->hasFlag(FLAG_AUTO_COPY_FN) ||
            fn->hasFlag(FLAG_INIT_COPY_FN))
        return true;

  return false;
}


// Returns true if the given SymExpr is the RHS of a bitwise copy and the LHS
// is a reference to the return value variable and the containing function is a
// constructor (a.k.a. initCopy and autoCopy).
static bool isRetVarCopyInConstructor(SymExpr* se)
{
  if (CallExpr* call = toCallExpr(se->parentExpr)) // This call
    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN)) // is a bitwise copy
      if (se == call->get(2)) // whose RHS is the given SymExpr
        if (FnSymbol* fn = toFnSymbol(call->parentSymbol)) // and the
          if (fn->hasFlag(FLAG_CONSTRUCTOR) ||             // containing
              fn->hasFlag(FLAG_AUTO_COPY_FN) ||            // function 
              fn->hasFlag(FLAG_INIT_COPY_FN)) // is a constructor
            if (SymExpr* lhse = toSymExpr(call->get(1))) // whose LHS
              if (lhse->var == fn->getReturnSymbol()) // is the RVV.
                return true;

  return false;
}


static bool isDestructorFormal(SymExpr* se)
{
  if (ArgSymbol* arg = toArgSymbol(se->var))
    if (FnSymbol* fn = toFnSymbol(se->parentSymbol))
      if (fn->hasFlag(FLAG_DESTRUCTOR) ||
          fn->hasFlag(FLAG_AUTO_DESTROY_FN))
      {
        // We presume this is the first (and only) argument.
        INT_ASSERT(fn->getFormal(1) == arg);
        return true;
      }

  return false;
}


static bool isDestructorArg(SymExpr* se)
{
  if (FnSymbol* parent = toFnSymbol(se->parentSymbol))
    if (parent->hasFlag(FLAG_DESTRUCTOR))
      if (CallExpr* call = toCallExpr(se->parentExpr))
        if (FnSymbol* fn = call->isResolved())
          if (fn->hasFlag(FLAG_AUTO_DESTROY_FN))
            return true;
  return false;
}


// Insert an autoCopy because this symbol is unowned and
// a receptor formal expects its operand to be owned.
static void insertAutoCopy(SymExpr* se)
{
  SET_LINENO(se);
  Symbol* sym = se->var;
  FnSymbol* autoCopyFn = autoCopyMap.get(sym->type);

  if (autoCopyFn == NULL)
    return;

  // Prevent autoCopy functions from calling themselves recursively.
  // TODO: Remove this clause after the autoCopy function becomes a copy constructor
  // method.
  if (isRetVarInReturn(se))
    return;

  // We don't want to copy the meme argument in a constructor.
  // TODO: When constructors are methods, there will be no meme argument, so
  // this will be dead code.
  if (se->var->hasFlag(FLAG_IS_MEME))
    return;

  // We also don't want an autocopy when we assign to the RVV in a constructor.
  if (isRetVarCopyInConstructor(se))
    return;

  // The argument to a destructor function is known to be live, so we do not
  // need to insert an autoCopy even if one is called for.
  // (Alternatively, we could pre-initialize the IN[0] set so the bit
  // corresponding to the argument is true, but heading off the insertion of a
  // called-for autoCopy seems simpler.)
  if (isDestructorFormal(se))
    return;

  // Record destructors call the autoDestroy function recursively, loading up
  // each field in turn (in reverse order).  We don't want to call autoCopy on
  // these arguments, because that just undoes the effect of the field-wise
  // destructor call.
  // TODO: This problem only occurs for sync and referencecounted types.  If we
  // can remove the useRefType predicate on line 687 of callDestructors.cpp and
  // always use ref types (PRIM_GET_MEMBER), then this test can be removed.
  if (isDestructorArg(se))
    return;

  Expr* stmt = se->getStmtExpr();
  VarSymbol* tmp = newTemp("auto_copy_tmp", sym->type);
  CallExpr* autoCopyCall = new CallExpr(autoCopyFn, se->copy());
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, autoCopyCall));
  se->replace(new SymExpr(tmp));
  insertReferenceTemps(autoCopyCall);
}


static void insertAutoCopy(OwnershipFlowManager::SymExprVector& symExprs,
                           BitVec* prod, BitVec* live, BitVec* cons,
                           const AliasVectorMap& aliases,
                           OwnershipFlowManager::SymbolIndexMap& symbolIndex)
{
  for_vector(SymExpr, se, symExprs)
  {
    // We are only interested in local symbols, so if this one does not appear
    // in our map, move on.
    Symbol* sym = se->var;
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    bool seIsRVV = false;
    if (FnSymbol* fn = toFnSymbol(se->parentSymbol))
      seIsRVV = sym == fn->getReturnSymbol();

    if (bitwiseCopyArg(se) == 1)
    {
      // If the live bit is set for the RHS symbol, we can leave it as a move and
      // transfer ownership.  Otherwise, we need to insert an autoCopy.
      if (seIsRVV)
      {
        CallExpr* call = toCallExpr(se->parentExpr);
        INT_ASSERT(call && call->isPrimitive(PRIM_MOVE));
        if (SymExpr* rhse = toSymExpr(call->get(2)))
        {
          Symbol* rsym = rhse->var;
          if (symbolIndex.find(rsym) == symbolIndex.end())
          {
            // The RHS is not local, so we need an autocopy.
            insertAutoCopy(rhse);
          }
          else
          {
            // The RHS is local.  We need an autocopy only if it is unowned.
            size_t rindex = symbolIndex[rsym];
            if (!live->get(rindex))
              insertAutoCopy(rhse);
          }
        }
      }
      processBitwiseCopy(se, prod, live, cons, aliases, symbolIndex);
      continue;
    }

    // We assume that this case is handled by the above.
    if (bitwiseCopyArg(se) == 2)
      continue;

    // Set a bit in the live set if this is a constructor.
    if (isCreated(se))
      processCreator(se, prod, live, aliases, symbolIndex);

    if (isConsumed(se))
    {
      // If the live bit is set for this symbol, we can leave it as a move and
      // transfer ownership.  Otherwise, we need to insert an autoCopy.
      size_t index = symbolIndex[sym];
      if (!live->get(index))
      {
        insertAutoCopy(se);
        // We can set the bit in the PROD set to show that ownership is
        // produced, but since it is consumed immediately, the state of OUT and
        // the rest of the forward-flowed bitsets is unchanged.
        prod->set(index);
      }
      processConsumer(se, live, cons, aliases, symbolIndex);
    }
  }
}


static void insertAutoCopy(BasicBlock& bb,
                           BitVec* prod, BitVec* live, BitVec* cons,
                           const SymbolVector& symbols,
                           OwnershipFlowManager::SymbolIndexMap& symbolIndex,
                           const AliasVectorMap& aliases)
{
  for_vector(Expr, expr, bb.exprs)
  {
    OwnershipFlowManager::SymExprVector symExprs;
    collectSymExprs(expr, symExprs);

    insertAutoCopy(symExprs, prod, live, cons, aliases, symbolIndex);
  }
}                          


void
OwnershipFlowManager::insertAutoCopies()
{
  for (size_t i = 0; i < nbbs; i++)
  {
    // We need to insert an autodestroy call for each symbol that is owned
    // (live) at the end of the block but is unowned (dead) in the OUT set.
    BasicBlock& bb = *(*basicBlocks)[i];
    // For this block traversal, we initialize live = IN[i].
    BitVec* live = new BitVec(*IN[i]);
    // We use temporaries for the prod and cons sets.  The results of
    // global flow analysis should yield the same values, and this allows us to
    // test that assertion.
    BitVec* prod = new BitVec(PROD[i]->size());
    BitVec* cons = new BitVec(CONS[i]->size());
    insertAutoCopy(bb, prod, live, cons, symbols, symbolIndex, aliases);
    delete cons; cons = 0;
    delete live; live = 0;
    delete prod; prod = 0;
  }
}


// Find the end of the scope containing the given symbol and insert an
// autoDestroy on it there.
static void insertAutoDestroyAtScopeExit(Symbol* sym)
{
  FnSymbol* autoDestroy = toFnSymbol(autoDestroyMap.get(sym->type));
  if (autoDestroy == NULL)
    // This type does not have a destructor, so we don't have a add an
    // autoDestroy call for it.
    return;


  if(DefExpr* def = toDefExpr(sym->defPoint))
  {
    SET_LINENO(def);

    CallExpr* autoDestroyCall = new CallExpr(autoDestroy, sym);
    BlockStmt* block = getScopeBlock(def);

    block->insertAtExit(autoDestroyCall);

    insertReferenceTemps(autoDestroyCall);
  }
  else
  {
    INT_FATAL("insertAutoDestroyAtScopeExit -- No def point for symbol %s", sym->name);
  }
}


// At the end of this basic block, insert an autodestroy for each symbol
// specified by the given bit-vector.
static void insertAutoDestroy(BitVec* to_cons, 
                              const SymbolVector& symbols,
                              OwnershipFlowManager::SymbolIndexMap& symbolIndex,
                              const AliasVectorMap& aliases)
{
  // For each true bit in the bit vector, add an autodestroy call.
  // But destroying one member of an alias list destroys them all.
  for (size_t j = 0; j < to_cons->size(); ++j)
  {
    if (to_cons->get(j))
    {
      Symbol* sym = symbols[j];

      // Remove this symbol and all its aliases from the cons set.
      SymbolVector* aliasList = aliases.at(sym);
      Symbol* last = aliasList->operator[](aliasList->size() - 1);
      resetAliasList(to_cons, *aliasList, symbolIndex);

      // Use the last symbol in the alias list.
      // This is a workaround because we do not track aliases in this
      // implementation.
      // See Note #1.
      insertAutoDestroyAtScopeExit(last);
    }
  }
}


// We may be able to simplify this routine, because we no longer have to
// associate a symbol to be autodestroyed with a specific block.  We just find
// the end of its declaration scope and insert the autoDestroy there.
void
OwnershipFlowManager::insertAutoDestroys()
{
  // We need to re-run BB analysis, so that inserted autoCopy() calls are added
  // to their respective basic blocks.
// This is probably not needed anymore.
//  buildBasicBlocks();

  // We need to insert an autodestroy call for each symbol that is owned
  // (live) at the end of the block but is unowned (dead) in the OUT set.

  // We form the union of all symbols that are live at the end of their
  // respective basic blocks,
  BitVec to_cons(nsyms);
  for (size_t i = 0; i < nbbs; i++)
  {
    to_cons |= *IN[i] + *PROD[i] - *CONS[i] - *OUT[i];
  }

  // and then destroy each symbol at the end of its containing scope.
  insertAutoDestroy(&to_cons, symbols, symbolIndex, aliases);
}


// TODO: Remove the computation of the EXIT set.  It is no longer needed,
// because the only information we need to determine whether to insert an
// autoDestroy for a given variable is whether it is still alive at the end of
// the function.  From this, we can infer that it was still alive at the end of
// its scope.  Then, it is simply a matter of inserting the autoDestroy at the
// right place (i.e. the end of its scope).

// TODO: In several places, we use std::map<,>::operator[]() where we would
// really like to use std::map<>::at().  The latter contains an assertion that
// the element being accessed is already a member of the map while the former
// just inserts a new element if none is present.  However, at() is only
// available in the C++11 version of the STL and PGI does not support that
// version (as of this writing).  Using find() and then [] is a bit wordy, so
// we just use [] with fingers crossed for now.  

// TODO: In several places, we would like to pass SymbolIndex& with a const
// qualifier, but since operator[] in the C++98 STL is not a const member
// function, we are prevented from doing so until we adopt the C++11 STL
// uniformly.

//########################################################################
//#
//# NOTES
//#
//# Note #1: No reference tracking
//#  We do not track references in this version, which can lead to
//# assumptions about aliases being wrong in certain cases.  We get to treat
//# bitwise copies of an object as if they are aliases because they are
//# effectively the same object -- just residing at different locations in
//# memory.  However, if one of these is modified, then they are no longer
//# equivalent.  They should be the same object, but one of them is obsolete.
//# Which one?
//#  In this implementation, temporaries are initialized only once, and typically
//# used only once as well.  Named variables are also initialized only once, but
//# may be used in multiple places -- including as lvalues.
//#  Currently, we use a heuristic to determine the "current" alias -- we simply
//# use the last one in the alias chain.  This is likely to be the named
//# variable into which the call temps are finally assigned.
//#  The heuristic can be made more robust in two ways: 
//#  1. In alias sets that contain a named variable, ensure that this named
//# variable is unique (i.e. that the remaining aliases are unnamed
//# temporaries).  Then, actually use that named variable.
//#  2. Ensure that temporaries cannot be updated.  That is, that they are in
//# fact assigned only one, and that only constant references (i.e. specifically
//# not modifiable references) to them are created.
//#
//# Note #2: The return-value variable is special
//#  To avoid ambiguity in the ownership state of a variable on entry to a join
//# node, the ownership state of that symbol at the end of all predecessor
//# nodes must match.  Logical arguments can be used to conclude that only the
//# return-value variable (RVV) can be initialized along multiple paths through
//# the flow graph, so we can treat it as a special case.
//#  We also know that the RVV must be owned on entry to the final block in the
//# function.  After normalization, functions returning a value will contain a
//# return label that marks the beginning of that final block.
//#  To force the desired behavior, we treat the RVV as if it is a global
//# variable.  That means that a bitwise copy to it is treated as a
//# consumption.  The RVV itself is not tracked as a local variable, meaning
//# that it cannot participate in any alias cliques, and we do not adjust its
//# ownership state in the return statement.
