//# OwnershipFlowManager.h -*- C++ -*-
//########################################################################
//# Ownership flow manager 
//#
//# Passing around the various pieces of state associated with ownership flow
//# analysis was getting cumbersome, so these have been consolidated in the
//# OwnershipFlowManager class.  
//# It is basically just a container that cleans up the contained objects when
//# it is destroyed.  For now, the main structure of the algorithm is left
//# external to the class, though certainly these parts can be pulled in
//# later.  
//# At that time, the test for whether a routine belongs here is whether it is
//# specific to ownership flow analysis.
//#
//########################################################################

#ifdef _CHPL_OWNERSHIPFLOWMANAGER_H_
#error Multiple inclusion
#else
#define _CHPL_OWNERSHIPFLOWMANAGER_H_

#ifndef ALIASVECTORMAP_H
#include "AliasVectorMap.h"
#endif


#include "view.h" // For list_view (debugging).

#include "bb.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "CForLoop.h"

#include "bitVec.h"


#define DEBUG_AMM 1


//########################################################################
//# supporting types and inlines
//########################################################################

//########################################################################
//# TODO: Move these flow set utilities into bb.h.
//########################################################################

typedef std::vector<BitVec*> FlowSet;

inline static void
createFlowSet(std::vector<BitVec*>& set,
              size_t                nbbs,
              size_t                nsyms)
{
  // Create a BitVec of length size for each block.
  for (size_t i = 0; i < nbbs; ++i)
    set.push_back(new BitVec(nsyms));
}


inline static void
destroyFlowSet(std::vector<BitVec*> set)
{
  for_vector(BitVec, vec, set)
    delete vec; vec = 0;
}


//########################################################################


class OwnershipFlowManager
{
  // Typedefs
 public:
  typedef BasicBlock::BasicBlockVector BasicBlockVector;
  typedef std::map<Symbol*, size_t> SymbolIndexMap;
  typedef SymbolIndexMap::value_type SymbolIndexElement;
  typedef std::vector<SymExpr*> SymExprVector;
  typedef std::vector<DefExpr*> DefExprVector;
  enum FlowSetFlags {
    FlowSet_PROD = 1<<0,
    FlowSet_CONS = 1<<1,
    FlowSet_USE = 1<<2,
    FlowSet_USED_LATER = 1<<3,
    FlowSet_EXIT = 1<<4,
    FlowSet_IN = 1<<5,
    FlowSet_OUT = 1<<6,
    FlowSet_ALL = 0xff
  };

  // Properties
 private:
  FnSymbol* _fn; // Records the function being analyzed (un-owned).
  // The vector of basic blocks is a property of the function being analyzed.
  // It is copied here for convience.  This pointer must be whenever basic
  // block analysis is re-run.
  BasicBlockVector* basicBlocks; // Cached basic block vector (un-owned).
  size_t nbbs; // Cached basic block count.  Valid after BB analysis is run.
  // A vector of symbols of interest in this function.
  // The bits in each  BitVec in the flow sets correspond to symbols in this
  // vector.
  SymbolVector symbols;
  size_t nsyms; // Cached symbol count.  Valid after the symbols is populated.
  SymbolIndexMap symbolIndex; // A map from a symbol to its index in the bit
                              // vectors.
  // The map contains one entry for each symbol.
  // Each entry is a list of symbols with which the given symbol is an alias.
  AliasVectorMap aliases;

  // PROD(i,j) is true iff symbol j gains ownership of an object in block i.
  //  PROD is valid after forward flow analysis has converged.
  //  A producer is an insn that assigns ownership of an object to a symbol.
  //  Each symbol should have only one producer on any given path.  This is a
  //  "duh" for user-defined symbols since initialization is associated with
  //  declaration -- meaning that ownership of a symbol is established before
  //  any conditional node; therefore this invariant is obeyed trivially.
  //  Internally, however, at present we break this rule for the return value
  //  variable.  It is declared at the beginning of the function but is
  //  initialized (in routines that do not declare an explicit return type) at
  //  each yield or return statement.
  //  If the invariant is violated, it is an internal error, since the user
  //  cannot write code that will cause a variable to be initialized later in
  //  the flow.  This analysis treats "noinit" initialization as
  //  initialization.  The language itself also obeys this rule, since it
  //  provides no syntax by which a user can call a constructor on an
  //  already-constructed object.
  FlowSet PROD;

  // CONS(i,j) is true iff symbol j is consumed at least once in block i.
  //  CONS is valid after forward flow anaysis has converged.
  //  A consumer is an insn that receives ownership of the given symbol.
  //  There can be multiple consumers on a given flow path.  A given insn can
  //  consume multple symbols, or even the same symbol multiple times.
  //  A given symbol need not be owned on entry to a given consumer insn.  If
  //  the "owned" bit for that symbol is false, then an autocopy is inserted so
  //  the ownership requirement of the consumer is satisfied.  Multiple
  //  autocopies may be inserted if multiple symbols are consumed or if one
  //  symbol is consumed multiply.
  //  If the "owned" bit for that symbol is true, then the symbol can be
  //  consumed without requiring an autocopy.  This is true only if consumption
  //  of that symbol is coincident with its last use.  Supposing, for example,
  //  that a symbol is consumed twice in one insn (and not consumed later in
  //  the flow), an autocopy would have to be inserted for the first use;
  //  ownership can be transferred in the second use.
  FlowSet CONS;

  // USE(i,j) is true iff block i contains a read of symbol j.
  //  Currently references are not tracked, but use of symbol j in an "addr_of"
  //  primitive is considered to be a read.  As long as "addr_of" and the use
  //  of the generated address occur within the same block, we can avoid the
  //  extra complexity of reference tracking.
  FlowSet USE;

  // USED_LATER(i,j) is true iff symbol j is used in the flow following block
  // i.  In a block with multiple successors, it is true if USE or USED_LATER
  // is true for *any* of its successors.
  FlowSet USED_LATER;

  // EXIT(i,j) is true iff block i is associated with the end-of-scope of
  // symbol j.  For this to be useful, we may need to insert empty join blocks
  // at the end of scopes containing "interesting" flows.
  FlowSet EXIT;

  // IN(i,j) is true iff symbol j is already owned on entry to block i.
  //  Valid after forward and backward flows have converged.
  FlowSet IN;

  // OUT(i,j) is true iff symbol j must be owned on exit from block i.
  //  Valid after forward and backward flows have converged.
  FlowSet OUT;

#if DEBUG_AMM
  // Debug level: 0 - off; 1 - verbose; 2 - very verbose.
  unsigned debug;
#endif


  // Old-style "do not compiler-generate these functions".
  // C++11 style is to use " = deleted" instead.
 private:
  // Cannot be default-initialized, copied or assigned.
  OwnershipFlowManager();
  OwnershipFlowManager(const OwnershipFlowManager&);
  void operator=(const OwnershipFlowManager&);

 public:
  ~OwnershipFlowManager();
  OwnershipFlowManager(FnSymbol* fn);

  void buildBasicBlocks();
  void extractSymbols();
  void populateAliases();
  void createFlowSets();
  void computeTransitions();
  void computeExits();
  void backwardFlowUse();
  void forwardFlowOwnership();
  void insertAutoCopies();
  void backwardFlowOwnership();
  void insertAutoDestroys();
  
  // Debug support.
  void debugPrintBasicBlocks();
  void debugPrintFlowSets(FlowSetFlags flags);

  // Support routines
 protected:
  void populateStmtAliases(SymExprVector& symExprs);
  void computeTransitions(BasicBlock& bb,
                          BitVec* prod, BitVec* live,
                          BitVec* use, BitVec* cons);
  void computeTransitions(SymExprVector& symExprs,
                          BitVec* prod, BitVec* live,
                          BitVec* use, BitVec* cons);
  void computeExits(std::map<BlockStmt*, size_t>& scopeToLastBBIDMap);
};


//########################################################################
//# inline implementations
//########################################################################

//############################## Predicates ##############################
//#

// Returns true if the given SymExpr is contructed by the expression in which it
// appears; false otherwise.
inline static bool isCreated(SymExpr* se)
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
          if (fn->hasFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED))
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


// Returns true if the given SymExpr is being accessed.
// Writes as well as reads return true, so the "last use" of a variable will be
// its definition if it is never read.
// We use a simple form here that ignores references.
// If it turns out that we need reference tracking, we should use the version
// from copyPropagation.cpp.
inline static bool isUsed(SymExpr* se)
{
  // We are only interested in CallExprs here (not DefExprs, e.g.)
  if (isCallExpr(se->parentExpr))
  {
    return true;
  }
  else if (isDefExpr(se->parentExpr))
  {
    return false;
  }
  else
  {
    INT_FATAL(se, "A SymExpr appears in an unexpected context.");
  }

  return false;
}


// Returns nonzero if the expression in which the given SymExpr appears makes it a
// bitwise copy of some other symbol or vice versa; zero otherwise.
// If the parent expression of the given se is a bitwise copy, the return value
// is the index of the given se in the argument list of the move (1 or 2).
inline static int
bitwiseCopyArg(SymExpr* se)
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

      // We just expect the lhs to be a SymExpr.
      INT_ASSERT(lhse);
      
      // To be a bitwise copy, the RHS expression must be a SymExpr (but other
      // ASTs are valid).
      if (! rhse)
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
inline static bool isConsumed(SymExpr* se)
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
        // We only care about calls where the se is the first operand.
        // (The function being called is also a SymExpr.)
        if (call->get(1) == se)
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
        // Unless the enclosing function is marked to the contrary.
        if (se->parentSymbol->hasFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED))
          return false;
        else
          return true;

       case PRIM_MOVE:
       case PRIM_ASSIGN:
        {
          // If the left side of a move is a global, it assumes ownership from the
          // RHS.  We can assume that the RHS is local.
          SymExpr* lhse = toSymExpr(call->get(1));
          INT_ASSERT(lhse);
          if (SymExpr* rhse = toSymExpr(call->get(2)))
            if (se == rhse)
            {
              if (//lhse->var->type->symbol->hasFlag(FLAG_REF) ||
                isModuleSymbol(lhse->var->defPoint->parentSymbol))
                return true;

              // Assignment to the return-value variable is treated as a consumption
              // (see Note #2).
              if( FnSymbol* fn = toFnSymbol(call->parentSymbol))
                if (lhse->var == fn->getReturnSymbol())
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
inline static bool isCStyleForLoopUpdateBlock(BasicBlock* bb)
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
  

// Returns true if this block one which is executed repeatedly within a loop;
// false otherwise.
// Only C-style for loops have init clauses that are essentially in the
// preceding scope.  All other loops contain only repeated clauses.
inline static bool isRepeatedInLoop(BlockStmt* block)
{
  BlockStmt* parent = toBlockStmt(block->parentExpr);
  if (parent == NULL)
    // Not block statement, so can't be a loop
    return false;

  if (parent->isLoopStmt())
  {
    // This is the case we're looking for -- the init clause in a CForLoop.
    if (CForLoop* cfl = toCForLoop(parent))
      if (block == cfl->initBlockGet())
        return false;

    // All other clauses in all other loop types are repeated.
    return true;
  }

  // But clauses in non-loop statements are not repeated (at this level).
  return false;
}


//#
//# End of predicates
//########################################################################


// We have to explicitly destroy the flow sets.
// The remaining properties are either owned elsewhere or "automatic".
inline 
OwnershipFlowManager::~OwnershipFlowManager()
{
  destroyFlowSet(PROD);
  destroyFlowSet(CONS);
  destroyFlowSet(USE);
  destroyFlowSet(USED_LATER);
  destroyFlowSet(EXIT);
  destroyFlowSet(IN);
  destroyFlowSet(OUT);
}

inline
OwnershipFlowManager::OwnershipFlowManager(FnSymbol* fn)
  : _fn(fn)
  , PROD(0), CONS(0), USE(0), USED_LATER(0), EXIT(0), IN(0), OUT(0)
#if DEBUG_AMM
  , debug(0)
#endif
{}

inline void
OwnershipFlowManager::buildBasicBlocks()
{
  BasicBlock::buildBasicBlocks(_fn);

  // This is a workaround for the presence of non-empty blocks with no
  // predecessors in the interior of the flow graph.  When dead block removal
  // works completely and runs before this pass, then this workaround may be removed.
  BasicBlock::ignoreUnreachableBlocks(_fn);

  basicBlocks = _fn->basicBlocks;
  nbbs = basicBlocks->size();
}

inline void
OwnershipFlowManager::createFlowSets()
{
  createFlowSet(PROD, nbbs, nsyms);
  createFlowSet(CONS, nbbs, nsyms);
  createFlowSet(USE, nbbs, nsyms);
  createFlowSet(USED_LATER, nbbs, nsyms);
  createFlowSet(EXIT, nbbs, nsyms);
  createFlowSet(IN, nbbs, nsyms);
  createFlowSet(OUT, nbbs, nsyms);
}

inline void
OwnershipFlowManager::debugPrintBasicBlocks()
{
  #if DEBUG_AMM
  if (debug > 1)
  {
    printf("\n");
    list_view(_fn);
  }

  if (debug > 0)
  {
    BasicBlock::printBasicBlocks(_fn);
  }
  #endif
}

inline void
OwnershipFlowManager::debugPrintFlowSets(FlowSetFlags flags)
{
  #if DEBUG_AMM
  if (debug > 0)
  {
    if (flags & FlowSet_PROD)
    {
      printf("PROD:\n"); 
      BasicBlock::printBitVectorSets(PROD);
    }
    if (flags & FlowSet_CONS) 
    {
      printf("CONS:\n"); 
      BasicBlock::printBitVectorSets(CONS);
    }
    if (flags & FlowSet_USE) 
    {
      printf("USE:\n"); 
      BasicBlock::printBitVectorSets(USE);
    }
    if (flags & FlowSet_USED_LATER) 
    {
      printf("USED_LATER:\n"); 
      BasicBlock::printBitVectorSets(USED_LATER);
    }
    if (flags & FlowSet_EXIT) 
    {
      printf("EXIT:\n"); 
      BasicBlock::printBitVectorSets(EXIT);
    }
    if (flags & FlowSet_IN) 
    {
      printf("IN:\n"); 
      BasicBlock::printBitVectorSets(IN);
    }
    if (flags & FlowSet_OUT) 
    {
      printf("OUT:\n"); 
      BasicBlock::printBitVectorSets(OUT);
    }
  }
  #endif
}

#endif
