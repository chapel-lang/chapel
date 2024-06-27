.. default-domain:: chpl

.. _best-practices-standard-module-style:

Standard Module Style
=====================

This document describes general guidance for style when writing a
standard module. Of course, there will be exceptions to this guidance when
there is a good reason.

PascalCase and camelCase
------------------------

Prefer PascalCase or camelCase to separating_with_underscores. The rest
of this document will use PascalCase or camelCase to signify the pattern
of indicating new words with an upper case letter and starting the
identifier; where PascalCase starts the name with an upper case letter
and camelCase starts with a lower case letter.

When including acronyms in PascalCase or camelCase, treat the acronym as
if it were a regular word. There is one exception - if using PascalCase
and the acronym is the entire symbol name, then let the acronym be all
uppercase.

Here are a few examples:
 * "rendered HTML DOM" in PascalCase is RenderedHtmlDom
 * "rendered HTML DOM" in camelCase is renderedHtmlDom
 * "HTML" in PascalCase is HTML
 * "HTML" in camelCase is html

Exceptions:
 The following are exceptions to the above capitalization guidelines that occur
 in the standard modules or packages:

  * :proc:`~ChapelLocale.locale.numPUs` (num Processor-Units)

 Our hope and intention is to update our rules over time so that they cover
 cases like this, obviating the need to call it out as an exception. As more
 exceptions accumulate, rules may be amended to cover multiple cases at once.

Modules
-------

Module names should be PascalCase.

.. note::

  Chapel does not use a separate namespace for types vs. module names. As
  a result, it's generally a goal to avoid using the same name for, say,
  a type and a module.

Classes
-------

Class type names should be PascalCase. The idea is that starting with an
uppercase letter is the convention for by-reference types.

Records
-------

Record type names should be camelCase. The idea is that starting with a
lowercase letter is the convention for by-value types.

Enums
-----

Enum type names should be camelCase. The enum values should also be
camelCase.

Functions and Methods
---------------------

Function and method names should be camelCase.

Generally speaking, it's desirable to use methods on a value (vs
functions that aren't methods) when there is a clear type responsible for
the operation.

Use parentheses-less methods only for returning properties that could be
reasonably implemented as fields. However, if such a method is named
isXYZ or hasXYZ it should use parentheses (so, use
``proc isReal() { ...  }`` rather than ``proc isReal { ... }``).
Parentheses-less functions that aren't methods should be avoided.

Many paren-ful methods take some notable action. Try to make these
methods method names be a verb. In particular, a method that modifies an
argument in-place should be a verb.

When possible, functions and methods that return should declare their return
type explicitly.  When not possible, such functions and methods should use the
``:returns:`` documentation field (as described in
:ref:`chpldoc-arg-return-yield-types`).

Factory Functions and Methods
+++++++++++++++++++++++++++++

Factory functions or methods are procedures where the main purpose of
that procedure is to create a new object or value. There are many other
procedures that return a value but constructing that value is not the main
purpose of the function. For example, 'open' and 'spawn' are not factory
functions -- the main action of these methods is to work with the OS to
open a file or launch a subprocess.

In many cases factory procedures are not needed. Regular initializers
will work just fine to support the pattern of ``new Something(...)``.
Type conversion is best supported by creating a cast (``:``) operator
overload.

One case in which factory procedures are useful is when it is awkward to
distinguish between several ways of creating something based on the
initializer arguments alone.

Factory procedures should have a name that begin with 'create'.

Factory procedures can be functions or type methods. For example, we
could have:

 * ``createStringWithBorrowedBuffer`` or
 * ``string.createBorrowingBuffer``.

Accessors
+++++++++

Accessor methods are methods intended to return a distinct piece of information
about the type on which they are defined.  This piece of information could be
reasonably implemented as a field.  Typically, such methods are defined without
parentheses, allowing them to appear similar to a field access.  However, there
is no requirement that the contents of the accessor be similarly simple -
whether the information returned is calculated based on the internal fields of
the type or whether it is actually a field is an implementation detail.

Accessor methods will avoid using "get" in their name.  E.g., instead of
``array.getIdxType``, the accessor is named ``array.idxType``.

Methods that are not accessors are still allowed to use "get" in their name.

Formals
+++++++

Formal names should be camelCase or PascalCase.  Descriptive names are
recommended, within reason.  Encoding the type name into a formal name is
generally avoided when multiple types are supported, as doing so makes it harder
to support generic functions.

Some commonly used names are:

- ``idx`` for an index

- ``x`` and ``y`` for general pairs of formals

- ``src`` and ``dst``/``dest`` (source and destination) for directional pairs of
  formals

  - ``lhs`` and ``rhs`` (left hand side and right hand side) may also be used

- ``eltType`` for the type of elements stored in a collection (also used as a
  field name)

- ``obj`` for a general object instance (as opposed to formals that can also be
  primitive types)

Other Identifiers
-----------------

Variables and fields should be camelCase or PascalCase.

Handling Failure
----------------

In general, when code in a library encounters erroneous behavior, an appropriate
:class:`~Errors.Error` should be thrown (see the :ref:`spec description
<Chapter-Error_Handling>` for error handling in general).  This will enable
users of the library to potentially recover from the bad behavior without having
to restart their program.

There are some known exceptions to this recommendation:

- When division by zero is encountered, libraries will halt (unless the check
  for division by zero has been disabled, see the flag's
  :ref:`man page entry <man-div-by-zero-checks>`).

- When accesses are made outside of the known bounds of a container type,
  libraries will halt (unless bounds checking has been disabled, see the flag's
  :ref:`man page entry <man-bounds-checks>`).

- Other checks controlled by flags that are listed when running ``chpl -h``.

- When the program runs out of memory and the function in question does not
  already throw other types of errors, the library will halt.
