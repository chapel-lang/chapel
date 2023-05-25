/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

// This file lists the primitive ops to help with macros generating
// code for each

// The format is
//   PRIMITIVE_R(ENUM_NAME, "string name")
//   PRIMITIVE_G(ENUM_NAME, "string name")

// use this file by defining PRIMITIVE_R and PRIMITIVE_G and then including it

// Primitives are declared in groups
//  PRIMITIVE_R -- does not exist during code generation
//  PRIMITIVE_G -- needs to be code generated
//                 and there will be a CallExpr::codegenPRIM_BLA method for it

// behavior for individual primitives are not documented in this file
// but rather in AST/primitive.cpp

// and the primitive value itself has a PRIM_ prefix e.g.
// PRIMITIVE_G(NOOP, "noop") is talking about an enum value PRIM_NOOP.

PRIMITIVE_G(UNKNOWN, "")

PRIMITIVE_R(ACTUALS_LIST, "actuals list")
PRIMITIVE_G(NOOP, "noop")
PRIMITIVE_G(MOVE, "move")

PRIMITIVE_R(DEFAULT_INIT_VAR, "default init var")
PRIMITIVE_R(NOINIT_INIT_VAR, "noinit init var")
PRIMITIVE_R(INIT_FIELD, "init field")
PRIMITIVE_R(INIT_VAR, "init var")
PRIMITIVE_R(INIT_VAR_SPLIT_DECL, "init var split decl")
PRIMITIVE_R(INIT_VAR_SPLIT_INIT, "init var split init")
PRIMITIVE_R(INIT_DONE, "init done")

PRIMITIVE_G(REF_TO_STRING, "ref to string")
PRIMITIVE_G(RETURN, "return")

PRIMITIVE_R(THROW, "throw")
PRIMITIVE_R(TRY_EXPR, "try-expr")
PRIMITIVE_R(TRYBANG_EXPR, "try!-expr")
PRIMITIVE_R(YIELD, "yield")
PRIMITIVE_R(REDUCE, "reduce")

PRIMITIVE_G(UNARY_MINUS, "u-")
PRIMITIVE_G(UNARY_PLUS, "u+")
PRIMITIVE_G(UNARY_NOT, "u~")
PRIMITIVE_G(UNARY_LNOT, "u!")
PRIMITIVE_G(ADD, "+")
PRIMITIVE_G(SUBTRACT, "-")
PRIMITIVE_G(MULT, "*")
PRIMITIVE_G(DIV, "/")
PRIMITIVE_G(MOD, "%")
PRIMITIVE_G(LSH, "<<")
PRIMITIVE_G(RSH, ">>")
PRIMITIVE_G(EQUAL, "==")
PRIMITIVE_G(NOTEQUAL, "!=")
PRIMITIVE_G(LESSOREQUAL, "<=")
PRIMITIVE_G(GREATEROREQUAL, ">=")
PRIMITIVE_G(LESS, "<")
PRIMITIVE_G(GREATER, ">")
PRIMITIVE_G(AND, "&")
PRIMITIVE_G(OR, "|")
PRIMITIVE_G(XOR, "^")
PRIMITIVE_G(POW, "**")

PRIMITIVE_R(LOGICAL_AND, "&&")
PRIMITIVE_R(LOGICAL_OR, "||")

PRIMITIVE_G(ASSIGN, "=")
PRIMITIVE_R(ASSIGN_ELIDED_COPY, "elided-copy=")
PRIMITIVE_G(UNORDERED_ASSIGN, "unordered=")
PRIMITIVE_G(ADD_ASSIGN, "+=")
PRIMITIVE_G(SUBTRACT_ASSIGN, "-=")
PRIMITIVE_G(MULT_ASSIGN, "*=")
PRIMITIVE_G(DIV_ASSIGN, "/=")
PRIMITIVE_G(MOD_ASSIGN, "%=")
PRIMITIVE_G(LSH_ASSIGN, "<<=")
PRIMITIVE_G(RSH_ASSIGN, ">>=")
PRIMITIVE_G(AND_ASSIGN, "&=")
PRIMITIVE_G(OR_ASSIGN, "|=")
PRIMITIVE_G(XOR_ASSIGN, "^=")
PRIMITIVE_R(REDUCE_ASSIGN, "reduce=")

PRIMITIVE_G(MIN, "_min")
PRIMITIVE_G(MAX, "_max")

PRIMITIVE_G(SETCID, "setcid")
PRIMITIVE_G(TESTCID, "testcid")
PRIMITIVE_G(GETCID, "getcid")
PRIMITIVE_G(SET_UNION_ID, "set_union_id")
PRIMITIVE_G(GET_UNION_ID, "get_union_id")
PRIMITIVE_G(GET_MEMBER, ".")
PRIMITIVE_G(GET_MEMBER_VALUE, ".v")
PRIMITIVE_G(SET_MEMBER, ".=")
PRIMITIVE_G(CHECK_NIL, "_check_nil")
PRIMITIVE_R(IF_VAR, "if var")

PRIMITIVE_R(NEW, "new")

PRIMITIVE_G(GET_REAL, "complex_get_real")
PRIMITIVE_G(GET_IMAG, "complex_get_imag")

PRIMITIVE_R(QUERY, "query")
PRIMITIVE_R(QUERY_PARAM_FIELD, "query param field")
PRIMITIVE_R(QUERY_TYPE_FIELD, "query type field")

PRIMITIVE_R(STATIC_DOMAIN_TYPE, "static domain type")

PRIMITIVE_G(ADDR_OF, "addr of")
PRIMITIVE_G(DEREF, "deref")
PRIMITIVE_G(SET_REFERENCE, "set reference")

PRIMITIVE_G(LOCAL_CHECK, "local_check")

PRIMITIVE_R(GET_END_COUNT, "get end count")
PRIMITIVE_R(SET_END_COUNT, "set end count")

PRIMITIVE_G(GET_DYNAMIC_END_COUNT, "get dynamic end count")
PRIMITIVE_G(SET_DYNAMIC_END_COUNT, "set dynamic end count")

PRIMITIVE_G(GPU_KERNEL_LAUNCH, "gpu kernel launch")
PRIMITIVE_G(GPU_KERNEL_LAUNCH_FLAT, "gpu kernel launch flat")
PRIMITIVE_G(GPU_THREADIDX_X, "gpu threadIdx x")
PRIMITIVE_G(GPU_THREADIDX_Y, "gpu threadIdx y")
PRIMITIVE_G(GPU_THREADIDX_Z, "gpu threadIdx z")
PRIMITIVE_G(GPU_BLOCKIDX_X, "gpu blockIdx x")
PRIMITIVE_G(GPU_BLOCKIDX_Y, "gpu blockIdx y")
PRIMITIVE_G(GPU_BLOCKIDX_Z, "gpu blockIdx z")
PRIMITIVE_G(GPU_BLOCKDIM_X, "gpu blockDim x")
PRIMITIVE_G(GPU_BLOCKDIM_Y, "gpu blockDim y")
PRIMITIVE_G(GPU_BLOCKDIM_Z, "gpu blockDim z")
PRIMITIVE_G(GPU_GRIDDIM_X, "gpu gridDim x")
PRIMITIVE_G(GPU_GRIDDIM_Y, "gpu gridDim y")
PRIMITIVE_G(GPU_GRIDDIM_Z, "gpu gridDim z")
PRIMITIVE_G(GPU_ALLOC_SHARED, "gpu allocShared")
PRIMITIVE_G(GPU_SYNC_THREADS, "gpu syncThreads")
PRIMITIVE_R(GPU_SET_BLOCKSIZE, "gpu set blockSize")
PRIMITIVE_G(ASSERT_ON_GPU, "chpl_assert_on_gpu")

PRIMITIVE_G(GET_SERIAL, "task_get_serial")
PRIMITIVE_G(SET_SERIAL, "task_set_serial")

PRIMITIVE_G(SIZEOF_BUNDLE, "sizeof_bundle")
PRIMITIVE_G(SIZEOF_DDATA_ELEMENT, "sizeof_ddata_element")

PRIMITIVE_R(INIT_FIELDS, "chpl_init_record")

PRIMITIVE_G(PTR_EQUAL, "ptr_eq")
PRIMITIVE_G(PTR_NOTEQUAL, "ptr_neq")

PRIMITIVE_R(IS_SUBTYPE, "is_subtype")
PRIMITIVE_R(IS_INSTANTIATION_ALLOW_VALUES, "is_instantiation_allow_values")
PRIMITIVE_R(IS_PROPER_SUBTYPE, "is_proper_subtype")
PRIMITIVE_R(IS_BOUND, "is bound")
PRIMITIVE_R(IS_COERCIBLE, "is_coercible")

PRIMITIVE_G(CAST, "cast")
PRIMITIVE_G(DYNAMIC_CAST, "dynamic_cast")
PRIMITIVE_R(LIFETIME_OF, "lifetime_of")
PRIMITIVE_R(TYPEOF, "typeof")
PRIMITIVE_R(STATIC_TYPEOF, "static typeof")
PRIMITIVE_R(SCALAR_PROMOTION_TYPE, "scalar promotion type")
PRIMITIVE_R(STATIC_FIELD_TYPE, "static field type")
PRIMITIVE_R(USED_MODULES_LIST, "used modules list")
PRIMITIVE_R(REFERENCED_MODULES_LIST, "referenced modules list")
PRIMITIVE_R(TUPLE_EXPAND, "expand_tuple")

PRIMITIVE_G(CHPL_COMM_GET, "chpl_comm_get")
PRIMITIVE_G(CHPL_COMM_PUT, "chpl_comm_put")

PRIMITIVE_G(CHPL_COMM_ARRAY_GET, "chpl_comm_array_get")
PRIMITIVE_G(CHPL_COMM_ARRAY_PUT, "chpl_comm_array_put")
PRIMITIVE_G(CHPL_COMM_REMOTE_PREFETCH, "chpl_comm_remote_prefetch")
PRIMITIVE_G(CHPL_COMM_GET_STRD, "chpl_comm_get_strd")
PRIMITIVE_G(CHPL_COMM_PUT_STRD, "chpl_comm_put_strd")

PRIMITIVE_G(ARRAY_GET, "array_get")
PRIMITIVE_G(ARRAY_SHIFT_BASE_POINTER, "shift_base_pointer")

PRIMITIVE_G(ARRAY_SET, "array_set")
PRIMITIVE_G(ARRAY_SET_FIRST, "array_set_first")

PRIMITIVE_R(MAYBE_LOCAL_THIS, "may be local access")
PRIMITIVE_R(MAYBE_LOCAL_ARR_ELEM, "may be local array element")
PRIMITIVE_R(MAYBE_AGGREGATE_ASSIGN, "may be aggregated assignment")

PRIMITIVE_R(ERROR, "error")
PRIMITIVE_R(WARNING, "warning")
PRIMITIVE_R(WHEN, "when case expressions")
PRIMITIVE_R(TYPE_TO_STRING, "typeToString")

// Maybe these should be in a different category
PRIMITIVE_R(BLOCK_PARAM_LOOP, "param loop")
PRIMITIVE_R(BLOCK_WHILEDO_LOOP, "while...do loop")
PRIMITIVE_R(BLOCK_DOWHILE_LOOP, "do...while loop")
PRIMITIVE_R(BLOCK_FOR_LOOP, "for loop")
PRIMITIVE_R(BLOCK_C_FOR_LOOP, "C for loop")

PRIMITIVE_R(BLOCK_BEGIN, "begin block")
PRIMITIVE_R(BLOCK_COBEGIN, "cobegin block")
PRIMITIVE_R(BLOCK_COFORALL, "coforall loop")
PRIMITIVE_R(BLOCK_ON, "on block")
PRIMITIVE_R(BLOCK_ELIDED_ON, "elided on block")
PRIMITIVE_R(BLOCK_BEGIN_ON, "begin on block")
PRIMITIVE_R(BLOCK_COBEGIN_ON, "cobegin on block")
PRIMITIVE_R(BLOCK_COFORALL_ON, "coforall on block")
PRIMITIVE_R(BLOCK_LOCAL, "local block")
PRIMITIVE_R(BLOCK_UNLOCAL, "unlocal block")

PRIMITIVE_R(HAS_LEADER, "has leader")
PRIMITIVE_R(TO_LEADER, "to leader")
PRIMITIVE_R(TO_FOLLOWER, "to follower")
PRIMITIVE_R(TO_STANDALONE, "to standalone")

PRIMITIVE_R(CALL_DESTRUCTOR, "call destructor")

PRIMITIVE_R(LOGICAL_FOLDER, "_paramFoldLogical")

PRIMITIVE_G(WIDE_MAKE, "_wide_make")

PRIMITIVE_G(WIDE_GET_LOCALE, "_wide_get_locale")

PRIMITIVE_G(WIDE_GET_NODE, "_wide_get_node")
PRIMITIVE_G(WIDE_GET_ADDR, "_wide_get_addr")
PRIMITIVE_R(IS_WIDE_PTR, "is wide pointer")

PRIMITIVE_G(ON_LOCALE_NUM, "chpl_on_locale_num")
PRIMITIVE_G(GET_REQUESTED_SUBLOC, "chpl_task_getRequestedSubloc")

PRIMITIVE_G(REGISTER_GLOBAL_VAR, "_register_global_var")
PRIMITIVE_G(BROADCAST_GLOBAL_VARS, "_broadcast_global_vars")
PRIMITIVE_G(PRIVATE_BROADCAST, "_private_broadcast")

PRIMITIVE_G(INT_ERROR, "_internal_error")

PRIMITIVE_R(CAPTURE_FN, "capture fn")
PRIMITIVE_R(CAPTURE_FN_TO_CLASS, "capture fn to class")
PRIMITIVE_R(CREATE_FN_TYPE, "create fn type")

PRIMITIVE_G(STRING_COMPARE, "string_compare")
PRIMITIVE_G(STRING_CONTAINS, "string_contains")
PRIMITIVE_G(STRING_CONCAT, "string_concat")
PRIMITIVE_G(STRING_LENGTH_BYTES, "string_length_bytes")
PRIMITIVE_G(STRING_LENGTH_CODEPOINTS, "string_length_codepoints")
PRIMITIVE_G(ASCII, "ascii")
PRIMITIVE_R(STRING_ITEM, "string item")
PRIMITIVE_R(BYTES_ITEM, "bytes item")
PRIMITIVE_G(STRING_INDEX, "string_index")
PRIMITIVE_G(STRING_COPY, "string_copy")
PRIMITIVE_G(STRING_SELECT, "string_select")

PRIMITIVE_G(CAST_TO_VOID_STAR, "cast_to_void_star")
PRIMITIVE_G(CAST_TO_TYPE, "cast_to_type")

PRIMITIVE_G(SLEEP, "sleep")
PRIMITIVE_G(REAL_TO_INT, "real2int")
PRIMITIVE_G(OBJECT_TO_INT, "object2int")
PRIMITIVE_G(CHPL_EXIT_ANY, "chpl_exit_any")

PRIMITIVE_G(RT_ERROR, "chpl_error")
PRIMITIVE_G(RT_WARNING, "chpl_warning")

PRIMITIVE_G(NEW_PRIV_CLASS, "chpl_newPrivatizedClass")

PRIMITIVE_R(GET_USER_LINE, "_get_user_line")
PRIMITIVE_R(GET_USER_FILE, "_get_user_file")

PRIMITIVE_R(RESOLUTION_POINT, "resolution point")
PRIMITIVE_G(FTABLE_CALL, "call ftable function")

PRIMITIVE_R(IS_TUPLE_TYPE, "is tuple type")
PRIMITIVE_R(IS_STAR_TUPLE_TYPE, "is star tuple type")

PRIMITIVE_G(SET_SVEC_MEMBER, "set svec member")
PRIMITIVE_G(GET_SVEC_MEMBER, "get svec member")
PRIMITIVE_G(GET_SVEC_MEMBER_VALUE, "get svec member value")

PRIMITIVE_G(VIRTUAL_METHOD_CALL, "virtual method call")

PRIMITIVE_R(NUM_FIELDS, "num fields")
PRIMITIVE_R(FIELD_NUM_TO_NAME, "field num to name")
PRIMITIVE_R(FIELD_NAME_TO_NUM, "field name to num")
PRIMITIVE_R(FIELD_BY_NUM, "field by num")

PRIMITIVE_G(CLASS_NAME_BY_ID, "class name by id")

PRIMITIVE_R(ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL, "iterator record field value by formal")
PRIMITIVE_R(ITERATOR_RECORD_SET_SHAPE, "iterator record set shape")
PRIMITIVE_R(IS_GENERIC_TYPE, "is generic type")
PRIMITIVE_R(IS_CLASS_TYPE, "is class type")
PRIMITIVE_R(IS_NILABLE_CLASS_TYPE, "is nilable class type")
PRIMITIVE_R(IS_NON_NILABLE_CLASS_TYPE, "is non nilable class type")
PRIMITIVE_R(IS_RECORD_TYPE, "is record type")
PRIMITIVE_R(IS_UNION_TYPE, "is union type")
PRIMITIVE_R(IS_EXTERN_UNION_TYPE, "is extern union type")
PRIMITIVE_R(IS_ATOMIC_TYPE, "is atomic type")
PRIMITIVE_R(IS_REF_ITER_TYPE, "is ref iter type")
PRIMITIVE_R(IS_EXTERN_TYPE, "is extern type")
PRIMITIVE_R(IS_BORROWED_CLASS_TYPE, "is borrowed class type")
PRIMITIVE_R(IS_ABS_ENUM_TYPE, "is abstract enum type")

PRIMITIVE_R(IS_POD, "is pod type")
PRIMITIVE_R(IS_COPYABLE, "is copyable type")
PRIMITIVE_R(IS_CONST_COPYABLE, "is const copyable type")
PRIMITIVE_R(IS_ASSIGNABLE, "is assignable type")
PRIMITIVE_R(IS_CONST_ASSIGNABLE, "is const assignable type")
PRIMITIVE_R(HAS_DEFAULT_VALUE, "type has default value")
PRIMITIVE_R(NEEDS_AUTO_DESTROY, "needs auto destroy")
PRIMITIVE_R(END_OF_STATEMENT, "end of statement")
PRIMITIVE_R(STEAL, "steal")
PRIMITIVE_R(AUTO_DESTROY_RUNTIME_TYPE, "auto destroy runtime type")

PRIMITIVE_R(GET_RUNTIME_TYPE_FIELD, "get runtime type field")

PRIMITIVE_R(COERCE, "coerce")

PRIMITIVE_R(CALL_RESOLVES, "call resolves")
PRIMITIVE_R(METHOD_CALL_RESOLVES, "method call resolves")
PRIMITIVE_R(CALL_AND_FN_RESOLVES, "call and fn resolves")
PRIMITIVE_R(METHOD_CALL_AND_FN_RESOLVES, "method call and fn resolves")
PRIMITIVE_R(RESOLVES, "resolves")

PRIMITIVE_G(START_RMEM_FENCE, "chpl_rmem_consist_acquire")
PRIMITIVE_G(FINISH_RMEM_FENCE, "chpl_rmem_consist_release")

PRIMITIVE_G(LOOKUP_FILENAME, "chpl_lookupFilename")

PRIMITIVE_R(GET_COMPILER_VAR, "get compiler variable")
PRIMITIVE_R(GET_VISIBLE_SYMBOLS, "get visible symbols")

PRIMITIVE_G(STACK_ALLOCATE_CLASS, "stack allocate class")

PRIMITIVE_G(ZERO_VARIABLE, "zero variable")

PRIMITIVE_R(ZIP, "zip")
PRIMITIVE_R(REQUIRE, "require")

PRIMITIVE_R(CHECK_ERROR, "check error")
PRIMITIVE_R(CURRENT_ERROR, "current error")

PRIMITIVE_R(TO_UNMANAGED_CLASS_CHECKED, "to unmanaged class from unknown")
PRIMITIVE_R(TO_BORROWED_CLASS_CHECKED, "to borrowed class from unknown")
PRIMITIVE_R(TO_NILABLE_CLASS_CHECKED, "to nilable class from type")
PRIMITIVE_R(TO_UNMANAGED_CLASS, "to unmanaged class")
PRIMITIVE_R(TO_BORROWED_CLASS, "to borrowed class")
PRIMITIVE_R(TO_UNDECORATED_CLASS, "to undecorated class")
PRIMITIVE_R(TO_NILABLE_CLASS, "to nilable class")
PRIMITIVE_R(TO_NON_NILABLE_CLASS, "to non nilable class")

PRIMITIVE_R(SET_ALIASING_ARRAY_ON_TYPE, "set aliasing array on type")

PRIMITIVE_G(INVARIANT_START, "invariant start")
PRIMITIVE_G(NO_ALIAS_SET, "no alias set")
PRIMITIVE_G(COPIES_NO_ALIAS_SET, "copies no alias set")

PRIMITIVE_G(OPTIMIZATION_INFO, "optimization info")

PRIMITIVE_R(GATHER_TESTS, "gather tests")
PRIMITIVE_R(GET_TEST_BY_INDEX, "get test by index")
PRIMITIVE_R(GET_TEST_BY_NAME, "get test by name")

PRIMITIVE_R(VERSION_MAJOR, "version major")
PRIMITIVE_R(VERSION_MINOR, "version minor")
PRIMITIVE_R(VERSION_UPDATE, "version update")
PRIMITIVE_R(VERSION_SHA, "version sha")

PRIMITIVE_R(REF_DESERIALIZE, "deserialize for ref fields")

PRIMITIVE_R(UINT32_AS_REAL32, "uint32 as real32")
PRIMITIVE_R(UINT64_AS_REAL64, "uint64 as real64")
PRIMITIVE_R(REAL32_AS_UINT32, "real32 as uint32")
PRIMITIVE_R(REAL64_AS_UINT64, "real64 as uint64")
