/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  if (fn->name != astrSassign)
    return false;

  return true;
}

static std::map<Type*, bool> containsRef;

static bool typeContainsRef(Type* t, bool isRoot = true)
{
  if (containsRef.count(t))
    return containsRef[t];

  bool hasRef = false;

  if (!isRoot && isReferenceType(t)) {
    hasRef = true;
  } else if (AggregateType* at = toAggregateType(t)) {
    if (!at->isClass()) {
      for_fields(field, at) {
        hasRef |= typeContainsRef(field->type, false);
      }
    }
  }

  containsRef[t] = hasRef;

  return hasRef;
}

/* Find assignment functions that this optimization
   should replace with bit copies.

   This function returns true if all of these conditions are met:
    - fn is an assignment function
    - the lhs and rhs arguments have the same type
    - the lhs and rhs arguments are POD types
    - the lhs and rhs arguments do not contain references
 */
static bool isTrivialAssignment(FnSymbol* fn)
{
  if (! isAssignment(fn))
    return false;
  
  // The base argument types must match.
  ArgSymbol* lhs = fn->getFormal(1);
  ArgSymbol* rhs = fn->getFormal(2);
  Type* argType = lhs->type->getValType();
  if (argType != rhs->type->getValType())
    return false;

  // Skip this optimization for string/wide string types
  // (due to problems providing additional arguments for
  //  PRIM_ASSIGN).
  if (argType == dtString)
    return false;

  // Neither type may contain references
  // (since these would be dereferenced in the assignment)
  if (typeContainsRef(lhs->type) || typeContainsRef(rhs->type))
    return false;

  // Both argument types must be POD types
  // (but at this point they are the same type, so we only check one)
  if (isPOD(argType))
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

  containsRef.clear();
}
