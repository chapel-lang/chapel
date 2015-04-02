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
//# heap-allocated data.  Reads of that record -- especially to class objects
//# it contains will yield unpredictable results, so the record object is
//# effectively "dead" after it is destroyed.
//#
//# Ownership of a record can also be transferred by a bit-wise copy.  When a
//# bit-wise copy of a record is made, class fields (which are implemented as
//# pointers) are copied verbatim.  That means that both record objects point to
//# the same class data.  The two copies are effectively the same object,
//# residing at two distinct locations in memory.  Ownership is then shared
//# between them.  Some time before both copies go out of scope, one of them
//# needs to call its destructor, the other should not (to avoid
//# double-deletion errors).
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
//# have its own initialization.  Thus, its absence would indicate an error
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
//# expected state.  Otherwise, ownership may be shared and a copy potentially
//# avoided. 
//#
//# After copy-constructor calls have been added, the routine has its full
//# complement of constructor calls.  Correct AMM is then just a matter of
//# inserting the minimum number of destructor calls to drive the ownership of
//# all local variables to false before their respective scopes are exited.
//#
//#############################################################################

// In its simplest form, the algorithm for inserting autocopies and
// autodestroys is just a matter of ensuring that each producer (of ownership)
// has a matching consumer within the same function.  
//
// At each place where ownership is required (i.e. the call is a consumer), the
// ownership state of the symbol being consumed is tested.  If true, then the
// code is fine as-is; otherwise, an autoCopy is inserted so a new, owned copy
// of the symbol is passed to the consumer.
//
// If a symbol survives to the end of its scope without being consumed, then an
// autoDestroy is inserted.  One way or another, every owned symbol must be
// consumed before the routine exits.
//
// One important constraint complicates this model: It is not permissible for a
// symbol that is consumed to be used later in its scope.  That is, for an owned
// variable to be consumed, that consumer must coincide with the last use of
// that symbol.  For consumers that precede the last use, an autocopy must be
// inserted between the symbol and its use.
//
// Also, a symbol that is owned on one path into a join node must be owned on
// all paths.  I think this only happens for the return value variable.
// All user variables are initialized close to their points of declaration --
// so they are already owned on all paths.  Arguments are not owned, but they
// are also never directly assigned to.  Regardless, a bitwise copy of a symbol
// (in one clique) into another symbol (in a different clique) is considered to
// be a consumer of the RHS and a producer of the LHS.  If the RHS is unowned
// at that point, an autocopy will be inserted.  By artificially forcing the
// RVV to be in a clique by itself, we can ensure that it will be owned
// irrespective of the path taken.  [This extra complexity can be removed after
// yields and returns are treated as initializations rather than assignments.]
//
//
// This pass is applied to each function in turn.  The algorithm has four main
// steps:
// 1. Gather local (argument and variable) symbols and populate the alias map.
// 2. Perform flow analysis to create a flow graph in which each producer (of
//    ownership) has a consumer and vice versa.
// 3. Insert autocopy and autodestroy calls as needed to satisfy the
//    assumptions used in constructing the flow graph.
// See detailed comments attached to the routines containing those main steps.

//////////////////////////////////////////////////////////////////////////
//
// Dataflow analysis is used to propagate this constraint backwards, in case
// ownership is transferred out of a block along one path but not another.
// The sets used for flow analysis are:
//  live -- This set is created and updated locally to a given block.  It is
//          used to track the current liveness state within a block and does
//          not participate in global flow analysis.  It is really only needed
//          for the step that inserts autoCopy calls; but since the flag update
//          routines are shared between initial population of the PROD and CONS
//          sets and the later traversal to add autoCopy calls, we maintain the
//          live set in both.
//  PROD  -- The set of symbols whose ownership transitions to true anywhere in
//          the block.
//  CONS -- The set of symbols whose ownership transitions to false anywhere in
//          the block.
//  USE  -- The set of symbols used (read) within the given block.
//  EXIT -- The set of symbols whose end-of-scope coincides with the end of
//          this block.
//  IN   -- The set of symbols that are owned at the beginning of the block
//          (after forward flow analysis).
//  OUT  -- The set of symbols that are owned at the end of the block (after
//          forward flow analysis); the complement of the set of symbols that
//          must not be owned at the end of the block (after backward flow).
//
// After global flow analysis, live is initialized to IN[i].  AST elements that
// create ownership cause the corresponding bit(s) to transition to true and
// AST elements that consume ownership cause it to transition to false.  At the
// end of any block i, the invariant live == IN[i] + PROD[i] - CONS[i] should hold.
//
// PROD[i] contains a true bit corresponding to each symbol that is constructed
// in that block.  We expect that symbol is constructed only once, so if IN[i]
// and PROD[i] have a true bit in the same position, we can flag an internal
// error.  This condition is unlikely, so at best it should be added as a
// verify check.  
//
// CONS[i] contains a true bit corresponding to each symbol whose ownership is
// consumed in that block.  
//
// aliases -- We keep a separate list of aliases.  When any member of that set
// of aliases is consumed, all members of the set are added to CONS[i].
//
// We start by performing forward flow analysis to establish the value of IN.
// Within each block, 
//  OUT[i] <- IN[i] + PROD[i] - CONS[i]. (1)
// IN[i] is the intersection of OUT[pred(i)] for all predecessors of block i.
//
// For a block that contains a constructor followed by a destructor for the
// same symbol j, PROD[i,j] will be true and CONS[i,j] will be true.  In the
// within-block forward flow equation (1), we assume that if a prod and cons of
// the same symbol appear in the same block, the prod precedes the cons.  That
// implies a general assumption that the value of a symbol cannot be used
// before it has been defined, and that symbols are not reused (SA form).
// For a block that contains a destructor followed by a constructor for the
// same symbol j, PROD[i,j] and CONS[i,j] will both be true (and live at the
// beginning and end of the block will be false).
//
// We perform backward flow to establosh the value of OUT.  With in each block,
//  IN[i] <- OUT[i] + CONS[i] - PROD[i]. (2)
// We compute OUT[i] as the intersection of IN[succ(i)] for all successors of
// block i.
//
// After forward flow analysis, we traverse blocks again and add autoCopy calls
// where a symexpr is unowned (at that point in the flow) but being passed to
// an argument that consumes it.  (Consumers expect the corresonding operand to
// be owned; the autoCopy call adds the required ownership.)  Note that adding
// an autoCopy to an operand does not change OUT set for that block even though
// it adds a bit to the PROD set, because an autoCopy always corresponds to an
// AST element that behaves as a consume.  Therefore, we don't have to update OUT
// sets before performing the backward flow.
//
// For backward flow, the OUT set of any terminal block is set to all zeroes,
// and this is propagated back through the flow graph using equation (2)
// above.  Then we use the forward flow equation (1) to compute the live set at
// the end of the same block.  Subtracting OUT from this gives the set of
// symbols that have to be consumeed before the end of that block, i.e.:
//  to_cons = IN[i] + PROD[i] - CONS[i] - OUT[i]. (3)
// A destructor call is added for each of this symbols in the reverse of their
// declaration order.  Each added destructor call effectively sets the
// corresponding CONS[i,j] and clears the corresponding PROD[i,j].  
//

#include "passes.h" // The main routine is declared here.

#include "OwnershipFlowManager.h"

#include "bb.h"
#include "stmt.h"
#include "CForLoop.h"
#include "expr.h"
#include "symbol.h"
#include "bitVec.h"

#include "astutil.h"

//########################################################################
//#
//# Add FLAG_RETURN_VALUE_IS_NOT_OWNED to those functions that return
//# records that already existed before the function was invoked.
//# Such functions will not have autoCopy on their return value
//# and their callers will consider the returned records un-owned.
//#

static bool returnsPreExistingRecord(FnSymbol* fn)
{
  // For now, only flag a function if:
  //  - it is a field accessor
  //  - it returns a record, including array/domain/dmap
  //
  // We allow these accessors be on records, too, not just classes.

  if (!fn->hasFlag(FLAG_FIELD_ACCESSOR))
    return false;
  if (!isRecord(fn->retType))
    return false;
  return true;
}


static void addFlagReturnValueNotOwned()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
    if (returnsPreExistingRecord(fn))
      fn->addFlag(FLAG_RETURN_VALUE_IS_NOT_OWNED);
}


static DefExpr* findSymbolDef(Symbol* sym, FnSymbol* fn)
{
  std::vector<DefExpr*> defExprs;
  collectDefExprsSTL(fn, defExprs);
  for_vector(DefExpr, def, defExprs)
  {
    // Select only DefExprs referring to the RVV.
    if (def->sym == sym)
    {
      // This is the symbol's declaration.
      return def;
    }
  }
  return NULL;
}


static void hoistReturnBlock(FnSymbol* fn, BlockStmt* block)
{
  std::vector<Expr*> stmts;
  collect_stmts_STL(fn->body, stmts);
  size_t nstmts = stmts.size();

  // Hoist the return statement.
  CallExpr* ret = toCallExpr(stmts[nstmts-1]);
  INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));
  block->insertAtHead(ret->remove());

  // See if the statement is preceded by a DefExpr containing a label.
  DefExpr* label = toDefExpr(stmts[nstmts-2]);
  if (label && isLabelSymbol(label->sym))
  {
    // If so, we assume it is the return statement label.
    // Move that one, too.
    block->insertAtHead(label->remove());
  }
}


static void hoistRVVintoItsOwnScope(FnSymbol* fn)
{
  SET_LINENO(fn);

  Symbol* rvv = fn->getReturnSymbol();
  DefExpr* def = findSymbolDef(rvv, fn);
  if (!def)
    return;
  def->remove();

  // Create and populate a new body block from the end forward.
  BlockStmt* block = new BlockStmt();
  BlockStmt* body = fn->body;
  hoistReturnBlock(fn, block);
  fn->body->replace(block);
  block->insertAtHead(body);
  block->insertAtHead(def);
}


static void hoistRVVintoItsOwnScope()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // Skip functions that return void
    Symbol* retSym = fn->getReturnSymbol();
    Type* retType = retSym->type;
    if (retType == dtVoid)
      continue;

    // Don't bother if the return value is an immediate.
    if (VarSymbol* retVar = toVarSymbol(retSym))
      if (retVar->immediate)
        continue;

    hoistRVVintoItsOwnScope(fn);
  }
}


//########################################################################
//#
//# extractSymbols
//#

//
// populateAliases
//

// Insert autoDestroy calls in a function as needed.                                  
static void insertAutoCopyAutoDestroy(FnSymbol* fn)
{
  OwnershipFlowManager ofm(fn);

  ofm.buildBasicBlocks();
  ofm.debugPrintBasicBlocks();
  ofm.extractSymbols();
  ofm.populateAliases();
  ofm.createFlowSets();
  ofm.computeTransitions();
  ofm.computeExits();
  ofm.debugPrintFlowSets(OwnershipFlowManager::FlowSet_ALL);

  ofm.backwardFlowUse();
  ofm.debugPrintFlowSets(OwnershipFlowManager::FlowSet_USE);

  ofm.forwardFlowOwnership();

  ofm.debugPrintFlowSets((OwnershipFlowManager::FlowSetFlags)
                         (OwnershipFlowManager::FlowSet_IN |
                          OwnershipFlowManager::FlowSet_OUT));

  ofm.insertAutoCopies();

#if 0
  // We need our own equation for backward flow.
  // Backward flow determines where ownership must be given up through a
  // delete, by making the OUT set the AND of all its successor INs and the IN
  // be no greater than OUT | CONS (that is, every symbol owned at the
  // beginning of the block (IN) must either appear in OUT or be consed in the
  // block.
  // Also, consumptions are propagated backward, so that a variable owned on one path
  // into a node is owned on all such paths.
  ofm.backwardFlowOwnership();

  ofm.debugPrintFlowSets(FlowSet_IN | FlowSet_OUT);
#endif

  ofm.insertAutoDestroys();

}


void insertAutoCopyAutoDestroy()
{
  addFlagReturnValueNotOwned();

  hoistRVVintoItsOwnScope();

  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // Function prototypes have no body, so we skip them.
    if (fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
      continue;

    insertAutoCopyAutoDestroy(fn);
  }
}


