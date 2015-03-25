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


// This flag is used in scalarReplace.cpp to determine if an assignment of a ref
// has an allocator as the RHS.  If so, then it is not creating an alias, since
// the allocator function does not retain a reference to the referenced object.
symbolFlag( FLAG_ALLOCATOR , ypr, "allocator" , "allocates heap storage" )
symbolFlag( FLAG_ALLOW_REF , ypr, "allow ref" , ncm )
symbolFlag( FLAG_ARG_THIS, npr, "arg this", "the hidden object argument")
symbolFlag( FLAG_ARRAY , ypr, "array" , ncm )
symbolFlag( FLAG_ARRAY_ALIAS , npr, "array alias" , "array alias declared via => syntax" )
symbolFlag( FLAG_ASSIGNOP, npr, "assignop", "this fn is assignment or an <op>= operator" )
symbolFlag( FLAG_ATOMIC_MODULE , ypr, "atomic module" , "module containing implementations of atomic types" )
symbolFlag( FLAG_ATOMIC_TYPE , ypr, "atomic type" , "type that implements an atomic" )
symbolFlag( FLAG_AUTO_COPY_FN,  ypr, "auto copy fn" , "auto copy function" )
symbolFlag( FLAG_AUTO_DESTROY_FN,  ypr, "auto destroy fn" , "auto destroy function" )
symbolFlag( FLAG_AUTO_DESTROY_FN_SYNC, ypr, "auto destroy fn sync", "auto destroy function for sync/single" )
symbolFlag( FLAG_AUTO_II , npr, "auto ii" , ncm )
symbolFlag( FLAG_BASE_ARRAY , ypr, "base array" , ncm )
symbolFlag( FLAG_BASE_DOMAIN , ypr, "base domain" , ncm )
symbolFlag( FLAG_BASE_DIST , ypr, "base dist" , ncm )
symbolFlag( FLAG_BEGIN , npr, "begin" , ncm )
symbolFlag( FLAG_BEGIN_BLOCK , npr, "begin block" , ncm )
symbolFlag( FLAG_BUILD_TUPLE , ypr, "build tuple" , "used to mark the build_tuple functions")

// When resolution encounters the def of the variable 'chpl__iter',
// as indicated by this flag, it launches into enacting forall intents
// for the forall loop that this variable was created for.
symbolFlag( FLAG_CHPL__ITER , npr, "chpl__iter", "used as a marker to implement forall intents" )
symbolFlag( FLAG_COBEGIN_OR_COFORALL , npr, "cobegin or coforall" , ncm )
symbolFlag( FLAG_COBEGIN_OR_COFORALL_BLOCK , npr, "cobegin or coforall block" , ncm )
symbolFlag( FLAG_COERCE_TEMP , npr, "coerce temp" , "a temporary that was stores the result of a coercion" )
symbolFlag( FLAG_CODEGENNED , npr, "codegenned" , "code has been generated for this type" )
symbolFlag( FLAG_COFORALL_INDEX_VAR , npr, "coforall index var" , ncm )
symbolFlag( FLAG_COMMAND_LINE_SETTING , ypr, "command line setting" , ncm )
// The compiler-generated flag is already overloaded in three ways.  We may
// want to split it if this becomes cumbersome:
// 1. In resolution, functions marked as compiler-generated are considered only if
// no functions without that flag (i.e. user-supplied) functions are found.
// 2. In printing filename/lineno information in error messages (when developer
// == false), the callstack is searched ignoring compiler-generated functions.
// 3. Assignment operations flagged as 'compiler generated' shall contain only
// field assignments and assignment primitives.
symbolFlag( FLAG_COMPILER_GENERATED , ypr, "compiler generated" , "marks functions that are compiler-generated or supplied by an internal module" )
symbolFlag( FLAG_COMPILER_NESTED_FUNCTION , npr, "compiler nested function" , ncm )
symbolFlag( FLAG_CONCURRENTLY_ACCESSED , npr, "concurrently accessed" , "local variables accessed by multiple threads" )
symbolFlag( FLAG_CONFIG , npr, "config" , "config variable, constant, or parameter" )
symbolFlag( FLAG_CONST , npr, "const" , "constant" )
symbolFlag( FLAG_CONSTRUCTOR , npr, "constructor" , "constructor (but not type constructor); loosely defined to include constructor wrappers" )
symbolFlag( FLAG_DATA_CLASS , ypr, "data class" , ncm )
symbolFlag( FLAG_DEFAULT_CONSTRUCTOR , npr, "default constructor" , ncm )
symbolFlag( FLAG_DEFAULT_STRING_VALUE , ypr, "default string value" , "default value for a string, defined in the modules" )
symbolFlag( FLAG_DESTRUCTOR , npr, "destructor" , "applied to functions that are destructors" )
symbolFlag( FLAG_DISTRIBUTION , ypr, "distribution" , ncm )
symbolFlag( FLAG_DOMAIN , ypr, "domain" , ncm )
symbolFlag( FLAG_DONOR_FN, ypr, "donor fn" , "function donates ownership of the returned object to the calling function" )
symbolFlag( FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING , ypr, "dont disable remote value forwarding" , ncm )
symbolFlag( FLAG_EXPANDED_VARARGS, npr, "expanded varargs", ncm)
symbolFlag( FLAG_EXPAND_TUPLES_WITH_VALUES , ypr, "expand tuples with values" , ncm )
symbolFlag( FLAG_EXPORT , ypr, "export" , ncm )
symbolFlag( FLAG_EXPORT_INIT, ypr, "export init", "indicate that the module's initialization function should be exported" )
symbolFlag( FLAG_EXPR_TEMP , npr, "expr temp" , "temporary that stores the result of an expression" )
symbolFlag( FLAG_EXTERN , npr, "extern" , "extern variables, types, and functions" )
symbolFlag( FLAG_FAST_ON , npr, "fast on" , "with FLAG_ON/FLAG_ON_BLOCK, \"on block\" , use fast spawning option (if available)" )
symbolFlag( FLAG_FIELD_ACCESSOR , npr, "field accessor" , "field setter/getter function, user-declared or compiler-generated" )
symbolFlag( FLAG_FIRST_CLASS_FUNCTION_INVOCATION, npr, "first class function invocation" , "proxy for first-class function invocation" )
symbolFlag( FLAG_FUNCTION_CLASS , npr, "function class" , "first-class function class representation" )
symbolFlag( FLAG_FUNCTION_PROTOTYPE , npr, "function prototype" , "signature for function prototypes" )
// When applied to an argument, this flag means that the arg accepts a value
// but has unspecified type.
symbolFlag( FLAG_GENERIC , npr, "generic" , "generic types, functions and arguments" )
symbolFlag( FLAG_GLOBAL_TYPE_SYMBOL, npr, "global type symbol", "is accessible through a global type variable")
symbolFlag( FLAG_HAS_RUNTIME_TYPE , ypr, "has runtime type" , "type that has an associated runtime type" )

// If any of the following three flags is attached to a class (resp. record or
// union), then that type is not a Plain-Old Data (POD) type.
symbolFlag( FLAG_HAS_USER_ASSIGNMENT , npr, "has user assignment" , "applied to classes and records for which assignment is explicitly defined")
symbolFlag( FLAG_HAS_USER_DESTRUCTOR , npr, "has user destructor" , "applied to classes that define a destructor explicitly" )
symbolFlag( FLAG_HAS_USER_INIT_COPY_FN , npr, "has user init copy fn" , "applied to classes that define an init copy function explicitly" )
symbolFlag( FLAG_HEAP , npr, "heap" , ncm )
symbolFlag( FLAG_IMPLICIT_ALIAS_FIELD , npr, "implicit alias field" , ncm )
symbolFlag( FLAG_INDEX_VAR , npr, "index var" , ncm )

// This can also mark a temp that serves as an intermediate step of
// destructuring a tuple-typed FLAG_INDEX_OF_INTEREST variable
// into loop index variables.
symbolFlag( FLAG_INDEX_OF_INTEREST , npr, "an _indexOfInterest or chpl__followIdx variable" , ncm )
symbolFlag( FLAG_INIT_COPY_FN,  ypr, "init copy fn" , "init copy function" )
symbolFlag( FLAG_INLINE , npr, "inline" , ncm )
symbolFlag( FLAG_INLINE_ITERATOR , npr, "inline iterator" , "iterators that are always inlined, e.g., leaders" )
symbolFlag( FLAG_INSERT_AUTO_COPY , npr, "insert auto copy" , ncm )
symbolFlag( FLAG_INSERT_AUTO_DESTROY , ypr, "insert auto destroy" , ncm )
symbolFlag( FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW , npr, "insert auto destroy for explicit new" , ncm )
symbolFlag( FLAG_INSERT_LINE_FILE_INFO , ypr, "insert line file info" , ncm )
symbolFlag( FLAG_INSTANTIATED_PARAM , npr, "instantiated param" , "this formal is bound to a param" )
symbolFlag( FLAG_INVISIBLE_FN , npr, "invisible fn" , "invisible function (not a candidate for resolution)" )
symbolFlag( FLAG_IS_MEME , npr, "is meme" , ncm )
symbolFlag( FLAG_ITERATOR_CLASS , npr, "iterator class" , ncm )
symbolFlag( FLAG_ITERATOR_FN , npr, "iterator fn" , ncm )
symbolFlag( FLAG_ITERATOR_RECORD , npr, "iterator record" , ncm )
symbolFlag( FLAG_ITERATOR_WITH_ON , npr, "iterator with on" , "iterator which contains an on block" )
symbolFlag( FLAG_LOCALE_MODEL_ALLOC , ypr, "locale model alloc" , "locale model specific alloc" )
symbolFlag( FLAG_LOCALE_MODEL_FREE , ypr, "locale model free" , "locale model specific free" )

// The arguments to this function are all values or narrow pointers.
// Calls to an extern function use only narrow args and expect a narrow return.
// Implementation of an export function declares only narrow args and has a
// narrow return value.
symbolFlag( FLAG_LOCAL_ARGS , ypr, "local args" , "arguments and return value are narrow" )
symbolFlag( FLAG_LOCAL_FIELD, ypr, "local field", "field is local to aggregate type" )
symbolFlag( FLAG_LOCAL_FN , npr, "local fn" , "function is completely local (no communication)" )
symbolFlag( FLAG_LOOP_BODY_ARGUMENT_CLASS , npr, "loop body argument class" , ncm )
symbolFlag( FLAG_MARKED_GENERIC , npr, "marked generic" , "formal is marked generic using the type query syntax" )
symbolFlag( FLAG_MAYBE_PARAM , npr, "maybe param" , "symbol can resolve to a param" )
symbolFlag( FLAG_MAYBE_TYPE , npr, "maybe type" , "symbol can resolve to a type" )
symbolFlag( FLAG_MEMORY_ORDER_TYPE , ypr, "memory order type" , "type implementing memory order (normally called memory_order)" )
symbolFlag( FLAG_METHOD , npr, "method" , "function that is a method" )
symbolFlag( FLAG_METHOD_PRIMARY , npr, "primary method" , "function that is a method and defined in type declaration" )
symbolFlag( FLAG_MODIFIES_CONST_FIELDS , npr, "modifies const fields" , "... of 'this' argument" )
symbolFlag( FLAG_MODULE_FROM_COMMAND_LINE_FILE, npr, "module from command line file", "This is a module that came from a file named on the compiler command line")
symbolFlag( FLAG_MODULE_INIT , npr, "module init" , "a module init function" )
// This flag marks the result of an autoCopy as necessary.
// Necessary autoCopies are not removed by the removeUnnecessaryAutoCopyCalls optimization.
symbolFlag( FLAG_NECESSARY_AUTO_COPY, npr, "necessary auto copy", "a variable containing a necessary autoCopy" )
symbolFlag( FLAG_IGNORE_NOINIT, ypr, "ignore noinit", "this type must be initialized" )
symbolFlag( FLAG_NON_BLOCKING , npr, "non blocking" , "with FLAG_ON/FLAG_ON_BLOCK, non-blocking on functions" )
symbolFlag( FLAG_NO_AUTO_DESTROY , ypr, "no auto destroy" , ncm )
symbolFlag( FLAG_NO_CODEGEN , ypr, "no codegen" , "do not generate e.g. C code defining this symbol" )
symbolFlag( FLAG_NO_COPY , ypr, "no copy" , "do not apply chpl__initCopy to initialization of a variable" )
symbolFlag( FLAG_NO_DEFAULT_FUNCTIONS , ypr, "no default functions" , ncm )
symbolFlag( FLAG_NO_DOC, ypr, "no doc", "do not generate chpldoc documentation for this symbol" )
symbolFlag( FLAG_NO_IMPLICIT_COPY , ypr, "no implicit copy" , "function does not require autoCopy/autoDestroy" )
symbolFlag( FLAG_NO_INSTANTIATION_LIMIT , ypr, "no instantiation limit", "The instantiation limit is not checked for this function" )
symbolFlag( FLAG_NO_OBJECT , ypr, "no object" , ncm )
symbolFlag( FLAG_NO_PARENS , npr, "no parens" , "function without parentheses" )
symbolFlag( FLAG_NO_PROTOTYPE , ypr, "no prototype" , "do not generate a prototype this symbol" )
symbolFlag( FLAG_NO_USE_CHAPELSTANDARD , ypr, "no use ChapelStandard" , "Do not implicitly use ChapelStandard" )
symbolFlag( FLAG_NO_WIDE_CLASS , ypr, "no wide class" , ncm )
symbolFlag( FLAG_NO_REMOTE_MEMORY_FENCE , ypr, "no remote memory fence" , ncm)
symbolFlag( FLAG_OBJECT_CLASS , npr, "object class" , ncm )
symbolFlag( FLAG_OMIT_FROM_CONSTRUCTOR , ypr, "omit from constructor" , ncm )

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
symbolFlag( FLAG_PARAM , npr, "param" , "parameter (compile-time constant)" )

symbolFlag( FLAG_PARTIAL_COPY, npr, "partial copy", ncm )
symbolFlag( FLAG_PARTIAL_TUPLE, npr, "partial tuple", ncm)

symbolFlag( FLAG_PRIMITIVE_TYPE , ypr, "primitive type" , "attached to primitive types to keep them from being deleted" )
symbolFlag( FLAG_PRINT_MODULE_INIT_FN , ypr, "print module init fn" , ncm )
symbolFlag( FLAG_PRINT_MODULE_INIT_INDENT_LEVEL , ypr, "print module init indent level" , ncm )
symbolFlag( FLAG_PRIVATE , ypr, "private" , ncm )
symbolFlag( FLAG_PRIVATIZED_CLASS , ypr, "privatized class" , "privatized array or domain class" )
symbolFlag( FLAG_PROMOTION_WRAPPER , npr, "promotion wrapper" , ncm )
symbolFlag( FLAG_RANGE , ypr, "range" , "indicates that this type can be iterated" )
symbolFlag( FLAG_RECURSIVE_ITERATOR , npr, "recursive iterator" , "iterators which call themselves" )
symbolFlag( FLAG_REDUCESCANOP , ypr, "ReduceScanOp" , "the ReduceScanOp class" )
symbolFlag( FLAG_REF , ypr, "ref" , ncm )
symbolFlag( FLAG_REF_FOR_CONST_FIELD_OF_THIS , npr, "reference to a const field of 'this'" , ncm )
symbolFlag( FLAG_REF_ITERATOR_CLASS , npr, "ref iterator class" , ncm )
symbolFlag( FLAG_REF_TO_CONST , npr, "reference to a const" , "a temp or a function that returns a reference to a Chapel const, e.g. an accessor to a const field or its result" )
symbolFlag( FLAG_REF_TO_CONST_WHEN_CONST_THIS , ypr, "reference to const when const this" , "a function that returns a reference to a Chapel const when 'this' is const" )
symbolFlag( FLAG_REF_VAR , ypr, "ref var" , "reference variable" )
symbolFlag( FLAG_REMOVABLE_AUTO_COPY , ypr, "removable auto copy" , ncm )
symbolFlag( FLAG_REMOVABLE_AUTO_DESTROY , ypr, "removable auto destroy" , ncm )
symbolFlag( FLAG_RESOLVED , npr, "resolved" , "this function has been resolved" )
// See buildRuntimeTypeToValueFns() in functionResolution.cpp for more info on FLAG_RUNTIME_TYPE_INIT_FN
symbolFlag( FLAG_RUNTIME_TYPE_INIT_FN , ypr, "runtime type init fn" , "function for initializing runtime time types" )
symbolFlag( FLAG_RUNTIME_TYPE_VALUE , npr, "runtime type value" , "associated runtime type (value)" )
symbolFlag( FLAG_SHOULD_NOT_PASS_BY_REF, npr, "should not pass by ref", "this symbol should be passed by value (not by reference) for performance, not for correctness")
symbolFlag( FLAG_SINGLE , ypr, "single" , ncm )
// Based on how this is used, I suggest renaming it to return_value_has_initializer
// or something similar <hilde>.
symbolFlag( FLAG_SPECIFIED_RETURN_TYPE , npr, "specified return type" , ncm )
symbolFlag( FLAG_STAR_TUPLE , npr, "star tuple" , "mark tuple types as star tuple types" )
symbolFlag( FLAG_SUPER_CLASS , npr, "super class" , ncm )
symbolFlag( FLAG_SUPPRESS_LVALUE_ERRORS , ypr, "suppress lvalue error" , "do not report an lvalue error if it occurs in a function with this flag" )
symbolFlag( FLAG_SYNC , ypr, "sync" , ncm )
symbolFlag( FLAG_SYNTACTIC_DISTRIBUTION , ypr, "syntactic distribution" , ncm )
symbolFlag( FLAG_TEMP , npr, "temp" , "compiler-inserted temporary" )
symbolFlag( FLAG_REF_TEMP , npr, "ref temp" , "compiler-inserted reference temporary" )
symbolFlag( FLAG_TRIVIAL_ASSIGNMENT, ypr, "trivial assignment", "an assignment which may be replaced by a bulk copy without changing its semantics")
symbolFlag( FLAG_TUPLE , ypr, "tuple" , ncm )
symbolFlag( FLAG_TYPE_CONSTRUCTOR , npr, "type constructor" , ncm )
symbolFlag( FLAG_TYPE_VARIABLE , npr, "type variable" , "contains a type instead of a value" )
symbolFlag( FLAG_VIRTUAL , npr, "virtual" , ncm )
// Used to mark where a compiler generated flag was removed (but is desired
// elsewhere).
symbolFlag( FLAG_WAS_COMPILER_GENERATED, npr, "was compiler generated", "used to be marked compiler generated")
symbolFlag( FLAG_WIDE_REF , npr, "wide" , ncm )
symbolFlag( FLAG_WIDE_CLASS , npr, "wide class" , ncm )
symbolFlag( FLAG_WRAPPER , npr, "wrapper" , "wrapper function" )
symbolFlag( FLAG_WRAP_WRITTEN_FORMAL , npr, "wrap written formal" , "formal argument for wrapper for out/inout intent" )

#undef ypr
#undef npr
#undef ncm
