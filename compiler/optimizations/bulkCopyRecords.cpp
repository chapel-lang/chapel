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
#include "resolution.h" // isPOD


static bool isAssignment(FnSymbol* fn);
static bool isTrivialAssignment(FnSymbol* fn);

static bool isAssignment(FnSymbol* fn)
{
  if (! fn->hasFlag(FLAG_ASSIGNOP))
    return false;
  if (strcmp(fn->name, "="))
    return false;

  return true;
}


static bool isTrivialAssignment(FnSymbol* fn)
{
  if (! isAssignment(fn))
    return false;
  
  // The base argument types must match.
  ArgSymbol* lhs = fn->getFormal(1);
  ArgSymbol* rhs = fn->getFormal(2);
  if (lhs->type->getValType() != rhs->type->getValType())
    return false;

  // Don't consider assignment for class types to be trivial.
  // That is because a class type represents a pointer, so
  // we don't really track whether or not it is POD.
  if (isClass(lhs->type->getValType()))
    return false;

  // Both argument types must be POD types
  if (isPOD(lhs->type->getValType()) && isPOD(rhs->type->getValType()))
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
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // We do not convert wrapper functions (only the functions that do the
    // actual assignment).
    if (fn->hasFlag(FLAG_WRAPPER))
      continue;

    if (isTrivialAssignment(fn))
      replaceSimpleAssignment(fn);
  }
}
