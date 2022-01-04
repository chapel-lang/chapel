:title: Users Guide: Variable Declarations

.. _ug-vars:

Variable Declarations
=====================

Chapel variables are declared using the **var** keyword.  As an
example, the following statement declares a variable named *x* of
integer type (``int``) and initializes it to 42:

.. literalinclude:: examples/users-guide/base/intVarTypeInit.chpl
  :caption:
  :language: chapel
  :lines: 1

We can verify its value by printing it out using ``writeln()``:

.. literalinclude:: examples/users-guide/base/intVarTypeInit.chpl
  :language: chapel
  :lines: 2

Compiling and running this program results in:

.. literalinclude:: examples/users-guide/base/intVarTypeInit.good
  :language: text

As we will see in the following sections, a variable declaration may
omit its initial value or its type, but not both.


Auto-initialized Variables
--------------------------

If a variable declaration does not contain an initial value, the
compiler will initialize the variable to a default value based on its
type.  For example, the default value for integers is zero, so if we
had instead declared *x* as:

.. literalinclude:: examples/users-guide/base/intVarDefaultInit.chpl
  :caption:
  :language: chapel
  :lines: 1-2

We would get:

.. literalinclude:: examples/users-guide/base/intVarDefaultInit.good
  :language: text


Type-inferred Variables
-----------------------

Alternatively, a variable declaration with an initial value may omit its
type specification.  In this case, the compiler will infer the
variable's type to be the same as its initialization expression's.
Thus, we could have written the original program simply as:

.. literalinclude:: examples/users-guide/base/intVarInferType.chpl
  :caption: 
  :language: chapel
  :lines: 1-2

In this case, the compiler knows that 42 is a value of integer type
(``int``), therefore it infers that the type of *x* is also ``int``.
Running this program, once again produces:

.. literalinclude:: examples/users-guide/base/intVarInferType.good
  :language: text

In practice, this style of coding is often used for brevity, resulting
in a scripting-like feel.  Note, however, that unlike most scripting
languages, a Chapel variable's type is determined at compile-time and
fixed for its lifetime.  Thus, in each of these three declaration
styles, *x* will be an integer for its lifetime and cannot be used to
store values other than integers.


Compound Variable Declarations
------------------------------

The **var** keyword can also be used to create more than one variable
at a time.  For example, the following statement will create three
integer variables, *a*, *b*, and *c*, all initialized to 42:

.. literalinclude:: examples/users-guide/base/compoundVarDecls.chpl
  :caption: 
  :language: chapel
  :lines: 1

As before, initialization and type information may be omitted.  For
example, the following statement creates three integer variables, *i*,
*j*, and *k*, all default-initialized to zero:

.. literalinclude:: examples/users-guide/base/compoundVarDecls.chpl
  :language: chapel
  :lines: 2

Similarly, the following declaration creates three variables, *x*,
*y*, and *z*, all inferred to be of type ``int`` due to being
initialized with the integer 42:

.. literalinclude:: examples/users-guide/base/compoundVarDecls.chpl
  :language: chapel
  :lines: 3

Finally, a single variable declaration statement may mix several type
and initializer expressions, separating them by commas.  For example,
the following statement declares *a*, *b*, and *c* to be integer
variables initialized to 42; *i*, *j*, and *k* to be integer variables
default-initialized to zero; *s* and *t* to be string variables
initialized to "hi"; and *x*, *y*, and *z* to be inferred integer
variables initialized to 42:

.. literalinclude:: examples/users-guide/base/compoundVarDecls2.chpl
  :caption:
  :language: chapel
  :lines: 1-4


(Note that whitespace doesn't matter in Chapel, so the use of
linefeeds in this example is simply for readability and has no
practical impact).

The rule of thumb is that any variable which has neither a type
specification nor an initial value will share the type and/or
initial value of the next variable which has one or both.
Thus, in the example above, *a* and *b* share *c*'s type and
initial value; *i* and *j* share *k*'s type; *s* shares *t*'s
initial value; and *x* and *y* share *z*'s initial value.

Needless to say, the last variable in a compound declaration must have
a type and/or initial value.
