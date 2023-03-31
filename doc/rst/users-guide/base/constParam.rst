:title: Users Guide: Const/Param Declarations

.. _ug-constParam:

Constant Declarations
=====================

In addition to variables, Chapel supports the declaration of
*constants* which can be thought of as variables whose values cannot
be changed once they've been initialized.  Chapel supports two
flavors of constants, each of which is introduced by a distinct
keyword:

* ``const`` introduces an execution-time constant whose value need not
  be known to the compiler

* ``param`` introduces a compile-time constant whose value must be
  known to the compiler

Declaring immutable variables as ``const`` or ``param`` provides
helpful semantic information, both to human readers of Chapel
programs, and to the compiler itself.

In practice, ``param`` values are most often used in cases where
compile-time operations or specialization will reduce execution-time
computation, resulting in better performance.  In other cases, certain
language constructs are required to be ``param`` values for the sake
of static typing or performance.  Examples include the indices used
when indexing heterogeneous tuples or the number of dimensions when
declaring rectangular arrays.  In situations other than the above,
``const`` declarations tend to be used since they are less restrictive
as we will see below.




Declaring *consts*
------------------

Constant declarations are a lot like :ref:`variable declarations
<ug-vars>`, except for the change in keyword.  As a simple example,
the following statement declares an integer constant named *two* whose
value is 2 (as you'd hope):

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :caption:
  :language: chapel
  :lines: 5

Unlike variables, attempting to assign to a constant after it has been
initialized will result in a compile-time error.  For example, if we
later attempted to perform this assignment:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 8
  :dedent: 2

The compiler would report the following error:

.. literalinclude:: examples/users-guide/base/constParam-error.good
  :language: text
  :lines: 1

As with variable declarations, the type specification or initializer
may be omitted in a ``const`` declaration, in which case the compiler
will fill in any missing details:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 10-11

Of course, since constants cannot be re-assigned, in practice they
typically do include an initialization expression.

Like variables, constants support multiple comma-separated
declarations per statement, where types and/or initializers may be
shared:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 15-21

One of the main things that distinguishes ``const`` from ``param``
declarations is that the initializing expression for a ``const`` need
not be a value that is known at compile-time.  For example, the
following constant declarations are initialized by reading a value
from the console and then using that value to do some math:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 23-24

If we were to print their values as follows:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 26-27

And then typed '4' at the console after compiling and running the program,
we'd see:

.. literalinclude:: examples/users-guide/base/constParam.good
  :language: text
  :lines: 4-5

As a final note, Chapel supports ``const`` declarations of any type.


Declaring *params*
------------------

Declaring a ``param`` value is syntactically identical to declaring a
``var`` or ``const``, but uses the keyword ``param``.
The initializer of a ``param`` must be able to be computed at
compile-time, unlike variables and execution-time constants.  Thus,
most of the ``const`` declarations of the previous section could
trivially be converted to ``param`` declarations as follows:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 30-36
  :dedent: 2

Attempting to initialize a ``param`` with a value not known to the
compiler will generate an error message.  For example, consider the
following initializers, which were legal in ``const`` declarations:

.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 39-40
  :dedent: 4

As ``param`` initializers, they result in the following error messages
since the value of *n* cannot be known at compile-time:

.. literalinclude:: examples/users-guide/base/constParam-error.good
  :language: text
  :lines: 2-3

Despite this restriction, Chapel knows how to perform certain
compile-time computations on ``param`` values, permitting them to be
initialized using expressions other than simple literal values:


.. literalinclude:: examples/users-guide/base/constParam.chpl
  :language: chapel
  :lines: 45-51

Specifically, procedures and operators that are declared to return
``param`` values are executed at compile-time and can be used to
initialize ``param`` constants.  A subsequent section will cover the
creation of such functions.

Unlike ``const`` declarations, ``param`` values are only supported for
certain types that the compiler knows how to compute with: booleans,
signed and unsigned integers, ``real`` and ``imag`` floating point
values, enumerated types, and strings.  Literal values of these types
can be considered to be anonymous ``param`` values.  In the future, we
expect to expand this list to include additional types, such as complex
values, ranges, domains, arrays, records, classes, and unions.


When to use ``const`` vs. ``param``
-----------------------------------

Generally speaking, programmers are encouraged to use a ``param``
whenever they are computing with a value that's known at compile-time,
is one of the supported ``param`` types, and one or more of the
following apply:

* the value is used in a place where the language requires a ``param``
  (e.g., indexing into a heterogeneous tuple),

* the value is useful for enabling (or forcing) a computation to occur
  at compile-time,

* the generated code should be specialized by the ``param`` value.

Programmers are encouraged to use ``const`` to declare any other
immutable variable, such as:

* constants whose values can't or won't be known at compile-time,

* constants for which compile-time specialization is neither required
  nor beneficial.

The ways in which a ``param`` value can affect code specialization or
optimization may seem vague now, but should become clearer as we see
how ``param`` values are used to create generic types and functions,
to unroll loops, etc.

While compilers can often determine that a symbol is ``const`` or
``param`` by analyzing its uses, Chapel programmers are encouraged to
declare variables as constants whenever possible in order to maximize
the amount of semantic information available to the compiler and to
people reading the code.  Such declarations can also help programmers
avoid simple mistakes by having the compiler's semantic checks ensure
that values which are intended to be constant are not accidentally
re-assigned.
