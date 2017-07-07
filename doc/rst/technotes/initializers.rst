.. _readme-initializers:

============
Initializers
============

The Chapel team is implementing a new approach to user-defined
initialization of variables with record type or instances of class
type.  This approach relies on methods known as initializers rather
than the original methods known as constructors.

A discussion of the current design and rationale is provided in
`CHIP 10 <https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst>`_.

Release Chapel 1.15.0 provides a strong preview implementation of this
new feature. This initial implementation is not of production quality
and has not been heavily tested, but the basic features are working
and it is believed that adventurous developers will have success using
init methods in new code.

It is anticipated that the implementation will continue to advance
steadily after this release and that many aspects of the internal
implementation of Chapel will transition from constructor methods to
initializer methods during this release.


Stability
---------

The functionality of an initializer method has four major aspects

  - user-defined code that runs in phase 2

  - user-defined code that runs in phase 1

  - compiler generated code that runs in phase 1

  - the generation of effective user-facing diagnostic messages

in approximate order of robustness and maturity.

The user-defined code that executes in phase 2 of an initializer is
comparable to the user-defined code in any method and is likely to
behave in the expected way.

The user-defined code that executes in phase 1 of an initializer has
many of the same properties and is also expected to behave well.
However there are more restrictions on user-defined code for phase 1
and the compiler may fail to apply these restrictions correctly.  The
most likely error is that the compiler will accept phase 1 code that
it should reject but it is possible that there will be situations in
which valid code is rejected.

The design for initializer methods attempts to define predictable
default behaviors within phase 1 and then rely on the compiler to
insert these defaults when appropriate.  The intent is to reduce the
amount of code that the type designer is responsible for.  This
includes the generation of code using field-level default
declarations.  Defining and implementing these default behaviors
relies on relatively less mature code within the compiler.

We believe that initializer methods are ready for use in application code
but we also expect that the implementation will mature significantly
during the 1.15 release.  We encourage their use but recognize that they
may prove to be less robust than we would like at the start of the release.
If you do choose to experiment with this feature then please do not
hesitate to seek help, report bugs, or offer suggestions.

Generic records and classes, i.e. records or classes in which the
definition of the type of one or more fields is parameterized are less
well developed than records and classes for which the type of every
fields can be inferred directly from the type declaration.


Status
------

As of the 1.15.0 release, support for initializers includes the following:

- Methods named "init" on a type are utilized during the creation of an
  instance of that type.

- An error message is generated if both a constructor and an initializer is
  defined on a type.

- User-defined initializers whose body consists of only Phase 2 statements
  appear to be fully supported

- User-defined initializers with Phase 1 statements that explicitly initialize
  all of the fields appear to be well supported

  - Some errors remain with omitted field initialization in Phase 1, notably
    with utilizing ``param`` fields in the omitted initialization of another
    field, and with utilizing some record fields.

- Many checks against Phase 1 and Phase 2 rules are implemented, including:

  - Forbidding the update of ``const`` fields during Phase 2
  - Forbidding the initialization of fields in parallel statements during
    Phase 1
  - Forbidding the initialization of fields in loops during Phase 1
  - Forbidding ``super.init()`` and ``this.init()`` calls in parallel
    statements
  - Forbidding ``super.init()`` and ``this.init()`` calls in loops
  - Forbidding access to a field prior to its initialization in Phase 1
  - Forbidding the initialization of fields prior to ``this.init()`` calls
  - Limiting the access of parent fields during Phase 1 of initialization

- Support for initializers on generic classes when instances infer their type
  from the result of a ``new`` call or another instance.

  - As a result, generic fields no longer require an argument sharing their
    name in the initializer argument list.

- Support for copy initializers
- Support for deinitializers, as a replacement of destructors.

  - Library types were converted to use deinitializers.

Remaining todo items include:

- Support for compiler generated initializers
- Improve the error message for out of order field initialization during
  Phase 1
- Forbid the utilization of methods on the type during Phase 1 of
  initialization, as well as the use of ``this`` as an argument to a function.
- Full support for ``param`` conditional situations
- Resolve an issue with secondary initializers
- Support for instance declarations of generic instantiations, i.e. ``var x:
  Foo(t);``
- Resolve an issue with generic initializers and records (relates to records
  being passed by ``ref`` to the initializer method)
- Convert library types to utilize initializers instead of constructors
- Support ``noinit``
