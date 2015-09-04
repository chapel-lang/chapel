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

#include "resolution.h" // for getAutoCopy etc
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

#if 0
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
#endif

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
  BlockStmt* lscope = lsym->getDeclarationScope();
  BlockStmt* rscope = rsym->getDeclarationScope();
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
    // Ignore SymExprs that are not in the tree.
    if (se->parentSymbol == NULL)
      continue;

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
    BlockStmt* lscope = lsym->getDeclarationScope();
    BlockStmt* rscope = rsym->getDeclarationScope();
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


// If this call uses the given symbol, then add that symbol and all its aliases
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
    // Ignore SymExprs that are not in the tree.
    if (se->parentSymbol == NULL)
      continue;

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

      // When the RVV is on the LHS of an assignment, it is considered
      // to be owned unless the function says no.
      FnSymbol* fn = toFnSymbol(se->parentSymbol);
      if (sym == fn->getReturnSymbol())
        if (! fn->hasFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED))
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


#if 0
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
    if (! def->parentSymbol)
      continue;

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
#else


// Get the first statement in the given basic block.
static Expr*
getFirstStatement(BasicBlock* bb)
{
  std::vector<Expr*>& exprs = bb->exprs;

  // If this block is empty, look in its successor.
  // In this case, we assume that its successor block is unique.
  if (exprs.size() > 0)
  {
    return exprs[0];
  }
  else
  {
    std::vector<BasicBlock*>& successors = bb->outs;
    if (successors.size() > 0)
    {
      // Two or more successors.  Assume that all successors lie in the same
      // scope and just take the first.
      return getFirstStatement(successors[0]);
    }
    // No successors.  We must be at the end of the function
    return NULL;
  }
}


// Use the IN set to compute whether the first statement in each block i lies
// within the scope in which symbol j is declared.  The corresponding bit
// IN(i,j) is set to true if so, false otherwise.
void
OwnershipFlowManager::computeScopeMap()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    BitVec* in = IN[i];

    Expr* first = getFirstStatement((*basicBlocks)[i]);

    for (size_t j = 0; j < nsyms; ++j)
    {
      Symbol* sym = symbols[j];
      BlockStmt* scope = sym->getDeclarationScope();
      if (first && scope->contains(first))
        in->set(j);
      else
        in->reset(j);
    }
  }
}


// We also have to handle the case where a symbol is declared and goes out of
// scope in the same basic block.  In this case, the variable will not be in
// scope at the top of the block, and it will not be in scope at the top of the
// next block.  We get arond this by adding it to the IN set as if it was in
// scope for the entire block.
void
OwnershipFlowManager::addInternalDefs()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    BitVec* in = IN[i];

    for_vector(Expr, expr, (*basicBlocks)[i]->exprs)
    {
      if (DefExpr* def = toDefExpr(expr))
      {
        Symbol* sym = def->sym;
        if (symbolIndex.find(sym) != symbolIndex.end())
        {
          size_t j = symbolIndex[sym];
          in->set(j);
        }
      }
    }
  }
}


// Compute the EXIT set.
// A bit in the EXIT set is true if the corresponding symbol is in scope in
// block i but not in scope in all successors of i.  If successors
// differ, then that case may have to be handled specially.
// Failing to set a bit in EXIT due to ambiguity will result in memory leakage,
// but still allow the program to compile.  We can add debug code here, to help
// track down leaks later.
void
OwnershipFlowManager::computeExitBlocks()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    // A bit in EXIT[i] is set if the corresponding symbol is IN scope,
    *EXIT[i] = *IN[i];

    // and then reset if it is still in scope in all successor blocks.
    BitVec still_alive = *EXIT[i];
    std::vector<BasicBlock*> successors = (*basicBlocks)[i]->outs;
    if (successors.size() == 0)
    {
      // No successors means this is a terminal block.
      // In that case, no symbols are still alive at the end of this block.
      still_alive.clear();
    }
    else
    {
      for_vector(BasicBlock, succ, successors)
        still_alive &= *IN[succ->id];
    }

    *EXIT[i] -= still_alive;
  }
}


// Another implementation.
// A statement can have multiple exits, by jumping to a label that is outside
// the block.  The above algorithm assumes that there is only one exit block
// per construct, but there are counterexamples such as tryGetPath().

// Basic block analysis has already computed the successor blocks of each BB.
// To find out whether a given scope is being exited, we can compute the scope
// membership of the first statement in all of its successor blocks.  These
// should all agree as to whether a given scope is being exited.  However, an
// exception I can imagine is a do-while loop -- in which the branch
// transfers control back to the top of the loop while sequential execution
// falls through to the statment following the do-while construct.  In that
// case, an inserted autodestroy should follow the end of the statement.  We
// may need to add another slot to the do-while construct, so we have a place
// to put autodestroys that belong there.

// To compute EXIT(i,j), in each basic block we extract the first statement in
// the block.  For each symbol in our list, we test if that statement lies
// within its declaration scope.  If so, we set the corresponding bit in IN to
// true.  (We use IN as a temporary set for this calculation.)
// When that is done, we have a map showing basic blocks where each symbol is
// in scope.
// We then compute the difference, so EXIT records transitions from
// in-scope to not-in-scope.
void
OwnershipFlowManager::computeExits()
{
  computeScopeMap();
  addInternalDefs();
  computeExitBlocks();
}
#endif


// Compute a set that tells if a symbol j is used later in the flow.  In any
// given block, if a symbol is consumed and it is not used or consumed later in
// the same block and USED_LATER in that block is false for that symbol, then
// ownership may be transferred to that consumer.  Otherwise, an autoCopy must
// be inserted.
//
// Input: USE(i,j) is true iff symbol j is read at least once in block i.
//
// Output: USED_LATER(i,j) is true iff USE(k,j) is true for some k such that k
// follows i in the global flow.  
//
// In general USED_LATER(i,j) is not true if the
// last use of j lies in block i.  However, if USE(i,j) is true for a block i
// that lies within a loop, then USE(i,j) and USED_LATER(i,j) may be true
// simultaneously.  In that case, there is no clear last use of symbol j; it
// must be reclaimed at the end of its scope.
//
// First, we use IN and OUT as temporary sets to flow future use backwards.  If
// a use in one block sets the corresponding OUT bit in a preceding block, that
// means the preceding block is not the last use.  All OUT sets are initially
// empty.  Then we compute:
//  IN[i] = OUT[i] + USE[i]
//  OUT[i] += IN[j], for every j a successor of block i.
//  and iterate.
// Then, we set USED_LATER <- OUT;
//
// Back edges are ignored.
void 
OwnershipFlowManager::backwardFlowUse()
{
  // Clear IN sets.
  // Assume that all OUT sets are empty.
  for (size_t i = 0; i < nbbs; ++i)
    IN[i]->clear();

  bool changed;
  do {
    changed = false;

    for (size_t i = nbbs; i--; )
    {
      // Compute the OUT set for block i.
      // A true bit means that the symbol is used on all paths out of that block.
      // OUT is the union of the IN sets in all successor blocks.
      // (If the last use of a symbol is in one or more successor branches,
      // it must flow through this block.)
      BitVec& out = *OUT[i];
      out.clear();
      for_vector(BasicBlock, succ, (*basicBlocks)[i]->outs)
        if ((size_t) succ->id > i)
          out |= *IN[succ->id];

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

  // Now just copy the OUT sets into USED_LATER.
  for (size_t i = 0; i < nbbs; ++i)
    *USED_LATER[i] = *OUT[i];
}


void
OwnershipFlowManager::forwardFlowOwnership()
{
  // Start with all out sets empty.
  // TODO: Change backwardFlowUse() so it does not use the IN and OUT sets.
  // Then this initialization block can be removed.
  for (size_t i = 0; i < nbbs; ++i)
    OUT[i]->clear();

  bool changed;
  do {
    changed = false;

    for (size_t i = 0; i < nbbs; ++i)
    {
      // The in set is the intersection of the OUTs of its predecessors, but a
      // symbol does not actually escape its declaration scope, so we subtract
      // off the respective EXITs first.
      BitVec* in = IN[i];
      in->clear();
      for_vector(BasicBlock, pred, (*basicBlocks)[i]->ins)
        *in |= *OUT[pred->id] - *EXIT[pred->id];

      // This means: ownership reaches the end of a block if it enters or is
      // produced in the block and it is not consumed within the block.  Also,
      // if this is the last block in which the variable of interest is in
      // scope, an autoDestroy will be inserted, so in that case ownership does
      // not survive to the end of the block.
      // There is an implicit assumption here that if a producer and consumer
      // of the same variable appear within the same block, then the producer
      // precedes the consumer.  But then, the other ordering makes no sense.
      BitVec new_out = *IN[i] + *PROD[i] - *CONS[i];
      if (new_out != *OUT[i])
      {
        *OUT[i] = new_out;
        changed = true;
      }
    }
  } while (changed);

  // We set the OUT set of any terminal block to all zeroes, to force cleanup
  // of any variables that are unused at that point. 
  // TODO: Try disabling this and see if it is needed after reworking the USE
  // computation.
#if 0
  for (size_t i = 0; i < nbbs; ++i)
  {
    if ((*basicBlocks)[i]->outs.size() == 0)
    {
      OUT[i]->clear();
    }
  }
#endif
}


// Move forward through expressions following the given stmt, until we
// encounter one that would cause a new basic block to be spawned.
// TODO: This is selfish code.  It ought to be a service provided by the
// BasicBlock module, but is currently not.  Any significant change to
// BasicBlock::buildBasicBlocks should be reflected here.
static Expr* getLastStmtInBB(Expr* stmt)
{
  Expr* curr = stmt;
  Expr* last = stmt;
  while (curr)
  {
    Expr* next = curr->getFirstChild();
    if (! next)
      next = curr->next;
    while (! next)
    {
      // Because we are enumerating nodes in preorder, we already enumerated
      // the parent, so we skip right to its first child.
      curr = curr->parentExpr;
      if (curr == stmt->parentExpr)
      {
        // We ran out of candidates without finding a valid successor, so just
        // return the last statment visited.
        return last->getStmtExpr();
      }
      next = curr->next;
    }

    // Break on any statement that alters flow in any way.
    if (isLoopStmt(next))
      break;
    if (isCondStmt(next))
      break;
    if (isGotoStmt(next))
      break;
    if (DefExpr* def = toDefExpr(next))
      if (isLabelSymbol(def->sym))
        break;

    last = curr;
    curr = next;
  }

  // We always expect a valid expression to be returned.
  // This is also an assert that the caller did not pass in stmt == NULL.
  INT_ASSERT(curr);
  INT_ASSERT(curr == curr->getStmtExpr());
  return curr;
}


// Conversion of an iterator into an "advance" function does unnatural
// things to scopes.  If a yield appears within a loop, on the first
// execution the advance function passes through all initializations up to
// that yield.  On the second and subsequent calls (until the loop exits),
// that initialization is bypassed, and we only execute one iteration of
// the body of the loop (from wherever the yield is and back to it).
// 1. Should a temporary variable go out of scope in the body of the loop,
//    it will be deleted once per iteration without ever being initialized.
// 2. Should a temporary variable go out of scope in a scope that contains the
//    loop (possibly just the loop itself), then the first incarnation of the
//    temporary will be leaked, and upon loop exit we will free an incarnation
//    of the temp that has never been initialized.
// We can argue that 1. can never happen because part of the conversion of
// the iterator function into "advance" involves scraping the function for
// all locals that can cross a yield, and placing these into the iterator
// state class (IC).  By the time we see the "advance" function, there are
// no locals that could survive past a yield.
// We can apply similar reasoning to conclude that a local in a loop body
// whose value is established before the yield must be used before the
// yield, and likewise one established after the yield must be used after
// the yield and before the end of the loop.  
// No temporary can be established after the yield and remain valid after
// returning to the top of the loop.  If this were true, then on the first
// iteration, its value would have to magically jump into existence before
// the body of the loop is first entered.
// Finally, any temporary whose value is established before entry to the loop
// portion of the construct must also be used before that entry.  Otherwise, we
// have the preceding problem in reverse: the variable must jump into existence
// after we resume from a yield in the loop body.
//
// What this means is that temporaries that have not been captured in the
// IC are "very local".  For a first cut, I think we can insert an autoDestroy
// just after the last use of a given symbol (as opposed to waiting for it to
// go out of scope.
static void insertAutoDestroyAfterStmt(SymExpr* se)
{
  Symbol* sym = se->var;
  FnSymbol* autoDestroy = toFnSymbol(getAutoDestroy(sym->type));
  if (isPOD(sym->type) || autoDestroy == NULL)
    // This type does not have a destructor, so we don't have to add an
    // autoDestroy call for it.
    return;

  Expr* stmt = se->getStmtExpr();
  // Actually, at least code involving runtime types creates references to
  // members of temporary variables, so we are not able to see the last use
  // (since we do not track references).  As a workaround, we march forward to
  // the end of the current basic block and use that as the insertion point.
  Expr* lastStmtInBB = getLastStmtInBB(stmt);

  SET_LINENO(sym->defPoint);
  CallExpr* autoDestroyCall = new CallExpr(autoDestroy, sym);
  lastStmtInBB->insertAfter(autoDestroyCall);
  insertReferenceTemps(autoDestroyCall);
}


void
OwnershipFlowManager::iteratorInsertAutoDestroys(BitVec* to_cons, BitVec* cons,
                                                 SymExprVector& symExprs)
{
  // Run the symexprs in reverse order
  size_t s = symExprs.size();
  while (s--)
  {
    SymExpr* se = symExprs[s];
    Symbol* sym = se->var;

    // Skip symbols we don't care about
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    size_t sindex = symbolIndex[sym];
    if (to_cons->get(sindex))
    {
      // Remove this symbol and all its aliases from the cons set.
      SymbolVector* aliasList = aliases.at(sym);
      resetAliasList(to_cons, *aliasList, symbolIndex);
      setAliasList(cons, *aliasList, symbolIndex);
      insertAutoDestroyAfterStmt(se);
    }
  }
}

void
OwnershipFlowManager::iteratorInsertAutoDestroys(BitVec* to_cons, BitVec* cons, BasicBlock* bb)
{
  // Run the expressions backwards
  size_t i = bb->exprs.size();
  while (i--)
  {
    Expr* stmt = bb->exprs[i];
    
    SymExprVector symExprs;
    collectSymExprs(stmt, symExprs);

    iteratorInsertAutoDestroys(to_cons, cons, symExprs);
  }
}


void
OwnershipFlowManager::iteratorInsertAutoDestroys()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    // Find symbols that are owned and last used in this block.
    BitVec to_cons = *OUT[i] & (*USE[i] - *USED_LATER[i]);
    BitVec* cons = CONS[i];
    iteratorInsertAutoDestroys(&to_cons, cons, (*basicBlocks)[i]);
  }
}


// After forward flow analysis and after autoCopies have been inserted, it
// should be the case that every consumer has a producer associated with it on
// all possible paths reaching the consumer node.
void
OwnershipFlowManager::checkForwardOwnership()
{
  for (size_t i = 0; i < nbbs; ++i)
  {
    // We expect the out sets of all predecessor to match the computed in set.
    // This means that ownership is unambiguous on entry to a join node.
    BitVec* in = IN[i];
    for_vector(BasicBlock, pred, (*basicBlocks)[i]->ins)
    {
      if (*OUT[pred->id] - *EXIT[pred->id] != *in)
      {
        fprintf(stderr, "OwnershipFlowManager::checkForwardOwnership -- Ownership mismatch");
        fprintf(stderr, " at edge from block %d to block %ld in %s\n", pred->id, i, _fn->name);
        INT_FATAL("Check failed.");
      }
    }
  }
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
            // Treat RTT init fns like constructors:
            fn->hasFlag(FLAG_RUNTIME_TYPE_INIT_FN) ||
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
              fn->hasFlag(FLAG_INIT_COPY_FN)) // is a constructor.
            // TODO: Can the above be generalized to all functions?
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
#if 0
  // Cautiously disable the portion of this predicate that makes sure this
  // it returns true only within a destructor or autodestroy function.
  if (FnSymbol* parent = toFnSymbol(se->parentSymbol))
    if (parent->hasFlag(FLAG_DESTRUCTOR) ||
        parent->hasFlag(FLAG_AUTO_DESTROY_FN))
#endif
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
  FnSymbol* autoCopyFn = getAutoCopy(sym->type);

  if (isPOD(sym->type) || autoCopyFn == NULL)
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
    bool rvvIsOwned = true;
    if (FnSymbol* fn = toFnSymbol(se->parentSymbol))
    {
      seIsRVV = sym == fn->getReturnSymbol();
      rvvIsOwned = ! fn->hasFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED);
    }

    if (bitwiseCopyArg(se) == 1)
    {
      // If the live bit is set for the RHS symbol, we can leave it as a move and
      // transfer ownership.  Otherwise, we need to insert an autoCopy.
      if (seIsRVV && rvvIsOwned)
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

    // I apologize for this slightly special case.
    // The main AMM routine deals with the ownership of symbols, so the code
    // appearing above in the bitwiseCopyArg(se) == 1 clause only triggers when
    // the RHS is a symbol.
    // If the RHS is a call to a function, no autocopy is needed because calls
    // are uniformly treated as returning an owned value.  If the thing being
    // copied into the RVV is owned, we don't need to insert an autoCopy.
    // But that still leaves the case where a CallExpr on the RHS returns
    // something that is unowned.  That special case is handle here.
    if (seIsRVV)
    {
      CallExpr* call = toCallExpr(se->parentExpr);
      INT_ASSERT(call);
      if (call->isPrimitive(PRIM_MOVE))
      {
        if (CallExpr* rhs = toCallExpr(call->get(2)))
        {
          if (! resultIsOwned(rhs))
          {
            Expr* stmt = rhs->getStmtExpr();
            SET_LINENO(stmt);

            VarSymbol* callTmp = newTemp("call_tmp", rhs->typeInfo());
            stmt->insertBefore(new DefExpr(callTmp));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, callTmp, rhs->remove()));
            SymExpr* rhse = new SymExpr(callTmp);
            call->insertAtTail(rhse);

            insertAutoCopy(rhse);
          }
        }
      }
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
      // When there is a return-value variable, it is assumed to be assigned
      // elsewhere and owned (see the clause for seIsRVV && rvvIsOwned under
      // (bitwiseCopyAr(se) == 1) above).  Therefore, it is not necessary to
      // add an autocopy here.
      // TODO: Ultimately, there will be no RVV, so this special case can be
      // removed.
      if (seIsRVV)
        if (CallExpr* call = toCallExpr(se->parentExpr))
          if (call->isPrimitive(PRIM_RETURN))
            continue;

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


// Visit each goto in the scope containing the given symbol, and insert the
// given autoDestroy call if the symbol is live at that point in the flow.
void
OwnershipFlowManager::insertAtOtherExitPoints(Symbol* sym,
                                              CallExpr* autoDestroyCall)
{
  BlockStmt* scope = sym->getDeclarationScope();

  // Search for gotos that leave this block (scope).
  // We can find the gotos quickly by traversing the basic blocks, because
  // every goto lies at the very end of the basic block containing it.
  // Start with the current block.
  // If the last block in scope ends in a goto, then the autoDestroy we just
  // put at the end of the scope will be skipped on this branch.  This case
  // arises e.g. if there is a return statement at the end of an else clause.
  for (size_t i = nbbs; i--; )
  {
    std::vector<Expr*>& exprs = (*basicBlocks)[i]->exprs;
    if (exprs.size() == 0)
      continue;

    // If the symbol is not owned at this point in the flow, we don't have to
    // insert an autoDestroy for it.
    size_t index = symbolIndex[sym];
    if (! OUT[i]->get(index))
      continue;

    Expr* lastStmt = exprs.back();
    if (GotoStmt* gotoStmt = toGotoStmt(lastStmt))
    {
      // The scope must contain the goto statement.
      // The negative case occurs when the scope block ends in the middle of a
      // basic block.
      if (! scope->contains(gotoStmt))
        continue;

      SymExpr* label = toSymExpr(gotoStmt->label);
      INT_ASSERT(label);
      DefExpr* target = toDefExpr(label->var->defPoint);
      INT_ASSERT(target);
      if (! scope->contains(target))
      {
        // This BlockStmt does not contain the DefExpr definition the target of
        // the goto.  So we treat this as an exit point.
        gotoStmt->insertBefore(autoDestroyCall->copy());
      }
    }
  }
}


// Find the end of the scope containing the given symbol and insert an
// autoDestroy on it there.
void
OwnershipFlowManager::insertAutoDestroyAtScopeExit(Symbol* sym)
{
  // If the function is marked "return value is not owned" and this is the
  // return symbol then ownership, if true, is dropped on the floor.
  // The only ways to create this situation are:
  //  - The function is marked "return value is not owned" AND
  //  - One or more return statements return the result of a call that in turn
  //    returns an owned value.
  // Autocopies will be inserted to make the ownership state of the return
  // value variable consistent, but then the RVV will be owned while the
  // function signature says that the returned value is not.  
  // Without this clause, an autoDestroy call will be inserted after the return
  // statement.  The C compiler may complain about this being unreachable code,
  // and we don't want it anyway, so we just don't insert it.
  // Note that if all return statements (of a function marked "return value is
  // not owned") return unowned values, then we do not reach this code, because
  // the RVV will be unowned (so its corresponding to_cons bit will be false).
  if (_fn->hasFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED) &&
      sym == _fn->getReturnSymbol())
    return;

  FnSymbol* autoDestroy = toFnSymbol(getAutoDestroy(sym->type));
  if (isPOD(sym->type) || autoDestroy == NULL)
    // This type does not have a destructor, so we don't have a add an
    // autoDestroy call for it.
    return;

  DefExpr* def = toDefExpr(sym->defPoint);
  if (def == NULL)
  {
    INT_FATAL("insertAutoDestroyAtScopeExit -- No def point for symbol %s", sym->name);
    return;
  }

  SET_LINENO(def);

  CallExpr* autoDestroyCall = new CallExpr(autoDestroy, sym);
  BlockStmt* scope = sym->getDeclarationScope();
  scope->insertAtTail(autoDestroyCall);

  // This is a workaround for the fact that structured statements may have more
  // than one exit point.  For each goto in the block whose target label lies
  // outside the block, we have to insert a copy of the autoDestroy call.
  insertAtOtherExitPoints(sym, autoDestroyCall);
}


// Heuristic for determining which alias to use when autodestroying an alias clique.
static Symbol*
getBestAlias(const SymbolVector& aliasList)
{
  size_t n = aliasList.size();

  // Look for and return the last named (non-temp) variable in the list.
  while (n--)
  {
    Symbol* alias = aliasList[n];
    if (! alias->hasFlag(FLAG_TEMP))
      return alias;
  }

  // If they are all temps, just return the last one on the list.
  return aliasList.back();
}

static CallExpr* removeReturnStmt(FnSymbol* fn) {
  if (CallExpr* call = toCallExpr(fn->body->body.tail)) {
    if (call->isPrimitive(PRIM_RETURN)) {
      call->remove();
      return call;
    }
  }
  return NULL;
}


// At the end of this basic block, insert an autodestroy for each symbol
// specified by the given bit-vector.
void 
OwnershipFlowManager::insertAutoDestroy(BitVec* to_cons)
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
      Symbol* best = getBestAlias(*aliasList);
      resetAliasList(to_cons, *aliasList, symbolIndex);

      // Use the last named symbol in the alias list (or just the last one if
      // they are all unnamed).
      // This is a workaround because we do not track aliases in this
      // implementation.
      // See Note #1.
      insertAutoDestroyAtScopeExit(best);
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

  // We must insert an autoDestroy for each symbol that is live on exit from
  // its declaration scope.

  // We form the union of all symbols that are live at the end of their
  // respective basic blocks,
  BitVec to_cons(nsyms);
  for (size_t i = 0; i < nbbs; i++)
  {
    // We must insert an autoDestroy for each symbol that is live on exit from
    // its declaration scope.
    to_cons |= *OUT[i] & *EXIT[i];
  }

  // Move away the return statement so insertAtTail() do not add after it.
  CallExpr* ret = removeReturnStmt(_fn);

  // and then destroy each symbol at the end of its containing scope.
  insertAutoDestroy(&to_cons);

  // Reinsert the return statement.
  if (ret) _fn->body->insertAtTail(ret);
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
