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

// bulkCopyRecords.cpp
//
// Look for record assignment functions that can be replaced by an assign
// primitive.
//
#include "passes.h" // For global declaration of the main routine.

#include "stmt.h"
#include "astutil.h"
#include "stlUtil.h"
#include <set>


// Can be an unoredered_set.
typedef std::set<FnSymbol*> FnSet;


static bool isTrivial(FnSymbol* fn, FnSet& visited);
static bool isAssignment(FnSymbol* fn);
static bool isTrivialAssignment(FnSymbol* fn, FnSet& visited);

// The argument is assumed to be an assignment function.
// It is deemed to be simple if it contains only 'move', 'addr of', '=',
// 'return' primitives and calls only simple assignments (recursively).
// We also require that it return void (for now).
// The FLAG_TRIVIAL_ASSIGNMENT is used to ensure that we visit each such
// function only once.
static bool isTrivial(FnSymbol* fn, FnSet& visited)
{
  // Visit each function only once.
  if (visited.find(fn) != visited.end())
  {
    // Found it.
    if (fn->hasFlag(FLAG_TRIVIAL_ASSIGNMENT))
      return true;
    else
      return false;
  }
  visited.insert(fn);

  // We assume that compiler-generated assignments are field-by-field copies.
  // Note that this overloads the meaning of this flag with:
  // "Assignment operations flagged as 'compiler generated' shall contain only
  // field assignments and assignment primitives."
  // We can then assume that all nested calls are field extractions or
  // assignments, and we only have to check the latter.
  if (! fn->hasFlag(FLAG_COMPILER_GENERATED))
    return false;

  // After all compiler-supplied assignments use the new signature, this can
  // become an assert.
  if (fn->retType != dtVoid)
    return false;

  // The base argument types must match.
  ArgSymbol* lhs = fn->getFormal(1);
  ArgSymbol* rhs = fn->getFormal(2);
  if (lhs->type->getValType() != rhs->type->getValType())
    return false;

  // Assume this is a field-by-field copy.
  // If none of the fields of this record (or tuple) has a user-defined
  // assignment (recursively), then it can be replaced by a bulk copy.
  // Traverse the call expressions in the body of the function.
  // These should all be one of the allowed primitives or calls to simple
  // assignments.  If anything else, the assignment is "interesting", and
  // cannot be replaced.
  std::vector<BaseAST*> asts;
  collect_asts(fn->body, asts);
  for_vector(BaseAST, ast, asts)
  {
    if (CallExpr* call = toCallExpr(ast))
    {
      if (FnSymbol* fieldAsgn = call->isResolved())
      {
        if (isAssignment(fieldAsgn))
          if (! isTrivial(fieldAsgn, visited))
            return false;
      }
      else
      {
        // This is a primitive.
        switch(call->primitive->tag)
        {
         default:
          return false;

          // These are the allowable primitives.
         case PRIM_MOVE:
         case PRIM_ASSIGN:
         case PRIM_ADDR_OF:
         case PRIM_DEREF:
         case PRIM_GET_MEMBER:
         case PRIM_GET_MEMBER_VALUE:
         case PRIM_SET_MEMBER:
         case PRIM_RETURN: // We expect return _void.
          break;
        }
      }
    }
  }

  fn->addFlag(FLAG_TRIVIAL_ASSIGNMENT);
  return true;
}


static bool isAssignment(FnSymbol* fn)
{
  if (! fn->hasFlag(FLAG_ASSIGNOP))
    return false;
  if (strcmp(fn->name, "="))
    return false;

  return true;
}


static bool isTrivialAssignment(FnSymbol* fn, FnSet& visited)
{
  if (! isAssignment(fn))
    return false;
  if (isTrivial(fn, visited))
    return true;

  return false;
}


// Replace functions marked as simple assignments with a bulk copy
// (PRIM_ASSIGN) operation.  In the generated code, PRIM_ASSIGN on a type that
// is represented by a C struct will be rendered as a struct assignment, which
// the C compiler can implement as a memcpy.
static void replaceSimpleAssignment(FnSymbol* fn)
{
  SET_LINENO(fn);
  SymExpr* lhs = new SymExpr(fn->getFormal(1));
  SymExpr* rhs = new SymExpr(fn->getFormal(2));
  BlockStmt* block = new BlockStmt();
  block->insertAtTail(new CallExpr(PRIM_ASSIGN, lhs, rhs));
  block->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  fn->body->replace(block);
}


void bulkCopyRecords()
{
  FnSet visited;
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // We do not convert wrapper functions (only the functions that do the
    // actual assignment).
    if (fn->hasFlag(FLAG_WRAPPER))
      continue;

    if (isTrivialAssignment(fn, visited))
      replaceSimpleAssignment(fn);
  }
}
