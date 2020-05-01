Release Changes List
====================

version 1.22.0
==============

Twenty-fifth public release of Chapel, April 16, 2020  
Second release candidate for Chapel 2.0 (RC2)

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* converted major instances of implicit indexing to be 0- rather than 1-based:
  - tuples
  - varargs arguments
  - `string` and `bytes`
  - array literals
  - type-inferred captures of iterator expressions
  - random number streams
  - field numbering
  - related library routines

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* changed tuples to use 0-based indexing rather than 1-based  
  (e.g., `(1.2, 3.4)[1]` now returns `3.4` where it used to return `1.2`)
* similarly, varargs arguments now use 0-based indexing rather than 1-based  
  (e.g., in `foo(1,2); proc foo(xs...) {}`, `xs(1)` is `2` where it was `1`)
* dimensional queries on multidimensional domains and arrays are now 0-based  
  (e.g., in `var A: [1..5, 0..2] int;`, `A.dim(1)` is now `0..2`, not `1..5`)
* changed the `string` and `bytes` types to be 0-based by default  
  (e.g., `"Chapel"[1]` now returns `"h"` where it used to return `"C"`)
* changed `find()` and `rfind()` on `string`/`bytes` to return -1 on failure  
  (e.g., see https://chapel-lang.org/docs/1.22/builtins/String.html#String.string.find)
* changed array literals to have 0-based domains by default  
  (e.g., `[1.2, 3.4]` now has type `[0..1] real` rather than `[1..2] real`)
* changed inferred-type captures of iterator expressions to use 0-based arrays  
  (e.g., in `var A = myIter();` A's domain is now `[0..]` rather than `[1..]`)

Standard Library Modules
------------------------
* changed the 'List' module to use 0-based indexing of the `list` type  
  (see https://chapel-lang.org/docs/1.22/modules/standard/List.html#List.list)
* 'Map' module methods that return arrays now return 0-based arrays  
  (e.g., see https://chapel-lang.org/docs/1.22/modules/standard/Map.html#Map.map.toArray)
* streams in 'Random' module now count from 0 rather than 1  
  (e.g., see https://chapel-lang.org/docs/1.22/modules/standard/Random.html#Random.RandomStreamInterface.skipToNth)
* field-based queries in 'Reflection' now count fields from 0 rather than 1  
  (e.g., see https://chapel-lang.org/docs/1.22/modules/standard/Reflection.html#Reflection.getField)
* `getFieldIndex()` in 'Reflection' now returns -1 when a field is not found  
  (see https://chapel-lang.org/docs/1.22/modules/standard/Reflection.html#Reflection.getFieldIndex)

Memory Improvements
-------------------
* closed leaks in strings and bytes for certain multi-locale executions

Documentation
-------------
* added `import` statements to the modules primer and language specification  
  (see https://chapel-lang.org/docs/1.22/primers/modules.html  
   and https://chapel-lang.org/docs/1.22/language/spec/modules.html#importing-modules)
* documented module renaming via `use` in the modules primer and language spec  
  (see https://chapel-lang.org/docs/1.22/primers/modules.html  
   and https://chapel-lang.org/docs/1.22/language/spec/modules.html#using-modules)
* moved the description of `use` statements to the 'Modules' spec chapter  
  (see https://chapel-lang.org/docs/1.22/language/spec/modules.html#using-modules)
* added `import` pattern to each documented module's 'usage' section  
  (e.g., see https://chapel-lang.org/docs/1.22/modules/standard/DateTime.html)

Syntax Highlighting
-------------------
* added the `import` keyword to various syntax highlighters

Error Messages / Semantic Checks
--------------------------------
* improved several error messages related to out-of-bounds indexing

Bug Fixes
---------
* fixed a bug in the mason spack spec parser and simplified its implementation
* fixed a bug in `mason add --external` where some versions were not allowed
* updated some stale code in the 'HDF5' module

version 1.21.0
==============

Twenty-fourth public release of Chapel, April 9, 2020

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* namespace/module improvements:
  - added support for `import` statements as a precise way of accessing modules
  - added support for relative `import`/`use` chains via `this` and `super`
  - implemented prototypical support for storing submodules in different files
  - made `use` private by default and added support for renaming modules
  - sub-modules no longer have lexical visibility into their parent modules
  - reduced the degree to which standard module symbols leak into user code
* language:
  - the `locale` type now has value semantics
  - strings are now validated to ensure they are UTF8
  - significantly improved the `bytes` type, making it on par with `string`
  - added support for split-initialization of variables, types, params, refs
  - improved the compiler's support for copy elision and deinitialization
  - made `Error` classes store strings and preserve line numbers
  - added several features in support of index-neutral programming
  - improved numerous core features related to backwards-breaking changes
* performance:
  - improved the performance/scalability of creating distributed arrays/domains
  - improved the compiler's unordered operation optimization
  - extended `unorderedCopy()` to all trivially copyable types
  - improved the performance of `on`-statements for Infiniband networks
  - improved the performance and correctness of misaligned `ugni` transfers
  - optimized more cases involving zippered loops over distributed arrays
  - optimized 1D array re-allocations to be in-place when possible
  - improved the performance of serial I/O
* additional highlights:
  - converted the language specification from PDF to HTML
  - made several usability improvements to the `mason` package manager
  - improved support for Python-Chapel interoperability
  - significantly improved `ofi` functionality, portability, and performance
  - added a routine for writing arrays to HDF5 files in parallel across locales
  - closed a number of memory leaks

Syntactic / Naming Changes
--------------------------
* `private config`s must now be fully qualified when set on the command-line
* made minor adjustments to the argument names of `string` and `bytes` methods

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* made `use` private by default  
  (see https://chapel-lang.org/docs/1.21/language/spec/statements.html#the-use-statement)
* sub-modules no longer have lexical visibility into their parent modules    
  (see https://chapel-lang.org/docs/1.21/language/spec/modules.html#nested-modules)
* the `locale` type now has value semantics and a default value of `Locales[0]`
* strings are now validated to ensure they are UTF8
* `bytes.this` and `bytes.these` now return/yield `uint(8)` instead of `bytes`  
  (see https://chapel-lang.org/docs/1.21/builtins/Bytes.html)
* stopped generating `init=` functions for records with a user-defined `init=`
* an `init=` containing compilerError() now indicates the type is not copyable  
  (see https://chapel-lang.org/docs/1.21/technotes/initequals.html#disabling-copyability)
* added support for split initialization of variables, refs, types, and params  
  (see https://chapel-lang.org/docs/1.21/language/spec/variables.html#split-initialization  
   and https://chapel-lang.org/docs/1.21/language/evolution.html#split-initialization)
* added support for copy elision transformations  
  (see https://chapel-lang.org/docs/1.21/language/evolution.html#copy-elision  
   and https://chapel-lang.org/docs/1.21/language/spec/variables.html#copy-elision)
* improved `out` intents to support split initialization and to reduce copies  
  (see https://chapel-lang.org/docs/1.21/language/spec/procedures.html#the-out-intent)
* `record` temporaries may now be deinitialized at the end of a statement  
  (see https://chapel-lang.org/docs/1.21/language/evolution.html#deinitialization-point-of-nested-call-expressions  
   and https://chapel-lang.org/docs/1.21/language/spec/variables.html#deinit-points)
* compile-time (`type`, `param`) methods now require `override` when overridden  
  (see https://chapel-lang.org/docs/1.21/language/spec/classes.html#overriding-base-class-methods)
* made `enum` casts that may fail throw an error rather than halt  
  (see https://chapel-lang.org/docs/1.21/language/spec/conversions.html#explicit-enumeration-conversions)
* prototype modules are no longer treated specially with respect to nilability
* added an execution-time check to guard against resizing arrays of non-nilable  
  (see https://chapel-lang.org/docs/1.21/language/spec/classes.html#class-values)
* assignment overloads for classes are no longer permitted

New Features
------------
* added support for `import` statements as a more precise way of using modules
* added support for renaming a module in its `use` statement
* added explicit relative `import` and `use` chains via `this` and `super`  
  (see https://chapel-lang.org/docs/1.21/language/spec/statements.html#the-use-statement)
* implemented prototypical support for storing submodules in different files  
  (see https://chapel-lang.org/docs/1.21/technotes/module_include.html)
* added atomic `compareExchange()` that matches the semantics of C++  
  (see https://chapel-lang.org/docs/1.21/builtins/Atomics.html#Atomics.compareExchange)
* added support for changing the memory management policy of a class object  
  (see https://chapel-lang.org/docs/1.21/builtins/OwnedObject.html#OwnedObject.owned.create  
   and https://chapel-lang.org/docs/1.21/builtins/SharedObject.html#SharedObject.shared.create)
* added support for looping directly over heterogeneous tuples  
  (see https://chapel-lang.org/docs/1.21/language/spec/tuples.html#iteration-over-tuples)
* added a `.indices` query for tuples, strings, bytes, arrays, and lists  
  (see https://chapel-lang.org/docs/1.21/builtins/ChapelTuple.html#ChapelTuple.tuple.indices,  
   https://chapel-lang.org/docs/1.21/builtins/Bytes.html#Bytes.bytes.indices,  
   https://chapel-lang.org/docs/1.21/builtins/String.html#String.string.indices,  
   https://chapel-lang.org/docs/1.21/builtins/ChapelArray.html#ChapelArray.indices,  
   and https://chapel-lang.org/docs/1.21/modules/standard/List.html#List.list.indices)
* added support for `.first` and `.last` queries on an `enum` type  
  (see https://chapel-lang.org/docs/1.21/language/spec/types.html#enum.first)
* added support for (upper) open-interval ranges  
  (e.g., `lo..<hi` represents 'lo, lo+1, lo+2, ..., hi-2, hi-1')  
  (see https://chapel-lang.org/docs/1.21/language/spec/ranges.html#range-literals)
* records now support lexicographical comparison with `<`, `<=`, `>`, `>=`  
  (see https://chapel-lang.org/docs/1.21/language/spec/records.html#default-comparison-operators)
* added a `.hostname` method to locales  
  (see https://chapel-lang.org/docs/1.21/builtins/ChapelLocale.html#ChapelLocale.locale.hostname)
* added `string.encode()` to convert strings with escaped data to `bytes`  
  (see https://chapel-lang.org/docs/1.21/builtins/String.html#String.string.encode)
* added support for `enum` serving as an "any enumerated type" type constraint  
  (e.g., `proc foo(e: enum) ...` can take any enum as an argument)

Feature Improvements
--------------------
* improvements to the `bytes` type:
  - `bytes` now supports `param` values
  - `bytes` now supports `toByte()`  
    (https://chapel-lang.org/docs/1.21/builtins/Bytes.html#Bytes.bytes.toByte)
  - `bytes` now supports comparison operations
  - added `bytes.format()`, similar to `string.format()`
  - `bytes` can now be cast to `enum`
  - `bytes` can now be indexed with `byteIndex`
  - `bytes` can now be used as the index type for associative domains
* added a `policy` argument to some `string` factories to escape non-UTF8 data
* made all `Error` classes store a string to describe the error
* made thrown errors preserve the original line number when rethrown
* made postfix-`!` no longer required to access `param` and `type` fields
* improved resolution of methods and fields, particularly for private types
* non-nilable classes are now considered subtypes of their nilable counterparts
* improved the implementation of left shift (`<<`) on integers  
  (see https://chapel-lang.org/docs/1.21/language/spec/expressions.html#shift-operators)
* made `extern` blocks automatically generate an implicit `use SysCTypes`

Deprecated / Unstable / Removed Language Features
-------------------------------------------------
* disabled associative arrays of non-nilable classes
* deprecated `.length`/`.numIndices`/`.numElements` queries in favor of `.size`
* deprecated `new owned/shared(c)` in favor of `owned/shared.create(c)`
* deprecated postfix-`!` on class types in favor of using casts
* deprecated `string` vs. `bytes` comparisons
* deprecated `decodePolicy.ignore` in favor of new `decodePolicy.drop`  
  (see https://chapel-lang.org/docs/1.21/builtins/Bytes.html#Bytes.bytes.decode)
* deprecated support for spaces in query expressions  
  (e.g. `proc foo(arg: ? t)` or `proc foo(args... ? n)`)
* deprecated C++-style deinitializer names e.g. `proc ~C()`
* deprecated the use of `enumerated` as a way of saying "any `enum`"
* added an unstable warning for `new borrowed C()`
* added an unstable warning for semi-concrete enums
* added an unstable warning for enums with repeated values
* added an unstable warning for arrays with negative strides
* added an unstable warning for identifiers beginning with `chpl_` or `_`
* added an unstable warning for using first-class functions
* added an unstable warning for the `union` type
* removed deprecated "array-as-vector", "array-as-map/set" capabilities
* removed overloads of `+` between strings and non-string values
* removed previously deprecated `string` initializers
* removed `ascii()` and `asciiToString()`
* removed support for deprecated `init` copy initializers
* removed opaque domains and arrays
* removed comparison operators for `imag` values
* removed the temporary `enableParScan` configuration parameter

Deprecated / Removed Library Features
-------------------------------------
* deprecated `makeRandomStream()` in favor of `createRandomStream()`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Random.html#Random.createRandomStream)
* deprecated `regexp` in favor of `regexp(string)`
* deprecated methods on `channel` used to get, set, or clear error codes
* removed deprecated sort functions
* removed deprecated versions of `open()` from 'IO'
* removed `LINGER`, `SUBSCRIBE`, `UNSUBSCRIBE` and `setsockopt` from 'ZMQ'

Standard Library Modules
------------------------
* adjusted I/O routines to support non-UTF8 paths/filenames via escaped strings
* stopped including most 'IO' symbols by default  
  (see https://chapel-lang.org/docs/1.21/builtins/ChapelIO.html  
   and https://chapel-lang.org/docs/1.21/modules/standard/IO.html)
* enabled special methods for I/O such as `readThis` or `writeThis` to `throw`  
  (see https://chapel-lang.org/docs/1.21/builtins/ChapelIO.html#readthis-writethis-readwritethis  
   and https://chapel-lang.org/docs/1.21/primers/specialMethods.html#io-methods)
* adjusted several `channel` methods in the 'IO' module to `throw`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/IO.html)
* added `channel.readbytes` and updated `channel.readline` to support `bytes`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/IO.html#IO.channel.readbytes  
   and https://chapel-lang.org/docs/1.21/modules/standard/IO.html#IO.channel.readline)
* the regular expression type `regexp` is now generic and supports `bytes`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Regexp.html#Regexp.regexp)
* updated `map` to work with nilable `owned` and non-nilable `shared` classes  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Map.html#Map.map.getBorrowed)
* added a `map.keys()` iterator  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Map.html#Map.map.keys)
* moved the `parSafe` field in `map` to be after `keyType` and `valType`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Map.html#Map.map)
* added an initializer to the `Error` base class that accepts a string message  
  (https://chapel-lang.org/docs/1.21/builtins/ChapelError.html#ChapelError.Error)
* added `math.isclose()` for approximate equality checking  
  (https://chapel-lang.org/docs/1.21/modules/standard/Math.html#Math.isclose)
* updated `canResolve()` to return `false` when encountering a `compilerError`  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Reflection.html#Reflection.canResolve)
* added `isCopyable()`, `isAssignable()`, `isDefaultInitializable()` to 'Types'  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Types.html#Types.isCopyable)
* added `c_aligned_alloc()` to the 'CPtr' module  
  (see https://chapel-lang.org/docs/1.21/builtins/CPtr.html#CPtr.c_aligned_alloc)
* `defaultRNG` can now be used to select the default random number generator  
  (see https://chapel-lang.org/docs/1.21/modules/standard/Random.html#Random.defaultRNG)
* stopped including the 'CommDiagnostics' module by default

Package Modules
---------------
* added a routine to write a single HDF5 file in parallel with multiple locales  
  (see https://chapel-lang.org/docs/1.21/modules/packages/HDF5/IOusingMPI.html#IOusingMPI.hdf5WriteDistributedArray)
* moved `hdf5ReadDistributedArray() into a sub-module named 'HDF5.IOusingMPI'  
  (see https://chapel-lang.org/docs/1.21/modules/packages/HDF5/IOusingMPI.html#IOusingMPI.hdf5ReadDistributedArray)
* added additional HDF5 symbols to the 'C_HDF5' module  
  (see https://chapel-lang.org/docs/1.21/modules/packages/HDF5/C_HDF5.html)
* extended `LinearAlgebra.eig()` to support `complex` types  
  (see https://chapel-lang.org/docs/1.21/modules/packages/LinearAlgebra.html#LinearAlgebra.eig)
* added support for `bytes` messages to the 'ZMQ' module
* improved comparison sort to better handle arrays containing `owned` classes
* improved the performance of `mergeSort()`
* improved the performance of the 'EpochManager' by removing a counter
* enabled remote value forwarding for `DistributedDeque` and `DistributedBag`

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* parallelized scan operations for `PrivateDist`
* implemented `.localSlice()` for `Replicated` arrays
* optimized distributions for when nil-checks are enabled

New Tools / Tool Changes
------------------------
* added `mason init` subcommand for creating a package in an existing directory  
  (see https://chapel-lang.org/docs/1.21/tools/mason/mason.html#starting-a-new-package)
* added `mason new --name` flag to differentiate package name from directory  
  (see https://chapel-lang.org/docs/1.21/tools/mason/mason.html#starting-a-new-package)
* added a feature to run a subset of tests in a mason package with `mason test`  
  (see https://chapel-lang.org/docs/1.21/tools/mason/mason.html#testing-your-package)
* improved the output for `mason update` with multiple registries
* improved the error message when `mason run` is used without building first
* added a `--project-version` flag to `chpldoc`  
  (see https://chapel-lang.org/docs/1.21/tools/chpldoc/man.html)
* fixed copyright formatting bugs in `chpldoc` when run without an author
* fixed a bug in `chpldoc` with intervening single line comments
* improved syntax highlighting for `highlight`

Interoperability Improvements
-----------------------------
* added support for passing strings with embedded null bytes externally
* mapped Chapel strings to Python strings instead of Python bytes
* added support for the `bytes` type to Python interoperability

Performance Optimizations / Improvements
----------------------------------------
* improved the performance and scalability of distributed array/domain creation
* improved the unordered compiler optimization to optimize more cases  
  (see `--optimize-forall-unordered-ops` in `man chpl`)
* extended `unorderedCopy()` to all trivially copyable types
* increased cases where zippered distributed array iteration can be optimized
* significantly improved the performance and correctness of `--cache-remote`
* improved the performance of `on`-statements for InfiniBand networks
* optimized 1D array re-allocations to be in-place when possible
* added short-circuiting for array reallocations whose indices didn't change
* improved the performance of serial I/O
* optimized sparse domain assignment between COO and CSR/CSC
* improved the performance of assigning an empty sparse domain
* improved the performance of comparison sorts
* enabled warming up the runtime before calling user-code

Cray-specific Performance Optimizations/Improvements
----------------------------------------------------
* improved the performance of misaligned GETs under `ugni` communication
* minimized memory pressure from short-lived tasks issuing bulk communications
* increased the default number of memory regions under `ugni` communication

Memory Improvements
-------------------
* closed memory leaks related to first-class functions
* closed a leak in array views
* closed leaks in `Cyclic` and `BlockCyclic` distributions
* closed a leak in functions with variadic string arguments
* closed a leak when returning a tuple from a paren-less function
* closed a leak in loop-based array initialization with throwing functions
* closed memory leaks in comparison sorts
* fixed problems with memory management of unions

Documentation
-------------
* converted the language specification from PDF (LaTeX) to HTML (rst)  
  (see https://chapel-lang.org/docs/1.21/language/spec/index.html)
* generally updated documentation with respect to the changes in this release
* improved the documentation for `owned` and `shared` classes  
  (see https://chapel-lang.org/docs/1.21/builtins/OwnedObject.html  
   and https://chapel-lang.org/docs/1.21/builtins/SharedObject.html)
* improved the description of tuple semantics in the language specification  
  (see https://chapel-lang.org/docs/1.21/language/spec/tuples.html#value-tuples-and-referential-tuples)
* increased usage of the `:throws:` tag in library documentation
* added details about special I/O methods to the forwarding technote  
  (see https://chapel-lang.org/docs/1.21/technotes/forwarding.html#resolving-forwarded-methods)
* improved the documentation of the `UnitTest` module  
  (see https://chapel-lang.org/docs/1.21/modules/packages/UnitTest.html)

Example Codes
-------------
* updated `ra.chpl` to use `localAccess()` rather than a `local` block
* updated example codes to typically use `new C()` over `new owned C()`
* updated example codes to typically avoid using `new borrowed C()`
* updated examples to avoid `.length`, `.numElements`, `.numIndices`
* updated examples to add `throws` to `readThis()`/`writeThis()` routines
* updated example codes with respect to nilability changes
* updated example codes with respect to deinitialization and expiring values
* updated example codes with respect to required `use` statements
* updated `miniMD.chpl` with respect to split initialization changes

Portability
-----------
* reduced third-party linking support scripts' reliance on `libtool`
* increased user program linking consistency across configurations
* improved the portability of the HDFS and Curl modules for 32-bit platforms

Cray-specific Changes and Bug Fixes
-----------------------------------
* brought Cray Shasta support into the mainline Chapel module build system
* fixed several bugs related to misaligned transfers in `ugni` communication

Compiler Flags
--------------
* stopped printing blank flag suggestions for unrecognized flags
* deprecated the `--legacy-classes` flag

Runtime Library Changes
-----------------------
* significantly improved `ofi` functionality, portability, and performance

Launchers
---------
* fixed the CPU count for `slurm-srun` on systems with hyperthreading disabled

Generated Executable Flags
--------------------------
* added an `--lldb` flag to generated executables to aid with debugging

Error Messages / Semantic Checks
--------------------------------
* added a warning for potentially surprising implicit modules
* improved the error message for modules declared in function bodies
* added errors for most ownership transfers from non-nilable owned
* added an error for `new owned X`
* improved compile-time nil-checking to consider copy elision
* improved detection of unsound uses of nilable types
* improved lifetime checking to consider copy elision
* improved error messages for failed array bounds checks
* added an error for assigning an associative domain to a rectangular array
* added checking for uses of global variables before they are initialized
* added an error for mixing user- and compiler-generated `init=` and `=`
* added an error for copy initializers that do not have exactly one argument
* improved error messages for uninitialized variables
* improved error messages for unresolved calls due to missing parentheses
* added errors for certain confusing generic initialization patterns
* improved the error message for secondary methods that are missing their types
* improved error messages for bad var args input to `LinearAlgebra.Matrix()`  
  (see https://chapel-lang.org/docs/1.21/modules/packages/LinearAlgebra.html#LinearAlgebra.Matrix)
* improved error messages for unsupported element types in 'Set' and 'List'
* added safety checks for shift operations on integers by default
* improved checks for generic fields to include undecorated class types
* changed an internal error for unsupported type queries to a user-facing error
* eliminated spurious "this source location is a guess" in some warnings

Bug Fixes
---------
* fixed a bug in which domain-to-string casts were not working as intended
* fixed a bug in which user identifiers could conflict with internal ones
* fixed a bug preventing records with `owned` fields from being swapped (`<=>`)
* fixed a bug involving task-private variables and throwing initializers
* fixed `.localSlice` for `Block` and `Cyclic` arrays
* fixed several problems with type queries of class types
* fixed problems with casts between class types like `C: C?`
* fixed a problem with returning an array of non-nilable owned classes
* fixed a problem with tuples containing `owned` classes passed by `in` intent
* fixed a bug where comparing tuples of mismatched size caused a compiler error
* fixed a bug in which `list.sort()` did not support different comparator types
* fixed a problem with arguments of nested generic type such as `list(Error)`
* fixed compiler crashes for some uses of `numFields()` and `compilerWarning()`
* improved our running task counter for inlined functions with `on`-statements
* stopped considering network atomics as safe for fast-ons
* fixed some bugs in `bytes.decode()`
* fixed a bug with remote `bytes` copies
* fixed a bug with taking the `.type` of a first-class function
* fixed a bug in which 'UnitTest' methods expected 1-based arrays
* fixed a bug in which `RandomStream.choice()` failed for non-numeric types
* addressed two memory errors within the 'Futures' package module
* fixed a bug in which 'DistributedIters' still relied on string + value ops
* addressed problems with updating lists contained within a `map`

Packaging / Configuration Changes
---------------------------------
* updated copyrights to reflect HPE's acquisition of Cray Inc.
* recompute settings files used by `--llvm` in more `make` invocations
* removed GASNet support for Cray XE/XK

Third-Party Software Changes
----------------------------
* upgraded GASNet-EX to version 2020.3.0
* upgraded Qthreads to version 1.15
* updated `chpldoc` to use a bundled chapel domain
* updated the sphinx version used for `chpldoc`

Testing System
--------------
* adapted testing system to work more dependably on Slurm-based systems
* `start_test` can now be run simultaneously in different directories
* `start_test -memleaks` now deletes the existing log file

Developer-oriented changes: Documentation improvements
------------------------------------------------------
* added documentation for multilocale performance/communication count testing
* removed online documentation for some internal functions in the 'IO' module

Developer-oriented changes: Module changes
------------------------------------------
* `locale` is now implemented using a `record` type
* improved the statistical properties of hash functions for records and tuples
* made most uses of standard modules within internal modules `private`
* made most uses of 'ChapelStandard' in internal modules `private`
* replaced `use _ only;` with `import _;` in modules
* made the 'Bytes' module more index-neutral
* changed `_ddata` initialization to be param-controlled
* changed `MPI_Abort()` calls into `halt()`s in 'MPI' for cleaner teardowns

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* set nitpick flag to default to catch cross-reference errors in docs builds
* squashed back-end warnings about incompatible pointer types

Developer-oriented changes: Compiler Flags
------------------------------------------
* added a `--[no]-split-initialization` to disable/enable split initialization
* added a `--[no]-early-deinit` flag to disable/enable early deinitialization
* added a `--[no]-copy-elision` flag to disable/enable copy elision

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* improved debugging output for multilocale interop. marshalling routines
* started printing loop information and submodules in AST logs
* removed a bad optimization that was breaking internal module visibility rules
* simplified visible function determination of `use` statement visibility
* added a cache for determining if an actual's type is coercible
* only check visibility of symbols once per scope
* made the compiler munge internal module symbols and stopped munging fields
* improved resolution of calls performing string literal initialization
* dead-code eliminated unused calls to `chpl__convertValueToRuntimeType`
* removed `PRIM_TYPE_INIT`

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added `debug`/`nodbg` to runtime library paths, reducing the need to rebuild
* updated verbose communication output to include comm ID numbers
* optimized verbose comm diagnostics calls
* simplified and improved `ugni`'s processor atomic implementation
* added support for testing the `numa` locale model on non-NUMA systems

Developer-oriented changes: Testing System
------------------------------------------
* added current git branch/sha to `start_test` output
* improved testing error message when compilation fails
* nightly memory leak testing now reports errors if postprocessing fails
* fixed annotations for compiler performance graphs
* added support for `CHPL_LAUNCHER_REAL_WRAPPER` for slurm-gasnetrun launchers


version 1.20.0
==============

Twenty-third public release of Chapel, September 19, 2019  
First release candidate for Chapel 2.0 (RC1)

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* language:
  - completed the conversion of the `string` type from ASCII to UTF-8
  - added a new `bytes` type for representing arbitrary binary data
  - improved safety by distinguishing whether class types can store `nil`
  - improved managed class types w.r.t. generic management and argument passing
  - improved support for partial and complete instantiations of generic types
  - added `private use` and required `use` of top-level modules for name safety
  - improved the user-facing interfaces for `atomic` types
  - added the `nothing` type and `none` value to avoid dual meanings of `void`
* libraries and domain maps:
  - added `list`, `set`, `map` types to replace array-as-vector/-map features
  - added a new `UnitTest` module with support via `mason test`
  - added new `Reflection` routines to reason about source code locations
  - added a new `EpochManager` package for concurrent memory reclamation
  - made several improvements to the `LinearAlgebra` and `Sort` packages
* performance improvements:
  - improved the affinity and performance of parallel loops
  - optimized bulk transfers for Block-distributed arrays
  - enabled compiler-generated unordered operation optimizations by default
  - improved the performance and usability of unordered operations for users
  - enabled the parallel implementation of scans on 1D local and `Block` arrays
  - improved the performance of parallel-safe data structures
* additional highlights:
  - added support for creating multi-locale libraries callable from Python or C
  - added initial support for a pre-built Chapel module on Cray Shasta systems
  - improved compiler errors for failures in resolution and generic routines
  - improved `mason` w.r.t. testing, publishing, searching, and working offline
  - improved completeness/portability for libfabric providers via CHPL_COMM=ofi
  - improved the robustness of the LLVM back-end

Syntactic/Naming Changes
------------------------
* added new reserved keywords: `bytes`, `none`, `nothing`, `void`
* changed atomic memory orders to a new `memoryOrder` enum  
  (see https://chapel-lang.org/docs/1.20/builtins/Atomics.html#atomics)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* top-level modules must now be `use`d in order to be referenced  
  (see the `Using Modules` and `The Use Statement` sections of the Chapel spec)
* class types must now opt into being able to store `nil` using `?`  
  (e.g., `var myC: C = ...` cannot store `nil` but `var myC: C? = ...` can;  
   see https://chapel-lang.org/docs/1.20/language/evolution.html#nilability-changes)
* for a class `C`, `new C(...)` is now equivalent to `new owned C(...)`  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#new-c-is-owned)
* for a class `C`, un-decorated `C` now means non-nil and generic management  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#undecorated-classes-have-generic-management)
* passing owned/shared objects to untyped formals no longer changes their types  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#new-default-intent-for-owned-and-shared)
* arguments declared with owned/shared types now default to `const ref` intent  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#new-default-intent-for-owned-and-shared)
* string indexing, iteration, and lengths now use codepoint units by default  
  (see https://chapel-lang.org/docs/1.20/builtins/String.html#String.string.length)
* `string.find` now returns `byteIndex` values
* `void` is no longer allowed as the type of a variable (use `nothing` instead)  
  (see 'The Void Type' section in the 'Types' chapter of the spec)
* made `#0` preserve the given indices of a range or domain  
  (e.g., `5..#0` now returns `5..4` rather than `1..0`)
* disabled support for assigning from ranges to multidimensional arrays  
  (see 'Array Assignment' in the `Arrays` chapter of the language spec)
* made slice expressions be governed by their slicing domain  
  (e.g., in `ref B = A[D];` the domain of `B` is `D` rather than a copy of it)
* made `proc foo(x: bool(?))` generic across all bool sizes including `bool`
* `require` statements are now processed relative to the source file directory  
  (see https://chapel-lang.org/docs/technotes/extern.html#expressing-dependencies)
* class downcasts now throw `ClassCastError` for incompatible runtime types  
  (see 'Explicit Class Conversions' in the spec  
   and https://chapel-lang.org/docs/1.20/language/evolution.html#readme-evolution-nilability-and-casts)
* casts from a C pointer type to a class type now require a nilable class  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#readme-evolution-nilability-and-casts)
* `isSubtype()` and comparison operators on types no longer consider coercions  
  (see https://chapel-lang.org/docs/1.20/builtins/UtilMisc_forDocs.html#UtilMisc_forDocs.isSubtype)

New Features
------------
* added support for `private` and `public` `use` statements  
  (see 'The Use Statement' in the 'Statements' chapter of the language spec)
* added a built-in `bytes` type  
  (see https://chapel-lang.org/docs/1.20/builtins/Bytes.html)
* added bytes literals `b""`, `b''`, `b""" """`, and `b''' '''`  
  (see https://chapel-lang.org/docs/1.20/builtins/Bytes.html)
* added new postfix `?` and `!` operators to support nilable class features  
  (see https://chapel-lang.org/docs/1.20/language/evolution.html#nilability-changes)
* added support for partial instantiation of generic types  
  (see https://chapel-lang.org/docs/1.20/technotes/partialInstantiations.html)
* generic types can now be passed as `type` arguments and returned  
  (see https://chapel-lang.org/docs/1.20/technotes/partialInstantiations.html#passing-and-returning-generic-types)
* added factory functions for creating strings using existing buffers
* added support for slicing dense arrays using sparse domains
* added a `nothing` type with the singleton value `none`  
  (see 'The Nothing Type' section in the 'Types' chapter of the spec)
* `class`, `record`, and `enum` are now available as generic types  
  (see 'Built-in Generic Types' in the spec)
* added a `byteIndex` type for string indexing  
  (see https://chapel-lang.org/docs/1.20/builtins/String.html#String.byteIndex)
* added 'string.toByte()' and 'string.toCodepoint()'  
  (see https://chapel-lang.org/docs/1.20/builtins/String.html#String.string.toByte)
* added an atomic fence  
  (see https://chapel-lang.org/docs/1.20/builtins/Atomics.html#Atomics.atomicFence)

Feature Improvements
--------------------
* `isClass()` now returns `true` for `owned C` and `shared C`
* `min` and `max` on real values now propagate `NaN` values  
  (see https://chapel-lang.org/docs/builtins/UtilMisc_forDocs.html#UtilMisc_forDocs.min)
* casts on class types can now compute new types with specific management  
  (see 'Explicit Class Conversions' in the spec)
* added support for `yield`ing within `on`-clauses for inlined serial iterators
* task/forall intents in methods on records now operate on fields of `this`  
  (see 'Task Intents' and 'Forall Intents' in the spec)
* improved complex division to avoid underflow and overflow
* made string slicing more consistent with array slicing w.r.t. OOB errors
* disabled `string.toLower()`/`toUpper()` for codepoints that change byte size
* enabled `var myField: T = expr;` when `T` has no default initializer
* `dmapped` and `new dmap` can now accept an `owned` distribution
* the `ref` return intent is now supported for `extern` procedures
* added support for passing the `string` type to `extern` procedures
* improved how first-class functions print themselves out
* enabled `in`-intents of POD record types in follower iterators

Deprecated / Removed Language Features
--------------------------------------
* deprecated "array-as-vector" methods in favor of the new `list` type  
  (e.g., given `var A: [1..10] real;`, `A.push_back()` is no longer supported)
* deprecated "associative array-as-map" methods in favor of the new `map` type
* deprecated support for applying `+` to string/value pairs  
  (e.g., `"hello " + 1` is no longer supported by default)
* deprecated inequality comparisons on `imag`
* deprecated support for `init`-based copy-initializers; use `init=` instead  
  (see https://chapel-lang.org/docs/1.20/technotes/initequals.html)
* deprecated support for opaque domains and arrays
* deprecated atomic `compareExchange` in favor of `compareAndSwap`  
  (see https://chapel-lang.org/docs/1.20/builtins/Atomics.html#Atomics.compareAndSwap)
* made atomic `peek`/`poke` no longer available by default  
  (see https://chapel-lang.org/docs/1.20/modules/packages/PeekPoke.html)
* deprecated `string.ascii` and `asciiToString`
* deprecated string initializers in favor of new clearer factory functions
* replaced the undocumented `_void` value with `none`
* removed previously deprecated string features  
  (e.g., `string.ulength`, `string.uchars`, `string.codePointIndex`, ...)
* removed support for explicit task/forall intents on `this`

Deprecated / Removed Library Features
-------------------------------------
* repurposed previously deprecated `List` module for the new `list` type
* deprecated algorithm-specific sorts like `bubbleSort()` from `Sort` module
* deprecated `ZMQ.Socket.setsockopt()` in favor of specific setters and getters  
  (e.g., deprecated constant `LINGER` in favor of `Socket.[set|get]Linger()`;  
   see https://chapel-lang.org/docs/1.20/modules/packages/ZMQ.html#ZMQ.Socket.setsockopt)
* retired deprecated usage of `LinearAlgebra.eigvals()`
* removed previously deprecated functions with `out error` arguments from `IO`

Standard Library Modules
------------------------
* added new `list`, `set`, and `map` collection types  
  (see https://chapel-lang.org/docs/modules/standard/List.html,  
   https://chapel-lang.org/docs/1.20/modules/standard/Set.html,  
   and https://chapel-lang.org/docs/1.20/modules/standard/Map.html)
* added `Reflection` functions to get module/routine/file name and line number  
  (see https://chapel-lang.org/docs/1.20/modules/standard/Reflection.html#Reflection.getLineNumber)
* added a `seek()` method on channels to the `IO` module  
  (see https://chapel-lang.org/docs/1.20/modules/standard/IO.html#IO.channel.seek)
* `channel.mark` now returns the marked offset and throws if there is an error  
  (see https://chapel-lang.org/docs/1.20/modules/standard/IO.html#IO.channel.mark)
* added `signbit` to the `Math` module  
  (see https://chapel-lang.org/docs/1.20/modules/standard/Math.html#Math.signbit)
* added `isNilableClass()` and `isNonNilableClass()` to the `Types` module  
  (see https://chapel-lang.org/docs/1.20/modules/standard/Types.html#Types.isNilableClass)
* added `isGeneric()` to the `Types` module  
  (see https://chapel-lang.org/docs/1.20/modules/standard/Types.html#Types.isGeneric)
* added `isCoercible()` to the `Types` module  
  (see https://chapel-lang.org/docs/1.20/builtins/UtilMisc_forDocs.html#UtilMisc_forDocs.isCoercible)
* made the `CommDiagnostics` module count atomic memory operations  
  (see https://chapel-lang.org/docs/1.20/modules/standard/CommDiagnostics.html)
* added file:line for executeOn in verbose comm diagnostics, matching other ops
* reduced the degree to which standard modules leak symbols into user code

Package Modules
---------------
* added new `UnitTest` module for writing unit tests in Chapel  
  (see https://chapel-lang.org/docs/1.20/modules/packages/UnitTest.html)
* Added a new `URL` package module  
  (see https://chapel-lang.org/docs/1.20/modules/packages/URL.html)
* added `AtomicObjects` to support atomic operations on unmanaged classes  
  (see https://chapel-lang.org/docs/1.20/modules/packages/AtomicObjects.html)
* added `LockFreeQueue` and `LockFreeStack` data structures  
  (see https://chapel-lang.org/docs/1.20/modules/packages/LockFreeQueue.html  
   and https://chapel-lang.org/docs/1.20/modules/packages/LockFreeStack.html)
* added an `EpochManager` package to support epoch-based memory reclamation  
  (see https://chapel-lang.org/docs/1.20/modules/packages/EpochManager.html)
* `sort` in the Sort module now uses radix sort for floating point types  
  (see https://chapel-lang.org/docs/1.20/modules/packages/Sort.html#Sort.sort)
* dropped the LLVM back-end requirement for uses of the `HDF5` module
* enabled distributed element-wise operations in the `LinearAlgebra` module
* improved distributed dot product in the `LinearAlgebra` module
* added sparse support for `LinearAlgebra.{isDiag, isSymmetric, isHermitian}`  
  (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra/Sparse.html#Sparse.isDiag)
* added several new operations to the `LinearAlgebra` module:
  - setting the matrix diagonal  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.setDiag)
  - LU factorization  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.lu)
  - computing determinants  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.det)
  - a linear system solver  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.solve)
  - matrix inversion  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.inv)
  - the Jacobi method  
    (see https://chapel-lang.org/docs/1.20/modules/packages/LinearAlgebra.html#LinearAlgebra.jacobi)
* added date and time support to the `TOML` module
* added per-task fences for unordered operations  
  (see https://chapel-lang.org/docs/1.20/modules/packages/UnorderedCopy.html#UnorderedCopy.unorderedCopyTaskFence  
   and https://chapel-lang.org/docs/1.20/modules/packages/UnorderedAtomics.html#UnorderedAtomics.unorderedAtomicTaskFence)
* added support for `param`/`const` source arguments to `unorderedCopy()`
* extended `unorderedCopy()` to support `bool` values  
  (see https://chapel-lang.org/docs/1.20/modules/packages/UnorderedCopy.html#UnorderedCopy.unorderedCopy)
* added support for several setter and getter routines on ZMQ sockets  
  (e.g., added `Socket.getLastEndpoint()`, `Socket.getLinger()`, etc.;  
   see https://chapel-lang.org/docs/1.20/modules/packages/ZMQ.html#ZMQ.Socket.getLastEndpoint)
* added new `Error` subclass `ZMQError` for `ZMQ`-specific errors  
  (see https://chapel-lang.org/docs/1.20/modules/packages/ZMQ.html#ZMQ.ZMQError)
* added support for the `PAIR` socket type to the `ZMQ` module  
  (see https://chapel-lang.org/docs/1.20/modules/packages/ZMQ.html#ZMQ.PAIR)
* renamed 'Buffers.bytes' to 'Buffers.byteBuffer'  
  (see https://chapel-lang.org/docs/1.20/modules/packages/Buffers.html)
* Significantly improved the performance and testing of the `Curl` module  
  (see https://chapel-lang.org/docs/1.20/modules/packages/Curl.html)
* Fixed the `HDFS` module and added regular testing for it  
  (see https://chapel-lang.org/docs/1.20/modules/packages/HDFS.html)
* reduced the degree to which package modules leak symbols into user code

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* added support for adding local indices to distributed sparse domains  
  (see https://chapel-lang.org/docs/1.20/builtins/ChapelArray.html#ChapelArray.bulkAdd)
* added support for bulk addition of indices to distributed sparse domains  
  (see https://chapel-lang.org/docs/1.20/builtins/ChapelArray.html#ChapelArray.makeIndexBuffer)
* optimized the implementation of ownership queries for `Cyclic` domains/arrays
* optimized communication out of `forall` loops over `Cyclic` domains/arrays

New Tools / Tool Changes
------------------------
* added `UnitTest` support to `mason test`  
  (see https://chapel-lang.org/docs/1.20/tools/mason/mason.html#testing-your-package)
* added `mason publish` subcommand to make publishing packages easier  
  (see https://chapel-lang.org/docs/1.20/tools/mason/mason.html#submit-a-package)
* added `mason search --show` flag to display manifest file of package  
  (see https://chapel-lang.org/docs/1.20/tools/mason/mason.html#submit-a-package)
* added `MASON_OFFLINE` to improve offline experience for mason users  
  (see https://chapel-lang.org/docs/1.20/tools/mason/mason.html#environment-variables)
* pinned the version of `Pygments` relied upon by `chpldoc`

Interoperability Improvements
-----------------------------
* added support for multi-locale Chapel libraries  
  (see https://chapel-lang.org/docs/1.20/technotes/libraries.html#using-your-library-in-multilocale-settings)
* the LLVM back-end now supports `--library-python` and `--library-makefile`
* added support for exporting routines that accept/return strings
* added support for overriding environment variables to `--library-makefile`
* `.c` files on the `chpl` command line can now `#include "chplrt.h"`
* `extern` blocks now support unary `+` in `#define` directives  
  (e.g. `#define X (+1)`)
* added support for passing the `string` type to extern procedures

Performance Optimizations/Improvements
--------------------------------------
* enabled optimized bulk transfers for `Block`-distributed arrays by default
* improved the unordered compiler optimization and enabled it by default  
  (see `--optimize-forall-unordered-ops` in `man chpl`)
* enabled a parallel implementation of scans for 1D local and `Block` arrays
* improved task affinity for parallel loops
* optimized `op=` overloads for array/scalar pairs
* improved the speed of parallel-safe `RandomStream` instances
* improved the performance of `sort` and `isSorted` in the `Sort` module
* reduced contention from polling threads for CHPL_COMM=gasnet
* improved the implementation for `log2` and `logBasePow` for integral types
* optimized iteration over `Stencil`-distributed arrays
* created a prototype optimization for slicing arrays by domains  
  (compile with `-schpl_serializeSlices` to enable)

Cray-specific Performance Optimizations/Improvements
----------------------------------------------------
* optimized `unorderedCopy()` for remote destinations
* improved the performance of all unordered operations
* made out-of-memory handling with `ugni` communication more robust

Memory Improvements
-------------------
* minimized memory pressure from short-lived tasks

Documentation
-------------
* added a new chapter to the language specification on error-handling  
  (see the 'Error Handling' chapter in the language specification)
* added a new technical note describing partial instantiations  
  (see https://chapel-lang.org/docs/1.20/technotes/partialInstantiations.html)
* updated `mason` documentation to reflect new mason features  
  (see https://chapel-lang.org/docs/1.20/tools/mason/mason.html)
* documented that a user's environment must support Unicode with UTF8 encoding  
  (see https://chapel-lang.org/docs/1.20/usingchapel/chplenv.html#character-set)
* added `bytes` to the language specification as a primitive type
* clarified that modules that are not referred to are not initialized  
  (see 'Module Initialization' in the spec)
* updated the language specification with respect to other language changes
* updated `chpldoc` documentation to use `--output-dir` instead of `--docs-dir`  
  (see https://chapel-lang.org/docs/1.20/tools/chpldoc/chpldoc.html#documenting-modules)
* updated reductions technote to use memory management annotations  
  (see https://chapel-lang.org/docs/1.20/technotes/reduceIntents.html)
* improved the `--help` description of the `--fast` flag
* added a warning to 'channel.readf' documentation  
  (see https://chapel-lang.org/docs/1.20/modules/standard/IO/FormattedIO.html#FormattedIO.channel.readf)
* removed a reference to old assignment behavior from interoperability technote
* fixed several broken links in the online documentation

Example Codes
-------------
* added a primer for the new `list` type  
  (see https://chapel-lang.org/docs/1.20/primers/listOps.html)
* replaced the `voidVariables` primer with a `nothingVariables` primer  
  (see https://chapel-lang.org/docs/1.20/primers/nothingVariables.html)
* improved the `specialMethods` primer to remove the files it creates  
  (see https://chapel-lang.org/docs/1.20/primers/specialMethods.html)
* disabled some numerically unstable kernels in LCALS

Portability
-----------
* fixed cross-compilation support for GMP builds
* fixed support for `--llvm` with GASNet on Crays
* resolved several warnings when building with GCC 9
* added new environment variables `CHPL_RT_MASTERIP` and `CHPL_RT_WORKERIP`  
  (see https://chapel-lang.org/docs/1.20/usingchapel/launcher.html#chpl-rt-masterip)
* fixed a code portability issue in the `pbs-aprun` launcher

Cray-specific Changes and Bug Fixes
-----------------------------------
* added initial support for a pre-built Chapel module on Cray Shasta systems  
  (see https://chapel-lang.org/docs/1.20/platforms/cray.html#getting-started-with-chapel-on-cray-shasta-systems)
* updated modulefile to work if there is an incompatible cray-mpich pre-loaded
* Fixed a problem with --llvm compilation when using dynamic linking on a Cray
* fixed a hang for strided communication

Compiler Improvements
---------------------
* added support for overload-set checking  
  (see https://chapel-lang.org/docs/1.20/technotes/overloadSets.html)
* stopped the compiler from parsing the same file multiple times
* improved the accuracy of detecting infinitely recursive instantiations
* sub-modules that are not referred to are removed early in compilation

Compiler Flags
--------------
* a new `--print-all-candidates` flag gives verbose output for unresolved calls

Error Messages / Semantic Checks
--------------------------------
* function resolution errors now describe a reason for the failure
* compiler errors in generic functions now describe the failing instantiation
* improved checks that procedures return along all paths for `try/catch/throw`
* improved checking for multiple management decorators on a class
* added an error message for incorrect tuple expansion usage
* added a useful error when creating arrays of a generic type
* added an error message for ranges over an enum type with a single value
* made defining a method named 'borrow' a compiler error
* added an error for domain expressions that are types rather than values
* added non-`[const] in` intents error messages in certain interop situations  
  (see https://chapel-lang.org/docs/1.20/technotes/libraries.html#intents  
   and https://chapel-lang.org/docs/1.20/technotes/libraries.html#intents-in-python-interoperability)
* improved error message wording for type mismatches in assignment
* made a user-facing "the type of the actual argument is generic" error

Bug Fixes
---------
* fixed a bug in which internal/standard module symbols shadowed user symbols
* fixed a bug in which the count operator (`#`) was not creating an array alias
* fixed a bug in parallel scans of Block arrays for non-`+` operators
* fixed a bug in which scans of array slices were not working
* fixed several bugs with casting for managed class types
* fixed problems when an `owned` argument is used in a later default expression
* `isRecordType` now returns `false` for `owned` and `shared` classes
* fixed a bug in which parsing files multiple times caused duplicate modules
* fixed a bug in tracking number of nonzeroes for distributed sparse domains
* fixed a bug in index addition to distributed CSC domains
* fixed a bug in which missing line numbers in the IR caused an internal error
* fixed a null-termination issue with `string.join`
* fixed a problem with the string-to-uint cast `"0xffffffffffffffff":uint`
* fixed a bug where `isRecord()` returned true for `string`
* resolved problems when calling `sort` on arrays with aligned domains
* fixed reading of associative domains
* enabled const checking on index variables of forall loops in some cases
* `defer` actions in an iterator are now executed upon a `break` from its loop
* fixed a bug with `use ZMQ` in `--library` compilation
* fixed problems capturing a throwing function as a first-class function
* fixed casts from `real` to `bool` when using --llvm
* the `--dynamic` flag now creates a shared library under the LLVM back-end
* object files generated by the LLVM back-end now respect `CHPL_LIB_PIC=pic`
* fixed a compiler segfault with certain recursive class declarations
* fixed incorrect instantiation for certain generic class methods
* fixed problems with arguments using `new` as a default expression
* fixed bug with error handling for default args giving an incorrect error
* fixed bug with error handling for default args not issuing an error correctly
* fixed a bug where range arguments with `in` intent were not modifiable
* fixed errors in JSON I/O for block arrays
* fixed precision loss when printing real numbers to JSON format
* fixed bug when opening a channel on a different locale from its file
* fixed a bug in override checking
* fixed a bug in the compiler's analysis of non-aliasing arrays
* fixed a `vim` syntax highlighting bug involving domain literals
* fixed bug when shrinking an array with `pop_back()`
* added a thrown error for `FileSystem.mkdir('')` instead of a seg-fault
* fixed a bug in `TOML` where empty lines were not counted in the line number
* fixed `mason test` bug where exit code would not reflect test results
* fixed `mason test` bug in which subdirectories were not being traversed
* fixed a bug in which `mason doc` did not process used modules
* fixed `mason` help flags such that being in a package is not required
* fixed `mason search` to only show bricks that support current Chapel version
* fixed `make mason` such that it will not hang if hugepages is unavailable
* fixed `make mason` to detect changes in any mason source files
* fixed `make clobber` such that it clobbers mason

Packaging / Configuration Changes
---------------------------------
* added a warning for unfamiliar CHPL_*_COMPILER settings
* cleaned up the output of `make` when building Chapel

Third-Party Software Changes
----------------------------
* upgraded GASNet-EX to version 2019.6.0
* upgraded hwloc to version 1.11.13
* updated the LLVM version 8.0
* removed bundled copy of libhdfs3 since this project is no longer maintained
* retired massivethreads tasking

Runtime Library Changes
-----------------------
* made CHPL_COMM=ofi significantly more complete and portable across providers
* I/O buffers are no longer page aligned when the buffer size is very small
* retired massivethreads tasking

Launchers
---------
* made verbose output from launchers include environment variables they set  
  (see https://chapel-lang.org/docs/1.20/usingchapel/launcher.html#chapel-launchers)
* added CHPL_COMM_USE_GDB support to many more launchers  
  (see https://chapel-lang.org/docs/1.20/usingchapel/debugging.html#running-in-gdb)
* improved CPU binding for gasnetrun* launchers
* added a slurm-gasnetrun_mpi launcher

Testing System
--------------
* added support for running multilocale C tests via `start_test`  
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst#comparing-to-a-c-version)

Developer-oriented changes: Module changes
------------------------------------------
* made many modules use `private use` to avoid leaking names
* added an optimized spinlock wrapper to replace manual test-and-set loops
* `assert()` prints out line numbers from the call site even with `--devel`
* removed stale modules: `ChapelDynDispHack`, `Chapel[Tasks|Tasks]Internal`
* refactored `ChapelStandard` to focus on modules implementing the language
* made arrays "always RVF"
* simplified the standalone parallel iterator for default rectangular arrays
* added missing 'override' keywords to dsiDisplayRepresentation overrides
* moved definition of `chpl_emptyLocale[s|Space]` to remaining `LocaleModel`s
* moved definitions of some `read/writeThis` functions to `ChapelIO`
* eliminated a self-assignment in `chpl__mod`
* added experimental distributed sort to the `Sort` module
* new internal module 'ByteBufferHelpers' for bytes/string buffer management
* new internal module 'BytesStringCommons' for common bytes/string helpers
* added minimum string allocation size 'param chpl_minStringAllocSize=0'

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* added a script that can install the bundled LLVM

Developer-oriented changes: Compiler Flags
------------------------------------------
* added flags to prepend directories to the internal / standard module paths  
  (see `--prepend-internal-module-dir` and `--prepend-standard-module-dir`)
* added `--library-ml-debug` to debug multilocale library communication

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* started respecting "always rvf" pragmas at module scope
* stopped heap-promoting local variables
* addressed several build problems when using newer LLVM versions
* generated `llvm.ident` now includes Chapel compiler information
* `--llvm` compilation avoids generating and compiling C code
* `--llvm` compilation now emits `llvm.lifetime.start` and `llvm.lifetime.end`
* `--llvm-print-ir` can now accept multiple functions to print
* generated more debug information under `--llvm -g`
* `--print-commands` more reliably prints the commands spawned
* added 'dtBytes' to represent the new `bytes` type
* improved an internal error message about actual / formal mismatches
* added new primitives in support of `UnitTest module  
  (see `PRIM_GATHER_TESTS`, `PRIM_GET_TEST_BY_NAME`, `PRIM_GET_TEST_BY_INDEX`)
* removed vestiges of `--heterogeneous` support
* refactored compiler to re-use stripdirectories() function
* the compiler now represents decorated class types with `DecoratedClassType`
* the compiler now handles certain cast calls directly
* added 'FLAG_CHAPEL_BYTES_LITERAL' AggregateType
* fixed an override checking bug with --devel flag

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added CHPL_RT_OVERSUBSCRIBED to generally indicate oversubscribed execution  
  (see https://chapel-lang.org/docs/1.20/usingchapel/tasks.html#overloading-system-nodes)
* fixed the qthreads build when CHPL_HOME doesn't match CHPL_MAKE_HOME
* reading I/O channels now default to using `pread` instead of `mmap`
* I/O plugins implementing Curl and HDFS are now implemented in Chapel
* added support for `bytes` in QIO
* reorganized and simplified runtime support for comm diagnostics
* moved multilocale global var dissemination from comm layers into shared code

Developer-oriented changes: Testing System
------------------------------------------
* added paratest support for `<dir>.notest` to reflect `start_test` behavior
* improved performance graph screenshot quality and resolution
* added Python 3 support for `chpl_launchcmd`
* lowered polling frequency for `chpl_launchcmd`
* rewrote gen-chpl-bash-completion in Python
* fixed a problem with `start_test` when using Python 3.7
* resolved a race condition when `sub_test` creates directories
* added a test suite for `sub_test` filters


version 1.19.0
==============

Twenty-second public release of Chapel, March 21, 2019

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* language:
  - added support for compile-time operations on floating point `param` values
  - improved initializers w.r.t. type aliases and copy initialization
  - improved strings with respect to accesses, iterators, and UTF-8 support
  - added support for lifetime annotations on functions to help with checking
  - added support for compile-time nil-checking to avoid potential errors
  - made `throw` and `catch` operate on `owned` errors
  - added shape / index preservation for scans and range expressions
  - added support for underscores in numeric literals and strings
  - defined `[]`/promotions as implying "may" parallelism vs. `forall`'s "must"
* libraries and domain maps:
  - added support for distributed associative domains/arrays via `HashedDist`
  - added support for unordered copies and non-fetching atomics
  - added a `choice()` method to the `Random` module for weighted sampling
  - added `map()`, `filter()`, and `consume()` for iterators
  - added most of the remaining routines to the `Path` module
  - added convenience routines for creating Block or Cyclic domains and arrays
  - added a parallel radix sort method to the `Sort` module
* performance improvements:
  - added a prototype optimization that makes use of unordered communications
  - added a prototype parallelization of 1D scans for block and default arrays
  - eliminated several remaining sources of memory leaks
  - optimized remote task spawns on Cray systems using `ugni` communication
  - optimized ordered, unordered, and oversubscribed communication for `ugni`
* interoperability:
  - added a `c_array` type for interoperating with fixed-size C arrays
  - improved the previous prototype for interoperating with Python
  - introduced initial support for interoperating with Fortran
* additional highlights:
  - dramatically improved the level of support for the LLVM back-end
  - added a new Open Fabrics Interfaces libfabric-based `ofi` runtime option
  - made `cstdlib` atomics the default for the gcc, clang, and llvm back-ends
  - introduced variants of RA that use unordered atomics and oversubscription
  - added new primers for records, tuples, and special object methods

Packaging / Configuration Changes
---------------------------------
* `CHPL_TARGET_ARCH` now refers to machine types and `CHPL_TARGET_CPU` to cpus
  (e.g., `export CHPL_TARGET_ARCH=x86_64`, `CHPL_TARGET_CPU=sandybridge`
   see https://chapel-lang.org/docs/1.19/usingchapel/chplenv.html#chpl-target-arch
   and https://chapel-lang.org/docs/1.19/usingchapel/chplenv.html#chpl-target-cpu)
* made `cstdlib` atomics the default for gcc, clang and llvm compilers
  (see https://chapel-lang.org/docs//1.19/usingchapel/chplenv.html#chpl-atomics)

Syntactic/Naming Changes
------------------------
* made `true`, `false`, and numeric type names into reserved words in Chapel
  (see the 'Keywords' section in the 'Lexical Structure' chapter of the spec)
* added support for underscores in integer and floating point literals
  (e.g., `1_000_000` is now the same as `1000000`
   see the 'Literals' section in the 'Lexical Structure' chapter of the spec)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* made `throw` and `catch` operate on `owned` errors, enabling re-throwing
  (see https://chapel-lang.org/docs/1.19/primers/errorHandling.html)
* for records, compiler-generated `=`, `==`, `!=` now require matching types
  (see 'Record Assignment' and 'Default Comparison Operators' in the spec)
* `[]`-loops now permit serial execution if parallelism isn't an option
  (see 'The Forall Statement' in the 'Data Parallelism' chapter of the spec)
* made default string accesses always return a string
  (see https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.this)
* default initializers for generic types require formal names to match fields
  (see https://chapel-lang.org/docs/1.19/technotes/initTypeAlias.html)

New Features
------------
* added support for directly initializing atomic variables from values
  (e.g., `var x: atomic int = 10;`)
* added lifetime annotations for expressing relative lifetimes between arguments
  (see https://chapel-lang.org/docs/1.19/technotes/lifetimeChecking.html#lifetime-annotations)
* certain dereferences of `nil` values are now caught by the compiler
  (see https://chapel-lang.org/docs/1.19/technotes/nilChecking.html)
* added support for using type aliases in `new` expressions
  (see https://chapel-lang.org/docs/1.19/technotes/initTypeAlias.html)
* added support for the `init=` method, a proposed change to copy initializers
  (see https://chapel-lang.org/docs/1.19/technotes/initequals.html)
* added support for ranges of codepoints and slicing of strings using them
  (see https://chapel-lang.org/docs/1.19/builtins/String.html)
* added `string.codepoint[s]()` to access and iterate over strings by codepoint
  (see https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.codepoint
   and https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.codepoints)
* added `string.byte[s]()` to access and iterate over strings by byte
  (see https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.byte
   and https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.bytes)
* added automatic fences for unordered operations at task termination

Feature Improvements
--------------------
* added support for compile-time operations on floating point `param` values
* added config disambiguation for `chpl` arguments via module names
  (e.g., `-sM.debug=true` can be used to set config `debug` in module `M`)
* made scan expressions preserve the shape/domain of the input expression
  (e.g., in `var B = + scan A;`, B will have A's shape and domain)
* inferred-type arrays defined using range expressions now preserve indices
  (e.g., `var A = [i in -2..2] i;` now results an array with domain {-2..2})
* extended `reshape()` to accept loop expressions
  (e.g., `reshape([i in 1..4] i, {1..2, 1..2})` now works)
* string casts to integral or floating point types may now contain underscores
  (e.g., `"1_000_000":int` is now supported)
* string casts to integral types can now be in base 2, 8, or 16
  (e.g., `"0xabcd":int` is now supported)
* array initializer expressions based on range expressions are now parallelized
* added support for declaring homogeneous tuple types using `uint`s
  (e.g., `param d = 3: uint; var t: d*int;` is now supported)
* added support for `where`-clauses to `extern` procedures
* added support for return type constraints on generic routines

Deprecated and Removed Features
-------------------------------
* removed support for `chpl-ipe`
* removed support for `noinit` for the time being
* deprecated `domain.member()` in favor of `domain.contains()`
* deprecated `string.isEmptyString()` in favor of `string.isEmpty()`
  (see https://chapel-lang.org/docs/1.19/builtins/String.html#String.string.isEmpty)
* deprecated support for `string.uchars()` in favor of `string.codepoints()`
* deprecated support for `string.ulength()` in favor of `string.numCodepoints()`
* deprecated support for the `codePointIndex` type in favor of `codepointIndex`
* deprecated support for `codePointToString()` in favor of `codepointToString()`
* deprecated the `List` module, renaming it `LinkedList`
  (see https://chapel-lang.org/docs/1.19/modules/standard/LinkedLists.html)
* deprecated the `BufferedAtomics` module, in favor of `UnorderedAtomics`
  (see https://chapel-lang.org/docs/1.19/modules/packages/UnorderedAtomics.html)
* removed previously deprecated behavior of `:` in where clauses
* removed previously deprecated `matPlus()`, `matMinus()` from `LinearAlgebra`
* removed previously deprecated `isBLAS_MKL` flag from `BLAS` module

Standard Modules / Library
--------------------------
* improved UTF-8 string support
  (see https://chapel-lang.org/docs/1.19/builtins/String.html#activating-unicode-support)
* added `Path` routines `[file].absPath()`, `[file].relPath()`, and `normPath()`
  (see https://chapel-lang.org/docs/1.19/modules/standard/Path.html)
* added `choice()` method to `Random` module for weighted sampling
  (see https://chapel-lang.org/docs/1.19/modules/standard/Random.html#Random.RandomStreamInterface.choice)
* added `getNext(resultType, min, max)` method to the PCG Random Stream
  (see https://chapel-lang.org/docs/1.19/modules/standard/Random/PCGRandom.html#PCGRandom.RandomStream.getNext)
* added bounds checking to `getNext(min, max)` in the `Random` module
  (see https://chapel-lang.org/docs/1.19/modules/standard/Random/PCGRandom.html#PCGRandom.RandomStream.getNext)
* added message sizes to verbose communication diagnostics
  (see https://chapel-lang.org/docs/1.19/modules/standard/CommDiagnostics.html)
* added cast from integral types to `bigint`
  (see https://chapel-lang.org/docs/1.19/modules/standard/BigInteger.html#biginteger)
* added `c_offsetof` to get memory offsets for fields in a record
  (see https://chapel-lang.org/docs/1.19/builtins/CPtr.html#CPtr.c_offsetof)

Package Modules
---------------
* added `map()`, `filter()`, and `consume()` for iterators
  (see https://chapel-lang.org/docs/1.19/modules/packages/FunctionalOperations.html)
* added parallel radix sort to the `Sort` module and made use of it in `sort()`
  (see https://chapel-lang.org/docs/1.19/modules/packages/Sort.html)
* added an interface for performing unordered copies
  (see https://chapel-lang.org/docs/1.19/modules/packages/UnorderedCopy.html)
* added unordered versions of non-fetching atomic operations
  (see https://chapel-lang.org/docs/1.19/modules/packages/UnorderedAtomics.html)
* renamed `eigvals()` to `eig()` and made `eigvals()` support eigenvalues only
  (see https://chapel-lang.org/docs/1.19/modules/packages/LinearAlgebra.html#LinearAlgebra.eigvals)
* added checks to `LinearAlgebra` routines to ensure non-distributed arrays
* stopped transitively using `BLAS` and `LAPACK` in the `LinearAlgebra` module

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* added `HashedDist` to support distributed associative domains and arrays
  (see https://chapel-lang.org/docs/1.19/modules/dists/HashedDist.html)
* added support for queries of remote subdomains to most standard domain maps
  (see https://chapel-lang.org/docs/1.19/builtins/ChapelArray.html#ChapelArray.hasSingleLocalSubdomain)
* added convenience functions to simplify creating Block/Cyclic domains/arrays
  (see https://chapel-lang.org/docs/1.19/modules/dists/BlockDist.html
   and https://chapel-lang.org/docs/1.19/modules/dists/CyclicDist.html)

New Tools / Tool Changes
------------------------
* added support for external Spack installations in `mason external`
  (https://chapel-lang.org/docs/1.19/tools/mason/mason.html#using-spack-dependencies)
* added `:throw:/:throws:` tags to `chpldoc` for documenting thrown errors
  (see https://chapel-lang.org/docs/1.19/tools/chpldoc/chpldoc.html#documenting-functions-that-throw)
* improved `c2chapel` handling of functions with pointer arguments
* improved syntax highlighting for `vim`, `source-highlight`, and `highlight`

Interoperability Improvements
-----------------------------
* added a `c_array` type that behaves like a fixed-size C array
  (see https://chapel-lang.org/docs/1.19/technotes/extern.html#c-array-t-n)
* added the ability to rename external variables and fields
  (e.g., `extern "x" var c_x: c_int;` lets `x` in C be called `c_x` in Chapel)
* added support for passing Chapel-unique array types opaquely to/from Python/C
  (see https://chapel-lang.org/docs/1.19/technotes/libraries.html#arrays)
* added support for literal default arguments in routines exported to Python
  (see https://chapel-lang.org/docs/1.19/technotes/libraries.html#argument-default-values)
* added `c_ptr` Python interop arguments using NumPy arrays or ctypes pointers
  (see https://chapel-lang.org/docs/1.19/technotes/libraries.html#c-ptr-arguments)
* added initial support for Fortran interoperability including 1D array passing
  (see https://chapel-lang.org/docs/1.19/technotes/fortranInterop.html)
* enabled casts between `c_string` and `c_ptr(int(8))`/`c_ptr(uint(8))`
* improved extern-block support for structs with fixed-size arrays

Performance Optimizations/Improvements
--------------------------------------
* optimized iteration for Block-distributed arrays
* prototyped parallelized 1D scans for block- and default-distributed arrays
  (use `-senableParScan` to enable this prototype)
* enabled parallelism for statement-level promotions
* improved the performance of string comparisons for local strings
* optimized internal testAndSet locks
* improved target-compiler optimizations for non-x86 architectures
* added compiler hints to the LLVM back-end for non-aliasing arrays
* enabled more floating point optimizations in the LLVM back-end
* added vectorization hints for the LLVM back-end

Cray-specific Performance Optimizations/Improvements
----------------------------------------------------
* optimized certain forall-loops to take advantage of unordered communication
  (see `--optimize-forall-unordered-ops` in `man chpl`)
* optimized oversubscribed communication under `ugni`
* optimized small data transfers and fetching atomics under `ugni`
* optimized remote task spawning under `ugni`
* optimized unordered communication under `ugni`

Memory Improvements
-------------------
* addressed memory leaks in sparse domains, reductions, and arrays-of-arrays
* fixed a memory leak when throwing errors from nested coforalls
* fixed a memory leak when popping the last element from an array
* optimized away some unnecessary memory allocations

Documentation
-------------
* added a primer for special methods on classes and records
  (see https://chapel-lang.org/docs/1.19/primers/specialMethods.html)
* added primers on records and tuples
  (see https://chapel-lang.org/docs/1.19/primers/records.html
   and https://chapel-lang.org/docs/1.19/primers/tuples.html)
* added a technical note on `nil`-checking
  (see https://chapel-lang.org/docs/1.19/technotes/nilChecking.html)
* added a technical note on lifetime checking
  (see https://chapel-lang.org/docs/1.19/technotes/lifetimeChecking.html)
* removed `examples/programs` from the release due to low perceived value
* added documentation for `CHPL_LIB_PIC`
  (see https://chapel-lang.org/docs/1.19/technotes/libraries.html#static-and-dynamic-libraries)
* documented that Chapel is now primarily tested using UTF-8
  (see https://www.chapel-lang.org/docs/1.19/usingchapel/chplenv.html#character-set)
* added documentation of the `:throw:/:throws:` tags in `chpldoc`
  (see https://chapel-lang.org/docs/1.19/tools/chpldoc/chpldoc.html#documenting-functions-that-throw)
* documented some throwing functions in the standard library documentation
* fixed typos in, and generally improved, various forms of documentation

Example Codes
-------------
* added a variant of HPCC RA that uses unordered atomics
  (see `examples/benchmarks/hpcc/variants/ra-unordered-atomics.chpl`)
* updated `ra-atomics.chpl` to support oversubscription via a `config`
  (see `examples/benchmarks/hpcc/ra-atomics.chpl`)
* updated chameneos benchmarks to use direct initializations of atomics
  (see `examples/benchmarks/shootout/chameneos*.chpl`)
* updated CLBG benchmarks to refer to the proper URL
  (see `examples/benchmarks/shootout/*.chpl`)
* other minor updates to keep up with Chapel evolution

Portability
-----------
* added an Open Fabrics Interfaces libfabric-based `ofi` communication layer
  (see https://chapel-lang.org/docs/1.19/platforms/libfabric.html)
* ported Chapel to MacOS Mojave
* ensured Chapel supports LLVM 7
* improved the LLVM back-end's ability to find gcc header files
* ported the LLVM back-end to 32-bit x86
* added support for urxvt terminal emulator with CHPL_COMM_USE_[G|LL]DB.
  (see https://chapel-lang.org/docs/1.19/usingchapel/debugging.html)

Cray-specific Changes and Bug Fixes
-----------------------------------
* improved `ugni` fixed heap behavior near the registration limit
  (see https://chapel-lang.org/docs/1.19/platforms/cray.html#ugni-communication-layer-and-the-heap)
* allowed specifying fixed heap size as a percentage of physical memory size
  (see https://chapel-lang.org/docs/1.19/platforms/cray.html#ugni-communication-layer-and-the-heap)
* fixed Python interoperability when using `prgenv-*` compilers
* fixed the LLVM back-end when using `prgenv-*` compilers
* improved the use of the LLVM back-end with GASNet on Cray systems
* fixed support for heterogeneous nodes with GASNet over the `aries` conduit
* fixed a bug in the `MPI` module for `ugni` which gave a compiler error

Compiler Improvements
---------------------
* added support for static linking when using the LLVM back-end
* made `--no-checks` disable several checks in range operations

Compiler Flags
--------------
* added a `--target-cpu` flag for setting `CHPL_TARGET_CPU`
* added a `--[no-]optimize-forall-unordered-ops` flag (off by default)
* removed the `--[no-]warn-constructors` flag

Error Messages / Semantic Checks
--------------------------------
* removed warnings for [op]= overloads whose LHS expressions weren't `ref`
  (e.g., `proc +=(lhs: C, rhs: C) { ... }` no longer results in a warning)
* added an error for new-expressions without argument lists
* improved error messages for initializers
* added an error for user-defined constructors
* removed the transitional warning for `new C` being borrowed by default
* added an error for `new owned borrowed C()`
* improved lifetime checking within task constructs and initializers
* fixed lifetime checking for module-scope code
* enabled lifetime checking for iterators and loop expressions
* improved errors when throwing from within `coforall`/`begin`/`on`/`cobegin`
* improved errors when throwing something that is not a subclass of `Error`
* made throwing borrowed errors into a compiler error
* added an error when a `use` statement's filters include the module itself
* improved errors when a `use` statement's filters refer to bad symbols
* improved checks that tuple destructuring is only applied to tuples
* improved const checking, for example when assigning to tuples
* improved error messages for creating a `ref` to `nil`
* improved errors for `param` loops over improper ranges
* improved errors relative to unsupported domain / partial domain queries
* added an error for applying `~` to booleans
* added an error for zippered for-loops that invoke parallel iterators
* improved error messages for compile-time `config`s
* squashed references to internal subroutines for certain classes of errors
* improved errors when exported routines return arrays without element types
* added an error for exported routines that have `where` clauses
* improved error messages for generic first-class functions
* added a warning for certain races that occur when promoting a method
* upgraded the format of internal error messages

Execution-time Checks
---------------------
* added an error when remote records are passed to external procedures

Bug Fixes
---------
* fixed support for task-private array variables
* fixed bugs in `writef()`/`readf()`/`format()` for enums without int values
* fixed bugs so that `LinearAlgebra` routines can accept rank-change slices
* made empty `Spawn.spawn()` calls throw errors instead of segfaulting
* fixed a bug when running `subprocess.wait` more than once
* resolved problems when multiple extern blocks `#include`d the same header
* resolved problems for extern blocks with mutually recursive `struct`s
* made `mason search` warn about bad manifest filenames instead of halting
* fixed a bug in which `mason doc` expected project dirs to match package names
* fixed a bug in which certain `try!` patterns caused internal errors
* fixed a bug in which I/O couldn't read integers starting with `+` like `+10`
* iterating over tuples now yields `ref` or `const ref` elements
* `on s` for `owned` or `shared` objects now runs where the object is allocated
* fixed support for forall loops over zippered loop expressions
* fixed support for forall loops over tuple expansion expressions
* enabled tuples to contain `owned` objects
* fixed a bug in range.member(range)
* fixed some bugs/inconsistencies in methods and functions on ranges
* fixed support for `cstdlib` atomics for clang and llvm compilers
* fixed some issues with `--library*` compilation using the LLVM back-end
  (see https://chapel-lang.org/docs/1.19/technotes/libraries.html#llvm)
* fixed `setchplenv` scripts for `pyenv` users
* fixed a bug in which `PATH` was not quoted in `chpldoc` & `chplvenv` builds
* fixed a problem where `setchplenv` added `' '` to MANPATH
* fixed a bug in which comparing rank-variant domains generated a compiler error
* fixed a bug in which global extern consts were incorrectly localized
* fixed a problem with override-checking for functions with `where`-clauses
* fixed a problem with control flow analysis in functions with `catch` blocks
* converted memcpy() calls that may overlap to use memmove()
* fixed some incorrect buffer sizes found using gcc 8.2 static analysis
* fixed target-compiler optimization flags for non-x86 architectures
* fixed problems caused by toggling `CHPL_LLVM` while running `make`
* fixed an error in the documentation of `string.match`

Third-Party Software Changes
----------------------------
* upgraded GASNet to GASNet-EX version 2018.9.0
* upgraded hwloc to version 1.11.11
* upgraded qthreads to version 1.14
* upgraded re2 to commit 0a6326b
* upgraded LLVM to version 7.0.1 and began storing it in an unpacked manner
  (see https://www.chapel-lang.org/docs/1.19/usingchapel/chplenv.html#chpl-llvm
   and https://www.chapel-lang.org/docs/1.19/technotes/llvm.html)
* upgraded fltk to version 1.3.5

Launchers
---------
* added support for setting a node access type for slurm launchers
  (see https://chapel-lang.org/docs/1.19/usingchapel/launcher.html#common-slurm-settings)
* improved propagation of environment variables for Perl-based GASNet launchers
* improved how processes are mapped to locales for gasnetrun-based launchers
* improved portability and usability of slurm-gasnetrun_ibv launcher

Testing System
--------------
* added Python 3 support to the testing infrastructure
* reorganized test output to first show all stdout, then all stderr
* fixed support for building our test virtualenv within an existing virtualenv
* removed our dependency on `subprocess32`

Developer-oriented changes: Module changes
------------------------------------------
* removed all instance of `domain._value` from the `LinearAlgebra` module

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* improved GASNet build speeds
* `make` no longer runs GMP and RE2 builds at the same time
* improved the build configuration of the bundled LLVM sources
* the bundled LLVM build now sets `GCC_INSTALL_PREFIX` appropriately
* the bundled LLVM build now builds shared objects by default
* the bundled LLVM build includes `dump` methods when `CHPL_DEVELOPER` is set
* enabled asserts in bundled LLVM build when `CHPL_DEVELOPER` or `ASSERTS` set
* source directories can now contain builds for several architectures

Developer-oriented changes: Compiler Flags
------------------------------------------
* added `--library-fortran[-name]` flags in support of Fortran interoperability

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* eliminated all remaining uses of `chpl__tryToken`
* dramatically improved the representation of forall intents within the IR
* modernized the representation of reduce expressions
* improved the creation of promotion iterators and made them more uniform
* improved the handling of outer variables in for-/forall-expressions
* fixed the scope resolution of LoopExprs
* fixed issues with reduce task intents
* improved the IR representation for default initialization
* made some internal module params and type aliases available earlier
* refactored the parser to simplify the declaration of variables
* made deserialization errors use cnames when compiling in --devel mode
* variables declared in scopeless blocks now destroyed at end of parent block
* in module-scope code, call temps are now module-scope variables
* removed use of `_defaultOf` for numeric types or classes
* removed `dtSymbol`
* enabled prototype integration with the Region Vectorizer for LLVM
* fixed problems with `--llvm-wide-opt` with LLVM 8
* `--llvm --specialize` compilation generates target metadata for functions
* migrated `chpl_getPrivatizedCopy()` to module code to improve LLVM performance
* removed special handling for `cabs()` with `--llvm`
* `--llvm --gen-ids` compilation now adds LLVM metadata storing Chapel AST ids
* added experimental support for emitting `llvm.invariant.start`
* source files supporting `--llvm` are now in `compiler/llvm`
* `inferConstRefs` now runs on both single-locale and multi-locale compilation
* removed `reprivatizeIterators`

Developer-oriented changes: Runtime improvements
------------------------------------------------
* restructured array memory allocation interfaces
* moved communication diagnostics from comm layers to shared, common code
* moved strided bulk transfer from comm layers to shared, common code
* improved speed and ease of gathering performance statistics under `ugni`
* added tasking layer interface queries for fixed-thread-count implementations
* addressed compilation errors with `slurm-gasnetrun_ibv` launcher
* unified the implementation of all gasnetrun-based launchers

Developer-oriented changes: Testing System
------------------------------------------
* fixed a problem with integer division in `sub_test`
* removed `sub_test` transient MPP error, which interfered with prediff filter


version 1.18.0
==============

Twenty-first public release of Chapel, September 20, 2018

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* language highlights:
  - deprecated constructors in favor of initializers for classes and records
  - added managed classes and lifetime checking to help with memory management
  - added an `override` keyword to avoid mistakes when overriding class methods
  - made loop expressions and promoted calls preserve shape for domains/arrays
  - added support for task-private variables in forall loops
  - improved enums with respect to casts, coercions, comparisons, and ranges
  - added initial support for UTF-8 strings
* standard library / package highlights:
  - added `HDF5` and `NetCDF` modules supporting I/O for those file formats
  - added support for buffered versions of non-fetching atomic operations
  - improved the `LinearAlgebra` module in terms of performance and usability
* performance highlights:
  - optimized `sync` variables and concurrent I/O for `qthreads` tasking
  - optimized big data transfers, non-fetching atomics, and on-clauses on Crays
  - improved memory utilization and reduced fragmentation for task stacks
* interoperability highlights:
  - added `mason` support for external system and `Spack` package dependencies
  - significantly improved support for creating libraries for use from C
  - added initial support for exporting library routines for use with Python
* additional highlights:
  - reduced compilation times for most programs
  - added several other features and improvements to the `mason` package manager
  - improved support for ARM systems
  - added a new version of the HPCC RA benchmark that uses buffered atomics
  - added scripts to help build and maintain Chapel configurations
  - numerous error message improvements, bug fixes, and feature improvements

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* deprecated constructors in favor of initializers which are now used by default
  (see 'Class Initializers' and 'User-Defined Initializers' in the spec)
* for a class C, `new C(...)` is now equivalent to `new borrowed C(...)`
  (see 'Class Types' in the 'Classes' chapter of the language specification)
* when overriding a class method, the `override` keyword must now be used
  (see 'Overriding Base Class Methods' in the spec's 'Classes' chapter)
* removed support for coercions from enums to integers
* associative domains of enums are no longer fully populated by default
* deprecated the interpretation of `:` within where clauses as a subtype query
* deprecated support for casting from numeric types to `c_string`
* deprecated support for extern classes
* made enums no longer have integer values unless specified by the user
  (see `Enumerated Types` in the language specification)
* disabled casts from enums to integers when they have no associated value
  (e.g., `enum color { red, green, blue};`  ...`red:int`... is no longer valid)

New Features
------------
* added support for managed class objects via `owned`, `shared`, and `borrowed`
  (see 'Class Types' in the 'Classes' chapter of the language specification)
* added an `unmanaged` keyword for manually managing class objects
  (see 'Class Types' in the 'Classes' chapter of the language specification)
* added a `.borrow()` method to all flavors of classes
  (see 'Class Lifetime and Borrows' in the 'Classes' chapter of the spec)
* added a compiler lifetime checker to flag borrows that refer to freed objects
* added support for task-private variables in forall loops
  (see 'Task-Private Variables' in the 'Data Parallelism' chapter of the spec)
* extended Chapel strings to support UTF-8 characters
  (see https://chapel-lang.org/docs/1.18/builtins/String.html)
* added support for ranges with `enum` and `bool` indices
  (see 'Range Types' in the 'Ranges' chapter of the language specification)
* added support for comparison operators (`<`, `<=`, `>=`, `>`) on enums
* added support for variables and fields with generic declared types
  (see 'Variable Declarations' in the language spec's 'Variables' chapter)
* added support for querying subtype relationships via `<`, `<=`, `>`, `>=`
  (see https://chapel-lang.org/docs/1.18/builtins/UtilMisc_forDocs.html#UtilMisc_forDocs.isSubtype)

Feature Improvements
--------------------
* significantly improved the robustness and capabilities of initializers
* made loop expressions and promoted expressions preserve shape (domain)
  (see `For[all] Expressions` and `Array Promotion of Scalar Functions` in spec)
* added support for specifying extern/export names via param string expressions
  (see 'Calling External Functions' in the spec's 'Interoperability' chapter)
* added support for min/max reduce intents on coforall loops
* added support for reduce intents on coforall+on loops
* added support for array return types with unspecified domains / element types
  (e.g., `proc foo(): []` says that `foo()` returns an array but little else)
* added support for dynamic dispatch on variadic (varargs) methods
* added support for rectangular domains and arrays of `enum` or `bool` idxType
* updated `--memStats` to work properly in multi-locale runs
  (see https://chapel-lang.org/docs/1.18/modules/standard/Memory.html#Memory.printMemAllocStats)
* added subtyping for owned and shared classes
  (see https://chapel-lang.org/docs/1.18/builtins/SharedObject.html#coercions-for-shared
   and https://chapel-lang.org/docs/1.18/builtins/OwnedObject.html#coercions-for-owned)
* added support for assigning `nil` to owned and shared classes
* made the default intent for formals declared with owned/shared type `const in`
* made forall loops borrow outer owned and shared class objects by default
* improved the appearance of shared objects printed via `writeln()`
* added support for argument types containing nested generic or query types
  (see 'Formal Arguments of Partially Generic Type' in the 'Generics' chapter)
* improved the instantiation of generic arguments when coercions are involved
* added support for infix bool operators to throw
* improved `isSubtype()` and `isProperSubtype()` to handle cases that can coerce
* extended `isSubtype()` and `isProperSubtype()` to work with generic types
  (see https://chapel-lang.org/docs/1.18/builtins/UtilMisc_forDocs.html#UtilMisc_forDocs.isSubtype)
* improved disambiguation and visibility rules when resolving function calls
  (see 'Function Visibility in Generic Functions' in the 'Generics' chapter)
* made `in` arguments more consistent with variable initialization
  (see 'Argument Intents' in the language specification's 'Procedures' chapter)
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/13.rst)
* added support for casts from `c_void_ptr` to `c_ptr`

Removed Features
----------------
* removed implicit `outer` field on nested types with initializers
* removed the `UtilReplicatedVar` module (deprecated in 1.17)
* disabled non-`ref` forall intents when using recursive parallel iterators

Standard Modules / Library
--------------------------
* increased the use of error-handling in standard modules
* deprecated 'out error' patterns in libraries in favor of throwing versions
* converted some non-fatal checks from using `halt()` to `warning()`
* deprecated `Owned(C)` and `Shared(C)` in favor of `owned C` and `shared C`
  (see 'Class Types' in the 'Classes' chapter of the language specification)
* added type query routines for `owned`, `shared`, `unmanaged`, and `borrowed`
  (see https://chapel-lang.org/docs/1.18/modules/standard/Types.html#Types.isOwnedClass)
* extended string methods and library routines to support UTF-8 strings
  (see https://chapel-lang.org/docs/1.18/builtins/String.html)
* added `array.front()`/`array.back()` to get the first/last element of an array
  (see https://chapel-lang.org/docs/1.18/builtins/ChapelArray.html#ChapelArray.back)
* made `array.pop_front()` and `array.pop_back()` return the removed element
  (see https://chapel-lang.org/docs/1.18/builtins/ChapelArray.html#ChapelArray.pop_back)
* added a new string `join()` overload that accepts iterator expressions
* added `expandVars()` to the `Path` module
  (see https://chapel-lang.org/docs/1.18/modules/standard/Path.html#Path.expandVars)
* updated `makeRandomStream()` to return an owned random stream object
  (see https://chapel-lang.org/docs/1.18/modules/standard/Random.html#Random.makeRandomStream)
* converted blocking calls in `Spawn` to yielding non-blocking loops
* made the Sync `Barrier` reusable

Package Modules
---------------
* added an `HDF5` module for reading/writing files using the HDF5 interface
  (see https://chapel-lang.org/docs/1.18/modules/packages/HDF5.html)
* added a `NetCDF` module for reading/writing files using the NetCDF interface
  (see https://chapel-lang.org/docs/1.18/modules/packages/NetCDF.html)
* added buffered versions of non-fetching atomic operations
  (see https://chapel-lang.org/docs/1.18/modules/packages/BufferedAtomics.html)
* added support for singular value decomposition to the `LinearAlgebra` module
  (see https://chapel-lang.org/docs/1.18/modules/packages/LinearAlgebra.html#LinearAlgebra.svd)
* enabled using `LinearAlgebra` without having `BLAS/LAPACK` installed
  (see https://chapel-lang.org/docs/1.18/modules/packages/LinearAlgebra.html#compiling-with-linear-algebra)
* added configs `blasImpl` and `blasHeader` to `BLAS` to select backend
  (see https://chapel-lang.org/docs/1.18/modules/packages/BLAS.html#BLAS.blasImpl)
* added configs `lapackImpl` and `lapackHeader` to `LAPACK` to select backend
  (see https://chapel-lang.org/docs/1.18/modules/packages/LAPACK.html#LAPACK.lapackImpl)
* deprecated `BLAS` config `isBLAS_MKL` in favor of using `blasImpl`
* made `LinearAlgebra` sparse matrices store indices in unsorted order
* replaced string options with enumerations in the `Crypto` module
  (see https://chapel-lang.org/docs/1.18/modules/packages/Crypto.html#Crypto.Digest
   and https://chapel-lang.org/docs/1.18/modules/packages/Crypto.html#Crypto.CryptoChainMode)
* updated the `Crypto` module to return owned pointers
* updated the sentinel value returned by `Search.linearSearch()`
  (see https://chapel-lang.org/docs/1.18/modules/packages/Search.html#Search.linearSearch)
* converted the `TOML` module over to use error-handling
  (see https://chapel-lang.org/docs/1.18/modules/packages/TOML.html#TOML.TomlError)

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* added support for storing the indices of sparse CS domains in unsorted order
  (see https://chapel-lang.org/docs/1.18/modules/layouts/LayoutCS.html#LayoutCS.CS)
* added missing core operations to sparse Block-distributed domains

New Tools / Tool Changes
------------------------
* added support for non-Chapel system and Spack packages in mason
  (see https://chapel-lang.org/docs/1.18/tools/mason/mason.html#using-system-dependencies
   and https://chapel-lang.org/docs/1.18/tools/mason/mason.html#using-spack-dependencies)
* added `mason add` and `mason rm` for managing dependencies
  (see https://chapel-lang.org/docs/1.18/tools/mason/mason.html#using-chapel-dependencies)
* added support for tests and examples within mason
  (see https://chapel-lang.org/docs/1.18/tools/mason/mason.html#testing-your-package
   and https://chapel-lang.org/docs/1.18/tools/mason/mason.html#creating-and-running-examples)
* added `mason run --release` flag to specify release vs. debug modes
* added `--no-update` flag for better offline support in mason
* added support for calling mason commands from any directory within a project
* added support for checking last modified timestamp before building in mason
* improved bash tab completion to offer developer flags in developer mode only
* added `chpldoc` support for new keywords: `overrides`, `owned`, `shared`, ...
* squashed `chpldoc` documentation of compiler-generated symbols
* added Python 3 support for chpldoc

Interoperability Improvements
-----------------------------
* significantly improved support for calling exported Chapel routines from C
  (see https://chapel-lang.org/docs/1.18/technotes/libraries.html)
* added initial support for exporting library routines for use from Python
  (see https://chapel-lang.org/docs/1.18/technotes/libraries.html#using-your-library-in-python)
* `--library` compilations now generate .h files and Makefiles for ease-of-use
  (see https://chapel-lang.org/docs/1.18/technotes/libraries.html#using-your-library-in-c)
* `lib` is now prepended to filenames generated by the `--library` flags
* files generated by `--library` compilations are now stored in a subdirectory
* `--library` compiles now auto-export module (de)initialization routines
* added initial support for exported functions taking/returning 1D arrays

Performance Optimizations/Improvements
--------------------------------------
* improved the performance of sync variables and concurrent I/O under `qthreads`
* optimized away communication for queries to the `id` field of a locale
* optimized sparse matrix addition and multiplication in `LinearAlgebra`
* parallelized a loop in array reallocation
* reduced overheads in the `Search.linearSearch()` routine

Cray-specific Performance Optimizations/Improvements
----------------------------------------------------
* improved performance of large transfers under `ugni`
* optimized non-fetching network atomics
* improved on-stmt performance for `ugni`
* optimized the Atomic `Barrier` for network atomics
* mapped `atomic real(32)` operations to network atomics

Memory Improvements
-------------------
* reduced memory leaks related to initializers
* fixed leaks in the `DateTime` and `Regexp` modules
* fixed memory leaks in `array.pop_front()`/`.pop_back()` and `list.pop_front()`
* eliminated memory fragmentation for task stacks hosted on hugepages
* reduced the amount of memory preemptively allocated for task stacks

Compiler Flags
--------------
* added `--warn-unstable` to flag code which uses features that are in flux
* added `--warn-constructors` flag to warn against user-defined constructors
* added a `--[no-]optimize-range-iteration` flag to control range optimizations
* made `--warn-const-loops` and `--warn-special` into developer flags
* made `--warn-domain-literal` and `--warn-tuple-iteration` into developer flags

Documentation
-------------
* added descriptions of managed and unmanaged classes to the classes primer
  (see https://chapel-lang.org/docs/1.18/primers/classes.html)
* improved the mason documentation
  (see https://chapel-lang.org/docs/1.18/tools/mason/mason.html)
* added documentation for the `TOML` module
  (see https://chapel-lang.org/docs/1.18/modules/packages/TOML.html)
* improved the documentation for the `LinearAlgebra` module
  (see https://chapel-lang.org/docs/1.18/modules/packages/LinearAlgebra.html)
* documented `bigint.divexact` in the `BigInteger` module
  (see https://chapel-lang.org/docs/1.18/modules/standard/BigInteger.html#BigInteger.bigint.divexact)
* simplified the version selection button in Chapel's online documentation
  (see the menu in the upper left at https://chapel-lang.org/docs/)

Example Codes
-------------
* added a new version of the HPCC RA benchmark that uses `BufferedAtomics`
  (see `$CHPL_HOME/examples/benchmarks/hpcc/variants/ra-buff-atomics.chpl`)
* updated many of the example codes to make use of managed classes
* updated example codes with respect to using the `override` keyword
* updated example codes with respect to changes to `enum` types
* updated example codes with respect to shape-preservation semantics
* clarified how vector operations modify domains in the arrayVectorOps primer
  (see https://chapel-lang.org/docs/1.18/primers/arrayVectorOps.html)
* updated our port of LCALS to use relative, rather than absolute, differences
* fixed an off-by-one bug in a loop bound in our port of LCALS

Portability
-----------
* updated the target compiler interface to handle 64-bit ARM, Skylake, KNL
  (see https://chapel-lang.org/docs/1.18/usingchapel/chplenv.html#chpl-target-arch)
* added support for the Allinea compiler on ARM systems
* improved LLVM back-end support for ARM systems
* improved our counting of compute node CPUs, with and without `hwloc`
* improved the `Crypto` module to work on platforms with OpenSSL 1.1
* ported the `Math` module's Bessel functions to `darwin`
* improved the portability of the code base to gcc 7.x
* resolved a build failure on certain systems with CUDA installed

Cray-specific Changes
---------------------
* made `ibv` the default substrate for `cray-cs` systems
* increased the number of memory regions while allowing for override for `ugni`
  (see https://chapel-lang.org/docs/1.18/platforms/cray.html#ugni-communication-layer-registered-memory-regions)
* added memory tracking of dynamically registered arrays for `ugni`
* added hugepage-related environment consistency checks for `ugni`
* fixed support for large non-blocking gets under `ugni`
* made LLVM work with dynamic linking on Cray XC systems
* suppressed an error about incompatible casting for `ZMQ` with PrgEnv-cray

Error Messages / Semantic Checks
--------------------------------
* significantly improved error messages for initializers
* added a compile-time error when declaring recursive records
* improved error messages to show types of non-default numeric param values
* made use-before-def error messages print to the symbol's definition point
* added an error message when we can't determine an iterator's yield type
* improved error messages for accessing tuples of types out of bounds
* improved const checking for tuple arguments
* added an error for passing too many arguments to a tuple-type index expression
* added a helpful error message when throwing an error type without `new`
* added an error message for applying `.type` to a type
* improved checking of intents on overridden methods
* improved the error message for library compilations using multiple modules
* added a compile-time error for exported functions using strings
* added an error message if `CHPL_COMM_SUBSTRATE` isn't detected by `GASNet`
* improved error-handling for certain operations on an invalid channel
* removed a warning regarding returned iterators being promoted to arrays

Execution-time Checks
---------------------
* added a better error for running a `--local` program on more than 1 locale
* added runtime memory exhaustion reporting for `CHPL_TASKS=qthreads`

Bug Fixes
---------
* fixed a number of bugs related to initializers
* fixed several bugs with `forwarding` fields
* fixed several problems with calls qualified by explicit module names
* fixed a bug in which private overloads could completely shadow public ones
* fixed a bug in which qualified 'use's hid methods with the same name
* fixed support for the swap operator (`<=>`) on owned and shared objects
* fixed problems with records containing `owned` class fields
* fixed problems with compiler-added copies for `owned` variables
* adjusted `make install` to enable the use of the setchplenv scripts
* fixed a bash tab completion bug for paths involving `~`
* fixed problems with `--llvm` in `make install` builds
* fixed problems with certain GASNet configurations using `--llvm`
* fixed `--llvm` compiles with `CHPL_TARGET_ARCH=unknown`
* fixed incorrect bulk-transfers between reindexed arrays
* fixed support for `atomic real(32)` on platforms with network atomics
* fixed bugs with operators and intents in `BigInteger` library routines
* fixed return type of casts from `bigint` to `integral`/`real`
* fixed bugs in `IO.readline()`
* fixed bugs with exists(""), isFile(""), isDir(""), isLink(""), isMount("")
* fixed problems in `FileSystem.copyFile` for files not ending in newlines
* fixed a bug where `Search` sometimes used the wrong bounds for strided arrays
* fixed `DynamicIterators` for 0-length ranges and domains
* fixed a bug in mergeSort's documentation claiming it was in-place
* fixed a bug when spawning a task with stdout set to PIPE and stderr to STDOUT
* fixed reported line numbers for non-generic class allocations
* fixed a bug with `createRandomBuffer()` in the `Crypto` module
* fixed potential `export`/`extern` naming conflicts in the generated code
* fixed a bug with a function called from forall loop nested in an on-statement
* fixed a double free with variable initialization and nested throwing calls
* fixed an issue with with the extern block feature
* fixed compiler crashes when compiling highly nested expressions
* fixed a bug with promoted casts from an `enum` type to itself
* fixed a bug in `chpldoc` for the rendering of forwarding fields
* fixed a problem where the compiler lost track of a point of instantiation
* improved function resolution to resolve return types only for legal candidates
* improved our running task counter for remote tasks
* fixed a bug in which quoted spaces were mishandled in `setchplenv.fish`
* fixed a bug in which `printchplenv -all` did not include defaults
* fixed a bug in which modules were not built as a dependency to mason
* fixed how we detect local spawning for setting locale names
* fixed an extremely rare deadlock during `qthreads` shutdown
* fixed an initialization order bug with `qthreads` and `CHPL_ATOMICS=locks`
* fixed buffer overflows found by the extra checking built into gcc 8.1
* fixed `isClassType()` to return false for `c_ptr` types

Compiler Performance
--------------------
* improved compiler speed by reducing uses of where clauses in standard modules
* improved compiler speed by refactoring how primitives are code-generated

Packaging / Configuration Changes
---------------------------------
* added scripts to make it easier to build and maintain Chapel configurations
  (see `$CHPL_HOME/util/build_configs/README.md`)

Third-Party Software Changes
----------------------------
* upgraded GASNet to version 1.32.0
* arranged for Qthreads to use the runtime's `hwloc` topology when that exists
* third-party libraries are now position-independent when `CHPL_LIBMODE=shared`
* disabled guard pages for `arm-thunderx` under `qthreads`
* suppressed a package conflict and pip 10 warnings when building virtualenvs

Testing System
--------------
* allowed performance tests to only define a .perfcompopts file
* fixed start_test escaping of `--compopts`/`--execopts`
* improved how start_test detects whether re2 was built with valgrind support
* disabled loose matching for paratest.server command-line options
* made the output of paratest.server appear in more deterministic sorted order
* retained the temporary log files when paratest.server fails

Developer-oriented changes: Configuration changes
-------------------------------------------------
* improved `MPI_CC` and `MPI_CXX` defaults for the Intel compiler
* added `CHPL_HWLOC` and `CHPL_TARGET_MEM` to qthread-specific paths
* added `JEMALLOC_LIB_DIR` to the runtime path
* reduced the dependence on `$CHPL_HOME` when computing the compiler's SHA

Developer-oriented changes: Module changes
------------------------------------------
* all internal, standard, and package modules now use initializers
* updated modules to work with the new class memory management features
* updated standard library and internal modules to utilize `override` keyword
* added a subclass of array/domain/distribution to handle external array types
* simplified the implementation of the minloc and maxloc reduction classes
* performed miscellaneous housekeeping cleanup of locale models
* removed several halts from the I/O module
* stopped exporting GMP routines

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* added `--compiler`, `--linker`, and `--linkershared` flags to `compileline`
  (see `util/config/compileline -h`)

Developer-oriented changes: Compiler Flags
------------------------------------------
* dramatically improved support for `--library` compilation
  (see 'Interoperability Improvements' above)
* added `--library-header` to rename a `--library`'s  .h file
* added `--library-dir` to specify the location for generated library files
* added `--library-makefile` to generate a Makefile for library compilations
* added `--library-python` to generate a Python module using Cython
* added `--library-python-name` to set the name of the generated Python module
* added `--ignore-user-errors` and `--stop-after-pass` compiler flags
* added the flag `--default-unmanaged` to make classes unmanaged by default
* changed the flag `--break-on-delete-id` to `--break-on-remove-id`
* removed `--user-constructor-error` flag as no longer being necessary

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* improved line number propagation for internal functions
* enabled pragmas to apply to formal arguments
* added functionality to reissue `compilerError()` messages
* changed the colors of links for user-modules in `--html` output to stand out
* AST dumps now show varargs arguments with `...`
* added an `always RVF` pragma to force remote-value forwarding on a given type
* added `chpl_anybool`, `chpl_anyimag`, and `chpl_anyreal` generic types
* replaced some uses of `Vec` with `std::vectors`
* simplified handling of forall loops and forall intents
* made LLVM alias analysis tests more tolerant of changes in name mangling
* updated `llvm.invariant.start` emission for records using initializers
* adjusted calls to `chpl__deserialize` to avoid runtime types
* compiler now prints some internal types without a leading underscore

Developer-oriented changes: Documentation improvements
------------------------------------------------------
* revamped the test system documentation to better serve new developers
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst)
* added a `check_path` script to find bad filepath references in repository
* documented the launcher interface
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/adding-a-launcher.rst)
* removed `404 error` output after successful documentation build
* removed `git clean` suggestion after failed documentation build

Developer-oriented changes: Module improvements
-----------------------------------------------
* simplified the implementation of atomic variables
* implemented experimental support for Polly optimization of Chapel arrays
* removed the unused `origin` field from DefaultRectangularArr
* added `dsiAlign*` methods to `CyclicDom` and `ReplicatedDom`

Developer-oriented changes: Runtime improvements
------------------------------------------------
* reorganized the topology layer into `none` and `hwloc` implementations
* added support for processing non-local /proc/cpuinfo files in counting CPUs
* cleaned up `ugni` implementation of network atomics
* improved the performance of comm diagnostic counters when using GASNet
* removed non-functional `qthreads` support for `--blockreport`/`--taskreport`
* removed code to set `GASNET_MAX_SEGSIZE`
* simplified implementation-specific `.h` file names for tasking and threading

Developer-oriented changes: Testing System
------------------------------------------
* avoided the use of spool files when using PBSPro via `chpl_launchcmd.py`
* added missing bracket to gengraphs messages

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* improved `CHPL_LLVM=system` support to avoid unpacking the LLVM sources
* enabled `chpl` to build with LLVM 7 pre-release
* moved GMP support out of the C runtime
* improved how runtime-oriented options are set in Qthreads builds

Developer-oriented changes: Tool improvements
---------------------------------------------
* added --break-on-id and --break-on-delete-id to `chpldoc`
* converted mason to use error-handling


version 1.17.1
==============

Update to twentieth public release of Chapel, April 2018
(see also changes below for 1.17.0)

Bug Fixes
---------
* fixed a bug in which postinit() was not called for generic records
* fixed a bug in which most launchers would not work with 'make install'
* fixed a portability problem w.r.t. our processing of /proc/cpuinfo
* fixed a bug in which LinearAlgebra.Sparse.eye() would unintentionally promote

Cray-specific Changes
---------------------
* reduced contention for polling threads for CHPL_COMM=ugni
* fixed a bug in computing the max number of comm domains for CHPL_COMM=ugni
* improved the handling of unregisterable memory segments for CHPL_COMM=ugni
* increased conservatism in IPA for CHPL_TARGET_COMPILER=cray-prgenv-cray
* fixed debug build mode for CHPL_COMM=ugni

Documentation
-------------
* simplified URLs to use https://chapel-lang/docs/ rather than .../docs/latest/
* fixed some typos and broken links in this file

Packaging Changes
-----------------
* converted our use of easy_install to pip when installing virtualenv

Third-Party Software Changes
----------------------------
* updated hwloc to 1.11.10
* fix our snapshot of fltk


version 1.17.0
==============

Twentieth public release of Chapel, April 5, 2018

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* tool / configuration highlights:
  - added support for bash tab completion for `chpl` compiler options
  - `chpl` now offers suggestions for unrecognized flags
  - changed the generated executable's name from the main module to its filename
  - improved support for `chpl`s LLVM back-end
  - added support for multiple mason repositories, including local registries
* language highlights:
  - dramatically improved support for user-defined and implicit initializers
  - added support for uninterpreted triple-quoted strings
  - usability improvements for `Owned(c)` and `Shared(c)`
  - improved coercions for `enum` and numeric types, particularly `real(32)`
* standard library / package highlights:
  - `LinearAlgebra` matrices now use 1-based indices by default
  - new Kronecker, Cholesky, eigenvector/value routines in `LinearAlgebra`
  - added the Blowfish cipher to the `Crypto` module
  - added support for joinPath(), isAbsPath(), commonPath() to the `Path` module
  - added Bessel functions to `Math` module
  - added new channel.advancePastByte(), mark(), commit(), and revert() methods
  - increased the use of error-handling in standard, package, internal modules
* performance / benchmark / memory highlights::
  - significant performance improvements when executing using Cray networks
  - added a new faster version of the reverse-complement benchmark
  - closed a number of minor memory leaks
* portability highlights:
  - ported Chapel to the Cray XC50 system with ARM processors
  - improved portability to FreeBSD and PowerPC
  - improved portability for specific versions of gcc
* documentation highlights:
  - moved Chapel's website from http://chapel.cray.com to https://chapel-lang.org
  - added a color-coded version selection menu to the online documentation

File / Directory Structure
--------------------------
* made `make install` install mason
* added the CHPL_COMM_SUBSTRATE setting to GASNet-specific paths

New Tools / Tool Changes
------------------------
* added support for bash tab completion for `chpl` compiler options
  (see https://chapel-lang.org/docs/1.17/usingchapel/compiling.html#tab-completion-for-flags)
* changed the generated executable's name from the main module to its filename
  (see the `-o` flag on https://chapel-lang.org/docs/1.17/usingchapel/man.html)
* added support for multiple mason repositories, including local registries
  (see https://chapel-lang.org/docs/1.17/tools/mason/mason.html#local-registries)
* added support for `mason help` and `mason version` commands
* `chpl` now offers suggestions for unrecognized flags
  (see error message for `chpl -fast`)
* rewrote `printchplenv` and reworked its interface and output
  (see `util/printchplenv --help`)
* rewrote `compileline`
  (see `util/config/compileline --help`)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* `in` intents now behave more like variable initialization
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/13.rst)
* ranges are now passed by `const in` intent by default
  (see 'The Default Intent' in the 'Procedures' chapter of the specification)
* disallowed implicit coercions for type arguments with a formal type specifier
  (see 'Implicit Conversions' in the specification's 'Conversions' chapter)
* changed the alignment of a non-stridable range from 0 to its low bound
  (i.e., `(3..5).alignment` is now 3 where it used to be 0)

New Features
------------
* added support for uninterpreted triple-quoted strings
  (see 'Literals' section in 'Lexical Structure' chapter of the specification)
* added support for `continue` statements within forall loops
* extended `delete` to accept arrays and list of expressions to delete
  (see 'Deleting Class Objects' in the 'Classes' chapter of the specification)
* type and param fields can now be accessed from a type variable
  (see 'Field Accesses' in the 'Classes' chapter of the specification)
* added support for forwarding methods on arrays, domains, and distributions
  (see https://chapel-lang.org/docs/1.17/technotes/dsi.html#overview)
* added an `isEmpty()` method on domains
* added support for associative domains whose index types contain ranges
* added support for subtype queries on distributions
  (see 'Methods on All Domain Types' in the 'Domains' chapter of the spec)
* added support for using GASNet's `smp` conduit

Feature Improvements
--------------------
* dramatically improved support for user-defined initializers
  (see https://chapel-lang.org/docs/1.17/technotes/initializers.html)
* improved `enum` and numeric coercions, greatly improving `real(32)` usability
  (see 'Implicit Numeric, Bool, and Enumeration Conversions' in the spec)
* made string->value casts throw rather than halt() when encountering errors
* `Owned(c)` and `Shared(c)` now coerce to type `c`
  (see https://chapel-lang.org/docs/1.17/modules/packages/OwnedObject.html)
* made `Owned(d)` coerce to `Owned(c)` when `c` is a superclass of `d`
  (see https://chapel-lang.org/docs/1.17/modules/packages/OwnedObject.html)
* writing out an `Owned(c)` now simply prints the `c` object
* improved support for casting arrays to strings
* made clear() for an array of records call the records' deinitializers
* added support for defining multiple config types in a single statement
  (see 'Type Aliases' in the 'Types' chapter of the language specification)
* made error-handling propagate through forwarded methods
* added support for invoking recursive parallel iterators via forall loops
* enabled wide pointers to be cast to `c_void_ptr`
* improved support for enums with non-trivial initialization expressions
* improved default argument handling

Removed Features
----------------
* removed the `msg` field from the `Error` class
* removed deprecated `Barrier` module in favor of `Barriers`
* removed deprecated `LayoutCSR` module in favor of `LayoutCS`
* removed `ReplicatedDist` in favor of `Replicated`

Standard Modules / Library
--------------------------
* added Bessel functions to `Math` module
  (see https://chapel-lang.org/docs/1.17/modules/standard/Math.html#Math.j0)
* added support for joinPath(), isAbsPath(), commonPath() to the `Path` module
  (see https://chapel-lang.org/docs/1.17/modules/standard/Path.html)
* added support for channel.advancePastByte() to read until a particular byte
  (see https://chapel-lang.org/docs/1.17/modules/standard/IO.html#IO.channel.advancePastByte)
* added versions of channel.mark(), commit(), revert() when locking==false
  (see https://chapel-lang.org/docs/1.17/modules/standard/IO.html#IO.channel.mark)
* improved support for I/O on `date`, `time`, and `datetime` types in `DateTime`
  (see https://chapel-lang.org/docs/1.17/modules/standard/DateTime.html#DateTime.date.readWriteThis)
* made DateTime.time.tzinfo `Shared` for automated memory management
  (see https://chapel-lang.org/docs/1.17/modules/standard/DateTime.html#DateTime.time.init)
* array `push*` methods now use `in` intents to improve their use with Owned
  (see https://chapel-lang.org/docs/1.17/builtins/ChapelArray.html#ChapelArray.push_back)
* added `string.size` overload for `string.length`
  (see https://chapel-lang.org/docs/1.17/builtins/String.html#String.string.size)
* updated several modules to use error handling rather than `try!` / `halt()`
* made the `Buffers` module into a package module
  (see https://chapel-lang.org/docs/1.17/modules/packages/Buffers.html)
* made the `UtilReplicatedVar` module into a package module `ReplicatedVar`
  (see https://chapel-lang.org/docs/1.17/modules/packages/ReplicatedVar.html)

Package Modules
---------------
* `LinearAlgebra` matrices now use 1-based indices by default
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra.html#linear-algebra-interface)
* added Kronecker product, `kron()`, to `LinearAlgebra` module
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra.html#LinearAlgebra.kron)
* added Cholesky factorization routine to `LinearAlgebra` module
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra.html#LinearAlgebra.cholesky)
* added eigenvalue/eigenvector solvers to `LinearAlgebra` module
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra.html#LinearAlgebra.eigvals)
* added support for `eye()` to `LinearAlgebra` for CS* domains
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra/Sparse.html#Sparse.eye)
* added the Blowfish cipher to the `Crypto` module
  (see https://chapel-lang.org/docs/1.17/modules/packages/Crypto.html#Crypto.Blowfish)
* added a new scalable barrier across all locales
  (see https://chapel-lang.org/docs/1.17/modules/packages/AllLocalesBarriers.html)
* added binaryInsertionSort to the `Sort` module
  (see https://chapel-lang.org/docs/1.17/modules/packages/Sort.html#Sort.binaryInsertionSort)
* added `--debugTomlReader` flag for `TOML.TomlReader` submodule

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* added the ability for sparse CS domains to have a sparse parent domain
* added support for querying the stridability of sparse domains
  (see https://chapel-lang.org/docs/1.17/builtins/ChapelArray.html#ChapelArray.stridable)
* added support for strided Block-sparse domains and arrays
* redesigned the bulk-transfer interface to reduce its complexity
  (see https://chapel-lang.org/docs/1.17/technotes/dsi.html#phase-4-bulk-transfer-interface)

Interoperability Improvements
-----------------------------
* extern blocks now support #defines containing casted literals
  (see https://chapel-lang.org/docs/1.17/technotes/extern.html#defines)

Performance Optimizations/Improvements
--------------------------------------
* improved performance of sparse matrix-matrix multiplication in `LinearAlgebra`
* improved remote value forwarding optimization for types with initializers
* reduced wide-pointer overhead for domains and distributions
* re-enabled and improved --llvm-wide-opt optimizations
* improved the performance of the `Barriers` module by using processor atomics
* improved execution start up times under `ugni`
* increased the amount of communication concurrency permitted for `ugni`
* significantly improved scalability of `coforall ... do on ... {}` under `ugni`
* increased the rate that on-stmts that can be processed under `ugni`
* improved performance of puts/gets for GASNet over the `aries` conduit

Memory Improvements
-------------------
* closed a number of memory leaks
* significantly improved iterator memory management
* optimized away storage of alignment-related fields from non-stridable ranges
* fixed some incorrect memory descriptors

Compiler Flags
--------------
* removed support for the `--wide-pointers` flag

Documentation
-------------
* moved Chapel's website from http://chapel.cray.com to https://chapel-lang.org
  (see https://chapel-lang.org)
* added a color-coded version selection menu to the online documentation
  (see the upper-left corner of https://chapel-lang.org/docs/)
* simplified the URLs for the current release's documentation
  (see https://chapel-lang.org/docs/)
* documented the built-in Error types
  (see https://chapel-lang.org/docs/1.17/builtins/ChapelError.html)
* updated function overload disambiguation rules in the language specification
  (see 'Determining More Specific Functions' in the 'Procedures' chapter)
* described combining promotion and default arguments in the specification
  (see 'Promotion' in the 'Data Parallelism' chapter of the specification)
* documented type arguments with type specifiers
  (see 'Formal Type Arguments' in the 'Generics' chapter of the specification)
* improved documentation for `-f` configuration file flag
  (see https://chapel-lang.org/docs/1.17/usingchapel/executing.html#configuration-file)
* improved documentation for which domain maps are supported by `LinearAlgebra`
  (see https://chapel-lang.org/docs/1.17/modules/packages/LinearAlgebra.html#linear-algebra-interface)
* added m4 to Chapel's prerequisites list (required for GMP)
  (see https://chapel-lang.org/docs/1.17/usingchapel/prereqs.html)
* documented `range.size`
  (see https://chapel-lang.org/docs/1.17/builtins/ChapelRange.html#ChapelRange.range.size)
* documented `compileline --compile-c++` in the libraries documentation
  (see https://chapel-lang.org/docs/1.17/technotes/libraries.html)
* added missing documentation for `dsiAssignDomain`
  (see https://chapel-lang.org/docs/1.17/technotes/dsi.html)
* updated `mason` documentation to reflect new features
* fixed dead links within documentation
* fixed a number of typos

Example Codes
-------------
* added a new faster version of the reverse-complement benchmark
  (see examples/benchmarks/shootout/revcomp-fast.chpl)
* updated example codes with respect to initializer changes
* converted leader-follower iterators in SSCA#2 into standalone iterators
  (see examples/benchmarks/ssca2/)
* fixed a timing bug in our implementation of ISx when using multiple trials
  (see examples/benchmarks/isx/isx.chpl)
* fixed a bug in the LCALS inner_prod loop kernel
  (see examples/benchmarks/lcals/)
* removed the explicit version of MiniMD from the release

Locale Models
-------------
* converted locale models to use initializers

Portability
-----------
* made CHPL_LLVM=system more portable and added support for Mac Homebrew
  (see https://www.chapel-lang.org/docs/1.17/technotes/llvm.html)
* improved portability to FreeBSD and PowerPC
* improved the portability of computing available memory on a locale
* made INFINITY and NAN independent of bit patterns
* made c2chapel Makefile POSIX-compliant
* improved portability of code w.r.t. certain versions of gcc and libmvec
* worked around a gcc bug by ensuring the last line in a file is not an #include

Cray-specific Changes
---------------------
* ported Chapel to the Cray XC50 system with ARM processors
  (see https://www.chapel-lang.org/docs/1.17/platforms/cray.html)
* made `ugni` registered heap extensions dynamic, improving NUMA locality
  (see https://chapel-lang.org/docs/1.17/platforms/cray.html#ugni-communication-layer-and-the-heap)
* raised the limit on Aries NIC resource usage for `ugni`
  (see https://chapel-lang.org/docs/1.17/platforms/cray.html#communication-layer-concurrency)
* improved upon `Bus error` messages when array allocation fails for `ugni`
* improved backwards compatibility of `ugni` with respect to chained operations
* improved Cray XC code to pass stricter requirements of Clang

Platform-specific Changes
-------------------------
* fixed a bug causing control-C to hang the compiler on Mac OS X
* improved portability of re2 for Cygwin

Syntax Highlighting
-------------------
* added error-handling constructs to syntax highlighters
* added `prototype` modules to syntax highlighters

Error Messages / Semantic Checks
--------------------------------
* improved parser errors to support multiple errors
* added an error message when trying to `use GMP` for CHPL_GMP=none
* added user errors for `param` arguments with a non-`param` default value
* added a user error for `yield` and `return` statements at module scope
* added a user error for mismatched default values vs. formal types
* added an error when mixing constructors and initializers in a class hierarchy
* added a hint for initializers improperly recognized as copy initializers
* significantly improved error messages for initializer problems
* improved error messages for improper break/continue/return in forall loops
* improved error messages for out-of-bounds tuple element accesses
* improved an error message for a missing module / enum to name the symbol
* improved error messages for bad entries in config files
* added an error for creating a `ref` to a type
* improved an error message when zipping a tuple with a multi-dimensional object
* disallowed throwing from within defer statements and deinit() routines
* improved error messages for unimplemented features in parallel iterators
* improved error messages for assignments within conditionals
* improved errors for arguments with inferred types and conditional defaults
* improved errors for functions returning generic type variables
* fixed the path to `multilocale.rst` in runtime error messages

Execution-time Checks
---------------------
* added bounds checking for indices as they're added to default sparse domains
* improved an error when trying to open a directory

Bug Fixes
---------
* fixed a bug in which `mason run` used directory name as project name
* made `mason` exit cleanly if a build fails on `mason run --build`
* fixed a bug in the interaction between `coforall` and `try!`
* fixed a bug in which `try` and `return` could result in garbage return values
* fixed a bug in which non-returning functions could be assigned to variables
* fixed a bug when assigning between void values
* fixed a bug for iterating over anonymous low-bounded ranges (e.g., `lo..`)
* fixed a bug with where clauses that use the integral type
* fixed a bug in which iterator default arguments could access invalid memory
* fixed a bug relating to promoting atomic operations with `order` arguments
* fixed several bugs relating to type arguments with a specified type
* fixed a bug with forwarding certain generic functions
* fixed a segfault when casting an empty string to a real
* fixed a bug in enum->int casts for enums with nontrivial value expressions
* fixed task intents for const ref variables
* fixed a bug with enums declared in generic functions
* fixed a bug with casting an enum's constants to certain types
* fixed a bug with assignment for opaque domains
* fixed a bug with arguments whose default value is a `new` record with init
* fixed an internal compiler error with dynamic dispatch of serial iterators
* fixed a bug in clear() for rectangular domains
* fixed bugs related to sparse matrix-matrix multiplication
* fixed a bug with `StencilDist` arrays in which cached values would be printed
* fixed default values of new array elements added to associative arrays
* fixed a bug in bounds-checking for the `insert()` method on arrays
* fixed a bug with JSON output of records with `enum` fields
* fixed a bug with chpldoc and general enum documentation comments
* fixed some bugs with respect to `Owned` patterns
* fixed a bug in which tuples could not be zipped with arrays
* fixed a bug relating to when multiple modules define a procedure named main()
* fixed bugs in `LinearAlgebra` routines relating to square / non-square matrices
* fixed a bug in which `LinearAlgebra.dot()` did not have sorted indices
* fixed an incorrect return type in the `Crypto` module
* fixed a bug where `TOML` module failed to parse quoted values with whitespace
* fixed a bug regarding zippering PCG random streams' iterate() method
* fixed a use-after-free error in task counting at program shutdown
* fixed a bug related to concurrent array creation under ugni
* fixed an incorrect data type being used in the KNL locale model
* fixed a bug with large arrays and --llvm
* fixed a bug where the memory layer was shut down too early
* fixed a build issue on systems with CUDA installed
* fixed a bug related to clock-skew with compiler performance timings
* fixed a build issue on systems with low-resolution timestamps

Launchers
---------
* added a `smp` launcher for simulating multi-locales on a shared-memory system
  (see https://chapel-lang.org/docs/1.17/usingchapel/launcher.html#currently-supported-launchers)
* added a `gasnetrun_ofi` launcher for running with GASNet over its OFI conduit
  (see https://chapel-lang.org/docs/1.17/usingchapel/launcher.html#currently-supported-launchers)

Compiler Performance
--------------------
* significantly improved the speed of --llvm compilation

Third-Party Software Changes
----------------------------
* made Chapel default to using LLVM 6.0.0 when enabled
  (see https://www.chapel-lang.org/docs/1.17/usingchapel/chplenv.html)
* removed support for LLVM versions older than 4.x
  (see https://www.chapel-lang.org/docs/1.17/usingchapel/chplenv.html)
* updated hwloc to 1.11.9
* updated fltk to 1.3.4-2

Testing System
--------------
* dramatically improved the usability of the web-based performance graphs
* added tab-completion support for start_test
* made `start_test` process files, subdirectories in sorted order
* allow performance testing flags to be overridden
* added support for executable compopts files with conditionally empty output
* added support for a -futures-only flag to parallel testing scripts
* improved testing system's ability to run from a non-CHPL_HOME util/ directory
* improved test system checks and support for valid valgrind configurations
* fixed a race in code that limits the number of concurrently executing tests
* fixed a sporadic error in tests using the high-precision timer option

Developer-oriented changes: Configuration changes
-------------------------------------------------
* added `CHPL_RUNTIME_ARCH` to specify architecture runtime is built for
* added `CHPL_TARGET_BACKEND_ARCH` to specify cpu-type used for `-march` flag
* removed a special case and detected 64-bit ARM atomics the same as others

Developer-oriented changes: Module changes
------------------------------------------
* converted most internal/standard/package module objects to use `[de]init()`
* simplified how automatic `use`s are handled in internal modules

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* added support for computing the compiler version when using git worktree
* added an error for unsupported flex versions when generating the lexer
* fixed a problem preventing `make check -j` from succeeding

Developer-oriented changes: Compiler Flags
------------------------------------------
* added a `--lifetime-checking` flag to enable prototype use-after-free checks
* added a `--warn-unstable` flag to warn of changing language features
* updated `--llvm-print-ir` to produce LLVM modules that work with other tools
* removed 1-character developer-only flags

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* greatly improved support for default initializers
* significant clean-up/refactoring of the compiler's resolution passes
* simplified the implementation of forall intents
* enabled more of the compiler to work with qualified references
* updated the version of `flex` used to generate the parser warning-free
* simplified the computation of yielded types for forall loops
* clang support no longer requires internal clang headers
* made the compiler stop distinguishing between `()` and `[]` promotions
* improvements to type encapsulation for AggregateType
* added support for enabling promotion on types with initializers
* some iterators are no longer treated as recursive
* stopped adding init methods to virtual maps
* stopped normalizing ContextCallExprs after their insertion at resolution
* added comparison operators for compiler representation of source locations
* permitted compiler analysis of loops to be run in more places
* improved the normalization of error handling
* removed the processIteratorYields pass
* removed dead code in removeUnnecessaryAutoCopyCalls
* removed the removeWrapRecords pass
* removed the `c_string_copy` type
* improved precision of LLVM alias analysis metadata
* added experimental targeting to ARM's Allinea compiler

Developer-oriented changes: Documentation improvements
------------------------------------------------------
* updated CONTRIBUTING.md content, to reduce redundancy with other docs
  (see https://github.com/chapel-lang/chapel/blob/master/.github/CONTRIBUTING.md)
* updated GitHub issue template to request users to note blocking issues
  (see https://github.com/chapel-lang/chapel/blob/master/.github/ISSUE_TEMPLATE.md)
* modified ContributorInfo.rst and TestSystem.rst for clarity
* document new future file format option
  (see https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/bestPractices/TestSystem.rst#futures-a-mechanism-for-tracking-bugs-feature-requests-etc)

Developer-oriented changes: Module improvements
-----------------------------------------------
* removed some initialize() methods on module code
* converted `proc these` on array records into iterators
* removed the multi-ddata implementation, previously disabled / unused

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added `ugni` statistics for dynamic allocation operations

Developer-oriented changes: Testing System
------------------------------------------
* converted most tests from constructors and initialize() to initializers
* updated smokeTest scripting with respect to whitespace

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* moved handling of CHPL_COMM_USE_GDB from a GASNet patch to Chapel's launchers


version 1.16.0
==============

Nineteenth public release of Chapel, October 5, 2017

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* tool / configuration highlights:
  - added an initial version of 'Mason', Chapel's package manager
  - added a new 'c2chapel' tool that simplifies interoperability with C
  - 'chpl' executable names now take the main module's name instead of 'a.out'
  - added a 'configure'/'make install' option for building+installing Chapel
* language improvements:
  - significantly improved support for user-defined initializers
  - dramatically improved error-handling, particularly across locales and tasks
  - added a 'defer' statement to aid with cleanup
  - made reduce intents preserve the reduction variable's initial value
* standard library/package modules:
  - added support for a new 'Crypto' module based on OpenSSL
  - added new 'Collection' modules, 'DistributedBag' and 'DistributedDeque'
  - added support for distributed guided and dynamic iterators
  - added support for an experimental 'TOML' module
  - improved support for mixing MPI with ugni, gasnet/aries, and/or qthreads
  - continued to improve the 'LinearAlgebra' module
  - improved Chapel-Python interoperability for the 'ZMQ' (ZeroMQ) module
  - converted many library routines to 'throw' in the event of errors
* performance improvements:
  - significantly improved the ability of the LLVM back-end to optimize Chapel
  - added support for serializing records across locales to cache local copies
  - started registering arrays dynamically for 'ugni' to improve NUMA locality
  - reduced the amount of locking used for associative array accesses
* array / domain / domain map improvements:
  - generalized 'LayoutCSR' to 'LayoutCS' to support CSR and CSC sparse layouts
  - improved the locality properties of sparse Block-distributed domains/arrays
  - improved the locality of distributed rank-change and reindex operations
  - improved the behavior and power of the 'Replicated' distribution
* other improvements:
  - various interoperability improvements for working with C pointers and memory
  - various improvements for the 'ugni' communication layer on Cray systems
  - numerous bug fixes, error message improvements, and third-party updates

New Dependencies / Configuration Changes
----------------------------------------
* added a new 'configure'/'make install' option for building+installing Chapel
  (see https://chapel-lang.org/docs/1.16/usingchapel/building.html#installing-chapel)
* users of the LLVM-based front- or back-ends must now have CMake to build
* users of the 'Regexp' module / RE2 must now have a C++11 compiler to build
* made 'printchplenv' distinguish between config-file- vs. env-set variables
* made 'printchplenv' infer the location of CHPL_HOME

New Tools / Tool Changes
------------------------
* added an initial version of 'Mason', Chapel's package manager
  (see https://chapel-lang.org/docs/1.16/tools/mason/mason.html)
* added a new 'c2chapel' tool that converts C headers to Chapel extern decls
  (see https://chapel-lang.org/docs/1.16/tools/c2chapel/c2chapel.html)
* 'chpl'-generated executables now take the main module name rather than 'a.out'
* added support for LaTeX in chpldoc comments via MathJax
* chpldoc now indicates 'throw'ing functions in its output
* made chpldoc issue a warning if it detects open/close comment mismatches

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* made explicit 'module' declarations use strict error handling by default
  (see https://chapel-lang.org/docs/1.16/technotes/errorHandling.html#relaxed-mode-for-explicit-modules)
* made reduce intents preserve the reduction variable's initial value
  (see https://chapel-lang.org/docs/1.16/technotes/reduceIntents.html#overview)
* added support for iterators that can yield void values
  (see https://chapel-lang.org/docs/1.16/technotes/voidVariables.html#void-functions-and-iterators)
* distinguished between functions returning 'void' values and non-returning fns
  (see https://chapel-lang.org/docs/1.16/technotes/voidVariables.html)
* added a requirement that 'deinit()' routines have parentheses
* return intent overloads now permit value and 'const ref' overloads w/out 'ref'
  (see 'Return Intent Overloads' in the 'Procedures' chapter of the spec)
* made it an error to apply bitwise-not (~) to boolean values

Syntactic/Naming Changes
------------------------
* single-statement 'local' blocks now require a 'do' keyword
* renamed the 'Barrier' module to 'Barriers'

New Features
------------
* added a 'defer' statement to aid with cleanup
  (see 'The Defer Statement' in the 'Statements' chapter of the language spec)
* added support for 'try'/'try!' expressions ('catch' only in statement form)
  (see https://chapel-lang.org/docs/1.16/technotes/errorHandling.html#try-expressions)
* added a 'prototype' keyword to relax checks for explicit module declarations
  (see https://chapel-lang.org/docs/1.16/technotes/errorHandling.html#fatal-mode-for-implicit-and-prototype-modules)
* added support for conditional 'local' statements
  (see https://chapel-lang.org/docs/1.16/technotes/local.html#syntax)
* added prototype support for choosing error strictness on a module granularity
  (see https://chapel-lang.org/docs/1.16/technotes/errorHandling.html#strict-mode)
* added support for reindex() on arrays to accept a list of ranges
  (see https://chapel-lang.org/docs/1.16/builtins/internal/ChapelArray.html#ChapelArray.reindex)

Feature Improvements
--------------------
* significantly improved the implementation of initializers
  (see https://chapel-lang.org/docs/1.16/technotes/initializers.html)
* significantly improved support for error-handling
  (see https://chapel-lang.org/docs/1.16/technotes/errorHandling.html)
* significantly improved the generality and robustness of 'forwarding' fields
* added support for #-based comments in the parsing of '-f' configuration files
  (see https://chapel-lang.org/docs/1.16/usingchapel/executing.html#setting-configuration-variables)
* improved the CHPL_UNWIND output to include more functions
* made --print-callgraph print calls into non-user code
* improved overload disambiguation for functions w/ partially generic arguments

Removed Features
----------------
* removed support for the deprecated '=>' operator for creating array aliases
* removed support for deprecated 'RandomStream' constructors
* retired support for the deprecated CHPL_TASKS=muxed runtime configuration

Standard Modules/Library
------------------------
* added 'throw'ing overloads to routines in 'IO', 'Regexp', 'FileSystem', ...
* added overloads to push_front(), push_back(), etc. that take array arguments
  (e.g., see https://chapel-lang.org/docs/1.16/builtins/internal/ChapelArray.html#ChapelArray.push_back)
* added support for 'waitAll()' to the 'Futures' module
  (see https://chapel-lang.org/docs/1.16/modules/packages/Futures.html#Futures.waitAll)
* added 'param' and 'type' overloads of getField() to the Reflection module
  (see https://chapel-lang.org/docs/1.16/modules/standard/Reflection.html#Reflection.getField)
* added a new lines() iterator to channels
  (see https://chapel-lang.org/docs/1.16/modules/standard/IO.html#IO.channel.lines)
* added support for file.getParentName() to the 'Path' module
  (see https://chapel-lang.org/docs/1.16/modules/standard/Path.html#Path.file.getParentName)
* added ctime() to the 'datetime' type in the 'DateTime' module
  (see https://chapel-lang.org/docs/1.16/modules/standard/DateTime.html#DateTime.datetime.ctime)
* added guided and adaptive iterators that accept domains
  (see https://chapel-lang.org/docs/1.16/modules/standard/DynamicIters.html)
* gave the dynamic() iterators a default chunk size of 1
* added an asciiToString function that converts a uint(8) into a string
  (see https://chapel-lang.org/docs/1.16/builtins/internal/String.html#String.asciiToString)
* made the printing of comm diagnostics in 'CommDiagnostics' suppress zeroes
* closed memory leaks in the 'Regexp' module

Package Modules
---------------
* added support for a new 'Crypto' module based on OpenSSL
  (see https://chapel-lang.org/docs/1.16/modules/packages/Crypto.html)
* added new 'Collection' modules, 'DistributedBag' and 'DistributedDeque'
  (see https://chapel-lang.org/docs/1.16/modules/packages/Collection.html,
   https://chapel-lang.org/docs/1.16/modules/packages/DistributedBag.html, and
   https://chapel-lang.org/docs/1.16/modules/packages/DistributedDeque.html)
* added support for distributed guided and dynamic iterators
  (see https://chapel-lang.org/docs/1.16/modules/packages/DistributedIters.html)
* added support for a new 'TOML' module (not 100% feature complete)
  (see $CHPL_HOME/modules/packages/TOML.chpl)
* improved support for 'MPI' with ugni, gasnet/aries, and/or qthreads
* improved Chapel-Python interoperability for the 'ZMQ' (ZeroMQ) module
* improved the 'LinearAlgebra' module in a number of ways:
  - added a new 'Sparse' sub-module to 'LinearAlgebra'
    (see https://chapel-lang.org/docs/1.16/modules/packages/LinearAlgebra/Sparse.html)
  - added diag() to support extracting diagonals and building diagonal matrices
    (see https://chapel-lang.org/docs/1.16/modules/packages/LinearAlgebra.html#LinearAlgebra.diag)
  - optimized the implementation of dense transpose
* added 'throw'ing functions in HDFS

Standard Domain Maps (Layouts and Distributions)
------------------------------------------------
* generalized 'LayoutCSR' to 'LayoutCS' to support CSR and CSC sparse layouts
  (see https://chapel-lang.org/docs/1.16/modules/layouts/LayoutCS.html)
* normalized the ReplicatedDist distribution and renamed it to 'Replicated'
  (see https://chapel-lang.org/docs/1.16/modules/dists/ReplicatedDist.html
   and https://chapel-lang.org/docs/1.16/primers/replicated.html)
* added a 'replicand' method to 'ReplicatedDist' supporting local access
  (see https://chapel-lang.org/docs/1.16/modules/dists/ReplicatedDist.html
   and https://chapel-lang.org/docs/1.16/primers/replicated.html)
* privatized sparse Block-distributed domains/arrays
* closed memory leaks in the DimensionalDist2D distribution

Interoperability Improvements
-----------------------------
* added support for 'c_sizeof()' to query sizes of C types
  (see https://chapel-lang.org/docs/1.16/builtins/internal/CPtr.html#CPtr.c_sizeof)
* added support for an 'isAnyCPtr()' query
  (https://chapel-lang.org/docs/1.16/builtins/internal/CPtr.html#CPtr.isAnyCPtr)
* permitted c_mem*() routines to work with c_void_ptr arguments
* added support for c_memset()
  (see https://chapel-lang.org/docs/1.16/builtins/internal/CPtr.html#CPtr.c_memset)
* added support for printing 'c_ptr' and 'c_void_ptr' variables
* added support for an 'isExternClassType()' query

Performance Optimizations/Improvements
--------------------------------------
* significantly improved the ability of the LLVM back-end to optimize Chapel
* improved locality for sparse Block-distributed domains/arrays
* optimized bulkAdd calls for empty sparse domains
* reduced the amount of locking used for associative array accesses
* added support for serializing records across locales, supporting local copies
* optimized remote task creation for cases like 'coforall ... do on ... {}'
* automatically inline iterators that have multiple yields (10 by default)
* improved the performance of dynamic casting / subclass checks
* improved wide-pointer analysis for 'const ref' arguments
* reduced wide pointer overheads for 'Block' and 'Stencil' distributions
* fixed bugs in counting tasks and creating the right number of new tasks
* added support for parallel array initialization for arrays of POD eltTypes
* optimized the implementation of updateFluff() on the Stencil distribution
* privatized domains and distributions created by rank changes and reindexing
* made the domains and distributions of reindexed arrays preserve locality

Memory Improvements
-------------------
* stopped heap-promoting local variables used within on-clauses for 'qthreads'
* closed memory leaks in the 'Regexp' module and DimensionalDist2D distribution

Compiler Flags
--------------
(see 'man chpl' or https://chapel-lang.org/docs/1.16/usingchapel/man.html for details)
* added --permit-unhandled-module-errors to enable relaxed error checking
* added --print-unused-functions to identify unused routines
* added --[no-]remote-serialization to control cross-locale record copies
* added an --mllvm flag to set LLVM-based optimizations when using --llvm
* added --inline-iterators-yield-limit to cap # of yields to inline for iters
* gave the --cache-remote flag a --no-cache-remote variation
* removed the --strict-errors flag in favor of new more precise features
* removed the --conditional-dynamic-dispatch-limit flag and feature

Documentation
-------------
* added new primers on error-handling and the 'LinearAlgebra' module
  (see https://chapel-lang.org/docs/1.16/primers/errorHandling.html and
   https://chapel-lang.org/docs/1.16/primers/LinearAlgebralib.html)
* added a document describing built-in 'Error' types in Chapel
  (see https://chapel-lang.org/docs/1.16/builtins/internal/ChapelError.html)
* broke information about the 'Replicated' distribution into its own primer
  (see https://chapel-lang.org/docs/1.16/primers/replicated.html)
* added a new 'Methods' chapter to the language spec and refreshed the content
* improved the language specification's definition of records
* documented the ability to specify configuration files via '-f'
  (see https://chapel-lang.org/docs/1.16/usingchapel/executing.html#setting-configuration-variables)
* added missing documentation for range.low, high, stride, alignment, aligned
  (see https://chapel-lang.org/docs/1.16/builtins/internal/ChapelRange.html#ChapelRange.range.stride)
* added missing documentation for reindex() and localSlice()
  (see https://chapel-lang.org/docs/1.16/builtins/internal/ChapelArray.html#ChapelArray.reindex
   and https://chapel-lang.org/docs/1.16/builtins/internal/ChapelArray.html#ChapelArray.localSlice)
* fixed an oversight in the specification to indicate that '=' is overloadable

Example Codes
-------------
* added new primer examples for error-handling, linear algebra, and replication
  (see examples/primers/errorHandling.chpl,
       examples/primers/LinearAlgebralib.chpl, and
       examples/primers/replicated.chpl)
* updated example programs to use initializers rather than constructors
* updated example programs with respect to new error-handling features
* added new faster versions of the mandelbrot and chameneos CLBG benchmarks
  (see examples/benchmarks/shootout/mandelbrot-fast.chpl and
   examples/benchmarks/shootout/chameneosredux-fast.chpl)
* changed the knucleotide CLBG program to use the default 'parSafe' mode
  (see examples/benchmarks/shootout/knucleotide.chpl)
* rewrote ISx to use a more elegant global domain declaration
  (see examples/benchmarks/isx/)
* rewrote SSCA2 to use the standard 'Barriers' module and closed memory leaks
  (see examples/benchmarks/ssca2/)
* renamed the 'opaque domains and arrays' primer to 'opaque-domains.chpl'
* made other updates to example programs to reflect changes since 1.15

Locale Models
-------------
* added a new locale model for AMD Accelerator Processing Units (APUs)
  (see https://github.com/chapel-lang/chapel/blob/release/1.16/doc/rst/developer/chips/22.rst)
* stopped storing arrays as multiple distinct chunks in the 'numa' locale model
* ensured that existing locale models clean up after themselves at program exit

Portability
-----------
* added support for using Chapel on an OmniPath cluster
  (see https://chapel-lang.org/docs/1.16/platforms/omnipath.html)
* improved code conformance with C++14
* improved code portability with respect to various versions of gcc
* improved portability of code with respect to Cygwin

Cray-specific Changes
---------------------
* started registering arrays dynamically for 'ugni' to improve NUMA locality
* optimized strided puts and gets for CHPL_COMM=ugni
* split large transfers for CHPL_COMM=ugni
* reduced the default heap size for CHPL_COMM=ugni
* removed caveat about ugni registration limits in Cray documentation
* retired official support for the 'cray-xt' platform

Syntax Highlighting
-------------------
* added some missing keywords to 'vim', 'emacs', and 'highlight' highlighters

Error Messages / Semantic Checks
--------------------------------
* improved 'const' checking
* extended --div-by-zero-checks to also check for modulus (%) 0 operations
* added an error message for exported functions with generic arguments
* improved error messages for illegal 'delete' statements
* removed checks that iterators must contain 'yield' statements; they need not
* added an error for records that try to subtype another type
* added a number of error messages for poorly formed (or unsupported) init()s
* added a warning for --library compiles on code containing a main() routine
* improved the error message generated when closing a file before its channels
* added an error for returning a tuple of the wrong size
* added a warning for potential confusions related to implicit module naming
* improved an error message for bad forwarding calls to parallel iterators
* improved an error message about type mismatches between fields

Bug Fixes
---------
* fixed a number of bugs related to initializers
* fixed a number of bugs related to error-handling
* fixed several bugs in the 'forwarding' feature for object fields
* fixed bugs in counting tasks and creating the right number of new tasks
* fixed bugs for several forall intent cases
* fixed a bug in which a qualified module reference was incorrectly shadowed
* fixed a bug in isAlpha() for characters between upper- and lowercase letters
* fixed a bug in bulk assignment for rank-change slices
* fixed a bug in variable deinitialization order
* fixed a bug in which 'use' statements were not considered in program order
* fixed a bug in which 'rmTree' would not remove directories with hidden files
* fixed some bugs in loop invariant code motion (LICM)
* fixed a portability bug in padding years in the DateTime module
* fixed a bug in dead code elimination relating to local record types
* fixed a bug comparing floating point expressions on linux32
* fixed a bug in complicated type aliases
* fixed a bug in denormalization for '~' for small integers
* fixed a bug in which remote-value forwarding didn't handle dereferences well
* fixed a bug relating to scoped accesses to internal modules
* fixed bugs with parallel iteration over domains with non-natural alignment
* fixed a bug in the implementation of the &= operator for associative domains
* fixed a bug in applying 'reindex()' to an empty domain/array
* fixed a bug in modules with just one non-initialization function declaration
* fixed a bug in dead code elimination for do-while loops
* fixed a bug in which isRecord*() returned 'true' for sync/single types
* fixed a bug related to task counters not being stored in task-local storage
* fixed a bug for ambiguous 'param' methods
* fixed a bug in 'fifo' tasking in which not enough threads were created

Launchers
---------
* added a 'gasnetrun_psm' launcher for running on OmniPath interconnects
  (see https://chapel-lang.org/docs/1.16/usingchapel/launcher.html#currently-supported-launchers)
* fixed some bugs in the pbs-gasnetrun_ibv and slurm-based launchers

Generated Code
--------------
* cleaned up generated code for no-op statements referencing symbols

Third-Party Software Changes
----------------------------
* updated the compiler to be compatible with newer versions of LLVM
* switched the LLVM back-end to use version 4.0.1 by default
* updated GASNet to version 1.30.0
* updated hwloc to version 1.11.8
* updated GMP to version 6.1.2
* updated RE2 to commit a810d71
* enabled support for multiple communication domains in GASNet for gemini/aries

Testing System
--------------
* dramatically improved the quality of the generated web performance graphs
* improved the redirection of stderr into log files

Developer-oriented changes: Module changes
------------------------------------------
* added a "function terminates program" pragma to identify halt() and exit()
* simplified how automatic 'use's are handled in internal modules

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* made our Makefiles respect CFLAGS, CPPFLAGS, and CXXFLAGS
* we now build the compiler with C++11 when possible for gcc, clang, icc

Developer-oriented changes: Compiler Flags
------------------------------------------
* added --user-constructor-error to flag uses of constructors in user code
* added --force-initializers to generate default initializers for some types
* added --llvm-print-ir to dump the LLVM internal representation to stdout
* added --llvm-print-ir-stage to specify which LLVM stage should print the IR
* improved the behavior of the --log flag
* flipped --log-ids to be on by default
* made --print-module-resolution print AST counts

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* significant clean-up/refactoring of the compiler's resolution passes
* cleaned up / refactored aspects of file handling, parsing, and building AST
* converted more compiler data structures to use the STL
* made greater use of qualified types in compiler passes
* improved the internal representation of forall loops
* reduced the amount of nondeterminism in the compiler
* removed support for trying to handle references in copyPropagation
* added a "last resort" pragma/flag for error-case overloads
* removed support for the conditional-based dynamic dispatch implementation
* improved the management of well-known functions within the compiler
* cleaned up how the compiler manages the 'program' and 'root' modules
* added support for traversing the top-level modules
* refactored the cleanup pass to process a module at a time and cleaned it up
* cleaned up aspects of buildDefaultFunctions
* removed the reaching definition analysis portion of dead code elimination
* added 'parentlocid' command within the compiler's gdb support
* many other code cleanups, reorganizations, and refactorings

Developer-oriented changes: Documentation improvements
------------------------------------------------------
* fixed a bug in which CHIPs couldn't be built into HTML

Developer-oriented changes: Module improvements
-----------------------------------------------
* added a field to the 'channel' record indicating the originating locale

Developer-oriented changes: Runtime improvements
------------------------------------------------
* improved the mechanism we use to look for bad memory allocation/exit calls
* updated get/put runtime calls to include a file-unique ID
* added wrapper functions for direct calls to the system memory allocator

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* changed our Makefiles so that third-party code gets rebuilt when needed



version 1.15.0
==============

Eighteenth public release of Chapel, April 6, 2017

Highlights (see subsequent sections for further details)
--------------------------------------------------------
* performance optimization highlights:
  - significantly improved the performance of many key idioms and benchmarks
  - dramatically improved task creation speed
  - reduced overheads related to array slicing, rank-change, and reindexing
  - significantly improved the remote value forwarding optimization
  - optimized the performance of latency-bound applications with CHPL_COMM=ugni
  - improved qthreads tasking such that it outperforms 'muxed' on studied codes
* standard library/package highlights:
  - added a new 'DateTime' module for operating on dates and times
  - added BLAS level 1 and 2 routines to the previous support for level 3
  - added an early-draft 'LinearAlgebra' module
  - added a new 'Futures' module supporting library-based futures
  - added initial support for some class-owner patterns, 'Owned' and 'Shared'
  - added support for choosing between implementations for the 'FFTW' module
* language improvements:
  - dramatically improved support for object initializers and deinitializers
  - added prototype support for error-handling in Chapel
  - significantly improved array semantics and memory management
  - improved support for generic classes and records
  - made 'void' a first-class type in the language and a way to fold variables
  - added support for module deinit() functions
  - added support for 'forwarding' methods to fields of classes and records
  - vastly improved the stability, generality, and precision of 'where' clauses
* portability highlights:
  - added a locale model for KNL with support for different memory types
  - added Chapel support for AWS EC2
  - verified that Chapel works with the Windows 10 bash shell as with Ubuntu
  - improved support for ARM processors including multi-locale support
* implementation highlights:
  - closed significant memory leaks caused by arrays, domains, and domain maps
  - added support for numa-localization of array memory
  - improved array slice, rank change, and reindexing support for domain maps
  - streamlined several aspects of the domain map standard interface (dsi)
* additional highlights:
  - added new users guide text for promotion, constants, type aliases, configs
  - enabled GitHub issues to file and track Chapel bugs and feature requests
  - added the 'ugni' communication layer to the open-source repository
  - updated our versions of GASNet, Qthreads, jemalloc, and hwloc

Deployment
----------
* enabled GitHub issues to file and track Chapel bugs and feature requests
  (see https://github.com/chapel-lang/chapel/issues)
* extended the Docker image to include a GASNet-based configuration
  (see https://hub.docker.com/r/chapel/chapel/)

New Features
------------
* dramatically improved support for initializers
  (see https://chapel-lang.org/docs/1.15/technotes/initializers.html
   and https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst)
* added prototype support for error-handling in Chapel
  (see https://chapel-lang.org/docs/1.15/technotes/errorHandling.html
   and https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/8.rst)
* added support for deinit() as a replacement for class/record destructors
  (see "Class Deinitializer" and "Record Deinitializer" sections in the spec
   and https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst)
* added support for 'forwarding' methods to fields of classes and records
  (see https://chapel-lang.org/docs/1.15/technotes/forwarding.html)
* made 'void' a first-class type in the language and a way to fold variables
  (see https://chapel-lang.org/docs/1.15/technotes/voidVariables.html
   and $CHPL_HOME/examples/primers/voidVariables.chpl)
* added support for module deinit() functions
  (see "Module Deinitialization" in the "Modules" chapter of the specification)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* functions that return array expressions now return by value by default
  (see https://chapel-lang.org/docs/1.15/language/evolution.html#arrays-return-by-value-by-default)
* made the default intent for arrays be 'ref' if modified, else 'const ref'
  (see https://chapel-lang.org/docs/1.15/language/evolution.html#array-default-intent
   and "Array Arguments to Functions" in the "Arrays" chapter of the spec)
* made the default intent for 'this' on record methods be 'ref' if modified
  (see https://chapel-lang.org/docs/1.15/language/evolution.html#record-this-default-intent)
* deprecated support for the '=>' operator to create array aliases
  (see https://chapel-lang.org/docs/1.15/language/evolution.html#array-alias-operator-deprecated)
* deprecated support for using '=>' in constructors to create alias fields

Syntactic/Naming Changes
------------------------
* renamed the 'Error' module to 'SysError'
  (see https://chapel-lang.org/docs/1.15/modules/standard/SysError.html)

Feature Improvements
--------------------
* vastly improved the stability, generality, and precision of 'where' clauses
  (see "Where Expressions" in the "Procedures" chapter of the specification)
* improved array slice, rank change, and reindexing support across domain maps
* extended return intent overloading to improve its utility and flexibility
  (see "Return Intents" in the "Procedures" chapter of the specification)
* added support for using 'const' and 'const ref' as 'this' intents
  (see "The Method Receiver and the this Argument" sections in the spec)
* added support for defining type aliases for generic classes
  (e.g., 'type Vec = Vector;' is now supported for a generic type 'Vector')
* added the ability to define secondary methods on instantiated types
  (e.g., 'proc (R(int)).foo() { ... }' defines foo on R(int) but not R(real)
   see "Class Methods" section in the Classes chapter of the specification)
* made tuple semantics more closely follow the behavior of their element types
* added min() and max() overloads that accept and generate 'param' values
* added support for casts between c_void_ptr and class objects/c_string
* improved the default hash functions used by associative domains
* reduced opportunities for races on set operations for associative arrays
* enabled 'param's and 'config param's to be specified without initializers

Known Feature Slips
-------------------
* domains of reindexed distributed arrays are not distributed as they should be
* removed support for having first-class functions capture outer variables

Removed Features
----------------
* removed the 'noRefCount' config const which is no longer necessary
* removed deprecated functions from the 'Sort' and 'Search' modules
* removed the deprecated 'BigInt' class in favor of the 'bigint' value type

Standard Modules/Library
------------------------
* added support for a 'DateTime' module for operating on dates and times
  (see https://chapel-lang.org/docs/1.15/modules/standard/DateTime.html)
* improved the initializer argument list and order for RandomStream
  (see https://chapel-lang.org/docs/1.15/modules/standard/Random.html)
* converted the 'barrier' type from a class into a record
  (see https://chapel-lang.org/docs/1.15/modules/standard/Barrier.html)
* added support for a .shape query on domains and arrays
  (see https://chapel-lang.org/docs/1.15/builtins/internal/ChapelArray.html#ChapelArray.shape)
* added support for 'targetLocales' query to default rectangular arrays/domains
* made count() on arrays parallel by default
* added support for passing comparators to the domain.sorted() iterator
  (see https://chapel-lang.org/docs/1.15/builtins/internal/ChapelArray.html#ChapelArray.sorted)
* made conjg() generate the same type as its argument
  (see https://chapel-lang.org/docs/1.15/modules/standard/Math.html#Math.conjg)
* improved memory management for the 'List' module

Package Modules
---------------
* added initial support for some class-owner record patterns, Owned and Shared
  (see https://chapel-lang.org/docs/1.15/modules/packages/OwnedObject.html
   and https://chapel-lang.org/docs/1.15/modules/packages/SharedObject.html)
* added support for a new 'Futures' module supporting library-based futures
  (see https://chapel-lang.org/docs/1.15/modules/packages/Futures.html)
* added BLAS level 1 and 2 routines to the previous support for level 3
  (see https://chapel-lang.org/docs/1.15/modules/packages/BLAS.html)
* removed support for the 'ldA' arguments in the BLAS module
* added an early-draft 'LinearAlgebra' module
  (see https://chapel-lang.org/docs/1.15/modules/packages/LinearAlgebra.html)
* added support for choosing between FFT implementations in the FFTW module
  (see https://chapel-lang.org/docs/1.15/modules/packages/FFTW.html#FFTW.isFFTW_MKL)
* improved support for the 'MatrixMarket' module

Interoperability Improvements
-----------------------------
* improved 'require' statements to accept 'param' string expressions
  (see https://chapel-lang.org/docs/1.15/technotes/extern.html#expressing-dependencies)
* now only 'require' statements in resolved code add requirements
  (see https://chapel-lang.org/docs/1.15/technotes/extern.html#expressing-dependencies)

Performance Optimizations/Improvements
--------------------------------------
* improved parallel tasking performance using a hybrid spin/condwait strategy
* optimized the creation of tasks for certain coforalls with known trip counts
* moved argument bundles for tasks and on-clauses from the heap to task stacks
* switched qthreads to use Chapel's registered heap for its memory requirements
* reduced array overhead by eliminating reference counting overheads
* optimized 1D strided iterations
* reduced the overhead of slicing, rank-change slicing, and reindexing arrays
* reduced the amount of communication required for a number of idioms
* improved the precision of the array index optimization introduced in 1.14
* stopped limiting the number of ugni communication domains with slurm on Crays
* optimized a costly temporary array out of the BlockCyclic distribution
* added intra-locale parallelism for iterations over BlockCyclic distributions
* improved loop-invariant code motion for iterator expressions
* improved the cases handled by, and accuracy of, remote value forwarding
* removed redundant field initializations in init() routines
* improved the performance of casts from strings to numeric types
* optimized '<~>' overloads to avoid unnecessary reference counting

Memory Improvements
-------------------
* closed significant memory leaks caused by arrays, domains, and domain maps
* reduced the memory footprint of a non-stridable range
* made the 'list' type reclaim its memory on destruction
* fixed a memory leak in the push_back() routine

Example Codes
-------------
* added a new primer for 'void' variables and fields
  (see $CHPL_HOME/examples/primers/voidVariables.chpl)
* made style and content improvements to the 'learnChapelInYMinutes' primer
  (see $CHPL_HOME/examples/primers/learnChapelInYMinutes.chpl)
* made a number of LCALS cleanups and fixes including support for a SPMD mode
  (see $CHPL_HOME/examples/benchmarks/lcals/)
* switched ISx's arrays to use anonymous domains for a performance boost
  (see $CHPL_HOME/examples/benchmarks/isx/)
* added a new 'regexdna-redux' benchmark
  (see $CHPL_HOME/examples/benchmarks/shootout/regexdna-redux.chpl)
* made a number of speed and clarity improvements to the 'fasta' benchmark
  (see $CHPL_HOME/examples/benchmarks/shootout/fasta.chpl)
* made readability and correctness improvements to our 'pidigits' benchmark
  (see $CHPL_HOME/examples/benchmarks/shootout/pidigits.chpl)
* fixed an off-by-one error in our 'binarytrees' benchmark
* fixed an assignment-to-const bug in binary-trees
* added missing Makefiles and fixed support for 'make -j'
* updated example codes to use new RandomStream() initializer
* updated example codes to reflect better const-checking
* updated example codes to use deinit() rather than destructors
* updated example codes to reflect that barrier is now a record
* removed the 'fastaredux' benchmark, which is no longer used by the CLBG

Documentation
-------------
* added new users guide sections on promotion, constants, type aliases, configs
  (see https://chapel-lang.org/docs/1.15/users-guide/index.html)
* revised QUICKSTART instructions for clarity
  (https://chapel-lang.org/docs/1.15/usingchapel/QUICKSTART.html)
* reorganized the doc/ directory in the release tarball
  (see $CHPL_HOME/doc/README.md
   or https://github.com/chapel-lang/chapel/blob/master/doc/README.rst)
* removed $CHPL_HOME/STATUS in favor of GitHub issues
* updated bugs.rst to refer users to GitHub issues
  (see https://chapel-lang.org/docs/1.15/usingchapel/bugs.html)
* improved the Docker README information
  (see https://hub.docker.com/r/chapel/chapel/)
* reorganized the platform-specific documentation pages into categories
  (see https://chapel-lang.org/docs/1.15/platforms/index.html)
* added documentation for dim() and dims() on arrays
  (see https://chapel-lang.org/docs/1.15/builtins/internal/ChapelArray.html#ChapelArray.dims)
* fixed the documentation for string.strip()
  (see https://chapel-lang.org/docs/1.15/builtins/internal/String.html#String.string.strip)
* updated documentation regarding reference counting of files and channels
* added an indication that IO is a module that is used by default
* updated documents to refer to download.html rather than install.html
* improved some entries in the "quick reference" document
  (see https://chapel-lang.org/docs/1.15/_downloads/quickReference.pdf)

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* added a '--[no-]print-callgraph flag to print a Chapel program's callgraph
* added a '--[no-]div-by-zero-checks' flag to disable divide-by-0 checks
* added a '--strict-errors' flag for enabling strict error-checking
* added a '--[no-]infer-local-fields' flag to control wide pointer optimization
* removed the --[no-]optimize-array-indexing flag, now performed automatically

Locale Models
-------------
* added a locale model for KNL with support for different memory types
  (see https://chapel-lang.org/docs/1.15/technotes/localeModels.html#knl-locale-model
   and https://chapel-lang.org/docs/1.15/platforms/knl.html)
* for 'numa', arrays are now allocated using multiple per-numa-domain chunks
  (see https://chapel-lang.org/docs/1.15/technotes/localeModels.html#numa-locale-model)

Portability
-----------
* added Chapel support for AWS EC2
  (see https://chapel-lang.org/docs/1.15/platforms/aws.html)
* verified that Chapel works with the Windows 10 bash shell as with Ubuntu
* improved support for ARM processors including multi-locale support
  (see https://chapel-lang.org/docs/1.15/platforms/arm.html)

Platform-specific Changes
-------------------------
* fixed an occasional cygwin failure caused by pthread_attr_init()

Cray-specific Changes
---------------------
* made Cray systems always default to CHPL_COMM=ugni
* stopped limiting the number of ugni communication domains under slurm
* tuned the amount of task spinning before sleeping for Crays
* CHPL_TASKS=muxed is now deprecated and will be removed in future releases
* removed extraneous/incorrect -O flags from cray-prgenv-cray command lines
* make the heap used by CHPL_COMM=ugni 'numa'-localized when appropriate
* fixed a problem when using 'muxed' tasking with 'gasnet' but no hugepages
* enabled support for CHPL_COMM=ugni for the pgi compiler
* fixed a bug in dynamic linking for gasnet-aries

Error Messages
--------------
* significantly improved const-checking in the compiler
* added an error message for re-assigning a 'param' after its initialization
* added an error when 'inline' is applied to a recursive function
* added an error when trying to cast to values rather than types
* added an error message for trying to capture generic functions
* added an error message for initializers that attempt to return a value
* added an error message for missing copy initializers
* made the compiler indicate illegal 'param' types more clearly
* added support for printing multiple 'param' errors before halting compilation
* added an error when using domain queries in field declarations
* added an error message for querying the 'IRV' of a non-sparse array
* added error messages for applying vector ops to non-1D rectangular arrays
* improved error messages for the $CHPL_HOME/util/chplenv/ scripts
* removed warnings for serialized assignments in some cases

Runtime Error Checks
--------------------
* added a runtime check for divide-by-zero by default
* added a runtime check for empty bounding boxes to the 'Block' distribution

Bug Fixes
---------
* fixed a number of bugs relating to array-of-array and sparse-array semantics
* fixed a bug in reading null bytes into strings when lengths are specified
* fixed a bug in resetting size when clearing sparse block-distributed domains
* fixed a bug in which 'uint **= uint' failed to resolve
* fixed a bug in the support of 'extern' blocks
* fixed a bug in which the wrong LD was selected by our Makefiles
* fixed a buffer overflow problem in the parser for long function signatures
* fixed a broken link to Quickstart.rst
* fixed a bug in which the LLVM back-end couldn't support multiple --ccflags
* fixed an off-by-one bug in string.split()
* fixed a bug related to extern procedures returning 'void'
* fixed a bug with arrays of c_strings in --no-local compilation mode
* fixed a bug in the order of module-scope variable deinitializations
* fixed a bug in the find() routine on arrays generalizing it to non-1D arrays
* fixed a bug in which type methods did not support default arguments well
* fixed a bug in pop_front() which caused the array to grow
* fixed a bug in which the --cpp-lines flag was sometimes ignored
* fixed a bug in 'Spawn' when one sub-process consumes another's output
* fixed a bug in BlockCyclic indexing
* fixed a bug in which user-defined initializers wouldn't accept array fields
* fixed a bug in creating virtual dispatch tables for generic class inheritance
* fixed a bug in recursive iterator inlining
* fixed a bug supporting 'stridable' queries on Replicated arrays
* fixed a copy propagation bug related to array allocation
* fixed a 'chplvis' bug that caused segmentation faults
* fixed a bug in which type methods were overly generic w.r.t. their receiver
* fixed a bug in casting reference expressions
* fixed a race condition in initializing locale models
* fixed bugs in the standalone parallel iterator for CSR domains
* fixed a bug related to module-scoped 'ref' declarations
* fixed a bug in which the number of CPUs was sometimes reported to be 0
* fixed bugs in the 'localSubdomain*' calls for local arrays/domains

Launchers
---------
* made the --spmd flag to the mpirun launcher default to '1'

Runtime Library Changes
-----------------------
* added the 'ugni' communication layer to the open-source repository
* added support for numa-localization of array memory
* switched qthreads to use Chapel's memory allocator
* changed our use of qthreads to initialize them in detached state
* updated massivethreads to make use of stack-allocated task bundles

File/Directory Structure
------------------------
* moved object files for the compiler and runtime into $CHPL_HOME/build
* changed the naming/structure of directories used to store binary files

Generated Code
--------------
* turned on --denormalize by default, resulting in shorter, cleaner code
* improved formatting of formal arguments in generated code for --gen-ids

Third-Party Software Changes
----------------------------
* updated to GASNet 1.28.0
* updated to qthreads 1.12
* updated to jemalloc 4.5.0
* updated to hwloc 1.11.6
* made GASNet build 'amudprun' using host compiler rather than target
* limited the maximum size of qthreads memory pools
* made 'hwloc' always use libnuma for non-flat locale models when available
* improved the portability of our libunwind support

Testing System
--------------
* added --multi-locale-only and --max-locales flags to 'start_test'
* added a '-dirs' option to paratest.server to specify directories to test
* added an '-env' flag to paratest.server for forwarding environment variables
* added the ability to filter on graph names in the performance tracker
* made the test system work even when CHPL_UNWIND is set
* added a checker for the ANNOTATIONS.yaml file used by performance tracking
* added overlays of the nightly performance graphs to the release-over-release

Developer-oriented changes: Configuration changes
-------------------------------------------------
* extended CHPL_LLVM to support new modes: llvm-minimal and system[-minimal]

Developer-oriented changes: Module changes
------------------------------------------
* refactored locale models code
* optimized out array fields used only in non-flat configurations using 'void'
* added 'export' to locale model routines to avoid widening arguments
* refactored rectangular I/O into a single helper routine
* moved c_void_ptr comparisons from CPtr to ChapelBase
* added a developer feature for specifying arrays' initialization approaches
* removed an unnecessary argument from _bulkGrow
* simplified the 'dsi' interface for slicing, rank-change, and reindexing
  (see https://chapel-lang.org/docs/1.15/technotes/dsi.html)
* updated the 'dsi' interfaces for establishing new domains
* cleaned up how the MPI module deinitializes/finalizes itself

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* improved the robustness of 'make check'
* worked around a race condition in GASNet's Makefiles
* cleaned up the runtime include paths
* improved linking of libnuma when using the memkind library
* improved Makefile portability for Bourne and limited shells

Developer-oriented changes: Compiler Flags
------------------------------------------
* removed the --report-optimized-array-indexing flag, no longer required

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* made PRIM_GETCID return an int(32) rather than a bool
* replaced four FnSymbol fields and a flag with a std::map
* improved the compiler's opt-in verification checks
* updated the compiler's representation of 'ref' types to use qualified types
* fixed a segfault in list_ast() for type-less symbols
* changed more compiler data structures over to their STL equivalents
* made PRIM_WIDE_GET_ADDR return a c_void_ptr
* each symbol now stores a list of associated SymExprs
* made some fixes to for_vector, for_set, and for_queue
* fixed a bug in getVisibleFunctions relating to forall intents
* added an ADT for representing forall intents
* removed unnecessary code from inlineFunctions
* refactored codegen-related methods from AST/expr.cpp to codegen/expr.cpp
* minor improvements to the AstDumpToNode debugging output
* made compute_call_sites() re-entrant
* refactored code implementing inlining
* improved the implementation of destructors/deinitializers
* rewrote buildDefUseMaps and compute_call_sites to use SymExpr info in Symbols
* eliminated an unnecessary vector computed in flattenClasses
* refactored FnSymbol::tag_generic(), renamed to tagIfGeneric() for clarity
* cleaned up and simplified normalize, particularly w.r.t. initializers
* improved ability to modify code around a function's epilogue label
* fixed a minor bug in printResolutionErrorUnresolved()
* unified the number of memory descriptors created for --devel vs. --no-devel
* significantly refactored build_record_init_function()
* fixed a bug in which copies of AggregateTypes lost their initializerStyles
* reduced the degree to which init was implemented similar to constructors
* added a type method to be used to initialize object init() methods
* removed unused FLAG_SPECIFIED_RETURN_TYPE
* replace uses of buildDefUseMaps with SymExpr information in Symbols
* extended CallInfo to support generic initializers
* refactored elements of resolution to better support initializers
* refactored the initialization of configs away from traditional declarations
* added AggregateType::isGeneric()
* updated the logic for pruning unused module-scope variables
* removed the complex but not very useful single-yield iterator optimization
* removed Symbol::getFnSymbol()
* streamlined the normalization of many variable declarations
* removed a leak-based workaround in formalRequiresTemp()
* added cast-related functions to the compiler
* converted several parser buffers over to std::string
* reduced number of cases where the compiler treats certain records specially
* removed the wide-return optimization due to complexity and lack of impact
* added a new primitive, PRIM_INIT_VAR, for variable initializations
* removed the old compiler-based optimization of array access inner multiplies
* revised the code to generate default initializers for tuples
* disabled deadStringLiteralElimination without inlining or copy-propagation
* hoisted gdbShouldBreakHere() calls to get earlier compiler breakpoints
* improved the precision of canRHSBeConstRef() in inferConstRefs
* made isPrimitiveScalar() recognize bools of size BOOL_SIZE_SYS
* have internal actual-formal argument count mismatches print function name

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added an API for performing task ID comparisons
* moved stray .c files out of runtime/include
* fixed some issues in how privatization works
* updated some interfaces to use c_nodeid_t rather than int32_t
* removed some dead code in the qthreads tasking layer
* changed how and when the heap page size is computed

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* protected our jemalloc symbols to avoid conflicts with the memkind library



version 1.14.0
==============

Seventeenth public release of Chapel, October 6, 2016

Highlights (see subsequent sections for more details)
-----------------------------------------------------
* performance optimization highlights:
  - optimized array accesses for programs using simple/common array operations
  - dramatically improved performance of promoted expressions over aligned data
  - streamlined memory management through new jemalloc upgrades and features
  - optimized reductions by using an atomic spin lock rather than a sync var
  - enabled the bulk transfer of strided array slices by default
  - significantly optimized 'sync' variables when using 'qthreads' tasking
* standard library highlights:
  - added a new standard 'BigInteger' module providing value-based 'bigint's
    (see https://chapel-lang.org/docs/1.14/modules/standard/BigInteger.html)
  - added new communication package modules for MPI and ZeroMQ
    (see https://chapel-lang.org/docs/1.14/modules/packages/MPI.html and
     https://chapel-lang.org/docs/1.14/modules/packages/ZMQ.html)
  - added new package modules for BLAS, MatrixMarket, and range chunking
    (see https://chapel-lang.org/docs/1.14/modules/packages/BLAS.html,
     $CHPL_HOME/modules/packages/MatrixMarket.chpl, and
     https://chapel-lang.org/docs/1.14/modules/packages/RangeChunk.html)
  - dramatically revamped the Sort and Search modules, including comparators
    (see https://chapel-lang.org/docs/1.14/modules/packages/Sort.html and
     https://chapel-lang.org/docs/1.14/modules/packages/Search.html)
* domain map highlights:
  - added support for Block-distributed sparse domains and arrays
    (see https://chapel-lang.org/docs/1.14/modules/dists/BlockDist.html)
  - added a new 'StencilDist' distribution in support of stencil access patterns
    (see https://chapel-lang.org/docs/1.14/modules/dists/StencilDist.html)
* implementation improvement highlights:
  - added a capability for generating stack traces on 'halt's
    (see https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html#chpl-unwind)
  - improved support for reduce intents on parallel loops
    (see https://chapel-lang.org/docs/1.14/technotes/reduceIntents.html)
  - added the ability to implement Chapel's atomics using C atomics
    (see https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html#chpl-atomics)
  - added the ability to pass Chapel functions to extern C routines
    (see https://chapel-lang.org/docs/1.14/technotes/extern.html#c-fn-ptr)
* documentation and example code highlights:
  - added the primer and "hello world" example codes to the online documentation
    (see https://chapel-lang.org/docs/1.14/primers/index.html and
     https://chapel-lang.org/docs/1.14/examples/index.html)
  - completed and updated our suite of Computer Language Benchmark Game codes
    (see $CHPL_HOME/examples/benchmarks/shootout/* and
     http://benchmarksgame.alioth.debian.org/)
  - added several new pages to the online users guide
    (see https://chapel-lang.org/docs/1.14/users-guide/index.html)
* additional highlights:
  - made significant improvements to the 'chplvis' execution analysis tool
    (see https://chapel-lang.org/docs/1.14/tools/chplvis/chplvis.html)
  - added support for a new .chplconfig file supporting default CHPL_* settings
    (see https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html)
  - clarified errors related to modifying 'const' intent-based shadow variables
  - updated versions of: jemalloc, GASNet, qthreads, hwloc, massivethreads
    (see $CHPL_HOME/third-party/*)
  - closed several classes of memory leaks, most notably due to sync/single vars
  - improved the portability of the code base
  - numerous bug fixes

Configuration Changes
---------------------
* added support for a new .chplconfig file supporting default CHPL_* settings
  (see https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html)
* added an '--anonymize' flag to 'printchplenv' to hide potential sensitivities
* enabled 'jemalloc' by default on 'darwin' platforms using 'gnu' compilers
* enabled 'jemalloc' for target compilers 'pgi' and 'cray-prgenv-cray'
* changed the 'numa' locale model to use the 'distrib' scheduler for 'qthreads'

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* Chapel now permits trailing commas within tuples and array literals
  (see 'Tuple Values' and 'Rectangular Array Literals' in the language spec)
* made casts between 'real' and 'imag' types preserve floating point values
* added support for empty 'only'/'except *' qualifiers on 'use' statements
  (e.g., 'use M only ;' or 'use M except *;' requires qualified naming)
* added a new reduce= operator for intent-based reductions
  (see https://chapel-lang.org/docs/1.14/technotes/reduceIntents.html)
* made assigning stridable ranges and domains to non-stridable an error
* stopped implicit 'use' of standard automatic modules within user sub-modules

Syntactic/Naming Changes
------------------------
* added the ability to specify an optional 'do' on 'otherwise' statements

New Features
------------
* added a capability for generating stack traces on 'halt's
  (see https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html#chpl-unwind)
* added support for iterating over, and querying the 'size' of enumerated types
  (see 'Enumerated Types' in the language specification)
* added support for recursive record/class initializers
* added new .bulkAdd()/+= features for sparse domains supporting bulk index adds
  (see https://chapel-lang.org/docs/1.14/builtins/ChapelArray.html)
* added support for reduce intents with different input/state/output types
* added an 'align' operator for domains
  (see 'Domain Alignment' in the language specification)
* added casts and safeCasts to turn stridable ranges/domains into non-stridable
* added support for 'retType' and 'argTypes' queries to first-class functions
  (https://chapel-lang.org/docs/1.14/technotes/firstClassFns.html#reflection)
* added the ability to create serial iterators for types
  (see 'The Method Receiver and the 'this' Argument' in the language spec)
* added early prototype support for class/record initializers (constructors)
  (see https://github.com/chapel-lang/chapel/blob/master/doc/chips/10.rst)

Feature Improvements
--------------------
* added support for a requestCapacity() method on associative domains
  (see https://chapel-lang.org/docs/1.14/builtins/ChapelArray.html)
* added the ability to query locality information of domains and distributions
  (see https://chapel-lang.org/docs/1.14/builtins/ChapelArray.html)
* improved the support for compilerError, compilerWarning, and compilerAssert
  (see https://chapel-lang.org/docs/1.14/builtins/UtilMisc_forDocs.html)
* made promoted array assignments always use a parallel loop
* extended the number of indices that an associative domain can store
* added the ability to have 'param' formal arguments on parallel iterators
* preserved 'param' values within the context of 'forall' loops
* added support for casting from 'param' values to arrays

Interoperability Improvements
-----------------------------
* added the ability to pass Chapel functions to extern C routines
  (see https://chapel-lang.org/docs/1.14/technotes/extern.html#c-fn-ptr)
* added support for casts to 'string' from 'c_void_ptr' and 'c_ptr'

Standard Modules/Library
------------------------
* added a new 'BigInteger' module providing a value-based 'bigint' type
  (see https://chapel-lang.org/docs/1.14/modules/standard/BigInteger.html)
* strings:
  - improved the flexibility of arguments passed to join() calls
    (see https://chapel-lang.org/docs/1.14/builtins/String.html#String.string.join)
  - made a version of split() that handles arbitrary whitespace
    (see https://chapel-lang.org/docs/1.14/builtins/String.html#String.string.split)
  - made the 'ascii()' routine return uint(8)s rather than int(32)/int(64)s
  - changed the default binary string format to data_toeof
  - removed most library functions on c_strings
* Math module: added support for standard operations on 'complex' values
  (see https://chapel-lang.org/docs/1.14/modules/standard/Math.html)
* Buffers module: added support for buffer.copyin/copyout methods
  (see https://chapel-lang.org/docs/1.14/modules/standard/Buffers.html)
* Reflection module:
  - added getFieldRef() functions to obtain a reference to a field
    (see https://chapel-lang.org/docs/1.14/modules/standard/Reflection.html)
  - added the ability to query whether a type method can be called
    (see https://chapel-lang.org/docs/1.14/modules/standard/Reflection.html)
* DynamicIters module: added a dynamic iterator for domains
  (see https://chapel-lang.org/docs/1.14/modules/standard/DynamicIters.html)
* GMP module: deprecated the 'BigInt' class in favor of 'bigint'

Package Modules
---------------
* added initial support for a 'BLAS' module supporting the level 3 routines
  (see https://chapel-lang.org/docs/1.14/modules/packages/BLAS.html)
* added an MPI module supporting message passing between Chapel images/locales
  (see https://chapel-lang.org/docs/1.14/modules/packages/MPI.html)
* added a ZeroMQ module supporting inter-application communication via 0MQ
  (see https://chapel-lang.org/docs/1.14/modules/packages/ZMQ.html)
* added support for a 'MatrixMarket' module providing file serialization
  (see $CHPL_HOME/modules/packages/MatrixMarket.chpl)
* added a new 'RangeChunk' package for dividing ranges into multiple chunks
  (see https://chapel-lang.org/docs/1.14/modules/packages/RangeChunk.html)
* Sort module: dramatically revamped the module, including comparator support
  (see https://chapel-lang.org/docs/1.14/modules/packages/Sort.html)
* Search module: dramatically revamped the module, including comparator support
  (see https://chapel-lang.org/docs/1.14/modules/packages/Search.html)

Domain Maps (Layouts and Distributions)
---------------------------------------
* added a new 'StencilDist' distribution in support of stencil access patterns
  (see https://chapel-lang.org/docs/1.14/modules/dists/StencilDist.html)
* added support for Block-distributed sparse domains and arrays
  (see https://chapel-lang.org/docs/1.14/modules/dists/BlockDist.html)
* extended Block distributions to support strided bounding boxes

Performance Optimizations/Improvements
--------------------------------------
* optimized array accesses for programs that use simple/common array operations
* optimized the performance of promoted expressions over aligned data
* optimized memory management through new jemalloc upgrades and features
* optimized reductions by using an atomic spin lock rather than a sync var
* enabled the bulk transfer of strided array slices by default
* implemented 'sync' variables natively for 'qthreads' tasking
* optimized functions returning references to avoid widening them when possible
* optimized on-statements that end up being local
* optimized the implementation of decrementing remote task counters
* stopped heap-allocating variables due to on-clauses for 'fifo' and 'muxed'
* improved the performance of intrinsic-based floating-point atomic 'fetchAdd's
* fixed a potential performance issue when growing/shrinking arrays as vectors
* made modest improvements to the performance of associative domains
* optimized the implementation of .re and .im for complex values
* optimized base**exp operations when 'base' is a param power of two

Memory Improvements
-------------------
* obtained memory allocation improvements through 'jemalloc' upgrades and usage
* closed memory leaks caused by 'sync'/'single' variables
* closed a memory leak relating to 'args' arguments sent to main()
* closed a memory leak caused by module-scope detuple variable declarations

Example Codes
-------------
* added all remaining Computer Language Benchmark Game programs
  (see $CHPL_HOME/examples/benchmarks/shootout/* and
   http://benchmarksgame.alioth.debian.org/)
* significantly improved existing Computer Language Benchmark Game programs
  (see $CHPL_HOME/examples/benchmarks/shootout/*)
* made improvements to LCALS for style and performance
  (see $CHPL_HOME/examples/benchmarks/lcals/*)
* changed ISx to use low-level PCG interface to match the reference version
  (see $CHPL_HOME/examples/benchmarks/isx/*)

Tool Changes
------------
* 'chplvis' improvements:
  - added a "settings" window
  - fixed undetected 'chplvis' regressions in 1.13
  - made significant improvements to the user interface
  - added new views of execution-time data
  (see https://chapel-lang.org/docs/1.14/tools/chplvis/chplvis.html)
* 'chpldoc' improvements: improved formatting and handling of many situations

Documentation
-------------
* added the primer example codes to the online documentation
  (see https://chapel-lang.org/docs/1.14/primers/index.html)
* added the 'hello world' examples to the online documentation
  (see https://chapel-lang.org/docs/1.14/examples/index.html)
* added a new primer example for modules and 'use' statements
  (see doc/release/examples/primers/modules.chpl)
* added a number of new sections to the user's guide
  (see https://chapel-lang.org/docs/1.14/users-guide/index.html)
* significantly re-worked the multi-locale execution and quickstart docs
  (see https://chapel-lang.org/docs/1.14/usingchapel/multilocale.html and
   https://chapel-lang.org/docs/1.14/usingchapel/QUICKSTART.html)
* moved information about execution on UDP-based systems into its own document
  (see https://chapel-lang.org/docs/1.14/platforms/udp.html)
* added usage ('use ...') information to chpldoc-generated module documentation
  (e.g., see https://chapel-lang.org/docs/1.14/modules/standard/Assert.html)
* added documentation for the 'chplvis' file format
  (see $CHPL_HOME/tools/chplvis/TextDataFormat.txt)
* updated the steps required to build Chapel with newer HDFS sources
  (see https://chapel-lang.org/docs/1.14/modules/packages/HDFS.html and
   https://chapel-lang.org/docs/1.14/technotes/auxIO.html#setting-up-hdfs)
* added an archive of past language specification versions to the online docs
  (see https://chapel-lang.org/docs/1.14/language/archivedSpecs.html)
* updated and fixed minor issues in the language specification
* fixed a plethora of spelling mistakes throughout the documentation
* fixed several online documentation formatting errors

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* added a new --denormalize flag that cleans up generated code before codegen
* stopped throwing --vectorize when --fast is thrown

Portability Improvements
------------------------
* improved the portability of the code base to gcc 6.x
* improved portability to FreeBSD, Solaris, illumos, OmniOS, Ubuntu
* added initial support for compiling single-locale programs for 64-bit ARM

Cray-specific Changes
---------------------
* removed an obsolete warning about having hugepage modules loaded on Crays

Syntax Highlighting
-------------------
* added instructions for using vim highlighting for Vundle and vim-plug users
* fixed portability of Chapel's emacs mode for emacs 24.x
* added support for nested block comments to emacs and 'highlight' highlighting
* fixed the highlighting of binary literals in Pygments
* updated our support for Andre Simon's highlighter to support version 3.x

Error Message Improvements
--------------------------
* added a clarifying statement for 'const' errors due to task/forall intents
* made string OOB messages print the offending index
* squashed C warnings considered to be noise in the back-end compiler
* added an error message for declaring constructors/initializers without parens
* added an error for user fields named 'outer'
* fixed some spelling errors in error messages
* removed warnings for assigning serial iterators/iterables to arrays
* removed a misleading warning about records and forall intents

Runtime Error Checks
--------------------
* added bounds checking to string index/slice operations

Bug Fixes
---------
* fixed a bug in which having CONFIG_SITE set could break third-party builds
* made 'compare_exchange' conform to memory order requirements on failure
* fixed incorrect 'fetchAdd' and 'fetchSub' implementations on atomics
* fixed a bug in which 64-bit atomic loads on 32-bit systems were not atomic
* fixed a longstanding but infrequent race condition in privatization
* fixed a bug in handling 'ref' return intents
* fixed a bug involving 'ref's to array elements
* fixed a bug for recursive functions that return records
* fixed a bug in accessing overridden methods
* fixed a bug in which reduce intents could not be applied to arrays
* fixed a bug in iterating over strided, unsigned domains and arrays
* fixed a bug in BlockCyclic array accesses for 3D+ arrays
* fixed a bug for membership queries in sparse CSR layouts
* fixed a bug when storing max(int) in an associative domain of ints
* fixed a bug in which arrays within records were incorrectly being localized
* fixed a bug in which (+/-1)**k was incorrect for negative values of k
* fixed a bug in advancing I/O channels
* fixed bugs in the mapping of GMP functions down to the underlying C library
* fixed a bug in which readline()'s 'amount' argument was used incorrectly
* fixed a bug in which writeln(str) and writef("%s", str) behaved differently
* fixed a bug related to using an 'enum' within a library module
* fixed a bug ignoring whitespace when casting strings to values
* fixed a bug in which the back-end compiler would warn about dead code
* fixed a bug in which 'printchplenv --sh' was missing some variables
* reduced cases when standard modules were automatically 'use'd in user code
* fixed some issues with copy propagation
* fixed an occasional compile-time failure in inserting wide references
* fixed a bug in which sleep() could under-sleep by 1 microsecond
* fixed some bugs with the --llvm-wide-opt flag
* fixed a race condition in the 'pbs-aprun' launcher
* improved how we handled environment variables with backtics in 'amudprun'
* fixed a bug related to locale models and order of resolution

Launchers
---------
* added an --spmd flag to the 'mpirun' launcher for SPMD Chapel+MPI runs
* added --walltime, --partition, --exclude flags to 'slurm_gasnet_ibv' launcher

Runtime Library Changes
-----------------------
* added the ability to implement atomics using C atomics ('cstdlib')
  (see https://chapel-lang.org/docs/1.14/technotes/atomics.html and
   https://chapel-lang.org/docs/1.14/usingchapel/chplenv.html#chpl-atomics)
* re-enabled support for massivethreads for single-locale executions
* added support for out-of-segment non-blocking puts and gets for 'gasnet'
* changed the 'fifo' tasking layer to allocate task stacks in heap memory

Generated Code
--------------
* improved generated code readability when using the new --denormalize flag
* added function IDs and a function ID -> name table to the generated code
* enabled the LLVM back-end compiler to produce debug information
* improved #line directives in the generated code when using --cpp-lines
* improved the generated code quality for 'local' blocks

Third-Party Software Changes
----------------------------
* added 'libunwind' as a new third-party package in support of backtraces
* upgraded jemalloc to version 4.2.1
* upgraded GASNet to version 1.26.4
* upgraded qthreads to version 1.11
* upgraded hwloc to version 1.11.4
* updated massivethreads to a new version
* enabled decay-based purging in jemalloc
* improved 'jemalloc's detection of 'librt' when using cray-prgenv-* compilers
* disabled 'jemalloc's statistics gathering features by default
* added support for the new Qthreads 'distrib' scheduler

Testing System
--------------
* added a 'close' button to the performance graphs to simplify screening them
* improved support for 'notest'ing and 'skipif'ing directories
* added the ability to perform multiple trials in correctness testing
* added the ability to limit the number of concurrently executing tests
* added the ability to squash email notifications by setting CHPL_TEST_NOMAIL
* fixed graph annotations for days without any associated data
* removed the need to provide an argument to '-[no]stdinredirect
* improved how compilation errors are reported for performance testing
* made the test system accept empty .compopts/COMPOPTS files
* avoided the use of spool files when using moab/torque via 'chpl_launchcmd.py'

Removal of Deprecated Features
------------------------------
* removed 'locale.numCores' which was deprecated in 1.13
* removed 'typeToString()', deprecated in 1.13 in favor of casts to strings
* removed support for 'setter', 'reader, 'writer', deprecated in 1.13
* removed support for 'fieldNumToName' & 'fieldValueByNum', deprecated in 1.13
* removed the deprecated 'AdvancedIters' module
* removed support for KNC

Developer-oriented changes: Module changes
------------------------------------------
* significantly refactored sparse domain map modules
* reimplemented 'sync' and 'single' variables as records
* made string code call more directly to mem* routines in C
* made the chpl_here_* routines more generic w.r.t. integral types
* added a 'PODValAccess' config param that returns POD array elements by value
* added a method to the 'bytes' type to get a raw pointer to memory
* adjusted printing within 'DefaultRectangular' to avoid IO dependencies
* cleaned up the use of '_desync' functions
* unified the formatting of the 'ChapelSyncVar' module

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* removed no-longer-used RUNTIME_GEN_C[XX]_FLAGS macros

Developer-oriented changes: Compiler Flags
------------------------------------------
* re-enabled support for the --minimal-modules flag
* added a --[no-]parse-only flag to optionally stop compilation after parsing
* added an experimental --incremental flag that generates multiple .o files
* added an experimental flag for performing CSE on array accesses
  (see --[no-]replace-array-accesses-with-ref-temps

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* moved the handling of PROC_NEW to the resolution pass
* cleaned up the representation and handling of varargs
* refactored the remote value forwarding pass
* refactored the gatherCandidates stage of function resolution
* fixed a bug in tryStack when resolving constructor calls
* optimized away the creation of tuples for varargs functions in some cases
* refactored CallExpr::codegen()
* revised isSyncType() to be more consistent and meaningful
* moved the "direct on" optimization to the parallel pass
* stopped treating 'defaultInitializer' specially for iterators
* fixed some filename arguments that were still strings rather than 'int's
* stopped unnecessarily remote value forwarding values to task functions
* improved how sync/single operations are identified by remote value forwarding
* changed how the compiler represents parallel iterable expressions
* improved the number and quality of checks performed by the --verify flag
* replaced uses of homegrown containers with STL equivalents in the compiler
* added QualifiedType support to prepare for revamping representation of 'ref's
* fixed the indentation of --html output when viewed within Google Chrome
* added support for a simple 'ForallExpr' AST node, used only during parsing
* implemented certain reduction expressions using forall expressions

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added callbacks to the Chapel runtime's communication calls
* simplified and sped up the implementation of chpl_executeOnNB()
* eliminated type punning and use of volatile in the implementation of atomics
* refactored 'extern "C"' declarations to avoid crossing #include boundaries
* changed uses of 'atomic_flag's to 'atomic_bool's to match C11's atomics

Developer-oriented changes: Documentation
-----------------------------------------
* added developer support for console-/man-based chpldocumentation
  ('make man-chapel')

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* changed many third-party directory structures to avoid using version numbers



version 1.13.1
==============

Update to sixteenth public release of Chapel, June 2016
(see also changes below for 1.13.0)

Bug Fixes
---------
* fixed a race condition causing lossy behavior in reductions as of 1.13.0
* fixed bugs in extern block support for C function pointers and structs
* restored support for --cpp-lines when not saving the generated C code
* fixed a bug in the remove() method on arrays
* fixed a bug relating to the index sets used by vector operations on arrays
* fixed an idxType/eltType mix-up in 'shuffle()' in the 'Random' module
* fixed an issue with closing a spawn channel before wait()ing
* fixed a bug in open() when a path the path was stored remotely
* fixed a bug with the printing of real floating point values
* fixed a bug in the GMP BigInt.mod_ui() function
* fixed some bugs in the congruent family of functions in the 'GMP' module

Feature Improvements
--------------------
* add support for casting from c_string to c_void_ptr

Standard Library/Modules
------------------------
* added bounds-checks to vector operations on arrays by default

Memory Improvements
-------------------
* closed memory leaks within the 'RandomStream' class



version 1.13.0
==============

Sixteenth public release of Chapel, April 7, 2016

Highlights
----------
* string improvements:
  - dramatically improved string performance and reduced memory leaks
  - added standard library routines on strings
    (see https://chapel-lang.org/docs/1.13/modules/internal/String.html)
* language feature highlights:
  - added support for filtering and renaming symbols with the 'use' statement
    (see 'The Use Statement' in the Chapel language specification)
  - added support for min/max reduce intents and reduce intents on coforalls
    (see https://chapel-lang.org/docs/1.13/technotes/reduceIntents.html)
  - significantly improved memory management and reduced leaks for record types
  - added support for creating subclasses of generic classes
  - replaced 'locale.numCores' with a richer 'locale.numPUs()' interface
    (see https://chapel-lang.org/docs/1.13/modules/internal/ChapelLocale.html)
  - added the ability to 'use' an enumerated type to avoid qualifying names
    (see 'The Use Statement' in the Chapel language specification)
  - added support for casting types to strings and deprecated typeToString()
    (see 'Explicit Type to String Conversions' in the Chapel language spec)
* standard library highlights:
  - added a new 'Reflection' module to support reasoning about code
    (see https://chapel-lang.org/docs/1.13/modules/standard/Reflection.html)
  - added a new JAMA-based Linear Algebra matrix package
    (see modules/packages/LinearAlgebraJama.chpl)
  - added a new PCG-based random number generator to 'Random' as the default
    (see https://chapel-lang.org/docs/1.13/modules/standard/Random.html)
  - extended the 'HDFS' module to support libhdfs3
    (see https://chapel-lang.org/docs/1.13/technotes/auxIO.html)
  - added additional routines to the 'FileSystem' and 'Path' modules
    (see https://chapel-lang.org/docs/1.13/modules/standard/FileSystem.html
     and https://chapel-lang.org/docs/1.13/modules/standard/Path.html)
* implementation improvement highlights:
  - made Chapel's default allocator 'jemalloc', resulting in better performance
    (see https://chapel-lang.org/docs/1.13/usingchapel/chplenv.html#chpl-mem)
  - 'complex' types are now code generated as C99 complex rather than a record
  - added compiler flags for all of the traditional CHPL_* environment vars.
    (see 'Compiler Configuration Options' in 'chpl --help' or the man page)
* added initial support for Intel Xeon Phi Knights Landing (KNL) processors
  (see https://chapel-lang.org/docs/1.13/platforms/knl.html)
* added new Chapel ports of the ISx and LCALS proxy applications to the release
  (see examples/benchmarks/isx and examples/benchmarks/lcals/)
* documentation highlights:
  - dramatically improved the organization of the online Chapel documentation
    (see https://chapel-lang.org/docs/1.13/)
  - created web-based versions of the 'chpl' and 'chpldoc' man pages
    (see https://chapel-lang.org/docs/1.13/usingchapel/man.html
     and https://chapel-lang.org/docs/1.13/tools/chpldoc/man.html)
* performance improvement highlights:
  - made significant performance improvements for common use cases of 'reduce'
  - implemented amortized reallocation when resizing arrays due to vector ops
  - made significant performance improvements for 'ugni' communication on Crays
  - broadened the applicability of the bulk communication optimization
  - significantly improved the performance resulting from the LLVM back-end
* additional highlights:
  - renamed the 'etc/' directory 'highlight/' to better identify its contents
    (see highlight/README)
  - made Chapel's scripts compatible with key versions of Python 2 and 3
  - unified the support of third-party packages and runtime options on Crays
    (see  https://chapel-lang.org/docs/1.13/platforms/cray.html)
  - numerous error message improvements, bug fixes, feature improvements, ...

Configuration Changes
---------------------
* made 'jemalloc' the default allocator for most configurations
  (see https://chapel-lang.org/docs/1.13/usingchapel/chplenv.html#chpl-mem)
* made 'cray-prgenv-cray' default to using 'qthreads'+'hwloc' and 'ugni'
  (see  https://chapel-lang.org/docs/1.13/platforms/cray.html)
* added support for 're2' when using Cray and PGI compilers
* changed 'knc' to default to using 'qthreads'+'hwloc'
* deprecated 'knc' support
  (see  https://chapel-lang.org/docs/1.13/platforms/knc.html)
* switched to using pgc++ rather than pgCC as the C++ compiler for 'pgi'
* removed stale support for Xcode

Syntactic/Naming Changes
------------------------
* added new keywords, 'as', 'except', and 'only', for filtering 'use'd symbols
  (see 'The Use Statement' in the Chapel language specification)
* replaced 'locale.numCores' with a richer interface 'locale.numPUs()'
  (see https://chapel-lang.org/docs/1.13/modules/internal/ChapelLocale.html)
* removed the 'Writer' and 'Reader' types in favor of unifying on 'channel's

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* added support for a 'const ref' return intent
  (see "The Const Ref Return Intent" in "Procedures" in the Chapel spec)
* improved the handling of "return by ref" functions in setter/getter contexts
  (see 'https://chapel-lang.org/docs/1.13/language/evolution.html
   and 'The Ref Return Intent' under 'Procedures' in the language spec)
* made default I/O routines ignore 'param' fields as they have 'type' fields
* restricted 'sync'/'single' types to sufficiently simple types
* removed support for performing Chapel I/O on 'c_string's
* removed coercions from 'c_string's to 'string's

New Features
------------
* added a standard library of routines on the 'string' type
  (see https://chapel-lang.org/docs/1.13/modules/internal/String.html)
* added support for filtering on 'use' statements with 'only' and 'except'
  (e.g., 'use M only foo;' or '... except foo;' to restrict to / exclude 'foo')
  (see 'The Use Statement' in the Chapel language specification)
* added support for renaming symbols made accessible by a 'use'
  (e.g., 'use M only foo as M_foo;' causes 'foo' to be referred to as 'M_foo')
  (see 'The Use Statement' in the Chapel language specification)
* added the ability to 'use' an enumerated type
  (e.g., "...mycolor.green..." can now be written "use mycolor;  ...green...")
* added support for casting types to strings and deprecated typeToString()
  (e.g., 'writeln(x.type:string);' can be used to print the name of x's type)
* added a capability to iterate over the fields in a class or record
  (see https://chapel-lang.org/docs/1.13/modules/standard/Reflection.html)
* added the ability to query whether a call can be resolved or not
  (see https://chapel-lang.org/docs/1.13/modules/standard/Reflection.html)
* added support for annotating an on-clause as being 'local'
  (see https://chapel-lang.org/docs/1.13/technotes/local.html)
* added support for 'c_string' values
  (e.g., "this is a Chapel string" while c"this is a C string")

Feature Improvements
--------------------
* added support for creating subclasses of a generic class
* added support for min/max reduce intents
  (see https://chapel-lang.org/docs/1.13/technotes/reduceIntents.html)
* added support for reduce intents on coforall loops
  (see https://chapel-lang.org/docs/1.13/technotes/reduceIntents.html)
* added the ability to pass sync/single variables via generic arguments
* improved the type signatures of min()/max() to support better type resolution
  (see https://chapel-lang.org/docs/1.13/modules/internal/UtilMisc_forDocs.html)
* extended comparison operators (==, !=, <=, etc.) to support mixed int/uints
  (see 'Expressions' in the Chapel language specification)
* added support for range slicing between mixed int/uint idxTypes
* added support for comparing between 'c_void_ptr' expressions and 'nil'
* added support for implicitly coercing 'c_ptr' types to 'c_void_ptr'
* added support for casting from fully-qualified enum strings to enum values
  (e.g., '"mycolor.green":mycolor' now works where it wouldn't have previously)
* extended channel.readbits/writebits to accept other integral types
  (see https://chapel-lang.org/docs/1.13/modules/standard/IO.html)
* added scalar versions of domain.exterior(), interior(), and translate()
  (see https://chapel-lang.org/docs/1.13/modules/internal/ChapelArray.html)

Standard Library/Modules
------------------------
* started distinguishing between "standard Chapel modules" and "packages"
  (see https://chapel-lang.org/docs/1.13/modules/modules.html
   and https://chapel-lang.org/docs/1.13/modules/packages.html)
* added a new JAMA-based linear algebra matrix package
  (see modules/packages/LinearAlgebraJama.chpl)
* added a new PCG-based random number generator to 'Random' as the default
  (see https://chapel-lang.org/docs/1.13/modules/standard/Random/PCGRandom.html)
* added a new 'Reflection' module supporting reflection about a program
  (see https://chapel-lang.org/docs/1.13/modules/standard/Reflection.html)
* extended the 'HDFS' module to support libhdfs3
  (https://chapel-lang.org/docs/1.13/technotes/auxIO.html)
* added support for skipping unknown fields when reading JSON records/classes
  (see https://chapel-lang.org/docs/1.13/modules/standard/IO.html)
* made the default I/O routines for records/classes support arbitrary orders
* added 'basename', 'dirname', and 'splitPath' to the 'Path' module
  (see https://chapel-lang.org/docs/1.13/modules/standard/Path.html)
* added rmTree(), moveDir() to the 'FileSystem' module
  (see https://chapel-lang.org/docs/1.13/modules/standard/FileSystem.html)
* added an optional time units argument to 'sleep()' in the Time module
  (see https://chapel-lang.org/docs/1.13/modules/standard/Time.html#Time.sleep)
* added support for sending signals to subprocesses for the 'Spawn' module
  (see https://chapel-lang.org/docs/1.13/modules/standard/Spawn.html)
* added an optional 'buffer' boolean to the 'Spawn' module's wait() routine
  (see https://chapel-lang.org/docs/1.13/modules/standard/Spawn.html)
* added an isclosed() method to the 'channel' type
  (see https://chapel-lang.org/docs/1.13/modules/standard/IO.html)
* improved the 'List' module to support JSON format
* improved error handling in the IO and Spawn modules
* added the ability to call exit() without arguments for a 0 default status
* renamed 'AdvancedIters' to 'DynamicIters' to better reflect its role
* removed deprecated functions from 'Memory' module

Performance Improvements
------------------------
* obtained notable performance improvements from the switch to 'jemalloc'
* made significant performance improvements to common uses of 'reduce'
* implemented amortized reallocation when resizing arrays due to vector ops
* made significant performance improvements for 'ugni' communication on Crays
* made bulk data transfers more likely to fire by removing a conservative check
* enabled bulk data transfers between portions of the same array
* generally improved the performance of string-based computations
* optimized iteration over anonymous range values of the form 'lo..#size'
* improved the performance of code generated by the LLVM back-end

Memory Improvements
-------------------
* dramatically reduced the compiler-generated leaks due to strings and records
* significantly improved construction/destruction/copy behavior for records
* stopped heap-allocating variables referred to within 'begin' scopes
* stopped heap-allocating variables referred to within non-blocking on-clauses
* stopped heap-allocating index variables of coforall loops

Documentation
-------------
* dramatically improved the organization of the online Chapel documentation
  (see https://chapel-lang.org/docs/1.13/)
* created web-based versions of the 'chpl' and 'chpldoc' man pages
  (see https://chapel-lang.org/docs/1.13/usingchapel/man.html
   and https://chapel-lang.org/docs/1.13/tools/chpldoc/man.html)
* linked to the language specification and quick reference from the online docs
  (see https://chapel-lang.org/docs/1.13/language/spec.html
   and https://chapel-lang.org/docs/1.13/language/reference.html)
* converted a number of top-level README-style files to markdown format
  (e.g., see $CHPL_HOME/CHANGES.md, CONTRIBUTORS.md, PERFORMANCE.md, ...)
* moved the Chapel evolution page from chapel-lang.org to the online docs
  (see https://chapel-lang.org/docs/1.13/language/evolution.html)
* split the top-level README.rst file into README.rst and QUICKSTART.rst
* started writing a Chapel Users Guide, though much work remains
  (see https://chapel-lang.org/docs/1.13/users-guide/index.html)
* improved the accuracy of Chapel's prerequisites list
  (see https://chapel-lang.org/docs/1.13/usingchapel/prereqs.html)
* improved wordings and descriptions in the language specification
* fixed many instances of broken links / formatting in online documentation

Example Codes
-------------
* added new Chapel ports of the ISx and LCALS proxy applications to the release
  (see examples/benchmarks/isx/ and examples/benchmarks/lcals/)
* made correctness, performance, and style improvements to MiniMD in Chapel
  (see examples/benchmarks/miniMD/)
* extended the linkedList example program to support additional methods
  (see examples/programs/linkedList.chpl)
* fixed some typos and explanations in the primer examples
  (see examples/primers/*.chpl)
* updated example tests to reflect the changes to the language and libraries
* removed README.features from the examples subdirectory

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* added compiler flags for all of the traditional CHPL_* environment variables
  (see 'Compiler Configuration Options' in 'chpl's '--help' output or man page)
* made the --ldflags stack when used multiple times, similar to --ccflags
* made the --no-warnings flag reversible by changing it to --[no-]warnings

Execution Flags
---------------
* added support for specifying enum configs using fully-qualified names
  (e.g., '--myColor=color.red' is now supported where only '--myColor=red' was)

Environment Changes
-------------------
* the setchplenv[.bash|.fish|.sh] scripts can now be sourced from any directory
* removed the CHPL_THREADS environment variable

Directory Structure Changes
---------------------------
* renamed the 'etc/' directory to 'highlight/' to better identify its contents
* split 'modules/standard' into 'modules/standard' and 'modules/packages'

Cray-specific Changes
---------------------
* streamlined 'ugni' resource management, dramatically improving performance
* hugepages are now optional with the 'ugni' communication layer
  (see https://chapel-lang.org/docs/1.13/platforms/cray.html)
* improved the Spawn module to support 'ugni' when hugepages are not in use
* started statically linking 'chpl' in the Cray module to ensure portability

Portability Improvements
------------------------
* added initial support for Intel Xeon Phi Knights Landing (KNL) processors
  (see https://chapel-lang.org/docs/1.13/platforms/knl.html)
* made Chapel's scripts compatible with key versions of Python 2 and 3
* improved the portability of the code base to gcc 5.1
* improved the portability of the code base to newer versions of Cygwin

Tool Changes
------------
* 'chpldoc' improvements:
  - made modules automatically list any submodules that they contain
  - it now generates a doc page per module rather than .chpl source file
  - it now only documents files listed on its command line by default
  - added a --[no-]html option to opt in/out of the rst->html phase
* added basic support for folding for 'vim' users
* added an option to disable chplvis tracing for a given run in 'VisualDebug'
* improved 'chpltags' support for older versions of 'ctags'

Error Message Improvements
--------------------------
* improved 'const' checking for domains and arrays
* added an error message for variable declarations over incomplete generics
* stopped warning about reductions being serialized -- that's completely legal
* added an error message for non-extern functions with no-op bodies
* reduced the degree to which internal module line numbers are named in errors
* implemented an error message for strings containing bad escape sequences
* improved the error message for bad CHPL_RT_NUM_THREADS_PER_LOCALE values
* improved error messages for illegal/incorrect 'use' statements
* added an error message for references to fields within 'with' clauses
* added an error message for 'real*2' suggesting the user wants '2*real'

Bug Fixes
---------
* fixed a bug in applying reduce intents to forall loops over ranges
* fixed a bug in zippered iteration over equivalent associative domains/arrays
* fixed the ability to declare a 'ref' to a domain or array
* fixed a bug in which overridden/overloaded methods were not resolved properly
* fixed a bug in assigning between distributed domains and non- with arrays
* fixed a bug in the computation of 'maxTaskPar' for the numa locale model
* fixed a bug in which generic types were not printed properly
* fixed a bug in which assignment between generic records did not work properly
* fixed a bug in which extern type declarations in generic procedures broke
* fixed a bug with isHomogeneousTuple*() queries when passed non-tuples
* fixed a bug with self-references in module use lists
* fixed a bug when calling exit() without arguments
* fixed the runtime sizes of boolean literals of varying bit widths
* fixed a bug w.r.t. reduce intents interacting poorly with on-clauses
* fixed a bug in which returning tuples of arrays did not work
* fixed a bug in the initialization of nearly-maximally-sized arrays
* fixed a bug in the Block and Cyclic parallel iterators for arrays of arrays
* fixed a bug in which subtype checking now handles transitive instantiation
* fixed bugs in default I/O routines for subclasses
* fixed a few bugs in the Spawn module's communicate() routine
* made some improvements to the Spawn module relative to QIO
* made some I/O functions more tolerant of EINTR
* fixed some bugs in which I/O routines executed on the wrong locale
* fixed a bug in formatted reads of digits
* fixed a bug preventing channel.itemWriter() from working
* improved the handling of C escapes in string literals
* fixed the handling of escape characters in compilerWarning()/compilerError()
* fixed a bug related to nested modules sharing a name, each defining main()
* fixed a bug in which Qthreads tasks weren't allocating sufficient storage
* fixed the line numbers reported for standalone iterators
* fixed a bug in which record fields were not properly widened
* fixed a bug in which records could be double-freed for out/inout arg passing
* fixed a bug in 'chpldoc' in the handling of multiline mid-variable comments
* fixed a bug in which failure to open files for codegen caused internal errors
* made the compiler resilient to usernames containing backslashes
* fixed a bug in the checking of definitions made in SysCTypes
* fixed the chpl-venv path to be based on the host platform rather than target
* fixed line numbers related to domain query expressions in formal arguments
* fixed a bug in which sleep() could result in tasks awakening prematurely

Cray-specific Bug Fixes
-----------------------
* 'ugni' now supports communication to/from any unregistered address
* resolved a rare program hang in 'ugni' communication
* fix 'ugni' memory mis-registration under CLE 6.*
* plug a memory leak in 'ugni' for certain remote on-statements

Third-Party Software Changes
----------------------------
* turned on optimizations when compiling 're2' for optimized builds
* enabled 're2' when using Cray and PGI compilers
* added jemalloc version 4.0.4 and retired tcmalloc and dlmalloc
* upgraded GASNet to version 1.26.0
* upgraded hwloc to version 1.11.2
  - and cherry-picked a /proc/mounts buffer overflow fix from hwloc master
* upgraded LLVM to version 3.7
* switched several third-party directories to a more Git-friendly organization

Runtime Library Changes
-----------------------
* updated the names of several runtime communication interface routines

Generated Code Cleanups
-----------------------
* started code-generating 'complex' types as C99 complexes rather than a record
* filenames are now represented a table + indices rather than string literals

Testing System
--------------
* added support for many files, such as compopts, to be executable scripts
* added .compenv files as a means of changing the compiler environment
* made 'make check' clean up after itself better
* made the test system no longer mail out the list of unresolved future tests
* parallel testing now ignores symbolic links to avoid infinite loops
* made the test system deal better with unexpected failures
* improved start_test's error message if CHPL_HOME is not set

Makefile Changes
----------------
* added an "always build chpldoc" capability via CHPL_ALWAYS_BUILD_CHPLDOC
* only generate 'tags' commands when CHPL_DEVELOPER or TAGS is set at make-time
* improved the logic for updating/re-building the 'chpl' version number

Developer-oriented changes: Process Improvements
------------------------------------------------
* changed the generation of the man page from txt2man into an rst-based scheme
* added a 'make check' option for Sphinx documentation to check for errors

Developer-oriented changes: Packaging Changes
---------------------------------------------
* reorganized files to better support packaging of Chapel
* began differentiating runtime launcher paths based on CHPL_COMM
* no longer specialize runtime library directories on CHPL_NETWORK_ATOMICS

Developer-oriented changes: Module improvements
-----------------------------------------------
* made use of 'local' on-clauses within internal modules targeting 'numa'
* changed a "c for loop" in DefaultRectangular into a direct range iterator
* refactored generation of unbounded range values
* changed some uses of c_calloc() to c_malloc()
* improved the extern declarations of the locale model chpl_mem* routines
* retired the ChapelLocale_forDocs.chpl workaround for documentation

Developer-oriented changes: Compiler Flags
------------------------------------------
* added --break-on-codegen-id to stop when a specific ID is code generated

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* introduced a new 'UseStmt' AST node for representing 'use' statements
* made all non-'flat' locale models outline on-clauses and use wide pointers
* removed the pragma form of 'export'
* added support for operator overloads on the bitVec type
* reworked how argument bundles are managed for begin- and on-functions
* changed wide_get_address mechanisms to return a 64-bit uint
* refactored duplicate code in buildForLoopExpr
* made addArgCoercion() sensitive to errors
* improve the compiler's handling of PRIM_ASSIGN
* improved the compiler's handling of POD types
* improved the compiler's handling of declared return types
* made the compiler remove casts from 'ref T' to 'T'
* unified the handling of 'by' and 'align' in the parser
* improved the handling of string varargs with default intent
* improved the normalization of initialization expressions
* added support for tracking the causes of wide pointers
* refactored the implementation of some reductions to use reduce intents
* removed the FLAG_FUNCTION_PROTOTYPE flag as not being interesting or useful

Developer-oriented changes: Generated code improvements
-------------------------------------------------------
* improved the representation of unions in the generated code

Developer-oriented changes: Runtime improvements
------------------------------------------------
* moved the Qthreads tasking shim into the Chapel runtime/ directory
* changed the communication interface to take sizes rather than elemsize+length
* fixed problems with GETs to unregistered remote memory
* simplified the interfaces for creating structured parallelism within Chapel
* removed deprecated code supporting Tilera processors
* extensively reworked debug support in 'ugni'

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* disabled OpenCL support in hwloc for the time being
* disabled libnuma in hwloc builds

Developer-oriented changes: Testing system improvements
-------------------------------------------------------
* added a script to help maintain .dat files when perfkeys change
  (see util/devel/updateDatFiles.py)
* renamed 'grepgoods' to 'greptestoutputs' and had it span .good and .bad files



version 1.12.0
==============

Fifteenth public release of Chapel, October 1, 2015

Highlights
----------
* significantly expanded the coverage of our online documentation
  (see https://chapel-lang.org/docs/1.12/ and "Documentation" below)
* added new 'Barrier', 'Spawn', and 'LAPACK' modules to the standard set
  (see "Standard Library/Modules" section below)
* added chplvis: a new tool for visualizing Chapel communication and concurrency
  (see https://chapel-lang.org/docs/1.12/tools/chplvis/index.html)
* added a new chapter describing Chapel's memory consistency model in detail
  (see 'Memory Consistency Model' in the language spec)
* added a 'vectorizeOnly()' iterator that vectorizes without task creation
  (https://chapel-lang.org/docs/1.12/modules/internal/ChapelIteratorSupport.html)
* new performance optimizations, particularly intra- and inter-node locality
  (see "Performance Improvements" section below)
* removed the 'local' block from stream-ep based on the above optimizations
  (see examples/benchmarks/hpcc/stream-ep.chpl)
* changed the default tasking layer on 'cray-x*' to 'qthreads' for performance
* added support for type methods and iterators
  (see 'Classes::Class Method Calls::The Method Receiver and the this Argument')
* added support for public/private module-level variables, functions, modules)
  (see 'Modules:Access of Module Contents::Visibility of Module's Symbols')
* domains in formal array arguments are now interpreted as a constraint
  (see 'Arrays::Array Arguments to Functions' in the language spec)
* significantly revamped and improved correctness and performance testing system
* improved how internal error messages are displayed to users
* added Chapel Improvement Proposals (CHIPs) framework for proposing changes
  (see https://github.com/chapel-lang/chapel/blob/master/doc/chips/1.rst)
* made more bug fixes than any release since version 1.1
  (see "Bug Fixes" section below)
* addressed portability issues for a number of platforms and compilers

Process Improvements
--------------------
* added Chapel Improvement Proposals (CHIPs) framework for proposed changes
  (see https://github.com/chapel-lang/chapel/blob/master/doc/chips/1.rst)

Environment/Configuration Changes
---------------------------------
* 'printchplenv' now indicates which settings come from environment variables
* made the quickstart scripts turn CHPL_LLVM off by default
* removed dependencies on 'bc' and 'patch'

Tool Changes
------------
* added chplvis: a new tool for visualizing Chapel communication and concurrency
  (see https://chapel-lang.org/docs/1.12/tools/chplvis/index.html)
* chpldoc improvements:
  - improved handling of enum, real, imag, and complex initializers
  - 'chpldoc' now generates values for enum symbols
  - automatically skip documentation of 'private' symbols
  - squashed generation of 'inline' in output
  - added support for --print-commands with chpldoc
* chpl-ipe:
  - added a quit() primitive for clean exits from the environment

Syntactic/Naming Changes
------------------------
* changed 'use' to 'require' for header files and external libraries
  (see https://chapel-lang.org/docs/1.12/technotes/extern.html)
* replaced the standalone format() routine, with string.format()
  (https://chapel-lang.org/docs/1.12/modules/standard/IO.html#IO.string.format)
* renamed memory diagnostics capabilities for clarity
  (see https://chapel-lang.org/docs/1.12/modules/standard/Memory.html)
* changed the '##.##' writef() pattern so that it requires '%{...}' around it
  (see https://chapel-lang.org/docs/1.12/modules/standard/IO.html#specifiers)
* added support for form-feed characters ('\f') as whitespace
  (see 'Lexical Structure::White Space' in the language specification)
* changed the term 'blank intent' to 'default intent'
  (see 'Functions::Argument Intents::Abstract Intents::The Default Intent')

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* domains in formal array arguments are now interpreted as a constraint
  (see 'Arrays::Array Arguments to Functions' in the language spec)
* Chapel no longer keeps referenced variables alive past their lexical scope
* c_strings are no longer made into strings when passed to generic extern args
* 'select' statements now only evaluate their argument expression once

New Features
------------
* added a 'vectorizeOnly()' iterator that vectorizes without using multitasking
  (https://chapel-lang.org/docs/1.12/modules/internal/ChapelIteratorSupport.html)
* added support for type methods and iterators
  (see 'Classes::Class Method Calls::The Method Receiver and the this Argument')
* added support for public/private module-level variables, functions, modules)
  (see 'Modules:Access of Module Contents::Visibility of Module's Symbols')
* added support for methods on enumerated types
  (e.g., 'enum color {red};' permits 'proc color.foo() { ... } color.red.foo();')
* added support for hexadecimal floating point literals
  (see 'Lexical Structure::Tokens::Literals' in the language specification)
* added the ability to rename extern records for use within Chapel
  (see 'Interoperation::...::Shared Types::Referring to External C Structs')
* added support for -lchplmalloc for compilers supporting weak symbols
  (see https://chapel-lang.org/docs/1.12/technotes/allocators.html)
* added the ability to use 'continue' statements in 'param' for-loops
* added support for == and != operators on domain maps

Standard Library/Modules
------------------------
* added a new 'Barrier' module defining a simple collective barrier capability
  (see https://chapel-lang.org/docs/1.12/modules/standard/Barrier.html)
* added a new 'Spawn' module for creating new subprocesses
  (see https://chapel-lang.org/docs/1.12/modules/standard/Spawn.html)
* added a new 'LAPACK' module that provides an interface to LAPACK
  (see https://chapel-lang.org/docs/1.12/modules/standard/LAPACK.html)
* added support for common mathematical constants to 'Math.chpl'
  (see https://chapel-lang.org/docs/1.12/modules/standard/Math.html#Math.e)
* replaced standalone format() routine with string.format() in 'IO'
  (https://chapel-lang.org/docs/1.12/modules/standard/IO.html#IO.string.format)
* unified string.format() format strings with writef()
  (https://chapel-lang.org/docs/1.12/modules/standard/IO.html#formatted-i-o)
* added a getFileSize() routine to the 'FileSystem' module
  (see https://chapel-lang.org/docs/1.12/modules/standard/FileSystem.html)
* merged the contents of the 'UtilMath' module into 'Math'
  (see https://chapel-lang.org/docs/1.12/modules/standard/Math.html)
* improved the 'Memory' module:
  - renamed memory diagnostics capabilities for clarity
  - added a --memLeaksByDesc flag to track only certain types of leaks
  - stopped tracking memory allocations not related to Chapel code by default
  (see https://chapel-lang.org/docs/1.12/modules/standard/Memory.html)
* errorToString(ENOERR) now portably returns "No error" in 'Error' module
  (see https://chapel-lang.org/docs/1.12/modules/standard/Error.html)
* applied the 'private' keyword to standard modules as appropriate/possible

Performance Improvements
------------------------
* parallelized large numeric array initialization for better first-touch
* improved analysis and optimization of variables that are provably local
* fixed a bug in which non-blocking 'on's were counted as local running tasks
* improved task counting when one task is waiting for others to complete
* optimized task counts to use processor atomics when profitable
* made 'cray-x*' systems default to 'qthreads' over 'muxed'
* moved polling threads to the last available CPU to avoid contention
* made array allocation use calloc(), not malloc() for better first-touch
* enabled more optimizations and streamlined code when using the LLVM back-end

Memory Improvements
-------------------
* stopped leaking sync and single variables on all platforms except Cygwin
* eliminated leaks for heap allocated variables used in cobegin/coforall

Documentation
-------------
* mirrored many of the READMEs from the release onto our online documentation
  (see https://chapel-lang.org/docs/1.12/usingchapel/ and .../1.12/technotes)
* added online documentation for built-in types, functions, methods
  (see https://chapel-lang.org/docs/1.12/modules/builtins.html)
* added online documentation for standard domain maps (layouts and distributions)
  (https://chapel-lang.org/docs/1.12/modules/distributions.html and layouts.html)
* added a copy of the online html documentation to the release itself
  (see $CHPL_HOME/doc/html)
* converted most READMEs to ReStructured Text (.rst) or markdown (.md)
  (for example, see $CHPL_HOME/doc/* and $CHPL_HOME/doc/technotes/*)
* refreshed the contents of most former READMEs as part of this effort
* language specification updates:
  - added a new chapter on Chapel's memory consistency model
  - added documentation of the I/O operator, <~>
  - improved the specification's page numbering and format
  - retired standard library chapters from the spec now that they're online
  (see https://chapel-lang.org/spec/spec-0.98.pdf)
* standard module documentation improvements
  - added a "Formatted I/O for C Programmers" table to the IO module
    (see https://chapel-lang.org/docs/1.12/modules/standard/IO.html#formatted-i-o)
  - refreshed the documentation of other standard modules
* refreshed the contents of the C interoperability technical note
  (see https://chapel-lang.org/docs/1.12/technotes/extern.html)
* retired README.format due to its documentation as part of a standard module
  (https://chapel-lang.org/docs/1.12/modules/standard/IO.html#IO.string.format)
* removed Tilera-specific documentation due to lack of use and testing

Example Codes
-------------
* rewrote the 'stream-ep' example to remove the now unnecessary 'local' block
  (see examples/benchmarks/hpcc/stream-ep.chpl)
* added a copy of Chapel code from learnXInYMinutes site to examples/primers
  (see examples/primers/learnChapelInYMinutes.chpl)
* added a primer for the new 'chplvis' tool
  (see examples/primers/chplvis/README)
* added a primer for the new LAPACK module
  (see examples/primers/LAPACKlib.chpl)
* added a new examples/patterns directory for storing common code patterns
  (see examples/patterns/README)
* added patterns/recordio.chpl to demonstrate reading tab-delimited records
  (see examples/patterns/recordio.chpl)
* updated shootout/nbody.chpl to use 'ref's rather than nested procedures
  (see examples/benchmarks/shootout/nbody.chpl)
* replaced uses of format() with writef() in SSCA2 benchmark
  (see examples/benchmarks/ssca2/)
* made the fileIO primer more tolerant of floating point roundoff
  (see examples/primers/fileIO.chpl)
* made the FFTW primer self-validating w.r.t. numerical differences
  (see examples/primers/FFTWlib.chpl)
* removed 'param' from lulesh loops that probably don't want/need it
  (see examples/benchmarks/lulesh/)

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* redefined --fast to no longer imply --no-ieee-float
* added --[no-]ieee-float support for CHPL_TARGET_COMPILER 'clang' and 'intel'
* made --ccflags arguments stack rather than override one another
* added support for --print-emitted-code-size for the LLVM back-end

Execution Flags
---------------
* added a --memLeaksByDesc flag to track only certain memory categories
* made --memLeaks[ByDesc] be governed by the --memThreshold flag

Error Message Improvements
--------------------------
* improved how internal error messages are displayed to users
* improved error messages for runaway comments
* improved source location provided by 'noinit' warnings

Bug Fixes
---------
* forall loops over a single array now correctly use the standalone iterator
* fixed bugs with applying reduce intents to forall loops over domains/arrays
* fixed bugs with scope resolution's choice of which symbol to resolve to
* fixed a bug in which overloads of '|' broke internal modules
* fixed a bug related to extern variables of types like c_ptr(c_int)
* fixed an internal error when a class calls its parent class's destructor
* fixed a bug in which large array copies could overflow runtime arguments
* fixed a bug in which calls of the form '<type>(x)' would cause internal errors
* fixed a bug in I/O when skipping whitespace encountered illegal characters
* fixed printing/reading of integers with '%{##.##}' format
* made readf() calls without error arguments halt on mismatches
* fixed a bug in which trailing whitespace was being consumed too aggressively
* made default rectangular arrays print in JSON as arrays-of-arrays
* fixed EOF-related bugs in Reader/Writer
* fixed a bug in which channel.read(style) was ignoring its style argument
* fixed a bug in which non-blocking 'on's were counted as local running tasks
* fixed a compiler exception caused by dividing by param '0'
* fixed a bug in the is*() FileSystem functions when given an invalid path/link
* improved chpldoc's handling of failure to create output and tmp directories
* added an error message for too-long compiler flags rather than truncating
* fixed a bug in which calls to malloc/free/etc in extern blocks caused errors
* fixed 'text file busy' errors in certain situations when invoking 'chpl'
* fixed a bug in which overloading was being permitted based on argument intents
* fixed a segfault in the glob() iterator when following 0..0 with 0 matches
* fixed a bug in which bad source location info was passed to string routines
* preserved source location information for cobegins
* fixed a bug relating to inlined iterators and generic array fields
* fixed a bug relating to passing c_strings to extern C functions

Platform-specific Changes
-------------------------
* made 'cray-x*' systems default to 'qthreads' over 'muxed'
* 'muxed' tasking now supports stack guard pages for non-hugepage heaps
  (see https://chapel-lang.org/docs/1.12/platforms/cray.html)
* added support for 'clang-included' to work with GASNet on Cray systems
* removed support for cray-prgenv-pgi from the module build on 'cray-x*'
* for cray-prgenv-cray compilations, stopped throwing -hipa2 by default

Platform-specific bug fixes
---------------------------
* fixed a stack underflow problem with 'muxed' tasking causing segfaults
* fixed a race in stack management for 'muxed' tasking

Portability Improvements
------------------------
* eliminated warnings related to [_BSD|_SVID]_SOURCE being deprecated
* improved portability of building the SysCTypes.chpl for Fedora 22
* removed symmetric address assumptions from error-handling code in the runtime
* fixed a number of I/O portability errors on cygwin
* fixed tcmalloc when compiled with clang 3.6 and used from C++
* fixed I/O for 32-bit Ubuntu 14.04
* added support for building GASNet with segment=fast on OS X
* fixed hwloc's cairo detection for certain OS X installations
* eliminated some Xcode-specific warnings

Third-Party Software Changes
----------------------------
* added 'fltk' to the third-party directory for use by 'chplvis'
* enabled the use of GMP with the LLVM back-end
* made LLVM build in non-debug mode and without asserts by default
* improved cross-compilation of third-party packages for 'cray-x*' platforms
* we're now storing RE2 in an unpacked form due to all of our local changes
* fixed a valgrind issue in RE2

Launcher-specific Changes
-------------------------
* improved 'slurm' launcher's handling of non-zero exit() codes
* changed how the 'amudprun' launcher deals with quoted arguments

Runtime Library Changes
-----------------------
* added support for out-of-segment put()/get() calls
* changed I/O to allocate buffer space out of the Chapel heap

Testing System
--------------
* rewrote 'start_test' and 'computePerfStats' in python
* replaced --suppress feature with .suppressif files
* added ability to write chpldoc tests using the .doc.chpl extension
* updated testing system to support testing of 'chpl-ipe'
* made the parallel testing system print an estimated end time
* added support for a -valgrindexe flag to 'paratest.server'
* made 'start_test' work when it's located outside of $CHPL_HOME/util
* added the ability to have multi-line COMPOPTS file
* improved support for #defines with quotes for C testing
* made C testing system more accepting of C compilers that print filenames
* specialized testing system to recognize some common launcher errors
* improved ability to use chpl_launchcmd.py from NFS-mounted Lustre filesystems
* extended timeout mechanism to work under Cygwin

Makefile Changes
----------------
* rewrote 'make check' to remove reliance on start_test/tcsh/python

Performance Testing/Graphing System
-----------------------------------
* added a 'screenshot' capability for performance graphs
* added the ability to annotate cray-xc performance graphs
* retired the old gnuplot-based graphing system
* reduced graphing system's reliance on CHPL_HOME being set
* improved graphing system's stability when there is no ANNOTATIONS.yaml file

Developer-oriented changes: Process Improvements
------------------------------------------------
* began using Jira for issue tracking

Developer-oriented changes: Documentation Improvements
------------------------------------------------------
* added notes about testing nightly scripts to util/cron/README

Developer-oriented changes: Compiler Flags
------------------------------------------
* added a --break-on-resolve-id flag for stopping when calling a function

Developer-oriented changes: Module improvements
-----------------------------------------------
* cleaned up how filename/line# information is attached to network atomics
* added support for casting directly to c_strings
* started using specific I/O read/write functions for int/real/imag types
* cleaned up handling of bool values in IO.chpl
* made initOnLocales() a standalone parallel iterator and added 'chpl_' prefix

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* added a top-level Makefile rule for building the parser

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* rewrote the compiler passes that widen references as necessary
* rewrote major portions of the scope resolution pass
* reduced reliance on homegrown ADTs with their STL equivalents
* improved the sharing of flags between 'chpl' and 'chpldoc'
* turned off sugaring of type names in --devel mode
* internal improvements to how tuples are handled
* removed unused flag FLAG_NO_USE_CHAPELSTANDARD
* renamed FLAG_PRIVATE to FLAG_LOCALE_PRIVATE for clarity
* retired the "auto destroy fn" pragma
* added tracking of zippered loops throughout compilation
* ensured the lexer returns a heap-allocated const char* for each literal

Developer-oriented changes: Runtime improvements
------------------------------------------------
* added support for task callbacks in the runtime
* eliminated reliance on `__always_inline`
* added chpl_mem_goodAllocSize() to runtime to support minimizing wasted memory
* added the ability to flag certain memory types to avoid tracking them
* added a capability to generate a header file #defining CHPL_* variables
* turned off remote caching for tasking layers where migration can occur

Developer-oriented changes: Third-party improvements
----------------------------------------------------
* cleaned up local modifications to Qthreads for contribution back to master
* updated third-party GMP/RE2 READMEs to refer to online module documentation



version 1.11.0
==============

Fourteenth public release of Chapel, April 2, 2015

Highlights
----------
* revamped 'chpldoc' and used it to create web docs for all standard modules
  (see https://chapel-lang.org/docs/)
* added a new FileSystem standard module supporting operations on files/dirs
  (see https://chapel-lang.org/docs/1.11/modules/standard/FileSystem.html)
* added a new FFTW standard module supporting key FFTW routines
  (see https://chapel-lang.org/docs/1.11/modules/standard/FFTW.html)
* added task intents to forall loops, plus new intents such as 'in' and 'reduce'
  (see 'Semantic Changes' and 'New Features' sections below)
* added support for standalone parallel iterators, used by non-zippered foralls
  (see examples/primers/parIters.chpl)
* added support for vectorization of data parallel loops and operations
  (see '--vectorize' flag in 'man chpl')
* optimized away communication code for module-scope variables and local arrays
  (see release note slides at https://chapel-lang.org/releaseNotes.html)
* added an early prototype interactive/interpreted mode for writing Chapel code
  (see doc/technotes/README.chpl-ipe.rst for more details)
* initial support for Python->Chapel interoperability via PyChapel
  (see http://pychapel.readthedocs.org/ for more details)
* made 'ugni'/'muxed' the default comm/tasking choices for cray-x* module users
  (see doc/platforms/README.cray and doc/README.chplenv)
* added a new 'chpltags' utility to generate emacs/vim tags for Chapel code
  (see 'util/chpltags --help' and 'util/README')
* numerous other improvements for better performance and cleaner generated code
  (see 'Performance Improvements' section below)
* improved portability to NetBSD and additional flavors of Linux
  (see 'Portability Improvements' section below)
* upgraded the bundled versions of GASNet, hwloc, llvm, and dygraphs
  (see 'Third-Party Software Changes' section below)
* numerous bug fixes
  (see 'Bug Fixes' section below)

Environment/Configuration Changes
---------------------------------
* made the setchplenv.* scripts prepend to PATH and MANPATH rather than append
* made the "quickstart" scripts set CHPL_COMM to 'none' and not set CHPL_MEM
* improved the logic for auto-detecting when CHPL_ATOMICS should be 'intrinsics'
* added auto-detection of CHPL_*_PLATFORM for 'cray-xc'/'cray-xe' systems
* added support for building runtime/third-party with the bundled clang version
  (see references to 'clang-included' in README.chplenv)
* split the 'cygwin' CHPL_*_PLATFORM value into 'cygwin32' and 'cygwin64'
* added new 'netbsd32' and 'netbsd64' values for CHPL_*_PLATFORM
* changed CHPL_COMM/TASKS to default to ugni/muxed when using the cray-x* module
  (see doc/README.chplenv, doc/platforms/README.cray, and/or doc/README.tasks)
* made CHPL_TASKS default to 'fifo' for 'cray-prgenv-cray' CHPL_TARGET_COMPILER

Packaging Changes
-----------------
* updated Makefiles to support building Chapel in parallel via 'make -j'
* avoid rebuilding gmp and re2 speculatively when previous builds have failed

Tool Changes
------------
* added an early prototype interactive/interpreted mode for writing Chapel code
  (see doc/technotes/README.chpl-ipe.rst for more details)
* revamped 'chpldoc' to create HTML files via Sphinx for documenting Chapel code
  (see doc/technotes/README.chpldoc.rst and 'man chpldoc' for more details)
* added a new 'chpltags' utility to generate emacs/vim tags for Chapel code
  (see 'util/chpltags --help' and 'util/README' for more information)

Syntactic/Naming Changes
------------------------
* removed the placeholder keyword 'refvar' in favor of the new keyword 'ref'
* removed support for the use of 'var' as a return intent (use 'ref' instead)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* added support for task intent clauses and semantics to forall loops
  (see 'Data Parallelism: Forall Intents' in the language specification)
* added initial support for 'reduce' intents on forall loops
  (see doc/technotes/README.reduceIntents)
* removed the 'type select x' syntax in favor of simply using 'select x.type'
* made read()s of sync/single variables result in a compiler error

New Features
------------
* added "blank", 'const', 'in', 'const in' and 'const ref' task intents
  (see 'Task Parallelism: Task Intents' in the language specification)
* added "standalone parallel iterators", used by non-zippered forall loops
  (see examples/primers/parIters.chpl)
* added support for aliasing of domains via the '=>' operator

New Interoperability Features
-----------------------------
* initial support for Python->Chapel interoperability via PyChapel
  (see http://pychapel.readthedocs.org/ for more details)
* added a prototype capability to embed external dependencies to source via 'use'
  (e.g., use "foo.h", "-lfoo"; is like adding these to the 'chpl' command-line)
* added support for passing multidimensional arrays to external routines
  (see doc/technotes/README.extern)
* added munging of all user identifiers by default to avoid naming conflicts
  (see --[no-]munge-user-idents in 'man chpl' and doc/README.executing)
* added an error when passing Chapel strings to C routines; use c_string instead

Standard Library/Modules
------------------------
* added a new module, FileSystem, containing numerous file system operations
  (see https://chapel-lang.org/docs/1.11/modules/standard/FileSystem.html)
* added support for key 64-bit FFTW routines (single- and multi-threaded)
  (see https://chapel-lang.org/docs/1.11/modules/standard/FFTW.html and
   https://chapel-lang.org/docs/1.11/modules/standard/FFTW_MT.html)
* added parity() and rotate() operations to the BitOps module
  (see https://chapel-lang.org/docs/1.11/modules/standard/BitOps.html)
* added parallel versions of the glob(), walkdirs(), and findfiles() iterators
  (see https://chapel-lang.org/docs/1.11/modules/standard/FileSystem.html)
* started a new module, Path, supporting realPath(), curDir, parentDir, pathSep
  (see https://chapel-lang.org/docs/1.11/modules/standard/Path.html)
* added a new module, Assert, defining a simple assertion capability
  (see https://chapel-lang.org/docs/1.11/modules/standard/Assert.html)
* added support for C types c_ptrdiff, c_uintptr, and c_intptr to SysCTypes.chpl
  (see https://chapel-lang.org/docs/1.11/modules/standard/gen/doc/SysCTypes.html)
* changed cwd() and chdir() from standalone functions to locale methods
  (see https://chapel-lang.org/docs/1.11/modules/standard/FileSystem.html)
* made mkdir(parents=true) behave more like 'mkdir -p'
* added a .safeCast() method to integer types to value-check downcasts
  (see https://chapel-lang.org/docs/1.11/modules/standard/Types.html)
* changed min(t) and max(t) to return 'param' values for integral types 't'
  (see https://chapel-lang.org/docs/1.11/modules/standard/Types.html)
* parameterized RandomStream by an 'eltType' and updated its methods accordingly
  (see https://chapel-lang.org/docs/1.11/modules/standard/Random.html)
* removed the parSafe arguments from RandomStream's methods in the Random module
  (see https://chapel-lang.org/docs/1.11/modules/standard/Random.html)
* added support for escaping pattern-style format strings using %{}
  (e.g., "writef("#%{##}", 10)" => "#10")
* added support for readf()/writef() of octal values
* changed 'BigInt' methods to use Chapel types rather than C types
* folded the prototypical 'Filerator' module into the new 'FileSystem' module
* retired the 'Containers' module in favor of vector operations on arrays
  (see examples/primers/arrayVectorOps.chpl)

Performance Improvements
------------------------
* added support for vectorization of data parallel loops and operations
  (see '--vectorize' flag in 'man chpl')
* optimized away the creation of anonymous ranges in loops
  (e.g., 'for i in 1..10' no longer creates an anonymous range object for 1..10)
* optimized away communication code for module-scope variables and local arrays
* optimized iterations over ranges in data parallel constructs like 'forall'
* added standalone parallel iterators for ranges and default domain maps
* extended loop-invariant code motion to include certain global variables
* added an 'assertNoSlicing' config that can be used to optimize array accesses
  (see $CHPL_HOME/PERFORMANCE)
* avoided creating new singleton tasks within serial execution contexts

Memory Improvements
-------------------
* reduced memory leaks due to compilerWarning()s and runtime string arguments
* distinguished between "owned" and "unowned" c_strings in implementation
* improved the handling of strings across multiple locales

Launcher-specific Changes
-------------------------
* in the 'pbs-aprun' launcher, removed reliance on the mpp* options
* added a --nodelist option to the 'slurm-srun' launcher
* in the 'slurm-srun' launcher, stopped using expect scripts

Platform-specific Changes
-------------------------
* fixed an issue in which Qthreads would not handle sleep() correctly for darwin
* added a micro-sleep when yielding threads on Cygwin to improve throughput
* changed CHPL_COMM/TASKS to default to ugni/muxed when using the cray-x* module
  (see doc/README.chplenv, doc/platforms/README.cray, and/or doc/README.tasks)
* enabled 'ugni' tasking to be usable with 'qthreads' as well as 'muxed'
* reduced default heap size used for CHPL_COMM='ugni' to 2/3 free node memory
  (see doc/platforms/README.cray)
* changed 'muxed' tasking to use # of threads equal to the # of CPUs by default
  (see doc/platforms/README.cray)
* disabled optimizations by default for cray-prgenv-cray compilations
* split the 'cygwin' platform value into 'cygwin32' and 'cygwin64'
* split the 'netbsd' platform value into 'netbsd32' and 'netbsd64'
* made CHPL_TASKS default to 'fifo' for 'cray-prgenv-cray' CHPL_TARGET_COMPILER

Portability Improvements
------------------------
* fixed several portability issues on NetBSD platforms
* fixed the use of the --llvm flag on Mac OS X ('darwin') platforms
* fixed some portability issues with the use of hwloc and -lnuma for 'qthreads'
* fixed some static/dynamic linking issues with certain platforms/compilers
* fixed a gmp build issue for 32-bit platforms

Third-Party Software Changes
----------------------------
* updated the bundled version of GASNet to 1.24.0
* updated the bundled version of hwloc to 1.10.1
* updated the bundled version of llvm to 3.6.0
* upgraded the bundled version of dygraphs to version 1.1.0
* added support for dynamically installing Python packages required by 'chpldoc'
  (see 'chpldoc-venv' in third-party/README)
* rearchitected scripting framework for third-party dependencies
* made the re2 build do a better job of propagating errors back to the Makefiles
* changed Qthreads to avoid using guard pages when mprotect() won't work
* fixed a bug in limiting the maximum size of Qthreads memory pools

Documentation
-------------
* added web-based docs for all standard modules and removed from spec/READMEs
  (see https://chapel-lang.org/docs/)
* made some minor improvements to the Quick Reference document
* added documentation for class and record destructors to the spec
* removed an outdated distinction between function calls using () vs. [] in spec
* clarified that integer literals may be 'int' or 'uint' in the spec
* added a note for 'zsh' users to the top-level README
* added a note to CONTRIBUTORS noting mis-attributed code authors within GitHub
* improved the documentation of the slurm* launchers
  (see doc/README.launcher, doc/platforms/README.cray)
* added a note about ugni memory registration and concurrency with slurm
  (see doc/platforms/README.cray)
* added a note about a longstanding undocumented feature to squash ref counting
  (see $CHPL_HOME/PERFORMANCE)

Example Codes
-------------
* added a new primer example demonstrating use of the FFTW module
  (see examples/primers/FFTWlib.chpl)
* added standalone parallel iterators to the leader-follower primer and renamed
  (see examples/primers/parIters.chpl)
* updated and renamed the chpldoc primer example to reflect chpldoc's changes
  (see examples/primers/chpldoc.doc.chpl)
* updated the random numbers primer to reflect changes to the interface
  (see examples/primers/randomNumbers.chpl)
* fixed a bug in our lulesh implementation when using the 3D representation
* updated miniMD's Stencil distribution to track changes in BlockDist
* changed miniMD to work around a bug in passing remote records by blank intent
* updated the HPCC FFT example to use new BitOps
* updated SSCA2 to reflect new intent semantics for forall-loops
* updated the regexdna shootout to accept extra arguments as used by framework
* made stream and ra save their problem sizes when performance testing
* replaced the use of format() in example codes with writef()

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* added --[no-]use-noinit to permit 'noinit' to be ignored
* added support for setting --fast via environment variable CHPL_FAST
* added --[no-]ignore-classes to disable local class optimizations
* added --[no-]vectorize to enable/disable generation of vectorization hints
* added --[no-]cast-checks to enable/disable value checking in safeCast methods
* added --[no-]munge-user-idents to control munging of generated identifiers
* removed all documentation-related options; use 'chpldoc' instead
* added a flag --[no-]task-tracking to enable/disable runtime task tracking

New Semantic Checks (for old semantics)
---------------------------------------
* improved 'const' checking for array/tuple fields and array/domain aliases

Error Message Improvements
--------------------------
* added an error when compiling for a configuration whose runtime is not built
* improved error messages for several range overflow conditions

Bug Fixes
---------
* fixed a bug with dynamically dispatched iterators
* fixed a bug with parallel iteration over non-naturally-aligned ranges
* fixed a bug in which record variables did not call user-defined constructors
* fixed a bug in which arrays of associative domains could cause a segfault
* fixed a bug in which user identifiers could easily conflict with C identifiers
* fixed a bug with formatted writes of integers using "###."
* fixed an infinite loop in binary reads of strings when EOF is reached
* fixed a bug preventing sparse domains from having 'uint' indices
* fixed a bug when casting param 'int(64)' and 'uint(?)' to 'c_string'
* fixed a bug in which secondary parentheses-less methods did not work well
* fixed a buffer overrun issue in certain I/O routines
* fixed a bug in which defining multiple destructors caused a segfault
* fixed some bugs in the standard Buffers module
* fixed a bug in which filenames generated by --cpp-lines would get corrupted
* fixed a bug in --cache-remote for 32-bit platforms
* fixed a bug with loop invariant code motion in the context of nested functions
* fixed a crash when using multiple formal type queries for primitives types
* fixed a bug in which iterators yielding locale types caused an internal error
* fixed a bug in which --no-remove-copy-calls would break certain extern records
* fixed a bug in the parser causing issues with &&= and ||= for --baseline
* fixed a bug in which developer pragmas were only attached to the first symbol

Platform-specific bug fixes
---------------------------
* fixed a bug in which I/O errors would print "unknown" for paths for 'cygwin'
* fixed a warning in our calls to glob() when compiling with 'prgenv-cray'
* fixed an issue in which generated identifiers could be too long for 'pgi'
* fixed a bug in which 'gmp' would not build cleanly for 32-bit Ubuntu systems
* fixed a bug in atomic operations on 'real' types for CHPL_COMM='ugni'

Runtime Library Changes
-----------------------
* enabled 'ugni' tasking to be usable with 'qthreads' as well as 'muxed'
* changed Qthreads to yield after uncontested sync var locks to avoid starvation

Compiler Performance
--------------------
* accelerated compilation times by disabling the task table by default
  (see '--[no-]task-tracking' on the 'chpl' man page and doc/README.executing)

Testing System
--------------
* added support for specifying 'skipif' files as executable scripts
* added a mode in which only futures with .skipif files are tested
* check for .skipif conditions before wasting any effort on a test
* switched to a consistent and simpler naming scheme for test configurations
* added timing measurements to the log for various aspects of testing
* revamped emails sent by testing scripts for maintainability and spam avoidance
* made the test system less sensitive to line/versions #s in .bad comparisons
* added a --test-root flag, improving start_test when run from outside CHPL_HOME
* fixed a bug in which '-nl' was passed to C tests for multilocale testing
* added support for jUnit XML-style report generation to start_test and paratest
* made paratest.server's compopts/execopts accumulate rather than overwrite
* added CHPL_TEST_UNIQUIFY_EXE to support simultaneous test runs on a directory
* added the ability to set the default testing timeout via CHPL_TEST_TIMEOUT
* added support for CHPL_TEST_NUM_TRIALS to be set by a user's environment
* squashed duplicate error messages due to sub_test failures
* fixed a bug with stdin redirects for tests with multiple .execopts
* made start_test smarter about stdin and launchers for multilocale testing
* improved the testing system's handling of timeouts when launchers are used
* added validation for CHPL_NIGHTLY_LOGDIR in nightly testing
* fixed a bug for tests with multiple execopts settings that timed out
* permitted suppression files to contain blank lines
* fix bug in which suppression-based issues only appeared in the test summary
* fixed a bad interaction between env. vars. set in sub_test vs. execenv files
* made sub_test ignore OS errors during unlinking to quiet false negatives

Performance Testing/Graphing System
-----------------------------------
* allow scrolling of the list of graphs independently from graphs themselves
* embed more information in the URL of performance graphs for sharing purposes
* improved the portability of URLs from the performance graphing system
* added support for 'today' as a date in the perf graph system's URL encodings
* accelerated graphs by moving sorting of graphs' legends into genGraphs script
* track top processes between performance runs to look for causes of noise
* improved support for comparing multiple performance testing configurations
* started logging the sha values for performance runs, for historical reference
* changed performance graphs to use json rather than csv
* automatically strip punctuation characters from the end of graph keys

Developer-oriented changes: Utility Changes
-------------------------------------------
* added a new compiler gdb command 'locid' which prints an AST's source location
* made build_configs.py support parallel execution

Developer-oriented changes: Compiler Flags
------------------------------------------
* added --report-order-independent-loops to note loops with vectorization hints
* added --remove-unreachable-blocks to attempt early removal of dead code
* fixed a code pointer in the --help message for --log

Developer-oriented changes: Module improvements
-----------------------------------------------
* added a pragma for asserting that a class field is local to the object
* moved routines in support of iterators to a new module ChapelIteratorSupport
* split _build_range() into several routines with better names
* refactored overloads of range operator definitions
* changed Filerator routines to use new isLink(), isDir(), and isFile() routines
* removed requirement to add 'no ChapelStandard' pragma to internal modules

Developer-oriented changes: Makefile improvements
-------------------------------------------------
* added a "WARNINGS" macro that can be used to turn on C-level warnings
* moved check for standard C headers into the post-commit smoke test
* simplified and regularized third-party Makefiles

Developer-oriented changes: Compiler improvements/changes
---------------------------------------------------------
* changed the parser to a "pure push" style of parser
* changed the compiler to require bison 2.5 (or later)
* dramatically reworked representation and manipulation of loop AST nodes
* made more AST members private and increased the use of query methods
* added a new, prototype AST pretty-printer for Chapel-oriented output
* made improvements to list_view()
* made flags on a symbol print in a deterministic order
* extended DeadCodeElimination to reclaim degenerate C for loops
* made 'narrowWideReferences' its own compiler pass and refactor it
* moved the 'docs' pass earlier in compilation
* reorder localCopyPropagation to avoid a ref-before-def bug
* improved verification of expressions to check correctness of AList links
* fixed a bug in which failed chpl__tryToken resolutions could roll back too far
* removed flags: FLAG_LABEL_BREAK, FLAG_LABEL_CONTINUE, FLAG_USER_NAMED
* distinguished FLAG_WIDE from FLAG_WIDE_REF
* deprecated the use of the MOD_MAIN tag
* simplified code related to calling user-defined constructors vs. _defaultOf
* converted the collapsing of nested BlockStmts to use a visitor
* added a predicate to determine if an expression is a "statement expression"
* converted some uses of C arrays to std::vector in compiler
* added binary operators to the BitVec implementation
* replaced the basic block macros with private static functions
* fixed a bug in basic block analysis
* moved the handling of boolean short-circuiting from 'parse' to 'normalize'
* renamed 'by' operators in the compiler's IR to avoid naming conflicts
* refactored fix_def_expr() in the normalize pass
* refactored constant folding vs. dead code elimination for CondStmts
* fixed several code issues identified by coverity scan
* many other code refactorings and improvements

Developer-oriented changes: Runtime improvements
------------------------------------------------
* simplified interactions between locale models and the runtime
* improved the non-blocking communication interface
* added support for the main process to run Chapel tasks
* guarded against some src==dst calls to memcpy()
* for optimized builds of the runtime, set NDEBUG to turn off assertions
* fixed several code issues identified by coverity scan

Developer-oriented changes: Generated Code improvements
-------------------------------------------------------
* improved the spacing when printing IDs via --gen-ids



version 1.10.0
==============

Thirteenth public release of Chapel, October 2, 2014

Highlights
----------
* changed Chapel's license and contributor agreement to Apache 2.0
  (see 'Licensing Changes' section below)
* made Qthreads the default tasking layer for Chapel on most platforms
  (see doc/README.chplenv and doc/README.tasks for details)
* added initial support for Intel Xeon Phi Knights Corner (KNC) processors
  (see doc/platforms/README.knc)
* added new standard modules for bit operations, prefetch, file/directory utils
  (see 'Standard Modules' section below)
* extended auxiliary file system support for HDFS to include Lustre and cURL
  (see doc/README.auxIO for details)
* added support for set-based operations on associative domains and arrays
  (see doc/technotes/README.sets and examples/primers/associative.chpl)
* added draft support for vector methods on 1D arrays with a personal domain
  (see examples/primers/arrayVectorOps.chpl)
* added the ability to query arrays for the subdomain(s) that the locale owns
  (see doc/technotes/README.subquery and examples/primers/distributions.chpl)
* added the ability to pass contiguous Chapel arrays to extern procedures
  (see doc/technotes/README.extern)
* performance boosts due to Qthreads tasking, improved LICM, less communication
  (see 'Performance Improvements' below)
* several generated code cleanups including C for loops and fewer ref temps
  (see 'Generated Code Cleanups' below)
* significantly improved const-ness checking for fields, indices, domains/arrays
* fixed the semantics for passing sync/single variables to generic arguments
* improved support for, and orthogonality of, constructor calls
* added a compiler flag to support specializing for the target architecture
  (see descriptions of CHPL_TARGET_ARCH and --[no-]specialize below)
* added symbolic CHPL_RT_NUM_THREADS_PER_LOCALE values to support common cases
  (see doc/README.tasks for details)
* added an --about flag to Chapel executables to print compilation information
* new primer examples for associative domains/arrays, vector ops, random numbers
  (see 'Example Codes' below)
* two new shootout benchmarks and many improvements to the existing ones
  (see 'Example Codes' below)
* split setchplenv.* source scripts into "quick start" and "production" modes
  (see $CHPL_HOME/README)
* by default, attempt to build and enable RE2 and GMP when possible
  (see doc/README.chplenv)
* updated versions of hwloc, tcmalloc, and gasnet third-party packages
  (see 'Third-Party Software Changes' below)
* numerous bug fixes
  (see 'Bug Fixes' below)
* many test system improvements including C-based testing and external timers
  (see 'Testing System' and 'Performance Testing/Graphing System' below)
* many developer process improvements such as GitHub, Travis, Coverity, Jenkins
  (see 'Developer-oriented Changes: Process Changes' below)
* many other developer-oriented improvements and refactorings
  (see other 'Developer-oriented Changes: ...' sections below)

Licensing Changes
-----------------
* changed Chapel's license from BSD to Apache 2.0
  (see $CHPL_HOME/LICENSE.chapel for details)
* changed Chapel's contributor agreement from a Cray agreement to Apache 2.0
  (see https://chapel-lang.org/developers.html for details)

Packaging Changes
-----------------
* split the util/setchplenv.* scripts into quick-start and "production" versions
  (see $CHPL_HOME/README and util/README for more information)
* for non-quickstart users, we now attempt to enable RE2 and GMP by default
  (see doc/README.chplenv for details)
* removed the util/update-sources script which had fallen out-of-date

Environment Changes
-------------------
* changed the default value of CHPL_TASKS to 'qthreads' for most platforms
  (see doc/README.chplenv and doc/README.tasks for details)
* added CHPL_TARGET_ARCH to describe the target processor architecture
  (see doc/README.chplenv for details)
* added CHPL_AUX* environment variables for accessing HDFS, Lustre, and/or cURL
  (see doc/technotes/README.auxIO)
* added symbolic physical/logical values for CHPL_RT_NUM_THREADS_PER_LOCALE
  (see doc/README.tasks for details)
* generalized CHPL_RT_MAX_HEAP_SIZE to control GASNet's segment size
  (see doc/README.executing for more information)
* generalized CHPL_RT_CALL_STACK_SIZE to control Qthread stack sizes
  (see doc/README.executing for more information)
* renamed the CHPL_MEM setting previously called 'default' to 'cstdlib'

Portability Improvements
------------------------
* added initial support for Intel Xeon Phi Knights Corner (KNC) processors
  (see doc/platforms/README.knc)
* fixed a bug in which newer versions of Cygwin weren't properly detected

Syntactic/Naming Changes
------------------------
* changed the syntax for task intents from 'ref(ident)' to 'with (ref ident)'
  (see the 'Task Parallelism' chapter of the language specification for details)
* deprecated 'var functions' in favor of returning by 'ref' intent
  (see the 'Procedures' chapter of the language specification for details)
* renamed the 'default' setting for CHPL_MEM to 'cstdlib'
  (see doc/README.chplenv for details)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* made passing syncs/singles to generic arguments pass them by 'ref'
  (previously, such calls would unwrap the sync/single, passing the value in)
* redefined op= on associative domains in terms of set operations
  (see doc/technotes/README.sets for more information)
* made loops over ranges that rely on overflow undefined
  (e.g., 'for i in 1..max(int) do ...' is not well-defined)
* make a more explicit distinction between the 'string' and 'c_string' types
  (see doc/technotes/README.extern for more information on 'c_string')
* added support for the logical negation of integral types
  (e.g., 'var x = 3;  if (!x) then ...' is now legal Chapel code)
* removed support for interpreting {D} as D for domain D
  (given 'const D: domain(...) = ...;', '{D}' is not a domain of domains)

New Features
------------
* added support for Lustre and cURL file systems
  (see doc/technotes/README.auxIO and README.curl for details)
* added support for standard set operations on associative domains and arrays
  (see doc/technotes/README.sets and examples/primers/associative.chpl)
* added support for extending an associative array's personal domain via stores
  (see examples/primers/associative.chpl)
* added draft support for vector methods on 1D arrays with a personal domain
  (see examples/primers/arrayVectorOps.chpl)
* added the ability to query arrays for the subdomain(s) that the locale owns
  (see doc/technotes/README.subquery and examples/primers/distributions.chpl)
* made the compiler call user-defined default constructors when defined
* added the ability to call constructors through type aliases
  (e.g., 'class C { ... }  type t = C;  var myT = new t();' now works)
* added support for numerous type-oriented queries for expressions
  (see doc/technotes/README.typeQueries)
* added octal literals
  (e.g., 0o777)
* added support for 'noinit' on record and class variables
  (see the 'Variables' chapter of the language specification)
* added experimental support for user-defined default values for record types
  (e.g., 'proc _defaultOf(type t) where t:R { var r: t; r = ...; return r; })
* added locale.maxTaskPar to query the # of parallel tasks a locale can support
  (see the 'Locales' chapter of the language specification)
* added initial support for 'ref' variables using a placeholder 'refvar' syntax
  (e.g., 'var x = 3; refvar y = x; x = 5; assert(y == 5);' will not fail)

Changes to the Implementation
-----------------------------
* improved the current support for HDFS file systems
  (see doc/technotes/README.auxIO and README.hdfs for more information)
* made 'dataParTasksPerLocale' default to 'locale.maxTaskPar'
  (see the 'Data Parallelism' chapter of the language specification)

New Interoperability Features
-----------------------------
* added the ability to pass contiguous Chapel arrays to extern procedures
  (see README.extern)
* extern C procedures should now take 'c_string' arguments rather than 'string'
* changed the return type of is_c_nil from c_int to bool

Standard Modules
----------------
* added a new standard module supporting some initial bitwise operations
  (see modules/standard/BitOps.chpl)
* added a new standard module supporting a prefetch() operation
  (see modules/standard/Prefetch.chpl)
* added some initial file/directory utility routines (more planned for 1.11)
  (e.g., remove(), rename(), cwd(), chdir(), mkdir(), chown()
* added draft support for glob() and walking directory trees
  (see modules/standard/Filerator.chpl -- feedback desired)
* added readstring()/readline() methods that read bytes into strings/arrays
  (see modules/standard/IO.chpl)
* added an experimental sorted() iterator for sorting arrays or other iterables
  (see modules/standard/Sort.chpl)
* added the ability to have the Sort.chpl module routines sort in reverse
* implemented new module NewString defining 'string_rec'
  (prototype to replace 'string' in version 1.11)
* added a printMemLeaks() routine for printing out a memory leaks table
  (see modules/standard/Memory.chpl)
* improved Random.chpl's SeedGenerator.currentTime to avoid duplicate seeds
* rewrote the AdvancedIters module to use 'atomic' rather than 'sync' vars
* removed the Profiling-related modules and source code

Documentation
-------------
* added doc/platforms/README.knc to describe using Chapel for KNC
* added doc/technotes/README.auxIO and .curl to describe new file system support
* added doc/technotes/README.sets to describe new associative domain set ops
* added doc/technotes/README.fileUtil to describe current file utility routines
* added doc/technotes/README.subquery to describe array subdomain queries
* added a description of CHPL_RT_MAX_HEAP_SIZE/GASNET_MAX_SEGSIZE to README.cray
* improved the specification of procedure arguments and returns
* added the proposed support for forall intents to the language specification
* fixed a spec code example that failed to use task intents as needed
* many updates, clarifications, and improvements to the language specification
* many updates and improvements to the READMEs in general

Example Codes
-------------
* added a new primer for associative domains/arrays and set operations on them
  (see examples/primers/associative.chpl)
* added a new primer for vector operations on 1D arrays
  (see examples/primers/arrayVectorOps.chpl)
* added a new primer for random number generation
  (see examples/primers/randomNumbers.chpl)
* added two new shootout benchmarks: meteor-fast, regexdna
  (see examples/benchmarks/shootout/meteor-fast.chpl and regexdna.chpl)
* replaced the fannkuch-redux benchmark with a parallel version
  (see examples/benchmarks/shootout/fannkuch-redux.chpl)
* updated the mandelbrot benchmark to use a dynamic iterator
  (see examples/benchmarks/shootout/mandelbrot.chpl)
* updated chameneos benchmark to use conditionals instead of select statements
  (see examples/benchmarks/shootout/chameneosredux.chpl)
* removed some CHPL_RT knob-fiddling from the thread-ring benchmark
* fixed the portability of the pidigits benchmark to 32-bit systems
  (see examples/benchmarks/shootout/pidigits.chpl)
* updated the fileIO primer to utilize/demonstrate whole-array I/O
  (see examples/primers/fileIO.chpl)
* improved the locking strategy used in verification for the HPCC RA benchmark
* removed the unnecessary locking and error tolerance from ra-atomics.chpl
* changed HPCC RA benchmarks to print problem sizes before declaring arrays
* minor edits to the domains primer

Source Highlighting Changes (see etc/README)
--------------------------------------------
* updated vim, emacs, highlight, and source-highlight modes with new keywords
* other general improvements to vim and emacs modes
* added a pointer to the Chapel support for pygments
  (see etc/pygments/README.rst)

Launcher-specific Notes
-----------------------
* improved the 'slurm-srun' launcher
* updated 'aprun' launcher to support KNC
* added environment forwarding for the 'amudprun' launcher

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* added a --[no-]specialize flag that supports CHPL_TARGET_ARCH specialization
  (see doc/README.chplenv for details beyond those on the man page)
* added a --[no-]stack-checks flag to the compiler
  (see README.tasks for details beyond those on the man page)
* made --fast imply --specialize and --no-stack-checks by default
* added a new --cache-remote flag to enable caching remote data
* --print-passes now prints additional timing information about passes

Flags on Generated Executables
------------------------------
* added an --about flag to print information about how a program was compiled

Error Message Improvements
--------------------------
* added an error message for function control paths that fail to return values
* added a warning if a hugepages module is/is not loaded as CHPL_COMM requires
* improved error messages for I/O failures
* improved the error message for assignments between unsupported type pairs
* improved resolution errors to distinguish between having 0 vs. 2+ candidates
* improved error messages for applying 'new' to incorrect expression types
* issue an error when using the --llvm flag when LLVM is not enabled
* fixed a warning message in which CHPL_GASNET_SEGMENT was mis-named

Performance Improvements
------------------------
* improved the performance of the Qthreads tasking layer
* improved the loop invariant code motion optimization for --no-local compiles
* improved the performance of the serial 1D array iterator
* optimized task counters to make use of network atomics when available
* reduced communication counts for several common code idioms
* added an experimental optimization for caching remote data with 'fifo' tasking
  (see --cache-remote flag on 'chpl' man page)
* improved the performance of readstring()

Third-Party Software Changes
----------------------------
* made many improvements to our use of Qthreads in the 'qthreads' tasking layer
* updated snapshot of hwloc to version 1.9.1
* updated snapshot of tcmalloc to version 2.2 and patched to work with Chapel
* applied GASNet patches to fix pshm gemini/aries memory registration issues
* improved the portability of the re2 code base and regexp feature
* parallelized the build of the llvm and gmp packages to reduce build time
* reduced the number of llvm components we build to reduce build time

New Semantic Checks (for old semantics)
---------------------------------------
* improved const-ness checking for 'const' fields, loop indices, domains/arrays
* added checks for assignments to illegal l-values via op= operators
* added semantic checks to prevent against overflow in range iteration

Bug Fixes
---------
* fixed a bug in which int literals > max(int(32)) could not coerce to uint(32)
* fixed support for uppercase binary and hexadecimal literals
  (e.g. 0B1101101 and 0Xbabe1055)
* fixed a bug in which ~1% of compiles would segfault on Macs
* fixed a bug relating to nested records and default constructors
* fixed a bug in which varargs functions with 'ref' intent did not work
* fixed a bug in which the compiler did not preserve param boolean sizes
* fixed a bug in 'noinit' initializations shared by multiple variables
* fixed a bug in which writef() was missing an overload
* fixed some bugs in readstring()
* fixed a race condition in associative domains' member() function
* fixed an internal error relating to record variables in param conditionals
* fixed some bugs in the GMP module
* fixed a bug in which multi-locale memory tracking was broken
* fixed a bug in loop-invariant code motion
* fixed several bugs in the LLVM backend

Runtime Library Changes
-----------------------
* refactored the runtime's auxiliary file system support to support many at once
* made the tasking layer specify the default value for dataParTasksPerLocale
* in 'qthreads' tasking, made the progress engine a pthread, not a task
* added new non-blocking communication functions to the runtime interface
* added yields to the communication layer's barriers to avoid wasting resources
* removed barrier-related communications from verbose comm output by default

Generated Code Cleanups
-----------------------
* changed the generated code to use C for-loops for ranges and certain arrays
* eliminated unnecessary reference temps in many common cases, like assignments
* added dead module elimination to reduce the amount of generated code
* removed extraneous chpl__readXX() calls from the generated code
* changed the mix of which compiler-generated functions are inlined vs. not

Compiler Performance
--------------------
* improved compilation speed by rewriting printchplenv and chplenv/* in Python
* consolidated the compiler's system calls to reduce compiler overhead

Testing System
--------------
* added the ability to add C-based tests to the test system
* added the ability for an *execopts file to redirect stdin as its last args
* added automatic squashing of internal module line numbers for .bad diffs
* the test system now logs a list of top CPU consumers if a test times out
* added a notion of a temporary test directory that is cleaned up after a run
* changed memory leaks test accounting to include strings
* added a --syspreexec flag to the start_test script
* added the ability to have locale-model-based .good files in testing
* added the ability to match against variants of an explicitly named .good file

Performance Testing/Graphing System
-----------------------------------
* add the ability to use different external timers in performance testing
* embedded the active graph list into the performance testing system's URLs
* removed duplicate checkboxes from the list of performance graphs
* fixed the support for annotating multiple graph series' at once
* made performance testing only run one trial for a higher-than-default timeout
* fixed a bug in which multiple performance test trials were replicating data
* made performance testing remove .exec.out.tmp on successful runs
* added the ability to graph .dat files listed by SVN revision rather than date
* made the default compiler performance directory unique by machine name
* avoided sync-ing performance graphs to the web if errors are encountered
* improved the behavior of the log-scale button's warning mode

Developer-oriented changes: Process Changes
-------------------------------------------
* migrated Chapel's repository from Subversion on SourceForge to git on github
* began using Travis for pre-commit sanity checks
* began using Coverity scan regularly to catch code quality issues
* converted testing from cron-based to Jenkins-based
* added a script to aid with building multiple Chapel configurations
  (see util/build_configs.py --help for more information)
* added copyrights to sources within git (rather than tagging at release time)

Developer-oriented changes: Compiler Flags
------------------------------------------
* added a new --lldb developer flag for debugging with LLDB
* added a --print-module-resolution developer flag
* changed developer --version numbers to display the sha1 + branch
* made minor improvements to the --minimal-modules compilation mode
* removed the developer --ignore-internal-modules flag

Developer-oriented changes: Executable flags
--------------------------------------------
* added a -E flag for launchers to forward environment variables to _reals

Developer-oriented changes: Module improvements
-----------------------------------------------
* added an ability for developers to access 'param' and 'type' fields in Chapel
* merged internal ChapelRange and ChapelRangeBase modules/types together
* converted remaining assignment ops to use the new '=(ref x, y)' signature
* rewrote assignments to use PRIM_ASSIGN rather than PRIM_MOVE
* made ioerror return without exiting on ENOERR
* added a remote prefetch primitive
* extended network atomic interface to accept memory_order (currently ignored)
* fixed the return types of non-fetch network atomic methods to be 'void'
* refactored the reference counting code for domain maps, domains, and arrays
* converted internal use of external procedures to use 'c_string', not 'string'
* refactored Memory module to avoid injecting extern symbols into global scope
* refactored string support into new files
  (see runtime/*/chpl-string.h/.c and modules/internal/String.chpl)
* moved privatized ID tracking from the runtime to the modules
* cleaned up deprecated file operations

Developer-oriented changes: Compiler improvements
-------------------------------------------------
* fixed a number of issues identified by Coverity and the Clang Static Analyzer
* created a "visitor" supporting a pre-order traversal of the IR
* rewrote the three viewing formats to use the new visitor capability
* added a PhaseTracker capability for taking timings within the compiler
* eliminated creation of order and coercion wrappers within compiler
* add memory fences to places that need them in the generated code and runtime
* added partial instantiation of generic functions to the resolution pass
* tightened up the use of function_exists() to avoid false matches
* significantly improved how default values are established by the compiler
* added an internal function for getting the first of an enum's values
* added a "ignore noinit" pragma to override noinit support
* squashed default initialization of certain compiler-introduced temp variables
* removed many compiler-introduced PRIM_MOVE operations
* improve how module uses are tracked and managed
* replaced the resolvedFns map with a flag on FnSymbols
* fixed the placement of an iterator tag argument
* removed some unnecessary scopeless block statements in the front-end
* replaced every procedure's nested BlockStmt with a single one
* moved module initialization function creation from parser to normalize
* various other code cleanups and refactorings

Developer-oriented changes: Compiler refactorings
-------------------------------------------------
* cleaned up the AST classes w.r.t. protected, private, const, etc.
* split statement AST nodes out from 'Expr' and into a new base class, 'Stmt'
* refactored view.cpp into three files matching its three viewing formats
  (see compiler/AST/AstDump.cpp, ASTDumpToHTML.cpp, and view.cpp)
* refactored and cleaned up code related to specifying and running passes
* cleaned up the compiler's timer class
* cleaned up arg.cpp
* refactored logging related code into log.h/cpp
* moved insertWideReferences into its own file
* replaced Symbol class bool fields with flags
* removed dead code relating to garbage collection and the early interpreter
* removed dead FLAGs and memory descriptor types

Developer-oriented changes: Runtime improvements
------------------------------------------------
* refactored task-local runtime data into a single structure
* made the runtime code more resilient to mixing C++/C compilers



version 1.9.0
=============

Twelfth public release of Chapel, April 17, 2014

Highlights (see entries in subsequent categories for details)
-------------------------------------------------------------
* numerous performance improvements (see 'Performance Improvements' below)
* significant reductions in compiler-introduced memory leaks
* added five new Computer Language Benchmark Games to the examples/ directory
* improved operator precedence for '|', '^', '&', '<<', '>>', '..', and 'in'
* added the ability for a user to create overloads of the assignment operator
* implemented atomic variables using intrinsics for Intel and Cray compilers
* flipped the default nested parallelism policy via dataParIgnoreRunningTasks
* significantly improved the stability/generality of the LLVM back-end
* added a slurm-srun launcher for use with native SLURM and improved pbs-aprun
* added a 'make check' rule to the top-level Makefile to validate a build
* improved the symmetry of program startup and polling across the locales
* significant improvements to the testing system, esp. performance testing
* updates to the GASNet and GMP packages and new snapshots of hwloc and re2
* improved the code base's portability w.r.t. clang, gcc, Mac OS X, Debian 7.4
* numerous bug fixes (see 'Bug Fixes' below)

Packaging Changes
-----------------
* added a 'make check' rule to the top-level Makefile to validate a build
* removed the half-hearted support for Chapel syntax highlighting in emacs 21

Environment Changes
-------------------
* added a new CHPL_HWLOC environment variable to control the use of 'hwloc'
  (see doc/README.chplenv)
* made CHPL_*_COMPILER default to 'clang' for CHPL_*_PLATFORM 'darwin'
* made CHPL_TASKS default to 'qthreads' when CHPL_LOCALE_MODEL == 'numa'
* made CHPL_HWLOC default to 'hwloc' when CHPL_TASKS = 'qthreads'
* established a 1:1 correspondence between CHPL_TASKS and CHPL_THREADS options
* deprecated the user-controlled CHPL_THREADS environment variable
* removed support for CHPL_TASKS=none due to lack of significant utility
* made GASNet use the 'large' segment by default for the 'ibv' conduit
* made CHPL_LAUNCHER default to 'gasnetrun_ibv' when using GASNet's mxm conduit
  (see doc/README.launcher)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
* improved operator precedence for '|', '^', '&', '<<', '>>', '..', and 'in'
  (see precedence tables in the 'Expressions' spec chapter or quick ref card)
* added the ability for a user to create overloads of the assignment operator
  (see 'Statements' chapter in the language specification)
* added a 'noinit' capability to squash default initialization for basic types
  (see 'Variables' chapter in the language specification)
* for a domain D, previously {D} == D; now it interprets it as 'domain(D.type)'
* added support for an expression-less 'serial' statement
  (i.e., 'serial do' == 'serial true do')
* added support for dynamic casts of the 'nil' value, producing 'nil'
  (see 'Conversions' chapter of the language specification)
* clarified that deleting a 'nil' value is OK and will have no effect
  (see 'Classes' chapter of the language specification)
* added the ability to mark the 'this' as having 'ref' intent for methods
  (see 'Classes' chapter of the language specification)

New Features
------------
* implemented support for the 'break' statement within param loops

Changes to the Implementation
-----------------------------
* dataParIgnoreRunningTasks is now 'false' by default for locale model 'flat'
  (see doc/README.executing for details)
* changed the default size of the call stack size to 8 MiB for all task options
  (see doc/README.tasks for details)

New Interoperability Features
-----------------------------
* extended c_ptrTo() to support 1D rectangular arrays
* added support for casts between c_ptr types

Standard Modules
----------------
* added support for abs() on imaginary types
* added isSubtype() and isProperSubtype() queries to the standard Types module
  (see 'Standard Modules' chapter of the spec for details)

Documentation
-------------
* added descriptions of 'atomic' variables and 'noinit' expressions to the spec
  (see the 'Task Parallelism and Synchronization' and 'Variables' sections)
* clarified specification of casting from numeric types to 'bool'
* reworked LICENSE files to clarify third-party licenses and isolate BSD text
  (see LICENSE and LICENSE.chapel)
* refreshed and reorganized README.tasks
* documented that 'clang' is available as a CHPL_*_COMPILER option
  (see doc/README.chplenv)
* improved description of Cray-specific runtime environment variables
  (see doc/platforms/README.cray)
* clarified formatted I/O documentation regarding width/precision
  (see doc/technotes/README.io)
* added a performance notes file (PERFORMANCE)
* removed the user agreement (AGREEMENT)
* generally refreshed README-based documentation
* general updates and improvements to the language specification

Example Codes
-------------
* added new Chapel ports of several Computer Language Benchmark Games (CLBG)
  (see spectralnorm.chpl, mandelbrot.chpl, fannkuchredux.chpl, meteor.chpl,
   and pidigits.chpl in benchmarks/shootout/)
* added an improved/simplified version of the CLBG chameneos-redux example
  (see benchmarks/shootout/chameneosredux.chpl)
* improved the release versions of RA to use atomic rather than sync vars
* made the examples/programs/tree.chpl example reclaim its memory
* made minor improvements to the MiniMD example and primer examples
* fixed a few incorrect statements in comments within primers/syncsingle.chpl

Cray-specific Notes
-------------------
* changed Cray XC Systems(TM) to use GASNet over the aries conduit by default
* added a slurm-srun launcher for use with Cray systems supporting native SLURM

Launcher-specific Notes
-----------------------
* added a slurm-srun launcher for use with native SLURM systems
* improved the pbs-aprun launcher for use with Moab/Torque
* made the gasnetrun_ibv launcher forward all CHPL_ environment variables
* made the 'aprun' launcher more careful about when it can correctly use '-j'

Portability of code base
------------------------
* improved code base's portability to newer versions of gcc
* improved code base's portability to Darwin/Mac OS X and Debian 7.4
* improved code base's portability for compilation with 'clang'
* enabled 'tcmalloc' to be built on Cray XC systems

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
* removed the --serial and --serial-forall flags; use serial statements instead
* started ignoring --static for Mac OS X since it isn't well-supported
* added a --print-passes-file flag to print passes to a specified filename

Error Message Improvements
--------------------------
* changed the wording of internal errors for succinctness

Performance Improvements
------------------------
* implemented atomic variables using intrinsics for Intel and Cray compilers
* optimized whole-array binary read/write operations for default domains/arrays
* extended global constant replication to additional types
* improved the compiler's ability to remote value forward values
* optimized away sublocale-related code for the 'flat' locale model
* improved 'numa' locale model performance for --local compilations)
* optimized blocking on sync variables for 'fifo' tasking based on # of tasks
* within serial sections, optimized forall loops over ranges, domains, arrays
* improved the task accounting for loops over Block-distributed arrays
* improved the loop-invariant code motion optimization's use of alias analysis
* removed unnecessary copies for formal arguments
* optimized program startup times in several ways

Locale Model Improvements
-------------------------
* improved the 'numa' locale model to reduce unnecessary changes in sublocale
* improved the 'numa' locale model's range, domain, and array iterators

Memory Improvements
-------------------
* reduced compiler-introduced memory leaks, particularly in I/O code
* reduced memory usage due to compiler-introduced copies on primitive types
* improved the reclamation of arrays of sync/single variables
* moved the end-of-program memory reporting to a point after the runtime exits

Third-Party Software Changes
----------------------------
* updated GASNet to version 1.22.0 with a patch to fix 'aries' conduit bugs
* added a snapshot of hwloc version 1.7.2 for compute node introspection
* added a snapshot of re2 (20140111) to the third-party directory
* added a snapshot of dygraphs to the third-party directory for perf graphs
* updated our snapshot of GMP to version 6.0.0
* various minor improvements to the Qthreads tasking layer
* disabled pshm for all non-udp GASNet conduits

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
* improved const-ness checking for const fields and const records/unions
* added a semantic check for tuple size mismatch when destructuring a tuple
* fixed a bug in which [u]int & imag literals were represented incorrectly
* fixed a bug where iterators with complex control flow could yield bad values
* fixed a bug in which timer.clear() did not reset the timer if it was running
* fixed a bug in which abs() on reals incorrectly called the complex version
* fixed a bug in converting between Chapel and C strings
* fixed a bug in which casts from ints/uints to bools were not always correct
* fixed some problems with the GMP random number routines
* fixed a bug on Cygwin for usernames with spaces in them
* extended global constant replication to additional types
* fixed a "read after freed" bug on domains used in nonblocking on calls
* fixed bug in loop invariant code motion related to aliasing in records/tuples
* fixed a subtle/minor race condition regarding accounting of tasks
* fixed Qthreads tasking layer bug resulting incorrect task placement
* fixed a bug in which Qthreads was asked for task-local storage prematurely
* fixed a potential race in task reporting (-t/--taskreport)
* fixed an optimization shortcut for array reindexing
* fixed a bug in which while loops warned about testing against local consts
* improved 'printchplenv' to avoid perl problems in unexpected cases

Runtime Library Changes
-----------------------
* improved the symmetry of program startup and polling across the locales
* improved descriptions of the runtime communication interface in chpl-comm.h
* simplified the implementation of the registry of global variables
* added ability for fifo tasks to implement thread-level storage using __thread

Generated Code Cleanups
-----------------------
* simplified the implementation of operations on homogeneous tuples
* removed the passing of wide strings by ref by default
* squashed the code generation of an unused program initialization function
* squashed redundant calls to initialize the ChapelStandard module
* folded out tautological comparisons between uints and 0s (e.g., myuint >= 0)

Compiler Performance
--------------------
* short-circuited the beautify pass if --savec is not specified
* short-circuited some logic in the parallel pass for --local compiles

Compiler Improvements
---------------------
* significantly improved the stability/generality of the LLVM back-end
* re-implemented copy propagation to handle aliases better
* made de-nested functions use 'const' arguments in more cases

Testing System
--------------
* added the ability to run multiple trials of each test to the testing system
* added support for sweeping current performance tests across past releases
* added the ability to track compiler performance during testing
* added a regexp-based correctness check capability for performance tests
* changed performance testing to support --fast by default
* added a script to splice .dat files created by performance testing
* permit SLURM to control the node list for parallel testing
* replaced 'paratest.server's -duplex flag with -nodepara for oversubscription
* added a capability to add annotations to performance graphs
* made performance testing compile with --static by default
* in generating performance graphs, the previous directory is now removed
* added a capability for the performance graphs to be rsync'd to SourceForge
* added a logarithmic/linear toggle to the generated performance graphs
* added a capability for the 'nightly' script to svn commit performance data
* added additional print messages to 'nightly' to better describe progress
* added a -retaintree option to 'nightly' to use the existing writable tree
* added support for testing the '--fast' flag in the 'nightly' script
* worked on making the testing system less Cray-centric in its design
* made test scripts send mail to the sourceforge mailing lists by default
* added options for naming test suites, specifying recipients, etc.
* unified the naming and structure of the cron job testing scripts
* removed reliance on tcsh-specific features for improved csh portability

Makefile Changes
----------------
* made all builds update Makefile dependencies, not just developer builds
* made Makefiles propagate CFLAGS/CXXFLAGS to third-party builds

Internal/Developer-oriented
---------------------------
* added dataPar* arguments to these() iterators for ranges/default rectangular
* made Block's leader iterator forward to DefaultRectangular
* made sure that arguments to exported functions are always local/narrow
* changed most assignment operator signatures to take the LHS by 'ref'
* added support for a "minimal modules" compilation mode for core language work
* added a developer flag --report-optimized-loop-iterators to track loop opts
* made internal errors appear as such in developer mode
* refactored the reference count code for domain maps, domains, and arrays
* switched to a symbolic initializer for locale IDs to improve flexibility
* refactored QIO and Regexp reference counting
* deprecated the internal InitPrivateGlobals module
* added config params to support program startup communication diagnostics
  (see 'printInitVerboseComm' and 'printInitCommCounts')
* added a verification pass to ensure ref types for return types are available
* renamed NUM_KIND_FLOAT to NUM_KIND_REAL
* renamed 'Class'->'Aggregate' in the compiler sources when record|class|union
* switched to a bulk copy of flags when copying Symbol classes
* changed the representation of intents to support bitmasks
* moved the initialization of memory tracking from the modules to the runtime
* removed user-level memory leaks from example codes in the language spec
* created FLAG_COMPILER_GENERATED to separate functions/labels from FLAG_TEMP
* added PRIM_IS_ATOMIC_TYPE to query whether something is an atomic type
* added new primitives for querying whether a type is a tuple, sync, or single
* improved the internal use cases of 'printchplenv'
* removed redundant overloads of ==/!= for syserr and err_t
* improved the implementation when 'noRefCount' is true
* removed no-longer-necessary _ensure_reference_type() feature
* changed extern routines that take string arguments to take 'c_string' instead
* changed extern routines that take 'inout' arguments to 'ref' when appropriate
* numerous refactorings of the compiler code for clarity and/or efficiency



version 1.8.0
=============

Eleventh public release of Chapel, October 17, 2013

Highlights (see entries in subsequent categories for details)
-------------------------------------------------------------
- added initial support for user-defined locale models and a NUMA locale model
- added initial support for accessing HDFS file systems via files/channels
- improved tasking semantics w.r.t. references to variables from outer scopes
- added new example codes for MiniMD and the Computer Language Benchmark Game
- performance optimizations related to array base pointers, loop-invariant code
- removed a number of compiler-generated memory leaks
- added new types for interoperating with C: c_string, c_ptr(type), c_void_ptr
- improved support for small-int computations, ranges, domains, and arrays
- support for dygraph-based performance monitoring in the testing system
- updates to the GASNet, Qthreads, MassiveThreads, LLVM, and CLANG packages
- added a new 'cray-cs' platform to represent Cray Cluster Supercomputers
- made 'gemini' the default GASNet conduit for 'cray-xe' and 'cray-xk'
- added support for a SLURM + GASNet-over-Infiniband launcher

New Features
------------
- added initial support for specifying locale models ('hierarchical locales')
  (see doc/technotes/README.localeModels)
- added support for a prototype NUMA locale model
  (see doc/technotes/README.localeModels)
- added initial support for accessing HDFS file systems via files/channels
  (see doc/technotes/README.hdfs)
- added new 'const in' and 'const ref' argument intents
  (see the 'Argument Intents' section in the language specification)
- added built-in config params for all CHPL_* environment settings

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- improved tasking semantics w.r.t. references to variables from outer scopes
  (e.g., in 'var x=0; begin { ...x... }', 'x' is a const copy within the task)
  (see 'Task Intents' section in the language specification)
- added 'ref' clauses to obtain the previous reference tasking semantics
  (e.g., 'var x: int; begin ref(x) { ...x... }' causes '...x...' to refer to x)
  (see 'Task Intents' section in the language specification)
- made blank intents for atomic variables get interpreted as 'ref'
  (see the 'Argument Intents' section in the language specification)
- added support for ranges, domains/arrays with small-int index types
  (e.g., 'range(int(8))' is now a supported type)
- added operator support for pairs of small int/uint types
  (e.g., 'int(8) + int(8)' now generates an int(8) rather than int(32))
- added operator support for homogeneous tuple/scalar pairs
  (e.g., (1,2,3) + 1 => (2,3,4))
- extended the member() method on domains to support varargs as well as tuples
  (e.g., 'var D = {1..n, 1..n};' supports 'D.member(i,j)' and 'D.member((i,j))')
- the compiler now only creates '==' operators for matching record types
- made 'bool' types prefer dispatching to 'int' args over smaller-int options
  (e.g., foo(true) will prefer to dispatch to foo(x:int) over foo(x:int(8)))

New Interoperability Features
-----------------------------
- added support for 'c_ptr(type)' and 'c_void_ptr' to refer to local C pointers
  (see doc/technotes/README.extern)
- added support for a 'c_string' type to refer to local C char* pointers
  (see doc/technotes/README.extern)

Deprecated Features
-------------------
- removed experimental GPU work to prepare for a revised approach to GPU support
- removed the callStackSize() query from the locale type
- removed certain config consts to control runtime arguments
  (see "Flags on Generated Executables" section below)

Standard Modules
----------------
- added a new 'UtilReplicatedVar.chpl' standard module for replicated variables
- new 'HDFSiterator' and 'RecordParser' modules to help with parsing HDFS files

Documentation
-------------
- added a new README to describe the new locale model concept and NUMA model
  (see doc/technotes/README.localeModels)
- added a new README.hdfs file to describe HDFS file system access in Chapel
  (see doc/technotes/README.hdfs)
- made significant improvements to the README.cray file
  (see doc/platforms/README.cray)
- refreshed the 'chpl' man page, particularly the SYNOPSIS/ENVIRONMENT sections
- refreshed the Chapel Quick Reference document
- major updates to the 'Task Parallelism' section of the spec for task intents
- made minor updates to several chapters in the language specification
- updated README.chplenv to reflect new CHPL_* environment variables
- minor updates to various README files

Example Codes
-------------
- added an initial port of the Mantevo MiniMD miniapp to Chapel
  (see examples/benchmarks/miniMD)
- added Chapel ports of four codes from the Computer Language Benchmark Game
  (see examples/benchmarks/shootout)
- fixed a bug in the atomics.chpl primer example
- cleaned up the beer.chpl example

Cray-specific Notes
-------------------
- added a new 'cray-cs' platform to represent Cray Cluster Supercomputers (CS)
  (see doc/platforms/README.cray for more information)
- made 'gasnet' the default CHPL_COMM setting for all Cray platforms
- made 'gemini' the default GASNet conduit for 'cray-xe' and 'cray-xk' platforms
  (see doc/platforms/README.cray)
- added support for the GASNet 'aries' conduit for the 'cray-xc' platform
  (see doc/platforms/README.cray)
- made 'aprun' the default launcher for 'cray-x*' systems when available
- removed support for the 'xmt', 'x1', and 'x2' platforms and related features
- generated warning messages on cray-x* systems when no PrgEnv is loaded
- removed -target=linux flag from cray-prgenv-* compiler options

Launcher-specific Notes
-----------------------
- added support for a SLURM + GASNet-over-Infiniband launcher
  (see 'slurm-gasnetrun_ibv' in doc/README.launcher)
- made the aprun-based launchers use the system's default number of CPUs
- brought the pbs launchers up-to-date with the current NCCS wrapper of qsub

Portability of code base
------------------------
- worked around a PGI portability issue with C structs in the generated code

Compiler Flags (see 'man chpl' for details)
-------------------------------------------
- added --[no-]warn-const-loops to control warnings about loops over constants
- added --[no-]loop-invariant-code-motion to enable/disable that optimization
- added --[no-]llvm-wide-opt to enable/disable LLVM wide pointer optimizations
- added warnings to '--[no-]serial' indicating its likely deprecation

Flags on Generated Executables
------------------------------
- replaced flags that govern runtime behavior with environment variables:
    callStackSize       -> CHPL_RT_CALL_STACK_SIZE
    commConcurrency     -> CHPL_RT_COMM_CONCURRENCY
    numHardwareThreads  -> CHPL_RT_NUM_HARDWARE_THREADS
    numThreadsPerLocale -> CHPL_RT_NUM_THREADS_PER_LOCALE

Error Message Improvements
--------------------------
- improved const checking for actuals passed to out/inout/const ref/ref intents
- improved error messages for programs with bad type resolution cycles
- added a warning for while loops over constants
- errors for assignments to non-lvalues no longer halt compilation immediately

Performance Improvements
------------------------
- improved performance of local arrays by storing a shifted base pointer
- added a loop-invariant code motion optimization
- passed more things by value ('this', array/domain/domain map wrappers)
- made some improvements to the bulk copy optimization

Memory Leak Improvements
------------------------
- reduced memory leaked by strings by sharing them more aggressively
- improved support for destroying more automatic variables as they leave scope
- fixed a memory leak relating to inlining of iterators
- plugged memory leaks related to file types
- freed temporary buffers used for formatted I/O on complex values

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- fixed bugs in extern blocks containing #define's
- fixed a bug when reading string literals that began with whitespace
- fixed a bug in which whitespace was written to binary files for array output
- fixed bugs related to writef() for complex values
- fixed size mismatches in IO code for 32-bit linux systems
- fixed a bug in the compareExchange implementation for atomic reals
- fixed bugs in Regexp convenience functions and the naming of string.split()
- fixed some bugs in BigInt functions in the GMP module
- fixed a bug in which AdvancedIterators would serialize if #chunks < #tasks
- fixed a bug related to 'ref' intents and the fast segment for GASNet
- fixed a bug related to using --make on the chpl command line
- bug fix for lost updates to fields of 'this' in certain scenarios
- fixed dead code elimination for blocks that are their own sole in-arc

Third-Party Software Changes
----------------------------
- updated to GASNet version 1.20.2 which improves the Cray native conduits
- updated to Qthreads version 1.10 and turned on support for stack overflow
  (see reference to QT_GUARD_PAGES in doc/README.tasks for more information)
- updated to MassiveThreads version 0.95 with support for task-private data
- updated the LLVM back-end to use version 3.3 and packaged it with the release
- packaged CLANG 3.3 with the release rather than requiring a separate download

Runtime Library Changes
-----------------------
- made several interface changes to support locale models/hierarchical locales

Generated Code Cleanups
-----------------------
- declared non-exported symbols 'static' in generated code
- added support for targeting 'op=' operators in the back-end
- removed chains of unnecessary autocopy/autodestroy calls

Compiler Performance
--------------------
- disabled compiler internal verification by default to improve compile time

LLVM Back-end Changes
---------------------
- added wide pointer support for the LLVM back-end via CHPL_WIDE_POINTERS
  (see doc/technotes/README.llvm and doc/README.chplenv)
- added support for LLVM-based communication optimizations via --llvm-wide-opt
  (see doc/technotes/README.llvm)

Testing System
--------------
- changed performance graphing to use dygraphs
- added support for filing .bad files against futures to track behavior changes
- added support for setting environment variables on a per-test/-directory basis
- added the ability to use an external 'time -p' command for performance tests
- added directory-level PERFCOMPOPTS/PERFEXECOPTS files
- added a timeout feature for performance testing (separate from correctness)
- fixed a bug in which spec tests weren't being generated correctly
- added the capability for the test directory to clean itself up more thoroughly

Makefile Changes
----------------
- changed the naming scheme for intermediate object file directories
- added a 'cleanall' target between 'clean' and 'clobber'
  (see README.building)
- made the 'clean' and 'cleanall' targets preserve a working installation
- accelerated the Makefile framework, particularly for no-op re-builds
- made the Makefiles smarter about rebuilding the compiler when necessary

Internal/Developer-oriented
---------------------------
- added a new 'createTaskFunctions' pass that creates functions defining tasks
- added a new 'resolveIntents' pass which resolves 'blank' and 'const' intents
- added a per-pass consistency/verification framework
- added a --verify compiler flag for running optional verification steps
- added an --ignore-errors-for-pass compiler flag for developers
- added a compiler analysis to compute dominators
- fixed a bug in isDefAndOrUse() for op= primitives
- rearchitected the QIO framework to support multiple file systems
- refactored the memory interface to better separate checking/tracking concerns
- switched to using 'ref' intents rather than 'inout' for I/O routines
- added "atomic type" flag for identifying atomic types to the compiler
- added "donor" function concept, making the callee responsible for deletion
- added "runtime type init fn" pragma to clarify overloaded "has runtime type"
- fixed a bug in which 'begin's in internal modules broke compilation
- restructured iterators so that not all yields would share the same variable
- improved cases where filename/line numbers aren't set correctly
- continued transition toward STL collections within the compiler
- added new capabilities to the bit vector class
- removed several instances of strcpy()s in the compiler
- added a functional interface for invoking remote functions by ID
- restructured #includes to avoid reliance on compiler-specific search policies
- added new features for debugging the compiler

Changes In Support of Hierarchical Locales
------------------------------------------
- made ranges and default rectangular domains/arrays use sublocales, if present
- made on-clauses get invoked through locale model module code in Chapel
- moved optimization of local on-clauses from the compiler to locale model code
- made task invocation flow through locale model module code written in Chapel
- mapped the tasking interface's support for hierarchical locales to Qthreads
- made hierarchical locale model compilations use wide pointers and on-clauses



version 1.7.0
=============

Tenth public release of Chapel, April 18, 2013

Highlights (see entries in subsequent categories for details)
-------------------------------------------------------------
- added support for regular expression operations on strings and channels
- added support for readf()/writef() routines for formatted I/O
- added support for injecting C declarations into Chapel via "extern blocks"
- added support for associative domain/array literals
- added a capability for main() to take arguments and return a status value
- added support for iteration over homogeneous tuples
- performance improvements related to stack-allocated and constant variables
- improved bulk transfer optimization (use -s useBulkTransferStride to enable)
- significant improvements to the Chapel version of LULESH
- enabled GMP using the system-installed version on Cray platforms by default
- switched to using processor atomics for reference counting on Cray systems
- numerous bug fixes and improvements to semantic checks and error messages

Environment Changes
-------------------
- renamed the platform 'cray-cascade' to 'cray-xc'
- renamed the platforms 'xmt'/'xmt-sim' to 'cray-xmt'/'cray-xmt-sim'
- enabled GMP support by default if it is already built in the third-party dir

Syntactic/Naming Changes
------------------------
- added support for associative domain/array literals
  (e.g., {"red", "blue"} is an associative domain of strings)
  (e.g., ["red" => 0, "blue => 1] is an assoc. array from strings to ints)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- added support for injecting C declarations into Chapel via "extern blocks"
  (see "Support for Extern Blocks" section of doc/technotes/README.extern)
- added support for iteration over homogeneous tuples
  (see "Tuples" section of the Chapel spec)
- added a capability for main() to take arguments and return a status value
  (see doc/technotes/README.main)
- added a 'requestCapacity()' capability to pre-size associative domains
  (see "Domains:Predefined Functions" section of the Chapel spec)
- added a warning() routine to generate warnings, similar to halt(), assert()
  (see "Standard Modules" section of the Chapel spec)

Newly Implemented Features
--------------------------
- added support for the .localSlice() method to take a domain argument
  (e.g., A.localSlice({loRow..hiRow, loCol..hiCol}))

Deprecated Features
-------------------
- removed the tuple() function
  (i.e., instead of tuple(x) or tuple(x,y,z), use (x,) or (x,y,z) instead)
- deprecated warning for old-style domain literals by default
  (i.e., [1..m, 1..n] is an array of ranges; re-enable w/ --warn-domain-literal)

Standard Modules
----------------
- added support for regular expression operations on strings and channels
  (see doc/technotes/README.regexp for details)
- added support for readf()/writef() routines for formatted I/O
  (see doc/technotes/README.formattedIO for details)
- refactored communication diagnostics into its own module (CommDiagnostics)
  (see doc/technotes/README.comm-diagnostics for documentation)
- added isDomainType(type t)/isArrayType(type t)/isRecordType(type t) queries

Documentation
-------------
- added README.formattedIO to describe support for writef()/readf() routines
  (see doc/technotes/README.formattedIO)
- added README.regexp to describe support for regular expression operations
  (see doc/technotes/README.regexp)
- added README.main to describe support for arguments/return values for main()
  (see doc/technotes/README.main)
- updated README.extern to describe C type aliases and extern blocks
- changed READMEs that used csh-style syntax to use bash-style consistently
- fixed a semantic typo in README.atomics
- minor updates and fixes to various chapters in the language specification
- minor updates to several READMEs

Example Codes
-------------
- improvements to the Chapel implementation of LULESH
  (see examples/benchmarks/lulesh/ for more details)
  - configuration improvements (see README for details):
    - added ability to compute the cube input set rather than reading from disk
    - support for 3D vs. 1D representations via 'use3DRepresentation'
    - support sparse vs. dense material representations via --sparseMaterials
    - added a --maxcycles config that can be used to stop the computation early
    - converted 'dtfixed' into a config const
    - added a per-iteration timing when doTiming and showProgress are enabled
  - code improvements:
    - refactored setup code into a helper module: luleshInit.chpl
    - changed the representation of nodesets to use a sparse subdomain
    - switched to using 'ref' intents rather than 'in' or 'inout'
    - converted vars to consts where appropriate
    - general code cleanup
  - performance improvements:
    - improved performance by avoiding an unnecessary reindexing operation
    - made some unnecessarily serial loops parallel
    - replaced error-oriented reductions with forall loops + halt
    - param-unrolled the loop in localizeNeighborNodes() for performance reasons
    - removed a redundant zero-initialization of an array
  - documentation improvements:
    - updated README to document new configuration options
    - added the LULESH callgraph to the README
- improved the Chapel implementation of RA:
  - added the ability to use an LCG random number generator via 'useLCG'
  - added a config const 'verify' that permits verification to be skipped
- fixed a typo in HPL that inflated the calculated GFLOPs by 10x
- improvements to the Chapel implementation of SSCA2:
  - fixed a bug in the torus representation
  - fixed a bug with VALIDATE_BC=true

Platform-specific Notes
-----------------------
- enabled GMP using the system-installed version on cray-x* platforms by default

Portability of code base
------------------------
- fixed an issue related to vsnprintf on OS X Lion
- fixed an assumption that platforms defining INT8_MIN will also define INT*_MIN

Launcher-specific Notes
-----------------------
- improved use of cnselect in launchers that use 'aprun'

Compiler Flags
--------------
- added --no- variants of the following compiler flags: --count-tokens, --llvm,
    --print-code-size, --print-commands, --print-passes, --print-search-dirs,
- added --[no-]warn-tuple-iteration to warn against old-style zipper iteration
  (see man page for details)
- added --[no-]warn-special as a meta-flag for all old-style syntax uses
  (see man page for details)
- improved support for environment variable setting of '--no-'style flags
  (see man page for details)

Error Message Improvements
--------------------------
- improved type mismatch error messages when assigning scalars to arrays
- added an error for extern functions with formal arguments of array type
- made Ctrl-C and the like result in a user, rather than internal, error
- added an error for associative domains with unsupported index types

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- fixed a bug in which sizeof() had leaked into the user namespace in v1.6
- fixed a regression in v1.6 in which type aliases of arrays were broken
- fixed a bug in which 0-element arrays of arrays would free domains prematurely
- fixed a bug related to writing out instances of the RandomStream class
- I/O bug fixes:
  - QIO_HINT_PARALLEL now works for new/zero length files
  - fixed bugs related to bit-oriented reads and writes
  - fixed a bug w.r.t. EOF and preemptive mmap
  - fixed portability bug related to Intel compilers
  - changed channels so that once EOF is hit, it will never read further
  - fixed issues related to ssize_t/int(64) mismatches on 32-bit platforms
  - fixed an issue related to reading empty strings
- fixed some bugs in the implementation of atomic variables
- fixed a bug in which AdvancedIters didn't default 'numTasks' as documented
- improved shape checking for array assignment
- restricted reshape() to be applied for rectangular domains only
- restricted reshape() to only work when the source and destination sizes match
- improved support for Cyclic distributions of idxType uint(64)
- fixed INFINITY and NAN to be of type 'real' rather than 'real(32)'
- fixed a bug with '/*/' within a nested comment
- fixed a bug in which uses of ** would dispatch to a user's pow() overload
- fixed a few bugs in the Buffers.chpl module
- fixed a bug in which memory leak tracking would mislabel some allocations
- fixed a bug with recursive iterator inlining
- fixed a bug in broadcasting large values at program startup

Packaging Changes
-----------------
- added Chapel mode support for emacs version 23.x (see etc/emacs/README)

Performance Improvements
------------------------
- switched to using processor atomics for reference counting on Cray systems
- improved bulk transfer optimization (use -s useBulkTransferStride to enable)
- put fewer variables on the heap in the presence of nested functions
- enabled remote value forwarding for more constants/arguments than before
- made atomic variables use compiler intrinsics for cray-prgenv-intel
- made associative domains of 'imag' indices more efficient

Runtime Library Changes
-----------------------
- split chpl_task_begin() to distinguish between new tasks and migrating ones
- improved the ability to build the Chapel runtime with a C++ compiler

Third-Party Software Changes
----------------------------
- updated to version 1.20 of GASNet
- updated to version 1.9 of Qthreads
- made the LLVM checkout script grab version 3.2 to ensure compatibility
- removed the nanox tasking layer due to lack of support/champion

Testing System
--------------
- extended the 'skipif' capability to support the ability to skip a subdirectory
- improved the robustness of start_test in cross-compiled environments
- added support for .no-local.good files for --no-local-specific test output
- fixed a bug in which the test system looked at incorrect .future files
- removed artificial limiting of stacksize in start_test
- refactored test system environment setup into util/test/testEnv
- made the test system use $cwd rather than 'pwd'
- made 'nightly' compopts/execopts get added to end of the command line
- made 'nightly' support a --no-futures option

Internal/Developer-oriented
---------------------------
- added some code that sets the groundwork for supporting hierarchical locales
- fixed portability of bswap.h (for once and for all?)
- restructured internal modules corresponding to specific runtime layers
- removed uses of alloca()
- removed PRIM_CHPL_ALLOC_PERMIT_ZERO
- deprecated 'make depend' and made use of gcc -MMD/-MP instead
- made 'rm -rf' commands a little safer in Makefile clean/clobber rules
- refactored Makefiles for Cray systems/compilers to avoid repetition
- added a map_view() function for printing out SymbolMaps
- removed some dead and possibly incorrect methods from Map and Vec classes
- refactored _cast functions for booleans
- made the _isPrimitiveType test more concise
- improved the mechanism by which line numbers are assigned to AST nodes
- factored sync/single out of ChapelBase.chpl and into ChapelSyncvar.chpl
- fixed the --gen-extern-prototypes flag
- improved the correspondence of FLAG_* and string forms of AST flags
- improved the correspondence of PRIM_* and string forms of primitives
- refactored chplrt.h
- added initial support for a .localAccess accessor to Block/Default arrays
- renamed compiler-generated types for classes in generated code
- refactored QIO code in the runtime
- converted macros in the compiler to inline functions for sanity/performance
- cleaned up the functions that build array/domain literals
- refactored and commented lowerIterators.cpp
- keep dtObject alive even if unused after function resolution
- added EOF to the list of reserved symbol names
- protected some C identifiers introduced by PGI when compiling with -Kieee



version 1.6.0
=============

Ninth public release of Chapel, October 18, 2012

Highlights (see entries in subsequent categories for details)
-------------------------------------------------------------
- syntax improvements:
  - added support for array literals using square brackets
  - improved domain literals to use curly brackets to suggest set notation
  - changed zippered iteration to use an explicit 'zip' keyword
- improved atomic variables: new methods, support for floating point atomics
- added support for 'ref' intents to support pass-by-reference arguments
- added support for user overloads of 'op=' and '<=>' assignments
- added a prototype of 'chpldoc', a code-based documentation feature
- added support for using LLVM as a back-end compiler target rather than C
- added support for a strided bulk communication optimization
- numerous performance improvements and bug fixes
- significant improvements to the SSCA2, RA, and LULESH example codes
- added a new tasking runtime that uses U Tokyo's MassiveThreads library
- network-based atomic variables for users of the Chapel module on Cray systems
- improved support for the Tilera chip architecture

Environment Changes
-------------------
- made compiler infer CHPL_HOME if unset, or complain if mis-set
- made util/setchplenv.* scripts more cleanly re-runnable
- added new platforms: 'cray-cascade' and 'cray-xk'
  (see doc/platforms/README.cray)
- renamed 'xe-cle' and 'xt-cle' to 'cray-xe' and 'cray-xt', respectively
- renamed Cray PrgEnv compiler settings to be more platform-independent
  (e.g., 'cray-xe-cray'/'cray-xe-gnu' -> 'cray-prgenv-cray'/'cray-prgenv-gnu')
- made CHPL_MEM default to 'tcmalloc' when CHPL_COMM is 'ugni'
- made CHPL_ATOMICS default to 'intrinsics' for CHPL_TARGET_COMPILER=PrgEnv-gnu

Syntactic/Naming Changes
------------------------
- changed domain literals to use curly brackets rather than square brackets
  (e.g., '[1..n, 1..n]' would now be written '{1..n, 1..n}')
- introduced array literals using square brackets
  (e.g., '[3, 5, 7, 11]' describes an array of type '[1..4] int')
- added a 'zip' keyword to express zippered iterations rather than using tuples
  (e.g., 'for (a,b) in (A,B)' is now written 'for (a,b) in zip(A,B)')
- added a bidirectional I/O operator (<~>) for reading from/writing to channels
  (e.g., 'myChannel <~> myStr;' will read/write 'myStr' from/to 'myChannel')
- added support for creating one-tuples using the syntax (x,)
  (e.g., '(1.0,)' describes a tuple of type '1*real')
- re-established the underscore as the means of ignoring a tuple component
  (e.g., '(x,,z)' would now be written '(x,_,z)')

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- added support for 'ref' intents, supporting pass-by-reference arguments
  (e.g., 'proc foo(ref x: int) ...' passes an int arg to 'foo' by reference)
- improved support for atomic variables
  - added support for floating point atomic types
    (e.g., 'var x:atomic real;')
  - added waitFor(val) method
    (e.g., 'x.waitFor(1.0)' would wait for x to hold the value 1.0)
  - added non-fetching variations of operations to avoid excess communication
    (e.g., 'x.add(1)' is like 'x.fetchAdd(1)' but doesn't return a value)
  - added compareExchange() method as a shorthand for compareExchangeStrong()
- added support for user-defined overloads of '<=>' and 'op=' assignments
  (e.g., 'proc <=>(ref x: t, ref y: t) ...' or 'proc +=(ref lhs:t, rhs:t) ...')
- added a '.size' method to arrays, domains, strings, and ranges
- made enum types behave more similarly to param ints (or collections thereof)
- removed certain expression forms as being valid types or standalone stmts

Newly Implemented Features
--------------------------
- added a prototype implementation of a code-based documentation feature
  (see doc/technotes/README.chpldoc)
- added an LLVM-based back-end
  (see doc/technotes/README.llvm)
- added support for degenerate reindexing of non-rectangular arrays

Deprecated Features
-------------------
- removed the '_extern' keyword; use 'extern' instead
- removed the ability to use blanks to drop tuple components -- use '_' instead

Standard Modules
----------------
- made Dimensional Block Cyclic distributions support non-default index types
- added HeapSort() to the standard Sort module
- added the ability to clear() timers without stopping them

Documentation
-------------
- added README.llvm to describe new LLVM back-end option
  (see doc/technotes/README.llvm)
- added README.chpldoc to describe new chpldoc documentation option
  (see doc/technotes/README.chpldoc)
- updated README.atomics to describe new capabilities for atomic variables
- added doc/platforms/README.tilera to describe using Chapel with Tilera chips
- noted that Python and bc are now required features to build Chapel
  (see doc/README.prereqs and doc/platforms/README.cygwin)
- updated README.extern to utilize 'ref' intents rather than 'inout'
- updated language specification to describe new features and syntax changes
- minor updates and fixes to several chapters of the language specification
- updated the Quick Reference to illustrate new syntax, features
- improved command-line help for -d option
- minor updates to several READMEs

Example Codes
-------------
- extensive revisions to SSCA#2 including:
  - extensive performance and memory improvements
  - improved ability to 'make' with different options from the command line
  - changed the neighbor list representation to use 1D arrays of 2-tuples
  - added ability to read/write graphs from/to files in setting up
  - added support for distributed/parallel graph construction
  - fixed a bug in the TEPs computation when starting from a subset of vertices
  - added the ability to skip various kernels via configs
  - optimized and fixed a bug in the generation of filtered edge lists
  - converted uses of sync variables to atomic variables
- improvements to RA:
  - extended ra.chpl to use either on-clauses or direct array accesses
  - made verification updates lossless using synchronization variables
    (see examples/benchmarks/hpcc/ra.chpl)
  - added a lossless version of RA that uses atomic variables
    (see examples/benchmarks/hpcc/ra-atomics.chpl)
- improvements to LULESH
  - replaced 'sync' variables with 'atomic's
  - converted a global array into tuples to minimize communication
  - general code cleanup improvements
- examples/primer/ updates:
  - added a new primer to demonstrate the --docs/chpldoc capability
    (see examples/primers/chpldoc.chpl)
  - updated atomic_vars.chpl primer to reflect recent changes
  - updated arrays.chpl primer to reflect array literal syntax
- updated all examples to reflect syntax changes

Platform-specific Notes
-----------------------
- improved support for the Tilera chip architecture
  (see doc/platforms/README.tilera)
- added support for Cray Gemini- and Aries-based atomics to the ugni comm layer
  (see doc/platforms/README.cray)

Launcher-specific Notes
-----------------------
- improved error handling of failed forked processes
- for aprun-based launchers:
  - made '-cc none' the default mode used
  - added -j option to aprun (for supported versions)
- for the pbs-aprun launcher:
  - fixed a timing bug
  - added a flag --generate-qsub-script

Compiler Flags
--------------
- added --docs and related flags for generating documentation for Chapel code
  (see doc/technotes/README.chpldoc)
- added a --gen-extern-prototypes flag to cross-verify extern prototypes
- added an --llvm flag for targeting LLVM as the back-end compiler
  (see doc/technotes/README.llvm)

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- fixed a bug with type-inferred module-level domains/arrays
- fixed a bug relating to strings on distributed memory 32-bit platforms
- improved support for uint idxTypes in domains and arrays
- improved the ability to use user-defined constructors in derived classes
- fixed a bug in which the block-cyclic distribution didn't support 1D domains
- fixed several bugs in I/O:
  - one related to using stdin on Mac OS X
  - one related to bitwise writing
  - one related to writing to/reading from a Writer/Reader from a remote locale
  - one related to issues on Cygwin
- fixed a bug in which classes were improperly considered unused and removed
- fixed a bug in tcmalloc when computing reallocation growth and shrinkage

Packaging Changes
-----------------
- simplified the etc/emacs/ directory organization to avoid duplication
  (emacs users may need to update their .emacs settings -- see etc/emacs/README)
- added a 'chpldoc' alias to the 'chpl' compiler to just generate documentation
- added setchplenv.fish to the release (intended for 1.5.0, but mis-packaged)
- made the subdirectories used to store object files more unique

Performance Improvements
------------------------
- added a strided bulk communication optimization, disabled by default
  (to enable, compile with -s useBulkTransferStride)
- replaced internal sync var counters with atomics when appropriate
- made initialization of remote array access caches lazy to reduce setup time
- removed a level of pointer indirection in the implementation of array data
- reduced the runtime cost of creating array aliases
- optimized reindexing for the case when the original and new domains match
- optimized the performance of equality/inequality on identical domains
- made most associative domain/array ops to occur on the owning locale
- enabled remote-value forwarding of array descriptors in the presence of syncs

Runtime Library Changes
-----------------------
- added support for MassiveThreads-based tasking (courtesy U Tokyo)
  (see doc/README.tasks)
- added the ability to implement atomic variables using network-based AMOs
- made counters used for communication diagnostics 64 bits to avoid overflow
- turned off the cooperative comm/task non-blocking get interface for GASNet

Third-Party Software Changes
----------------------------
- updated Qthreads to version 1.8
- updated GASNet to version 1.18.2
- added MassiveThreads 0.3beta to the third-party directory
- added a directory to download creoleparser into for use with chpldoc
  (see doc/technotes/README.chpldoc)

Testing System
--------------
- reordered the order in which execution options are passed to tests
  (see #comments at top of start_test for details)
- added a 'lastexecopts' capability to require an execution option to come last
- improved the 'timedexec' script to kill the child's process group

Internal/Developer-oriented
---------------------------
- significantly refactored the code generation pass
- began the process of replacing homegrown ADTs with STL classes
- removed pragma "inline" (use 'inline' keyword instead)
- renamed some primitives: 'get ref' -> 'deref' and 'set ref' -> 'addr of'
- fixed type-punned pointers in comm-gasnet.c
- fixed a case of deleting global type symbols prematurely
- fixed a use-before-def warning for default-initialized vars of extern type
- added consistency checking to basic block analysis
- made use of BLOCK_TYPE tag more consistent
- reduced reliance on primitives and locale internals in standard modules



version 1.5.0
=============

Eighth public release of Chapel, April 19, 2012


Highlights (see entries in subsequent categories for details)
-------------------------------------------------------------
- completely revamped and improved Chapel's I/O features
- added support for atomic bool/int/uint variables to Chapel
- added a version of the LLNL/DARPA LULESH benchmark
- improved GMP support for integer types
- added an initial implementation of a 2D Dimensional distribution
- added Cray XE (TM)-specific tasking/thread/communication layers for
  the Cray XE module (binary only)
- updated GASNet from 1.16.2 to 1.18.0 which includes initial ugni support
- made integers default to 64-bits in most contexts rather than the previous 32-bits
- numerous performance improvements, bug fixes, and documentation improvements

Environment Changes
-------------------
- added support for fish shell users (http://fishshell.com)
  (see $CHPL_HOME/README and $CHPL_HOME/util/setchplenv.fish)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- completely revamped and improved Chapel's I/O features
  (see examples/primers/fileIO.chpl, doc/technotes/README.io, I/O spec chapter)
- added support for atomic bool/int/uint variables to Chapel
  (see doc/technotes/README.atomics, examples/primers/atomics.chpl)
- added support for /* ... */-style comments to be nested
  (e.g., /* in comment! /* in nested comment! */ still in comment! */)
- made integers default to 64-bits in most contexts rather than the previous 32-bits
  - integer literals are now of type int(64)/uint(64) by default
  - the default 'int'/'uint' types now correspond to int(64)/uint(64)
  - the default idxType for ranges, domains, and arrays is now int(64)
  - a number of locale-oriented capabilities now use/return 64-bit ints:
    - numLocales, LocaleSpace, locale.id
    - locale.numCores, locale.callStackSize
    - numThreadsPerLocale, maxThreadsPerLocale
    - locale.*Threads(), locale.*tasks()
- the 'by' and '#' operators now accept int(w)/uint(w) args for w-bit idxTypes
- param arguments now play a role in disambiguation between function overloads
  (see the 'Function Resolution' section in the Chapel language specification)

Newly Implemented Features
--------------------------
- added an initial implementation of a 2D Dimensional distribution
  (see examples/primers/distributions.chpl)
- added support for reading arrays, domains, ranges, and tuples
  (e.g., var A:[1..10] real;  read(A); )
- added support for an iterator's arguments to have intents
- added a new getCommDiagnosticsHere() routine for querying local communications
  (see doc/technotes/README.comm-diagnostics)

Deprecated Features
-------------------
- removed support for tensor iteration from the Chapel compiler
  (e.g., 'forall (i,j) in [foo(), bar()]' no longer supported; use nested loop)
- removed volatile types in favor of atomic types
- removed the ability to use 'def' to define functions; use 'proc' or 'iter' now

Standard Modules
----------------
- improved GMP support for integer types
  (see doc/technotes/README.gmp)
- changed sgn() in Math.chpl to return an int(8)/uint(8)

Documentation
-------------
- major updates to the I/O chapter of the spec to reflect new capabilities
- added a new technical note describing support for atomic types
  (doc/technotes/README.atomics)
- revamped technical notes for I/O and GMP to reflect revamped features
  (doc/technotes/README.io, doc/technotes/README.gmp)
- minor updates to several other chapters of the language specification
- minor fixes and updates to several READMEs

Example Codes
-------------
- added a version of the LLNL/DARPA LULESH benchmark
  (see examples/benchmarks/lulesh/README)
- numerous updates to the Chapel version of SSCA#2
  - store visited children in forward pass to avoid recomputing in backwards
  - use hand-coded task-private variables to store private variables
  - replaced arrays of sync vars with arrays of atomics in most instances
  - fixed a number of on-clauses that referred to a poor choice of locale
  (see examples/benchmarks/ssca2/README)
- updated hpcc/hpl.chpl to use Dimensional distributions and replicated storage
- moved ssca2 and hpcc examples into a new benchmarks/ subdirectory
- fixed some bugs in hpcc/hpl.chpl
- in ra.chpl, copied manual optimizations from timed loop into verification loop
- updated the arrays.chpl primer to deprecate tensors and improve explanations

Platform-specific Notes
-----------------------
- added Cray XE-specific tasking/thread/communication layers to the
  Cray XE module (available using 'module load chapel', see
  doc/platforms/README.xe-cle-system-specific-layers)
- fixed GASNet executions on Mac OS X Lion
- generated C identifier lengths are now capped to avoid PGI warnings

Compiler Flags
--------------
- added --max-c-ident-len to cap the length of C identifiers in generated code

Error Message Improvements
--------------------------
- in most contexts, the compiler now prints out a fully-specified type
  (e.g., where before it printed 'int', it now prints 'int(64)')
- line number reporting has been improved in certain cases

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- added a compile-time error for incorrect domain slicing cases
- fixed an issue in which users couldn't use 'this' as an identifier
- fixed the ability to delete an object located on a remote locale
- fixed a non-deterministic seg fault for parent classes with types/params only
- fixed a race condition in the implementation of halt()
- fixed an issue in which a .exe filename could not be specified using -o
- the --explain-calls flag now properly displays 'param' intents as such
- protected a user's ability to name variables j0, j1, jn, y0, yn

Performance Improvements
------------------------
- changed array/domain reference counting to use atomics rather than syncs
- made Chapel allocation/free routines inline to reduce overheads
- optimized local on-clauses to reduce overheads by avoiding chpl_comm_fork()
- optimized bulk transfers for Block-distributed arrays that share a domain
- optimized the creation of simple Block- and Cyclic- array aliases

Runtime Library Changes
-----------------------
- no longer count communication threads against numThreadsPerLocale for pthreads
- removed assumption that globals in generated code will have identical addrs
- added support for keeping builds for multiple GASNet segments in one tree
- moved most Chapel module initialization from main() to first task
- simplified the tcmalloc memory layer to support use with GASNet
- added dlmalloc support for communication layers other than GASNet-fast
- broke GASNet AM_signal handler request/reply to use short messages
- fixed a bug in which non-blocking gets could seg-fault
- simplified the initialization and shutdown of the communication layer
- fixed a potential race in broadcasts at program startup

Third-Party Software Changes
----------------------------
- updated GASNet from 1.16.2 to 1.18.0 which includes initial ugni support
- made third-party packages build automatically when needed by the runtime
- updated Qthreads from version 1.7.0 to 1.7.1
- added support for a UTF-8 decoder from Bjoern Hoehrmann for use in the new I/O
- added support for Nanos++ tasking to yield when firing off an active message
- added support for the 'serial' statement to Nanos++ tasking layer
- turned off tcmalloc's tendency to print messages for large allocations

Testing System
--------------
- added the ability for a skipif file to check for substrings in an env. var.
- fixed several bugs in the testing system:
  - when using named .good files in compopts files
  - when using multiple compopts files and no execopts files
  - in the generation of performance data filenames
- condensed how compilation/execution options are identified in testing logs
- added a --logtmp flag to start_test to support single-node parallelism

Internal/Developer-oriented
---------------------------
- fixed an issue in which internal modules resulted in too many circularities
- changed module initialization code so that modules initialize those they use
- added a pragma to control implicit inclusion of ChapelStandard.chpl
- added support for automatically mapping external C types to Chapel types
- added the ability to dump the compiler IR using a text-file format
- added filenames back into the BaseAST class to preserve info during inlining
- added chpl__unsignedType(), chpl__maxIntTypeSameSign() utility functions
- added a typeToString() overload to catch common error cases
- removed debugWrite* capability
- fixed a bug in the compiler's basic block analysis framework
- made existing tests much less sensitive to ordering of output
- added a --[no-]preserve-inlined-line-numbers flag to control inlining behavior
- added a "no instantiation limit" pragma/flag to the compiler
- added an isParameter() method to the ArgSymbol class
- removed strcmps against types to normalize scalar type queries (e.g., int(?w))
- made several refactorings to functionResolution.cpp
- refactored common flag queries into helper query functions
- refactored creation of wrapper functions
- fixed get_int()/get_uint() to avoid reliance on underspecified C int sizes
- made external classes less like Chapel classes within the Chapel compiler
- cleaned up the memory tracking enums associated with the communication layer
- removed chpl_error_noexit primitive
- added code to trace through the disambiguation logic of function resolution
- refactored Makefiles, replacing CHAPEL_ROOT with CHPL_MAKE_HOME
- made third-party/*/Makefile rules more consistent



version 1.4.0
=============

Seventh public release of Chapel, October 20, 2011

Highlights (see below for details)
----------------------------------
- added 'extern', 'export', and 'inline' keywords
- new optimizations for whole-array assignments and remote array accesses
- improved support for Qthreads and Nanos++ tasking, including multi-locale
- added dynamic range iterators via an 'AdvancedIters' module
- continued improving range semantics and implementation
- added support for # operator on dense rectangular domains and arrays
- made associative domain index modifications parallel-safe
- added --static/--dynamic linking flags
- improved communication diagnostics capabilities
- new example codes: SSCA2; primers on sync vars, leader-follower iterators
- updated documentation
- many bug fixes

Syntactic/Naming Changes
------------------------
- renamed '_extern' keyword to 'extern'
- renamed 'maxThreadsPerLocale' to 'numThreadsPerLocale'
  (see doc/README.tasks and doc/README.executing for new definitions)
- made it illegal for a string literal to contain an unescaped newline
- renamed dsiIndexLocale to dsiIndexToLocale in domain map standard interface
  (see doc/technotes/README.dsi)
- reserved 'ref' as a keyword for future use
- renamed 'iterator' enumeration to 'iterKind' for leader-follower iterators
- renamed 'follower' argument to follower iterators 'followThis'

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- added 'export' keyword to support calling Chapel functions from C
- added 'inline' keyword to force a function to be inlined
- added support to # operator for dense rectangular domains and arrays
- removed support for tensor iteration from the language definition
- removed the realm concept from the language
- ambiguous ranges are now == if their representations match
- disabled promotion of the '..' operator

Newly Implemented Features
--------------------------
- improved qthreads and nanox task layers to support multilocale execution
  (see doc/README.tasks)
- adding and removing indices from associative domains is now parallel-safe
- updated the communication diagnostics interface
  (see doc/technotes/README.comm-diagnostics)

Standard Modules
----------------
- added an 'AdvancedIters' module with various dynamic scheduling algorithms
- added floor/ceiling/mod functions for integers to 'Math' module
- added new sorting routines to standard 'Sort' module
- added a 'UtilMath' module with other integer ceiling/floor functions
- added a param version of abs() for integers to 'Math' module

Documentation
-------------
- spec updates:
  - added a new chapter on 'Interoperability'
  - refreshed 'Data Parallelism', 'Task Parallelism', and 'Conversions' chapters
  - updated 'Ranges' chapter significantly
  - minor updates to 'Domains', 'Arrays', and 'Classes' chapters
  - split 'Memory Consistency Model' into its own chapter (still poorly defined)
  - documented new Advanced Iterators and Sort routines in 'Standard Modules'
- added documentation about the compiler's module search path to man page/technotes
  (see 'man chpl' and doc/technotes/README.module_search)
- added note about parallel safety to doc/technotes/README.extern
- generally refreshed README* and other text-based documentation files

Example Codes
-------------
- added a(n in-progress) implementation of SSCA2 to the release examples
  (see examples/ssca2)
- added a primer for sync and single variables
  (see examples/primers/syncsingle.chpl)
- added a primer on leader-follower iterators (user-defined forall scheduling)
  (see examples/primers/leaderfollower.chpl)
- added an example of creating a private locale view to distributions primer
  (see examples/primers/distributions.chpl)
- improved the ranges primer
  (see examples/primers/ranges.chpl)
- added a description of subdomains to the domains primer
  (see examples/primers/domains.chpl)
- minor wordsmithing for the procedures primer
  (see examples/primers/procedures.chpl)

Platform-specific Notes
-----------------------
- improved Makefile settings for IBM/AIX platforms and compilers

Feature Improvements
--------------------
- several improvements to the implementation of ranges
  (see 'Ranges' chapter of spec for details)
- made casts-to-string produce the same string as write() does

Compiler Flags
--------------
- added --static/--dynamic flags to specify how the binary should be linked

Interoperability Changes
------------------------
- made 'extern' a keyword; '_extern' is deprecated

Error Message Improvements
--------------------------
- added a compiler warning for param functions with declared return types
- improved a few other difficult compiler error messages

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- fixed the result widths of complex math operations
- improved parsing of escaped characters in string literals
- deleting a record now generates an error
- added a variety of bug fixes for range operations
- fixed a bug with passing strings to external functions for --no-local cases
- fixed a bug for var functions that returned nothing
- fixed a bug related to terminating a CHPL_COMM=gasnet program early
- fixed a bug in which ref temps were not inserted for virtual method calls
- fixed a bug related to removing a label from the AST but not its goto
- fixed bug with applying serial statement to optimized coforall+on loops
- fixed bugs regarding single variables not being treated like sync
- fixed a bug related to incorrect complaints about ambiguous module paths
- fixed a bug related to followers accepting empty ranges
- fixed a few reference-after-deallocation bugs in the compiler
- fixed a bug related to flattening var functions multiple times

Packaging Changes
-----------------
- removed the second-level 'chapel/' directory that releases used to have
- updated emacs/vim modes to bring syntax up-to-date
- util/setchplenv.* scripts now permit $CHPL_HOME to be other than 'chapel'
- added the util/ directory to PATH in the setchplenv.* scripts
- improved make 'clean' and 'clobber' rules for third-party directories

Third-Party Software Changes
----------------------------
- removed requirement to specify LD_LIBRARY_PATHs for qthreads and nanox tasks
- updated to GASNet version 1.16.2 (see third-party/gasnet/README)
- updated Qthreads to version 1.7 (see third-party/qthreads/README)
- added tcmalloc version 1.7 (see third-party/tcmalloc/README)
- updated dlmalloc to version 2.8.5

Runtime Library Changes
-----------------------
- split the 'none' task layer into 'none' and 'minimal'
  (see doc/README.tasks)
- added a tcmalloc-based memory layer to the runtime
- improved program initialization order and sequence
- made nanox/qthreads task layers use a #threads==#cores by default
- increased the default callstack size for qthreads tasks
- removed pthread calls from the gasnet communication layer
- began process of unifying runtime file and directory names
- standardized our threading layer interface
- made tasking layers more robust to early calls during program setup
- moved Chapel code in the runtime/ directory into internal modules

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- added a block transfer optimization for whole-array assignment
- reduced the number of communications for remote Block or Cyclic array access
- added an optimization to remove dead code blocks

Testing System
--------------
- valgrind testing with CHPL_COMM!=none now valgrinds the child processes

Internal
--------
- the threading layer of the runtime now has its own header file
- added a --library flag for creation of a standalone Chapel library
- made normalize_returns() able to be called multiple times
- fixed a bug related to internal module dependencies of depth > 2
- added capabilities to help debug deleted AST nodes
  (see --break-on-delete-id and --log-deleted-ids-to)
- removed --runtime flag from compiler
- added a compilerAssert() capability



version 1.3.0
=============

Sixth public release of Chapel, April 21, 2011

Highlights (see below for details)
----------------------------------
- split 'def' keyword into 'proc' (for procedures) and 'iter' (for iterators)
  (see '$CHPL_HOME/util/convert-defs --help' for help updating existing codes)
- added 'Replicated' distribution and improved 'Block-Cyclic' distribution
  (see "Standard Distributions" in spec and examples/primers/distributions.chpl)
- added single-locale task layers for Nanos++ (BSC) and Qthreads (Sandia)
  (see doc/README.tasks)
- major reworking of range semantics and implementation
  (see "Semantic Changes / Changes to Chapel Language" below for detail)
- initial support for volatile types: bool, int, uint, real, imag
  (see doc/technotes/README.volatile)
- support for extern consts and classes (pointers-to-structs)
  (see doc/technotes/README.extern)
- improved support for data parallelism on the Cray XMT
- extensive improvements to the sample codes in the examples/ directory
  (see 'Example Codes' below and examples/README)
- added most code examples from the spec to a new examples/ directory
  (see examples/spec/*)
- added a technical note describing the user-defined domain map interface
  (see doc/technotes/README.dsi)
- extensive updates to the language specification
  (see 'Documentation' below)
- several improvements to the runtime tasking and communications layers
  (see "Runtime Library Changes" below)

Syntactic/Naming Changes
------------------------
- split 'def' keyword into 'proc' (for procedures) and 'iter' (for iterators)
  (see '$CHPL_HOME/util/convert-defs --help' for help updating existing codes)
- renamed 'arithmetic' domains/arrays to 'rectangular' domains/arrays
- added support for floating point literals of the form '1.e...'
  (previously, only '1e...' and '1.0e...' were supported)

Semantic Changes / Changes to Chapel Language
---------------------------------------------
- several improvements to ranges (see 'Ranges' chapter in the language spec):
  - added a concept of alignment to indicate an alignment modulo |stride|
  - added an 'align' operator and query to ranges to specify/query alignment
    (e.g., '1..10 by 3 align 2' => '2, 5, 8' since 2 == 5 == 8, mod 3)
  - defined == on ranges in terms of the integer sequences they generate
  - defined ident(r1, r2) to say whether r1 and r2 are semantically identical
  - defined '# k' for k < 0 as meaning "count from the end of the range"
  - added 'first', 'last', 'alignedLow, 'alignedHigh' methods to ranges
    (e.g., '1..10 by -2' => first==10, last==2, alignedLow=2, alignedHigh=10)
  - added support for a variety of new range query functions
    (e.g., alignLow(), hasFirst(), hasLowBound(), isAligned(), boundsCheck()...)
- added support for volatile bool, int, uint, real, and imag types
  (see doc/technotes/README.volatile)
- added an end-of-file check to the file type
  (e.g., 'while !infile.eof ...')
- removed support for treating 'return' in iterator as a 'yield'
  (e.g., change 'iter foo() { return x; }' to 'iter foo() { yield x; return; }')
- added support to referencing modules without first 'use'-ing them
  (e.g., module M1 { var x...} module M2 { ...M1.x... } is now legal)
- added a callStackSize query to the locale type
  (e.g., 'here.callStackSize' returns the call stack size on the current locale)
- removed the previously-required semicolon from the end of enum declarations
  (e.g., 'enum colors {red, blue, green}' is now legal; previously needed ';')
- added support for a single trailing comma at the end of enum lists
  (e.g., 'enum colors {red, blue, green, }' is now legal)
- removed support for +/- operators on rectangular domains; use translate()

Newly Implemented Features
--------------------------
- implemented ==/!= for rectangular, associative, and sparse domains

Standard Distributions
----------------------
- added a first-draft 'Replicated' distribution for storing data redundantly
  (see examples/primers/distributions.chpl)
- improved 'Block-Cyclic', sufficient to correctly implement HPCC PTRANS and HPL
  (see examples/hpcc/ptrans.chpl, hpl.chpl; examples/primers/distributions.chpl)
- removed redundancy in specification of leader iterators between domains/arrays

Standard Modules
----------------
- added INFINITY and NAN constants to the Math.chpl module
  (see 'Math' under "Standard Modules" in language spec)
- added isinf() and isfinite() test routines to the Math.chpl module
  (see 'Math' under "Standard Modules" in language spec)
- added a half dozen more routines to the GMP.chpl module

Documentation
-------------
- added a new technical note describing the user-defined domain map interface
  (see doc/technotes/README.dsi)
- made many improvements to the language specification
  - major revisions to the Domains chapter to improve clarity/accuracy
  - improved and unified structure of Records and Classes chapters
  - major revisions to the Domain Maps chapter to improve clarity/accuracy
  - named example codes to provide cross-reference to examples/spec/ programs
  - clarified the legality of label/break/continue in parallel loops
  - updated the descriptions of read[ln]/write[ln]
  - fixed some problems in file fields ordering
  - reformatted the specification as a 'book' in LaTeX
  - various minor fixes of typos/mistakes
  - improved definitions/descriptions of several terms
  - added documentation of locale.name
  - and many other changes...
- updated quick reference document to include def->proc/iter change, align op.
- rewrote/reorganized README.extern to clarify explanations and bring up-to-date
- added a README describing the 'local' statement (doc/technotes/README.local)
- added a README describing volatile types (doc/technotes/README.volatile)
- updated README.comm-diagnostics to mention fast fork statistics
- updated various README files

Example Codes
-------------
- placed most code examples from the language spec. to a new spec/ subdirectory
- improved the primers/ directory:
  - added a new primer for using procedures (primers/procedures.chpl)
  - added a basic primer for domains (primers/domains.chpl)
  - renamed the locales primer to locales.chpl and improved it
  - improved the distributions primer and added Block-Cyclic and Replicated uses
  - updated the range primer (primers/range.chpl)
  - improved the iterators primer (primers/iterators.chpl)
  - improved the reductions primer (primers/reductions.chpl)
  - added getCurrentTime() to the timers primer (primers/timers.chpl)
  - added descriptions of type aliases and config types to variables primer
    (see primers/variables.chpl)
- improved hpcc/ benchmarks directory
  - added first version of HPCC PTRANS to hpcc/ directory (ptrans.chpl)
  - added cleaner version of HPCC HPL to hpcc/ directory (hpl.chpl)
  - added a variants/ subdirectory to examples/hpcc to exhibit other approaches
    - added stream-promoted.chpl to show use of promotion in global STREAM Triad
    - added ra-cleanloop.chpl to show cleaner (but currently slower) RA kernel
- improved top-level examples:
  - renamed existing "hello, world" examples to suggest a logical reading order
    (see examples/hello*.chpl)
  - added data parallel and distributed data parallel hello, world examples
    (see examples/hello3-datapar.chpl, examples/hello4-datapar-dist.chpl)
  - added comments to the various hello, world examples (examples/hello*.chpl)
- updated all examples to current language semantics
  - use 'proc'/'iter' rather than 'def'
  - update to reflect new range semantics

Platform-specific Notes
-----------------------
- for Cray XT/Cray XE, improved the aprun and pbs-aprun launchers
  (see "Launcher-specific notes" below)
- for Cray XMT, added support for parallel loops over ranges
- for Cray XMT, added generation of noalias pragmas to forall loop variables
- added support for BSC's MareNostrum including docs and a special launcher
  (see doc/platforms/README.marenostrum)

Launcher-specific notes
-----------------------
- added launcher-specific options to --help capability
- improved propagation of exit status codes through launchers
- changed launchers to support exec-based (rather than system-based) commands
- made launchers use 'unlink' rather than system('rm') to remove temporary files
- added a launcher for BSC's MareNostrum
- reduced number of temporary files used to capture output of 'system' commands
- aprun launcher improvements:
  - added a --cc flag to specify CPU assignment w/in a node
  - added a -q flag to run the launcher in quiet mode
  - added debug capability via CHPL_LAUNCHER_DEBUG to keep tmp files around
- pbs-aprun launcher improvements:
  - added a --cc flag to specify CPU assignment w/in a node
  - added debug capability via CHPL_LAUNCHER_DEBUG to keep tmp files around
  - cleaned up output to get rid of some extraneous printing
  - made launcher support our testing system
  - added --walltime and --queue flags to launcher as alternatives to env. vars.

Compiler Changes
----------------
- made the compiler print 'true'/'false' out in type signatures rather than 1/0
- added config param 'noRefCount' to disable reference counting (may leak memory)

Compiler Flags
--------------
- added a --print-callstack-on-error flag to show what led to a bad call
- added a --no-codegen flag to skip the code-generation and linking phase

Interoperability Changes
------------------------
- added support for extern C "classes" (pointer-to-struct types)
- added support for extern consts (see README.extern)

Error Message Improvements
--------------------------
- added a warning orphaned 'use' statements outside of declared modules
  (e.g., 'use M1; module M2 { }' should typically be 'module M2 { use M1; }')
- made parse-time error message use the standard 'filename:lineno:' format
- generated an error message for referring to a class name in a primary method
  (e.g., 'class C { def C.m() {...} }' now generates an error message)

Bug Fixes / New Semantic Checks (for old semantics)
---------------------------------------------------
- prevented config types from being set using execution time command-line flags
- fixed the compiler's ability to handle unions with no fields
- fixed the minloc/maxloc reductions to handle empty ranges/subranges of values
- fixed a bug in which the 'path' argument to file constructors was ignored
- made empty unions work correctly
- fixed an overflow issue in chunking up ranges for distribution/parallelization
- fixed a bug involving support for empty extern record types
- fixed a bug involving extern functions that return records
- fixed a bug related to copying external records
- fixed implementation of +/- operators on associative/sparse domains
- fixed a bug relating to filename-based module names containing multiple '.'s
- added error messages for expand/exterior/interior on irregular domains
- added an error for constructors/destructors with a specified return type
- fixed a pair of bugs relating to the 'fast on' optimization
- added a check against constructing classes without specifying generic fields

Packaging Changes
-----------------
- added syntax highlighter code for GNU source-highlight
  (see etc/source-highlight/README)
- improved emacs coloring support for version 22.x
  (see etc/emacs/README)
- updates to vim syntax coloring to reflect new 'proc'/'iter' keywords
  (see etc/vim/README)
- added a script for converting existing codes' use of 'def' to 'iter'/'proc'
  (see $CHPL_HOME/util/convert-defs)
- removed multirealm examples & documentation for the time being

Third-Party Software Changes
----------------------------
- updated GASNet to version 1.16.1
- added copies of BSC's Nanos++ and Sandia's Qthreads
  (see third-party/README)
- made a post-install step for GASNet that changes absolute paths to relative
- removed PVM for the time being due to instability

Runtime Library Changes
-----------------------
- added single-locale task layers for Nanos++ (BSC) and Qthreads (Sandia)
- several improvements to the runtime tasking layer:
  - added a new interface for having the tasking layer call chpl_main()
  - changed tasking init function to take maxThreadsPerLocale/callStackSize args
  - added a task yield capability to the tasking layer interface
  - improved runtime code to avoid using sync vars before initializing tasking
  - removed macro-ized form of tasking interface; using standard names instead
  - unified naming of all task layer routines to chpl_sync_*() and chpl_task_*()
  - removed requirement to implement the single variable interface
  - moved termination of threads from fifo tasking layer to pthreads threading
  - simplified implementation of sync variables in the pthread threading layer
  - moved responsibility for thread counts from fifo tasking to pthread threading
- several improvements to the runtime communication layer:
  - restructured the communication interface to support length/type arguments
  - added an interface for non-blocking get operations to the comm. interface
  - unified all remote get operations to use CHPL_COMM_GET() macro
  - refactored/renamed the communication interface headers

Testing System
--------------
- vastly improved the performance testing and graphing capabilities
  (see comments at the front of start_test for documentation)
- improved the testing system's support for cleaning up after itself
- added ability to avoid using any indirection of stdin
- added an option to provide a system-wide prediff option
- added ability to use a launcher's native timeout feature rather than Python's
- improved how the testing system kills a test that times out
- fixed a bug in which .preexec files only worked if '.' was in your path

Internal
--------
- added a --break-on-id compiler flag for developers to locate AST node creates
- improved organization of util/ directory contents
- changed the way version numbers are computed/displayed for SVN-based users
- added support for task-/thread-specific module code
- added a capability to print the AST counts allocated between each pass
- added a new gdb alias to compiler --gdb: 'loc' prints an AST node's location
- greatly improved implementation of pragmas/flags to avoid error cases
- removed several instances of unnecessary string comparisons in the compiler
- removed error-prone use of CHPL_TASKS and CHPL_THREADS as preprocessor symbols
- added bestPractices documentation for developers (not included in release)
- changed internal munging of '=' and '==' to 'ASSIGN' and 'EQUALS' respectively
- added a --print-id-on-error developer flag to print AST ID on errors
- improved the strictness of C++ prototype checking
- added external contributor agreements to the SVN tree (not in release)
- added a notion of internal types used by the compiler but not the user



version 1.2.0
=============

Fifth public release of Chapel, October 21, 2010

High-Level Themes
-----------------
- single- and multi-locale task parallelism
- improved single- and multi-locale data parallelism
  - single- and multi-locale parallelism for arithmetic domains/arrays
  - single-locale parallelism for sparse and associative domains/arrays
  - serial reference version of opaque domains/arrays
- improved stability and portability

Highlights
----------
- support for parallel iteration over sparse and associative domains and arrays
- added support for the Cray XE6 (TM) and Cray CX1000 (TM)
- improved parallelization of loops for the Cray XMT (TM)
- support for config types and improved support for config params
- improved support for external C types, vars, and functions
- initial support for first-class and anonymous functions
- prototype implementation of a GMP multiprecision math module

Environment Changes
-------------------
- renamed the 'cx1-linux' platform variable to 'cx-linux'

Syntactic/Naming Changes
------------------------
- renamed the range's index type from 'eltType' to 'idxType'
- made string length a parenthesis-less method rather than a standalone function
  (i.e., 'length(mystring)' should now be written 'mystring.length')

Semantic Changes/Changes to Chapel Language
-------------------------------------------
- added initial support for first-class and anonymous functions
  (see doc/technotes/README.firstClassFns)
- added support for config types
- enabled config params to support very general command-line expressions
- added support for param 'this' arguments to methods
- added support for assigning tuples of ranges to arithmetic domains
- added support for .stridable and .stride queries on domains
- changed the range's stride type from an int to a signed int of the index type
  (e.g., range(uint(64)) and range(int(64)) now store an int(64) stride value)
- made the by operator for a range accept the range's idxType or its stride type
- added min()/max() functions for tuples that fill the tuple with min/max vals
  (see 'Tuples' chapter of language spec)
- added isTupleType(type t) param function that tells whether t is a tuple
- made a dataParTasksPerLocale value of '0' evaluate to here.numCores
- converted dataParMinGranularity from a uint to an int and made its default '1'
- made numBits/numBytes for default-width bools generate a compiler error

Newly Implemented Features
--------------------------
- added support for parallel iteration over sparse domains/arrays
- added support for parallel iteration over associative domains/arrays
- added support for sparse strided domains
- added support for printing of default layout arrays with negative strides
- fixed iteration over/printing of default layout arrays with negative strides
- implemented initialization of strided arrays by tuples
- added the ability to slice a domain with another domain
- added support for localSlice method on arrays with default layout
- added support for indexToLocale method on the default layout

Standard Domain Maps
--------------------
- made default sparse and associative domains/array layouts support parallelism

Standard Modules
----------------
- added a prototype GMP (GNU Multiple Precision Arithmetic) module
  (see doc/technotes/README.gmp)

Platform-specific notes
-----------------------
- added support for the Cray XE6 (TM) via a new platform variable xe-cle
  (see doc/platforms/README.xe-cle)
- added support for the Cray CX1000 (TM) as part of the cx-linux platform
  (see doc/platforms/README.cx-linux)
- added support for using LSF as a launcher for cx-linux via lsf-gasnetrun_ibv
- for xt-cle/xe-cle:
  - made MPI the default conduit due to portals conduit issues in GASNet 1.14.x
  - added support for specifying dynamic vs. static linking
    (see doc/platforms/README.xt-cle or .../README.xe-cle)
  - made the default launcher depend on the availability of pbs/aprun
    (see doc/platforms/README.xt-cle or README.xe-cle)
- for xmt:
  - improved the parallelization of operations on default layout arrays/domains
  - made XMT compilations ignore dmapped annotations on domains

Documentation
-------------
- improved the language specification to improve clarity, fix bugs
- updated the various READMEs, text files, and man page

Example Codes
-------------
- switched hpcc benchmarks to use sugared form of dmapped for clarity

Packaging Changes
-----------------
- moved test system helper scripts from test/Bin to util/test
- added ability for multiple communication substrate builds to coexist at once

Compiler Changes
----------------
- converted the compiler version to the format: major.minor.update

Third Party Changes
-------------------
- updated to GASNet version 1.14.2
- added GMP (the GNU Multiple Precision Arithmetic Library)

Runtime Library Changes
-----------------------
- improved the implementation of the fast fork optimization
- removed the mutex-related calls from the tasking layer interface

Launcher-specific notes
-----------------------
- made the pbs-aprun launcher use options to qsub rather than a script file

Compiler Flags
--------------
--report-scalar-replace : print statistics about scalar replacement
--scalar-replace-limit : sets the max size of variables replaced (default=8)
--[no-]tuple-copy-opt : enables tuple assignment optimization
--tuple-copy-limit <limit> : limits size of tuples for --tuple-copy-opt

Execution Flags
---------------
--callStackSize : specify the task call stack size
  (see doc/README.executing and doc/README.tasks)

Interoperability Changes
------------------------
- improved ability to express external C functions, types, and variables:
  - added support for external type and variable declarations
  - added support for expressing and passing external C array types
  - added support for type arguments to an external function
  (see doc/technotes/README.extern)

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed a bug in multiplying two imag values
- added a check to prevent comparisons between types and values using == or !=
  (e.g., 'var x = ...;  type t = ...; ...(x == t)...' is now illegal)
- added a check to ensure tuple-style decls match the size of their initializer
  (e.g., 'var (x, y) = (1, 2, 3);' will now generate an error)
- fixed a pair of nil pointer dereference bugs in the Cyclic distribution
- made string.length return an 'int' as documented rather than an 'int(64)'
- fixed a bug in which the compiler seg faulted if no .chpl files were given
- generate a compiler error when applying '#' to a completely unbounded range
  (i.e., ".. # 10" is now a compiler error)
- fixed a bug in which queried argument types sometimes became void incorrectly
- made values of dataParTasksPerLocale and dataParMinGranularity < 0 an error
- added a check to make sure maxThreadsPerLocale >= 0
- fixed a race condition in the initialization of the fifo tasking layer
- fixed a bug in which functions returning enums were flagged as not
- fixed a bug in which failing to capture an iterator caused it not to be called
- removed a race condition on calling halt() from multiple tasks in a locale

Error Message Improvements
--------------------------
- 'illegal use of a function that does not return a value' now says which fn.

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- improved the compiler's ability to narrow wide references when possible
- optimized small tuple assignment to use scalar assignment rather than memcpy
- turned the --localize-global-consts flag on by default
- removed empty records from the compiler's intermediate representation
- improved scalar replacement for nested records

Testing system
--------------
- added ability for testing system to generate graphs in performance mode
  (see comments in start_test for details)

Internal
--------
- added documentation on compiler arch. to doc/developer/compilerOverview (SVN)
- refactored the $CHPL_HOME/util directory into several subdirectories
- refactored third-party directory and Makefiles
- generally removed file extensions from shell scripts
- made internal version number take the form: major.minor.update.svnrev
- made the compileTest script more robust w.r.t. -I, -M, .h, etc. arguments
- cached the number of processor cores in the locale type
- added versions of compilerError and compilerWarning that take a call depth
- reduced the number of runtime files that get rebuilt when the compiler changes
- simplified the implementation of associative arrays to reduce copies/temps
- removed the 'size' field from the default arithmetic array class
- made our implementation of Norm.chpl use slicing rather than rank change
- replaced the isEnumType primitive with calls to _isEnumeratedType()
- replaced several primitives with extern functions
- added a new repositionDefExprs pass to make local variables safe on XMT
- added --local-temp-names flag to use a different temp naming scheme
- changed the --gen-communicated-structures developer flag to --heterogeneous



version 1.1.2
=============

Update to fourth public release of Chapel, September, 2010
(see also changes below for 1.1.1 and 1.1)

Platform-specific notes
-----------------------
- added an xe-cle PLATFORM to support the XE6 (currently using GASNet over MPI)
  (see doc/platforms/README.xe-cle for details)



version 1.1.1
=============

Update to fourth public release of Chapel, July 8, 2010
(see also changes below for 1.1)

Platform-specific notes
-----------------------
- removed dependence on linking against libpct on Cray XTs
- fixed a bug in which linux did not use 'make' if 'gmake' was unavailable

Bug fixes
---------
- fixed a race condition in our mutex initialization code
- fixed a bug in fillRandom() in which using a clock-based seed did not do so



version 1.1
===========

Fourth public release of Chapel, April 15, 2010

High-Level Themes
-----------------
- single- and multi-locale task parallelism
- improved single- and multi-locale data parallelism
  - all data parallel operations on arithmetic arrays are now parallelized
  - improved control over task quantity and granularity for data parallelism
  - improved completeness and stability of Block and Cyclic distributions
- bug fixes

Environment Changes
-------------------
- added CHPL_TASKS to select the tasking layer to use (was CHPL_THREADS)
  (see doc/README.chplenv and doc/README.tasks for details)
- CHPL_THREADS now describes lower-level threading and is typically inferred
  (see doc/README.tasks)
- changed the name of the default tasking layer from 'default' to 'fifo'
  (see doc/README.chplenv and doc/README.tasks)
- added CHPL_TIMERS to select between timer implementations
  (see doc/README.chplenv)
- changed the CHPL_*_PLATFORM string for 32-bit linux from 'linux' to 'linux32'
  (see doc/README.chplenv)

Syntactic/Naming Changes
------------------------
- several changes related to the naming and syntax of distributions:
  - renamed "distributions" to "domain maps" (or 'dmap's in Chapel syntax)
  - redefined the term "distributions" to refer to multi-locale domain maps
  - defined the term "layouts" to refer to single-locale domain maps
  - replaced placeholder 'distributionType'/'distributionValue' with 'dmap'
    (e.g., 'const D = distributionValue(...)' => 'const D = new dmap(...)'
  - changed the 'distributed' keyword to 'dmapped'
  - added a syntactic sugar to drop 'new dmap(new' for anonymous domain maps
    (e.g., '...dmapped new dmap(new Block(...))...' => '...dmapped Block(...)')
  (see 'Domain Maps' chapter of language specification for more information)
- renamed maxThreads to maxThreadsPerLocale (see README.tasks)
- renamed the 'ind2loc' method on domain maps to 'idxToLocale'
- renamed the Cyclic constructor 'low' argument to 'startIdx'
  (see 'Standard Distributions' chapter of the specification)
- replaced the use of underscore (_) to ignore things to leaving the space blank
  (e.g., '(_, x) = getTuple()' => '( , x) = getTuple()')
- made underscore (_) a legal identifier
- changed name of the Random module's SeedGenerator from clockMS to currentTime
  (see 'Standard Modules' chapter of the specification)
- added support for empty (no-op) statements within a class declaration
- added ability to elide the # of arguments specification/query from varargs fns
  (e.g., can now do 'def f(x...)' in addition to 'def f(x...?n)'/'def f(x...n)')

Semantic Changes/Changes to Chapel Language
-------------------------------------------
- added config consts to control the number of tasks used for data parallelism
  (see README.executing and 'Data Parallelism' chapter of specification)
- redefined enumerated domains to be a special case of associative domains
  (see 'Domains' chapter of specification)
- made iteration over enumerated domains/arrays use the enum's declaration order
  (see 'Domains' chapter of the specification)
- removed the ability to assign array aliases to normal array variables
  (e.g., 'var A => B;' is legal but 'var A: [1..n] real;  A => B;' is not)
- added initial support for passing array aliases to default class constructors
  (e.g., 'class C {var X: [1..n] int;} var A: [1..n] int; var c = new C(X=>A);')
- removed the sugar for declaring array types with forall expressions:
  (e.g., 'var A:[i in 1..n] real =...' => 'var A:[1..n] real = [i in 1..n] ...')
- defined reindexing to ignore the domain map of the indexing domain
- specified that minloc/maxloc reductions return the lowest matching index
  (e.g., 'minloc (A, 1..3)' for A = '5 0 0' would return (0, 2))
- type aliases may now only be used after they are defined
  (e.g., 'var x: t; type t = int;' is now illegal)
- changed resolution to favor functions with where clauses over those without
  (e.g., for 'foo(3)', 'foo(x)' is not as good a match as 'foo(x) where x:int')
  (see 'Function Resolution' section of the specification)
- added the ability to index into 1-dimensional arithmetic arrays using 1-tuples
  (e.g., 'var i: 1*int;  ...  A(i)' where A is a 1D arithmetic array)
- added the ability to query the component types of formal tuple arguments
  (e.g., 'def f(t: (real, ?t)) { ... }')
- redefined order-of-evaluation of tuple assignments to avoid array temps
  (see 'Tuples' chapter of specification)
- added support for indexing into tuples using any integral type, not just 'int'
  (e.g., given a tuple variable t, 'var i = 1:uint; ...t(i)...' is now legal)
- added support for 1-tuple detupled variable declarations for completeness
  (e.g., in 'var (x): 1*int;', x is of type 'int')
- redefined assignments to evaluate the left-hand side before the right
  (e.g., in 'x = y;', x is evaluated first and then y)

Newly Implemented Features
--------------------------
- whole-array assignment now results in parallel execution for arithmetic arrays
  (e.g., 'A = B; and 'A = B + alpha * C' now result in parallel execution)
  (see 'Data Parallelism' chapter for details)
- added support for assigning to a variable of dmap type if it has no domains
  (e.g., 'var myMap: dmap(Block(2)); myMap = new dmap(new Block([1..10]));')
- can now apply domain maps to domain values rather than simply domain types
  (e.g., 'const Dom1 = [1..10] dmapped ...')
- added support for sparse domain assignment (executed serially with a warning)
- added support for removing indices from a sparse domain (not thread-safe)
- added support for declaring formal tuple arguments of explicit tuple types
  (e.g., 'def f((x, y): (int, real)) { ... }')
- improved support for zippering domains/arrays with distinct domain maps

Standard Domain Maps
--------------------
- the Block and Cyclic distributed domain maps are now feature-complete
- added reindexing and rank change operations to the Block distribution
- reordered Block's constructor args so that rank and idxType can be inferred
  (e.g., 'new Block(rank=1, idxType=int(64), [1..n])' => 'new Block([1..n])')
- similarly reordered Cyclic's constructor args and renamed 'low' to 'startIdx'
- made int(32) the default idxType for standard distributions like Block
- moved all standard domain maps into modules/layouts/ and .../distributions/
- factored common routines for arithmetic domains into ChapelArray.chpl
  (e.g., nothing about exterior() is specific to a domain map's implementation)
- refactored helper functions for distributions into a new file: DSIUtil.chpl

Standard Modules
----------------
- refactored the code in the Random module and improved the implementation
- changed the Random module's SeedGenerator to an enumerated type
- added support for filling random vectors of imaginary types

Platform-specific notes
-----------------------
- added an mtarun launcher for the Cray XMT
  (see README.launcher)
- on the Cray XMT added support for platform-specific timing routines
  (see README.chplenv)
- for the Cray XMT, fixed a bug with uninitialized variables and poison bits

Portability of code base
------------------------
- ported code base to recent versions of NetBSD (thanks to Phil Nelson)
- removed hard-coded paths to perl and bash in scripts, relying on /usr/bin/env
- added parentheses around some expressions to avoid compiler warnings

Documentation
-------------
- updated the STATUS file
- added the first description of using domain maps to the specification
  (see 'Domain Maps')
- performed a major restructuring of the language specification:
  - split the 'Domains and Arrays' chapter into two distinct chapters
    (see the 'Domains' and 'Arrays' chapters)
  - split the 'Parallelism and Synchronization' chapter of the spec into two
    (see 'Task Parallelism and Synchronization' and 'Data Parallelism')
  - moved the 'Reductions and Scans' chapter of the spec into 'Data Parallelism'
  - moved discussion of promotion into the 'Data Parallelism' chapter
  - split the 'Locales and Distributions' chapter of the spec into three
    (see 'Locales', 'Domain Maps', and 'User-Defined Domain Maps')
  - reordered the chapters of the specification for clarity
  - added a placeholder chapter to the specification for user-defined reductions
    (see 'User-Defined Reductions and Scans')
  - added a 'Standard Distributions' chapter that describes Block and Cyclic
  - added a placeholder chapter for 'Standard Layouts'
- improved the specification:
  - added a publication date to the title page of the specification
  - updated the operator precedence table in the specification
    (see 'Lexical Structure' chapter)
  - improved the specification's syntax diagrams for [co]for[all] loops
  - minor improvements to the 'Modules' chapter of the specification
  - made major improvements to the 'Tuples' chapter
  - improved the accuracy of the description of function resolution
  - reorganized/rewrote the 'Domains' and 'Arrays' chapters (more remains)
  - improved the consistency of prototypes in the 'Standard Modules' chapter
  - updated the description of the Random number module to reflect changes
  - fixed several bugs in example codes within the specification
- added doc/platforms/README.xmt to describe the use of Chapel on a Cray XMT
- added doc/platforms/README.sgi to describe the use of Chapel on an SGI Altix
- updated READMEs based on task/thread change
  (see doc/README.tasks and doc/README.executing)
- added a README to the $CHPL_HOME/util directory to describe its contents
- updated READMEs for clarity and accuracy
- reformatted the CONTRIBUTORS file to reflect contributors to a given release
- updated the quick reference card's contents

Example Codes
-------------
- added a new example 'distributions.chpl showing use of Block and Cyclic
- removed 'block2D.chpl' example, considered redundant with the above
- added a new example to show simple multitasking (hello-multicore.chpl)
- renamed the 'examples/tutorials' directory 'examples/primers'
- rewrote the taskParallel.chpl primer example program
- updated all examples to reflect language changes:
  - to reflect new dmap/dmapped syntax for specifying domain maps
  - the change of Block and Cyclic's default idxType to int(32)
  - to remove reliance on the 'A:[i in ...]' syntactic sugar for array decls
  - to remove the use of underscores to represent ignored values
  - to track the changes to the Random module interface
  - to reflect the new controls for specifying degree of data parallelism
  - to reflect the new constructor argument orders for Block and Cyclic
  - to reflect the renaming of ind2loc to idxToLocale
- renamed helper files in hpcc/ subdirectory to match the modules they define
- updated comments in ra-randstream.chpl, quicksort.chpl
- stopped unrolling a loop in ra-randstream.chpl's computeM2Vals routine
- added hpcc/stream-ep to the Makefile and README (overlooked in last release)

Packaging Changes
-----------------
- pushed tarball directory structure down one level (i.e., chapel-1.1/chapel)
- moved non-user scripts from $CHPL_HOME/util/ to $CHPL_HOME/util/chplenv/

Compiler Changes
----------------
- added warnings for most serialized data parallel operations
- added warnings for array reindexings using strided ranges/domains
- improved orthogonality of parsing type expressions in expression contexts
- turned off optimizations by default for CHPL_COMM=gasnet; use -O to enable
- reimplemented scans to remove the creation of a temporary array

Runtime Library Changes
-----------------------
- reorganized the runtime directories to use better hierarchies
  (e.g., 'runtime/foo-bar' => 'runtime/src/foo/bar'
- broke the threading layer of the runtime into tasking and threading layers
- renamed most external tasking/threading routines in the runtime interface
- added support for different timer implementations and platform-specific timers
- renamed chapel_code.chpl to chpl_rt_utils.chpl
- exposed sleep() calls to the tasking layer

Launcher-specific notes
-----------------------
- added an mtarun launcher for the Cray XMT

Compiler Flags
--------------
- --conditional-dynamic-dispatch-limit flag for using vtables vs. conditionals
- --[no-]fast-followers to control optimization of aligned parallel iterators

Interoperability Changes
------------------------
- added initial support for declaring external C struct types
  (see doc/technotes/README.extern for details)

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- removed the ability to assign values to types
- added a check that config param enums are set to a legal value for the enum
- added a check that tuple assignments are between tuples of matching size
- added a check that iterators are not defined to yield types or params
- added a check to ensure zippered sparse domains/arrays share the same domain
- added an error message for param for loops that iterate over non-param ranges
- fixed a bug in which a shared initializer was evaluated multiple times
  (e.g., 'var x, y, z = foo()' called foo() three times rather than one)
- fixed a bug in which yield statements could omit the expression to yield
  (i.e., 'yield;' is not a legal statement in Chapel)
- fixed a bug in which applying .locale to a global variable always returned #0
- fixed a few race conditions in the runtime tasking layer
- fixed an integer overflow problem in the locale.physicalMemory() query
- fixed a runaway recursive error message in erroneous CHPL_COMM=gasnet exits
- fixed a bug in associative domains in which stale array values were not reset
- fixed bugs in which argument coercion and promotion broke extern functions
- fixed a bug in which a 'use' in a module-level non-function scope leaked names
- fixed a bug where returning a sync var was flagged as not returning anything
- fixed a bug in which generic class member types could not be queried
- fixed a bug in which a class' type declaration could not define multiple types
- fixed a bug in which printing an empty record would print an internal field
- fixed a bug in which recursive fns in local blocks led to infinite compilation
- fixed bugs in function resolution that led to choosing a poorer match
- fixed implementation of type aliases to evaluate them during fn resolution
- fixed a bug in the visibility of parallel iterators in promotion wrappers
- fixed a bug in dead code elimination considered already eliminated code
- fixed a bug in which compiler warnings were sometimes not generated
- fixed a bug in which standalone forall loops incorrectly localized references
- fixed a bug in flattening nested functions that removed non-dead references
- fixed a bug in which the compiler would incorrectly drop line#/filename info
- fixed a bug in which duplicating a loop would not update its labels
- fixed the instantiation point for compiler-introduced wrapper functions
- fixed a bug in which "false" where clauses were counted as instantiations

Error Message Improvements
--------------------------
- squashed "too many instantiation" error messages for internal types/functions
- somewhat improved the error message for applying 'new' to a non-class

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- turned on optimizations of on-clauses by default (--optimize-on-clauses)
- re-implemented homogeneous tuples to use C arrays rather than structs
- generated new loop idioms to replace fragile static alignment analysis
- Cyclic distributions now result in similar performance to Block for e.p. codes
- added the ability to use a virtual method table for dynamic dispatch

Compiler Performance and Cleanup
--------------------------------
- reordered compiler optimizations to improve performance
- rewrote copy propagation to remove a quadratic aspect of the algorithm
- changed conditional-based dynamic dispatch to use statements rather than exprs
- improved performance of function resolution
- improved performance of the reaching definitions analysis

Cleanup of Generated Code
-------------------------
- made generated code less sensitive to small changes to better support diffs
- broke user code within chpl_main() into its own function: chpl_user_main()

Testing system
--------------
- added ability to test properly formatted codes embedded in the specification
- changed sub_test script to use Python for better flexibility/maintainability
- fixed the redundant cleaning of directories when tests are explicitly named
- made sub_test clean up executables for .noexec tests

Internal
--------
- added a developer optimization to localize global constants (off by default)
- added a new helper function gen() that serves as a printf for codegen
- added a debugging function printCallStack() for use during function resolution
- added a new mechanism for building the IR after parsing, before normalization
- refactored parser to treat types and expressions more uniformly
- refactored parsing of serial and parallel loops to share more code
- simplified the implementation of operator overloading on tuples
- cleaned up the implementation of reductions
- reimplemented Math.chpl routines using _extern rather than primitives
- reimplemented most Memory.chpl routines using _extern rather than primitives
- generalized a mechanism for soft-resolving a possibly erroneous function call
- added a displayRepresentation method to dmap, domain, and array classes
- removed the unused 'reindexed' param from default arithmetic arrays
- added the "dsi" prefix to methods that are part of the dmap standard interface
- better distinguished between thread and task IDs
- added getValType(), getRefType(), getWideRefType() helper functions
- added support for _isSignedType() and isUnsignedType() helper param functions
- moved the implementation of queries in formal types to function resolution
- simplified the generation of BUILD_VERSION for non-developers



version 1.02
============

Update to third public release of Chapel, November 12, 2009
(see also changes below for version 1.01 and 1.0)

High-Level Themes
-----------------
- single- and multi-locale task parallelism
- single- and multi-locale data parallelism
- improved performance and scalability
- bug fixes

Packaging Changes
-----------------
- added GASNet 1.14.0 to third-party/ though 1.12.0 is still the default
  (see third-party/gasnet/README)

Runtime Library Changes
-----------------------
- vastly improved the performance of CHPL_COMM=pvm, esp. when oversubscribing

Documentation
-------------
- removed the HPCC-related PDFs from doc/ as they seem better suited for the web

Example Codes
-------------
- added an EP version of stream to showcase Chapel in an SPMD-like mode
  (see $CHPL_HOME/examples/hpcc/stream-ep.chpl)
- added local blocks to ra.chpl and fft.chpl to assert regions of locality
- changed fft.chpl to use both Block and Cyclic arrays to optimize for locality

Standard Distributions
----------------------
- optimized the Block distribution via privatization and remote value forwarding
- added support for a .localSlice() operation for Block and Cyclic array
- optimized away some extraneous copies from standard distributions

Compiler Flags
--------------
- added --[no-]optimize-on-clauses and --optimize-on-clause-limit

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed a bug in which wide references weren't removed from local blocks
- fixed a bug in which range slicing resulted in divide-by-zero due to overflow
- fixed a bug in which Block misdistributed indices at sufficiently large sizes
- fixed a bug in which multirealm PVM runs could set up locales in wrong order

Error Message Improvements
--------------------------
- improved the error messages for failed pvm launches

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- added an optimization to execute on-clauses using a remote locale's listener
  (see --[no-]optimize-on-clauses and --optimize-on-clause-limit)

Internal
--------
- added initial support for recursive leader iterators
- added support for generating Chapel errors in terms of C line/file
- refactored and cleaned code in PVM launcher and comm layer (more remains)
- added Makefile.tau for experimental use of Chapel with Tau



version 1.01
============

Update to third public release of Chapel, October 30, 2009
(see also changes for version 1.0)

High-Level Themes
-----------------
- multi-locale task parallelism
- support for single- and multi-locale data parallelism
- improved scalability and stability
- prototype implementation of realms for hybrid computing in Chapel
  (see doc/technotes/README.multirealm)

Packaging Changes
-----------------
- added a utility script, chpl-multirealm, to help with multi-realm compilation
- added PVM to the third-party directory to support multi-realm computation

Newly Implemented Features
--------------------------
- added support for multi-realm features to support heterogeneous computation
- extended replication of global constants to include record types

Platform-specific notes
-----------------------
- made XT GASNet compiles use "cc" without the full path discovered by GASNet

Documentation
-------------
- updated .emacs excerpts in etc/emacs/README to use more modern idioms

Example Codes
-------------
- added an example demonstrating multirealm computation features
- cleaned up RA to refer to TableDist in on-clauses rather than T.domain.dist

Standard Distributions
----------------------
- made the default distributions throttle parallelism based on #tasks running
- improved the Cyclic distribution to support strides and slicing
- added initial support for a "Private" distribution with one index per locale

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed a bug in which distributions deadlocked when numLocales > maxThreads
- fixed a bug with parallel iteration when problem size ~= #cores
- fixed some bugs with slicing and querying the length of degenerate ranges
- fixed some bugs in distributions when iterating over degenerate ranges

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- reduced the number of communications required to set up distributions
- reduced the time required to set up Block distributions
- reduced the time required to create privatized data structures
- optimized iterators for Block- and Default-distributed Arithmetic arrays
- optimized the expression .locale.uid to avoid communication
- optimized .dist methods on default domains to return the private distribution

Testing system
--------------
- softened the signal used by our timeout script from -9 to -15

Internal
--------
- fixed some const char* issues in strstr, strchr, strrchr calls
- added verification that gotos are in same functions as their labels



version 1.0
===========

Third public release of Chapel compiler, October 15, 2009

High-Level Themes
-----------------
- multi-locale task parallelism
- improved support for single- and multi-locale data parallelism
- improved stability and portability
- improved memory utilization of compiler-generated code
- target audience: general public

Environment Changes
-------------------
- added emacs/vim modes to release -- see $CHPL_HOME/etc/README

Changes to Chapel Language
--------------------------
- removed implicit coercions from primitive types to strings to avoid confusion
- a default array variable can now be made to alias another via the => operator
- accesses to variable x in module M using 'M.x' must now follow a 'use M'

Newly Implemented Features
--------------------------
- forall loops over ranges & arithmetic domains/arrays are now parallelized
- improved support for and correctness of record and class destructors
- array declaration+initialization syntax now results in parallel evaluation
  e.g., var A: [i in D] real = foo(i);  will be evaluated in parallel
- added == and != for imag and complex types; added >, >=, <, <= for imag types

Portability of code base
------------------------
- improved support for the Cray compiler on XT systems (cray-xt-cray)
- reduced warnings for gcc > 4.3
- improved portability with respect to Intel icc/icpc v11.x
- removed outdated assumptions about Sun compiler environments
- removed the makechpl script for Mac users because of portability issues

Platform-specific notes
-----------------------
- added a PBS launcher for the Cray CX1 named pbs-gasnetrun_ibv

Launcher-specific notes
-----------------------
- several improvements to the pbs launcher (see README.launcher/README.xt-cle)
  - environment variables are now propagated to the application
  - a queue can be specified via the CHPL_LAUNCHER_QUEUE environment variable
  - a wallclock limit can be specified via CHPL_LAUNCHER_WALLTIME
  - the NCCS pbs launcher no longer uses the debug queue by default
- added support for CHPL_LAUNCHER_SUFFIX to launch a file other than ..._real

Semantic Changes
----------------
- changed distributions from having class/reference semantics to value semantics
- made module initialization occur at program startup rather than use statements
- only modules specified on the command-line are candidates for the main module
- added support for returning locally scoped arrays from variable functions
- changed interpretation of method definitions on scalar types
  e.g., 'def int.foo()' now defines foo() for default-sized ints, not all ints

Syntactic/Naming Changes
------------------------
- renamed MultiBlockDist.chpl to BlockDist.chpl
- removed the Block1D distribution since Block subsumed it
- added placeholder notation for creating new distribution values
  e.g., new Block(...) => distributionValue(new Block(...))
- renamed the pbs launcher for Cray XT to pbs-aprun since it wraps both packages

Compiler Changes
----------------
- improved support for slicing [strided] domains/arrays with [strided] slices
- improved flushing of writeln() statements to the file being written to
- removed support for goto from the compiler's front-end

Runtime Library Changes
-----------------------
- improved pthread setup, termination, and cleanup for non-erroneous exits
- refactored threading runtime to support code reuse for pthread-like threads
- added support for memory tracking for multi-locale executions

Documentation
-------------
- improved the Types, Modules, and Ranges chapters of the language specification
- added mention of 'delete' to language specification
- improved the Label, Break, and Continue subsection of the language spec
- minor changes to other chapters of the language specification
- updated README.xt-cle and README.launcher to reflect new pbs features
- updated the various READMEs to reflect minor changes and wording

Example Codes
-------------
- changed fft to use a Block distribution
- changed reference to MultiBlockDist module in block2D.chpl to BlockDist
- changed distributions to use the placeholder value type notation
- changed default value of tasksPerLocale in HPCC benchmarks to avoid reductions
- changed RA's constant array m2 into a constant scalar for performance reasons
- changed follower iterator in ra-randstream.chpl to accept tuple of ranges
- deleted classes in example programs to reclaim memory
- increased problem size for reductions.chpl to avoid bug w/ 5+ cores per locale

Standard Modules
----------------
- added printMemStat() to the standard Memory module; improved printMemTable()
- added start/stopVerboseMem[Here]() to the Memory module for tracing memory use
- improved reference counting of domains and arrays
- removed the (undocumented) Schedules module

Standard Distributions
----------------------
- merged Block1D and Block since the latter subsumed the former
- removed the default rank of 1 for the Block distribution
- added support for a multidimensional target array of locales to Block
- improved support for strided domains/arrays in the Block distribution
- added support for slicing to the Block distribution
- added support for member(), order(), and position() to the Block distribution
- added initial support for a Cyclic distribution
- added very preliminary support for a Block-Cyclic distribution
- improved the support for the CSR distribution to match the default sparse case
- unified leader/follower iterators to always work on tuples of ranges
- removed subBlocks() from the standard distribution interface

Compiler Flags
--------------
- added support for a module path flag (-M) to search for modules via filenames
- added a flag to print the module search path (--print-search-dirs)
- added a flag to print module files being parsed (--print-module-files)
- added support for a -I flag to specify a search directory for C headers

Generated Code Flags
--------------------
- added support for specifying configuration variables/constants without =
  e.g., you can now use './a.out --n 4' in addition to ./a.out --n=4'
- improved flags for tracking memory utilization (see README.executing)
- improved error messages to indicate the argument number
- made compiler-generated generic type names deterministic
- improved robustness of --numLocales flag

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- added an error for using => on non-array types
- added an error for using (...x) on non-tuple types
- added a semantic check against tuples sized by 0 or a negative value
- made labels on statements other than serial loops be errors
- made break and continue only applicable within serial loops
- improved error checking when assigning between ranges of different boundedness
- fixed a bug in which breaks in serial loops gave errors in parallel contexts
- fixed a bug in which tuple copies sometimes aliased the original tuple
- fixed a bug in which generic fields were incorrectly aliased in constructors
- fixed a bug in which we were accidentally supporting illegal parameter casts
- fixed a bug in which string parameter members broke the compile
- fixed a bug in which indices were inadvertently shared/non-local in promotions
- fixed a bug in which pbs launchers did not work with shell prompts ending in $
- fixed a bug in which the compiler attempted to clone external functions
- for Cray XT, fixed default setting of GASNET_MAX_SEGSIZE to specify size in KB
- fixed a race in the creation of private, replicated distribution classes
- fixed a bug in which tensor iteration resulted in internal errors
- removed a subtle race condition in program startup
- fixed a bug in which we called default constructors by name
- fixed deletion of list elements in List module
- added support for generating the implicit Program module in --html view

Error Message Improvements
--------------------------
- fixed line numbers in errors involving dynamic dispatch and [con/de]structors

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- vastly reduced amount of memory leaked due to compiler-allocated data
- improved performance of loops using Block distributions
- improved performance and reduced memory requirements for memory tracking

Cleanup of Generated Code
-------------------------
- embedded information about compilation options to the generated code

Testing system
--------------
- improved precedence of execution options specified via .execopts or EXECOPTS
- made parallel testing place -nl x flags at the end of the command line
- added support for PVM-based testing to the test script

Internal
--------
- replaced uses of "[unsigned] long long" with [u]int64_t for sane portability
- some unification of reserved names, though more remains
- improved Chapel's launcher runtime interface to be more general
- added a mechanism for intercepting printf/fprintf calls if required
- Makefile refactorings working toward supporting parallel make
- refactored runtime/mem- directories to decrease duplicated code
- renamed runtime files to improve standardization, though more remains
- removed linked list pointers from memory tracking table
- reduced amount of runtime code linked into the launcher binary
- made the use of chpl_globals_registry more consistent across locales
- relaxed compiler assumptions about classes with the "data class" pragma
- added support for generating type and offset information for communications
- some initial work toward supporting execution on heterogeneous architectures
- some initial work toward supporting CPU<->GPU computations in Chapel
- some initial work toward supporting profiling tools with Chapel
- removed "valid var" pragma
- made wrapper functions use blank intent rather than inheriting from wrappee
- changed strategy for determining when value types should be copied/destroyed
- made domain and array classes always have reindexed set to true by default
- added a developer flag for disabling compiler-introduced memory frees
- removed support for _init functions from the compiler
- removed assumptions that replicated global constants are stored symmetrically
- added thread cancel and join functions to the threading runtime interface
- added a type, chpl_threadID_t to pass thread IDs between C and Chapel
- changed point at which variables are put on the heap
- made all built-in modules be filename based
- refactored directory structure of $CHPL_HOME/modules into standard/internal...
- added optimizations to remove unnecessary autocopy/autodestroy calls
- improved robustness of internal modules that use standard ones
- changed the scan implementation to generate an array rather than a list
- changed array assignments to use array iteration rather than random access
- made --no-cpp-lines the default for -g for developers
- improved handling, robustness of built-in configuration variables
- split chpl_comm_md.h into two files to permit platform- and comm- overrides



version 0.9
===========

Second public release of Chapel compiler, April 16, 2009

High-Level Themes
-----------------
- multi-locale task parallelism
- very initial/limited support for data parallelism, distributed arrays
- improved stability, portability, and performance
- target audience: general public

Environment Changes
-------------------
- added a script to print out set and inferred CHPL env vars
  (see util/printchplenv.pl)
- added support for the IBM xlc/xlC compilers by setting CHPL_*_COMPILER to ibm
- added an environment variable for naming a queue for the PBS launcher
  (see doc/platforms/README.xt-cle)
- added an environment variable for specifying #cores/locale to pbs/aprun
  (see doc/platforms/README.xt-cle)
- improved the logic for inferring host and target compilers for xt-cle

Changes to Chapel Language
--------------------------
- added a typeToString() function for converting a static type into a string

Newly Implemented Features
--------------------------
- added queries for task/thread counts per locale for CHPL_THREADS=pthreads
  (see doc/README.threads)
- added a printMemTable() routine to the Memory module for use in Chapel
  (see doc/README.executing)
- added a prototype multidimensional Block distribution: MultiBlockDist.chpl
  (see examples/tutorials/block2D.chpl)

Portability of code base
------------------------
- improved our general method of building and using GASNet for portability
- improved portability of GASNet configure + build steps for xt-cle
- improved the heuristic for setting GASNET_MAX_SEGSIZE for xt-cle
  (see doc/platforms/README.xt-cle)
- added initial support for IBM Power5 and Power6 systems
  (see doc/platforms/README.ibm)
- made flex and bison generate .cpp files rather than .c files for portability
- added capabilities to build Chapel on Macs using xcodebuild
  (see doc/platforms/README.macosx)
- added a -d option to our use of aprun in the aprun and pbs launchers
- removed the assumption that function pointers are identical between nodes

Syntactic/Naming Changes
------------------------
- disallowed identifiers that start with $
- removed compilerError and compilerWarning as keywords
- permitted binary and hex literals to be written using uppercase B/X
- renamed classes used for defining distributions

Runtime Library Changes
-----------------------
- added support for communication diagnostics and debugging
  (see doc/technotes/README.comm-diagnostics)

Documentation
-------------
- added a new ACKNOWLEDGEMENTS file for financial and machine time support
- refactored the doc directory, pushing platform & technical notes into subdirs
- minor changes and improvements to the language specification
- minor edits to many of the README files
- updated the man page

Example Codes
-------------
- pushed most top-level examples into tutorials/ and programs/ subdirectories
- added new tutorial examples for variables, ranges, classes, generic classes
- added new tutorial examples for timers and the prototype Block distribution
- changed quicksort to ensure levels beyond thresh do not re-enable parallelism

Compiler Flags
--------------
- added a flag to disable/enable the remote value forwarding optimization

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed a number of spurious errors caused by how we were using GASNet
- fixed problems in how we terminated multithreaded GASNet programs
- added the ability for _chpl_comm_fork[_nb] to handle large arguments
- applied a GASNet patch that fixes an x86 alignment error
- fixed a bug which had previously limited execution to 1024 locales
- fixed a bug in which arrays could only store 2**31 elements
- eliminated a race condition when two tasks initialize the same module
- fixed a bug in which integer exponentiation was being computed using pow()
- fixed a bug in which slicing via .. could only be done for int(32) indices
- fixed a bug with Block1D domains that exceed the bounding box argument
- added a check to ensure that return statements are not used at module scope
- improved verification of formal/actual argument types
- fixed a bug in which long program names broke the PBS launcher
- fixed a bug with incorrectly spacing operators in generated code
- fixed Chapel error messages that referred to C's errno variable
- fixed a bug with accidentally shared state during parallel iterations
- fixed a bug related to reductions involving forall expressions

Error Message Improvements
--------------------------
- added a runtime error message if the launcher binary can't find the _real one

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- improved the correctness and effectiveness of remote value forwarding
- optimized performance of local communications
- optimized parallel iteration for Block1D to minimize remote references
- parallelized declarations of Block1D distributions, domains, and arrays
- optimized implementation of broadcasts used to initialize private data
- optimized away a thread per locale used for progress+termination
- improved reclamation of compiler-allocated memory
- improved the correctness and effectiveness of re-privatization optimizations
- privatized the default distribution object
- removed array/domain wrapper records

Compiler Performance and Cleanup
--------------------------------
- removed hand-baked and occasionally incorrect integer types int32, int64
- fixed an infinite loop when inlining recursive iterators
- fixed cases in which compiler temps were used outside the variable's scope

Cleanup of Generated Code
-------------------------
- removed redundant reference types from the generated code

Internal
--------
- switched iterator implementation to use records rather than classes
- improved the naming of identifiers in the runtime and generated code
- removed compiler support for (undocumented) anonymous record types



version 0.8
===========

First public release of Chapel compiler, November 14, 2008

High-Level Themes
-----------------
- multi-locale task parallelism
- improved task parallel correctness, features, thread management
- improved stability of core features
- very initial/limited support for data parallelism, distributed arrays
- target audience: general public

License Changes
---------------
- switched from MIT to BSD license (see LICENSE)
- updated user agreement text (see AGREEMENT)

Environment Changes
-------------------
- changed the identifier for the Cray XT platform from xt-cnl to xt-cle
- added a CHPL_MEM setting to select memory allocators (see README.chplenv)

Changes to Chapel Language
--------------------------
- changed operator precedence for : vs. **, bitwise ops, reduce, !, unary +/-
- added destructors and a 'delete' keyword for destroying class instances
- removed open interval syntax in which [0..n) == 0..n-1
- added range operator # that counts a number of elements
  e.g., 0..#n == 0..n-1;  0.. by 2 #5 == 0, 2, 4, 6, 8
- added support for locale.name to query a locale's node name
- added support for overriding iterators within a class hierarchy
- added the capability to declare generic formals for gen. classes with defaults
  e.g., "x: range" expects a default range; "x: range(?)" expects a generic one
- added the ability to query argument types without naming the queried type
  e.g., "x: ?" rather than "x:?t"
- added support for +/- on (arith. domain, index) and + on (index, arith. domain)
- removed support for * and / on (range, integer) and - on (integer, range)
- added support for .clear() on arithmetic domain vars to reset to degenerate
- added .order() and .position() methods to ranges and domains
  e.g., (1..3).order(3) == 2;  ([1..3, 1..3]).position((2,1)) = (1,0)
- added support for variable-width boolean types
- added support for ~ on bool types
- initial support for a local block that asserts code requiring no communications
  e.g., on Locale(1) { const x: int;  local { x = x + 1 } }
- added initial support for leader/follower iterators supporting zippered foralls
- added initial support for distributed domains and arrays using a Block1d class
- added initial support for parallel reductions and forall expressions

Newly Implemented Features
--------------------------
- added support for tuples of lvalues
  e.g., "for (a,b) in (A,B)" where A and B are arrays
- array initializer expressions can now be used for array class members
- added support for user-defined constructors for generic classes
- added support for overriding methods using subtype return types
- added support for querying the eltType of an array
  e.g., def foo(A: [D] ?t) { ... }
- added support for .remove() on opaque domains
- added support for explicitly naming a function's module at the callsite
- added support for freeing compiler-introduced heap-allocated variables
- added initial support for freeing value type memory (records, arrays, domains)

Portability of code base
------------------------
- added support for the Cray CX1 platform running Linux  (see README.cx1-linux)
- improved level of support for the Cray XT platform (see README.xt-cle)

Semantic Changes
----------------
- disallowed calling a method on a nil reference as previously permitted
- changed the instantiation point for dynamically-dispatched methods
- permitted partially-bounded out-of-bounds degenerate ranges to be legal, empty
  e.g., var A: [1..10] real;  A[11..] == A[11..10] == a legal, empty slice of A
- on clauses are now evaluated for side effects when using the --local flag
- made single-statement sync statements create a new scope
- added support for param string comparisons, concatenation, and other ops
- added support for casting an integer to a string at compile time
- made the implicit 'setter' argument in var functions constant and foldable

Syntactic/Naming Changes
------------------------
- all non-compound single-statement conditionals/loops now require then/do
- functions now require a block statement unless they only contain a return stmt
- added support for a single-statement serial statement
  e.g., "serial test { foo(); }"  =>  "serial test do foo();"
- removed "opaque" from the list of reserved words--it is now a type like "int"
- 'distributed' clauses no longer require parenthesis

Compiler Changes
----------------
- the compiler now requires CHPL_HOME to be set
- added support for opaque types for use when interfacing with extern functions

Third Party Changes
-------------------
- updated GASNet version to GASNet 1.12.0
- added support for using dlmalloc as a memory allocator

Runtime Library Changes
-----------------------
- all tasks in a cobegin are now set up before any start executing for pthreads
- made the task that starts a cobegin/coforall help execute the component tasks
- made the task that enters a sync statement help execute the component tasks
- added "launcher" capability to wrap job launch mechanism (see README.launcher)
- improved the thread-safety of declaring arrays over a domain

Documentation
-------------
- improved the --help descriptions of --[no-] flags
- updated and improved the language specification
  - improved the specification of instantiated fns: visibility & point-of-inst.
- updated the various README files
- updated the man page

Example Codes
-------------
- added a hello-multiloc example to run a "Hello, world!" on multiple locales
- improved hpcc codes from our 2008 HPC Challenge entry and added HPL (hpl.chpl)
- made the stream.chpl and ra.chpl benchmarks use Block1D-distributed domains
- added support for the stream and ra benchmarks to use multiple tasks per locale
- improved the verification step of stream.chpl to make it stronger
- improved the taskParallel example to reduce the likelihood of races

Standard Modules
----------------
- added a Norm module for computing vector and matrix norms
- added a Containers module for standard container classes
- added a day of week enumeration and getCurrentDayOfWeek() to the Time module
- fillRandom() calls in the Random module now fill Block1D arrays in parallel

Compiler Flags
--------------
- improved the parsing of single-dash compiler flags to avoid common errors
  e.g., -abcd != -a -b -c -d; -hepl != -h -e -p -l
- renamed --no-expand-iterators-inline-opt to --[no-]inline-iterators
- renamed --no-single-loop-iterator-opt to --[no-]optimize-loop-iterators
- removed the --chplhome flag
- removed the --flow-analysis flag
- made all optimization control flags into --[no-] flags
- added --[no-]dead-code-elimination to turn dead-code elimination off/on
- added --[no-]live-analysis to turn live variable analysis off/on

Generated Code Flags
--------------------
- added a -b/--blockreport flag to help debug deadlocked 1-locale programs
- added a -t/--taskreport flag to print a list of tasks for 1-locale programs
- made the -v flag print node names for locales on which code is executing
- made the -v flag print the commands used by the launcher

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed some subtle race conditions in the pthreads version of the runtime
- fixed a bug in which write statements would not print atomically as intended
- fixed a bug in which multidimensional tuple-to-array assignment was failing
- fixed support for readXX() on single variables
- added some runtime checks to try and avoid integer wraparound in malloc sizes
- improved support for range slicing at extreme integer values
- made 1-tuple-style declarations illegal:  var (x) = ...;
- fixed a number of bugs related to generic function instantiation points
- fixed a bug in which casts on return expressions were ignored
- fixed a bug related to iterating over a single iterator class multiple times
- fixed a bug in which param functions with multiple returns corrupted constants

Error Message Improvements
--------------------------
- improved an error message related to illegal assignments of domains to arrays
- added an error check to prevent iteration over a type
- improved error messages related to bad tuple index types
- improved error messages related to iterators and promoted expressions
- added a warning about cobegin statements with fewer than two statements

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- made "here" into a per-locale variable, reducing communication
- optimized read-only non-local accesses to value types
- added initial support for privatizing global domain/array descriptors
- added initial support for optimizing zippered iteration over aligned arrays
- optimized coforall...on and begin...on idioms to reduce thread requirements
- added reference counting to domain classes to aid in freeing them

Compiler Performance and Cleanup
--------------------------------
- rewrote the compiler's symboltable
- added support for a BitVec class to save space and time
- tightened up memory leaks in the compiler
- specialized parameter tuple indexing for performance and fewer special cases
- reimplemented the IR traversal mechanism
- removed def/use information from the Symbol class to save space
- removed filename references from IR nodes to save space
- removed unused fields from internal classes
- reimplemented the caching data structures used in function resolution
- streamlined function instantiation when a function is not a viable candidate
- refactored and cleaned up the wrapper/generic function caching code
- applied reference propagation to functions just prior to inlining
- moved the prune pass prior to optimizations to save time
- optimized the verification pass by hoisting out back pointer checks
- optimized reference propagation to build smaller use/def sets
- optimized the scalar replacement pass to localize the iterative aspects
- improved the set of live references added/omitted from iterator classes
- eliminated lots of dead code

Testing System
--------------
- made the test system execute .numlocales tests on 1 locale for CHPL_COMM=none
- made the testing system set the CHPL_* environment variables if they aren't
- made the test system generate its output using relative paths
- fixed a bug in the testing system's "skipif" capability

Internal
--------
- added support for dynamically turning on/off communication and memory tracing
- improved debug output for the gasnet communication layer
- reorganized abstract distribution, domain, array classes
- generalized creation of runtime types
- renamed SingleLocale* classes for domains/arrays to Default* classes
- removed need for buildEmptyDomain methods within domain classes
- removed redundant type/param fields from array wrapper records
- simplified the AList class
- revamped implementation of pragmas and unified with various boolean flags
- added a --runtime flag to compile Chapel code into the runtime and used it
- removed --nostdincs developer compiler flag
- improved the format of the --print-statistics flag and added summary info
- made the --baseline flag into an aggregate flag
- fixed a bug with the --gdb flag in which quit and run would fail after errors
- improved the cleanup of temporary files when using the --gdb flag for gdb >6.6
- added support for developers to print references in 0x0000abcd form
- added a rootModule that contains the program module but is not codegen'd
- decoupled the visible functions data structure from the symbol table
- removed the need for the symboltable after scope resolution
- changed module representation of CHPL_* variables from enums to strings
- added support for compiler CHPL_* variables of type const char*
- added an internal traceback capability to unravel inlined functions
- changed the mechanism for specifying reserved symbol names needing mangling
- refactored reference propagation to share code with scalar replacement
- added debugging functionality to the scalar replacement pass
- replaced compute_sym_uses() with buildDefUseMaps() and for_defs/uses macros
- simplified and optimized the normalize pass
- removed the def/use map in function resolution
- added a promotion wrappers cache to avoid creating duplicate wrappers
- general cleanup of coercion wrapper code
- refactored instantiation code
- changed handling of on, begin, cobegin, coforall to use blocks, not nested fns
- reimplemented heap allocated variables
- changed arithmetic array of primes into function returning primes
- made all exits from the compiler use our clean exit routine



version 0.7
===========

Fourth preliminary release of Chapel Compiler, March 31, 2008

High-Level Themes
-----------------
- multi-locale task parallelism
- improved task parallel correctness, features, thread management
- improved stability of core features
- target audience: HPCS mission partners, select HPC/academic/industry users

Changes to Chapel Language
--------------------------
- added "new" keyword to invoke class/record constructors
  e.g., "class C { ... }  var myC = C(...);" => "... var myC = new C(...);"
- enumerated types now require the type to be named before a component symbol
  e.g., "enum myEnum {A, B, C}; var x = B;" => "... var x = myEnum.B;"
- changed a file's mode from a string to an enumeration
  e.g., file(..., "r") => file(..., FileAccessMode.read)
- added "sync" statement to join dynamic parallelism
  e.g., sync { /* create parallelism */ }  // waits here for parallelism to end
- added "here" constant to refer to the locale on which a task is executing
- added support for readXX and isFull on single variables
- added support for functions with type varargs
  e.g., def foo(type t ...?numTypes) { ... }
- added support for vararg read()/readln() functions that return a tuple value
  e.g., "read(int, float, string);" returns an (int, float, string) value
- added a sorted() iterator to associative domains/arrays and opaque arrays
  e.g., var table: domain(string); ...  for entry in table.sorted() do ...
- added support for a compilerWarning() call similar to compilerError()
  e.g., compilerWarning("I didn't expect this function to be called");
- added string relational operators
  e.g., ..."hi" >= "bye"...
- removed the "of" keyword
- removed the "ordered" keyword and concept

Newly Implemented Features
--------------------------
- execution using multiple locales (see: doc/README.multilocale)
- use of on clauses taking locale/lvalue expressions to generate remote tasks
  e.g., "on Locales(i) do ...",  "var x = ...;  ...  on x do ..."
- use of <expression>.locale to query the locale on which an lvalue lives
  e.g., "var x = ...;  ... x.locale ..."
- task pool to support the creation of more tasks than executing threads
  (see doc/README.threads for more details)
- added support for calling methods within class constructors
- improved support for member initializer expressions within classes
- strengthened distinction between type specifiers and constructor calls
- temporarily removed support for real(128), imag(128), complex(256)
- enumerated and opaque arrays now support iteration
- added support for tuples of sync variables
- added support for declaring enumerations within classes

Portability of code base
------------------------
- removed a number of warnings specific to particular compilers/platforms

Semantic Changes
----------------
- in a type context, (int) is no longer a 1-tuple of type int; use 1*int instead
- Chapel now parses types in the same way that it parses general expressions

Syntactic/Naming Changes
------------------------
- renamed opaque domain's .new() method to .create()
- renamed the (undocumented) Locales domain from LocalesDomain to LocaleSpace

Documentation
-------------
- new README files in the doc/ directory
  - README.multilocale file explains how to execute using multiple locales
  - README.threads explains how tasks are implemented using threads
- language specification revisions
  - greatly improved the "Parallelism and Synchronization" chapter
  - greatly improved the "Locality and Distribution" chapter
  - made other updates to reflect other changes mentioned in this file
  - removed most of the language overview section, believing it needs updating
- updated the quick reference document to include locality concepts
- updated HPCC documents to reflect "new" keyword and enumerated type qualifiers
- generally refreshed the README, CHANGES, STATUS files

Example Codes
-------------
- added new example codes
  - multilocale.chpl to serve as a multi-locale execution tutorial
  - taskParallel.chpl to serve as an introduction to task-parallel concepts
  - varargs.chpl to demonstrate the user of variable argument functions
- updated existing examples to reflect language changes
  - added "new" keyword to tests that create class instances
  - for tests that use files, changed access mode from string to enum
  - added enumeration type qualifiers to uses of enumerated types
  - changed opaque.chpl test to rename the opaque domain .create() method
- changed blockLU's input data to improve stability across compilers
- made automated testing of opaque.chpl deterministic

Standard Modules
----------------
- added a skipToNth() method to the RandomStream class

Compiler Flags
--------------
- changed flags to have a --no-... variation (see chpl man page for details)
  --local => --[no-]local where the default is based on CHPL_COMM
  --serial => --[no-]serial where the default is based on CHPL_THREADS
- added new flags (see chpl man page for details)
  --[no-]ieee-float controls the strictness of IEEE floating point operations
  --fast flag turns on optimizations & disables runtime checks and IEEE floats
  --explain-instantiation describes the instantiations of functions/types

Generated Code Flags
--------------------
- added a config const "maxThreads" to limit the number of threads per locale
  (see doc/README.threads for more information)
- made --help flag list built-in config consts/vars (numLocales, maxThreads)
- added a --quiet/-q flag to suppress runtime warnings

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- fixed support for arrays of arrays and arrays of syncs within classes
- fixed several bugs in the implementation of associative/opaque domains/arrays
- improved composability of forall expressions and coforall statements
- improved checks preventing the use of types in value contexts and vice-versa
- added checks for illegal control flow within parallel/sync/on statements
- added warning if locale is applied to an expression known to be local
- added a runtime warning when using distributions on more than 1 locale
- removed incorrect copies that were inserted for certain default argument types
- fixed ability to specify C source files with paths
- fixed a bug in which standalone reads of sync vars did not consume full/empty
  e.g., "myflag$;" is now equivalent to "myflag$.readFE();"
- fixed a bug in the implementation of .length for degenerate ranges
- ensured user cannot create new locales
- prevented user from being able to modify a locale's id
- removed instantiation limit error messages generated by tuples
- fixed a bug with the --cpp-line flag for long generated C lines

Error Message Improvements
--------------------------
- improved error messages for generic type specifiers

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
(no significant ones in this release)

Compiler Performance and Cleanup
--------------------------------
- reduced memory footprint of certain compiler IR classes

Generated Code
--------------
- changed implementation of subclasses
- added support for remote references and remote classes
- improved line number reporting for certain functions
- renamed some identifiers to avoid initial underscores -- more remain
- improved name mangling policy
- changed integer type aliases to use stdint.h types
- turned up C compilation warning levels for developers

Testing System
--------------
- added support for listing any number of tests and directories on command-line
- removed -onetest and -startdir flags due to above support
- allowed start_test to be run from any directory
- removed support for Makefile-driven testing
- removed start_clean script
- added "skipif" support for skipping a test or directory based on env vars

Internal
--------
- unified implementation of cobegin/coforall to use begins
- use endCounts to implement cobegins/coforalls rather than syncStacks
- improved runtime TAGS file and added TAGS file for generated code
- improved unification/normalization of AST classes and primitives
- removed unnecessary primitives, types, symbols
- changed places in code where pragmas can occur
- converted from CVS to SVN
- made --ccwarnings into --[no-]ccwarnings where the default is based on --devel



version 0.6
===========

Third preliminary release of Chapel Compiler, December 21, 2007

High-Level Themes
-----------------
- single-locale, multiple threads
- improved stability of core features
- support for additional serial features
- improved single-threaded performance and memory utilization of generated code
- improved multi-threaded correctness, features
- improved compiler performance and memory utilization
- improved portability of compiler/runtime code and generated code
- support for cross-compilation environments
- target audience: HPCS mission partners, select HPC users and centers

Changes to Chapel Language
--------------------------
- added support for defining type functions
  e.g., def myint(param x: bool) type { return if x then int(32) else int(64);}
- changed routines supported on sync/single variables and made them methods
  e.g., var x: sync int;  x.writeXF(2);  if (x.isFull) { ... }
- added support for expression-level for and forall loops using keyword syntax
  e.g., var A: [D] int = for i in D do foo(i);
- added support for readln() function which skips to EOL after reading its args
- added support for a main() function per module, resolvable via --main-module
- added support for translate, interior, exterior, and expand methods on ranges
- added support for membership test on ranges
  e.g., var r: range = ...;  if (r.member(3)) { ... }
- added support for range/scalar arithmetic using +, -, *, /
  e.g., (1..10) + 1 == 2..11; (1..10) * 2 == 2..20 by 2
- removed support for operations between scalars and tuples
  e.g., (1, 2) + 3 is no longer supported; (1, 2) + (3, 4) still is
- added support for bitwise operators on int/uint pairs of the same size
  e.g., int(32) & uint(32) => uint(32)

Newly Implemented Features
--------------------------
- added support for arrays of arrays whose inner arrays are of uniform size
- added support for arrays of domains
- added support for instantiating types/functions using array types
  e.g., class C { type t; ... }   type vect = [1..n] int;   var myC: C(vect);
- implemented a reshape function for arrays
- initial implementation of opaque domains and arrays
- added support for slicing using unbounded and degenerate ranges
  e.g., for A: [1..n, 1..n] real, A[2.., ..] is equivalent to A[2..n, 1..n]
- added support for rank change slicing of domains and arrays
  e.g., var V: [1..n] int;  V = A[n/2 + 1, 1..n];
- relaxed requirements that slicer and slicee objects use the same integer type
- added support for | and & reductions over boolean aggregates
- added support for a clear() method on associative domains
- preliminary support for "on" statements with restricted functionality
- improved support for user-defined class constructors
- added support for runtime checks against nil class references
  e.g., class C { ... }  var myC: C;   C.foo();  generates a nice error
- improved support for nested modules
- improved symbol resolution for external modules
- implicit enum-to-string coercions and explicit string-to-enum conversions
- initial support for controlling type-to-string formatting (doc/README.format)
- initial support for prototyping and calling extern C functions from Chapel
  (see doc/README.extern for more details)
- added support for generic function arguments of sync types
  e.g., def foo(x: sync) { ... }
- improved support for garbage collection in single-threaded codes (see STATUS)
- improved tuple type orthogonality; they can now be used in most type contexts
- added support for strided param for loops

Portability of code base
------------------------
- added support for compiling with Intel, PGI, Pathscale, and Cray compilers
- added initial support for compiling for Cray MTA/XMT, X1s, and XTs
- improved portability of source and generated codes across compilers/platforms
- refactored Makefiles to better separate compiler settings from platforms
- added support for cross-compilation environments -- see doc/README.chplenv
- added mechanisms for selecting threading/communication libraries -- see "
- added native support for task parallelism on Cray MTA/XMT platforms

Semantic Changes
----------------
- removed implicit coercions from integer types to real(32)
- made queries of an array's domain via A.domain or A:[?D] constant
- removed dynamic dispatch on method calls with no parenthesis
- user-defined constructors now squelch the creation of a default constructor
- redefined range and domain slicing to use intersection semantics
- changed location of instantiated generics to be their definition point
- defined semantics of default functions as being "return a const"
- function resolution now favors non-promoted functions as a tie-breaker
- better defined casts between complex, imaginary, real, and integral types

Syntactic/Naming Changes
------------------------
- added support for $ in identifier names; used by convention to identify syncs
- allow floating point literals with an exponent to use an integral base
  e.g., can now write 1e4 rather than 1.0e4
- changed locale array name from Locale to Locales (as described in spec)
- added ability to tag a function as returning a const value for documentation

Documentation
-------------
- updates to the language specification
- updated CHANGES and STATUS files
- refreshed READMEs
- updated compiler man page and --help message
- added AGREEMENT file listing user agreement
- refreshed and refactored CONTRIBUTORS file

Example Codes
-------------
- added tutorial example for using arithmetic arrays (arrays.chpl)
- added tutorial example for using opaque arrays (opaque.chpl)
- converted all HPCC example codes to use 64-bit indices for their domains
- fixed a bug in the HPCC FFT calculation of GFlop/s
- renamed sync vars to use $ naming convention

Standard Modules
----------------
- added support for 32-bit math routines

Compiler Flags
--------------
- added new flags:
  --help-env : prints environment variables for each compiler flag
  --help-settings : prints current setting of each compiler flag
  --no-nil-checks : disables runtime checks for nil class references
  --no-checks : disables all runtime checks (currently nil & bounds checks)
  --main-module : selects between multiple possible program entry points
  --ldflags : supplies flags for the C link step
  --serial : disables all parallelism in a program
  --local : optimizes compilation for single-locale execution
  --no-scalar-replacement : disables the scalar replacement optimization
  --short-names : generates the C code using short variable names (T<#>)
  --make : specifies the GNU-compatible make utility used by the back-end
- removed flags:
  --parallel : became the default and was redundant with --serial
- added ability to specify .c, .o, and .h files for interfacing to C routines
- improved error-reporting for incorrect compiler flags
- compiler now parses all flags before any significant ones are acted upon
- better error checking for parsing config param values

Generated Code Flags
--------------------
- added new flags:
  -nl #/--numLocales=# flags for specifying the number of locales to use
  -v/--verbose flags for increasing the verbosity of the generated code
- improved error-reporting for incorrect flags
- improved parsing of config const/var values

Bug Fixes/New Semantic Checks (for old semantics)
-------------------------------------------------
- whole-array assignment of arrays of sync variables now works correctly
- += and -= are now supported on sync variables
- fixed bugs in sync variable read/write functions
- added checks for blocking sync var reads/writes when $CHPL_THREADS == none
- fixed associative domain/array bugs caused by removing indices
- added bounds checking for associative domain/array operations
- improved bounds checking for array slicing
- bounds checking for string.substring() arguments
- improved legality checks for string-to-value conversions
- added legal-value checking for casts from integers to enumerated types
- improved robustness of tensor function promotion
  e.g., exp[A, B] where A and B are arrays
- improved checks preventing against assignment to const variables
- added check that actual arguments to inout or out formals are valid lvalues
- added check against returning constants from var functions
- added checks against assigning to consts and params
- fixed use of inout and out intents with variable-length argument lists
- default argument values specified using a function call now work properly
  e.g., def foo(x = bar()) ...
- improved handling of unifying multiple returns from a single function
- improved checking that a function returns a value along all control paths
- return statements are now supported in type select statements
- type checking of LHS and RHS on op= assignments
- added ability for user to overload operators for uint(64)/int combinations
- improved error checks for the use of values in a type context and vice-versa
- support for overriding this() method in subclasses
- improved error checks for legal expressions to iterate over
- added semantic check for degenerate rank-change slicing

Error Message Improvements
--------------------------
- added filename/line# information to many error messages
- improved notation for types in error messages from ": t" to "type t"
- added warning when C-style ++ or -- are used; these are duplicated unary ops
- improved clarity of function prototypes in function resolution errors
- improved clarity of error for creating a domain with mixed integral types

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- improved performance for codes using type "complex"
- removed extraneous threads created by uses of the begin statement
- improved the performance of the bitMatMultOr routine in the BitOps module
- added strength reduction for ** with an exponent of 0-6 or 8
- added strength reduction for certain comparisons between uints and 0

Compiler Performance and Cleanup
--------------------------------
- improved performance of scalar replacement, copy propagation passes
- improved performance of compiling codes using type "complex"
- reduced memory requirements of compiler

Generated Code
--------------
- added support for generating code using short variable names (--short-names)

Testing System
--------------
- added support for Makefile-driven testing within a subdirectory
- added support for testing on multiple locales
- added support for "compilation only" testing to avoid running executables
- added support for the ability to specify a launch command to begin execution
- added the ability to suppress tests that are expected to fail
- added support for communication-layer-specific .good files

Internal
--------
- support for array type descriptor at runtime
- many refactorings and improvements to the compiler and runtime sources



version 0.5
===========
Second preliminary release of Chapel Compiler, June 29, 2007

High-Level Themes
-----------------
- single-locale, multiple threads
- support for additional serial features
- improved single-threaded performance, memory utilization
- improved multi-threaded correctness, features
- target audience: HPLS evaluation team, HPCS mission partners,
  select HPC users and centers

Changes to Chapel Language
--------------------------
- eliminated Chapel's sequence type
  - in the future, a standard List library will provide similar functionality
  - removed # and #= operators, since they were designed for sequences
  - removed support for sequence literals; use tuple syntax instead
    e.g., var A: [1..3] real = (0.1, 2.3, 4.5);
  - removed support for casting sequences to tuples and tuples to sequences
  - iterators in an expression context now result in a 1D, 1-based array
    e.g., "iterator foo() {...}  var A = foo();" creates a 1D array over [1..]
- added "range" type to language to replace old "arithmetic sequence" concept
  e.g., "lo..hi" or "lo..hi by str" are ranges
  - open interval syntax supported for ranges
    e.g., "[lo..hi)" is equivalent to lo..hi-1
  - +, -, *, / operators supported for range/scalar combinations
- added coforall loop construct for expressing explicit task parallelism
  e.g., "coforall 1..4 do ..." will create four explicit tasks
- removed "iterator" keyword -- iterators are now functions that contain yields
- added support for defining a default ("these") iterator for classes/records
- added support for creating array aliases
  e.g., "var Amid => A[2..n-1, 2..n-1];" creates an alias, Amid, to the slice
- support for standard math, bitwise, and comparison operators for tuples
  e.g., "(i,j) + (1,-1)" yields "(i+1, j-1)"
- added a swap operator: <=>
  e.g., "a <=> b" is semantically equivalent to "const t = b; b = a; a = t;"
- better support for writing output
  - enabled support for writing to strings using a write() method
    e.g., var s: string;  s.write("x is: ", x);
  - added a Writer class that enables user classes to be written to
  - added writeThis() method to specify how a type should be output to a Writer
- added minloc and maxloc reductions that return min/max value and index
  e.g., var (minVal, minInd) = minloc reduce (A, A.domain);
- changed arithmetic domain range access from D(<dim#>) to D.dim(<dim#>)
  e.g., var rows = D.dim(1);
- arithmetic domains and arrays support low and high methods that return bounds
  e.g., var loInd = A.low;
- support for range, domain, and array slicing via bounded or unbounded ranges
  e.g., "A[2.., ..n]" is equivalent to "A[2..A.dim(1).high, A.dim(2).low..n]"
- support for promoting casts across array elements
  e.g., var x: [1..4] real, y: [1..4] int;  y = x:int;
- added support for param functions that support compile-time evaluation
  e.g., "def square(param x) param { return x**2; }  param n2 = square(14);"
- support for default values for param members of classes
  e.g., class C { param bitWidth = 32; ... }
- support for top-level 0-argument functions without parentheses
  e.g., def f { writeln("In function f"); }
  - allow "main" function to be declared with or without parentheses
    e.g., def main { writeln("Program will start here"); ... }
- allow "<index> in" to be elided in for/forall loops/expressions
  e.g., "for 1..n { ... }", "var A = [1..n] 0.0;"
- support for multiple modules to be listed within a single "use" statement
- added support for reading a value of a specified type from a file
  e.g., "var myint = infile.read(int);"

Newly Implemented Features
--------------------------
- support for sparse domains/arrays
  - assignment of aggregate index expressions to sparse domains
  - accumulation of indices into sparse domains using +=
  - re-assignment of a sparse array's "zero" value using the .IRV field
  - clear() method for emptying a sparse domain's index set
- arrays of sync variables are now supported
- added support for partially and fully unbounded ranges ("lo..", "..hi", "..")
- enumerated domains and arrays
- support for a numCores() function on the locale type to query # of cores
- subBlocks iterator on domains now generates a sub-block per processor core
- support for nested classes, records, and iterators
- serial statement
- added support for using "_" to ignore elements during tuple destructuring
  e.g., var t: (int, real) = ...;   (t1, _) = t;
- added support for nested tuple destructuring in variable decls/formal args
- support for config params of primitive and enumerated types
- initial support for accessing a module's members via modname.membername
- implemented dynamic casts between class types
- added initial support for copy collection-based garbage collection
- subdomain keyword is now supported, though subset relationship is not checked
- initial support for specifying a distribution class; interface still in flux

Syntactic/Naming Changes
------------------------
- removed support for using "?" in identifier names
  - changed standard method member? to member
  - changed standard method true? to isTrue
- removed support for "--" as a single-line comment style;  use "//"
- an array's domain should now be accessed via A.domain rather than A.dom
- array element type identifier changed from elt_type to eltType
- made file open/close methods take parenthesis
- renamed "indefinite domains/arrays" to "associative domains/arrays"

Semantic Changes
----------------
- changed relative precedence of casts and unary +/- (unary +/- is now tighter)
  e.g., -2:uint(32) is equivalent to (-2):uint(32) rather than -(2:uint(32))
- for/forall loops over arrays now result in references to elements, not copies
  e.g., "for e in A do e = 0;" could be used to zero out an array
- enumerated types now start counting at "1" rather than "0" by default
- conditional expressions are no longer valid l-values
- improved semantics of module use transitivity, nested modules
- removed support for structural iterator interface (getHeadCursor, etc.)
- nested classes can refer to their outer class using the identifier "outer"
- initial support for querying parameters/types in a formal class argument
  - similar support for variable-length argument lists of generics
- where clauses can now compare against formal generic types
- short-circuiting operators only require isTrue rather than true? & false?
- in x op=y, a cast to x.type is used in assigning back to x if it is primitive
- compilerError() can now take multiple arguments like a halt()
- replaced setter/getter functions with richer support for var functions

Documentation
-------------
- added man page for Chapel compiler (chpl)
- added CHANGES file
- added Quick Reference document
- added language overview chapter to the specification
- updated HPCC tutorial document and broke into two docs: overview and tutorial
- updated STATUS file
- cleaned up README files a bit

Example Codes
-------------
- added README.features file to map Chapel features to examples that use them
- several improvements to the HPCC benchmarks due to bug fixes, new features
- added several new examples:
  - iterators example to demonstrate use of iterators
  - linkedList example for nested, generic, iteratable, printable classes
  - parallelQuickSort example to demonstrate serial/cobegin statements
  - blockLU example to demonstrate domain slicing, array aliases, swap
  - sparse example to demonstrate sparse domains and arrays
  - slices example to demonstrate array slicing and reindexing
  - reductions example to demonstrate minloc/maxloc reductions
  - fileIO example to demonstrate simple use of file I/O
- rewrote the producer-consumer example to use a buffer of sync variables
- added a hello-module example to demonstrate simple Chapel program structure
- improved the comments for several tests
- removed stream-nopromote.chpl example, which had become redundant
- combined the two genericStack examples into a single file
- added performance testing support for stream.chpl and ra.chpl

Standard Modules
----------------
- Search: new standard module
- Sort: new standard module
- Types: numBits() and numBytes() are now param functions
- Random: fillRandom() routines now work for arrays of arbitrary rank

Compiler Flags
--------------
- improved the formatting of the --help message
- added -g flag for debugging generated code; turns on --cpp-lines by default
- added -O flag for optimizing generated code
- added -s/--set for setting config params
- added --explain-call flag that helps describe how a function call is resolved
- added --baseline flag to disable all compiler optimizations
- added --no-flow-analysis flag that disables all flow-based optimizations
- added --no-warnings flag to disable all compiler warnings
- added --no-single-loop-iterator-opt flag to disable 1-loop iterator rewrites
- added --no-expand-iterators-inline-opt flag to disable iterator inlining
- removed --unoptimized flag
- removed --cg-chpl-lineno flag; this capability is now always on
- removed --no-gc used for a stopgap reference counting scheme

Generated Code Flags
--------------------
-- added a --gdb flag to make it easier to debug generated code

Bug Fixes/New Semantic Checks (for old Semantics)
-------------------------------------------------
- test expressions in loops/conditionals restricted to bools, classes, integers
- runtime checks added to ensure that zippered iterations are of equal size
- fixed portability issues in flex lexer file, chapel.lex
- domains are now passed by reference by default
- implemented bytesPerLocale for Macintosh
- classes can now have iterator methods
- recursive iterators now work
- "true + true" now yields "2" rather than "true"
- casts from uints to complexes now supported
- casts from integral types to enumerated types now supported
- initial support for casts from strings to primitive types
- modules can now refer to param values and types from other modules correctly
- disabled parameter folding of floating point values
- parameter folding of relational operators on enum types
- parameter folding of short-circuiting &&, ||, and conditional expressions
- type aliases can now be created for array types
- fixed numerical bugs in array slicing and reindexing
- config variable initializers are no longer evaluated if set on command-line
- const values can no longer be passed to arguments with inout or out intent
- constructor keyword no longer supported -- use def <classname> instead
- fixed "bug" with log2 on integers and made it halt on zero or negatives
- improved thread safety of generated code
- made creation of too many threads result in halt() rather than lost work
  - eventually we'll need to queue up tasks; this is a stopgap
- passing unions to out/inout intents now works
- better semantic checking of legal l-values
- fixed an infinite loop bug in associative domains/arrays

Error Message Improvements
--------------------------
- added message to compiler warning that "atomic" is unimplemented when used
- caused compiler to complain about non-module scope configuration variables
- unified runtime and compiler error message formats
- runtime errors identify source file/line#
- improved quality of compilerError() error message output
- cleaned up syntax error messages
- improved error message for tuples of non-param size
- improved error reporting when attempting to assign to constants
- better error checking of legal vs. illegal casts
- generally improved quality of compiler error messages

Compiler Analysis and Optimizations/Performance of Generated Code
-----------------------------------------------------------------
- removed the insertion of all large sequence/array temporaries
- extensive optimizations of iterators and loop generation
- global/local copy propagation
- dead variable elimination
- dead code elimination
- live variable analysis
- reaching definitions analysis
- def-use and use-def chains

Compiler Performance and Cleanup
--------------------------------
- improvements to function resolution, vararg function instantiation
- made compiler build with debugging off/optimizations on for non-developers
- removed unused scalar replacement code
- renamed and refactored internal modules
- removed old reference counting code
- optimized several passes to improve compile time

Generated Code
--------------
- now generating structured loops rather than gotos and labels
- large reduction in lines of generated code
  e.g., ~40% reduction in lines of code for jacobi example (13275 -> 7753)
- module functions generated in order, with comments to identify source line #
- _chpl_header.h file is generated in a cleaner manner
- changed munging of variable names to improve gdb support and readability
- using pragmas, generated code has been shown to vectorize on X1/thread on MTA
- initial support for cross-compiling to the XT

Testing System
--------------
- added support for doing performance-tracking experiments

Internal
--------
- new IR traversal mechanisms
- flow analysis optimization framework
- vararg functions handled within function resolution
- parameter folding folded into function resolution
- new compiler gdb commands: lv (list view)
- domain and array wrappers now implemented as records
- improved creation of init functions
- removed ability to cast primitives to strings by casting to "%<format>"
- new mechanism for indicating that a class is promotable
- added AND_EXPAND and PRIMITIVE_TUPLE_AND_EXPAND
- added reference types



version 0.4
===========
Preliminary release of Chapel Compiler, December 15, 2006

High-Level Themes
----------------
- single-locale, multiple threads
- support for base language features
- designed for correctness over performance
- target audience: HPLS language evaluation team and HPCS mission partners
