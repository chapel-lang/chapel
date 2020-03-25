/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

// checkAST.cpp
//
// Additional structural checks on the health of the AST.
//

#include "passes.h"

#include "expr.h"
#include "driver.h" // For prototypes.

//#include <unordered_set> // C++11 (not fully supported yet)
#include <set>
#include <map>


void checkForDuplicateUses()
{
  // The specific case we're looking for is where there is an argument that
  // appears in more than one function.
  // So, scan the list of functions, cache their arguments, and barf if a
  // duplicate is encountered.
  std::set<ArgSymbol*> args_seen;
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)
  {
    for_formals(formal, fn)
    {
      if (args_seen.count(formal))
        INT_FATAL("Argument is used in multiple function definitions.");
      args_seen.insert(formal);
    }
  }
}

void checkArgsAndLocals()
{
  // Check that each VarSymbol and each ArgSymbol
  // has a DefExpr that is in the FnSymbol
  // or a parent of it.
  for_alive_in_Vec(SymExpr, se, gSymExprs)
  {
    DefExpr* def = se->symbol()->defPoint;
    Symbol* defInSym = def->parentSymbol;
    Symbol* useInSym = se->parentSymbol;

    if (isFnSymbol(defInSym) && isFnSymbol(useInSym)) {
      if (defInSym->hasFlag(FLAG_MODULE_INIT)) {
        // OK, module init functions can define globals
      } else {
        if (defInSym != useInSym) {
          INT_FATAL(se, "Refers to a local/arg in another function");
        }
      }
    }
  }
}

// Check that no unresolved symbols remain in the tree.
// This one is pretty cheap, so can be run after every pass (following
// resolution).
void checkNoUnresolveds()
{
  // BHARSH INIT TODO: Can this be '0' now that the tuple constructor is gone?
  //
  // TODO: This value should really be cranked down to 0.
  // But in some cases _construct__tuple remains unresolved after
  // resolution ... .
  if (gUnresolvedSymExprs.n > 1)
    INT_FATAL("Structural error: "
      "At this point, the AST should not contain any unresolved symbols.");
}

// Ensures that primitives are used only where they are expected.
void checkPrimitives()
{
  for_alive_in_Vec(CallExpr, call, gCallExprs)
  {
    // Only interested in primitives
    if (!call->primitive)
      continue;

    switch(call->primitive->tag)
    {
     default:
      INT_FATAL("Unhandled case in checkPrimitives");
      break;

      // These do not survive past parsing.
     case PRIM_ACTUALS_LIST:
      if (parsed)
        INT_FATAL("Primitive should not appear after parsing is complete.");
      break;

      // These do not survive past resolution.
     case PRIM_DEFAULT_INIT_VAR:
     case PRIM_INIT_FIELD:
     case PRIM_INIT_VAR:

     case PRIM_TYPE_TO_STRING:
     case PRIM_HAS_LEADER:
     case PRIM_TO_LEADER:
     case PRIM_TO_FOLLOWER:
     case PRIM_TO_STANDALONE:
     case PRIM_FIELD_NUM_TO_NAME:
     case PRIM_FIELD_NAME_TO_NUM:
     case PRIM_FIELD_BY_NUM:
     case PRIM_IS_RECORD_TYPE:
     case PRIM_IS_UNION_TYPE:
     case PRIM_IS_ATOMIC_TYPE:
     case PRIM_IS_EXTERN_TYPE:
     case PRIM_IS_TUPLE_TYPE:
     case PRIM_IS_STAR_TUPLE_TYPE:
     case PRIM_IS_SUBTYPE:
     case PRIM_IS_INSTANTIATION_ALLOW_VALUES:
     case PRIM_IS_PROPER_SUBTYPE:
     case PRIM_NEW:                 // new keyword
     case PRIM_ERROR:
     case PRIM_WARNING:
      if (resolved)
        INT_FATAL("Primitive should not appear after resolution is complete.");
      break;

     case PRIM_ADDR_OF:             // set a reference to a value
      if (resolved) {
        // Check that the argument is not already a reference.
        // references can only go 1 level
        if (isReferenceType(call->get(1)->typeInfo()))
          INT_FATAL("Invalid PRIM_ADDR_OF of a reference");
      }
      break;

     case PRIM_DEREF:               // dereference a reference
      if (resolved) {
        // Check that the argument is a reference.
        if (!isReferenceType(call->get(1)->typeInfo()))
          INT_FATAL("Invalid PRIM_DEREF of a non-reference");
      }
      break;

     case PRIM_MOVE:
      if (resolved) {
        // Check that the LHS has the same type as the RHS.
        if (call->get(1)->typeInfo() != call->get(2)->typeInfo())
          INT_FATAL("PRIM_MOVE types do not match");
      }
      break;

     case PRIM_GET_MEMBER:
     case PRIM_GET_MEMBER_VALUE:
     case PRIM_SET_MEMBER:
      if (resolved) {
        // For expr.field, check that field is a VarSymbol
        // in the class expr.type.
        AggregateType* ct = toAggregateType(call->get(1)->typeInfo());
        SymExpr* getFieldSe = toSymExpr(call->get(2));
        Symbol* getField = getFieldSe->symbol();
        INT_ASSERT(ct);
        INT_ASSERT(getField);
        Symbol* name_match = NULL;
        for_fields(field, ct) {
          if (0 == strcmp(field->name, getField->name))
            name_match = field;
        }
        if (name_match != getField) {
          // Note: name_match contains the field that was
          // probably meant...
          INT_FATAL("Field access for field not in type");
        }
      }
      break;

     case PRIM_UNKNOWN:
     case PRIM_NOOP:
     case PRIM_REF_TO_STRING:
     case PRIM_RETURN:
     case PRIM_YIELD:
     case PRIM_UNARY_MINUS:
     case PRIM_UNARY_PLUS:
     case PRIM_UNARY_NOT:
     case PRIM_UNARY_LNOT:
     case PRIM_ADD:
     case PRIM_SUBTRACT:
     case PRIM_MULT:
     case PRIM_DIV:
     case PRIM_MOD:
     case PRIM_LSH:
     case PRIM_RSH:
     case PRIM_EQUAL:
     case PRIM_NOTEQUAL:
     case PRIM_LESSOREQUAL:
     case PRIM_GREATEROREQUAL:
     case PRIM_LESS:
     case PRIM_GREATER:
     case PRIM_AND:
     case PRIM_OR:
     case PRIM_XOR:
     case PRIM_POW:
     case PRIM_ASSIGN:
     case PRIM_SET_REFERENCE:
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
     case PRIM_MIN:
     case PRIM_MAX:
     case PRIM_SETCID:
     case PRIM_TESTCID:
     case PRIM_GETCID:
     case PRIM_SET_UNION_ID:
     case PRIM_GET_UNION_ID:
     case PRIM_CHECK_NIL:
     case PRIM_GET_REAL:            // get complex real component
     case PRIM_GET_IMAG:            // get complex imag component
     case PRIM_QUERY:               // query expression primitive
     case PRIM_LOCAL_CHECK:         // assert that a wide ref is on this locale
     case PRIM_GET_END_COUNT:
     case PRIM_SET_END_COUNT:
     case PRIM_GET_DYNAMIC_END_COUNT:
     case PRIM_SET_DYNAMIC_END_COUNT:
     case PRIM_GET_SERIAL:              // get serial state
     case PRIM_SET_SERIAL:              // set serial state to true or false
     case PRIM_SIZEOF_BUNDLE:
     case PRIM_SIZEOF_DDATA_ELEMENT:
     case PRIM_INIT_FIELDS:             // initialize fields of a temporary record
     case PRIM_PTR_EQUAL:
     case PRIM_PTR_NOTEQUAL:
     case PRIM_CAST:
     case PRIM_DYNAMIC_CAST:
     case PRIM_TYPEOF:
     case PRIM_USED_MODULES_LIST:       // used modules in BlockStmt::modUses
     case PRIM_TUPLE_EXPAND:
     case PRIM_CHPL_COMM_GET:           // Direct calls to the Chapel comm layer
     case PRIM_CHPL_COMM_PUT:
     case PRIM_CHPL_COMM_ARRAY_GET:
     case PRIM_CHPL_COMM_ARRAY_PUT:
     case PRIM_CHPL_COMM_REMOTE_PREFETCH:
     case PRIM_CHPL_COMM_GET_STRD:      // Direct calls to the Chapel comm layer for strided comm
     case PRIM_CHPL_COMM_PUT_STRD:      //  may eventually add others (e.g.: non-blocking)
     case PRIM_ARRAY_GET:
     case PRIM_ARRAY_GET_VALUE:
     case PRIM_ARRAY_SHIFT_BASE_POINTER:
     case PRIM_ARRAY_SET:
     case PRIM_ARRAY_SET_FIRST:
     case PRIM_WHEN:
     case PRIM_BLOCK_PARAM_LOOP:        // BlockStmt::blockInfo - param for loop
     case PRIM_BLOCK_WHILEDO_LOOP:      // BlockStmt::blockInfo - while do loop
     case PRIM_BLOCK_DOWHILE_LOOP:      // BlockStmt::blockInfo - do while loop
     case PRIM_BLOCK_FOR_LOOP:          // BlockStmt::blockInfo - for loop
     case PRIM_BLOCK_BEGIN:             // BlockStmt::blockInfo - begin block
     case PRIM_BLOCK_COBEGIN:           // BlockStmt::blockInfo - cobegin block
     case PRIM_BLOCK_COFORALL:          // BlockStmt::blockInfo - coforall block
     case PRIM_BLOCK_ON:                // BlockStmt::blockInfo - on block
     case PRIM_BLOCK_ELIDED_ON:
     case PRIM_BLOCK_BEGIN_ON:
     case PRIM_BLOCK_COBEGIN_ON:
     case PRIM_BLOCK_COFORALL_ON:
     case PRIM_BLOCK_LOCAL:             // BlockStmt::blockInfo - local block
     case PRIM_BLOCK_UNLOCAL:           // BlockStmt::blockInfo - unlocal local block
     case PRIM_CALL_DESTRUCTOR:         // call destructor on type (do not free)
     case PRIM_LOGICAL_FOLDER:          // Help fold logical && and ||
     case PRIM_WIDE_GET_LOCALE:         // Returns the "locale" portion of a wide pointer.
     case PRIM_WIDE_GET_NODE:           // Get just the node portion of a wide pointer.
     case PRIM_WIDE_GET_ADDR:           // Get just the address portion of a wide pointer.
     case PRIM_ON_LOCALE_NUM:           // specify a particular localeID for an on clause.
     case PRIM_REGISTER_GLOBAL_VAR:
     case PRIM_BROADCAST_GLOBAL_VARS:
     case PRIM_PRIVATE_BROADCAST:
     case PRIM_INT_ERROR:
     case PRIM_CAPTURE_FN_FOR_CHPL:
     case PRIM_CAPTURE_FN_FOR_C:
     case PRIM_CREATE_FN_TYPE:
     case PRIM_STRING_COPY:
     case PRIM_CAST_TO_VOID_STAR:       // Cast the object argument to void*.
     case PRIM_RT_ERROR:
     case PRIM_RT_WARNING:
     case PRIM_NEW_PRIV_CLASS:
     case PRIM_GET_USER_LINE:
     case PRIM_GET_USER_FILE:
     case PRIM_FTABLE_CALL:
     case PRIM_SET_SVEC_MEMBER:
     case PRIM_GET_SVEC_MEMBER:
     case PRIM_GET_SVEC_MEMBER_VALUE:
     case PRIM_VIRTUAL_METHOD_CALL:
     case PRIM_NUM_FIELDS:
     case PRIM_IS_POD:
     case PRIM_GATHER_TESTS:
      break;
    }
  }
}

// Look for cases where the return type is a value type,
// and it has no corresponding ref type.
void checkReturnTypesHaveRefTypes()
{
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)
  {
    Type* retType = fn->retType;

    if (retType->symbol->hasFlag(FLAG_REF))
      continue;

    if (fn->retTag != RET_TYPE && retType->refType == NULL)
      INT_FATAL(fn, "every return type must also have a ref type.");
  }
}
