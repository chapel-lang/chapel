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

// These are flags for Symbols - see flags.h for details.
// The format is:
//   symbolFlag(NAME, PRAGMA, MAPNAME, COMMENT)
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


// Indicates an array implementation class can alias other array implementations
// e.g. array views
symbolFlag( FLAG_ALIASING_ARRAY , ypr, "aliasing array" , ncm )
// alias analysis can assume that the marked field (or return from a function)
// can alias the same scopes as 'this'
symbolFlag( FLAG_ALIAS_SCOPE_FROM_THIS , ypr, "alias scope from this" , ncm )

// This flag is used in scalarReplace.cpp to determine if an assignment of a ref
// has an allocator as the RHS.  If so, then it is not creating an alias, since
// the allocator function does not retain a reference to the referenced object.
symbolFlag( FLAG_ALLOCATOR , ypr, "allocator" , "allocates heap storage" )
symbolFlag( FLAG_ALLOW_REF , ypr, "allow ref" , ncm )
symbolFlag( FLAG_ALWAYS_PROPAGATE_LINE_FILE_INFO , ypr, "always propagate line file info" , "counterpart to INSERT_LINE_FILE_INFO" )
symbolFlag( FLAG_ALWAYS_RVF, ypr, "always RVF", "attach to a type to force RVF for objects of that type" )
symbolFlag( FLAG_DEAD_END_OF_BLOCK , ypr, "dead at end of block" , ncm )
symbolFlag( FLAG_DEAD_LAST_MENTION , ypr, "dead after last mention" , ncm )
symbolFlag( FLAG_DONT_ALLOW_REF , ypr, "do not allow ref" , ncm )
symbolFlag( FLAG_ARG_THIS, npr, "arg this", "the hidden object argument")
symbolFlag( FLAG_ARRAY , ypr, "array" , ncm )
symbolFlag( FLAG_ARRAY_OF_BORROWS , npr, "array of borrows", "array of borrows" )
symbolFlag( FLAG_ASSIGNOP, npr, "assignop", "this fn is assignment or an <op>= operator" )
symbolFlag( FLAG_ATOMIC_MODULE , ypr, "atomic module" , "module containing implementations of atomic types" )
symbolFlag( FLAG_ATOMIC_TYPE , ypr, "atomic type" , "type that implements an atomic" )
symbolFlag( FLAG_AUTO_COPY_FN,  ypr, "auto copy fn" , "auto copy function" )
symbolFlag( FLAG_AUTO_DESTROY_FN,  ypr, "auto destroy fn" , "auto destroy function" )
symbolFlag( FLAG_AUTO_II , npr, "auto ii" , ncm )
symbolFlag( FLAG_BASE_ARRAY , ypr, "base array" , ncm )
symbolFlag( FLAG_BASE_DOMAIN , ypr, "base domain" , ncm )
symbolFlag( FLAG_BASE_DIST , ypr, "base dist" , ncm )
symbolFlag( FLAG_BEGIN , npr, "begin" , ncm )
symbolFlag( FLAG_BEGIN_BLOCK , npr, "begin block" , ncm )
symbolFlag( FLAG_BROADCAST_FN, npr, "serialized broadcast function", ncm)
symbolFlag( FLAG_BUILD_TUPLE , ypr, "build tuple" , "used to mark the build_tuple functions")
symbolFlag( FLAG_BUILD_TUPLE_TYPE , ypr, "build tuple type" , "used to mark the build_tuple type functions")

symbolFlag( FLAG_CHAPEL_STRING_LITERAL, npr, "chapel string literal id" , "mark Chapel strings created from literals")
symbolFlag( FLAG_CHAPEL_BYTES_LITERAL, npr, "chapel bytes literal id" , "mark Chapel bytes created from literals")
// When resolution encounters the def of the variable 'chpl__iter',
// as indicated by this flag, it launches into enacting forall intents
// for the forall loop that this variable was created for.
symbolFlag( FLAG_CHPL__ITER , npr, "chpl__iter", "used as a marker to implement forall intents" )
// Marks chpl__iter things created for ForallStmt.
symbolFlag( FLAG_CHPL__ITER_NEWSTYLE , npr, "chpl__iter_newstyle", ncm )
symbolFlag( FLAG_COBEGIN_OR_COFORALL , npr, "cobegin or coforall" , ncm )
symbolFlag( FLAG_COBEGIN_OR_COFORALL_BLOCK , npr, "cobegin or coforall block" , ncm )
symbolFlag( FLAG_COERCE_TEMP , npr, "coerce temp" , "a temporary that was stores the result of a coercion" )
symbolFlag( FLAG_CODEGENNED , npr, "codegenned" , "code has been generated for this type" )
symbolFlag( FLAG_COFORALL_INDEX_VAR , npr, "coforall index var" , ncm )
symbolFlag( FLAG_COMMAND_LINE_SETTING , ypr, "command line setting" , ncm )
// The compiler-generated flag has these meanings:
// 1. In various parts of the compiler, when printing filename/lineno
//    information in error messages, callstack locations marked
//    compiler-generated may be ignored when developer == false.
// 2. In relation to determination of POD types when "compiler generated"
//    applies to assignment, copy, initialization routines
// 3. When additional checking for user-written code can be relaxed
//    for functions added by the compiler (e.g. with error handling)
symbolFlag( FLAG_COMPILER_GENERATED , ypr, "compiler generated" , "marks functions that are compiler-generated or supplied by an internal module" )
symbolFlag( FLAG_COMPILER_ADDED_WHERE , npr, "compiler added where" , "marks functions that have a where clause only because compiler added one" )

symbolFlag( FLAG_COMPILER_NESTED_FUNCTION , npr, "compiler nested function" , ncm )
symbolFlag( FLAG_CONCURRENTLY_ACCESSED , npr, "concurrently accessed" , "local variables accessed by multiple threads" )
symbolFlag( FLAG_CONFIG , npr, "config" , "config variable, constant, or parameter" )
symbolFlag( FLAG_CONST , npr, "const" , "constant" )
// this shadow variable is constant, whereas the outer variable is not
symbolFlag( FLAG_CONST_DUE_TO_TASK_FORALL_INTENT , npr, "const due to task or forall intent", ncm )
symbolFlag( FLAG_C_ARRAY , ypr, "c_array record" , "marks c_array record" )
symbolFlag( FLAG_C_PTR_CLASS , ypr, "c_ptr class" , "marks c_ptr class" )
symbolFlag( FLAG_COPY_MUTATES , ypr, "copy mutates" , "the initCopy function / copy initializer takes its argument by ref")
symbolFlag( FLAG_DATA_CLASS , ypr, "data class" , ncm )

// Flag for temporaries created for default values
symbolFlag( FLAG_DEFAULT_ACTUAL, npr, "default actual temp", ncm)
symbolFlag( FLAG_DEFAULT_ACTUAL_FUNCTION, npr, "default actual function", "applied to functions created for formal default values")

// Enable override for default-intent for types defined in terms of record/class
symbolFlag( FLAG_DEFAULT_INTENT_IS_REF, ypr, "default intent is ref", "The default intent for this type is ref")
// Default intent should be determined by function body
// (ie. it is a ref if it is modified in the function body)
symbolFlag( FLAG_DEFAULT_INTENT_IS_REF_MAYBE_CONST, ypr, "default intent is ref if modified", "The default intent for this type is ref if modified const ref otherwise")

symbolFlag( FLAG_COPY_INIT, npr, "copy initializer", ncm )
symbolFlag( FLAG_DESTRUCTOR , npr, "destructor" , "applied to functions that are destructors" )
symbolFlag( FLAG_DISTRIBUTION , ypr, "distribution" , ncm )
symbolFlag( FLAG_DOMAIN , ypr, "domain" , ncm )
symbolFlag( FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING , ypr, "dont disable remote value forwarding" , ncm )
symbolFlag( FLAG_DOWN_END_COUNT_FN , ypr, "down end count fn" , ncm )
symbolFlag( FLAG_END_COUNT , ypr, "end count" , ncm )
symbolFlag( FLAG_ERRONEOUS_AUTOCOPY, ypr, "erroneous autocopy", ncm)
symbolFlag( FLAG_ERRONEOUS_INITCOPY, ypr, "erroneous initcopy", ncm)
symbolFlag( FLAG_ERROR_MODE_FATAL, ypr, "error mode fatal", ncm)
symbolFlag( FLAG_ERROR_MODE_RELAXED, ypr, "error mode relaxed", ncm)
symbolFlag( FLAG_ERROR_MODE_STRICT, ypr, "error mode strict", ncm)
symbolFlag( FLAG_ERROR_VARIABLE, npr, "error variable", ncm)
symbolFlag( FLAG_EPILOGUE_LABEL , npr, "epilogue label" , "distinguishes the epilogue label from other labels" )
symbolFlag( FLAG_ERROR_LABEL, ypr, "error label", ncm)
symbolFlag( FLAG_EXPANDED_VARARGS, npr, "expanded varargs", ncm)
symbolFlag( FLAG_EXPAND_TUPLES_WITH_VALUES , ypr, "expand tuples with values" , ncm )
symbolFlag( FLAG_EXPORT , npr, "export" , ncm )
symbolFlag( FLAG_EXPORT_INIT, ypr, "export init", "indicate that the module's initialization function should be exported" )
symbolFlag( FLAG_EXPORT_WRAPPER, ypr, "export wrapper", ncm )
symbolFlag( FLAG_EXPR_TEMP , npr, "expr temp" , "temporary that stores the result of an expression" )
symbolFlag( FLAG_EXTERN , npr, "extern" , "extern variables, types, and functions" )
symbolFlag( FLAG_EXTERN_FN_WITH_ARRAY_ARG, "npr", "extern fn with array arg", "extern functions with array arguments" )
symbolFlag( FLAG_FAST_ON , npr, "fast on" , "with FLAG_ON/FLAG_ON_BLOCK, \"on block\" , use fast spawning option (if available)" )
symbolFlag( FLAG_FAST_ON_SAFE_EXTERN, ypr, "fast-on safe extern function", "extern function is safe for fast-on optimization")
symbolFlag( FLAG_FIELD_ACCESSOR , npr, "field accessor" , "field setter/getter function, user-declared or compiler-generated" )
symbolFlag( FLAG_FIRST_CLASS_FUNCTION_INVOCATION, npr, "first class function invocation" , "proxy for first-class function invocation" )
symbolFlag( FLAG_FN_RETARG, npr, "fn returns via _retArg", ncm )
symbolFlag( FLAG_FOLLOWER_INDEX, npr, "follower index", "a variable representing a follower loop index" )
symbolFlag( FLAG_FORMAL_TEMP,     npr, "formal temp", "a formal temp to back an in, out, or inout argument" )
symbolFlag( FLAG_FORWARDING_FN , npr, "forwarding function" , ncm )
symbolFlag( FLAG_FUNCTION_CLASS , npr, "function class" , "first-class function class representation" )
symbolFlag( FLAG_FUNCTION_TERMINATES_PROGRAM, ypr, "function terminates program", "function that causes the program to exit" )
symbolFlag( FLAG_GENERATE_SIGNATURE, ypr, "generate signature", "compiler should codegen a function signature" )
// When applied to an argument, this flag means that the arg accepts a value
// but has unspecified type.
symbolFlag( FLAG_GENERIC , npr, "generic" , "generic types, functions and arguments" )
symbolFlag( FLAG_DELAY_GENERIC_EXPANSION, npr, "delay instantiation", "generics instances whose instantiation  will be determined shortly")
symbolFlag( FLAG_GEN_MAIN_FUNC, npr, "generated main", "compiler generated main function")

symbolFlag( FLAG_GET_LINE_NUMBER, ypr, "get line number", "replace calls to this function with the line number of the call" )
symbolFlag( FLAG_GET_FILE_NAME, ypr, "get file name", "replace calls to this function with the name of the file the call is in" )
symbolFlag( FLAG_GET_FUNCTION_NAME, ypr, "get function name", "replace calls to this function with the name of the function that called it" )
symbolFlag( FLAG_GET_MODULE_NAME, ypr, "get module name", "replace calls to this function with the name of the module the call was in" )

symbolFlag( FLAG_GLOBAL_TYPE_SYMBOL, ypr, "global type symbol", "is accessible through a global type variable")
symbolFlag( FLAG_HAS_RUNTIME_TYPE , ypr, "has runtime type" , "type that has an associated runtime type" )
symbolFlag( FLAG_RVV, npr, "RVV", "variable is the return value variable" )
symbolFlag( FLAG_YVV, npr, "YVV", "variable is a yield value variable" )
symbolFlag( FLAG_HEAP , npr, "heap" , ncm )
symbolFlag( FLAG_IF_EXPR_RESULT, npr, "if-expr result", ncm )
symbolFlag( FLAG_IMPLICIT_ALIAS_FIELD , npr, "implicit alias field" , ncm )
symbolFlag( FLAG_IMPLICIT_MODULE, npr, "implicit top-level module", ncm )
symbolFlag( FLAG_INDEX_VAR , npr, "index var" , ncm )

// This can also mark a temp that serves as an intermediate step of
// destructuring a tuple-typed FLAG_INDEX_OF_INTEREST variable
// into loop index variables.
symbolFlag( FLAG_INDEX_OF_INTEREST , npr, "an _indexOfInterest or chpl__followIdx variable" , ncm )
symbolFlag( FLAG_INIT_COPY_FN,  ypr, "init copy fn" , "init copy function" )
symbolFlag( FLAG_INITIALIZED_LATER,  ypr, "initialized later" , "initialized in subsequent statement" )
symbolFlag( FLAG_INIT_TUPLE, ypr, "tuple init fn", ncm)
symbolFlag( FLAG_INLINE , npr, "inline" , ncm )
symbolFlag( FLAG_INLINED_FN , npr, "inlined function" , ncm )
symbolFlag( FLAG_INLINE_ITERATOR , npr, "inline iterator" , "iterators that are always inlined, e.g., leaders" )
symbolFlag( FLAG_INSERT_AUTO_DESTROY , ypr, "insert auto destroy" , ncm )
symbolFlag( FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW , npr, "insert auto destroy for explicit new" , ncm )
symbolFlag( FLAG_INSERT_LINE_FILE_INFO , ypr, "insert line file info" , ncm )
symbolFlag( FLAG_INSTANTIATED_PARAM , npr, "instantiated param" , "this formal is bound to a param" )
symbolFlag( FLAG_INSTANTIATED_GENERIC , npr, "instantiated generic" , "this is an instantiation of a generic" )
symbolFlag( FLAG_INSTANTIATED_FROM_ANY , npr, "instantiated from any" , "this is an instantiation from any type" )
symbolFlag( FLAG_INVISIBLE_FN , npr, "invisible fn" , "invisible function (not a candidate for resolution)" )
symbolFlag( FLAG_ITERATOR_CLASS , npr, "iterator class" , ncm )
symbolFlag( FLAG_ITERATOR_FN , npr, "iterator fn" , ncm )
symbolFlag( FLAG_ITERATOR_RECORD , npr, "iterator record" , ncm )
symbolFlag( FLAG_ITERATOR_WITH_ON , npr, "iterator with on" , "iterator which contains an on block" )
// In resolution, functions marked as last-resort are considered only if
// no functions without that flag are found. This usually is used to create
// a pattern enabling user-supplied replacement of default behavior.
symbolFlag( FLAG_LAST_RESORT , ypr, "last resort" , "overload of last resort in resolution" )

// These flags help to decorate module code to enable nil-checking

// indicates that the function, on return, will leave 'this' argument
// storing 'nil'
symbolFlag( FLAG_LEAVES_THIS_NIL, ypr, "leaves this nil", ncm )
// indicates that the function, on return, will leave the marked
// argument storing 'nil'
symbolFlag( FLAG_LEAVES_ARG_NIL, ypr, "leaves arg nil", ncm )
// indicates that nil-ness of the result should be based
// upon the nil-ness of the particular argument
symbolFlag( FLAG_NIL_FROM_ARG, ypr, "nil from arg", ncm )
// indicates that the nil-ness of the result should be based
// upon the nil-ness of 'this'
symbolFlag( FLAG_NIL_FROM_THIS, ypr, "nil from this", ncm )

// Tells resolution to use this function's line number even if that function
// has FLAG_COMPILER_GENERATED.
symbolFlag( FLAG_LINE_NUMBER_OK, ypr, "lineno ok", ncm )

symbolFlag( FLAG_LLVM_READNONE , ypr, "llvm readnone" , ncm )

symbolFlag( FLAG_LOCALE_MODEL_ALLOC , ypr, "locale model alloc" , "locale model specific alloc" )
symbolFlag( FLAG_LOCALE_MODEL_FREE , ypr, "locale model free" , "locale model specific free" )
symbolFlag( FLAG_INC_RUNNING_TASK , ypr, "inc running task" , "running task incrementer" )
symbolFlag( FLAG_DEC_RUNNING_TASK , ypr, "dec running task" , "running task decrementer" )
symbolFlag( FLAG_LOCALE_PRIVATE , ypr, "locale private" , ncm )

// The arguments to this function are all values or narrow pointers.
// Calls to an extern function use only narrow args and expect a narrow return.
// Implementation of an export function declares only narrow args and has a
// narrow return value.
symbolFlag( FLAG_LOCAL_ARGS , ypr, "local args" , "arguments and return value are narrow" )
symbolFlag( FLAG_LOCAL_FIELD, ypr, "local field", "field is local to aggregate type" )
symbolFlag( FLAG_LOCAL_FN , npr, "local fn" , "function is completely local (no communication)" )
symbolFlag( FLAG_LOCAL_ON, npr, "local on", ncm)
symbolFlag( FLAG_LOOP_BODY_ARGUMENT_CLASS , npr, "loop body argument class" , ncm )
symbolFlag( FLAG_MANAGED_POINTER , ypr, "managed pointer" , "e.g. Owned and Shared" )
symbolFlag( FLAG_MANAGED_POINTER_NONNILABLE , npr, "managed pointer nonnilable" , "e.g. non-nilable Owned and Shared" )
symbolFlag( FLAG_MARKED_GENERIC , npr, "marked generic" , "formal is marked generic using the type query syntax" )
symbolFlag( FLAG_MAYBE_ARRAY_TYPE , npr, "maybe array type" , "function may be computing array type")
symbolFlag( FLAG_MAYBE_PARAM , npr, "maybe param" , "symbol can resolve to a param" )
symbolFlag( FLAG_MAYBE_REF , npr, "maybe ref" , "symbol can resolve to a ref" )
symbolFlag( FLAG_MAYBE_TYPE , npr, "maybe type" , "symbol can resolve to a type" )
symbolFlag( FLAG_MEMORY_ORDER_TYPE , ypr, "memory order type" , "type implementing chpl memory order (normally called memoryOrder)" )
symbolFlag( FLAG_C_MEMORY_ORDER_TYPE , ypr, "c memory order type" , "type implementing c memory order (normally called memory_order)" )
symbolFlag( FLAG_METHOD , npr, "method" , "function that is a method" )
symbolFlag( FLAG_METHOD_PRIMARY , npr, "primary method" , "function that is a method and defined in type declaration" )
symbolFlag( FLAG_MODIFIES_CONST_FIELDS , npr, "modifies const fields" , "... of 'this' argument" )
symbolFlag( FLAG_MODULE_FROM_COMMAND_LINE_FILE, npr, "module from command line file", "This is a module that came from a file named on the compiler command line")
symbolFlag( FLAG_MODULE_INIT , npr, "module init" , "a module init function" )
// This flag marks the result of an autoCopy as necessary.
// Necessary autoCopies are not removed by the removeUnnecessaryAutoCopyCalls optimization.
symbolFlag( FLAG_NECESSARY_AUTO_COPY, npr, "necessary auto copy", "a variable containing a necessary autoCopy" )
symbolFlag( FLAG_NEW_WRAPPER, npr, "_new wrapper", ncm)
symbolFlag( FLAG_IGNORE_NOINIT, ypr, "ignore noinit", "this type must be initialized" )
symbolFlag( FLAG_NON_BLOCKING , npr, "non blocking" , "with FLAG_ON/FLAG_ON_BLOCK, non-blocking on functions" )
symbolFlag( FLAG_NO_AUTO_DESTROY , ypr, "no auto destroy" , ncm )
symbolFlag( FLAG_NO_FN_BODY , npr, "no fn body", "function has no body" )
symbolFlag( FLAG_NO_BORROW_CONVERT , ypr, "no borrow convert", "arguments that are instantiated as owned/shared/etc do not need to convert to borrow" )
symbolFlag( FLAG_NO_CAPTURE_FOR_TASKING , npr, "no capture for tasking", "does not need to be captured before spawning tasks" )
symbolFlag( FLAG_NO_CODEGEN , ypr, "no codegen" , "do not generate e.g. C code defining this symbol" )
symbolFlag( FLAG_NO_COPY , ypr, "no copy" , "do not apply chpl__initCopy to initialization of a variable" )
symbolFlag( FLAG_NO_COPY_RETURN, ypr, "no copy return", ncm)
symbolFlag( FLAG_NO_DEFAULT_FUNCTIONS , ypr, "no default functions" , ncm )
symbolFlag( FLAG_NO_DOC, ypr, "no doc", "do not generate chpldoc documentation for this symbol" )
symbolFlag( FLAG_NO_IMPLICIT_COPY , ypr, "no implicit copy" , "function does not require autoCopy/autoDestroy" )
symbolFlag( FLAG_NO_INIT , ypr, "no init", "Do not initialize this variable" )
symbolFlag( FLAG_NO_OBJECT , ypr, "no object" , ncm )
symbolFlag( FLAG_NO_PARENS , npr, "no parens" , "function without parentheses" )
symbolFlag( FLAG_NO_REMOTE_MEMORY_FENCE , ypr, "no remote memory fence" , ncm)
symbolFlag( FLAG_NO_RENAME, npr, "no rename", ncm)
symbolFlag( FLAG_NO_RVF, npr, "do not RVF", ncm)
symbolFlag( FLAG_NO_WIDE_CLASS , ypr, "no wide class" , ncm )

// See FLAG_POD below
symbolFlag( FLAG_NOT_POD , ypr, "not plain old data" , "bit copy overridden")

symbolFlag( FLAG_NOT_FULLY_GENERIC , npr, "argument is not fully generic" , ncm)

symbolFlag( FLAG_OBJECT_CLASS , ypr, "object class" , ncm )

// FLAG_ON and FLAG_ON_BLOCK mark task functions and their wrappers,
// respectively, that perform remote operations, i.e. corresponding to
// 'on' statements. Some task functions with FLAG_ON also create new
// Chapel tasks - as a result of the "begin+on" optimization. They
// have the following flags:
//  begin+on       FLAG_ON  FLAG_NON_BLOCKING  FLAG_BEGIN
//  on+begin       FLAG_ON  FLAG_NON_BLOCKING  FLAG_BEGIN
//  cobegin+on     FLAG_ON  FLAG_NON_BLOCKING  FLAG_COBEGIN_OR_COFORALL
//  coforall+on    FLAG_ON  FLAG_NON_BLOCKING  FLAG_COBEGIN_OR_COFORALL
//  just 'on'      FLAG_ON  // no new Chapel tasks
// For each of the above flags, the task function's wrapper has
// the corresponding flag:
//   FLAG_ON                  --> FLAG_ON_BLOCK
//   FLAG_NON_BLOCKING        --> FLAG_NON_BLOCKING (the same flag;
//     btw it does not apply to local (non-'on') task functions/wrappers)
//   FLAG_BEGIN               --> FLAG_BEGIN_BLOCK
//   FLAG_COBEGIN_OR_COFORALL --> FLAG_COBEGIN_OR_COFORALL_BLOCK
//
symbolFlag( FLAG_ON , npr, "on" , ncm )
symbolFlag( FLAG_ON_BLOCK , npr, "on block" , ncm )

symbolFlag( FLAG_OVERRIDE , npr, "method overrides" , ncm )

// variables added by flatten functions
symbolFlag( FLAG_OUTER_VARIABLE , npr, "outer variable" , ncm )

symbolFlag( FLAG_OWNED , ypr, "owned", "owned class instance for lifetime checking" )

symbolFlag( FLAG_PARAM , npr, "param" , "parameter (compile-time constant)" )

symbolFlag( FLAG_PARENT_FIELD , npr, "parent field" , "field from parent type" )

symbolFlag( FLAG_PARTIAL_TUPLE, npr, "partial tuple", ncm)

// Is this type a Plain-Old Data (POD) type - ie no autocopy/destructor/=
// need ever be called - bit copies will do.
// Each aggregate type gets either FLAG_POD or FLAG_NOT_POD during resolution.
// To check POD-ness of an aggregate type:
//  * during resolution: use propagateNotPOD()
//  * after resolution: use isPOD()
// To check POD-ness of an arbitrary type after resolution:
//  * use isPOD()
symbolFlag( FLAG_POD , ypr, "plain old data" , "data can be bit copied")


symbolFlag( FLAG_PRIMITIVE_TYPE , ypr, "primitive type" , "attached to primitive types to keep them from being deleted" )
symbolFlag( FLAG_PRINT_MODULE_INIT_FN , ypr, "print module init fn" , ncm )
symbolFlag( FLAG_PRINT_MODULE_INIT_INDENT_LEVEL , ypr, "print module init indent level" , ncm )
symbolFlag( FLAG_PRIVATE, npr, "private", ncm )
symbolFlag( FLAG_PROMOTION_WRAPPER , npr, "promotion wrapper" , ncm )
symbolFlag( FLAG_PROTOTYPE_MODULE , npr, "prototype module" , ncm )
symbolFlag( FLAG_RANGE , ypr, "range" , "indicates the range type" )
symbolFlag( FLAG_RECURSIVE_ITERATOR , npr, "recursive iterator" , "iterators which call themselves" )
symbolFlag( FLAG_REDUCESCANOP , ypr, "ReduceScanOp" , "the ReduceScanOp class" )
symbolFlag( FLAG_REF , ypr, "ref" , ncm )
symbolFlag( FLAG_REF_FOR_CONST_FIELD_OF_THIS , npr, "reference to a const field of 'this'" , ncm )
symbolFlag( FLAG_REF_ITERATOR_CLASS , npr, "ref iterator class" , ncm )
// "ref to const" is like Chapel's 'const ref' variable:
// * it is illegal to modify the referenced thing through this variable
// * the referenced thing may change, observably through this variable
symbolFlag( FLAG_REF_TO_CONST , npr, "reference to a const" , "a temp or a function that returns a reference to a Chapel const, e.g. an accessor to a const field or its result" )
symbolFlag( FLAG_REF_TO_CONST_WHEN_CONST_THIS , ypr, "reference to const when const this" , "a function that returns a reference to a Chapel const when 'this' is const" )
symbolFlag( FLAG_REF_TO_IMMUTABLE , npr, "ref to immutable" , "a reference to something that never changes during its lifetime")
symbolFlag( FLAG_REF_VAR , ypr, "ref var" , "reference variable" )
symbolFlag( FLAG_REF_TEMP , npr, "ref temp" , "compiler-inserted reference temporary" )
symbolFlag( FLAG_REMOVABLE_ARRAY_ACCESS, ypr, "removable array access", "array access calls that can be replaced with a reference")
symbolFlag( FLAG_REMOVABLE_AUTO_COPY , ypr, "removable auto copy" , ncm )
symbolFlag( FLAG_REMOVABLE_AUTO_DESTROY , ypr, "removable auto destroy" , ncm )
symbolFlag( FLAG_COMPILER_ADDED_REMOTE_FENCE , ypr, "compiler added remote fence" , ncm )
symbolFlag( FLAG_RESOLVED , npr, "resolved" , "this function has been resolved" )
symbolFlag( FLAG_RETARG, npr, "symbol is a _retArg", ncm )
symbolFlag( FLAG_RETURNS_ALIASING_ARRAY, ypr, "fn returns aliasing array", "array alias/slice/reindex/rank change function" )
symbolFlag( FLAG_FN_RETURNS_ITERATOR, ypr, "fn returns iterator", "proc that can return an iterator instead of promoting it to an array")
symbolFlag( FLAG_FN_UNORDERED_SAFE, ypr, "fn unordered safe", "function does not inhibit unordered optimization")
symbolFlag( FLAG_FN_SYNCHRONIZATION_FREE, ypr, "fn synchronization free", "function does not include any task synchronization")
symbolFlag( FLAG_RETURNS_INFINITE_LIFETIME, ypr, "fn returns infinite lifetime", "function returns a pointer with infinite lifetime for lifetime analysis" )
symbolFlag( FLAG_RETURN_SCOPE, npr, "return scope", "indicates an argument that can be returned without error in lifetime checking")
symbolFlag( FLAG_RETURN_NOT_OWNED, ypr, "return not owned", "scope for return value should ignore owned fields")
// See buildRuntimeTypeToValueFns() in functionResolution.cpp for more info on FLAG_RUNTIME_TYPE_INIT_FN
symbolFlag( FLAG_RUNTIME_TYPE_INIT_FN , ypr, "runtime type init fn" , "function for initializing runtime time types" )
symbolFlag( FLAG_RUNTIME_TYPE_VALUE , npr, "runtime type value" , "associated runtime type (value)" )
symbolFlag( FLAG_SAFE, ypr, "safe", "safe (activate lifetime checking)")
symbolFlag( FLAG_SCOPE, npr, "scope", "scoped (lifetime checking like a local variable)")
symbolFlag( FLAG_SHOULD_NOT_PASS_BY_REF, npr, "should not pass by ref", "this symbol should be passed by value (not by reference) for performance, not for correctness")
symbolFlag( FLAG_SINGLE , ypr, "single" , ncm )
// Based on how this is used, I suggest renaming it to return_value_has_initializer
// or something similar <hilde>.
symbolFlag( FLAG_STAR_TUPLE , ypr, "star tuple" , "mark tuple types as star tuple types" )
symbolFlag( FLAG_STAR_TUPLE_ACCESSOR , ypr, "star tuple accessor" , "this function for star tuple types" )
symbolFlag( FLAG_SUPER_CLASS , npr, "super class" , ncm )
symbolFlag( FLAG_SUPER_TEMP, npr, "temporary of super field", ncm)
symbolFlag( FLAG_SUPPRESS_LVALUE_ERRORS , ypr, "suppress lvalue error" , "do not report an lvalue error if it occurs in a function with this flag" )
symbolFlag( FLAG_SYNC , ypr, "sync" , ncm )
symbolFlag( FLAG_SYNTACTIC_DISTRIBUTION , ypr, "syntactic distribution" , ncm )
symbolFlag( FLAG_TASK_FN_FROM_ITERATOR_FN , npr, "task fn from iterator fn" , ncm )
symbolFlag( FLAG_TASK_SPAWN_IMPL_FN , ypr, "task spawn impl fn" , ncm )
symbolFlag( FLAG_TASK_COMPLETE_IMPL_FN , ypr, "task complete impl fn" , ncm )
symbolFlag( FLAG_TASK_JOIN_IMPL_FN , ypr, "task join impl fn" , ncm )
symbolFlag( FLAG_TEMP , npr, "temp" , "compiler-inserted temporary" )
symbolFlag( FLAG_TUPLE , ypr, "tuple" , ncm )
symbolFlag( FLAG_TUPLE_CAST_FN , ypr, "tuple cast fn" , ncm )
symbolFlag( FLAG_TUPLE_WITH_REF , npr, "tuple contains ref" , ncm )
symbolFlag( FLAG_TYPE_VARIABLE , npr, "type variable" , "contains a type instead of a value" )
symbolFlag( FLAG_UNALIAS_FN,  ypr, "unalias fn" , "function to copy array slices when assigning to a user variable")
symbolFlag( FLAG_UNCHECKED_THROWS,  ypr, "unchecked throws" , "function throws but handling the errors is not required even in strict mode")
symbolFlag( FLAG_UNREF_FN,  ypr, "unref fn" , "function to remove reference fields from tuples or copy array slices when returning")
symbolFlag( FLAG_UNSAFE,  ypr, "unsafe" , "unsafe (disable lifetime and nilability checking)")
symbolFlag( FLAG_USER_VARIABLE_NAME,  npr, "user variable name" , "temporary uses name of user variable")
symbolFlag( FLAG_VECTORIZE_YIELDING_LOOPS, ypr, "vectorize yielding loops", "used to explicitly vectorize yielding loops in iterators" )
symbolFlag( FLAG_VIRTUAL , npr, "virtual" , ncm )
symbolFlag( FLAG_VOID_NO_RETURN_VALUE, npr, "no return value for void", "function does not return a value if the return type is void" )
// Used to mark where a compiler generated flag was removed (but is desired
// elsewhere).
symbolFlag( FLAG_WAS_COMPILER_GENERATED, npr, "was compiler generated", "used to be marked compiler generated")
symbolFlag( FLAG_WIDE_REF , npr, "wide" , ncm )
symbolFlag( FLAG_WIDE_CLASS , npr, "wide class" , ncm )
symbolFlag( FLAG_WRAPPER , npr, "wrapper" , "wrapper function" )
symbolFlag( FLAG_WRAPPER_NEEDS_START_FENCE , npr, "wrapper needs start fence" , "add PRIM_START_RMEM_FENCE to the start of the wrapper function" )
symbolFlag( FLAG_WRAPPER_NEEDS_FINISH_FENCE , npr, "wrapper needs finish fence" , "add PRIM_FINISH_RMEM_FENCE to the end of the wrapper function" )
symbolFlag( FLAG_WRAP_WRITTEN_FORMAL , npr, "wrap written formal" , "formal argument for wrapper for out/inout intent" )
symbolFlag( FLAG_YIELD_WITHIN_ON, npr, "yield within on", "iterator that yields within an on" )


// These flags are for use with PRIM_OPTIMIZATION_INFO
symbolFlag( OPT_INFO_LHS_OUTLIVES_FORALL , npr, "lhs outlives forall" , "lhs or destination outlives forall" )
symbolFlag( OPT_INFO_RHS_OUTLIVES_FORALL , npr, "rhs outlives forall" , "rhs or sourceoutlives forall" )
symbolFlag( OPT_INFO_FLAG_NO_TASK_PRIVATE , npr, "forall no task private" , "forall does not use task private storage" )
symbolFlag( OPT_INFO_FLAG_NO_BLOCKING , npr, "forall not blocking" , "forall does not have blocking synchronization" )

#undef ypr
#undef npr
#undef ncm
