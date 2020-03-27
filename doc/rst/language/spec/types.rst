.. _Chapter-Types:

.. default-domain:: chpl

Types
=====

Chapel is a statically typed language with a rich set of types. These
include a set of predefined primitive types, enumerated types,
structured types (classes, records, unions, tuples), data parallel types
(ranges, domains, arrays), and synchronization types (sync, single,
atomic).

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

.. _Primitive_Types:

Primitive Types
---------------

The concrete primitive types are: ``void``, ``nothing``, ``bool``,
``int``, ``uint``, ``real``, ``imag``, ``complex``, ``string`` and
``bytes``. They are defined in this section.

In addition, there are several generic primitive types that are
described in :ref:`Built_in_Generic_types`.

The primitive types are summarized by the following syntax: 

.. code-block:: syntax

   primitive-type:
     `void'
     `nothing'
     `bool' primitive-type-parameter-part[OPT]
     `int' primitive-type-parameter-part[OPT]
     `uint' primitive-type-parameter-part[OPT]
     `real' primitive-type-parameter-part[OPT]
     `imag' primitive-type-parameter-part[OPT]
     `complex' primitive-type-parameter-part[OPT]
     `string'
     `bytes'
     `enum'
     `record'
     `class'
     `owned'
     `shared'
     `unmanaged'
     `borrowed'

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

.. _The_Void_Type:

The Void Type
~~~~~~~~~~~~~

The ``void`` type is used to represent the lack of a value, for example
when a function has no arguments and/or no return type. It is an error
to assign the result of a function that returns ``void`` to a variable.

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

.. _The_Bool_Type:

The Bool Type
~~~~~~~~~~~~~

Chapel defines a logical data type designated by the symbol ``bool``
with the two predefined values ``true`` and ``false``. This default
boolean type is stored using an implementation-defined number of bits. A
particular number of bits can be specified using a parameter value
following the ``bool`` keyword, such as ``bool(8)`` to request an 8-bit
boolean value. Legal sizes are 8, 16, 32, and 64 bits.

Some statements require expressions of ``bool`` type and Chapel supports
a special conversion of values to ``bool`` type when used in this
context (:ref:`Implicit_Statement_Bool_Conversions`).

.. _Signed_and_Unsigned_Integral_Types:

Signed and Unsigned Integral Types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The integral types can be parameterized by the number of bits used to
represent them. Valid bit-sizes are 8, 16, 32, and 64. The default
signed integral type, ``int``, and the default unsigned integral type,
``uint`` correspond to ``int(64)`` and ``uint(64)`` respectively.

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

The unary and binary operators that are pre-defined over the integral
types operate with 32- and 64-bit precision. Using these operators on
integral types represented with fewer bits results in an implicit
conversion to the corresponding 32-bit types according to the rules
defined in :ref:`Implicit_Conversions`.

.. _Real_Types:

Real Types
~~~~~~~~~~

Like the integral types, the real types can be parameterized by the
number of bits used to represent them. The default real type, ``real``,
is 64 bits. The real types that are supported are machine-dependent, but
usually include ``real(32)`` (single precision) and ``real(64)`` (double
precision) following the IEEE 754 standard.

.. _Imaginary_Types:

Imaginary Types
~~~~~~~~~~~~~~~

The imaginary types can be parameterized by the number of bits used to
represent them. The default imaginary type, ``imag``, is 64 bits. The
imaginary types that are supported are machine-dependent, but usually
include ``imag(32)`` and ``imag(64)``.

   *Rationale*.

   The imaginary type is included to avoid numeric instabilities and
   under-optimized code stemming from always converting real values to
   complex values with a zero imaginary part.

.. _Complex_Types:

Complex Types
~~~~~~~~~~~~~

Like the integral and real types, the complex types can be parameterized
by the number of bits used to represent them. A complex number is
composed of two real numbers so the number of bits used to represent a
complex is twice the number of bits used to represent the real numbers.
The default complex type, ``complex``, is 128 bits; it consists of two
64-bit real numbers. The complex types that are supported are
machine-dependent, but usually include ``complex(64)`` and
``complex(128)``.

| The real and imaginary components can be accessed via the methods
  ``re`` and ``im``. The type of these components is real. The standard
  ``Math`` module provides some functions on complex types. See
| https://chapel-lang.org/docs/modules/standard/Math.html

   *Example*.

   Given a complex number ``c`` with the value ``3.14+2.72i``, the
   expressions ``c.re`` and ``c.im`` refer to ``3.14`` and ``2.72``
   respectively.

.. _The_String_Type:

The String Type
~~~~~~~~~~~~~~~

Strings are a primitive type designated by the symbol ``string``
comprised of Unicode characters in UTF-8 encoding. Their length is
unbounded.

   *Open issue*.

   There is an expectation of future support for fixed-length strings.

.. _The_Bytes_Type:

The Bytes Type
~~~~~~~~~~~~~~

Bytes is a primitive type designated by the symbol ``bytes`` comprised
of arbitrary bytes. Bytes are immutable in-place and their length is
unbounded.

   *Open issue*.

   There is an expectation of future support for mutable bytes.

.. _Enumerated_Types:

Enumerated Types
----------------

Enumerated types are declared with the following syntax:



.. code-block:: syntax

   enum-declaration-statement:
     `enum' identifier { enum-constant-list }

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

      enum statesman { Aristotle, Roosevelt, Churchill, Kissinger }

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
          when statesman.Kissinger do
             { write("No one will ever win the battle of the sexes; ");
               writeln("there's too much fraternizing with the enemy."); }
        }
      } 

   

   .. BLOCK-test-chapelnoprint

      for s in statesman do
        quote(s:statesman);

   

   .. BLOCK-test-chapeloutput

      All paid jobs absorb and degrade the mind.
      Every reform movement has a lunatic fringe.
      A joke is a very serious thing.
      No one will ever win the battle of the sexes; there's too much fraternizing with the enemy.

   outputs a quote from the given statesman. Note that enumerated
   constants must be prefixed by the enumerated type name and a dot
   unless a use statement is employed
   (see :ref:`The_Use_Statement`).

It is possible to iterate over an enumerated type. The loop body will be
invoked on each named constant in the enum. The following method is also
available:



.. function:: proc enum.size: param int

     Returns the number of constants in the given enumerated type.

.. function:: proc enum.first: enum

     Returns the first constant in the enumerated type.

.. function:: proc enum.last: enum

     Returns the last constant in the enumerated type.

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

.. _Synchronization_Types:

Synchronization Types
---------------------

The synchronization types are summarized by the following syntax:



.. code-block:: syntax

   synchronization-type:
     sync-type
     single-type
     atomic-type

Sync and single types are discussed in
:ref:`Synchronization_Variables`. The atomic type is discussed
in :ref:`Atomic_Variables`.

.. _Type_Aliases:

Type Aliases
------------

Type aliases are declared with the following syntax: 

.. code-block:: syntax

   type-alias-declaration-statement:
     privacy-specifier[OPT] `config'[OPT] `type' type-alias-declaration-list ;
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

