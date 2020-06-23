.. _Chapter-Conversions:

Conversions
===========

A *conversion* converts an expression of one type to another type,
possibly changing its value. In certain cases noted below the source
expression can be a type expression. We refer to these two types the
*source* and *target* types. Conversions can be either
implicit (:ref:`Implicit_Conversions`) or
explicit (:ref:`Explicit_Conversions`).

.. _Implicit_Conversions:

Implicit Conversions
--------------------

An *implicit conversion* is a conversion that occurs implicitly, that
is, not due to an explicit specification in the program. Implicit
conversions occur at the locations in the program listed below. Each
location determines the target type. The source and target types of an
implicit conversion must be allowed. They determine whether and how the
expression’s value changes.

An implicit conversion occurs at each of the following program
locations:

-  In an assignment, the expression on the right-hand side of the
   assignment is converted to the type of the variable or another lvalue
   on the left-hand side of the assignment.

-  In a variable or field declaration, the initializing expression is
   converted to the type of the variable or field. The initializing
   expression is the r.h.s. of the ``=`` in the declaration, if present,
   or in the field initialization statement in an initializer.

-  The actual argument of a function call or an operator is converted to
   the type of the corresponding formal argument, if the formal’s intent
   is ``param``, ``in``, ``const in``, or an abstract intent
   (:ref:`Abstract_Intents`) with the semantics of ``in`` or
   ``const in``.

-  The actual type argument of a function call or an operator is
   converted to the corresponding formal argument of the ``type`` intent
   or the ``this`` formal of a type method. See
   :ref:`Implicit_Type_Arg_Conversions`.

-  If the formal argument’s intent is ``out``, the formal argument is
   converted to the type of the corresponding actual argument upon
   function return.

-  The return or yield expression within a function without a ``ref``
   return intent is converted to the return type of that function.

-  The condition of a conditional expression, conditional statement,
   while-do or do-while loop statement is converted to the boolean type.
   See :ref:`Implicit_Statement_Bool_Conversions`.

Implicit conversions are not applied for actual arguments passed to
``ref`` or ``const ref`` formal arguments.

Implicit conversions *are allowed* between the following source and
target types, as defined in the referenced subsections:

-  numeric and boolean
   types (:ref:`Implicit_NumBool_Conversions`),

-  numeric types in the special case when the expression’s value is a
   compile-time
   constant (:ref:`Implicit_Compile_Time_Constant_Conversions`),
   and

-  class types (:ref:`Implicit_Class_Conversions`),

-  class and generic types in certain cases
   (:ref:`Implicit_Type_Arg_Conversions`)

-  from an integral or class type to ``bool`` in certain
   cases (:ref:`Implicit_Statement_Bool_Conversions`).

-  generic target types
   (:ref:`Implicit_Generic_Type_Conversions`)

In addition, an implicit conversion from a type to the same type is
allowed for any type. Such conversion does not change the value of the
expression.

Implicit conversion is not transitive. That is, if an implicit
conversion is allowed from type ``T1`` to ``T2`` and from ``T2`` to
``T3``, that by itself does not allow an implicit conversion from ``T1``
to ``T3``.

.. _Implicit_NumBool_Conversions:

Implicit Numeric and Bool Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Implicit conversions among numeric types are allowed when all values
representable in the source type can also be represented in the target
type, retaining their full precision. In addition, implicit conversions
from types ``int(64)`` and ``uint(64)`` to types ``real(64)`` and
``complex(128)`` are allowed, even though they may result in a loss of
precision.

   *Rationale*.

   We allow these additional conversions because they are an important
   convenience for application programmers. Therefore we are willing to
   lose precision in these cases. The largest real and complex types are
   chosen to retain precision as often as as possible.

Any boolean type can be implicitly converted to any other boolean type,
retaining the boolean value. Any boolean type can be implicitly
converted to any integral type by representing ``false`` as 0 and
``true`` as 1, except (if applicable) a boolean cannot be converted to
``int(1)``.

   *Rationale*.

   We disallow implicit conversion of a boolean to a real, imaginary, or
   complex type because of the following. We expect that the cases where
   such a conversion is needed will more likely be unintended by the
   programmer. Marking those cases as errors will draw the programmer’s
   attention. If such a conversion is actually desired, a cast
   :ref:`Explicit_Conversions` can be inserted.

Legal implicit conversions with numeric and boolean types may thus be
tabulated as follows:

==================== ================= ================= ============================== ======================= ================= =========================
\                                                                                                                                
Source Type          bool(\ :math:`t`) uint(\ :math:`t`) int(\ :math:`t`)               real(\ :math:`t`)       imag(\ :math:`t`) complex(\ :math:`t`)
\                                                                                                                                
bool(\ :math:`s`)    all :math:`s,t`   all :math:`s,t`   all :math:`s`; :math:`2 \le t`                                          
uint(\ :math:`s`)                      :math:`s \le t`   :math:`s < t`                  :math:`s \le mant(t)`                     :math:`s \le mant(t/2)`
uint(64)                                                                                real(64)                                  complex(128)
int(\ :math:`s`)                                         :math:`s \le t`                :math:`s \le mant(t)+1`                   :math:`s \le mant(t/2)+1`
int(64)                                                                                 real(64)                                  complex(128)
real(\ :math:`s`)                                                                       :math:`s \le t`                           :math:`s \le t/2`
imag(\ :math:`s`)                                                                                               :math:`s \le t`   :math:`s \le t/2`
complex(\ :math:`s`)                                                                                                              :math:`s \le t`
==================== ================= ================= ============================== ======================= ================= =========================

Here, :math:`mant(i)` is the number of bits in the (unsigned) mantissa
of the :math:`i`-bit floating-point type. [1]_ Conversions for the
default integral and real types (``uint``, ``complex``, etc.) are the
same as for their explicitly-sized counterparts.

.. _Implicit_Compile_Time_Constant_Conversions:

Implicit Compile-Time Constant Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A parameter of numeric type can be implicitly converted to any other
numeric type if the value of the parameter can be represented exactly by
the target type. This rule does not allow conversions from ``real`` to
``imag``, or from ``complex`` to a non-complex type. It does allow
conversions from ``real`` or ``imag`` to ``complex``.

.. _Implicit_Class_Conversions:

Implicit Class Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of class type can be implicitly converted to the borrow
type; to a nilable type; or to a parent class type. The value ``nil``
can be implicitly converted to any nilable class type.

First, class types can be converted to the corresponding ``borrowed``
type. For example, ``owned C`` can be implicitly converted to
``borrowed C``, and ``shared C?`` can be implicitly converted to
``borrowed C?``. This coercion is equivalent to calling the
``.borrow()`` method. See :ref:`Class_Lifetime_and_Borrows`.
For example:

   *Example (implicit-conversion-to-borrow.chpl)*.

   

   .. code-block:: chapel

      class C { }
      var c:owned C = new owned C();

      proc f(arg: borrowed C) { }
      f(c); // equivalent to f(c.borrow())

Second, an expression of non-nilable class type can be implicitly
converted to the nilable class type. Continuing the above example:

   *Example (implicit-conversion-to-nilable.chpl)*.

   

   .. BLOCK-test-chapelpre

      class C { }
      var c:owned C = new owned C();

   

   .. code-block:: chapel

      var b:borrowed C = c.borrow();

      proc g(arg: borrowed C?) { }
      g(b); // equivalent to g(b:borrowed C?)

Third, an implicit conversion from class type ``D`` to another class
type ``C`` is allowed when ``D`` is a subclass of ``C``.

Any combination of these three conversions is allowed.

.. _Implicit_Type_Arg_Conversions:

Implicit Type Argument Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An implicit type argument conversion applies only when a type actual is
passed to a formal with the ``type`` intent. This includes the ``this``
formal of a type method. In this case, a subset of Implicit Class
Conversions (:ref:`Implicit_Class_Conversions`) applies, in
addition to Implicit Conversions To Generic Types
(:ref:`Implicit_Generic_Type_Conversions`).

   .. note::
      
      *Future:*

      The details are forthcoming.

.. _Implicit_Statement_Bool_Conversions:

Implicit Statement Bool Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the condition of an if-statement, while-loop, and do-while-loop, the
following implicit conversions to ``bool`` are supported:

-  An expression of integral type is taken to be false if it is zero and
   is true otherwise.

-  An expression of a class type is taken to be false if it is nil and
   is true otherwise.

.. _Implicit_Generic_Type_Conversions:

Implicit Conversions To Generic Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When the target type ``T`` is generic (:ref:`Generic_Types`), an
implicit conversion is allowed when there is an instantiation of this
type such that an implicit conversion is allowed between the source type
and that instantiation by another rule in this section.

That instantiation is taken to be the instantiated type of the variable,
field, formal argument, or the return type whose declared type is the
generic type ``T``.

The conversions in this subsection apply when the source is either an
expression or a type expression.

.. _Explicit_Conversions:

Explicit Conversions
--------------------

Explicit conversions require a cast in the code. Casts are defined
in :ref:`Casts`. Explicit conversions are supported between more
types than implicit conversions, but not between all types.

The explicit conversions are a superset of the implicit conversions. In
addition to the following definitions, an explicit conversion from a
type to the same type is allowed for any type. Such conversion does not
change the value of the expression.

.. _Explicit_Numeric_Conversions:

Explicit Numeric Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Explicit conversions are allowed from any numeric type or boolean to
bytes or string, and vice-versa.

When a ``bool`` is converted to a ``bool``, ``int`` or ``uint`` of equal
or larger size, its value is zero-extended to fit the new
representation. When a ``bool`` is converted to a smaller ``bool``,
``int`` or ``uint``, its most significant bits are truncated (as
appropriate) to fit the new representation.

When a ``int``, ``uint``, or ``real`` is converted to a ``bool``, the
result is ``false`` if the number was equal to 0 and ``true`` otherwise.

When an ``int`` is converted to a larger ``int`` or ``uint``, its value
is sign-extended to fit the new representation. When a ``uint`` is
converted to a larger ``int`` or ``uint``, its value is zero-extended.
When an ``int`` or ``uint`` is converted to an ``int`` or ``uint`` of
the same size, its binary representation is unchanged. When an ``int``
or ``uint`` is converted to a smaller ``int`` or ``uint``, its value is
truncated to fit the new representation.

   .. note::
   
      *Future:*.

      There are several kinds of integer conversion which can result in a
      loss of precision. Currently, the conversions are performed as
      specified, and no error is reported. In the future, we intend to
      improve type checking, so the user can be informed of potential
      precision loss at compile time, and actual precision loss at run
      time. Such cases include: When an ``int`` is converted to a ``uint``
      and the original value is negative; When a ``uint`` is converted to
      an ``int`` and the sign bit of the result is true; When an ``int`` is
      converted to a smaller ``int`` or ``uint`` and any of the truncated
      bits differs from the original sign bit; When a ``uint`` is converted
      to a smaller ``int`` or ``uint`` and any of the truncated bits is
      true;

..

   *Rationale*.

   For integer conversions, the default behavior of a program should be
   to produce a run-time error if there is a loss of precision. Thus,
   cast expressions not only give rise to a value conversion at run
   time, but amount to an assertion that the required precision is
   preserved. Explicit conversion procedures would be available in the
   run-time library so that one can perform explicit conversions that
   result in a loss of precision but do not generate a run-time
   diagnostic.

When converting from a ``real`` type to a larger ``real`` type, the
represented value is preserved. When converting from a ``real`` type to
a smaller ``real`` type, the closest representation in the target type
is chosen. [2]_

When converting to a ``real`` type from an integer type, integer types
smaller than ``int`` are first converted to ``int``. Then, the closest
representation of the converted value in the target type is chosen. The
exact behavior of this conversion is implementation-defined.

When converting from ``real(k)`` to ``complex(2k)``, the original
value is copied into the real part of the result, and the imaginary part
of the result is set to zero. When converting from a ``real(k)`` to a
``complex(j)`` such that ``j > 2k``, the conversion is
performed as if the original value is first converted to
``real(j/2)`` and then to ``j``.

The rules for converting from ``imag`` to ``complex`` are the same as
for converting from real, except that the imaginary part of the result
is set using the input value, and the real part of the result is set to
zero.

.. _Explicit_Tuple_to_Complex_Conversion:

Explicit Tuple to Complex Conversion
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A two-tuple of numerical values may be converted to a ``complex`` value.
If the destination type is ``complex(128)``, each member of the
two-tuple must be convertible to ``real(64)``. If the destination type
is ``complex(64)``, each member of the two-tuple must be convertible to
``real(32)``. The first member of the tuple becomes the real part of the
resulting complex value; the second member of the tuple becomes the
imaginary part of the resulting complex value.

.. _Explicit_Enumeration_Conversions:

Explicit Enumeration Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Explicit conversions are allowed from any enumerated type to ``bytes``
or ``string`` and vice-versa, including ``param`` conversions. For
enumerated types that are either `concrete` or `semi-concrete` (see
:ref:`Enumerated_Types`), conversions are supported from the enum to
any numeric or boolean type, including ``param`` conversions.
Explicit conversions are also supported from integer values back to
concrete or semi-concrete enumerated types.

When converting from an enum to a ``bytes`` or ``string``, the value
becomes the name of the enumerator.

When converting from a ``bytes`` or ``string`` to an enum, the result
is the constant whose name matches the source value. If no matching
value exists, an ``IllegalArgumentError`` is thrown.

For a semi-concrete enumerated type, if a numeric conversion is
attempted for a constant with no underlying integer value, it will
generate a compile-time error for a ``param`` conversion or throw an
``IllegalArgumentError`` otherwise.

When converting from an enum to an integer type, the value is first
converted to the enum's underlying integer type and then to the target
type, following the rules above for converting between integers.

When converting from an enum to a real, imaginary, or complex type,
the value is first converted to the enum's underlying integer type and
then to the target type.

When converting from an enum to a boolean type, the value is first
converted to the enum's underlying integer type. If the result is
zero, the value of the ``bool`` is ``false``; otherwise, it is
``true``.

When converting from an integer value to an enum, the value is
converted to the enum’s underlying integer type and then converted to
the matching symbol.  If no symbol has the given integer value, an
``IllegalArgumentError`` is thrown.


.. _Explicit_Class_Conversions:

Explicit Class Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of static class type ``C`` can be explicitly converted to
a class type ``D`` provided that ``C`` is derived from ``D`` or ``D`` is
derived from ``C``.

When at run time the source expression refers to an instance of ``D`` or
it subclass, its value is not changed. Otherwise, the cast fails and the
result depends on whether or not the destination type is nilable. If the
cast fails and the destination type is not nilable, the cast expression
will throw a ``classCastError``. If the cast fails and the destination
type is nilable, as with ``D?``, then the result will be ``nil``.

An expression of class type can also be converted to a different
nilability with a cast. For conversions from a nilable class type to a
non-nilable class type, the cast will throw a ``NilClassError`` if the
value was actually ``nil``.

In some cases a new variant of a class type needs to be computed that
has different nilability or memory management strategy. Supposing that
``T`` represents a class type, then these casts may compute a new type:

-  ``T:owned`` - new management is ``owned``, nilability from ``T``

-  ``T:shared`` - new management ``shared``, nilability from ``T``

-  ``T:borrowed`` - new management ``borrowed``, nilability from ``T``

-  ``T:unmanaged`` - new management ``unmanaged``, nilability from ``T``

-  ``T:class`` - non-nilable type with specific concrete or generic
   management from ``T``

-  ``T:class?`` - nilable type with specific concrete or generic
   management from ``T``

-  ``T:owned class`` - non-nilable type with ``owned`` management

-  ``T:owned class?`` - nilable type with ``owned`` management

-  ``T:shared class`` - non-nilable type with ``shared`` management

-  ``T:shared class?`` - nilable type with ``shared`` management

-  ``T:borrowed class`` - non-nilable type with ``borrowed`` management

-  ``T:borrowed class?`` - nilable type with ``borrowed`` management

-  ``T:unmanaged class`` - non-nilable type with ``unmanaged``
   management

-  ``T:unmanaged class?`` - nilable type with ``unmanaged`` management

The conversions in this subsection apply when the source is either an
expression or a type expression.

.. _Explicit_Range_Conversions:

Explicit Range Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of stridable range type can be explicitly converted to an
unstridable range type, changing the stride to 1 in the process.

.. _Explicit_Domain_Conversions:

Explicit Domain Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of stridable domain type can be explicitly converted to an
unstridable domain type, changing all strides to 1 in the process.

.. _Explicit_String_to_Bytes_Conversions:

Explicit String to Bytes Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of ``string`` type can be explicitly converted to a
``bytes``. However, the reverse is not possible as a ``bytes`` can
contain arbitrary bytes. Instead, ``bytes.decode()`` method should be
used to produce a ``string`` from a ``bytes``.

.. _Explicit_Type_to_String_Conversions:

Explicit Type to String Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A type expression can be explicitly converted to a ``string``. The
resultant ``string`` is the name of the type.

   *Example (explicit-type-to-string.chpl)*.

   For example: 

   .. code-block:: chapel

      var x: real(64) = 10.0;
      writeln(x.type:string);

   

   .. BLOCK-test-chapeloutput

      real(64)

   This program will print out the string ``"real(64)"``.
   
.. [1]
   For the IEEE 754 format, :math:`mant(32)=24` and :math:`mant(64)=53`.

.. [2]
   When converting to a smaller real type, a loss of precision is
   *expected*. Therefore, there is no reason to produce a run-time
   diagnostic.
