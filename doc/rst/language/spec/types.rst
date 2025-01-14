.. default-domain:: chpl

.. index::
   single: types
.. _Chapter-Types:

=====
Types
=====

Chapel is a statically typed language with a rich set of types. These
include a set of predefined primitive types, enumerated types,
structured types (classes, records, unions, tuples), data parallel types
(ranges, domains, arrays), and synchronization types (sync, atomic).

The syntax of a type is as follows:



.. code-block:: syntax

   type-expression:
     primitive-type
     enum-type
     structured-type
     dataparallel-type
     synchronization-type
     lvalue-expression
     if-expression
     unary-expression
     binary-expression
     expression

Many expressions are syntactically allowed as a type; however not all
expressions produce a type. For example, a call to a function is
syntactically allowed as the type of a variable. However it would be an
error for that call to result in a value (rather than a type) in that
context.

Programmers can define their own enumerated types, classes, records,
unions, and type aliases using type declaration statements:



.. code-block:: syntax

   type-declaration-statement:
     enum-declaration-statement
     class-declaration-statement
     record-declaration-statement
     union-declaration-statement
     type-alias-declaration-statement

These statements are defined in Sections :ref:`Enumerated_Types`,
:ref:`Class_Declarations`, :ref:`Record_Declarations`,
:ref:`Union_Declarations`, and :ref:`Type_Aliases`,
respectively.

.. index::
   single: types; primitive
.. _Primitive_Types:

Primitive Types
---------------

The concrete primitive types are: ``void``, ``nothing``, ``bool``,
``int``, ``uint``, ``real``, ``imag``, ``complex``, ``string`` and
``bytes``. They are defined in this section.

In addition, there are several generic primitive types that are
described in :ref:`Built_in_Generic_Types`.

The primitive types are summarized by the following syntax:

.. code-block:: syntax

   primitive-type:
     'void'
     'nothing'
     'bool'
     'int' primitive-type-parameter-part[OPT]
     'uint' primitive-type-parameter-part[OPT]
     'real' primitive-type-parameter-part[OPT]
     'imag' primitive-type-parameter-part[OPT]
     'complex' primitive-type-parameter-part[OPT]
     'string'
     'bytes'
     'enum'
     'record'
     'class'
     'owned'
     'shared'
     'unmanaged'
     'borrowed'

   primitive-type-parameter-part:
     ( integer-parameter-expression )

   integer-parameter-expression:
     expression

If present, the parenthesized ``integer-parameter-expression`` must
evaluate to a compile-time constant of integer type.
See :ref:`Compile-Time_Constants`

   *Open issue*.

   There is an expectation of future support for larger bit width
   primitive types depending on a platform’s native support for those
   types.

.. index::
   single: void
   single: types; void
.. _The_Void_Type:

The Void Type
~~~~~~~~~~~~~

The ``void`` type is used to represent the lack of a value. It is
primarily used to indicate that a function does not return anything.

   *Example (returnVoid.chpl)*.

   For example, the below declares ``f`` to return ``void``:

   .. code-block:: chapel

      proc f() : void { }

   The compiler can infer the return type of ``void`` as well. See
   See :ref:`Return_Types` for more information.

   .. BLOCK-test-chapelpost

      f();
      writeln("done");

   .. BLOCK-test-chapeloutput

      done

It is an error to assign the result of a function that returns ``void``
to a variable.

.. index::
   single: nothing
   single: types; nothing
.. _The_Nothing_type:

The Nothing Type
~~~~~~~~~~~~~~~~

The ``nothing`` type is used to indicate a variable or field that should
be removed by the compiler. The value ``none`` is the only value of type
``nothing``.

The value ``none`` can only be assigned to a variable of type
``nothing``, or to a generic variable that will take on the type
``nothing``. The variable will be removed from the program and have no
representation at run-time.

   *Rationale*.

   The ``nothing`` type can be used to conditionally remove a variable
   or field from the code based on a ``param`` conditional expression.

   *Example (noneNothing.chpl)*.

   The ``nothing`` type and ``none`` values typically come up in a
   generic programming context (see also :ref:`Chapter-Generics`). For
   example, the following program defines a generic function ``g`` that
   can determine if it was called with an integer or with ``none``:

   .. code-block:: chapel

      proc g(arg) {
        if arg.type != nothing {
          writeln(arg);
        }
      }
      g(1);    // outputs 1
      g(none); // does not create output

   .. BLOCK-test-chapeloutput

       1

.. index::
   single: bool
   single: types; bool
.. _The_Bool_Type:

The Bool Type
~~~~~~~~~~~~~

Chapel defines a logical data type designated by the symbol ``bool``
with the two predefined values ``true`` and ``false``. Values of this
boolean type are stored using an implementation-defined number of
bits.

Some statements require expressions of ``bool`` type and Chapel supports
a special conversion of values to ``bool`` type when used in this
context (:ref:`Implicit_Statement_Bool_Conversions`).

Variables of type ``bool`` have a default value of ``false`` if they are
not initialized to something else (see also :ref:`Chapter-Variables`).

   *Example (bools.chpl)*.

   This program demonstrates creating a variable with type ``bool`` and
   setting it to ``true``, and then setting another variable to the
   logical negation of it:

   .. code-block:: chapel

      var x: bool = true;
      var y = !x;
      var z: bool;

   All three variables have type ``bool``.  Note that the types of ``x``
   and ``y`` are optional; the program indicates the type of ``x`` but
   the compiler infers the type of ``y``.  See :ref:`Chapter-Variables`
   for more details. The last variable is initialized to the default
   value of ``bool``, which is ``false`` (see :ref:`Default_Values_For_Types`).

   .. BLOCK-test-chapelpost

      writeln(x);
      writeln(y);
      writeln(z);

   .. BLOCK-test-chapeloutput

      true
      false
      false


.. index::
   single: uint
   single: int
   single: types; uint
   single: types; int
.. _Signed_and_Unsigned_Integral_Types:

Signed and Unsigned Integral Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The integral types can be parameterized by the number of bits used to
represent them. Valid bit-sizes are 8, 16, 32, and 64. The default
signed integral type, ``int``, is a synonym for ``int(64)``; and the
default unsigned integral type, ``uint``, is a synonym for ``uint(64)``.

Variables of integral type have a default value of ``0`` if they are
not initialized to something else (see also :ref:`Chapter-Variables`).

The integral types and their ranges are given in the following table:

============== ==================== ====================
**Type**       **Minimum Value**    **Maximum Value**
============== ==================== ====================
int(8)         -128                 127
uint(8)        0                    255
int(16)        -32768               32767
uint(16)       0                    65535
int(32)        -2147483648          2147483647
uint(32)       0                    4294967295
int(64), int   -9223372036854775808 9223372036854775807
uint(64), uint 0                    18446744073709551615
============== ==================== ====================

Integer literals such as `3` have type ``int``. However, such literals
can implicitly convert to other numeric types that can losslessly store
the value. See :ref:`Implicit_Compile_Time_Constant_Conversions`.
Integer literals can be written in hexadecimal, octal, or binary. See
:ref:`Literals`.

Signed integral types of can implicitly convert to signed integral types
of larger width. Additionally, signed integral types can implicitly
convert to unsigned integral types of the same or larger width. Unsigned
integral types can implicitly convert to both signed and unsigned
integral type of larger width. See :ref:`Implicit_NumBool_Conversions`
for details.

It is possible for overflow to occur with binary operators on integers.
For signed integers, overflow leads to undefined behavior. For unsigned
integers, overflow leads to wrapping since any bits not representable
will be discarded.

   *Example (integers.chpl)*.

   Here, ``x`` is inferred to have type ``int``:

   .. code-block:: chapel

      var x = 1;

   and ``y`` is initialized by converting ``2`` to a ``uint(8)``:

   .. code-block:: chapel

      var y:uint(8) = 2;

   Then, ``z`` is set to an expression that would evaluate to ``257``,
   but that is not representable as a ``uint(8)``, so it results in the
   wrapped value ``1``.

   .. code-block:: chapel

      var z = 255 + y;

   .. BLOCK-test-chapelpost

      writeln("x = ", x, " : ", x.type:string);
      writeln("y = ", y, " : ", y.type:string);
      writeln("z = ", z, " : ", z.type:string);

   .. BLOCK-test-chapeloutput

      x = 1 : int(64)
      y = 2 : uint(8)
      z = 1 : uint(8)



.. index::
   single: real
   single: types; real
   single: types; floating point
.. _Real_Types:

Real Types
~~~~~~~~~~

Unlike integral types, ``real`` types are floating point types that can
store fractional values. Like the integral types, the real types can be
parameterized by the number of bits used to represent them. The default
real type, ``real``, is 64 bits. The real types that are supported are
machine-dependent, but usually include ``real(32)`` (single precision)
and ``real(64)`` (double precision) following the IEEE 754 standard.

Variables of ``real`` type have a default value of ``0.0`` if they are
not initialized to something else (see also :ref:`Chapter-Variables`).

All integral types can implicitly convert to all ``real`` types, and
``real(32)`` can implicitly convert to ``real(64)``. See
:ref:`Implicit_NumBool_Conversions` for details.

``real`` literals such as `5.2` have type ``real``. However, such literals
can implicitly convert to other numeric types that can losslessly store
the value. See :ref:`Implicit_Compile_Time_Constant_Conversions`.
``real`` literals can be written in decimal or hexadecimal and with or
without an exponent (see :ref:`Literals` for details):

 * in decimal without an exponent, e.g. ``5.2``
 * in decimal with an exponent, e.g. ``6.02e23``
 * in hexadecimal without an exponent, e.g. ``0x2.fe``
 * in hexadecimal with a decimal exponent, e.g. ``0x2.fep23``

   *Example (harmonic.chpl)*.

   For example, this program computes the first ``n`` terms of the
   harmonic series.

   First, it defines a ``config const`` to allow setting the value of
   ``n`` on the command line (see :ref:`Variable_Declarations`):

   .. code-block:: chapel

      config const n = 100;

   Next, it declares a ``real`` variable. Since this variable isn't
   initialized, it will be initialized to 0.0:

   .. code-block:: chapel

      var sum:real;

   Then, it loops over the first `n` elements and adds them to the
   sum (see also :ref:`The_For_Loop`):

   .. code-block:: chapel

      for i in 1..n {
        sum += 1.0/i;
      }

   Note that it uses `1.0/i` in order to do a floating point division. If
   it used `1/i`, it would do integer division (rounding towards zero),
   which evaluates to ``0`` for ``i > 1``.

   Finally, it prints out the sum:

   .. code-block:: chapel

      writeln(sum);

   .. BLOCK-test-chapeloutput

      5.18738


.. index::
   single: imaginary
   single: types; imaginary
   single: types; imag
.. _Imaginary_Types:

Imaginary Types
~~~~~~~~~~~~~~~

Imaginary types are floating-point types, and similarly to ``real``
types, they can be parameterized by the number of bits used to
represent them. The default imaginary type, ``imag``, is 64 bits. The
imaginary types that are supported are machine-dependent, but usually
include ``imag(32)`` and ``imag(64)``.

   *Rationale*.

   The imaginary type is included to avoid numeric instabilities and
   under-optimized code stemming from always converting real values to
   complex values with a zero imaginary part.

Imaginary literals can be created by appending ``i`` to a numeric
literal; for example, ``0.6i``. Such literals have type ``imag``.
However, such literals can implicitly convert to other numeric types that
can losslessly store the value. See
:ref:`Implicit_Compile_Time_Constant_Conversions`.  As with ``real``
literals, imaginary literals can be written in decimal or hexadecimal and
with or without an exponent (see :ref:`Literals` for details):

Variables of ``imag`` type have a default value of ``0.0i`` if they are
not initialized to something else (see also :ref:`Chapter-Variables`).

It is possible to convert between a ``real`` value and an ``imag`` value
using an explicit cast (see :ref:`Explicit_Conversions`). Similarly, an
``imag`` value can be cast to a ``real`` value. Such casts preserve the
floating-point value while changing whether or not it is imaginary.

   *Example (imaginary.chpl)*.

   For example, this program creates imaginary numbers in two different
   ways. First, ``a`` is an ``imag`` variable initialized to a literal:

   .. code-block:: chapel

      var a = 0.6i;

   Now, suppose we have a ``real`` value ``s``:

   .. code-block:: chapel

      var s = 10.25;

   We can initialize an ``imag`` variable with the same numeric value,
   but as an imaginary value, with a cast:

   .. code-block:: chapel

      var b = s:imag;
      assert(b == 10.25i);

   .. BLOCK-test-chapelpost

      writeln("a = ", a, " : ", a.type:string);
      writeln("b = ", b, " : ", b.type:string);

   .. BLOCK-test-chapeloutput

      a = 0.6i : imag(64)
      b = 10.25i : imag(64)


.. index::
   single: complex
   single: types; complex
.. _Complex_Types:

Complex Types
~~~~~~~~~~~~~

The ``complex`` type represents a complex number. A ``complex`` value has
floating-point values for the real and imaginary components.

As with the integral and real types, the type ``complex`` can be
parameterized by the number of bits used to represent the complex number.
Since the complex number consists of two components, the number of bits
used to represent it is twice the number of bits used to represent each
component.

In particular:

 * ``complex(64)`` contains two ``real(32)`` fields
 * ``complex(128)`` contains two ``real(64)`` fields

The real and imaginary components can be accessed via the methods ``re``
and ``im``. Note that ``im`` returns a ``real`` of appropriate width,
rather than an ``imag``.

   *Example*.

   Given a complex number ``c`` with the value ``3.14+2.72i``, the
   expressions ``c.re`` and ``c.im`` refer to ``3.14`` and ``2.72``
   respectively.

.. method:: proc complex.re ref


   When used as a value, this returns the real component of
   the complex number as a *real*.

   When used as an lvalue, this is a setter that assigns the
   real component.


.. method:: proc complex.im ref


   When used as a value, this returns the imaginary component of
   the complex number as a *real*.

   When used as an lvalue, this is a setter that assigns the
   imaginary component.

The standard :mod:`Math` module provides more functions on complex types.
See the :mod:`Math` module documentation.


.. index::
   single: string
   single: types; string
.. _The_String_Type:

The String Type
~~~~~~~~~~~~~~~

Strings are a primitive type designated by the symbol ``string``
comprised of Unicode characters in UTF-8 encoding. Their length is
unbounded. Strings are defined in :ref:`Chapter-Strings`.

.. index::
   single: bytes
   single: types; bytes
.. _The_Bytes_Type:

The Bytes Type
~~~~~~~~~~~~~~

Bytes is a primitive type designated by the symbol ``bytes`` comprised
of arbitrary bytes. Bytes are immutable in-place and their length is
unbounded. Bytes are defined in :ref:`Chapter-Bytes`.

.. index::
   single: enumerated types
   single: types; enumerated
   single: enumerated types; abstract
   single: enumerated types; concrete
   single: enumerated types; semi-concrete
   single: enumerated types; iterating
   single: enumerated types; size
   single: predefined functions; size (enum)
.. _Enumerated_Types:

Enumerated Types
----------------

Enumerated types are declared with the following syntax:



.. code-block:: syntax

   enum-declaration-statement:
     'enum' identifier { enum-constant-list }

   enum-constant-list:
     enum-constant
     enum-constant , enum-constant-list[OPT]

   enum-constant:
     identifier init-part[OPT]

   init-part:
     = expression

The enumerated type can then be referenced by its name, as summarized by
the following syntax:



.. code-block:: syntax

   enum-type:
     identifier

An enumerated type defines a set of named constants that can be referred
to via a member access on the enumerated type. Each enumerated type is a
distinct type.

If the ``init-part`` is omitted for all of the named constants in an
enumerated type, the enumerated values are *abstract* and do not have
associated integer values. Any constant that has an ``init-part`` will
be associated with that integer value. Such constants must be parameter
values of integral type. Any constant that does not have an
``init-part``, yet which follows one that does, will be associated with
an integer value one greater than its predecessor. An enumerated type
whose first constant has an ``init-part`` is called *concrete*, since
all constants in the enum will have an associated integer value, whether
explicit or implicit. An enumerated type that specifies an ``init-part``
for some constants, but not the first is called *semi-concrete*. Numeric
conversions are automatically supported for enumerated types which are
concrete or semi-concrete
(see :ref:`Explicit_Enumeration_Conversions`).

   *Example (enum-statesmen.chpl)*.

   The code

   .. code-block:: chapel

      enum statesman { Aristotle, Roosevelt, Churchill, Socrates }

   defines an abstract enumerated type with four constants. The function


   .. code-block:: chapel

      proc quote(s: statesman) {
        select s {
          when statesman.Aristotle do
             writeln("All paid jobs absorb and degrade the mind.");
          when statesman.Roosevelt do
             writeln("Every reform movement has a lunatic fringe.");
          when statesman.Churchill do
             writeln("A joke is a very serious thing.");
          when statesman.Socrates do
             { write("I only wish that wisdom were the kind of thing that flowed ");
               writeln("... from the vessel that was full to the one that was empty."); }
        }
      }



   .. BLOCK-test-chapelnoprint

      for s in statesman do
        quote(s:statesman);



   .. BLOCK-test-chapeloutput

      All paid jobs absorb and degrade the mind.
      Every reform movement has a lunatic fringe.
      A joke is a very serious thing.
      I only wish that wisdom were the kind of thing that flowed ... from the vessel that was full to the one that was empty.

   outputs a quote from the given statesman. Note that enumerated
   constants must be prefixed by the enumerated type name and a dot
   unless a use statement is employed
   (see :ref:`The_Use_Statement` and :ref:`Using_Modules`).

It is possible to iterate over an enumerated type. The loop body will be
invoked on each named constant in the enum. The following method is also
available:



.. function:: proc enum.size: param int

   Returns the number of constants in the given enumerated type.

.. function:: proc enum.first: enum

   Returns the first constant in the enumerated type.

.. function:: proc enum.last: enum

   Returns the last constant in the enumerated type.

.. index::
   single: types; structured
.. _Structured_Types:

Structured Types
----------------

The structured types are summarized by the following syntax:



.. code-block:: syntax

   structured-type:
     class-type
     record-type
     union-type
     tuple-type

Classes are discussed in :ref:`Chapter-Classes`. Records are
discussed in :ref:`Chapter-Records`. Unions are discussed in
:ref:`Chapter-Unions`. Tuples are discussed in
:ref:`Chapter-Tuples`.

.. _Types_Class_Types:

Class Types
~~~~~~~~~~~

A class can contain variables, constants, and methods.

Classes are defined in :ref:`Chapter-Classes`. The class type can
also contain type aliases and parameters. Such a class is generic and is
defined in :ref:`Generic_Types`.

A class type ``C`` has several variants:

-  ``C`` and ``C?``

-  ``owned C`` and ``owned C?``

-  ``shared C`` and ``shared C?``

-  ``borrowed C`` and ``borrowed C?``

-  ``unmanaged C`` and ``unmanaged C?``

The variants with a question mark, such as ``owned C?``, can store
``nil`` (see :ref:`Nilable_Classes`). Variants without a
question mark cannot store ``nil``. The keywords ``owned``, ``shared``,
``borrowed``, and ``unmanaged`` indicate the memory management strategy
used for the class. When none is specified, as with ``C`` or ``C?``, the
class is considered to have generic memory management strategy.
See :ref:`Class_Types`.

.. _Types_Record_Types:

Record Types
~~~~~~~~~~~~

Records can contain variables, constants, and methods. Unlike class
types, records are values rather than references. Records are defined
in :ref:`Chapter-Records`.

.. _Types_Union_Types:

Union Types
~~~~~~~~~~~

The union type defines a type that contains one of a set of variables.
Like classes and records, unions may also define methods. Unions are
defined in :ref:`Chapter-Unions`.

.. _Types_Tuple_Types:

Tuple Types
~~~~~~~~~~~

A tuple is a light-weight record that consists of one or more anonymous
fields. If all the fields are of the same type, the tuple is
homogeneous. Tuples are defined in :ref:`Chapter-Tuples`.

.. index::
   single: types; data parallel
.. _Data_Parallel_Types:

Data Parallel Types
-------------------

The data parallel types are summarized by the following syntax:



.. code-block:: syntax

   dataparallel-type:
     range-type
     domain-type
     mapped-domain-type
     array-type
     index-type

Ranges and their index types are discussed in :ref:`Chapter-Ranges`.
Domains and their index types are discussed in
:ref:`Chapter-Domains`. Arrays are discussed in
:ref:`Chapter-Arrays`.

.. _Types_Range_Types:

Range Types
~~~~~~~~~~~

A range defines an integral sequence of some integral type. Ranges are
defined in :ref:`Chapter-Ranges`.

.. _Domain_and_Array_Types:

Domain, Array, and Index Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A domain defines a set of indices. An array defines a set of elements
that correspond to the indices in its domain. A domain’s indices can be
of any type. Domains, arrays, and their index types are defined in
:ref:`Chapter-Domains` and :ref:`Chapter-Arrays`.

.. index::
   single: types; synchronization
.. _Synchronization_Types:

Synchronization Types
---------------------

The synchronization types are summarized by the following syntax:



.. code-block:: syntax

   synchronization-type:
     sync-type
     atomic-type

The sync type is discussed in
:ref:`Synchronization_Variables`. The atomic type is discussed
in :ref:`Atomic_Variables`.

.. index::
   single: types; aliases
.. _Type_Aliases:

Type Aliases
------------

Type aliases are declared with the following syntax:

.. code-block:: syntax

   type-alias-declaration-statement:
     privacy-specifier[OPT] 'config'[OPT] 'type' type-alias-declaration-list ;
     external-type-alias-declaration-statement

   type-alias-declaration-list:
     type-alias-declaration
     type-alias-declaration , type-alias-declaration-list

   type-alias-declaration:
     identifier = type-expression
     identifier

A type alias is a symbol that aliases the type specified in the
``type-expression``. A use of a type alias has the same meaning as using
the type specified by ``type-expression`` directly.

Type aliases defined at the module level are public by default. The
optional ``privacy-specifier`` keywords are provided to specify or
change this behavior. For more details on the visibility of symbols, see
 :ref:`Visibility_Of_Symbols`.

If the keyword ``config`` precedes the keyword ``type``, the type alias
is called a configuration type alias. Configuration type aliases can be
set at compilation time via compilation flags or other
implementation-defined means. The ``type-expression`` in the program is
ignored if the type-alias is alternatively set.

If the keyword ``extern`` precedes the ``type`` keyword, the type alias
is external. The declared type name is used by Chapel for type
resolution, but no type alias is generated by the backend. See the
chapter on interoperability
(:ref:`Chapter-Interoperability`) for more information on
external types.

The ``type-expression`` is optional in the definition of a class or
record. Such a type alias is called an unspecified type alias. Classes
and records that contain type aliases, specified or unspecified, are
generic (:ref:`Type_Aliases_in_Generic_Types`).

   *Example (type-alias.chpl)*.

   The declaration

   .. code-block:: chapel

      type t = int;

   defines a ``t`` as a synonym for the type ``int``. Functions and
   methods available on ``int`` will apply to variables declared with
   type ``t``. For example,

   .. code-block:: chapel

      var x: t = 1;
      x += 1;
      writeln(x);

   will print out ``2``.

   .. BLOCK-test-chapeloutput

      2


.. _Querying_the_Type_of_an_Expression:

Querying the Type of an Expression
----------------------------------

.. code-block:: syntax

   type-query-expression:
     expression . 'type'

The type of a an expression can be queried with ``.type``. This
functionality is particularly useful when doing generic programming
(see :ref:`Chapter-Generics`).

   *Example (dot-type.chpl)*.

   For example, this code uses ``.type`` to query the type of the
   variable ``x`` and store that in the type alias ``t``:

   .. code-block:: chapel

      var x: int;
      type t = x.type;

   .. BLOCK-test-chapelpost

      writeln(t:string);

   .. BLOCK-test-chapeloutput

      int(64)

   *Open issue*.

   Given a nested expression that has ``.type`` called on it,
   for example ``f()`` in ``f().type``, in which circumstances should
   ``f()`` be evaluated for side effects?

   At first it might seem that ``f()`` should never be evaluated for side
   effects. However, it must be evaluated for side effects if ``f()`` returns an
   array or domain type, as these have a runtime component (see
   :ref:`Types_with_Runtime_Components`). As a result, should ``f()`` in
   such a setting always be evaluated for side effects?  The answer to
   this question also also connected to the question of whether or not a
   when a function returning a ``type`` is evaluated for side effects at
   runtime.

   One approach might be to introduce different means to query only the
   compile-time component of the type or only the runtime component of
   the time.


.. _Operations_Available_on_Types:

Operations Available on Types
-----------------------------

This section discusses how type expressions can be used. Type expressions
include types, type aliases, ``.type`` queries, and calls to functions
that use the ``type`` return intent.

A type expression can be used to indicate the type of a value, as with
``var x: typeExpression;`` (see :ref:`Variable_Declarations`).

A type expression can be passed to a ``type`` formal of a generic
function (see :ref:`Formal_Type_Arguments`).

The :mod:`Types` module provides many functions to query properties of
types.

The language provides :proc:`isCoercible <Types.isCoercible>`,
:proc:`isSubtype <Types.isSubtype>`, and
:proc:`isProperSubtype <Types.isProperSubtype>` for comparing types.
The normal comparison operators are also available to compare types:

 * ``==`` checks if two types are equivalent
 * ``!=`` checks if two types are different

It is possible to cast a type to a ``param`` string. This allows a type
to be printed out.

  *Example (type-to-string.chpl)*.

   For example, this code casts the type ``myType`` to a string in order
   to print it out:

   .. code-block:: chapel

      type myType = int;
      param str = myType:string;
      writeln(str);

   It produces the output:

   .. code-block:: printoutput

      int(64)

   *Open issue*.

   If type comparison with ``==`` is called on two types with runtime
   components (see :ref:`Types_with_Runtime_Components`), should the
   runtime component be included in the comparison? Or, should ``==`` on
   types only consider if the compile-time components match?

.. _Types_with_Runtime_Components:

Types with Runtime Components
-----------------------------

Domain and array types include a *runtime component*. (See
:ref:`Chapter-Domains` and :ref:`Chapter-Arrays` for more on arrays and
domains).

For a domain type, the runtime component of the type is the distribution over
which the domain was declared.

For an array type, the runtime component of the type contains the domain
over which the array was declared and the runtime component of the
array's element type, if present.

As a result, an array or domain type will be represented and manipulated
at runtime. In particular, a function that returns a type with a runtime
component will be executed at runtime.

These features combine with the ``.type`` syntax to allow one to create
an array that has the same element type, shape, and distribution as an
existing array.

  *Example (same-domain-array.chpl)*.

   The example below shows a function that accepts an array and then
   creates another array with the same element type, shape, and distribution:

   .. code-block:: chapel

      proc makeAnotherArray(arr: []) {
        var newArray: arr.type;
        return newArray;
      }

   The above program is equivalent to this program:

   .. code-block:: chapel

      proc equivalentAlternative(arr: []) {
        var newArray:[arr.domain] arr.eltType;
        return newArray;
      }

   Both create and return an array storing the same element type as the
   passed array.

    .. BLOCK-test-chapelpost

      var A:[1..4] int = 1..4;
      var B = makeAnotherArray(A);
      var C = equivalentAlternative(A);
      writeln("A.domain ", A.domain);
      writeln("A ", A);
      writeln("B.domain ", B.domain);
      writeln("B ", B);
      writeln("C.domain ", C.domain);
      writeln("C ", C);

   .. BLOCK-test-chapeloutput

      A.domain {1..4}
      A 1 2 3 4
      B.domain {1..4}
      B 0 0 0 0
      C.domain {1..4}
      C 0 0 0 0

   *Open issue*.

   Should a record or class type also have a runtime component when it
   contains array/domain field(s)? This runtime component is needed, for
   example, to create a default-initialized instance of such a type in
   the absence of user-defined default initializer.

   *Open issue*.

   Class types are not currently considered to have a runtime component.
   Should class types be considered to have a runtime component, so that
   querying an instance's type with ``myObject.type`` will produce the
   type of the object known at runtime, rather than the type with which
   ``myObject`` was declared?

   *Open issue*.

   Should functions returning a type always be evaluated for side
   effects, or only evaluated for side effects when returning a type with
   a runtime component?
