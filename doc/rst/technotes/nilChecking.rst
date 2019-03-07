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

Limitations of Compile-time Nil Checking
========================================

The compiler analysis for nil checking only does per-function analysis
using very conservative alias analysis rules. It cannot detect all cases
where a nil is dereferenced. Here are some limitations of the analysis:

 * it gives up in certain complex control flow situations
 * it does not track may-alias sets
 * it is not interprocedural

Nonetheless we hope that the additional checking provided by this pass
is useful.
