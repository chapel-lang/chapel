.. _readme-nil-checking:

=============================
Checking for Nil Dereferences
=============================

The Chapel compiler includes two different mechanisms to check for nil
dereferences. First, at compile-time, the compiler looks for places where
it can prove a `nil` will be dereferenced or a method called on `nil`.
When it can find such a case, it raises an error.

The second mechanism is a run-time check inserted at each method call on
an object. It checks that the receiver object is not `nil`.  These checks
are on by default and can be disabled by `--fast`, `--no-checks`, or
`--no-nil-checks`.

Example of Compile-time Nil Checking
====================================

Consider this example program:

.. literalinclude:: ../../../test/technotes/doc-examples/NilCheckingExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0

Compiling this program will produce the following compilation errors:

.. literalinclude:: ../../../test/technotes/doc-examples/NilCheckingExamples.good

Limitations of Compile-time Nil Checking
========================================

The compiler analysis for nil checking only does per-function analysis
using very conservative alias analysis rules. It cannot detect all cases
where a nil is dereferenced. Here are some limitations of the analysis:

 * it gives up in certain complex control flow situations
 * it does not track may-alias sets
 * it is not interprocedural

Additionally, compile-time checks are not performed for module-scope
code. If the code in the previous example were moved outside ``main``,
then the program would compile without issues.

Nonetheless we hope that the additional checking provided by this pass
is useful. If you run into a surprising case where it reports an error,
please file a bug report. Also, note that using the compiler flag
``--no-compile-time-nil-checking`` will disable these checks. However,
the exact nature of the checking and the name of the flag are likely to
change in future releases.
