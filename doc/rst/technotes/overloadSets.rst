.. _readme-overload-sets:

======================
Checking Overload Sets
======================

The Chapel compiler provides an initial implementation of the
Overload Sets feature. It is discussed in `CHIP 20
<https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/20.rst>`_
as a solution to "hijacking" problems where program behavior can change
unexpectedly as libraries it uses change.

An "overload set" is a group of functions with the same name declared
in the same module. Overload sets checking is performed at each
function call. It verifies that all candidate functions identified
while resolving this call are defined in the same module. See the
"Function Resolution" section of the language spec for
resolution-related terminology.

Additionally, if the call resolves to return intent overloads,
i.e. for ref, const-ref, and/or value return intents,
these overloads must be defined in the same module.

The current implementation does not report an error in the following cases:

- The best candidate is "more visible" than the other candidates.
  That is, it shadows the other candidates along all module import paths
  starting at the call.

- If it is a method call on a class, the best candidate is defined
  in the same module as the class of the receiver actual argument.

- The call already generates an "unresolved" or "ambiguous" error.

- The candidates defined in internal modules are ignored, as if
  they are not present among the identified candidates.

In some cases utilizing function definitions from multiple overload sets
is actually desirable. To support this, we plan to provide a way
to merge overload sets. Merging is discussed in `GitHub issue 12635
<https://github.com/chapel-lang/chapel/issues/12635>`_.

The compiler provides the option ``--no-overload-sets-checks``
to work around the overload set errors when they are undesirable.

We are evaluating this feature and invite user feedback.
Our open questions include:

- Should overload sets continue to consider only module scoping,
  or should they be updated to differentiate, for example, between
  scopes inside functions?

- What should a "merge the overload sets" declaration look like?

- How to handle overload sets for a method call when dynamic dispatch
  is possible?

- What are the situations when the currently-implemented check
  is too restrictive or too permissive?
