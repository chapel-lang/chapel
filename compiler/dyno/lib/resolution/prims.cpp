/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "prims.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

CallResolutionResult resolvePrimCall(Context* context,
                                     const PrimCall* call,
                                     const CallInfo& ci,
                                     const Scope* inScope,
                                     const PoiScope* inPoiScope) {
  bool allParam = true;
  for (const CallInfoActual& actual : ci.actuals()) {
    if (!actual.type().hasParamPtr()) {
      allParam = false;
      break;
    }
  }

  MostSpecificCandidates candidates;
  QualifiedType type;
  PoiInfo poi;

  // handle param folding
  auto prim = call->prim();
  if (Param::isParamOpFoldable(prim) && allParam && ci.numActuals() == 2) {
      type = Param::fold(context, prim, ci.actual(0).type(), ci.actual(1).type());
      return CallResolutionResult(candidates, type, poi);
  }

  // otherwise, handle each primitive individually
  using namespace uast::primtags;
  switch (prim) {
    /* resolution queries */
    case PRIM_IS_SUBTYPE:
    case PRIM_IS_INSTANTIATION_ALLOW_VALUES:
    case PRIM_IS_PROPER_SUBTYPE:
    case PRIM_IS_BOUND:
    case PRIM_IS_COERCIBLE:
    case PRIM_TYPE_TO_STRING:
    case PRIM_HAS_LEADER:
    case PRIM_IS_TUPLE_TYPE:
    case PRIM_NUM_FIELDS:
    case PRIM_FIELD_NUM_TO_NAME:
    case PRIM_FIELD_NAME_TO_NUM:
    case PRIM_FIELD_BY_NUM:
    case PRIM_CLASS_NAME_BY_ID:
    case PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL:
    case PRIM_IS_GENERIC_TYPE:
    case PRIM_IS_CLASS_TYPE:
    case PRIM_IS_NILABLE_CLASS_TYPE:
    case PRIM_IS_NON_NILABLE_CLASS_TYPE:
    case PRIM_IS_RECORD_TYPE:
    case PRIM_IS_UNION_TYPE:
    case PRIM_IS_EXTERN_UNION_TYPE:
    case PRIM_IS_ATOMIC_TYPE:
    case PRIM_IS_REF_ITER_TYPE:
    case PRIM_IS_EXTERN_TYPE:
    case PRIM_IS_ABS_ENUM_TYPE:
    case PRIM_IS_POD:
    case PRIM_IS_COPYABLE:
    case PRIM_IS_CONST_COPYABLE:
    case PRIM_IS_ASSIGNABLE:
    case PRIM_IS_CONST_ASSIGNABLE:
    case PRIM_HAS_DEFAULT_VALUE:
    case PRIM_NEEDS_AUTO_DESTROY:
    case PRIM_CALL_RESOLVES:
    case PRIM_METHOD_CALL_RESOLVES:
    case PRIM_CALL_AND_FN_RESOLVES:
    case PRIM_METHOD_CALL_AND_FN_RESOLVES:
    case PRIM_RESOLVES:
      assert(false && "not implemented yet");
      break;

    case PRIM_IS_STAR_TUPLE_TYPE:
      if (ci.numActuals() == 1) {
        bool result = false;
        if (auto t = ci.actual(0).type().type())
          if (auto tt = t->toTupleType())
            result = tt->isStarTuple();

        type = QualifiedType(QualifiedType::PARAM,
                             BoolType::get(context, 0),
                             BoolParam::get(context, result));
      }
      break;

    /* cast-like things */
    case PRIM_CAST:
    case PRIM_DYNAMIC_CAST:
    case PRIM_TO_LEADER:
    case PRIM_TO_FOLLOWER:
    case PRIM_TO_STANDALONE:
    case PRIM_CAST_TO_VOID_STAR:
    case PRIM_REAL_TO_INT:
    case PRIM_OBJECT_TO_INT:
    case PRIM_COERCE:
    case PRIM_TO_UNMANAGED_CLASS_CHECKED:
    case PRIM_TO_BORROWED_CLASS_CHECKED:
    case PRIM_TO_NILABLE_CLASS_CHECKED:
    case PRIM_TO_UNMANAGED_CLASS:
    case PRIM_TO_BORROWED_CLASS:
    case PRIM_TO_UNDECORATED_CLASS:
    case PRIM_TO_NILABLE_CLASS:
    case PRIM_TO_NON_NILABLE_CLASS:
      assert(false && "not implemented yet");
      break;

    /* string operations */
    case PRIM_STRING_COMPARE:
    case PRIM_STRING_CONTAINS:
    case PRIM_STRING_CONCAT:
    case PRIM_STRING_LENGTH_BYTES:
    case PRIM_STRING_LENGTH_CODEPOINTS:
    case PRIM_ASCII:
    case PRIM_STRING_ITEM:
    case PRIM_BYTES_ITEM:
    case PRIM_STRING_INDEX:
    case PRIM_STRING_COPY:
    case PRIM_STRING_SELECT:

    /* primitives that always return bool */
    case PRIM_UNARY_LNOT:
    case PRIM_EQUAL:
    case PRIM_NOTEQUAL:
    case PRIM_LESSOREQUAL:
    case PRIM_GREATEROREQUAL:
    case PRIM_LESS:
    case PRIM_GREATER:
    case PRIM_TESTCID:
    case PRIM_LOGICAL_AND:
    case PRIM_LOGICAL_OR:
    case PRIM_GET_SERIAL:
    case PRIM_PTR_EQUAL:
    case PRIM_PTR_NOTEQUAL:
    case PRIM_BLOCK_C_FOR_LOOP:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           BoolType::get(context, 0));
      break;

    /* Primitives that return the type of the 1st operand.
       Could use a more complex rule (e.g. compute numeric
       size that can contain both actuals) but using the type
       of the 1st operand is sufficient if they are only ever
       called from within Chapel wrappers. */
    case PRIM_RETURN:
    case PRIM_THROW:
    case PRIM_TRY_EXPR:
    case PRIM_TRYBANG_EXPR:
    case PRIM_YIELD:
    case PRIM_UNARY_MINUS:
    case PRIM_UNARY_PLUS:
    case PRIM_UNARY_NOT:
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
    case PRIM_DIV:
    case PRIM_MOD:
    case PRIM_LSH:
    case PRIM_RSH:
    case PRIM_AND:
    case PRIM_OR:
    case PRIM_XOR:
    case PRIM_POW:
    case PRIM_MIN:
    case PRIM_MAX:
      if (ci.numActuals() > 0) {
        type = QualifiedType(QualifiedType::CONST_VAR,
                             ci.actual(0).type().type());
      }
      break;

    /* primitives that return void */
    case PRIM_NOOP:
    case PRIM_MOVE:
    case PRIM_DEFAULT_INIT_VAR:
    case PRIM_NOINIT_INIT_VAR:
    case PRIM_INIT_FIELD:
    case PRIM_INIT_VAR:
    case PRIM_INIT_VAR_SPLIT_DECL:
    case PRIM_INIT_VAR_SPLIT_INIT:
    case PRIM_REDUCE:
    case PRIM_ASSIGN:
    case PRIM_ASSIGN_ELIDED_COPY:
    case PRIM_UNORDERED_ASSIGN:
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
    case PRIM_REDUCE_ASSIGN:
    case PRIM_SETCID:
    case PRIM_SET_UNION_ID:
    case PRIM_SET_MEMBER:
    case PRIM_CHECK_NIL:
    case PRIM_IF_VAR:
    case PRIM_LOCAL_CHECK:
    case PRIM_SET_END_COUNT:
    case PRIM_SET_DYNAMIC_END_COUNT:
    case PRIM_SET_SERIAL:
    case PRIM_ERROR:
    case PRIM_WARNING:
    case PRIM_WHEN:
    case PRIM_CALL_DESTRUCTOR:
    case PRIM_INT_ERROR:
    case PRIM_SLEEP:
    case PRIM_CHPL_EXIT_ANY:
    case PRIM_RT_ERROR:
    case PRIM_RT_WARNING:
    case PRIM_NEW_PRIV_CLASS:
    case PRIM_SET_SVEC_MEMBER:
    case PRIM_ITERATOR_RECORD_SET_SHAPE:
    case PRIM_START_RMEM_FENCE:
    case PRIM_FINISH_RMEM_FENCE:
    case PRIM_ZERO_VARIABLE:
    case PRIM_REQUIRE:
    case PRIM_CHECK_ERROR:
    case PRIM_SET_ALIASING_ARRAY_ON_TYPE:
    case PRIM_INVARIANT_START:
    case PRIM_GET_TEST_BY_NAME:
    case PRIM_GET_TEST_BY_INDEX:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           VoidType::get(context));
      break;

    /* primitives that are not yet handled in dyno */
    case PRIM_ACTUALS_LIST:
    case PRIM_REF_TO_STRING:
    case PRIM_GETCID:
    case PRIM_GET_UNION_ID:
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    case PRIM_NEW:
    case PRIM_GET_REAL:
    case PRIM_GET_IMAG:
    case PRIM_QUERY:
    case PRIM_QUERY_PARAM_FIELD:
    case PRIM_QUERY_TYPE_FIELD:
    case PRIM_ADDR_OF:
    case PRIM_DEREF:
    case PRIM_SET_REFERENCE:
    case PRIM_GET_END_COUNT:
    case PRIM_GET_DYNAMIC_END_COUNT:
    case PRIM_GPU_KERNEL_LAUNCH:
    case PRIM_GPU_KERNEL_LAUNCH_FLAT:
    case PRIM_GPU_THREADIDX_X:
    case PRIM_GPU_THREADIDX_Y:
    case PRIM_GPU_THREADIDX_Z:
    case PRIM_GPU_BLOCKIDX_X:
    case PRIM_GPU_BLOCKIDX_Y:
    case PRIM_GPU_BLOCKIDX_Z:
    case PRIM_GPU_BLOCKDIM_X:
    case PRIM_GPU_BLOCKDIM_Y:
    case PRIM_GPU_BLOCKDIM_Z:
    case PRIM_GPU_GRIDDIM_X:
    case PRIM_GPU_GRIDDIM_Y:
    case PRIM_GPU_GRIDDIM_Z:
    case PRIM_GPU_ALLOC_SHARED:
    case PRIM_GPU_SYNC_THREADS:
    case PRIM_SIZEOF_BUNDLE:
    case PRIM_SIZEOF_DDATA_ELEMENT:
    case PRIM_INIT_FIELDS:
    case PRIM_LIFETIME_OF:
    case PRIM_TYPEOF:
    case PRIM_STATIC_TYPEOF:
    case PRIM_SCALAR_PROMOTION_TYPE:
    case PRIM_STATIC_FIELD_TYPE:
    case PRIM_USED_MODULES_LIST:
    case PRIM_REFERENCED_MODULES_LIST:
    case PRIM_TUPLE_EXPAND:
    case PRIM_CHPL_COMM_GET:
    case PRIM_CHPL_COMM_PUT:
    case PRIM_CHPL_COMM_ARRAY_GET:
    case PRIM_CHPL_COMM_ARRAY_PUT:
    case PRIM_CHPL_COMM_REMOTE_PREFETCH:
    case PRIM_CHPL_COMM_GET_STRD:
    case PRIM_CHPL_COMM_PUT_STRD:
    case PRIM_ARRAY_GET:
    case PRIM_ARRAY_SHIFT_BASE_POINTER:
    case PRIM_ARRAY_SET:
    case PRIM_ARRAY_SET_FIRST:
    case PRIM_MAYBE_LOCAL_THIS:
    case PRIM_MAYBE_LOCAL_ARR_ELEM:
    case PRIM_MAYBE_AGGREGATE_ASSIGN:
    case PRIM_BLOCK_PARAM_LOOP:
    case PRIM_BLOCK_WHILEDO_LOOP:
    case PRIM_BLOCK_DOWHILE_LOOP:
    case PRIM_BLOCK_FOR_LOOP:
    case PRIM_BLOCK_BEGIN:
    case PRIM_BLOCK_COBEGIN:
    case PRIM_BLOCK_COFORALL:
    case PRIM_BLOCK_ON:
    case PRIM_BLOCK_ELIDED_ON:
    case PRIM_BLOCK_BEGIN_ON:
    case PRIM_BLOCK_COBEGIN_ON:
    case PRIM_BLOCK_COFORALL_ON:
    case PRIM_BLOCK_LOCAL:
    case PRIM_BLOCK_UNLOCAL:
    case PRIM_LOGICAL_FOLDER:
    case PRIM_WIDE_MAKE:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_WIDE_GET_NODE:
    case PRIM_WIDE_GET_ADDR:
    case PRIM_IS_WIDE_PTR:
    case PRIM_ON_LOCALE_NUM:
    case PRIM_GET_REQUESTED_SUBLOC:
    case PRIM_REGISTER_GLOBAL_VAR:
    case PRIM_BROADCAST_GLOBAL_VARS:
    case PRIM_PRIVATE_BROADCAST:
    case PRIM_CAPTURE_FN_FOR_CHPL:
    case PRIM_CAPTURE_FN_FOR_C:
    case PRIM_CREATE_FN_TYPE:
    case PRIM_GET_USER_LINE:
    case PRIM_GET_USER_FILE:
    case PRIM_RESOLUTION_POINT:
    case PRIM_FTABLE_CALL:
    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_SVEC_MEMBER_VALUE:
    case PRIM_VIRTUAL_METHOD_CALL:
    case PRIM_END_OF_STATEMENT:
    case PRIM_CURRENT_ERROR:
    case PRIM_STEAL:
    case PRIM_AUTO_DESTROY_RUNTIME_TYPE:
    case PRIM_GET_RUNTIME_TYPE_FIELD:
    case PRIM_LOOKUP_FILENAME:
    case PRIM_GET_COMPILER_VAR:
    case PRIM_GET_VISIBLE_SYMBOLS:
    case PRIM_STACK_ALLOCATE_CLASS:
    case PRIM_ZIP:
    case PRIM_NO_ALIAS_SET:
    case PRIM_COPIES_NO_ALIAS_SET:
    case PRIM_OPTIMIZATION_INFO:
    case PRIM_GATHER_TESTS:
    case PRIM_VERSION_MAJOR:
    case PRIM_VERSION_MINOR:
    case PRIM_VERSION_UPDATE:
    case PRIM_VERSION_SHA:
    case PRIM_REF_DESERIALIZE:
    case PRIM_UNKNOWN:
    case NUM_KNOWN_PRIMS:
      assert(false && "not implemented yet");

    // no default to get a warning when new primitives are added
  }

  if (type.kind() == QualifiedType::UNKNOWN) {
    context->error(call, "bad call to primitive \"%s\"", primTagToName(prim));
    type = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  }

  return CallResolutionResult(candidates, type, poi);
}


} // end namespace resolution
} // end namespace chpl
