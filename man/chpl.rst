.. _man-chpl:

chpl
====

.. confchpl.rst

SYNOPSIS
--------

|   **chpl** [**-O**] [**\--no-checks**] [**\--fast**]
|            [**-g**] [**\--savec** *directory*]
|            [**-M** *directory*...] [**\--main-module** *mod*]
|            [**-o** *outfile*] [*options*] source-files...
|

DESCRIPTION
-----------

The **chpl** command invokes the Chapel compiler. **chpl** converts one
or more Chapel source files into an executable. **chpl** can be configured to
produce object files using either LLVM or a C compiler. See LLVM Code Generation
Options or C Code Generation Options below.

SOURCE FILES
------------

Chapel recognizes four source file types: **.chpl**, .c, .h, and .o.

**foo.chpl**

    Chapel source file.

**foo.c**

    C source file.

**foo.h**

    C header file.

**foo.o**

    Object file.

OPTIONS
-------

*Module Processing Options*

.. _man-count-tokens:

**\--[no-]count-tokens**

    Prints the total number of static lexical tokens in the Chapel code
    files named on the command line.

.. _man-main-module:

**\--main-module <module>**

    For programs that supply multiple possible entry points (main()
    functions or module initializers that can serve as an entry point), this
    option can be used to specify which module should serve as the starting
    point for program execution.

.. _man-module-dir:

**-M, \--module-dir <**\ *directory*\ **>**

    Add the specified *directory* to the module search path. The module
    search path is used to satisfy module 'use' statements. In the current
    implementation, the compiler tries to locate unresolved modules by
    searching for a filename whose name matches that of the module. For
    example, if the user program contains 'use foo' and the .\ **chpl**
    files listed by the programmer on the compiler's command line do not
    define a module named 'foo', the compiler will search for files named
    'foo.chpl' in the module search path. The complete path that will be
    searched can be displayed using the **\--print-search-dirs** flag and is
    composed of (1) the directories containing the .\ **chpl** files that
    were specified on the compiler command-line (in left-to-right order),
    (2) all directories specified by **-M** flags (in left-to-right order),
    (3) all directories specified by the $CHPL\_MODULE\_PATH environment
    variable (colon-separated directories), (4) the compiler's standard
    module search path.

.. _man-print-code-size:

**\--[no-]print-code-size**

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

.. _man-print-module-files:

**\--print-module-files**

    Prints the Chapel module source files parsed by the Chapel compiler.

.. _man-print-search-dirs:

**\--[no-]print-search-dirs**

    Print the module search path used to resolve module for further details.

*Warning and Language Control Options*

.. _man-permit-unhandled-module-errors:

**\--[no-]permit-unhandled-module-errors**

    Normally, the compiler ensures that all errors are handled for code
    inside of a module declaration (unless the module overrides that
    behavior). This flag overrides this default, so that the compiler
    will compile code in a module that does not handle its errors. If any
    error comes up during execution, it will cause the program to halt.

.. _man-warn-unstable:

**\--[no-]warn-unstable**

    Enable [disable] warnings for code that has recently or will recently
    change in meaning due to language changes.

.. _man-warnings:

**\--[no-]warnings**

    Enable [disable] the printing of compiler warnings. Defaults to printing
    warnings.

.. _man-warn-unknown-attribute-toolname:

**\--[no-]warn-unknown-attribute-toolname**

    [Don't] warn about attribute tool names that aren't recognized. Without this
    warning, attributes belonging to unknown tools will be silently ignored.
    The default is to warn about all unknown tool names.

.. _man-using-attribute-toolname:

**\--using-attribute-toolname <**\ *toolname*\ **>**

    Provide a tool name whose use in an attribute will not trigger an
    "unknown tool name" warning. To provide multiple tool names, use one
    **\--using-attribute-toolname** flag per name.

.. _man-warn-potential-races:

**\--[no-]warn-potential-races**

    Enable [disable] the printing of compiler warnings for potential race
    conditions. For example, modifying the result of a promoted array indexing
    operation may be race condition and will warn with this flag. Defaults to
    not printing race condition warnings.

.. _man-warn-int-to-uint:

**\--[no-]warn-int-to-uint**

    Enable [disable] compilation warnings for when implicitly converting
    from a value of ``int`` type of any width to a ``uint`` value.

.. _man-warn-small-integral-to-float:

**\--[no-]warn-small-integral-to-float**

    Enable [disable] compilation warnings for when implicitly converting
    from a value of small integral type to a small floating-point value.
    More specifically, it will warn when implicitly converting something
    of type ``int(t)`` or ``uint(t)`` where ``t<64``, to something of
    type ``real(u)`` or ``complex(2*u)`` where ``u<64``.

.. _man-warn-integral-to-float:

**\--[no-]warn-integral-to-float**

    Enable [disable] compilation warnings for when implicitly converting
    from a value of ``int`` or ``uint`` type of any width to a ``real``
    or ``complex`` type of any width.

.. _man-warn-float-to-float:

**\--[no-]warn-float-to-float**

    Enable [disable] compilation warnings for when implicitly converting
    from a floating-point type of one precision to another. That includes
    implicitly converting from ``real(32)`` to ``real(64)`` as well as
    similar cases with ``imag`` and ``complex`` types.

.. _man-warn-integral-to-integral:

**\--[no-]warn-integral-to-integral**

    Enable [disable] compilation warnings for when implicitly converting
    from a value of integral type to another integral type of different width.
    (An integral type is an ``int`` or ``uint`` type).

.. _man-warn-implicit-numeric-conversions:

**\--[no-]warn-implicit-numeric-conversions**

    Enable [disable] the above compilation warnings for implicitly
    converting between numeric types.

.. _man-warn-param-implicit-numeric-conversions:

**\--[no-]warn-param-implicit-numeric-conversions**

    When used in conjunction with ``warn-int-uint``,
    ``--warn-real-real``, or ``--warn-integral-integral``, this flag
    enables [or disables] these compilation warnings about implicitly
    converting between numeric types to also apply when the converted
    value is a ``param``.

*Parallelism Control Options*

.. _man-local:

**\--[no-]local**

    Compile code for single/[multi-] *locale* execution, changing *on
    blocks* to normal blocks, evaluating the *locale* expression for side
    effects, and optimizing away all remote references in the code. When
    $CHPL\_COMM is set to "none", **\--local** is the default; otherwise
    **\--no-local** is the default.

*Optimization Control Options*

.. _man-baseline:

**\--baseline**

    Turns off all optimizations in the Chapel compiler and generates naive C
    code with many temporaries.

.. _man-cache-remote:

**\--[no-]cache-remote**

    Enables the cache for remote data. This cache can improve communication
    performance for some programs by adding aggregation, write behind, and
    read ahead.

.. _man-copy-propagation:

**\--[no-]copy-propagation**

    Enable [disable] copy propagation.

.. _man-dead-code-elimination:

**\--[no-]dead-code-elimination**

    Enable [disable] dead code elimination.

.. _man-fast:

**\--fast**

    Turns off all runtime checks using **\--no-checks**, turns on **-O** and
    **\--specialize**.

.. _man-fast-followers:

**\--[no-]fast-followers**

    Enable [disable] the fast follower optimization in which fast
    implementations of followers will be invoked for specific leaders.

.. _man-ieee-float:

**\--[no-]ieee-float**

    Disable [enable] optimizations that may affect IEEE floating point
    conformance. The default is whatever level of optimization/IEEE floating
    point support your C compiler provides at the optimization level
    provided by '\ **chpl**\ '.

.. _man-loop-invariant-code-motion:

**\--[no-]loop-invariant-code-motion**

    Enable [disable] the optimization that moves loop invariant code from
    loop runs into the loop's "pre-header." By default invariant code is
    moved. This is currently a rather conservative pass in the sense that it
    may not identify all code that is truly invariant.

.. _man-optimize-forall-unordered-ops:

**\--[no-]optimize-forall-unordered-ops**

    Enable [disable] optimization of the last statement in forall statements
    to use unordered communication. This optimization works with runtime
    support for unordered operations with CHPL_COMM=ugni.

.. _man-ignore-local-classes:

**\--[no-]ignore-local-classes**

    Disable [enable] local classes

.. _man-inline:

**\--[no-]inline**

    Enable [disable] function inlining.

.. _man-inline-iterators:

**\--[no-]inline-iterators**

    Enable [disable] iterator inlining. When possible, the compiler
    optimizes the invocation of an iterator in a loop header by inlining the
    iterator's definition around the loop body.

.. _man-inline-iterators-yield-limit:

**\--inline-iterators-yield-limit**

    Limit on the number of yield statements permitted in an inlined iterator.
    The default value is 10.

.. _man-live-analysis:

**\--[no-]live-analysis**

    Enable [disable] live variable analysis, which is currently only used to
    optimize iterators that are not inlined.

.. _man-optimize-range-iteration:

**\--[no-]optimize-range-iteration**

    Enable [disable] anonymous range iteration optimizations. This allows the
    compiler to avoid creating ranges when they are only used for iteration.
    By default this is enabled.

.. _man-optimize-loop-iterators:

**\--[no-]optimize-loop-iterators**

    Enable [disable] optimizations to aggressively optimize iterators that
    are defined in terms of a single loop. By default this is enabled.

.. _man-vectorize:

**\--[no-]vectorize**

    Enable [disable] generating vectorization hints for the target compiler.
    If enabled, hints will always be generated, but the effects on performance
    (and in some cases correctness) will vary based on the target compiler.

.. _man-optimize-on-clauses:

**\--[no-]optimize-on-clauses**

    Enable [disable] optimization of on clauses in which qualifying on
    statements may be optimized in the runtime if supported by the
    $CHPL\_COMM layer.

.. _man-optimize-on-clause-limit:

**\--optimize-on-clause-limit**

    Limit on the function call depth to allow for on clause optimization.
    The default value is 20.

.. _man-privatization:

**\--[no-]privatization**

    Enable [disable] privatization of distributed arrays and domains if the
    distribution supports it.

.. _man-remove-copy-calls:

**\--[no-]remove-copy-calls**

    Enable [disable] removal of copy calls (including calls to what amounts
    to a copy constructor for records) that ensure Chapel semantics but
    which can often be optimized away.

.. _man-remote-value-forwarding:

**\--[no-]remote-value-forwarding**

    Enable [disable] remote value forwarding of read-only values to remote
    threads if reading them early does not violate program semantics.

.. _man-remote-serialization:

**\--[no-]remote-serialization**

    Enable [disable] serialization for globals and remote constants.

.. _man-scalar-replacement:

**\--[no-]scalar-replacement**

    Enable [disable] scalar replacement of records and classes for some
    compiler-generated data structures that support language features such
    as tuples and iterators.

.. _man-scalar-replace-limit:

**\--scalar-replace-limit**

    Limit on the size of tuples being replaced during scalar replacement.
    The default value is 8.

.. _man-tuple-copy-opt:

**\--[no-]tuple-copy-opt**

    Enable [disable] the tuple copy optimization in which whole tuple copies
    of homogeneous tuples are replaced with explicit assignment of each
    tuple component.

.. _man-tuple-copy-limit:

**\--tuple-copy-limit**

    Limit on the size of tuples considered for the tuple copy optimization.
    The default value is 8.

.. _man-infer-local-fields:

**\--[no-]infer-local-fields**

    Enable [disable] analysis to infer local fields in classes and records
    (experimental)

.. _man-auto-local-access:

**\--[no-]auto-local-access**

    Enable [disable] an optimization applied to forall loops over domains in
    which accesses of the form of `A[i]` within the loop are transformed to use
    local accesses if the array `A` is aligned with the domain and `i` is the
    loop index variable. With this flag, the compiler does some static analysis
    and adds calls that can further analyze alignment dynamically during
    execution time.

.. _man-dynamic-auto-local-access:

**\--[no-]dynamic-auto-local-access**

    Enable [disable] the dynamic portion of the analysis described in
    `\--[no-]auto-local-access`.  This dynamic analysis can result in loop
    duplication that increases executable size and compilation time. There
    may also be execution time overheads independent of loop domain size.

.. _man-offset-auto-local-access:

**\--[no-]offset-auto-local-access**

    Enable [disable] automatic local access optimization for array accesses with
    simple offsets like `A[i+1]`. This optimization only applies to
    stencil-distributed arrays.

.. _man-auto-aggregation:

**\--[no-]auto-aggregation**

    Enable [disable] optimization of the last statement in forall statements to
    use aggregated communication. This optimization is disabled by default.

.. _man-array-view-elision:

**\--[no-]array-view-elision**

    Enable [disable] an optimization eliding array views in some statements.

*Run-time Semantic Check Options*

.. _man-checks:

**\--[no-]checks**

    Enable [disable] all of the run-time checks in this section of the man page.
    Currently, it is typically necessary to use this flag (or **\--fast**,
    which implies **\--no-checks**) to achieve performance competitive with
    hand-coded C or Fortran.

.. _man-bounds-checks:

**\--[no-]bounds-checks**

    Enable [disable] run-time bounds checking, e.g. during slicing and array
    indexing.

.. _man-cast-checks:

**\--[no-]cast-checks**

    Enable [disable] run-time checks in safeCast calls for casts that
    wouldn't preserve the logical value being cast.

.. _man-const-arg-checks:

**\--[no-]const-arg-checks**

    Enable [disable] const argument checks (only when **--warn-unstable** is
    also used).  These checks will warn when an argument is inferred to be
    `const ref` and is indirectly modified over the course of the function.  To
    silence the warning for a particular argument, give it a concrete argument
    intent (such as `const ref` or `const in`, depending on if the indirect
    modification behavior should be preserved or avoided).

.. _man-div-by-zero-checks:

**\--[no-]div-by-zero-checks**

    Enable [disable] run-time checks in integer division and modulus operations
    to guard against dividing by zero.

.. _man-formal-domain-checks:

**\--[no-]formal-domain-checks**

    Enable [disable] run-time checks to ensure that an actual array
    argument's domain matches its formal array argument's domain in terms of
    (a) describing the same index set and (b) having an equivalent domain
    map (if the formal domain explicitly specifies a domain map).

.. _man-local-checks:

**\--[no-]local-checks**

    Enable [disable] run-time checking of the locality of references within
    local blocks.

.. _man-nil-checks:

**\--[no-]nil-checks**

    Enable [disable] run-time checking for accessing nil object references.

.. _man-stack-checks:

**\--[no-]stack-checks**

    Enable [disable] run-time checking for stack overflow.

*C Code Generation Options*

.. _man-codegen:

**\--[no-]codegen**

    Enable [disable] generating C code and the binary executable. Disabling
    code generation is useful to reduce compilation time, for example, when
    only Chapel compiler warnings/errors are of interest.

.. _man-cpp-lines:

**\--[no-]cpp-lines**

    Causes the compiler to emit cpp #line directives into the generated code
    in order to help map generated C code back to the Chapel source code
    that it implements. The [no-] version of this flag turns this feature
    off.

.. _man-max-c-ident-len:

**\--max-c-ident-len**

    Limits the length of identifiers in the generated code, except when set
    to 0. The default is 0, except when $CHPL\_TARGET\_COMPILER indicates a
    PGI compiler (pgi or cray-prgenv-pgi), in which case the default is
    1020.

.. _man-munge-user-idents:

**\--[no-]munge-user-idents**

    By default, **chpl** munges all user identifiers in the generated C code
    in order to minimize the chances of conflict with an identifier or
    keyword in C (in the current implementation, this is done by appending
    '\_chpl' to the identifier). This flag provides the ability to disable
    this munging. Note that whichever mode, the flag is in, **chpl** may
    perform additional munging in order to implement Chapel semantics in C,
    or for other reasons.

.. _man-savec:

**\--savec <dir>**

    Saves the compiler-generated C code in the specified *directory*,
    creating the *directory* if it does not already exist. This option may
    overwrite existing files in the *directory*.

*C Code Compilation Options*

.. _man-ccflags:

**\--ccflags <flags>**

    Add the specified flags to the C compiler command line when compiling
    the generated code. Multiple **\--ccflags** *options* can be provided and
    in that case the combination of the flags will be forwarded to the C
    compiler.

.. _man-debug:

**-g, \--[no-]debug**

    Causes the generated C code to be compiled with debugging turned on. If
    you are trying to debug a Chapel program, this flag is virtually
    essential along with the **\--savec** flag. This flag also turns on the
    **\--cpp-lines** option unless compiling as a developer (for example, via
    **\--devel**).

.. _man-dynamic:

**\--dynamic**

    Use dynamic linking when generating the final binary. If neither
    **\--dynamic** or **\--static** are specified, use the backend compiler's
    default.

.. _man-hdr-search-path:

**-I, \--hdr-search-path <dir>**

    Add the specified dir[ectories] to the back-end C compiler's
    search path for header files along with any directories in the
    CHPL\_INCLUDE\_PATH environment variable.  Both the environment
    variable and this flag accept a colon-separated list of
    directories.

.. _man-ldflags:

**\--ldflags <flags>**

    Add the specified flags to the back-end C compiler link line when
    linking the generated code. Multiple **\--ldflags** *options* can
    be provided and in that case the combination of the flags will be
    forwarded to the C compiler.

.. _man-lib-linkage:

**-l, \--lib-linkage <library>**

    Specify a C library to link to on the back-end C compiler command
    line.

.. _man-lib-search-path:

**-L, \--lib-search-path <dir>**

    Add the specified dir[ectories] to the back-end C compiler's
    search path for libraries along with any directories in the
    CHPL\_LIB\_PATH environment variable.  Both the environment
    variable and this flag accept a colon-separated list of
    directories.

.. _man-optimize:

**-O, \--[no-]optimize**

    Causes the generated C code to be compiled with [without] optimizations
    turned on. The specific set of flags used by this option is
    platform-dependent; use the **\--print-commands** option to view the C
    compiler command used. If you would like additional flags to be used
    with the C compiler command, use the **\--ccflags** option.

.. _man-specialize:

**\--[no-]specialize**

    Causes the generated C code to be compiled with flags that specialize
    the executable to the architecture that is defined by
    CHPL\_TARGET\_CPU. The effects of this flag will vary based on choice
    of back-end compiler and the value of CHPL\_TARGET\_CPU.

.. _man-output:

**-o, \--output <filename>**

    Specify the name of the compiler-generated executable. Defaults to
    the filename of the main module (minus its `.chpl` extension), if
    unspecified.

.. _man-static:

**\--static**

    Use static linking when generating the final binary. If neither
    **\--static** or **\--dynamic** are specified, use the backend compiler's
    default.

*LLVM Code Generation Options*

.. _man-llvm-wide-opt:

**\--[no-]llvm-wide-opt**

    Enable [disable] LLVM wide pointer communication optimizations. This
    option requires CHPL_TARGET_COMPILER=llvm. You must also supply
    **\--fast** to enable wide pointer optimizations. This flag allows
    existing LLVM optimizations to work with wide pointers - for example,
    they might be able to hoist a 'get' out of a loop. See
    $CHPL\_HOME/doc/rst/technotes/llvm.rst for details.

.. _man-mllvm:

**\--mllvm <option>**

    Pass an option to the LLVM optimization and transformation passes.
    This option can be specified multiple times.


*Compilation Trace Options*

.. _man-print-commands:

**\--[no-]print-commands**

    Prints the system commands that the compiler executes in order to
    compile the Chapel program.

.. _man-print-passes:

**\--[no-]print-passes**

    Prints the compiler passes during compilation and the amount of wall
    clock time required for the pass. After compilation is complete two
    tables are produced that provide more detail of how time is spent in
    each pass (compiling, verifying, and memory management) and the
    percentage of the total time that is attributed to each pass. The first
    table is sorted by pass and the second table is sorted by the time for
    the pass in descending order.

.. _man-print-passes-file:

**\--print-passes-file <filename>**

    Saves the compiler passes and the amount of wall clock time required for
    the pass to <filename>. An error is displayed if the file cannot be
    opened but no recovery attempt is made.

*Miscellaneous Options*

.. _man-detailed-errors:

**\--[no-]detailed-errors**

    Enables [disables] the compiler's detailed error message mode. In this
    mode, the compiler will print additional information about errors when
    it is available. This could include printing and underlining relevant
    segments of code, or providing suggestions for how to fix the error.

.. _man-devel:

**\--[no-]devel**

    Puts the compiler into [out of] developer mode, which takes off some of
    the safety belts, changes default behaviors, and exposes additional
    undocumented command-line *options*. Use at your own risk and direct any
    questions to the Chapel team.

.. _man-explain-call:

**\--explain-call <call>[:<module>][:<line>]**

    Helps explain the function resolution process for the named function by
    printing out the visible and candidate functions. Specifying a module
    name and/or line number can focus the explanation to those calls within
    a specific module or at a particular line number.

.. _man-explain-instantiation:

**\--explain-instantiation <function\|type>[:<module>][:<line>]**

    Lists all of the instantiations of a function or type. The location of
    one of possibly many points of instantiation is shown. Specifying a
    module name and/or line number can focus the explanation to those calls
    within a specific module or at a particular line number.

.. _man-explain-verbose:

**\--[no-]explain-verbose**

    In combination with explain-call or explain-instantiation, causes the
    compiler to output more debug information related to disambiguation.

.. _man-instantiate-max:

**\--instantiate-max <max>**

    In order to avoid infinite loops when instantiating generic functions,
    the compiler limits the number of times a single function can be
    instantiated. This flag raises that maximum in the event that a legal
    instantiation is being pruned too aggressively.

.. _man-print-all-candidates:

**\--[no-]print-all-candidates**

    By default, function resolution errors show only a few candidates.
    Use this flag to see all of the candidates for a call that could
    not be resolved.

.. _man-print-callgraph:

**\--[no-]print-callgraph**

    Print a textual call graph representing the program being compiled. The
    output is in top-down and depth first order. Recursive calls are marked
    and cause the traversal to stop along the path containing the recursion.
    Only a single call to each function is displayed from within any given
    parent function.

.. _man-print-callstack-on-error:

**\--[no-]print-callstack-on-error**

    Accompany certain error and warning messages with the Chapel call stack
    that the compiler was working on when it reached the error or warning
    location. This is useful when the underlying cause of the issue is in
    one of the callers.

.. _man-print-unused-functions:

**\--[no-]print-unused-functions**

    Print the names and source locations of unused functions within the
    user program.

.. _man-set:

**-s, \--set <config>[=<value>]**

    Overrides the default value of a configuration param, type, var,
    or const in the code.  If the value is omitted, it will default
    to the value `true`.

.. _man-task-tracking:

**\--[no-]task-tracking**

    Enable [disable] the Chapel-implemented task tracking table that
    supports the execution-time **-b** / **-t** flags. This option is
    currently only useful when $CHPL\_TASKS is set or inferred to 'fifo' and
    adds compilation-time overhead when it will not be used, so is off by
    default.


*Compiler Configuration Options*

Note that the flags in this section all have corresponding environment
variables.  Details on those environment variables, including potential values
for them, can be found at
https://chapel-lang.org/docs/usingchapel/chplenv.html or at
doc/rst/usingchapel/chplenv.rst in your Chapel installation.

.. _man-home:

**\--home <path>**

    Specify the location of the Chapel installation *directory*. This flag
    corresponds with and overrides the $CHPL\_HOME environment variable.

.. _man-atomics:

**\--atomics <atomics-impl>**

    Specify the implementation to use for Chapel's atomic variables. This
    flag corresponds with and overrides the $CHPL\_ATOMICS environment
    variable. (defaults to a best guess based on $CHPL\_TARGET\_COMPILER,
    $CHPL\_TARGET\_PLATFORM, and $CHPL\_COMM)

.. _man-network-atomics:

**\--network-atomics <network>**

    Specify the network atomics implementation for all atomic variables.
    This flag corresponds with and overrides the $CHPL\_NETWORK\_ATOMICS
    environment variable (defaults to best guess based on $CHPL\_COMM).

.. _man-aux-filesys:

**\--aux-filesys <aio-system>**

    Specify runtime support for additional file systems. This flag
    corresponds with and overrides the $CHPL\_AUX\_FILESYS environment
    variable (defaults to 'none').

.. _man-comm:

**\--comm <comm-impl>**

    Specify the communication implementation to use for inter-\ *locale*
    data transfers. This flag corresponds with and overrides the $CHPL\_COMM
    environment variable (defaults to 'none').

.. _man-comm-substrate:

**\--comm-substrate <conduit>**

    Specify the communication conduit for the communication implementation.
    This flag corresponds with and overrides the $CHPL\_COMM\_SUBSTRATE
    environment variable (defaults to best guess based on
    $CHPL\_TARGET\_PLATFORM).

.. _man-gasnet-segment:

**\--gasnet-segment <segment>**

    Specify memory segment to use with GASNet. This flag corresponds with
    and overrides the $CHPL\_GASNET\_SEGMENT environment variable. (defaults
    to best guess based on $CHPL\_COMM\_SUBSTRATE).

.. _man-gmp:

**\--gmp <gmp-version>**

    Specify the GMP library implementation to be used by the GMP module.
    This flag corresponds with and overrides the $CHPL\_GMP environment
    variable (defaults to best guess based on $CHPL\_TARGET\_PLATFORM and
    whether you've built the included GMP library in the third-party
    *directory*).

.. _man-hwloc:

**\--hwloc <hwloc-impl>**

    Specify whether or not to use the hwloc library. This flag corresponds
    with and overrides the $CHPL\_HWLOC environment variable (defaults to a
    best guess based on whether you've built the included library in the
    third-party hwloc *directory*).

.. _man-launcher:

**\--launcher <launcher-system>**

    Specify the launcher, if any, used to start job execution. This flag
    corresponds with and overrides the $CHPL\_LAUNCHER environment variable
    (defaults to a best guess based on $CHPL\_COMM and
    $CHPL\_TARGET\_PLATFORM).

.. _man-lib-pic:

**\--lib-pic <pic>**

    Specify whether to use position-dependent or position-independent code.
    Position-independent code is suitable for shared libraries and this flag is
    intended to be used when calling Chapel code from other languages,
    especially Python or when building with '--dynamic'.  This flag corresponds
    with and overrides the $CHPL\_LIB\_PIC environment variable (defaults to
    'none').

.. _man-locale-model:

**\--locale-model <locale-model>**

    Specify the *locale* model to use for describing your *locale*
    architecture. This flag corresponds with and overrides the
    $CHPL\_LOCALE\_MODEL environment variable (defaults to 'flat').

.. _man-make:

**\--make <make utility>**

    Specify the GNU compatible make utility. This flag corresponds with and
    overrides the $CHPL\_MAKE environment variable (defaults to a best guess
    based on $CHPL\_HOST\_PLATFORM).

.. _man-mem:

**\--mem <mem-impl>**

    Specify the memory allocator used for dynamic memory management. This
    flag corresponds with and overrides the $CHPL\_MEM environment variable
    (defaults to a best guess based on $CHPL\_COMM).

.. _man-re2:

**\--re2 <re2>**

    Specify the RE2 library to use. This flag corresponds with and overrides
    the $CHPL\_RE2 environment variable (defaults to 'none' or 'bundled' if
    you've installed the re2 package in the third-party *directory*).

.. _man-target-arch:

**\--target-arch <architecture>**

    Specify the machine type or general architecture to use.
    This flag corresponds with and overrides the $CHPL\_TARGET\_ARCH
    environment variable (defaults to the result of `uname -m`).

.. _man-target-compiler:

**\--target-compiler <compiler>**

    Specify the compiler suite that should be used to build the generated C
    code for a Chapel program and the Chapel runtime. This flag corresponds
    with and overrides the $CHPL\_TARGET\_COMPILER environment variable
    (defaults to a best guess based on $CHPL\_HOST\_PLATFORM,
    $CHPL\_TARGET\_PLATFORM, and $CHPL\_HOST\_COMPILER).

.. _man-target-cpu:

**\--target-cpu <cpu>**

    Specify the cpu model that the compiled executable will be
    specialized to when **\--specialize** is enabled. This flag corresponds
    with and overrides the $CHPL\_TARGET\_CPU environment variable
    (defaults to a best guess based on $CHPL\_COMM, $CHPL\_TARGET\_COMPILER,
    and $CHPL\_TARGET\_PLATFORM).

.. _man-target-platform:

**\--target-platform <platform>**

    Specify the platform on which the target executable is to be run for the
    purposes of cross-compiling. This flag corresponds with and overrides
    the $CHPL\_TARGET\_PLATFORM environment variable (defaults to
    $CHPL\_HOST\_PLATFORM).

.. _man-tasks:

**\--tasks <task-impl>**

    Specify the tasking layer to use for implementing tasks. This flag
    corresponds with and overrides the $CHPL\_TASKS environment variable
    (defaults to a best guess based on $CHPL\_TARGET\_PLATFORM).

.. _man-timers:

**\--timers <timer-impl>**

    Specify a timer implementation to be used by the Time module. This flag
    corresponds with and overrides the $CHPL\_TIMERS environment variable
    (defaults to 'generic').

*Compiler Information Options*

.. _man-copyright:

**\--copyright**

    Print the compiler's copyright information.

.. _man-help:

**-h, \--help**

    Print a list of the command line *options*, indicating the arguments
    that they expect and a brief summary of their purpose.

.. _man-help-env:

**\--help-env**

    Print the command line option help message, listing the environment
    variable equivalent for each flag (see ENVIRONMENT) and its current
    value.

.. _man-help-settings:

**\--help-settings**

    Print the command line option help message, listing the current setting
    of each option as specified by environment variables and other flags on
    the command line.

.. _man-license:

**\--license**

    Print the compiler's license information.

.. _man-print-chpl-home:

**\--print-chpl-home**

    Prints the compiler's notion of $CHPL\_HOME.

.. _man-version:

**\--version**

    Print the version number of the compiler.

ENVIRONMENT
-----------

Most compiler command-line *options* have an environment variable that
can be used to specify a default value. Use the **\--help-env** option to
list the environment variable equivalent for each option. Command-line
*options* will always override environment variable settings in the
event of a conflict, except for ccflags and ldflags, which stack.

If the environment variable equivalent is set to empty, it is considered
unset. This does not apply to *options* expecting a string or a path.

For *options* that can be used with or without the leading **\--no**
(they are shown with "[no-]" in the help text), the environment variable
equivalent, when set to a non-empty string, has the following effect.
When the first character of the string is one of:

|

    Y y T t 1 - same as passing the option without \--no,

    N n F f 0 - same as passing the option with \--no,

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

| Copyright 2020-2024 Hewlett Packard Enterprise Development LP
| Copyright 2004-2019 Cray Inc.
