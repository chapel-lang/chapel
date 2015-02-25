/*
 * Copyright 2004-2014 Cray Inc.
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

// TODO: This should be moved to symbol.cpp
bool fWarnOwnership = false;

// TODO: Remove this #define for normal compilation
#define DEBUG_AMM 1
static int debug = 0;

// insertAutoCopyAutoDestroy
//
//#############################################################################
//#
//# Inserts copy-constructor and destructor calls as needed, by tracking the
//# "ownership" of record objects.
//#
//# Ownership represents the "liveness" of a record object in the sense that it
//# may contain heap-allocated data.  After a record object is constructed, any
//# of its fields of class type may refer to a class object.  In that state, any
//# heap-allocated fields in the record are "owned" by the record.  Calling the
//# destructor for that record type gives the record author the opportunity to
//# clean up any contained class fields and thereby avoid leaking memory.  After
//# the destructor is called, the record variable no longer owns any
//# heap-allocated data, so that data is "unowned".
//#
//# Ownership of a record can also be transferred by a bit-wise copy.  When a
//# bit-wise copy of a record is made, class fields (which are implemented as
//# pointers) are copied verbatim.  That means that both record objects point to
//# the same class data.  Ownership is then shared between them.  Some time
//# before both copies go out of scope, one of them needs to call its
//# destructor, the other should not (to avoid double-deletion errors).
//#
//# When ownership is shared in this way, we can perform a sort of internal
//# reference-counting, maintaining a list of aliases, and ensuring that the
//# destructor is called on one of them before the last member of that alias set
//# goes out of scope. 
//#
//# For these purposes, we assume that none of the class fields of an alias is
//# updated, so they remain equivalent throughout.  If that is not true, then
//# the two variables represent independent variables.  In that case, a
//# copy-constructor call should have been inserted because each variable should
//# have its own initialization.  So its absence would indicate an error
//# somewhere upstream.
//#
//# We assume that the input AST contains the minimum number of
//# copy-constructor calls required to provide the specified semantics.  That
//# is, record values passed by in intent must be copied.  We do not yet
//# attempt to pass ownership of an object into a called routine.
//# Variables whose ownersihp must be true before exiting a routine are:
//#  - The return value variable
//#  - A module-level variable.
//#  - A record field in a record.
//# If ownership of any of these is false when they (or their container) is
//# about to go out of scope, a copy must be made to bring ownership to the
//# expected state.  Otherwise, ownership can be shared and a copy potentially
//# avoided. 
//#
//# After copy-constructor calls have been added, the routine has its full
//# complement of constructor calls.  Correct AMM is then just a matter of
//# inserting the minimum number of destructor calls to drive the ownership of
//# all local variables to false before the routine ends.  
//#
//#############################################################################

// This pass is applied to each function in turn.  The algorithm has four main
// steps:
// 1. Gather local (argument and variable) symbols and build a vector and index
//    of these.
// 2. Traverse each block to populate the GEN and KILL sets and the alias map.
// 3. Perform forward flow analysis to determine which symbols are owned at the
//    beginning of each block.  Traverse each block again and add autoCopy
//    calls so symbols passed to receptor arguments are always owned.
// 4. Perform backward flow analysis to determine which symbols must be unowned
//    by the end of each block.  Add autoDestroy calls at the end of each block
//    for symbols that are owned at the end of the block but must be unowned
//    before the block is exited.
//
// Dataflow analysis is used to propagate this constraint backwards, in case
// ownership is transferred out of a block along one path but not another.
// The sets used for flow analysis are:
//  GEN  -- The set of symbols whose ownership transitions to true by the end of
//          the block.
//  KILL -- The set of symbols whose ownership transitions to false anywhere in
//          the block.
//  IN   -- The set of symbols that are owned at the beginning of the block
//          (after forward flow analysis).
//  OUT  -- The set of symbols that are owned at the end of the block (after
//          forward flow analysis); the complement of the set of symbols that
//           must not be owned at the end of the block (after backward flow).
//
// GEN[i] contains a true bit corresponding to each symbol that is constructed
// in that block.  We expect that symbol is constructed only once, so if IN[i]
// and GEN[i] have a true bit in the same position, we can flag an internal
// error.  This condition is unlikely, so at best it should be added as a
// verify check.  After forward flow analysis, GEN[i] is initialized to IN[i]
// to show that ownership has reached this block from another 
//
// KILL[i] contains a true bit corresponding to each symbol whose ownership is
// consumed in that block.  We keep a separate list of aliases.  When any
// member of that set of aliases is consumed, all members of the set are added
// to KILL[i].
//
// We start by performing forward flow analysis to establish the value of IN.
// Within each block, OUT[i] <- IN[i] - KILL[i] + GEN[i].
// IN[i] is the intersection of OUT[pred(i)] for all predecessors of block i.
//
// For a block that contains a constructor followed by a destructor for the
// same symbol j, GEN[i,j] will be false and KILL[i,j] will be true.
// For a block that contains a destructor followed by a constructor for the
// same symbol j, GEN[i,j] and KILL[i,j] will both be true.
//
// For backward flow, we compute OUT[i] as the intersection of IN[succ(i)] for
// all successors of block i.  (No information is propagated backward through
// the blocks, so this can be done in one iteration.)  
//
// After forward flow analysis, we traverse blocks again and add autoCopy calls
// where a symexpr is unowned (at that point in the flow) but being passed to
// an argument that consumes it.  (Consumers expect the corresonding operand to
// be owned; the autoCopy call adds the required ownership.)  Note that adding
// an autoCopy to an operand does not change GEN[i] or KILL[i], so the results
// of forward flow do not have to be recomputed after this pass.
//
// For any symbol that remains owned at the end of block i where its OUT[i] is
// false, we add a destructor call so that the results of forward and backward
// flow agree.  Each added destructor call effectively sets the corresponding
// KILL[i,j] and clears the corresponding GEN[i,j].  If we update these flow
// sets during this pass, then we could check IN[i] - KILL[i] + GEN[i] ==
// OUT[i].  But since we are iterating on the difference between these two
// bitsets (i.e. OUT - (IN - KILL + GEN)), the test becomes a tautology.
//

#include "passes.h" // The main routine is declared here.

#include "bb.h"
#include "expr.h"
#include "symbol.h"
#include "bitVec.h"
#include "stlUtil.h"
#include "astutil.h"
#include "view.h" // For list_view (debugging).
#include "resolution.h" // For AutoDestroyMap.

#include <map>
#include <vector>

typedef std::vector<BitVec*> FlowSet;
typedef std::vector<Symbol*> SymbolVector;
typedef std::map<Symbol*, size_t> SymbolIndexMap;
typedef SymbolIndexMap::value_type SymbolIndexElement;
typedef std::vector<SymExpr*> SymExprVector;
typedef std::vector<DefExpr*> DefExprVector;


static void createFlowSet(std::vector<BitVec*>& set,
                          size_t                nbbs,
                          size_t                size)
{
  // Create a BitVec of length size for each block.
  for (size_t i = 0; i < nbbs; ++i)
    set.push_back(new BitVec(size));
}


static void destroyFlowSet(std::vector<BitVec*> set)
{
  for_vector(BitVec, vec, set)
    delete vec; vec = 0;
}


//############################## AliasVectorMap ##############################
//#
//# A utility class derived from std::map<Symbol*, SymbolVector*>.
//# An element in this map can contain dynamically-allocated memory through its
//# SymbolVector (second) field.  So the main point of the class is to provide
//# a destructor that will clean up an instance properly.
//# The insert and merge utilities manipulate the data structure while
//# maintaining its main invariants:
//#  1. Each member maps a symbol to a vector of symbols.
//#  2. The vector of symbols contains at least the same symbol and also other
//#     symbols with which it is an alias.
//#  3. The set of mutual aliases forms a clique.  Since this set is common to
//#     all its members, the symbol vector representing a clique is shared among all
//#     members belonging to that clique.
//#
//########################################################################

class AliasVectorMap : public std::map<Symbol*, SymbolVector*>
{
 public:
  typedef value_type AliasMapElement;

 private:
  // Copying an alias vector map involves copying the symbol vector -- which we
  // don't want to do, so we just disallow copying instead.
  AliasVectorMap(const AliasVectorMap& rhs);
  void operator=(const AliasVectorMap& rhs);

 public:
  ~AliasVectorMap();
  AliasVectorMap() : std::map<Symbol*, SymbolVector*>() {}

  void insert(Symbol* sym);
  SymbolVector* at(Symbol* sym) const;
  void merge(Symbol* orig, Symbol* alias);
};

// To destroy all the lists in the map, we find the list through the first
// member of the clique, then set the map element for each member in the list
// to NULL.  Then we delete the list.
AliasVectorMap::~AliasVectorMap()
{
  for (iterator it = begin();
       it != end(); ++it)
  {
    if (it->second == NULL)
      // This entry is part of a clique that was already deleted.
      continue;

    SymbolVector* aliasList = it->second;
    for_vector(Symbol, sym, *aliasList)
      operator[](sym) = NULL;
      
    delete aliasList;
  }
}

// Insert a symbol into the alias vector map.
// When a symbol is first inserted it is associated with an alias vector that
// contains only itself.
void
AliasVectorMap::insert(Symbol* sym)
{
  SymbolVector* aliasList = new SymbolVector();
  aliasList->push_back(sym);
  operator[](sym) = aliasList;
}

// Retrieves the SymbolVector* store in the selected element if it exists;
// returns null otherwise.
SymbolVector* AliasVectorMap::at(Symbol* sym) const
{
  const_iterator e = find(sym);
  if (e == end())
    return NULL;
  return e->second;
}

// Merge the alias lists of two symbols that have become aliases.
void
AliasVectorMap::merge(Symbol* orig, Symbol* alias)
{
  SymbolVector* origList = operator[](orig);
  SymbolVector* aliasList = operator[](alias);

  if (aliasList == origList)
  {
    // These two symbols are already aliases.
    return;
  }

  // Copy elements from the alias's list into the original symbol's list and
  // set each entry in the map corresponding to those elements to point to the
  // updated original list.
  for (size_t i = 0; i < aliasList->size(); ++i)
  {
    Symbol* sym = aliasList->at(i);
    origList->push_back(sym);
    operator[](sym) = origList;
  }

  // After that, no elements point to this aliasList, so it is deleted.
  delete aliasList;
}

//######################### End of AliasVectorMap #########################


//############################## Predicates ##############################
//#

// Returns true if the given SymExpr is contructed by the expression in which it
// appears; false otherwise.
static bool isCreated(SymExpr* se)
{
  if (CallExpr* call = toCallExpr(se->parentExpr))
  {
    // In the current incarnation, we expect construction to look like:
    //  ('move' lhs (construct <args>))
    // When constructors turn into methods, this will look a bit different.
    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN))
    {
      // We expect the target symbol to appear as the first operand.
      if (call->get(1) != se)
        return false;

      if (CallExpr* rhsCall = toCallExpr(call->get(2)))
      {
        if (FnSymbol* fn = rhsCall->isResolved())
        {
          // A "normal" function.

          // Return values of class type are ruled out.
          Type* retType = fn->retType;
          if (AggregateType* at = toAggregateType(retType))
            if (at->isClass())
              return false;
        }
        else
        {
          // A primitive.

          // We are only interested in record types, so if the RHS is not of
          // aggregate type, we can skip it.
          if (AggregateType* at = toAggregateType(rhsCall->typeInfo()))
            if (at->isClass())
              return false;

          switch (rhsCall->primitive->tag)
          {
           default:
            break; // Fall through and return true.
           case PRIM_DEREF:
            // Because the operand is a reference, its contents are "unowned" by the
            // result.  An autoCopy needs to be inserted where an owned copy is required.
            return false;
           case PRIM_GET_MEMBER_VALUE:
            // Returns a bitwise copy of the referenced field which is, as a
            // consequence, unowned.
           case PRIM_ARRAY_GET_VALUE:
            // Returns a bitwise copy of the selected array element which is, as a
            // consequence, unowned.
            return false;
          }
        }
        return true;
      }
    }
  }

  // Return false by default.
  return false;
}


// Returns nonzero if the expression in which the given SymExpr appears makes it a
// bitwise copy of some other symbol; zero otherwise.  If the parent expression
// of the given se is a bitwise copy, the return value is the
// index of the given se in the argument list of the move (1 or 2).
static int bitwiseCopyArg(SymExpr* se,
                          SymbolIndexMap& symbolIndex)
{
  // Must be a call (as opposed to a DefExpr or LabelExpr, etc.
  if (CallExpr* call = toCallExpr(se->parentExpr))
  {
    // Call must be a move or assignment
    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhse = toSymExpr(call->get(1));
      SymExpr* rhse = toSymExpr(call->get(2));

      // To be a bitwise copy, the RHS expression must be a SymExpr.
      if (! rhse)
        return 0;
      
      // And it must contain a local symbol.
      Symbol* rhs = rhse->var;
      if (symbolIndex.find(rhs) == symbolIndex.end())
            // Not found, so skip this move.
            // The lhs will be considered unowned, which is correct because it
            // is just a bitwise copy of the value of the global.
        return 0;

      if (se == lhse)
        return 1;
      if (se == rhse)
        return 2;
      INT_FATAL(se, "SymExpr does not appear in its parent move expression");
    }
  }
  return 0;
}


// Returns true if the expression in which the given SymExpr appears causes
// its ownership to be consumed; false otherwise.
static bool isConsumed(SymExpr* se)
{
  if (CallExpr* call = toCallExpr(se->parentExpr))
  {
    if (FnSymbol* fn = call->isResolved())
    {
      // This is a function call.

      // The only ones we're interested in right now are destructor calls and
      // autodestroy calls.
      if (fn->hasFlag(FLAG_DESTRUCTOR) ||
          fn->hasFlag(FLAG_AUTO_DESTROY_FN))
      {
        // Paranoid check: This SymExpr is the thing being destroyed, right?
        INT_ASSERT(call->get(1) == se);
        return true;
      }
    }
    else
    {
      // This is a primitive.
      switch(call->primitive->tag)
      {
       default:
        // Assume that, generally speaking, a primitive does not consume its argument.
        return false;

       case PRIM_RETURN:
        // Returns act like destructors.
        return true;

       case PRIM_MOVE:
       case PRIM_ASSIGN:
        {
          // If the left side of a move is a global, it assumes ownership from the
          // RHS.  We can assume that the RHS is local.
          Expr* lhs = call->get(1);
          if (SymExpr* lhse = toSymExpr(lhs))
          {
            if (//lhse->var->type->symbol->hasFlag(FLAG_REF) ||
              isModuleSymbol(lhse->var->defPoint->parentSymbol))
              return true;
          }
        }
        break;

       case PRIM_SET_MEMBER:
       case PRIM_ARRAY_SET_FIRST:
        if (call->get(3) == se)
          return true;
        break;
      }
    }
  }

  return false;
}


#if 0
static bool isCStyleForLoopUpdateBlock(BasicBlock* bb)
{
  Expr*& first = bb->exprs.front();
  Expr*& last = bb->exprs.back();
  if (Expr* parent = getCommonParentExpr(first, last))
  {
    if (CForLoop* cfl = dynamic_cast<CForLoop*>(parent->parentExpr))
    {
      if (cfl->incrBlockGet() == parent)
        return true;
    }
  }
  return false;
}
#endif
  

static bool isFlowStmt(Expr* stmt)
{
  // A goto is definitely a jump.
  if (isGotoStmt(stmt))
    return true;

  // A return primitive works like a jump. (Nothing should appear after it.)
  if (CallExpr* call = toCallExpr(stmt))
  {
    if (call->isPrimitive(PRIM_RETURN))
      return true;

    // _downEndCount is treated like a flow statement because we do not want to
    // insert autoDestroys after the task says "I'm done."  This can result in
    // false-positive memory allocation errors because the waiting (parent
    // task) can then proceed to test that the subtask has not leaked before
    // the subtask release locally-(dynamically-)allocated memory.
    if (FnSymbol* fn = call->isResolved())
      if (!strcmp(fn->name, "_downEndCount"))
        return true;
  }
  return false;
}

//#
//# End of predicates
//########################################################################


//#
//# extractSymbols
//#

// Scans the body of the given function and inserts all of the variable and
// argument symbols defined in it into the vector of symbols.  Bits in the flow
// analysis bit-vectors correspond to the entries in this vector.
// Also constructs an index map, to make it easier to find the index of a
// symbol in the vector.  (Otherwise, a linear search is required.)
// The alias map can also be populated at the same time.  
static void
extractSymbols(FnSymbol* fn,
               SymbolVector& symbols,
               AliasVectorMap& aliases,
               SymbolIndexMap& symbolIndex)
{
  DefExprVector defExprs;
  collectDefExprsSTL(fn, defExprs);

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
}


//
// populateAliases
//

static void createAlias(SymExpr* se, AliasVectorMap& aliases)
{
  CallExpr* call = toCallExpr(se->parentExpr);

  SymExpr* lhs = toSymExpr(call->get(1));
  SymExpr* rhs = toSymExpr(call->get(2));

  Symbol* lsym = lhs->var;
  Symbol* rsym = rhs->var;

  // Merge aliases whether or not they are live.
  aliases.merge(rsym, lsym);
}


static void populateAliases(SymExprVector& symExprs,
                            const SymbolVector& symbols,
                            AliasVectorMap& aliases,
                            SymbolIndexMap& symbolIndex)
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
    if (bitwiseCopyArg(se, symbolIndex) == 1)
      // It suffices to create the alias when the se is the left operand (1),
      // though it probably does no harm to do this symmetrically.
      createAlias(se, aliases);
  }
}


static void populateAliases(FnSymbol* fn,
                            const SymbolVector& symbols,
                            AliasVectorMap& aliases,
                            SymbolIndexMap& symbolIndex)
{
  for_vector(BasicBlock, bb, *fn->basicBlocks)
  {
    for_vector(Expr, expr, bb->exprs)
    {
      SymExprVector symExprs;
      collectSymExprsSTL(expr, symExprs);
      populateAliases(symExprs, symbols, aliases, symbolIndex);
    }
  }
}


//
// computeTransitions
//


// Finds the last "normal" statement in a basic block and returns it.
// A non-normal statement is a flow-altering statement, like a jump or return.
//
// If the block contains no "normal" statements, then NULL is returned.
// Then, new code can be inserted ahead of the first statement in the block.
static Expr* getLastNonflowStatement(BasicBlock* bb)
{
#if 0
  // As a slight code cleanup, autoDestroys that appear at the end of a loop
  // body (after the increment expression) in a C-style for loop are pushed
  // back into the preceding block.  
  // Without this clause, we get C-sytle for loops that look like:
  // /* 980641 */ for (i3 = start2; ((i3 <= end2)); i3 += INT64(1),chpl__autoDestroy23(&_yieldedIndex2),chpl__autoDestroy23(&_yieldedIndex),chpl__autoDestroy23(&call_tmp13)) {
  // which is arguably correct, but not very pretty.
  // This code depends on the increment expression in a C-style for loop being
  // set off in a block by itself in basic block analysis.  (Look for clauses
  // related to CForLoop blocks in BasicBlock::buildLoopStmt().)
  if (isCStyleForLoopUpdateBlock(bb))
  {
    // Back up to the predecessor of the C-style for loop update block and use
    // its last statement as the insertion point instead.

    // We assume that the update expression of a C-style for loop has only one
    // predecessor.  (This would not be true if we supported continue
    // statements in C-sytle for loops.)
    INT_ASSERT(bb->ins.size() == 1);
    bb = bb->ins.at(0);
  }
  // Actually, this doesn't quite work, because the increment clause may depend
  // on one or more of the values being deleted.  So we also have to ensure
  // that they are not read in the increment clause before moving them.
#endif

  size_t i = bb->exprs.size();
  while (i-- > 0)
  {
    Expr*& expr = bb->exprs[i];
    // TODO: It might be good to insert only statements into the bb expr list.
    Expr* stmt = expr->getStmtExpr(); // Is this necessary?
    if (! isFlowStmt(stmt))
      return stmt;
  }

  return NULL;
}


static void processCreator(SymExpr* se, 
                           BitVec* gen,
                           const AliasVectorMap& aliases,
                           SymbolIndexMap& symbolIndex)
{
  // Any function returning a value is considered to be a constructor.
  Symbol* sym = se->var;
  size_t index = symbolIndex[sym];

  // We expect that each symbol gets constructed only once, so if we are
  // about to set a bit in the gen set, it cannot already be true.
  // If this assumption turns out to be false, it means we are reusing
  // symbols.  That case can be accommodated, but it means we should
  // insert a destructor call ahead of the symbol's reinitialization.
  if (gen->get(index))
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
  for_vector(Symbol, alias, *aliasList)
  {
    size_t index = symbolIndex[alias];
    gen->set(index);
  }
}


static void processBitwiseCopy(SymExpr* se, BitVec* gen, BitVec* kill,
                               const AliasVectorMap& aliases,
                               SymbolIndexMap& symbolIndex)
{
  CallExpr* call = toCallExpr(se->parentExpr);

  SymExpr* lhs = toSymExpr(call->get(1));
  SymExpr* rhs = toSymExpr(call->get(2));

  Symbol* lsym = lhs->var;
  Symbol* rsym = rhs->var;

  SymbolVector* laliasList = aliases.at(lsym);
  SymbolVector* raliasList = aliases.at(rsym);

  // Copy ownership state from RHS.
  size_t rindex = symbolIndex[rsym];
  bool owned = gen->get(rindex);

  // If the two alias lists are identical, then the two symbols are in the
  // same alias clique.  Otherwise, ownership is being transferred out of one
  // clique into the other.  When that happens, we clear the gen bit and set
  // the kill bit for each member of the RHS clique.
  if (laliasList != raliasList)
  {
    for_vector(Symbol, ralias, *raliasList)
    {
      size_t index = symbolIndex[ralias];
      gen->reset(index);
      kill->set(index);
    }
  }

  // Now set the gen bit for each member of the lhs clique, but only if the RHS
  // was owned.
  if (owned)
  {
    for_vector(Symbol, lalias, *laliasList)
    {
      size_t index = symbolIndex[lalias];
      gen->set(index);
    }
  }
}


// If this call acts like a destructor, then add the symbols it affects to the
// kill set and remove them from the gen set.
static void processConsumer(SymExpr* se, BitVec* gen, BitVec* kill,
                            const AliasVectorMap& aliases,
                            SymbolIndexMap& symbolIndex)
{
  // Add all members of an alias clique to the kill set and remove them from
  // the gen set.

  // All members of an alias clique point to the same SymbolVector, so we only
  // need to look up one arbitrarily and then run the list.
  Symbol* sym = se->var;
  SymbolVector* aliasList = aliases.at(sym);
  for_vector(Symbol, alias, *aliasList)
  {
    size_t index = symbolIndex[alias];
    gen->reset(index);
    kill->set(index);
  }
}


static void computeTransitions(SymExprVector& symExprs,
                               BitVec* gen, BitVec* kill,
                               AliasVectorMap& aliases,
                               SymbolIndexMap& symbolIndex)
{
  for_vector(SymExpr, se, symExprs)
  {
    // We are only interested in local symbols, so if this one does not appear
    // in our map, move on.
    Symbol* sym = se->var;
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    if (isCreated(se))
      processCreator(se, gen, aliases, symbolIndex);

    if (bitwiseCopyArg(se, symbolIndex) == 1)
      processBitwiseCopy(se, gen, kill, aliases, symbolIndex);

    if (isConsumed(se))
      processConsumer(se, gen, kill, aliases, symbolIndex);
  }
}


// Look for expressions that create, destroy or transfer ownership of a record
// object.
// Right now, we can get away with just looking for MOVE or ASSIGN primitives
// to determine where ownership is created or transferred.  When constructors
// become methods, we'll key off the CONSTRUCTOR flag and modify the state of
// the first (receiver) argument.
static void computeTransitions(BasicBlock& bb, BitVec* gen, BitVec* kill,
                               AliasVectorMap& aliases,
                               SymbolIndexMap& symbolIndex)
{
  for_vector(Expr, expr, bb.exprs)
  {
    // Are all expressions in this list mutually exclusive?
    // Can we speed things up by processing only statement expressions?

    SymExprVector symExprs;
    collectSymExprsSTL(expr, symExprs);

    computeTransitions(symExprs, gen, kill, aliases, symbolIndex);
  }
}


static void computeTransitions(FnSymbol* fn,
                               FlowSet& GEN, FlowSet& KILL,
                               AliasVectorMap& aliases,
                               SymbolIndexMap& symbolIndex)
{
  size_t nbbs = fn->basicBlocks->size();
  for (size_t i = 0; i < nbbs; ++i)
  {
    computeTransitions(*(*fn->basicBlocks)[i], GEN[i], KILL[i], aliases, symbolIndex);
  }
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


static void insertAutoCopy(SymExprVector& symExprs, BitVec* gen, BitVec* kill,
                           const AliasVectorMap& aliases,
                           SymbolIndexMap& symbolIndex)
{
  for_vector(SymExpr, se, symExprs)
  {
    // We are only interested in local symbols, so if this one does not appear
    // in our map, move on.
    Symbol* sym = se->var;
    if (symbolIndex.find(sym) == symbolIndex.end())
      continue;

    // Set a bit in the gen set if this is a constructor.
    if (isCreated(se))
      processCreator(se, gen, aliases, symbolIndex);

    // Pass ownership to this symbol if it is the result of a bitwise copy.
    if (bitwiseCopyArg(se, symbolIndex) == 1)
      processBitwiseCopy(se, gen, kill, aliases, symbolIndex);

    if (isConsumed(se))
    {
      // If the gen bit is set for this symbol, we can leave it as a move and
      // transfer ownership.  Otherwise, we need to insert an autoCopy.
      size_t index = symbolIndex[sym];
      if (!gen->get(index))
        insertAutoCopy(se);

      processConsumer(se, gen, kill, aliases, symbolIndex);
    }
  }
}


static void insertAutoCopy(BasicBlock& bb,
                           BitVec* gen, BitVec* kill,
                           const SymbolVector& symbols,
                           SymbolIndexMap& symbolIndex,
                              const AliasVectorMap& aliases)
{
  for_vector(Expr, expr, bb.exprs)
  {
    SymExprVector symExprs;
    collectSymExprsSTL(expr, symExprs);

    insertAutoCopy(symExprs, gen, kill, aliases, symbolIndex);
  }
}                          


static void insertAutoCopy(FnSymbol* fn,
                              FlowSet& GEN, FlowSet& KILL,
                              FlowSet& IN, FlowSet& OUT,
                              const SymbolVector& symbols,
                              SymbolIndexMap& symbolIndex,
                              const AliasVectorMap& aliases)
{
  size_t nbbs = fn->basicBlocks->size();
  for (size_t i = 0; i < nbbs; i++)
  {
    // We need to insert an autodestroy call for each symbol that is owned
    // (live) at the end of the block but is unowned (dead) in the OUT set.
    BasicBlock& bb = *(*fn->basicBlocks)[i];
    // For this block traversal, we initialize GEN[i] = IN[i].
    // We don't care about the local kill, so we just use a new one.
    // (We could compare new with old as a sanity check following the traversal.)
    BitVec* local_gen = new BitVec(*IN[i]);
    BitVec* local_kill = new BitVec(KILL[i]->size());
    insertAutoCopy(bb, local_gen, local_kill, symbols, symbolIndex, aliases);
    delete local_kill; local_kill = 0;
    delete local_gen; local_gen = 0;
  }
}


// At the end of this basic block, insert an autodestroy for each symbol
// specified by the given bit-vector.
static void insertAutoDestroy(BasicBlock* bb, BitVec* to_kill, 
                              const SymbolVector& symbols,
                              SymbolIndexMap& symbolIndex,
                              const AliasVectorMap& aliases)
{
  // Skip degenerate basic blocks.
  if (bb->exprs.size() == 0)
    return;

  // Find the last statement in the block.
  Expr* stmt = getLastNonflowStatement(bb);

  // If we didn't find one, use the first statement in the block, and insert
  // autoDestroy calls in front of it.
  bool isJump = false;
  if (stmt == NULL)
  {
    stmt = bb->exprs[0];
    isJump = true;
  }

  // For each true bit in the bit vector, add an autodestroy call.
  // But destroying one member of an alias list destroys them all.
  SET_LINENO(stmt);
  for (size_t j = 0; j < to_kill->size(); ++j)
  {
    if (to_kill->get(j))
    {
      Symbol* sym = symbols[j];

      // Remove this symbol and all its aliases from the kill set.
      SymbolVector* aliasList = aliases.at(sym);
      for_vector(Symbol, alias, *aliasList)
      {
        size_t index = symbolIndex[alias];
        to_kill->reset(index);
      }

      FnSymbol* autoDestroy = toFnSymbol(autoDestroyMap.get(sym->type));
      if (autoDestroy == NULL)
        // This type does not have a destructor, so we don't have a add an
        // autoDestroy call for it.
        continue;

#if 0
      // For now, we ignore internally reference-counted types.  We'll add the
      // code to manage those later.
      if (isRefCountedType(sym->type))
        continue;
#endif

      CallExpr* autoDestroyCall = new CallExpr(autoDestroy, sym);

      if (isJump)
        stmt->insertBefore(autoDestroyCall);
      else
        stmt->insertAfter(autoDestroyCall);

      insertReferenceTemps(autoDestroyCall);
    }
  }
}


static void insertAutoDestroy(FnSymbol* fn,
                              FlowSet& GEN, FlowSet& KILL,
                              FlowSet& IN, FlowSet& OUT,
                              const SymbolVector& symbols,
                              SymbolIndexMap& symbolIndex,
                              const AliasVectorMap& aliases)
{
  size_t nbbs = fn->basicBlocks->size();
  for (size_t i = 0; i < nbbs; i++)
  {
    // We need to insert an autodestroy call for each symbol that is owned
    // (live) at the end of the block but is unowned (dead) in the OUT set.
    BasicBlock* bb = (*fn->basicBlocks)[i];
    BitVec to_kill = *IN[i] + *GEN[i] - *KILL[i] - *OUT[i];
    insertAutoDestroy(bb, &to_kill, symbols, symbolIndex, aliases);
  }
}


// In backward flow, we adjust the out set so it is the intersection of the IN
// sets of its successors.  If the block is a terminal block (a block with no
// successors), however, we set its OUT to all zeroes.  This forces ownership to
// be driven to zero before the function is exited.
// This analysis will work correctly even if the function has multiple exits.
static void backwardFlowOwnership(const BasicBlock::BasicBlockVector& bbs,
                                  FlowSet& IN, FlowSet& OUT)
{
  size_t nbbs = bbs.size();
  for (size_t i = 0; i < nbbs; ++i)
  {
    if (bbs[i]->outs.size() == 0)
    {
      // This is a terminal block because it has no successors.
      // Its OUT set must be empty.
      OUT[i]->clear();
    }
    else
    {
      BitVec* out = OUT[i];
      out->set();
      for_vector(BasicBlock, succ, bbs[i]->outs)
        *out &= *IN[succ->id];
    }
  }
}


// Insert autoDestroy calls in a function as needed.                                  
static void insertAutoCopyAutoDestroy(FnSymbol* fn)
{
  BasicBlock::buildBasicBlocks(fn);

  // This is a workaround for the presence of non-empty blocks with no
  // predecessors in the interior of the flow graph.  When dead block removal
  // works completely and runs before this pass, then this workaround may be removed.
  BasicBlock::ignoreUnreachableBlocks(fn);

#if DEBUG_AMM
  if (debug > 0)
  {
    printf("\n");
    list_view(fn);

    BasicBlock::printBasicBlocks(fn);
  }
#endif

  SymbolVector symbols;
  SymbolIndexMap symbolIndex;
  AliasVectorMap aliases;
  extractSymbols(fn, symbols, aliases, symbolIndex);
  populateAliases(fn, symbols, aliases, symbolIndex);

  size_t nbbs = fn->basicBlocks->size();
  size_t size = symbols.size();
  FlowSet GEN;
  FlowSet KILL;
  FlowSet IN;
  FlowSet OUT;

  createFlowSet(GEN, nbbs, size);
  createFlowSet(KILL, nbbs, size);
  createFlowSet(IN,   nbbs, size);
  createFlowSet(OUT,  nbbs, size);

  computeTransitions(fn, GEN, KILL, aliases, symbolIndex);

#if DEBUG_AMM
  if (debug > 0)
  {
    printf("GEN:\n"); BasicBlock::printBitVectorSets(GEN);
    printf("KILL:\n"); BasicBlock::printBitVectorSets(KILL);
  }
#endif

  // We can reuse the existing forward flow analysis to compute how ownership
  // flows through the basic blocks.
  // The OUT sets are initially all ones.
  for(size_t i = 0; i < nbbs; ++i) OUT[i]->set();
  BasicBlock::forwardFlowAnalysis(fn, GEN, KILL, IN, OUT, true);

#if DEBUG_AMM
  if (debug > 0)
  {
    printf("IN:\n");  BasicBlock::printBitVectorSets(IN);
    printf("OUT:\n"); BasicBlock::printBitVectorSets(OUT);
  }
#endif

  insertAutoCopy(fn, GEN, KILL, IN, OUT, symbols, symbolIndex, aliases);

  // We need our own equation for backward flow.
  // Backward flow determines where ownership must be given up through a
  // delete, by making the OUT set the AND of all its successor INs and the IN
  // be no greater than OUT | KILL (that is, every symbol owned at the
  // beginning of the block (IN) must either appear in OUT or be killed in the
  // block.
  // Also, kills are propagated backward, so that a variable owned on one path
  // into a node is owned on all such paths.
  backwardFlowOwnership(*fn->basicBlocks, IN, OUT);

#if DEBUG_AMM
  if (debug > 0)
  {
    printf("IN:\n");  BasicBlock::printBitVectorSets(IN);
    printf("OUT:\n"); BasicBlock::printBitVectorSets(OUT);
  }
#endif

  insertAutoDestroy(fn, GEN, KILL, IN, OUT, symbols, symbolIndex, aliases);

  destroyFlowSet(GEN);
  destroyFlowSet(KILL);
  destroyFlowSet(IN);
  destroyFlowSet(OUT);
}


void insertAutoCopyAutoDestroy()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // Function prototypes have no body, so we skip them.
    if (fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
      continue;

    insertAutoCopyAutoDestroy(fn);
  }
}


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
