/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

// These are flags for Symbols - see flags.h for details.
// The format is:
//   PRAGMA(NAME, PRAGMA, MAPNAME, COMMENT)
// where
//   NAME     - the enum symbol (-> enum Flag, flagNames)
//   PRAGMA   - whether this flag can be set via a Chapel pragma
//              see YPR/NPR shorthands (-> flagPragma)
//   MAPNAME  - a unique string (-> flagMap);
//              the pragma string, if the flag is settable via a pragma
//   COMMENT  - a (possibly-empty) comment string (-> flagComments)

#define ypr true  /* YES, the flag can be set via a pragma */
#define npr false /* NO, cannot be set via a pragma */
#define ncm ""    /* no comment */

// flags to be added to the aggregation markers
PRAGMA(AGG_MARKER, npr, "aggregation marker", ncm)
PRAGMA(AGG_IN_STATIC_ONLY_CLONE, npr,
       "static only aggregation marker",
       "this aggregation is happening in a static only forall clone")
PRAGMA(AGG_IN_STATIC_AND_DYNAMIC_CLONE, npr,
       "static and dynamic aggregation marker",
       "this aggregation is happening in a static and dynamic forall clone")
PRAGMA(AGG_GENERATOR, ypr,
       "aggregator generator",
       "this function generates and returns an aggregator")

// Indicates an array implementation class can alias other array implementations
// e.g. array views
PRAGMA(ALIASING_ARRAY, ypr, "aliasing array", ncm)
// alias analysis can assume that the marked field (or return from a function)
// can alias the same scopes as 'this'
PRAGMA(ALIAS_SCOPE_FROM_THIS, ypr, "alias scope from this", ncm)

// This flag is used in scalarReplace.cpp to determine if an assignment of a ref
// has an allocator as the RHS.  If so, then it is not creating an alias, since
// the allocator function does not retain a reference to the referenced object.
PRAGMA(ALLOCATOR, ypr, "allocator", "allocates heap storage")
PRAGMA(ALLOW_REF, ypr, "allow ref", ncm)
PRAGMA(ALWAYS_PROPAGATE_LINE_FILE_INFO, ypr,
       "always propagate line file info",
       "counterpart to INSERT_LINE_FILE_INFO")
PRAGMA(ALWAYS_RVF, ypr,
       "always RVF",
       "attach to a type to force RVF for objects of that type")
PRAGMA(ANONYMOUS_FN, npr, "anonymous fn", ncm)
PRAGMA(LEGACY_LAMBDA, npr, "legacy lambda", ncm)
PRAGMA(ANONYMOUS_FORMAL, npr, "anonymous formal", ncm)
PRAGMA(DEAD_END_OF_BLOCK, ypr, "dead at end of block", ncm)
PRAGMA(DEAD_LAST_MENTION, ypr, "dead after last mention", ncm)
PRAGMA(DONT_ALLOW_REF, ypr, "do not allow ref", ncm)
PRAGMA(DONT_UNREF_FOR_YIELDS, ypr, "do not unref for yields", ncm)
PRAGMA(ARG_THIS, npr, "arg this", "the hidden object argument")
PRAGMA(ARRAY, ypr, "array", ncm)
PRAGMA(ARRAY_OF_BORROWS, npr, "array of borrows", "array of borrows")
PRAGMA(ASSIGNOP, npr,
       "assignop",
       "this fn is assignment or an <op>= operator")
PRAGMA(ATOMIC_MODULE, ypr, "atomic module",
       "module containing implementations of atomic types")
PRAGMA(ATOMIC_TYPE, ypr, "atomic type", "type that implements an atomic")
PRAGMA(AUTO_COPY_FN,  ypr, "auto copy fn", "auto copy function")
PRAGMA(AUTO_DESTROY_FN,  ypr, "auto destroy fn", "auto destroy function")
PRAGMA(AUTO_II, npr, "auto ii", ncm)
PRAGMA(BASE_ARRAY, ypr, "base array", ncm)
PRAGMA(BASE_DOMAIN, ypr, "base domain", ncm)
PRAGMA(BASE_DIST, ypr, "base dist", ncm)
PRAGMA(BEGIN, npr, "begin", ncm)
PRAGMA(BEGIN_BLOCK, npr, "begin block", ncm)
PRAGMA(BROADCAST_FN, npr, "serialized broadcast function", ncm)
PRAGMA(BUILD_TUPLE, ypr,
       "build tuple",
       "used to mark the build_tuple functions")
PRAGMA(BUILD_TUPLE_TYPE, ypr,
       "build tuple type",
       "used to mark the build_tuple type functions")

PRAGMA(C_FLEXIBLE_ARRAY_FIELD, npr,
       "c flexible array member",
       "marks fields generated for C flexible array members")

PRAGMA(CHAPEL_STRING_LITERAL, npr,
       "chapel string literal id",
       "mark Chapel strings created from literals")
PRAGMA(CHAPEL_BYTES_LITERAL, npr,
       "chapel bytes literal id",
       "mark Chapel bytes created from literals")
// When resolution encounters the def of the variable 'chpl__iter',
// as indicated by this flag, it launches into enacting forall intents
// for the forall loop that this variable was created for.
PRAGMA(CHPL__ITER, npr,
       "chpl__iter",
       "used as a marker to implement forall intents")
// Marks chpl__iter things created for ForallStmt.
PRAGMA(CHPL__ITER_NEWSTYLE, npr, "chpl__iter_newstyle", ncm)
PRAGMA(COBEGIN_OR_COFORALL, npr, "cobegin or coforall", ncm)
PRAGMA(COBEGIN_OR_COFORALL_BLOCK, npr, "cobegin or coforall block", ncm)
PRAGMA(COERCE_TEMP, npr,
       "coerce temp",
       "a temporary that was stores the result of a coercion")
PRAGMA(COERCE_FN, ypr, "coerce fn", "coerce copy/move function")
PRAGMA(CODEGENNED, npr,
       "codegenned",
       "code has been generated for this type")
PRAGMA(COFORALL_INDEX_VAR, npr, "coforall index var", ncm)
PRAGMA(COMMAND_LINE_SETTING, ypr, "command line setting", ncm)
// The compiler-generated flag has these meanings:
// 1. In various parts of the compiler, when printing filename/lineno
//    information in error messages, callstack locations marked
//    compiler-generated may be ignored when developer == false.
// 2. In relation to determination of POD types when "compiler generated"
//    applies to assignment, copy, initialization routines
// 3. When additional checking for user-written code can be relaxed
//    for functions added by the compiler (e.g. with error handling)
PRAGMA(COMPILER_GENERATED, ypr,
       "compiler generated",
       "marks functions that are compiler-generated or supplied by an "
       "internal module")
PRAGMA(COMPILER_ADDED_WHERE, npr,
       "compiler added where",
       "marks functions that have a where clause only because compiler "
       "added one")
PRAGMA(COMPILER_ADDED_AGGREGATOR, npr,
       "compiler added aggregator",
       "marks aggregator symbols that the compiler added")

PRAGMA(COMPILER_NESTED_FUNCTION, npr, "compiler nested function", ncm)
PRAGMA(CONCURRENTLY_ACCESSED, npr,
       "concurrently accessed",
       "local variables accessed by multiple threads")
PRAGMA(CONFIG, npr, "config", "config variable, constant, or parameter")
PRAGMA(CONST, npr, "const", "constant")
// this shadow variable is constant, whereas the outer variable is not
PRAGMA(CONST_DUE_TO_TASK_FORALL_INTENT, npr,
       "const due to task or forall intent",
       ncm)
PRAGMA(C_ARRAY, ypr, "c_array record", "marks c_array record")
PRAGMA(C_PTR_CLASS, ypr, "c_ptr class", "marks c_ptr class")
PRAGMA(C_PTRCONST_CLASS, ypr, "c_ptrConst class", "marks c_ptrConst class")
PRAGMA(COPY_MUTATES, ypr,
       "copy mutates",
       "the initCopy function / copy initializer takes its argument by ref")
PRAGMA(DATA_CLASS, ypr, "data class", ncm)

// Flag for temporaries created for default values
PRAGMA(DEFAULT_ACTUAL, npr, "default actual temp", ncm)
PRAGMA(DEFAULT_ACTUAL_FUNCTION, npr,
       "default actual function",
       "applied to functions created for formal default values")

// Enable override for default-intent for types defined in terms of record/class
PRAGMA(DEFAULT_INTENT_IS_REF, ypr,
       "default intent is ref",
       "The default intent for this type is ref")

// Default intent should be determined by function body
// (ie. it is a ref if it is modified in the function body)
PRAGMA(DEFAULT_INTENT_IS_REF_MAYBE_CONST, ypr,
       "default intent is ref if modified",
       "The default intent for this type is ref if modified const "
       "ref otherwise")

PRAGMA(COPY_INIT, npr, "copy initializer", ncm)
PRAGMA(DEFAULT_INIT, npr, "default initializer", ncm)
PRAGMA(DESTRUCTOR, npr,
       "destructor",
       "applied to functions that are destructors")
PRAGMA(DEPRECATED, npr,
       "deprecated",
       "applied to symbols that are deprecated")
PRAGMA(DISTRIBUTION, ypr, "distribution", ncm)
PRAGMA(DOCS_ONLY, ypr,
       "docs only",
       "this symbol is for documentation purposes only - ignore this "
       "symbol when compiling")
PRAGMA(DOMAIN, ypr, "domain", ncm)
PRAGMA(DONT_DISABLE_REMOTE_VALUE_FORWARDING, ypr,
       "dont disable remote value forwarding",
       ncm)
PRAGMA(DOWN_END_COUNT_FN, ypr, "down end count fn", ncm)
PRAGMA(END_COUNT, ypr, "end count", ncm)
PRAGMA(ERRONEOUS_COPY, ypr, "erroneous copy", ncm)
PRAGMA(ERROR_MODE_FATAL, ypr, "error mode fatal", ncm)
PRAGMA(ERROR_MODE_RELAXED, ypr, "error mode relaxed", ncm)
PRAGMA(ERROR_MODE_STRICT, ypr, "error mode strict", ncm)
PRAGMA(ERROR_VARIABLE, npr, "error variable", ncm)
PRAGMA(EPILOGUE_LABEL, npr,
       "epilogue label",
       "distinguishes the epilogue label from other labels")
PRAGMA(ERROR_LABEL, ypr, "error label", ncm)
PRAGMA(EXPANDED_VARARGS, npr, "expanded varargs", ncm)
PRAGMA(EXPAND_TUPLES_WITH_VALUES, ypr, "expand tuples with values", ncm)
PRAGMA(EXPORT, npr, "export", ncm)
PRAGMA(EXPORT_INIT, ypr,
       "export init",
       "indicate that the module's initialization function should "
       "be exported")
PRAGMA(EXPORT_WRAPPER, ypr, "export wrapper", ncm)
PRAGMA(EXPR_TEMP, npr,
       "expr temp",
       "temporary that stores the result of an expression")
PRAGMA(EXTERN, npr, "extern", "extern variables, types, and functions")
PRAGMA(EXTERN_UNION, npr, "extern union", "extern union")
PRAGMA(EXTERN_FN_WITH_ARRAY_ARG, "npr",
       "extern fn with array arg",
       "extern functions with array arguments")
PRAGMA(FAST_ON, npr,
       "fast on",
       "with ON/ON_BLOCK, \"on block\", use fast spawning "
       "option (if available)")
PRAGMA(FAST_ON_SAFE_EXTERN, ypr,
       "fast-on safe extern function",
       "extern function is safe for fast-on optimization")
PRAGMA(FIELD_ACCESSOR, npr,
       "field accessor",
       "field setter/getter function, user-declared or compiler-generated")
PRAGMA(FIRST_CLASS_FUNCTION_INVOCATION, npr,
       "first class function invocation",
       "proxy for first-class function invocation")
PRAGMA(FN_RETARG, npr, "fn returns via _retArg", ncm)
PRAGMA(FOLLOWER_INDEX, npr,
       "follower index",
       "a variable representing a follower loop index")
PRAGMA(FORALL_BREAK_LABEL, npr,
       "forall break label",
       "target of error handling in the forall")
PRAGMA(FORMAL_TEMP, npr,
       "formal temp",
       "a formal temp requiring write-back for an out or inout argument")
PRAGMA(FORMAL_TEMP_OUT, npr, "formal temp out", "a formal temp to back an out argument")
PRAGMA(FORMAL_TEMP_OUT_CALLSITE, npr, "formal temp out callsite", "a formal temp to handle out/inout writeback at the callsite")
PRAGMA(FORWARDING_FN, npr, "forwarding function", ncm)
PRAGMA(FUNCTION_CLASS, npr, "function class", "function class representation")
PRAGMA(FUNCTION_TERMINATES_PROGRAM, ypr, "function terminates program", "function that causes the program to exit")
PRAGMA(GENERATE_SIGNATURE, ypr, "generate signature", "compiler should codegen a function signature")
// When applied to an argument, this flag means that the arg accepts a value
// but has unspecified type.
PRAGMA(GENERIC, npr, "generic", "generic types, functions and arguments")
PRAGMA(DELAY_GENERIC_EXPANSION, npr, "delay instantiation", "generics instances whose instantiation  will be determined shortly")
PRAGMA(GEN_MAIN_FUNC, npr, "generated main", "compiler generated main function")

PRAGMA(GET_LINE_NUMBER, ypr, "get line number", "replace calls to this function with the line number of the call")
PRAGMA(GET_FILE_NAME, ypr, "get file name", "replace calls to this function with the name of the file the call is in")
PRAGMA(GET_FUNCTION_NAME, ypr, "get function name", "replace calls to this function with the name of the function that called it")
PRAGMA(GET_MODULE_NAME, ypr, "get module name", "replace calls to this function with the name of the module the call was in")

PRAGMA(GLOBAL_TYPE_SYMBOL, ypr, "global type symbol", "is accessible through a global type variable")
PRAGMA(GLOBAL_VAR_BUILTIN, ypr, "global var builtin", "is accessible through a global symbol variable")
PRAGMA(GPU_CODEGEN, ypr, "codegen for GPU", "generate GPU code and set function calling convention to kernel launch")
PRAGMA(GPU_AND_CPU_CODEGEN, ypr, "codegen for CPU and GPU", "generate both GPU and CPU code")
PRAGMA(ASSERT_ON_GPU, ypr, "assert on gpu", "triggers runtime assertion if not running on device")

PRAGMA(HAS_POSTINIT, ypr, "has postinit", "type that has a postinit method")
PRAGMA(HAS_RUNTIME_TYPE, ypr, "has runtime type", "type that has an associated runtime type")

PRAGMA(IGNORE_RUNTIME_TYPE, ypr, "ignore runtime type", "use the static type only in the return value")
PRAGMA(IGNORE_IN_GLOBAL_ANALYSIS, ypr, "ignore in global analysis", "ignore this function in global use-before-def analysis")
PRAGMA(RVV, npr, "RVV", "variable is the return value variable")
PRAGMA(YVV, npr, "YVV", "variable is a yield value variable")
PRAGMA(HEAP, npr, "heap", ncm)
PRAGMA(IF_EXPR_RESULT, npr, "if-expr result", ncm)
PRAGMA(IMPLICIT_ALIAS_FIELD, npr, "implicit alias field", ncm)
PRAGMA(IMPLICIT_MODULE, npr, "implicit top-level module", ncm)
PRAGMA(INCLUDED_MODULE, npr, "included sub-module", ncm)
PRAGMA(INDEX_VAR, npr, "index var", ncm)

PRAGMA(MANAGER_HANDLE, npr, "manager handle", ncm)
PRAGMA(MANAGER_RESOURCE_INFER_STORAGE, npr, "manager resource infer storage", ncm)

// This can also mark a temp that serves as an intermediate step of
// destructuring a tuple-typed INDEX_OF_INTEREST variable
// into loop index variables.
PRAGMA(INDEX_OF_INTEREST, npr, "an _indexOfInterest or chpl__followIdx variable", ncm)
PRAGMA(INIT_COPY_FN,  ypr, "init copy fn", "init copy function")
PRAGMA(INIT_TUPLE, ypr, "tuple init fn", ncm)
PRAGMA(INLINE, npr, "inline", ncm)
PRAGMA(INLINED_FN, npr, "inlined function", ncm)
PRAGMA(INLINE_ITERATOR, npr, "inline iterator", "iterators that are always inlined, e.g., leaders")
PRAGMA(INSERT_AUTO_DESTROY, ypr, "insert auto destroy", ncm)
PRAGMA(INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW, npr, "insert auto destroy for explicit new", ncm)
PRAGMA(INSERT_LINE_FILE_INFO, ypr, "insert line file info", ncm)
PRAGMA(INSTANTIATED_PARAM, npr, "instantiated param", "this formal is bound to a param")
PRAGMA(INSTANTIATED_GENERIC, npr, "instantiated generic", "this is an instantiation of a generic")
PRAGMA(INSTANTIATED_FROM_ANY, npr, "instantiated from any", "this is an instantiation from any type")
PRAGMA(INTENT_REF_MAYBE_CONST_FORMAL, ypr, "intent ref maybe const formal", "The intent for this formal is ref if it is modified, const ref otherwise")
PRAGMA(IMPLEMENTS_WRAPPER, npr, "ImplementsStmt wrapper", ncm)
PRAGMA(INVISIBLE_FN, npr, "invisible fn", "invisible function (not a candidate for resolution)")
PRAGMA(ITERATOR_CLASS, npr, "iterator class", ncm)
PRAGMA(ITERATOR_FN, npr, "iterator fn", ncm)
PRAGMA(ITERATOR_RECORD, npr, "iterator record", ncm)
PRAGMA(ITERATOR_WITH_ON, npr, "iterator with on", "iterator which contains an on block")

PRAGMA(ALWAYS_RESOLVE, ypr, "always resolve function", "keep this function even if it is not called so it can be called during codegen e.g.")

// In resolution, functions marked as last-resort are considered only if
// no functions without that flag are found. This usually is used to create
// a pattern enabling user-supplied replacement of default behavior.
PRAGMA(LAST_RESORT, ypr, "last resort", "overload of last resort in resolution")

// These flags help to decorate module code to enable nil-checking

// indicates that the function, on return, will leave 'this' argument
// storing 'nil'
PRAGMA(LEAVES_THIS_NIL, ypr, "leaves this nil", ncm)
// indicates that the function, on return, will leave the marked
// argument storing 'nil'
PRAGMA(LEAVES_ARG_NIL, ypr, "leaves arg nil", ncm)
// indicates that nil-ness of the result should be based
// upon the nil-ness of the particular argument
PRAGMA(NIL_FROM_ARG, ypr, "nil from arg", ncm)
// indicates that the nil-ness of the result should be based
// upon the nil-ness of 'this'
PRAGMA(NIL_FROM_THIS, ypr, "nil from this", ncm)

// Tells resolution to use this function's line number even if that function
// has COMPILER_GENERATED.
PRAGMA(LINE_NUMBER_OK, ypr, "lineno ok", ncm)

PRAGMA(LLVM_READNONE, ypr, "llvm readnone", ncm)
PRAGMA(LLVM_RETURN_NOALIAS, ypr, "llvm return noalias", ncm)

PRAGMA(LOCALE_MODEL_ALLOC, ypr, "locale model alloc", "locale model specific alloc")
PRAGMA(LOCALE_MODEL_FREE, ypr, "locale model free", "locale model specific free")
PRAGMA(INC_RUNNING_TASK, ypr, "inc running task", "running task incrementer")
PRAGMA(DEC_RUNNING_TASK, ypr, "dec running task", "running task decrementer")
PRAGMA(LOCALE_PRIVATE, ypr, "locale private", ncm)

// The arguments to this function are all values or narrow pointers.
// Calls to an extern function use only narrow args and expect a narrow return.
// Implementation of an export function declares only narrow args and has a
// narrow return value.
PRAGMA(LOCAL_ARGS, ypr, "local args", "arguments and return value are narrow")
PRAGMA(LOCAL_FIELD, ypr, "local field", "field is local to aggregate type")
PRAGMA(LOCAL_FN, ypr, "local fn", "function is completely local (no communication)")
PRAGMA(LOCAL_ON, npr, "local on", ncm)
PRAGMA(LOOP_BODY_ARGUMENT_CLASS, npr, "loop body argument class", ncm)
PRAGMA(MANAGED_POINTER, ypr, "managed pointer", "e.g. Owned and Shared")
PRAGMA(MANAGED_POINTER_NONNILABLE, npr, "managed pointer nonnilable", "e.g. non-nilable Owned and Shared")
PRAGMA(MARKED_GENERIC, npr, "marked generic", "formal is marked generic using the type query syntax")
PRAGMA(MAYBE_ARRAY_TYPE, npr, "maybe array type", "function may be computing array type")
PRAGMA(MAYBE_COPY_ELIDED, npr, "maybe copy elided", "symbol might be dead early due to copy elision")
PRAGMA(MAYBE_PARAM, npr, "maybe param", "symbol can resolve to a param")
PRAGMA(MAYBE_REF, npr, "maybe ref", "symbol can resolve to a ref")
PRAGMA(SPLIT_INITED, npr, "split inited", "variable was initialized with split init")
PRAGMA(MAYBE_TYPE, npr, "maybe type", "symbol can resolve to a type")
PRAGMA(MEMORY_ORDER_TYPE, ypr, "memory order type", "type implementing chpl memory order (normally called memoryOrder)")
PRAGMA(C_MEMORY_ORDER_TYPE, ypr, "c memory order type", "type implementing c memory order (normally called memory_order)")
PRAGMA(METHOD, npr, "method", "function that is a method")
PRAGMA(METHOD_PRIMARY, npr, "primary method", "function that is a method and defined in type declaration")
PRAGMA(MODIFIES_CONST_FIELDS, npr, "modifies const fields", "... of 'this' argument")

// marks top-level modules mentioned on the compiler command line
PRAGMA(MODULE_FROM_COMMAND_LINE_FILE, npr, "module from command line file", "This is a module that came from a file named on the compiler command line")

PRAGMA(MODULE_INIT, npr, "module init", "a module init function")

PRAGMA(MODULE_INCLUDED_BY_DEFAULT, ypr, "module included by default", "module is included by default")

// This flag marks the result of an autoCopy as necessary.
// Necessary autoCopies are not removed by the removeUnnecessaryAutoCopyCalls optimization.
PRAGMA(NECESSARY_AUTO_COPY, npr, "necessary auto copy", "a variable containing a necessary autoCopy")
PRAGMA(NEW_WRAPPER, npr, "_new wrapper", ncm)
PRAGMA(IGNORE_NOINIT, ypr, "ignore noinit", "this type must be initialized")
PRAGMA(NON_BLOCKING, npr, "non blocking", "with ON/ON_BLOCK, non-blocking on functions")
PRAGMA(NO_AUTO_DESTROY, ypr, "no auto destroy", ncm)
PRAGMA(NO_FN_BODY, npr, "no fn body", "function has no body")
PRAGMA(NO_BORROW_CONVERT, ypr, "no borrow convert", "arguments that are instantiated as owned/shared/etc do not need to convert to borrow")
PRAGMA(NO_CAPTURE_FOR_TASKING, npr, "no capture for tasking", "does not need to be captured before spawning tasks")
PRAGMA(NO_CODEGEN, ypr, "no codegen", "do not generate e.g. C code defining this symbol")
PRAGMA(NO_COPY, ypr, "no copy", "do not apply chpl__initCopy to initialization of a variable")
PRAGMA(ERROR_ON_COPY, ypr, "error on copy", "error if actual is copied at callsite")
PRAGMA(NO_COPY_RETURN, ypr, "no copy return", ncm)
PRAGMA(NO_COPY_RETURNS_OWNED, ypr, "no copy returns owned", ncm)
PRAGMA(NO_DEFAULT_FUNCTIONS, ypr, "no default functions", ncm)
PRAGMA(NO_DOC, ypr, "no doc", "do not generate chpldoc documentation for this symbol")
PRAGMA(NO_IMPLICIT_COPY, ypr, "no implicit copy", "function does not require autoCopy/autoDestroy")

// This flag disables initialization entirely. In contrast, `= noinit`
// will possibly run a `proc noinit` initializer.
PRAGMA(NO_INIT, ypr, "no init", "Do not initialize this variable")

PRAGMA(NO_OBJECT, ypr, "no object", ncm)

PRAGMA(NO_PARENS, npr, "no parens", "function without parentheses")

PRAGMA(NO_REMOTE_MEMORY_FENCE, ypr, "no remote memory fence", ncm)
PRAGMA(NO_RENAME, npr, "no rename", ncm)
PRAGMA(NO_RVF, npr, "do not RVF", ncm)
PRAGMA(NO_WIDE_CLASS, ypr, "no wide class", ncm)

PRAGMA(NO_GPU_CODEGEN, ypr, "no gpu codegen", ncm)

// See ORDER_INDEPENDENT_YIELDING_LOOPS below
PRAGMA(NOT_ORDER_INDEPENDENT_YIELDING_LOOPS, ypr, "not order independent yielding loops", "yielding loops in iterator itself are not order independent")

// See POD below
PRAGMA(NOT_POD, ypr, "not plain old data", "bit copy overridden")

PRAGMA(NOT_FULLY_GENERIC, npr, "argument is not fully generic", ncm)

PRAGMA(OBJECT_CLASS, ypr, "object class", ncm)

// ON and ON_BLOCK mark task functions and their wrappers,
// respectively, that perform remote operations, i.e. corresponding to
// 'on' statements. Some task functions with ON also create new
// Chapel tasks - as a result of the "begin+on" optimization. They
// have the following flags:
//  begin+on       ON  NON_BLOCKING  BEGIN
//  on+begin       ON  NON_BLOCKING  BEGIN
//  cobegin+on     ON  NON_BLOCKING  COBEGIN_OR_COFORALL
//  coforall+on    ON  NON_BLOCKING  COBEGIN_OR_COFORALL
//  just 'on'      ON  // no new Chapel tasks
// For each of the above flags, the task function's wrapper has
// the corresponding flag:
//   ON                  --> ON_BLOCK
//   NON_BLOCKING        --> NON_BLOCKING (the same flag;
//     btw it does not apply to local (non-'on') task functions/wrappers)
//   BEGIN               --> BEGIN_BLOCK
//   COBEGIN_OR_COFORALL --> COBEGIN_OR_COFORALL_BLOCK
//
PRAGMA(ON, npr, "on", ncm)
PRAGMA(ON_BLOCK, npr, "on block", ncm)

PRAGMA(OPERATOR, npr, "operator", ncm)

PRAGMA(OVERRIDE, npr, "method overrides", ncm)

// variables added by flatten functions
PRAGMA(OUTER_VARIABLE, npr, "outer variable", ncm)

// this (task) function is not within a try or try! or a 'throws' function
PRAGMA(OUTSIDE_TRY, npr, "outside try", ncm)

// This means that the yielding loops themselves within an iterator
// are order independent. It does not mean that all uses of the iterator
// are order independent. And, it does not assert that iterators invoked
// by this iterator are also order independent.
//
//  for x in myIter()  // still not order independent
//
// if myIter contains
//    for i in otherIterator()
//
// then the resulting for loop is only order independent if otherIterator
// is as well.
//
// So this applies only to code within this specific iterator.
// It should generally be set on serial, standalone, and follower iterators.
// Not setting it implies that the loop has a vectorization hazard.
PRAGMA(ORDER_INDEPENDENT_YIELDING_LOOPS, ypr, "order independent yielding loops", "yielding loops in iterator itself are order independent")

PRAGMA(OWNED, ypr, "owned", "owned class instance for lifetime checking")

PRAGMA(PARAM, npr, "param", "parameter (compile-time constant)")

PRAGMA(PARENT_FIELD, npr, "parent field", "field from parent type")

PRAGMA(PARTIAL_TUPLE, npr, "partial tuple", ncm)

// Is this type a Plain-Old Data (POD) type - ie no autocopy/destructor/=
// need ever be called - bit copies will do.
// Each aggregate type gets either POD or NOT_POD during resolution.
// To check POD-ness of an aggregate type:
//  * during resolution: use propagateNotPOD()
//  * after resolution: use isPOD()
// To check POD-ness of an arbitrary type after resolution:
//  * use isPOD()
PRAGMA(POD, ypr, "plain old data", "data can be bit copied")

// This flag is used to identify variables to which accesses within
// an order-independent loop can be vectorized by the LLVM loop vectorizer.
// The LLVM loop vectorizer has problems with loop-local stack variables.
// These will be allocated outside of the loop in the IR (with an alloca
// instruction). So, it should mark all variables other than those that
// are loop-local stack variables inside of an order-independent loop.
PRAGMA(POINTS_OUTSIDE_ORDER_INDEPENDENT_LOOP, npr, "points outside order independent loop", "points to memory other than local variables inside order independent loop")

PRAGMA(PRIMITIVE_TYPE, ypr, "primitive type", "attached to primitive types to keep them from being deleted")
PRAGMA(PRINT_MODULE_INIT_FN, ypr, "print module init fn", ncm)
PRAGMA(PRINT_MODULE_INIT_INDENT_LEVEL, ypr, "print module init indent level", ncm)
PRAGMA(PRIVATE, npr, "private", ncm)
PRAGMA(PROMOTION_WRAPPER, npr, "promotion wrapper", ncm)
PRAGMA(PROMOTION_PROTO_FIELD, npr, "temporary promotion field", ncm)
PRAGMA(PROMOTION_ITERATOR_RECORD, npr, "promotion iterator record", ncm)
PRAGMA(PROTOTYPE_MODULE, npr, "prototype module", ncm)
PRAGMA(RANGE, ypr, "range", "indicates the range type")
PRAGMA(RECURSIVE_ITERATOR, npr, "recursive iterator", "iterators which call themselves")
PRAGMA(REDUCESCANOP, ypr, "ReduceScanOp", "the ReduceScanOp class")
PRAGMA(REF, ypr, "ref", ncm)
PRAGMA(REF_IF_MODIFIED, npr, "ref if modified", ncm)
PRAGMA(REF_FOR_CONST_FIELD_OF_THIS, npr, "reference to a const field of 'this'", ncm)
PRAGMA(REF_ITERATOR_CLASS, npr, "ref iterator class", ncm)
// "ref to const" is like Chapel's 'const ref' variable:
// * it is illegal to modify the referenced thing through this variable
// * the referenced thing may change, observably through this variable
PRAGMA(REF_TO_CONST, npr, "reference to a const", "a temp or a function that returns a reference to a Chapel const, e.g. an accessor to a const field or its result")
PRAGMA(REF_TO_CONST_WHEN_CONST_THIS, ypr, "reference to const when const this", "a function that returns a reference to a Chapel const when 'this' is const")
PRAGMA(REF_TO_IMMUTABLE, npr, "ref to immutable", "a reference to something that never changes during its lifetime")
PRAGMA(REF_VAR, ypr, "ref var", "reference variable")
PRAGMA(REF_TEMP, npr, "ref temp", "compiler-inserted reference temporary")
PRAGMA(REMOVABLE_ARRAY_ACCESS, ypr, "removable array access", "array access calls that can be replaced with a reference")
PRAGMA(REMOVABLE_AUTO_COPY, ypr, "removable auto copy", ncm)
PRAGMA(REMOVABLE_AUTO_DESTROY, ypr, "removable auto destroy", ncm)
PRAGMA(COMPILER_ADDED_REMOTE_FENCE, ypr, "compiler added remote fence", ncm)
PRAGMA(RESOLVED, npr, "resolved", "this function has been resolved")
PRAGMA(RETARG, npr, "symbol is a _retArg", ncm)
PRAGMA(RETURNS_ALIASING_ARRAY, ypr, "fn returns aliasing array", "array alias/slice/reindex/rank change function")
PRAGMA(FN_RETURNS_ITERATOR, ypr, "fn returns iterator", "proc that can return an iterator instead of promoting it to an array")
PRAGMA(FN_UNORDERED_SAFE, ypr, "fn unordered safe", "function does not inhibit unordered optimization")
PRAGMA(FN_SYNCHRONIZATION_FREE, ypr, "fn synchronization free", "function does not include any task synchronization")
PRAGMA(RETURNS_INFINITE_LIFETIME, ypr, "fn returns infinite lifetime", "function returns a pointer with infinite lifetime for lifetime analysis")
PRAGMA(RETURN_SCOPE, npr, "return scope", "indicates an argument that can be returned without error in lifetime checking")
PRAGMA(RETURN_NOT_OWNED, ypr, "return not owned", "scope for return value should ignore owned fields")
// See buildRuntimeTypeToValueFns() in functionResolution.cpp for more info on RUNTIME_TYPE_INIT_FN
PRAGMA(RUNTIME_TYPE_INIT_FN, ypr, "runtime type init fn", "function for initializing runtime time types")
PRAGMA(RUNTIME_TYPE_VALUE, npr, "runtime type value", "associated runtime type (value)")
PRAGMA(SAFE, ypr, "safe", "safe (activate lifetime checking)")
PRAGMA(SCOPE, npr, "scope", "scoped (lifetime checking like a local variable)")
PRAGMA(SHOULD_NOT_PASS_BY_REF, npr, "should not pass by ref", "this symbol should be passed by value (not by reference) for performance, not for correctness")

PRAGMA(SUPER_CLASS, npr, "super class", ncm)
PRAGMA(SUPER_TEMP, npr, "temporary of super field", ncm)
PRAGMA(SUPPRESS_LVALUE_ERRORS, ypr, "suppress lvalue error", "do not report an lvalue error if it occurs in a function with this flag")

// represents an interface formal, assoc. type, or required function
// within a constrained generic function
PRAGMA(CG_REPRESENTATIVE, npr, "cg representative", ncm)
// this instantiation does not need to be resolved
PRAGMA(CG_INTERIM_INST, npr, "cg interim instantiation", ncm)

PRAGMA(SINGLE, ypr, "single", ncm)
PRAGMA(SYNC, ypr, "sync", ncm)

PRAGMA(SYNTACTIC_DISTRIBUTION, ypr, "syntactic distribution", ncm)
PRAGMA(TASK_FN_FROM_ITERATOR_FN, npr, "task fn from iterator fn", ncm)
PRAGMA(TASK_SPAWN_IMPL_FN, ypr, "task spawn impl fn", ncm)
PRAGMA(TASK_COMPLETE_IMPL_FN, ypr, "task complete impl fn", ncm)
PRAGMA(TASK_JOIN_IMPL_FN, ypr, "task join impl fn", ncm)
PRAGMA(TEMP, npr, "temp", "compiler-inserted temporary")

PRAGMA(TUPLE, ypr, "tuple", ncm)
PRAGMA(TUPLE_CAST_FN, ypr, "tuple cast fn", ncm)
PRAGMA(TUPLE_WITH_REF, npr, "tuple contains ref", ncm)
PRAGMA(STAR_TUPLE, ypr, "star tuple", "mark tuple types as star tuple types")
PRAGMA(STAR_TUPLE_ACCESSOR, ypr, "star tuple accessor", "this function for star tuple types")

PRAGMA(TYPE_ASSIGN_FROM_CONST, npr, "type has = from const", "type supports assignment from a const rhs")
PRAGMA(TYPE_ASSIGN_FROM_REF, npr, "type has = from ref", "type supports assignment from a potentially non-const rhs")
PRAGMA(TYPE_ASSIGN_MISSING, npr, "type has no =", "type has no assign overload")
PRAGMA(TYPE_INIT_EQUAL_FROM_CONST,  npr, "type has init= from const", "type supports init= with const other")
PRAGMA(TYPE_INIT_EQUAL_FROM_REF,  npr, "type has init= from ref", "type supports init= from a potentially non-const other argument")
PRAGMA(TYPE_INIT_EQUAL_MISSING, npr, "type has no init=", "type has no init=")
PRAGMA(TYPE_DEFAULT_VALUE, npr, "type has default value", "type has a default value")
PRAGMA(TYPE_NO_DEFAULT_VALUE, npr, "type has no default value", "type has no default value")

PRAGMA(TYPE_DEFAULT_INIT_EQUAL, npr, "type uses default init=", "type uses compiler-generated default init=")
PRAGMA(TYPE_CUSTOM_INIT_EQUAL, npr, "type uses custom init=", "type has user-provided custom init=")
PRAGMA(TYPE_DEFAULT_ASSIGN, npr, "type uses default =", "type uses compiler-generated default =")
PRAGMA(TYPE_CUSTOM_ASSIGN, npr, "type uses custom =", "type has user-provided custom =")

PRAGMA(TYPE_VARIABLE, npr, "type variable", "contains a type instead of a value")
PRAGMA(UNCHECKED_THROWS,  ypr, "unchecked throws", "function throws but handling the errors is not required even in strict mode")
PRAGMA(UNREF_FN,  ypr, "unref fn", "function to remove reference fields from tuples or copy array slices when returning")
PRAGMA(UNSAFE,  ypr, "unsafe", "unsafe (disable lifetime and nilability checking)")
PRAGMA(UNSTABLE, npr, "@unstable", "applied to symbols that may misbehave or are subject to change in the future, hence unstable")
PRAGMA(IGNORE_TRANSFER_ERRORS,  ypr, "ignore transfer errors", "ignore errors for ownership transfer from non-nilable owned")
PRAGMA(FIND_USER_LINE,  ypr, "find user line", "report errors with user line number")
PRAGMA(USER_VARIABLE_NAME,  npr, "user variable name", "temporary uses name of user variable")
PRAGMA(VECTORIZE_YIELDING_LOOPS, ypr, "vectorize yielding loops", "used to explicitly vectorize yielding loops in iterators")
PRAGMA(VIRTUAL, npr, "virtual", ncm)
PRAGMA(VOID_NO_RETURN_VALUE, npr, "no return value for void", "function does not return a value if the return type is void")
// Used to mark where a compiler generated flag was removed (but is desired
// elsewhere).
PRAGMA(WAS_COMPILER_GENERATED, npr, "was compiler generated", "used to be marked compiler generated")
PRAGMA(WIDE_REF, npr, "wide", ncm)
PRAGMA(WIDE_CLASS, npr, "wide class", ncm)
PRAGMA(WRAPPER, npr, "wrapper", "wrapper function")
PRAGMA(WRAPPER_NEEDS_START_FENCE, npr, "wrapper needs start fence", "add PRIM_START_RMEM_FENCE to the start of the wrapper function")
PRAGMA(WRAPPER_NEEDS_FINISH_FENCE, npr, "wrapper needs finish fence", "add PRIM_FINISH_RMEM_FENCE to the end of the wrapper function")
PRAGMA(WRAP_WRITTEN_FORMAL, npr, "wrap written formal", "formal argument for wrapper for out/inout intent")
PRAGMA(YIELD_WITHIN_ON, npr, "yield within on", "iterator that yields within an on")


// These flags are for use with PRIM_OPTIMIZATION_INFO
PRAGMA(OPT_INFO_LHS_OUTLIVES_FORALL, npr, "lhs outlives forall", "lhs or destination outlives forall")
PRAGMA(OPT_INFO_RHS_OUTLIVES_FORALL, npr, "rhs outlives forall", "rhs or sourceoutlives forall")
PRAGMA(OPT_INFO_NO_TASK_PRIVATE, npr, "forall no task private", "forall does not use task private storage")
PRAGMA(OPT_INFO_NO_BLOCKING, npr, "forall not blocking", "forall does not have blocking synchronization")

PRAGMA(DESERIALIZATION_BLOCK_MARKER, npr, "marks deserialization options", "then block is var else block is ref serialization")

PRAGMA(EXEMPT_INSTANTIATION_LIMIT, ypr, "fn exempt instantiation limit", "compiler will not limit the number of instantiations of this function")

PRAGMA(COMPUTE_UNIFIED_TYPE_HELP, ypr, "compute unified type helper", "identify the internal chpl_computeUnifiedTypeHelp() routine")

#undef ypr
#undef npr
#undef ncm
