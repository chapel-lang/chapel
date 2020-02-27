.. _man-chpl:

chpl
====

.. confchpl.rst

SYNOPSIS
--------

|   **chpl** [**-O**] [**--no-checks**] [**--fast**]
|            [**-g**] [**--savec** *directory*]
|            [**-M** *directory*...] [**--main-module** *mod*]
|            [**-o** *outfile*] [*options*] source-files...
|

DESCRIPTION
-----------

The **chpl** command invokes the Chapel compiler. **chpl** converts one
or more Chapel source files into an executable. It does this by
compiling Chapel code to C99 code and then invoking the target
platform's C compiler to create the executable. However, most users will
not need to be aware of the use of C as an intermediate format during
compilation.

SOURCE FILES
------------

Chapel recognizes four source file types: **.chpl**, .c, .h, and .o.

**foo.chpl**

    Chapel sources are compiled by the Chapel compiler into C intermediate
    code, which is then passed to the target compiler to be compiled into
    object code.

**foo.c**

    C source files are passed directly to the target C compiler.

**foo.h**

    C header files are included in the generated C code.

**foo.o**

    Object files are passed directly to the target linker.

OPTIONS
-------

*Module Processing Options*

**--[no-]count-tokens**

    Prints the total number of static lexical tokens in the Chapel code
    files named on the command line.

**--main-module <module>**

    For programs that supply multiple possible entry points (main()
    functions or module initializers that can serve as an entry point), this
    option can be used to specify which module should serve as the starting
    point for program execution.

**-M, --module-dir <**\ *directory*\ **>**

    Add the specified *directory* to the module search path. The module
    search path is used to satisfy module 'use' statements. In the current
    implementation, the compiler tries to locate unresolved modules by
    searching for a filename whose name matches that of the module. For
    example, if the user program contains 'use foo' and the .\ **chpl**
    files listed by the programmer on the compiler's command line do not
    define a module named 'foo', the compiler will search for files named
    'foo.chpl' in the module search path. The complete path that will be
    searched can be displayed using the **--print-search-dirs** flag and is
    composed of (1) the directories containing the .\ **chpl** files that
    were specified on the compiler command-line (in left-to-right order),
    (2) all directories specified by **-M** flags (in left-to-right order),
    (3) all directories specified by the $CHPL\_MODULE\_PATH environment
    variable (colon-separated directories), (4) the compiler's standard
    module search path.

**--[no-]print-code-size**

    Prints out the size of the Chapel code files named on the command line
    in great detail: For each code file, first the code is echoed back to
    the screen, prefixing each line with the number of lexical tokens it
    contains, or \*C\* if the line only contains comments, or \*B\* if the
    line is blank. Next, the total number of tokens for the file is printed.
    Then the number of lines is displayed, broken down into code lines,
    comment-only lines, and blank lines. Then the maximum and average number
    of tokens per line is displayed. Finally, a histogram of the number of
    tokens per line is shown. After this information is printed for each
    file, a grand total of the number of tokens across all the files is
    displayed.

**--print-module-files**

    Prints the Chapel module source files parsed by the Chapel compiler.

**--[no-]print-search-dirs**

    Print the module search path used to resolve module for further details.

*Warning and Language Control Options*

**--[no-]permit-unhandled-module-errors**

    Normally, the compiler ensures that all errors are handled for code
    inside of a module declaration (unless the module overrides that
    behavior). This flag overrides this default, so that the compiler
    will compile code in a module that does not handle its errors. If any
    error comes up during execution, it will cause the program to halt.

**--[no-]warn-unstable**

    Enable [disable] warnings for code that has recently or will recently
    change in meaning due to language changes.

**--[no-]warnings**

    Enable [disable] the printing of compiler warnings. Defaults to printing
    warnings.

*Parallelism Control Options*

**--[no-]local**

    Compile code for single/[multi-] *locale* execution, changing *on
    blocks* to normal blocks, evaluating the *locale* expression for side
    effects, and optimizing away all remote references in the code. When
    $CHPL\_COMM is set to "none", **--local** is the default; otherwise
    **--no-local** is the default.

*Optimization Control Options*

**--baseline**

    Turns off all optimizations in the Chapel compiler and generates naive C
    code with many temporaries.

**--[no-]cache-remote**

    Enables the cache for remote data. This cache can improve communication
    performance for some programs by adding aggregation, write behind, and
    read ahead. This cache is not enabled by any other optimization
    *options* such as **--fast**.

**--[no-]copy-propagation**

    Enable [disable] copy propagation.

**--[no-]dead-code-elimination**

    Enable [disable] dead code elimination.

**--fast**

    Turns off all runtime checks using **--no-checks**, turns on **-O** and
    **--specialize**.

**--[no-]fast-followers**

    Enable [disable] the fast follower optimization in which fast
    implementations of followers will be invoked for specific leaders.

**--[no-]ieee-float**

    Disable [enable] optimizations that may affect IEEE floating point
    conformance. The default is whatever level of optimization/IEEE floating
    point support your C compiler provides at the optimization level
    provided by '\ **chpl**\ '.

**--[no-]loop-invariant-code-motion**

    Enable [disable] the optimization that moves loop invariant code from
    loop runs into the loop's "pre-header." By default invariant code is
    moved. This is currently a rather conservative pass in the sense that it
    may not identify all code that is truly invariant.

**--[no-]optimize-forall-unordered-ops**

    Enable [disable] optimization of the last statement in forall statements
    to use unordered communication. This optimization works with runtime
    support for unordered operations with CHPL_COMM=ugni.

**--[no-]ignore-local-classes**

    Disable [enable] local classes

**--[no-]inline**

    Enable [disable] function inlining.

**--[no-]inline-iterators**

    Enable [disable] iterator inlining. When possible, the compiler
    optimizes the invocation of an iterator in a loop header by inlining the
    iterator's definition around the loop body.

**--inline-iterators-yield-limit**

    Limit on the number of yield statements permitted in an inlined iterator.
    The default value is 10.

**--[no-]live-analysis**

    Enable [disable] live variable analysis, which is currently only used to
    optimize iterators that are not inlined.

**--[no-]optimize-range-iteration**

    Enable [disable] anonymous range iteration optimizations. This allows the
    compiler to avoid creating ranges when they are only used for iteration.
    By default this is enabled.

**--[no-]optimize-loop-iterators**

    Enable [disable] optimizations to aggressively optimize iterators that
    are defined in terms of a single loop. By default this is enabled.

**--[no-]vectorize**

    Enable [disable] generating vectorization hints for the target compiler.
    If enabled, hints will always be generated, but the effects on performance
    (and in some cases correctness) will vary based on the target compiler.

**--[no-]optimize-on-clauses**

    Enable [disable] optimization of on clauses in which qualifying on
    statements may be optimized in the runtime if supported by the
    $CHPL\_COMM layer.

**--optimize-on-clause-limit**

    Limit on the function call depth to allow for on clause optimization.
    The default value is 20.

**--[no-]privatization**

    Enable [disable] privatization of distributed arrays and domains if the
    distribution supports it.

**--[no-]remove-copy-calls**

    Enable [disable] removal of copy calls (including calls to what amounts
    to a copy constructor for records) that ensure Chapel semantics but
    which can often be optimized away.

**--[no-]remote-value-forwarding**

    Enable [disable] remote value forwarding of read-only values to remote
    threads if reading them early does not violate program semantics.

**--[no-]remote-serialization**

    Enable [disable] serialization for globals and remote constants.

**--[no-]scalar-replacement**

    Enable [disable] scalar replacement of records and classes for some
    compiler-generated data structures that support language features such
    as tuples and iterators.

**--scalar-replace-limit**

    Limit on the size of tuples being replaced during scalar replacement.
    The default value is 8.

**--[no-]tuple-copy-opt**

    Enable [disable] the tuple copy optimization in which whole tuple copies
    of homogeneous tuples are replaced with explicit assignment of each
    tuple component.

**--tuple-copy-limit**

    Limit on the size of tuples considered for the tuple copy optimization.
    The default value is 8.

**--[no-]use-noinit**

    Enable [disable] ability to skip default initialization through the
    keyword noinit

**--[no-]infer-local-fields**

    Enable [disable] analysis to infer local fields in classes and records
    (experimental)

*Run-time Semantic Check Options* 

**--no-checks**

    Turns off all of the run-time checks in this section of the man page.
    Currently, it is typically necessary to use this flag (or **--fast**,
    which implies **--no-checks**) to achieve performance competitive with
    hand-coded C or Fortran.

**--[no-]bounds-checks**

    Enable [disable] run-time bounds checking, e.g. during slicing and array
    indexing.

**--[no-]cast-checks**

    Enable [disable] run-time checks in safeCast calls for casts that
    wouldn't preserve the logical value being cast.

**--[no-]div-by-zero-checks**

    Enable [disable] run-time checks in integer division and modulus operations
    to guard against dividing by zero.

**--[no-]formal-domain-checks**

    Enable [disable] run-time checks to ensure that an actual array
    argument's domain matches its formal array argument's domain in terms of
    (a) describing the same index set and (b) having an equivalent domain
    map (if the formal domain explicitly specifies a domain map).

**--[no-]local-checks**

    Enable [disable] run-time checking of the locality of references within
    local blocks.

**--[no-]nil-checks**

    Enable [disable] run-time checking for accessing nil object references.

**--[no-]stack-checks**

    Enable [disable] run-time checking for stack overflow.

*C Code Generation Options* 

**--[no-]codegen**

    Enable [disable] generating C code and the binary executable. Disabling
    code generation is useful to reduce compilation time, for example, when
    only Chapel compiler warnings/errors are of interest.

**--[no-]cpp-lines**

    Causes the compiler to emit cpp #line directives into the generated code
    in order to help map generated C code back to the Chapel source code
    that it implements. The [no-] version of this flag turns this feature
    off.

**--max-c-ident-len**

    Limits the length of identifiers in the generated code, except when set
    to 0. The default is 0, except when $CHPL\_TARGET\_COMPILER indicates a
    PGI compiler (pgi or cray-prgenv-pgi), in which case the default is
    1020.

**--[no-]munge-user-idents**

    By default, **chpl** munges all user identifiers in the generated C code
    in order to minimize the chances of conflict with an identifier or
    keyword in C (in the current implementation, this is done by appending
    '\_chpl' to the identifier). This flag provides the ability to disable
    this munging. Note that whichever mode, the flag is in, **chpl** may
    perform additional munging in order to implement Chapel semantics in C,
    or for other reasons.

**--savec <dir>**

    Saves the compiler-generated C code in the specified *directory*,
    creating the *directory* if it does not already exist. This option may
    overwrite existing files in the *directory*.

*C Code Compilation Options*

**--ccflags <flags>**

    Add the specified flags to the C compiler command line when compiling
    the generated code. Multiple **--ccflags** *options* can be provided and
    in that case the combination of the flags will be forwarded to the C
    compiler.

**-g, --[no-]debug**

    Causes the generated C code to be compiled with debugging turned on. If
    you are trying to debug a Chapel program, this flag is virtually
    essential along with the **--savec** flag. This flag also turns on the
    **--cpp-lines** option unless compiling as a developer (for example, via
    **--devel**).

**--dynamic**

    Use dynamic linking when generating the final binary. If neither
    **--dynamic** or **--static** are specified, use the backend compiler's
    default.

**-I, --hdr-search-path <dir>**

    Add the specified dir[ectories] to the back-end C compiler's
    search path for header files along with any directories in the
    CHPL\_INCLUDE\_PATH environment variable.  Both the environment
    variable and this flag accept a colon-separated list of
    directories.

**--ldflags <flags>**

    Add the specified flags to the back-end C compiler link line when
    linking the generated code. Multiple **--ldflags** *options* can
    be provided and in that case the combination of the flags will be
    forwarded to the C compiler.

**-l, --lib-linkage <library>**

    Specify a C library to link to on the back-end C compiler command
    line.

**-L, --lib-search-path <dir>**

    Add the specified dir[ectories] to the back-end C compiler's
    search path for libraries along with any directories in the
    CHPL\_LIB\_PATH environment variable.  Both the environment
    variable and this flag accept a colon-separated list of
    directories.

**-O, --[no-]optimize**

    Causes the generated C code to be compiled with [without] optimizations
    turned on. The specific set of flags used by this option is
    platform-dependent; use the **--print-commands** option to view the C
    compiler command used. If you would like additional flags to be used
    with the C compiler command, use the **--ccflags** option.

**--[no-]specialize**

    Causes the generated C code to be compiled with flags that specialize
    the executable to the architecture that is defined by
    CHPL\_TARGET\_CPU. The effects of this flag will vary based on choice
    of back-end compiler and the value of CHPL\_TARGET\_CPU.

**-o, --output <filename>**

    Specify the name of the compiler-generated executable. Defaults to
    the filename of the main module (minus its `.chpl` extension), if
    unspecified.

**--static**

    Use static linking when generating the final binary. If neither
    **--static** or **--dynamic** are specified, use the backend compiler's
    default.

*LLVM Code Generation Options*

**--[no-]llvm**

    Use LLVM as the code generation target rather than C. See
    $CHPL\_HOME/doc/rst/technotes/llvm.rst for details.

**--[no-]llvm-wide-opt**

    Enable [disable] LLVM wide pointer communication optimizations. This
    option requires **--llvm** and packed wide pointers. Packed wide
    pointers are enabled by setting CHPL\_WIDE\_POINTERS = node16. You must
    also supply **--fast** to enable wide pointer optimizations. This flag
    allows existing LLVM optimizations to work with wide pointers - for
    example, they might be able to hoist a 'get' out of a loop. See
    $CHPL\_HOME/doc/rst/technotes/llvm.rst for details.

**--mllvm <option>**

    Pass an option to the LLVM optimization and transformation passes.
    This option can be specified multiple times.


*Compilation Trace Options*

**--[no-]print-commands**

    Prints the system commands that the compiler executes in order to
    compile the Chapel program.

**--[no-]print-passes**

    Prints the compiler passes during compilation and the amount of wall
    clock time required for the pass. After compilation is complete two
    tables are produced that provide more detail of how time is spent in
    each pass (compiling, verifying, and memory management) and the
    percentage of the total time that is attributed to each pass. The first
    table is sorted by pass and the second table is sorted by the time for
    the pass in descending order.

**--print-passes-file <filename>**

    Saves the compiler passes and the amount of wall clock time required for
    the pass to <filename>. An error is displayed if the file cannot be
    opened but no recovery attempt is made.

*Miscellaneous Options*

**--[no-]devel**

    Puts the compiler into [out of] developer mode, which takes off some of
    the safety belts, changes default behaviors, and exposes additional
    undocumented command-line *options*. Use at your own risk and direct any
    questions to the Chapel team.

**--explain-call <call>[:<module>][:<line>]**

    Helps explain the function resolution process for the named function by
    printing out the visible and candidate functions. Specifying a module
    name and/or line number can focus the explanation to those calls within
    a specific module or at a particular line number.

**--explain-instantiation <function\|type>[:<module>][:<line>]**

    Lists all of the instantiations of a function or type. The location of
    one of possibly many points of instantiation is shown. Specifying a
    module name and/or line number can focus the explanation to those calls
    within a specific module or at a particular line number.

**--[no-]explain-verbose**

    In combination with explain-call or explain-instantiation, causes the
    compiler to output more debug information related to disambiguation.

**--instantiate-max <max>**

    In order to avoid infinite loops when instantiating generic functions,
    the compiler limits the number of times a single function can be
    instantiated. This flag raises that maximum in the event that a legal
    instantiation is being pruned too aggressively.

**--[no-]print-all-candidates**

    By default, function resolution errors show only a few candidates.
    Use this flag to see all of the candidates for a call that could
    not be resolved.

**--[no-]print-callgraph**

    Print a textual call graph representing the program being compiled. The
    output is in top-down and depth first order. Recursive calls are marked
    and cause the traversal to stop along the path containing the recursion.
    Only a single call to each function is displayed from within any given
    parent function.

**--[no-]print-callstack-on-error**

    Accompany certain error and warning messages with the Chapel call stack
    that the compiler was working on when it reached the error or warning
    location. This is useful when the underlying cause of the issue is in
    one of the callers.

**--[no-]print-unused-functions**

    Print the names and source locations of unused functions within the
    user program.

**-s, --set <config>[=<value>]**

    Overrides the default value of a configuration param, type, var,
    or const in the code.  If the value is omitted, it will default
    to the value `true`.

**--[no-]task-tracking**

    Enable [disable] the Chapel-implemented task tracking table that
    supports the execution-time **-b** / **-t** flags. This option is
    currently only useful when $CHPL\_TASKS is set or inferred to 'fifo' and
    adds compilation-time overhead when it will not be used, so is off by
    default.


*Compiler Configuration Options*

**--home <path>**

    Specify the location of the Chapel installation *directory*. This flag
    corresponds with and overrides the $CHPL\_HOME environment variable.

**--atomics <atomics-impl>**

    Specify the implementation to use for Chapel's atomic variables. This
    flag corresponds with and overrides the $CHPL\_ATOMICS environment
    variable. (defaults to a best guess based on $CHPL\_TARGET\_COMPILER,
    $CHPL\_TARGET\_PLATFORM, and $CHPL\_COMM)

**--network-atomics <network>**

    Specify the network atomics implementation for all atomic variables.
    This flag corresponds with and overrides the $CHPL\_NETWORK\_ATOMICS
    environment variable (defaults to best guess based on $CHPL\_COMM).

**--aux-filesys <aio-system>**

    Specify runtime support for additional file systems. This flag
    corresponds with and overrides the $CHPL\_AUX\_FILESYS environment
    variable (defaults to 'none').

**--comm <comm-impl>**

    Specify the communication implementation to use for inter-\ *locale*
    data transfers. This flag corresponds with and overrides the $CHPL\_COMM
    environment variable (defaults to 'none').

**--comm-substrate <conduit>**

    Specify the communication conduit for the communication implementation.
    This flag corresponds with and overrides the $CHPL\_COMM\_SUBSTRATE
    environment variable (defaults to best guess based on
    $CHPL\_TARGET\_PLATFORM).

**--gasnet-segment <segment>**

    Specify memory segment to use with GASNet. This flag corresponds with
    and overrides the $CHPL\_GASNET\_SEGMENT environment variable. (defaults
    to best guess based on $CHPL\_COMM\_SUBSTRATE).

**--gmp <gmp-version>**

    Specify the GMP library implementation to be used by the GMP module.
    This flag corresponds with and overrides the $CHPL\_GMP environment
    variable (defaults to best guess based on $CHPL\_TARGET\_PLATFORM and
    whether you've built the included GMP library in the third-party
    *directory*).

**--hwloc <hwloc-impl>**

    Specify whether or not to use the hwloc library. This flag corresponds
    with and overrides the $CHPL\_HWLOC environment variable (defaults to a
    best guess based on whether you've built the included library in the
    third-party hwloc *directory*).

**--launcher <launcher-system>**

    Specify the launcher, if any, used to start job execution. This flag
    corresponds with and overrides the $CHPL\_LAUNCHER environment variable
    (defaults to a best guess based on $CHPL\_COMM and
    $CHPL\_TARGET\_PLATFORM).

**--locale-model <locale-model>**

    Specify the *locale* model to use for describing your *locale*
    architecture. This flag corresponds with and overrides the
    $CHPL\_LOCALE\_MODEL environment variable (defaults to 'flat').

**--make <make utility>**

    Specify the GNU compatible make utility. This flag corresponds with and
    overrides the $CHPL\_MAKE environment variable (defaults to a best guess
    based on $CHPL\_HOST\_PLATFORM).

**--mem <mem-impl>**

    Specify the memory allocator used for dynamic memory management. This
    flag corresponds with and overrides the $CHPL\_MEM environment variable
    (defaults to a best guess based on $CHPL\_COMM).

**--regexp <regexp>**

    Specify the regular expression library to use. This flag corresponds
    with and overrides the $CHPL\_REGEXP environment variable (defaults to
    'none' or 're2' if you've installed the re2 package in the third-party
    *directory*).

**--target-arch <architecture>**

    Specify the machine type or general architecture to use.
    This flag corresponds with and overrides the $CHPL\_TARGET\_ARCH
    environment variable (defaults to the result of `uname -m`).

**--target-compiler <compiler>**

    Specify the compiler suite that should be used to build the generated C
    code for a Chapel program and the Chapel runtime. This flag corresponds
    with and overrides the $CHPL\_TARGET\_COMPILER environment variable
    (defaults to a best guess based on $CHPL\_HOST\_PLATFORM,
    $CHPL\_TARGET\_PLATFORM, and $CHPL\_HOST\_COMPILER).

**--target-cpu <cpu>**

    Specify the cpu model that the compiled executable will be
    specialized to when **--specialize** is enabled. This flag corresponds
    with and overrides the $CHPL\_TARGET\_CPU environment variable
    (defaults to a best guess based on $CHPL\_COMM, $CHPL\_TARGET\_COMPILER,
    and $CHPL\_TARGET\_PLATFORM).

**--target-platform <platform>**

    Specify the platform on which the target executable is to be run for the
    purposes of cross-compiling. This flag corresponds with and overrides
    the $CHPL\_TARGET\_PLATFORM environment variable (defaults to
    $CHPL\_HOST\_PLATFORM).

**--tasks <task-impl>**

    Specify the tasking layer to use for implementing tasks. This flag
    corresponds with and overrides the $CHPL\_TASKS environment variable
    (defaults to a best guess based on $CHPL\_TARGET\_PLATFORM).

**--timers <timer-impl>**

    Specify a timer implementation to be used by the Time module. This flag
    corresponds with and overrides the $CHPL\_TIMERS environment variable
    (defaults to 'generic').

*Compiler Information Options*

**--copyright**

    Print the compiler's copyright information.

**-h, --help**

    Print a list of the command line *options*, indicating the arguments
    that they expect and a brief summary of their purpose.

**--help-env**

    Print the command line option help message, listing the environment
    variable equivalent for each flag (see ENVIRONMENT) and its current
    value.

**--help-settings**

    Print the command line option help message, listing the current setting
    of each option as specified by environment variables and other flags on
    the command line.

**--license**

    Print the compiler's license information.

**--version**

    Print the version number of the compiler.

ENVIRONMENT
-----------

Most compiler command-line *options* have an environment variable that
can be used to specify a default value. Use the **--help-env** option to
list the environment variable equivalent for each option. Command-line
*options* will always override environment variable settings in the
event of a conflict, except for ccflags and ldflags, which stack.

If the environment variable equivalent is set to empty, it is considered
unset. This does not apply to *options* expecting a string or a path.

For *options* that can be used with or without the leading **--no**
(they are shown with "[no-]" in the help text), the environment variable
equivalent, when set to a non-empty string, has the following effect.
When the first character of the string is one of:

|

    Y y T t 1 - same as passing the option without --no,

    N n F f 0 - same as passing the option with --no,

    anything else - generates an error.

For the other *options* that enable, disable or toggle some feature, any
non-empty value of the environment variable equivalent has the same
effect as passing that option once.

BUGS
----

See $CHPL\_HOME/doc/rst/usingchapel/bugs.rst for instructions on reporting bugs.

SEE ALSO
--------

$CHPL\_HOME/doc/rst/usingchapel/QUICKSTART.rst for more information on how to
get started with Chapel.

AUTHORS
-------

See $CHPL\_HOME/CONTRIBUTORS.md for a list of contributors to Chapel.

COPYRIGHT
---------

Copyright 2004-2020 Hewlett Packard Enterprise Development LP
