:title: Users Guide: Variable Declarations

Variable Declarations
=====================

Declaring a Variable
--------------------

Chapel variables are declared using the **var** keyword.  As an
example, the following statement declares a variable of integer type
(``int``) named *x* and initializes it to 42:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarTypeInit.chpl
  :language: chapel
  :lines: 1

We can verify its value by printing it out:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarTypeInit.chpl
  :language: chapel
  :lines: 2

Compiling and running this program results in:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarTypeInit.good
  :language: text

As we will see in the next two sections, a variable declaration may
omit its initializer or its type, but not both.


Auto-initialized Variables
--------------------------

If the programmer does not initialize a variable, the compiler will
initialize it to a default value based on its type.  For example, the
default value for integers is zero, so if we had declared *x* as:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarDefaultInit.chpl
  :language: chapel
  :lines: 1

We would see:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarDefaultInit.good
  :language: text


Type-inferred Variables
-----------------------

The programmer may also choose to omit the type specification, in
which case the compiler will infer the variable's type to match the
type of its initialization expression.  Thus, we could have written
the original program simply as:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarInferType.chpl
  :language: chapel
  :lines: 1

Which would produce:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-intVarInferType.good
  :language: text

In this case, the compiler knows that 42 is an integer, therefore it
infers that the type of *x* is ``int``.

In practice, this style of coding is often used for brevity, to
achieve a scripting-like feel.  Note, however, that unlike most
scripting languages, a variable's type is determined at compile-time
and fixed for its lifetime.  Thus, in any of these three declaration
styles, *x* will always be an integer and cannot be used to store
values other than integers.


Compound Variable Declarations
------------------------------

The **var** keyword can also be used to create more than one variable
at a time.  For example, the following statement will create three
integer variables, *a*, *b*, and *c*, all initialized to 42:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-compoundVarDecls.chpl
  :language: chapel
  :lines: 1

As before, initialization and type information may be omitted, so the
following statement creates three integer variables, *i*, *j*, and
*k*, all default-initialized to zero:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-compoundVarDecls.chpl
  :language: chapel
  :lines: 2

Similarly, the following declaration will create three variables, *x*,
*y*, and *z*, all inferred to be of type ``int`` due to being
initialized with the integer 42:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-compoundVarDecls.chpl
  :language: chapel
  :lines: 3

Finally, a single variable declaration statement may mix several type
and initializer expressions, separating them by commas.  For example,
the following statement declares *a*, *b*, and *c* to be integer
variables initialized to 42; *i*, *j*, and *k* to be integer variables
initialized to zero; *s* and *t* to be string variables initialized to
"hi"; and *x*, *y*, and *z* to be inferred integer variables
initialized to 42:

.. literalinclude:: ../../../../../test/release/examples/guide/base/10-compoundVarDecls2.chpl
  :language: chapel
  :lines: 1-4


Note that whitespae doesn't matter in Chapel, so the use of linefeeds
in this example is simply for readability and has no practical impact.
The rule of thumb is that any variable which has neither a type
specification nor an initializer will share any type and/or
initializer expressions of the next variable which has one or both.
Thus, in the example above, *a* and *b* share *c*'s type and
initializer; *i* and *j* share *k*'s type; *s* shares *t*'s
initializer; and *x* and *y* share *z*'s initializer.

Needless to say, the last variable in a declaration must have a type
and/or initalizer.
