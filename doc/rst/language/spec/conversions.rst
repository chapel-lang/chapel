.. default-domain:: chpl

.. _Chapter-Conversions:

===========
Conversions
===========

A *conversion* converts an expression of one type to another type,
possibly producing a new value. In certain cases noted below, the source
expression can be a type expression. We refer to these two types as the
*source* and *target* types. Conversions can be either
implicit (:ref:`Implicit_Conversions`) or
explicit (:ref:`Explicit_Conversions`).

.. _Implicit_Conversions:

Implicit Conversions
--------------------

An *implicit conversion* is a conversion that occurs implicitly—that
is, without an explicit operation within the program. Implicit conversions
fall into the following categories:

 * implicit conversions for initialization and assignment
   (:ref:`Implicit_Conversion_Init_Assign`)
 * implicit conversions for function calls
   (:ref:`Implicit_Conversion_Call`)
 * implicit conversions for conditionals
   (:ref:`Implicit_Conversion_Conditionals`)

If an implicit conversion for a function call is allowed from type ``T1`` to
type ``T2`` then implicit conversion for initialization and assignment is
allowed.

In addition, an implicit conversion from a type to the same type is
allowed for any type. Such a conversion does not change the value of the
expression.

Implicit conversion is not transitive. That is, if an implicit
conversion is allowed from type ``T1`` to ``T2`` and from ``T2`` to
``T3``, that by itself does not allow an implicit conversion from ``T1``
to ``T3``.

Implicit conversion for function calls, initialization, and assignment
are allowed between the following source and target types, as defined in
the referenced subsections:

-  boolean and numeric types
   types (:ref:`Implicit_NumBool_Conversions`),

-  numeric types in the special case when the expression’s value is a
   compile-time
   constant (:ref:`Implicit_Compile_Time_Constant_Conversions`),

-  class types (:ref:`Implicit_Class_Conversions`), and

-  when the source type is a subtype of the target type (including when
   the target type is generic)
   (:ref:`Subtype_Arg_Conversions`)

Additionally, implicit conversions for initialization and assignment can
be defined for record types, as specified in
:ref:`Implicit_Conversion_Init_Assign`.

.. _Implicit_NumBool_Conversions:

Implicit Numeric and Bool Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Implicit conversions among numeric types are allowed when all values
representable in the source type can also be represented in the target
type, retaining their full precision. In addition, implicit conversions
are permitted from ``int(s)`` and ``uint(s)`` values to ``real(t)`` and
``complex(2*t)``, for any widths ``s`` and ``t``, even though these cases
may result in a loss of precision.

   *Rationale*.

   We allow these additional conversions because they provide an
   important convenience for application programmers who want to mix
   integral and floating point values in mathematical expressions, and
   for computing using values using a specific bit-width. For these
   benefits, the loss of precision seemed like a reasonable tradeoff,
   particularly given that floating point types are approximate by
   nature.

Signed integral types ``int(s)`` can implicitly convert to ``uint(s)``
where ``s <= t``.

   *Rationale*.

   We allow these conversions to avoid the situation that something
   similar to a binary operator produces surprising results when mixing
   ``int`` and ``uint`` types. In particular, without this rule, the
   ``plus`` function defined below would surprisingly produce values of a
   different width or a different kind:

   .. code-block:: chapel

     proc plus(a: int(32), b: int(32)) : int(32) { ... }
     proc plus(a: int(64), b: int(64)) : int(64) { ... }
     proc plus(a: uint(32), b: uint(32)) : uint(32) { ... }
     proc plus(a: uint(32), b: uint(32)) : uint(32) { ... }
     proc plus(a: real(64), b: real(64)) : real(64) { ... }

     var myInt32: int(32);
     var myUint32: uint(32);
     plus(myInt32, myUint32); // calls 'uint(32)' version, but
                              // without int->uint implicit conversion,
                              // would call the 'int(64)' version
     var myInt64: int(64);
     var myUint64: uint(64);
     plus(myInt64, myUint64); // calls 'uint(64)' version, but
                              // without int->uint implicit conversion,
                              // would call the 'real(64)' version

   While implicitly converting an ``int`` to a ``uint`` can lead to
   surprising behavior, this behavior is less problematic than the
   surprising behavior that comes from the above scenario.

A ``bool`` can be implicitly converted to any integral type by
representing ``false`` as 0 and ``true`` as 1.

   *Rationale*.

   We disallow implicit conversion of a ``bool`` to a real, imaginary,
   or complex type because we expect that such conversions are most
   likely to be an unintended mistake by the programmer.
   Marking such cases as errors will draw the programmer’s attention
   to the issue, and if such a conversion is actually desired, a cast
   can be used (see :ref:`Explicit_Conversions`).

Legal implicit conversions with numeric and boolean types may thus be
summarized as follows:

==================== ================= ================ ================= ================= ====================
\                                                **Destination Type**
-------------------- -------------------------------------------------------------------------------------------
**Source Type**      uint(\ :math:`t`) int(\ :math:`t`) real(\ :math:`t`) imag(\ :math:`t`) complex(\ :math:`t`)
bool                 all :math:`t`     all :math:`t`
uint(\ :math:`s`)    :math:`s \le t`   :math:`s < t`    all :math:`s,t`                     all :math:`s,t`
int(\ :math:`s`)     :math:`s \le t`   :math:`s \le t`  all :math:`s,t`                     all :math:`s,t`
real(\ :math:`s`)                                       :math:`s \le t`                     :math:`s \le t/2`
imag(\ :math:`s`)                                                         :math:`s \le t`   :math:`s \le t/2`
complex(\ :math:`s`)                                                                        :math:`s \le t`
==================== ================= ================ ================= ================= ====================


.. _Implicit_Compile_Time_Constant_Conversions:

Implicit Compile-Time Constant Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A ``param`` of numeric type can be implicitly converted to another numeric
type in some cases if the ``param`` value can be represented exactly by
the target type. In particular:

 * ``param`` ``int(s)`` and ``uint(s)`` values that are exactly
   representable in the target type can implicit convert to ``int(t)``
   and ``uint(t)`` regardless of the values of ``s`` and ``t``.
 * ``param`` ``real(s)`` that is exactly representable in the target
   type can implicitly convert to ``real(t)`` or to ``complex(t)``
   regardless of the values of ``s`` and ``t``.
 * ``param`` ``imag(s)`` that is exactly representable in the target
   type can implicitly convert to ``imag(t)`` or to ``complex(t)``
   regardless of the values of ``s`` and ``t``.
 * ``param`` ``complex(s)`` that is exactly representable in the target
   type can implicitly convert to ``complex(t)``.

As with the implicit numeric conversions, integral ``param`` values can
implicitly convert:

 * to ``uint`` of matching or greater size or to ``real``
 * or, to ``complex`` of any size.

.. _Implicit_Class_Conversions:

Implicit Class Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~

An expression of class type can be implicitly converted to:
 * to a parent class type,
 * to a nilable type, or
 * to the borrow type.

Any combination of these three conversions is allowed.

The value ``nil`` can be implicitly converted to any nilable class type.

Conversion to a parent class type or to a nilable type is a subtype
conversion and is discussed in the next section
(:ref:`Subtype_Arg_Conversions`).

Class types can be converted to the corresponding ``borrowed`` type. For
example, ``owned C`` can be implicitly converted to ``borrowed C``, and
``shared C?`` can be implicitly converted to ``borrowed C?``. This
coercion is equivalent to calling the ``.borrow()`` method.
See :ref:`Class_Lifetime_and_Borrows`.  For example:

   *Example (implicit-conversion-to-borrow.chpl)*.

   .. code-block:: chapel

      class C { }
      var c:owned C = new owned C();

      proc f(arg: borrowed C) { }
      f(c); // equivalent to f(c.borrow())


.. _Subtype:
.. _Subtype_Arg_Conversions:
.. _Implicit_Type_Arg_Conversions:
.. _Implicit_Generic_Type_Conversions:

Implicit Subtype Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An implicit subtype conversion is allowed when the source type is a
subtype of the target type.

Given any two types ``T1`` and ``T2``, the type ``T1`` is considered to be a
subtype of a type ``T2`` if:

 * ``T2`` is a generic type (:ref:`Generic_Types`) and ``T1`` is an
   instantiation that type
 * ``T1`` is a class type that inherits from the class ``T2``
   (:ref:`Inheritance`)
 * ``T1`` is a non-nilable class type (e.g. ``borrowed C``) and ``T2`` is
   the nilable version of the same class type (e.g. ``borrowed C?``)
   (:ref:`Nilable_Classes`)
 * or a combination of the above.

The below examples use :proc:`isSubtype <Types.isSubtype>` to demonstrate
when one type is a subtype of another.

   *Example (not-a-subtype.chpl)*

   The following code snippet demonstrates that ``int(8)`` is not a
   subtype of ``int``. Note that, even though an ``int(8)`` value can be
   implicitly converted to ``int``, ``int(8)`` is not a subtype of
   ``int``.

   .. BLOCK-test-chapelpre

      param x =

   .. code-block:: chapel

      isSubtype(int(8), int); // evaluates to false

   .. BLOCK-test-chapelpost

      writeln(x);

   .. BLOCK-test-chapeloutput

      false

   *Example (subtype-int8-integral.chpl)*

   However, ``int(8)`` is a subtype of the generic type ``integral``
   according to the first rule above (:ref:`Built_in_Generic_types`).

   .. BLOCK-test-chapelpre

      param x =

   .. code-block:: chapel

      isSubtype(int(8), integral); // evaluates to true

   .. BLOCK-test-chapelpost

      writeln(x);

   .. BLOCK-test-chapeloutput

      true

   *Example (subtype-pass-int8-integral.chpl)*

   Since ``int(8)`` is a subtype of ``integral``, the type ``int(8)`` can
   be passed to the type argument ``type t: integral``
   (:ref:`Legal_Argument_Mapping`). As a result the following program
   will compile:

   .. code-block:: chapel

      proc f(type t: integral) { }
      f(int(8));

   *Example (subtype-parent-class.chpl)*

   This example demonstrates that ``ChildClass`` is a subtype of
   ``ParentClass``.

   .. code-block:: chapel

     class ParentClass { }
     class ChildClass : ParentClass { }

     writeln(isSubtype(ChildClass, ParentClass)); // outputs true
     writeln(isSubtype(borrowed ChildClass, borrowed ParentClass)); // outputs true

     proc f(type t: ParentClass) { }
     f(ChildClass); // implicit subtype conversion

     proc g(type t: borrowed ParentClass) { }
     g(borrowed ChildClass); // implicit subtype conversion

     // The implicit subtype conversion can also apply to non-type arguments:
     proc h(in arg: owned ParentClass) { }
     h(new owned ChildClass()); // implicit subtype conversion

   .. BLOCK-test-chapeloutput

     true
     true

   *Example (subtype-nilable.chpl)*.

   This example shows that a non-nilable class type is a subtype of a
   nilable class type with the same management.

   .. code-block:: chapel

      class C { }

      writeln(isSubtype(C, C?)); // outputs true
      writeln(isSubtype(owned C, owned C?)); // outputs true

   .. BLOCK-test-chapeloutput

     true
     true

   *Example (subtype-three.chpl)*.

   This example demonstrates a combination of all three rules. Note that
   ``ParentClass`` indicates a generic memory management strategy
   (:ref:`Class_Types`).

   .. code-block:: chapel

     class ParentClass { }
     class ChildClass : ParentClass { }

     writeln(isSubtype(ChildClass, ParentClass?)); // outputs true

     proc f(type t: ParentClass?) { }
     f(ChildClass); // uses implicit subtype conversion

     proc g(in arg: ParentClass?) { }
     g(new owned ChildClass()); // uses implicit subtype conversion

   .. BLOCK-test-chapeloutput

     true

.. _Implicit_Conversion_Init_Assign:

Implicit Conversions for Initialization and Assignment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An implicit conversion for initialization or assignment occurs at each of
the following program locations:

-  In an assignment, the expression on the right-hand side of the
   assignment is converted to the type of the expression on the
   left-hand side of the assignment.

-  In a variable or field declaration that is not a ref variable,
   the initializing expression is converted to the type of the variable
   or field. The initializing expression is the right-hand side of the
   ``=`` in the declaration, if present, or in the field initialization
   statement in an initializer.

-  The return or yield expression within a function without a ``ref`` or
   ``const ref`` return intent is converted to the return type of that
   function.

-  For a call to a function with a formal argument with ``out`` or
   ``inout`` intent. The value of the formal argument is converted to the
   type of the corresponding actual argument when setting that actual
   with assignment or initialization (see :ref:`The_Out_Intent`).

Implicit conversions for initialization or assignment are allowed between
numeric and boolean types (:ref:`Implicit_NumBool_Conversions`), numeric
types in the special case when the expression’s value is a compile-time
constant (:ref:`Implicit_Compile_Time_Constant_Conversions`), class types
(:ref:`Implicit_Class_Conversions`), and for generic target types
(:ref:`Subtype_Arg_Conversions`).

In addition, these implicit conversions can be defined for record types
by implementing ``init=`` and possibly the ``=`` operator between two
types as described in :ref:`Advanced_Copy_Initialization` and
:ref:`Function_Overloading`.  ``init=`` will be called for initialization
as described in :ref:`Split_Initialization` and the ``=`` operator will
be invoked for other uses of assignment.

In the event that an ``=`` overload is provided to support assignment
between two types, the compiler will check that a corresponding ``init=``
also exists and emit an error if not.  Additionally, if ``init=`` is
provided to initialize one type from another, the corresponding ``:``
overload must also exist. See also :ref:`Explicit_Conversions` for more
information on the ``:`` operator. It is possible to provide ``:``
without ``init=`` or to provide ``init=`` without ``=``.

   *Example (implementing-assignment.chpl)*

   Suppose that we have defined a record type to wrap an integer:

   .. code-block:: chapel

      record myInteger {
        var intValue: int;
      }

   We might wish to support assignments setting a ``myInteger`` from
   ``int``. In that event, we can provide the following functions:

   .. code-block:: chapel

      operator =(ref lhs: myInteger, rhs: int) {
        lhs.intValue = rhs;
      }
      proc myInteger.init=(rhs: int) {
        this.intValue = rhs;
      }
      operator :(from: int, type toType: myInteger) {
        var tmp: myInteger = from; // invoke the init= above
        return tmp;
      }

   Since we defined ``operator =``, it is necessary to also define
   ``init=`` and ``operator :`` between these types.

   We can invoke these functions like this:

   .. code-block:: chapel

      var a = 1:myInteger;  // cast -- invokes operator :

      var b: myInteger = 2; // initialization -- invokes init=

      var c: myInteger;
      c = 3;                // split-initialization -- invokes init=

      var d = new myInteger();
      d = 4;                // assignment -- invokes operator =

   .. BLOCK-test-chapelnoprint

      writeln("a is ", a, " : ", a.type:string);
      writeln("b is ", b, " : ", b.type:string);
      writeln("c is ", c, " : ", c.type:string);
      writeln("d is ", d, " : ", d.type:string);

   .. BLOCK-test-chapeloutput

      a is (intValue = 1) : myInteger
      b is (intValue = 2) : myInteger
      c is (intValue = 3) : myInteger
      d is (intValue = 4) : myInteger

.. _Implicit_Conversion_Call:

Implicit Conversions for Function Calls
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An implicit conversion for a function call - also called a *coercion* -
occurs when the actual argument of a function call is converted to the
type of the corresponding formal argument, if the formal’s intent is
``param``, ``in``, ``const in``, or an abstract intent
(:ref:`Abstract_Intents`) with the semantics of ``in`` or ``const in``.

Implicit conversions for function calls are allowed between numeric
and boolean types (:ref:`Implicit_NumBool_Conversions`), numeric types
in the special case when the expression’s value is a compile-time
constant (:ref:`Implicit_Compile_Time_Constant_Conversions`), class
types (:ref:`Implicit_Class_Conversions`), and for generic target
types (:ref:`Subtype_Arg_Conversions`).

Additionally, an implicit conversion for a function call occurs when the
actual type is a subtype of the formal type. This rule applies to ``in``,
``const in``, ``const ref``, and ``type`` intent formals and includes
generic formal types. See :ref:`Subtype_Arg_Conversions`.

Implicit conversions are not applied for actual arguments passed to
``ref`` formal arguments.

   *Open issue*.

   For the ``const ref`` intent, subtype conversions can be allowed while
   keeping the ``const ref`` formal referring to the original actual
   argument's value. However, this feature is still under discussion.

   *Open issue*.

   Should Chapel allow user-defined implicit conversions for function
   calls?  If so, how would the user define them?

.. _Implicit_Conversion_Conditionals:
.. _Implicit_Statement_Bool_Conversions:

Implicit Conversions for Conditionals
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An implicit conversion for a conditional occurs for the condition of:

  - a conditional expression,
  - a conditional statement,
  - a while-do loop, or
  - a do-while loop.

In such a condition, the following implicit conversions to ``bool`` are
supported:

-  An expression of integral type is taken to be ``false`` if it is ``0`` and
   is ``true`` otherwise.

-  An expression of a class type is taken to be ``false`` if it is ``nil`` and
   is ``true`` otherwise.

Other standard types also allow implicit conversion for conditionals as
indicated in their documentation.

   *Open issue*.

   Should Chapel allow user-defined implicit conversions for
   conditionals? If so, how would the user define them?


.. _Explicit_Conversions:

Explicit Conversions
--------------------

Explicit conversions require a cast in the code. Casts are defined
in :ref:`Casts`. Explicit conversions are supported between more
types than implicit conversions, but not between all types.

The allowed explicit conversions are described in the following sections:

 * conversions among primitive numeric and bool types (see
   :ref:`Explicit_Numeric_Conversions`)
 * tuple to complex (see :ref:`Explicit_Tuple_to_Complex_Conversion`)
 * enumerated types (see :ref:`Explicit_Enumeration_Conversions`)
 * class conversions (see :ref:`Explicit_Class_Conversions`)
 * range conversions (see :ref:`Explicit_Range_Conversions`)
 * domain conversions (see :ref:`Explicit_Domain_Conversions`)
 * string to bytes conversions (see
   :ref:`Explicit_String_to_Bytes_Conversions`)
 * type to string conversions (see
   :ref:`Explicit_Type_to_String_Conversions`)
 * user-defined explicit conversions (see :ref:`User_Defined_Casts`).

The available explicit conversions are a superset of the available
implicit conversions for initialization and assignment
(:ref:`Implicit_Conversion_Init_Assign`), which, in turn, are a superset
of the implicit conversions for function calls.  As a result, the
implicit conversions described in :ref:`Implicit_Conversions` are also
available as explicit conversions.

An explicit conversion from a type to the same type is allowed for any
type. Such a conversion does not change the value of the expression.


.. _Explicit_Numeric_Conversions:

Explicit Numeric Conversions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Explicit conversions are allowed from ``bool`` or any numeric type to
``bytes`` or ``string``, and vice-versa.  When converting to ``bytes``
or ``string`` the result will hold the string ``true`` or ``false``
for a ``bool``, or a representation of the expression's numerical
value in other cases.  When converting from a ``string`` or ``bytes``,
the reverse occurs, converting the represented value into a numerical
or ``bool`` value.  If the ``string``/``bytes`` does not represent a
legal value of the given type, an ``IllegalArgumentError`` is thrown.

When a ``bool`` is converted to an ``int`` or ``uint``, ``false``
converts to the value 0 and ``true`` to 1.

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
is chosen. [1]_

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

When converting from an enum to a ``bool``, the value is first
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
will throw a ``ClassCastError``. If the cast fails and the destination
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


.. _User_Defined_Casts:

User-Defined Casts
~~~~~~~~~~~~~~~~~~

An explicit conversion can be defined by implementing ``operator :`` (see
also :ref:`Function_Overloading`). An ``operator :`` should accept two
arguments: the value to convert and the type to convert it to.

   *Example (implementing-cast.chpl)*

   Suppose that we have defined a record type to wrap an integer:

   .. code-block:: chapel

      record myInteger {
        var intValue: int;
      }

   We might wish to support casts from ``myInteger`` to ``int``. In that
   event, we can provide this cast operator:

   .. code-block:: chapel

      operator :(from: myInteger, type toType: int) {
        return from.intValue;
      }

   and we can invoke it using the cast syntax like this:

   .. code-block:: chapel

      var x = new myInteger(1);
      var y = x:int;

   .. BLOCK-test-chapelnoprint

      writeln("x is ", x, " : ", x.type:string);
      writeln("y is ", y, " : ", y.type:string);

   .. BLOCK-test-chapeloutput

      x is (intValue = 1) : myInteger
      y is 1 : int(64)



.. [1]
   When converting to a smaller real type, a loss of precision is
   *expected*. Therefore, there is no reason to produce a run-time
   diagnostic.
