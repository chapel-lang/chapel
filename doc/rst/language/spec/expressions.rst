.. _Chapter-Expressions:

Expressions
===========

Chapel provides the following expressions:



.. code-block:: syntax

   expression:
     literal-expression
     nil-expression
     variable-expression
     enum-constant-expression
     call-expression
     type-expression
     iteratable-call-expression
     member-access-expression
     new-expression
     query-expression
     cast-expression
     lvalue-expression
     parenthesized-expression
     unary-expression
     binary-expression
     let-expression
     if-expression
     for-expression
     forall-expression
     reduce-expression
     scan-expression
     module-access-expression
     tuple-expression
     tuple-expand-expression
     locale-access-expression
     mapped-domain-expression

Individual expressions are defined in the remainder of this chapter and
additionally as follows:

-  forall, reduce, and scan
   :ref:`Chapter-Data_Parallelism`

-  module access :ref:`Explicit_Naming`

-  tuple and tuple expand :ref:`Chapter-Tuples`

-  locale access :ref:`Querying_the_Locale_of_a_Variable`

-  mapped domain :ref:`Chapter-Domain_Maps`

-  initializer calls :ref:`Class_New`

-  ``nil`` :ref:`Class_nil_value`

.. _Literal_Expressions:

Literal Expressions
-------------------

A literal value for any of the predefined types is a literal expression.

Literal expressions are given by the following syntax: 

.. code-block:: syntax

   literal-expression:
     bool-literal
     integer-literal
     real-literal
     imaginary-literal
     string-literal
     bytes-literal
     range-literal
     domain-literal
     array-literal

Literal values for primitive types are described in
:ref:`Literals`. Literal
range values are described in :ref:`Range_Literals`. Literal
tuple values are described in :ref:`Tuple_Values`. Literal
values for domains are described in
:ref:`Rectangular_Domain_Values` and
:ref:`Associative_Domain_Values`. Literal values for arrays
are described in :ref:`Rectangular_Array_Literals` and
:ref:`Associative_Array_Literals`.

.. _Variable_Expressions:

Variable Expressions
--------------------

A use of a variable, constant, parameter, or formal argument, is itself
an expression. The syntax of a variable expression is given by:


.. code-block:: syntax

   variable-expression:
     identifier 

.. _Enumeration_Constant_Expression:

Enumeration Constant Expression
-------------------------------

A use of an enumeration constant is itself an expression. Such a
constant must be preceded by the enumeration type name. The syntax of an
enumeration constant expression is given by: 

.. code-block:: syntax

   enum-constant-expression:
     enum-type . identifier

For an example of using enumeration constants,
see :ref:`Enumerated_Types`.

.. _Parenthesized_Expressions:

Parenthesized Expressions
-------------------------

A ``parenthesized-expression`` is an expression that is delimited by
parentheses as given by: 

.. code-block:: syntax

   parenthesized-expression:
     ( expression )

Such an expression evaluates to the expression. The parentheses are
ignored and have only a syntactical effect.

.. _Call_Expressions:

Call Expressions
----------------

Functions and function calls are defined
in :ref:`Chapter-Procedures`.

.. _Indexing_Expressions:

Indexing Expressions
--------------------

Indexing, for example into arrays, tuples, and domains, has the same
syntax as a call expression.

Indexing is performed by an implicit invocation of the ``this`` method
on the value being indexed, passing the indices as the actual arguments.

.. _Member_Access_Expressions:

Member Access Expressions
-------------------------

Member access expressions provide access to a field or invoke a method
of an instance of a class, record, or union. They are defined in
:ref:`Class_Field_Accesses` and
:ref:`Class_Method_Calls`, respectively.



.. code-block:: syntax

   member-access-expression:
     field-access-expression
     method-call-expression

.. _The_Query_Expression:

The Query Expression
--------------------

A query expression is used to query a type or value within a formal
argument type expression. The syntax of a query expression is given by:


.. code-block:: syntax

   query-expression:
     ? identifier[OPT]

Querying is restricted to querying the type of a formal argument, the
element type of a formal argument that is an array, the domain of a
formal argument that is an array, the size of a primitive type, or a
type or parameter field of a formal argument type.

The identifier can be omitted. This is useful for ensuring the
genericity of a generic type that defines default values for all of its
generic fields when specifying a formal argument as discussed
in :ref:`Formal_Arguments_of_Generic_Type`.

   *Example (query.chpl)*.

   The following code defines a generic function where the type of the
   first argument is queried and stored in the type alias ``t`` and the
   domain of the second argument is queried and stored in the variable
   ``D``: 

   .. BLOCK-test-chapelnoprint

      { // }

   

   .. code-block:: chapel

      proc foo(x: ?t, y: [?D] t) {
        for i in D do
          y[i] = x;
      }

   

   .. BLOCK-test-chapelnoprint

      // {
      var x = 1.5;
      var y: [1..4] x.type;
      foo(x, y);
      writeln(y);
      }

   This allows a generic specification of assigning a particular value
   to all elements of an array. The value and the elements of the array
   are constrained to be the same type. This function can be rewritten
   without query expression as follows: 

   .. BLOCK-test-chapelnoprint

      { // }

   

   .. code-block:: chapel

      proc foo(x, y: [] x.type) {
        for i in y.domain do
          y[i] = x;
      }

   

   .. BLOCK-test-chapelnoprint

      // {
      var x = 1.5;
      var y: [1..4] x.type;
      foo(x, y);
      writeln(y);
      }

   

   .. BLOCK-test-chapeloutput

      1.5 1.5 1.5 1.5
      1.5 1.5 1.5 1.5

There is an expectation that query expressions will be allowed in more
places in the future.

.. _Casts:

Casts
-----

A cast is specified with the following syntax: 

.. code-block:: syntax

   cast-expression:
     expression : type-expression

The expression is converted to the specified type. A cast expression
invokes the corresponding explicit
conversion (:ref:`Explicit_Conversions`). A resolution error
occurs if no such conversion exists.

.. _LValue_Expressions:

LValue Expressions
------------------

An *lvalue* is an expression that can be used on the left-hand side of
an assignment statement or on either side of a swap statement, that can
be passed to a formal argument of a function that has ``out``, ``inout``
or ``ref`` intent, or that can be returned by a function with a ``ref``
return intent (:ref:`Ref_Return_Intent`). Valid lvalue
expressions include the following:

-  Variable expressions.

-  Member access expressions.

-  Call expressions of functions with a ``ref`` return intent.

-  Indexing expressions.

LValue expressions are given by the following syntax: 

.. code-block:: syntax

   lvalue-expression:
     variable-expression
     member-access-expression
     call-expression
     parenthesized-expression

The syntax is less restrictive than the definition above. For example,
not all ``call-expression``\ s are lvalues.

.. _Operator_Precedence_and_Associativity:

Precedence and Associativity
----------------------------

+--------------------+----------------+--------------------------------------+
| Operator           | Associativity  | Use                                  |
+====================+================+======================================+
| | ``.``            | left           | | member access                      |
| | ``()``           |                | | function call or access            |
| | ``[]``           |                | | function call or access            |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``new``            | right          | initializer call                     |
+--------------------+----------------+--------------------------------------+
| | ``owned``        | right          | apply management strategy to a class |
| | ``shared``       |                |                                      |
| | ``borrowed``     |                |                                      |
| | ``unmanaged``    |                |                                      |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| | postfix ``?``    | left           | | compute a nilable class type       |
| | postfix ``!``    |                | | assert non-nilable and borrow      |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``:``              | left           | cast                                 |
+--------------------+----------------+--------------------------------------+
| ``**``             | right          | exponentiation                       |
+--------------------+----------------+--------------------------------------+
| | ``reduce``       | left           | | reduction                          |
| | ``scan``         | scan           | | scan                               |
| | ``dmapped``      |                | | domain map application             |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| | prefix ``!``     | right          | | logical negation                   |
| | ``~``            |                | | bitwise negation                   |
+--------------------+----------------+--------------------------------------+
| | ``*``            | left           | | multiplication                     |
| | ``/``            |                | | division                           |
| | ``%``            |                | | modulus                            |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| | unary ``+``      | right          | | positive identity                  |
| | unary ``-``      |                | | negation                           |
+--------------------+----------------+--------------------------------------+
| | ``<<``           | left           | | left shift                         |
| | ``>>``           |                | | right shift                        |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``&``              | left           | bitwise/logical and                  |
+--------------------+----------------+--------------------------------------+
| ``^``              | left           | bitwise/logical xor                  |
+--------------------+----------------+--------------------------------------+
| ``|``              | left           | bitwise/logical or                   |
+--------------------+----------------+--------------------------------------+
| | ``+``            | left           | | addition                           |
| | ``-``            |                | | subtraction                        |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| | ``..``           | | left         | | range initialization               |
| | ``..<``          | | left         | | open-interval range initialization |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| | ``<=``           | left           | | less-than-or-equal-to comparison   |
| | ``>=``           |                | | greater-than-or-equal-to comparison|
| | ``<``            |                | | less-than comparison               |
| | ``>``            |                | | greater-than comparison            |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``==``             | left           | equal-to comparison                  |
| ``!=``             |                | not-equal-to comparison              |
+--------------------+----------------+--------------------------------------+
| ``&&``             | left           | short-circuiting logical and         |
+--------------------+----------------+--------------------------------------+
| ``||``             | left           | short-circuiting logical or          |
+--------------------+----------------+--------------------------------------+
| | ``by``           | left           | | range/domain stride application    |
| | ``#``            |                | | range count application            |
| | ``align``        |                | | range alignment                    |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``in``             | left           | forall expression                    |
+--------------------+----------------+--------------------------------------+
| | ``if then else`` | left           | | conditional expression             |
| | ``forall do``    |                | | forall expression                  |
| | ``[ ]``          |                | | forall expression                  |
| | ``for do``       |                | | for expression                     |
| | ``sync``         |                | | sync type modifier                 |
| | ``single``       |                | | single type modifier               |
| | ``atomic``       |                | | atomic type modifier               |
|                    |                |                                      |
+--------------------+----------------+--------------------------------------+
| ``,``              | left           | comma separated expressions          |
+--------------------+----------------+--------------------------------------+


The above table summarizes operator and expression precedence and
associativity. Operators and expressions listed earlier have higher
precedence than those listed later.

   *Rationale*.

   In general, our operator precedence is based on that of the C family
   of languages including C++, Java, Perl, and C#. We comment on a few
   of the differences and unique factors here.

   We find that there is tension between the relative precedence of
   exponentiation, unary minus/plus, and casts. The following three
   expressions show our intuition for how these expressions should be
   parenthesized.

   ================== ===== ======================
   ``-2**4``          wants ``-(2**4)``
   ``-2:uint``        wants ``(-2):uint``
   ``2:uint**4:uint`` wants ``(2:uint)**(4:uint)``
   ================== ===== ======================

   Trying to support all three of these cases results in a
   circularity—exponentiation wants precedence over unary minus, unary
   minus wants precedence over casts, and casts want precedence over
   exponentiation. We chose to break the circularity by making unary
   minus have a lower precedence. This means that for the second case
   above:

   =========== ======== =============
   ``-2:uint`` requires ``(-2):uint``
   =========== ======== =============

   We also chose to depart from the C family of languages by making
   unary plus/minus have lower precedence than binary multiplication,
   division, and modulus as in Fortran. We have found very few cases
   that distinguish between these cases. An interesting one is:

   ::
     const minint = min(int(32));``
     ...-minint/2...``

   Intuitively, this should result in a positive value, yet C’s
   precedence rules results in a negative value due to asymmetry in
   modern integer representations. If we learn of cases that argue in
   favor of the C approach, we would likely reverse this decision in
   order to more closely match C.

   We were tempted to diverge from the C precedence rules for the binary
   bitwise operators to make them bind less tightly than comparisons.
   This would allow us to interpret:

   ============== == ================
   ``a | b == 0`` as ``(a | b) == 0``
   ============== == ================

   However, given that no other popular modern language has made this
   change, we felt it unwise to stray from the pack. The typical
   rationale for the C ordering is to allow these operators to be used
   as non-short-circuiting logical operations.

   In contrast to C, we give bitwise operations a higher precedence than
   binary addition/subtraction and comparison operators. This enables
   using the shift operators as shorthand for multiplication/division by
   powers of 2, and also makes it easier to extract and test a bitmapped
   field:

   ======================= == =====================
   ``(x & MASK) == MASK``  as ``x & MASK == MASK``
   ``a + b * pow(2,y)``    as ``a * b << y``
   ======================= == =====================

   One final area of note is the precedence of reductions. Two common
   cases tend to argue for making reductions very low or very high in
   the precedence table:

   =============================== ===== ===================================
   ``max reduce A - min reduce A`` wants ``(max reduce A) - (min reduce A)``
   ``max reduce A * B``            wants ``max reduce (A * B)``
   =============================== ===== ===================================

   The first statement would require reductions to have a higher
   precedence than the arithmetic operators while the second would
   require them to be lower. We opted to make reductions have high
   precedence due to the argument that they tend to resemble unary
   operators. Thus, to support our intuition:

   ==================== ======== ======================
   ``max reduce A * B`` requires ``max reduce (A * B)``
   ==================== ======== ======================

   This choice also has the (arguably positive) effect of making the
   unparenthesized version of this statement result in an aggregate
   value if A and B are both aggregates—the reduction of A results in a
   scalar which promotes when being multiplied by B, resulting in an
   aggregate. Our intuition is that users who forget the parentheses
   will learn of their error at compilation time because the resulting
   expression is not a scalar as expected.

.. _Binary_Expressions:

Operator Expressions
--------------------

[Unary_Expressions]

The application of operators to expressions is itself an expression. The
syntax of a unary expression is given by: 

.. code-block:: syntax

   unary-expression:
     unary-operator expression

   unary-operator: one of
     + - ~ !

The syntax of a binary expression is given by: 

.. code-block:: syntax

   binary-expression:
     expression binary-operator expression

   binary-operator: one of
     + - * / % ** & | ^ << >> && || == != <= >= < > `by' #

The operators are defined in subsequent sections.

.. _Arithmetic_Operators:

Arithmetic Operators
--------------------

This section describes the predefined arithmetic operators. These
operators can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

For each operator, implicit conversions are applied to the operands of
an operator such that they are compatible with one of the function forms
listed, those listed earlier in the list being given preference. If no
compatible implicit conversions exist, then a compile-time error occurs.
In these cases, an explicit cast is required.

.. _Unary_Plus_Operators:

Unary Plus Operators
~~~~~~~~~~~~~~~~~~~~

The unary plus operators are predefined as follows: 

.. code-block:: chapel

   proc +(a: int(8)): int(8)
   proc +(a: int(16)): int(16)
   proc +(a: int(32)): int(32)
   proc +(a: int(64)): int(64)

   proc +(a: uint(8)): uint(8)
   proc +(a: uint(16)): uint(16)
   proc +(a: uint(32)): uint(32)
   proc +(a: uint(64)): uint(64)

   proc +(a: real(32)): real(32)
   proc +(a: real(64)): real(64)

   proc +(a: imag(32)): imag(32)
   proc +(a: imag(64)): imag(64)

   proc +(a: complex(64)): complex(64)
   proc +(a: complex(128)): complex(128)

For each of these definitions, the result is the value of the operand.

.. _Unary_Minus_Operators:

Unary Minus Operators
~~~~~~~~~~~~~~~~~~~~~

The unary minus operators are predefined as follows: 

.. code-block:: chapel

   proc -(a: int(8)): int(8)
   proc -(a: int(16)): int(16)
   proc -(a: int(32)): int(32)
   proc -(a: int(64)): int(64)

   proc -(a: real(32)): real(32)
   proc -(a: real(64)): real(64)

   proc -(a: imag(32)): imag(32)
   proc -(a: imag(64)): imag(64)

   proc -(a: complex(64)): complex(64)
   proc -(a: complex(128)): complex(128)

For each of these definitions that return a value, the result is the
negation of the value of the operand. For integral types, this
corresponds to subtracting the value from zero. For real and imaginary
types, this corresponds to inverting the sign. For complex types, this
corresponds to inverting the signs of both the real and imaginary parts.

It is an error to try to negate a value of type ``uint(64)``. Note that
negating a value of type ``uint(32)`` first converts the type to
``int(64)`` using an implicit conversion.

.. _Addition_Operators:

Addition Operators
~~~~~~~~~~~~~~~~~~

The addition operators are predefined as follows: 

.. code-block:: chapel

   proc +(a: int(8), b: int(8)): int(8)
   proc +(a: int(16), b: int(16)): int(16)
   proc +(a: int(32), b: int(32)): int(32)
   proc +(a: int(64), b: int(64)): int(64)

   proc +(a: uint(8), b: uint(8)): uint(8)
   proc +(a: uint(16), b: uint(16)): uint(16)
   proc +(a: uint(32), b: uint(32)): uint(32)
   proc +(a: uint(64), b: uint(64)): uint(64)

   proc +(a: real(32), b: real(32)): real(32)
   proc +(a: real(64), b: real(64)): real(64)

   proc +(a: imag(32), b: imag(32)): imag(32)
   proc +(a: imag(64), b: imag(64)): imag(64)

   proc +(a: complex(64), b: complex(64)): complex(64)
   proc +(a: complex(128), b: complex(128)): complex(128)

   proc +(a: real(32), b: imag(32)): complex(64)
   proc +(a: imag(32), b: real(32)): complex(64)
   proc +(a: real(64), b: imag(64)): complex(128)
   proc +(a: imag(64), b: real(64)): complex(128)

   proc +(a: real(32), b: complex(64)): complex(64)
   proc +(a: complex(64), b: real(32)): complex(64)
   proc +(a: real(64), b: complex(128)): complex(128)
   proc +(a: complex(128), b: real(64)): complex(128)

   proc +(a: imag(32), b: complex(64)): complex(64)
   proc +(a: complex(64), b: imag(32)): complex(64)
   proc +(a: imag(64), b: complex(128)): complex(128)
   proc +(a: complex(128), b: imag(64)): complex(128)

For each of these definitions that return a value, the result is the sum
of the two operands.

It is a compile-time error to add a value of type ``uint(64)`` and a
value of type ``int(64)``.

Addition over a value of real type and a value of imaginary type
produces a value of complex type. Addition of values of complex type and
either real or imaginary types also produces a value of complex type.

.. _Subtraction_Operators:

Subtraction Operators
~~~~~~~~~~~~~~~~~~~~~

The subtraction operators are predefined as follows: 

.. code-block:: chapel

   proc -(a: int(8), b: int(8)): int(8)
   proc -(a: int(16), b: int(16)): int(16)
   proc -(a: int(32), b: int(32)): int(32)
   proc -(a: int(64), b: int(64)): int(64)

   proc -(a: uint(8), b: uint(8)): uint(8)
   proc -(a: uint(16), b: uint(16)): uint(16)
   proc -(a: uint(32), b: uint(32)): uint(32)
   proc -(a: uint(64), b: uint(64)): uint(64)

   proc -(a: real(32), b: real(32)): real(32)
   proc -(a: real(64), b: real(64)): real(64)

   proc -(a: imag(32), b: imag(32)): imag(32)
   proc -(a: imag(64), b: imag(64)): imag(64)

   proc -(a: complex(64), b: complex(64)): complex(64)
   proc -(a: complex(128), b: complex(128)): complex(128)

   proc -(a: real(32), b: imag(32)): complex(64)
   proc -(a: imag(32), b: real(32)): complex(64)
   proc -(a: real(64), b: imag(64)): complex(128)
   proc -(a: imag(64), b: real(64)): complex(128)

   proc -(a: real(32), b: complex(64)): complex(64)
   proc -(a: complex(64), b: real(32)): complex(64)
   proc -(a: real(64), b: complex(128)): complex(128)
   proc -(a: complex(128), b: real(64)): complex(128)

   proc -(a: imag(32), b: complex(64)): complex(64)
   proc -(a: complex(64), b: imag(32)): complex(64)
   proc -(a: imag(64), b: complex(128)): complex(128)
   proc -(a: complex(128), b: imag(64)): complex(128)

For each of these definitions that return a value, the result is the
value obtained by subtracting the second operand from the first operand.

It is a compile-time error to subtract a value of type ``uint(64)`` from
a value of type ``int(64)``, and vice versa.

Subtraction of a value of real type from a value of imaginary type, and
vice versa, produces a value of complex type. Subtraction of values of
complex type from either real or imaginary types, and vice versa, also
produces a value of complex type.

.. _Multiplication_Operators:

Multiplication Operators
~~~~~~~~~~~~~~~~~~~~~~~~

The multiplication operators are predefined as follows: 

.. code-block:: chapel

   proc *(a: int(8), b: int(8)): int(8)
   proc *(a: int(16), b: int(16)): int(16)
   proc *(a: int(32), b: int(32)): int(32)
   proc *(a: int(64), b: int(64)): int(64)

   proc *(a: uint(8), b: uint(8)): uint(8)
   proc *(a: uint(16), b: uint(16)): uint(16)
   proc *(a: uint(32), b: uint(32)): uint(32)
   proc *(a: uint(64), b: uint(64)): uint(64)

   proc *(a: real(32), b: real(32)): real(32)
   proc *(a: real(64), b: real(64)): real(64)

   proc *(a: imag(32), b: imag(32)): real(32)
   proc *(a: imag(64), b: imag(64)): real(64)

   proc *(a: complex(64), b: complex(64)): complex(64)
   proc *(a: complex(128), b: complex(128)): complex(128)

   proc *(a: real(32), b: imag(32)): imag(32)
   proc *(a: imag(32), b: real(32)): imag(32)
   proc *(a: real(64), b: imag(64)): imag(64)
   proc *(a: imag(64), b: real(64)): imag(64)

   proc *(a: real(32), b: complex(64)): complex(64)
   proc *(a: complex(64), b: real(32)): complex(64)
   proc *(a: real(64), b: complex(128)): complex(128)
   proc *(a: complex(128), b: real(64)): complex(128)

   proc *(a: imag(32), b: complex(64)): complex(64)
   proc *(a: complex(64), b: imag(32)): complex(64)
   proc *(a: imag(64), b: complex(128)): complex(128)
   proc *(a: complex(128), b: imag(64)): complex(128)

For each of these definitions that return a value, the result is the
product of the two operands.

It is a compile-time error to multiply a value of type ``uint(64)`` and
a value of type ``int(64)``.

Multiplication of values of imaginary type produces a value of real
type. Multiplication over a value of real type and a value of imaginary
type produces a value of imaginary type. Multiplication of values of
complex type and either real or imaginary types produces a value of
complex type.

.. _Division_Operators:

Division Operators
~~~~~~~~~~~~~~~~~~

The division operators are predefined as follows: 

.. code-block:: chapel

   proc /(a: int(8), b: int(8)): int(8)
   proc /(a: int(16), b: int(16)): int(16)
   proc /(a: int(32), b: int(32)): int(32)
   proc /(a: int(64), b: int(64)): int(64)

   proc /(a: uint(8), b: uint(8)): uint(8)
   proc /(a: uint(16), b: uint(16)): uint(16)
   proc /(a: uint(32), b: uint(32)): uint(32)
   proc /(a: uint(64), b: uint(64)): uint(64)

   proc /(a: real(32), b: real(32)): real(32)
   proc /(a: real(64), b: real(64)): real(64)

   proc /(a: imag(32), b: imag(32)): real(32)
   proc /(a: imag(64), b: imag(64)): real(64)

   proc /(a: complex(64), b: complex(64)): complex(64)
   proc /(a: complex(128), b: complex(128)): complex(128)

   proc /(a: real(32), b: imag(32)): imag(32)
   proc /(a: imag(32), b: real(32)): imag(32)
   proc /(a: real(64), b: imag(64)): imag(64)
   proc /(a: imag(64), b: real(64)): imag(64)

   proc /(a: real(32), b: complex(64)): complex(64)
   proc /(a: complex(64), b: real(32)): complex(64)
   proc /(a: real(64), b: complex(128)): complex(128)
   proc /(a: complex(128), b: real(64)): complex(128)

   proc /(a: imag(32), b: complex(64)): complex(64)
   proc /(a: complex(64), b: imag(32)): complex(64)
   proc /(a: imag(64), b: complex(128)): complex(128)
   proc /(a: complex(128), b: imag(64)): complex(128)

For each of these definitions that return a value, the result is the
quotient of the two operands.

It is a compile-time error to divide a value of type ``uint(64)`` by a
value of type ``int(64)``, and vice versa.

Division of values of imaginary type produces a value of real type.
Division over a value of real type and a value of imaginary type
produces a value of imaginary type. Division of values of complex type
and either real or imaginary types produces a value of complex type.

When the operands are integers, the result (quotient) is also an
integer. If ``b`` does not divide ``a`` exactly, then there are two
candidate quotients :math:`q1` and :math:`q2` such that :math:`b * q1`
and :math:`b * q2` are the two multiples of ``b`` closest to ``a``. The
integer result :math:`q` is the candidate quotient which lies closest to
zero.

.. _Modulus_Operators:

Modulus Operators
~~~~~~~~~~~~~~~~~

The modulus operators are predefined as follows: 

.. code-block:: chapel

   proc %(a: int(8), b: int(8)): int(8)
   proc %(a: int(16), b: int(16)): int(16)
   proc %(a: int(32), b: int(32)): int(32)
   proc %(a: int(64), b: int(64)): int(64)

   proc %(a: uint(8), b: uint(8)): uint(8)
   proc %(a: uint(16), b: uint(16)): uint(16)
   proc %(a: uint(32), b: uint(32)): uint(32)
   proc %(a: uint(64), b: uint(64)): uint(64)

For each of these definitions that return a value, the result is the
remainder when the first operand is divided by the second operand.

The sign of the result is the same as the sign of the dividend ``a``,
and the magnitude of the result is always smaller than that of the
divisor ``b``. For integer operands, the ``%`` and ``/`` operators are
related by the following identity: 

.. code-block:: chapel

   var q = a / b;
   var r = a % b;
   writeln(q * b + r == a);    // true

It is a compile-time error to take the remainder of a value of type
``uint(64)`` and a value of type ``int(64)``, and vice versa.

There is an expectation that the predefined modulus operators will be
extended to handle real, imaginary, and complex types in the future.

.. _Exponentiation_Operators:

Exponentiation Operators
~~~~~~~~~~~~~~~~~~~~~~~~

The exponentiation operators are predefined as follows: 

.. code-block:: chapel

   proc **(a: int(8), b: int(8)): int(8)
   proc **(a: int(16), b: int(16)): int(16)
   proc **(a: int(32), b: int(32)): int(32)
   proc **(a: int(64), b: int(64)): int(64)

   proc **(a: uint(8), b: uint(8)): uint(8)
   proc **(a: uint(16), b: uint(16)): uint(16)
   proc **(a: uint(32), b: uint(32)): uint(32)
   proc **(a: uint(64), b: uint(64)): uint(64)

   proc **(a: real(32), b: real(32)): real(32)
   proc **(a: real(64), b: real(64)): real(64)

For each of these definitions that return a value, the result is the
value of the first operand raised to the power of the second operand.

It is a compile-time error to take the exponent of a value of type
``uint(64)`` by a value of type ``int(64)``, and vice versa.

There is an expectation that the predefined exponentiation operators
will be extended to handle imaginary and complex types in the future.

.. _Bitwise_Operators:

Bitwise Operators
-----------------

This section describes the predefined bitwise operators. These operators
can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

.. _Bitwise_Complement_Operators:

Bitwise Complement Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The bitwise complement operators are predefined as follows: 

.. code-block:: chapel

   proc ~(a: int(8)): int(8)
   proc ~(a: int(16)): int(16)
   proc ~(a: int(32)): int(32)
   proc ~(a: int(64)): int(64)

   proc ~(a: uint(8)): uint(8)
   proc ~(a: uint(16)): uint(16)
   proc ~(a: uint(32)): uint(32)
   proc ~(a: uint(64)): uint(64)

For each of these definitions, the result is the bitwise complement of
the operand.

.. _Bitwise_And_Operators:

Bitwise And Operators
~~~~~~~~~~~~~~~~~~~~~

The bitwise and operators are predefined as follows: 

.. code-block:: chapel

   proc &(a: bool, b: bool): bool

   proc &(a: int(?w), b: int(w)): int(w)
   proc &(a: uint(?w), b: uint(w)): uint(w)

   proc &(a: int(?w), b: uint(w)): uint(w)
   proc &(a: uint(?w), b: int(w)): uint(w)

For each of these definitions, the result is computed by applying the
logical and operation to the bits of the operands.

Chapel allows mixing signed and unsigned integers of the same size when
passing them as arguments to bitwise and. In the mixed case the result
is of the same size as the arguments and is unsigned. No run-time error
is issued, even if the apparent sign changes as the required conversions
are performed.

   *Rationale*.

   The mathematical meaning of integer arguments is discarded when they
   are passed to bitwise operators. Instead the arguments are treated
   simply as bit vectors. The bit-vector meaning is preserved when
   converting between signed and unsigned of the same size. The choice
   of unsigned over signed as the result type in the mixed case reflects
   the semantics of standard C.

.. _Bitwise_Or_Operators:

Bitwise Or Operators
~~~~~~~~~~~~~~~~~~~~

The bitwise or operators are predefined as follows: 

.. code-block:: chapel

   proc |(a: bool, b: bool): bool

   proc |(a: int(?w), b: int(w)): int(w)
   proc |(a: uint(?w), b: uint(w)): uint(w)

   proc |(a: int(?w), b: uint(w)): uint(w)
   proc |(a: uint(?w), b: int(w)): uint(w)

For each of these definitions, the result is computed by applying the
logical or operation to the bits of the operands. Chapel allows mixing
signed and unsigned integers of the same size when passing them as
arguments to bitwise or. No run-time error is issued, even if the
apparent sign changes as the required conversions are performed.

   *Rationale*.

   The same as for bitwise and (:ref:`Bitwise_And_Operators`).

.. _Bitwise_Xor_Operators:

Bitwise Xor Operators
~~~~~~~~~~~~~~~~~~~~~

The bitwise xor operators are predefined as follows: 

.. code-block:: chapel

   proc ^(a: bool, b: bool): bool

   proc ^(a: int(?w), b: int(w)): int(w)
   proc ^(a: uint(?w), b: uint(w)): uint(w)

   proc ^(a: int(?w), b: uint(w)): uint(w)
   proc ^(a: uint(?w), b: int(w)): uint(w)

For each of these definitions, the result is computed by applying the
XOR operation to the bits of the operands. Chapel allows mixing signed
and unsigned integers of the same size when passing them as arguments to
bitwise xor. No run-time error is issued, even if the apparent sign
changes as the required conversions are performed.

   *Rationale*.

   The same as for bitwise and (:ref:`Bitwise_And_Operators`).

.. _Shift_Operators:

Shift Operators
---------------

This section describes the predefined shift operators. These operators
can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

The shift operators are predefined as follows: 

.. code-block:: chapel

   proc <<(a: int(8), b): int(8)
   proc <<(a: int(16), b): int(16)
   proc <<(a: int(32), b): int(32)
   proc <<(a: int(64), b): int(64)

   proc <<(a: uint(8), b): uint(8)
   proc <<(a: uint(16), b): uint(16)
   proc <<(a: uint(32), b): uint(32)
   proc <<(a: uint(64), b): uint(64)

   proc >>(a: int(8), b): int(8)
   proc >>(a: int(16), b): int(16)
   proc >>(a: int(32), b): int(32)
   proc >>(a: int(64), b): int(64)

   proc >>(a: uint(8), b): uint(8)
   proc >>(a: uint(16), b): uint(16)
   proc >>(a: uint(32), b): uint(32)
   proc >>(a: uint(64), b): uint(64)

The type of the second actual argument must be any integral type.

The ``<<`` operator shifts the bits of ``a`` left by the integer ``b``.
The new low-order bits are set to zero.

The ``>>`` operator shifts the bits of ``a`` right by the integer ``b``.
When ``a`` is negative, the new high-order bits are set to one;
otherwise the new high-order bits are set to zero.

The value of ``b`` must be non-negative.

The value of ``b`` must be less than the number of bits in ``a``.

.. _Logical_Operators:

Logical Operators
-----------------

This section describes the predefined logical operators. These operators
can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

.. _Logical_Negation_Operators:

The Logical Negation Operator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The logical negation operator is predefined for booleans and integers as
follows:



.. code-block:: chapel

   proc !(a: bool): bool
   proc !(a: int(?w)): bool
   proc !(a: uint(?w)): bool

For the boolean form, the result is the logical negation of the operand.
For the integer forms, the result is true if the operand is zero and
false otherwise.

.. _Logical_And_Operators:

The Logical And Operator
~~~~~~~~~~~~~~~~~~~~~~~~

The logical and operator is predefined over bool type. It returns true
if both operands evaluate to true; otherwise it returns false. If the
first operand evaluates to false, the second operand is not evaluated
and the result is false.

The logical and operator over expressions ``a`` and ``b`` given by


.. code-block:: chapel

   a && b 

is evaluated as the expression 

.. code-block:: chapel

   if isTrue(a) then isTrue(b) else false

The function ``isTrue`` is predefined over bool type as follows:


.. code-block:: chapel

   proc isTrue(a:bool) return a; 

Overloading the logical and operator over other types is accomplished by
overloading the ``isTrue`` function over other types.

.. _Logical_Or_Operators:

The Logical Or Operator
~~~~~~~~~~~~~~~~~~~~~~~

The logical or operator is predefined over bool type. It returns true if
either operand evaluate to true; otherwise it returns false. If the
first operand evaluates to true, the second operand is not evaluated and
the result is true.

The logical or operator over expressions ``a`` and ``b`` given by


.. code-block:: chapel

   a || b

is evaluated as the expression 

.. code-block:: chapel

   if isTrue(a) then true else isTrue(b)

The function ``isTrue`` is predefined over bool type as described
in :ref:`Logical_And_Operators`. Overloading the logical or
operator over other types is accomplished by overloading the ``isTrue``
function over other types.

.. _Relational_Operators:

Relational Operators
--------------------

This section describes the predefined relational operators. These
operators can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

.. _Ordered_Comparison_Operators:

Ordered Comparison Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The “less than” comparison operators are predefined over numeric types
as follows: 

.. code-block:: chapel

   proc <(a: int(8), b: int(8)): bool
   proc <(a: int(16), b: int(16)): bool
   proc <(a: int(32), b: int(32)): bool
   proc <(a: int(64), b: int(64)): bool

   proc <(a: uint(8), b: uint(8)): bool
   proc <(a: uint(16), b: uint(16)): bool
   proc <(a: uint(32), b: uint(32)): bool
   proc <(a: uint(64), b: uint(64)): bool

   proc <(a: int(64), b: uint(64)): bool
   proc <(a: uint(64), b: int(64)): bool

   proc <(a: real(32), b: real(32)): bool
   proc <(a: real(64), b: real(64)): bool

The result of ``a < b`` is true if ``a`` is less than ``b``; otherwise
the result is false.

The “greater than” comparison operators are predefined over numeric
types as follows: 

.. code-block:: chapel

   proc >(a: int(8), b: int(8)): bool
   proc >(a: int(16), b: int(16)): bool
   proc >(a: int(32), b: int(32)): bool
   proc >(a: int(64), b: int(64)): bool

   proc >(a: uint(8), b: uint(8)): bool
   proc >(a: uint(16), b: uint(16)): bool
   proc >(a: uint(32), b: uint(32)): bool
   proc >(a: uint(64), b: uint(64)): bool

   proc >(a: int(64), b: uint(64)): bool
   proc >(a: uint(64), b: int(64)): bool

   proc >(a: real(32), b: real(32)): bool
   proc >(a: real(64), b: real(64)): bool

The result of ``a > b`` is true if ``a`` is greater than ``b``;
otherwise the result is false.

The “less than or equal to” comparison operators are predefined over
numeric types as follows: 

.. code-block:: chapel

   proc <=(a: int(8), b: int(8)): bool
   proc <=(a: int(16), b: int(16)): bool
   proc <=(a: int(32), b: int(32)): bool
   proc <=(a: int(64), b: int(64)): bool

   proc <=(a: uint(8), b: uint(8)): bool
   proc <=(a: uint(16), b: uint(16)): bool
   proc <=(a: uint(32), b: uint(32)): bool
   proc <=(a: uint(64), b: uint(64)): bool

   proc <=(a: int(64), b: uint(64)): bool
   proc <=(a: uint(64), b: int(64)): bool

   proc <=(a: real(32), b: real(32)): bool
   proc <=(a: real(64), b: real(64)): bool

The result of ``a <= b`` is true if ``a`` is less than or equal to
``b``; otherwise the result is false.

The “greater than or equal to” comparison operators are predefined over
numeric types as follows: 

.. code-block:: chapel

   proc >=(a: int(8), b: int(8)): bool
   proc >=(a: int(16), b: int(16)): bool
   proc >=(a: int(32), b: int(32)): bool
   proc >=(a: int(64), b: int(64)): bool

   proc >=(a: uint(8), b: uint(8)): bool
   proc >=(a: uint(16), b: uint(16)): bool
   proc >=(a: uint(32), b: uint(32)): bool
   proc >=(a: uint(64), b: uint(64)): bool

   proc >=(a: int(64), b: uint(64)): bool
   proc >=(a: uint(64), b: int(64)): bool

   proc >=(a: real(32), b: real(32)): bool
   proc >=(a: real(64), b: real(64)): bool

The result of ``a >= b`` is true if ``a`` is greater than or equal to
``b``; otherwise the result is false.

The ordered comparison operators are predefined over strings as follows:


.. code-block:: chapel

   proc <(a: string, b: string): bool
   proc >(a: string, b: string): bool
   proc <=(a: string, b: string): bool
   proc >=(a: string, b: string): bool

Comparisons between strings are defined based on the ordering of the
character set used to represent the string, which is applied elementwise
to the string’s characters in order.

.. _Equality_Comparison_Operators:

Equality Comparison Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The equality comparison operators ``==`` and ``!=`` are predefined
over bool and the numeric types as follows: 

.. code-block:: chapel

   proc ==(a: int(8), b: int(8)): bool
   proc ==(a: int(16), b: int(16)): bool
   proc ==(a: int(32), b: int(32)): bool
   proc ==(a: int(64), b: int(64)): bool

   proc ==(a: uint(8), b: uint(8)): bool
   proc ==(a: uint(16), b: uint(16)): bool
   proc ==(a: uint(32), b: uint(32)): bool
   proc ==(a: uint(64), b: uint(64)): bool

   proc ==(a: int(64), b: uint(64)): bool
   proc ==(a: uint(64), b: int(64)): bool

   proc ==(a: real(32), b: real(32)): bool
   proc ==(a: real(64), b: real(64)): bool

   proc ==(a: imag(32), b: imag(32)): bool
   proc ==(a: imag(64), b: imag(64)): bool

   proc ==(a: complex(64), b: complex(64)): bool
   proc ==(a: complex(128), b: complex(128)): bool

   proc !=(a: int(8), b: int(8)): bool
   proc !=(a: int(16), b: int(16)): bool
   proc !=(a: int(32), b: int(32)): bool
   proc !=(a: int(64), b: int(64)): bool

   proc !=(a: uint(8), b: uint(8)): bool
   proc !=(a: uint(16), b: uint(16)): bool
   proc !=(a: uint(32), b: uint(32)): bool
   proc !=(a: uint(64), b: uint(64)): bool

   proc !=(a: int(64), b: uint(64)): bool
   proc !=(a: uint(64), b: int(64)): bool

   proc !=(a: real(32), b: real(32)): bool
   proc !=(a: real(64), b: real(64)): bool

   proc !=(a: imag(32), b: imag(32)): bool
   proc !=(a: imag(64), b: imag(64)): bool

   proc !=(a: complex(64), b: complex(64)): bool
   proc !=(a: complex(128), b: complex(128)): bool

The result of ``a == b`` is true if ``a`` and ``b`` contain the same
value; otherwise the result is false. The result of ``a != b`` is
equivalent to ``!(a == b)``.

The equality comparison operators are predefined over classes as
follows: 

.. code-block:: chapel

   proc ==(a: object, b: object): bool
   proc !=(a: object, b: object): bool

The result of ``a == b`` is true if ``a`` and ``b`` reference the same
storage location; otherwise the result is false. The result of
``a != b`` is equivalent to ``!(a == b)``.

Default equality comparison operators are generated for records if the
user does not define them. These operators are described
in :ref:`Record_Comparison_Operators`.

The equality comparison operators are predefined over strings as
follows: 

.. code-block:: chapel

   proc ==(a: string, b: string): bool
   proc !=(a: string, b: string): bool

The result of ``a == b`` is true if the sequence of characters in ``a``
matches exactly the sequence of characters in ``b``; otherwise the
result is false. The result of ``a != b`` is equivalent to ``!(a == b)``.

.. _Class_Operators:

Class Operators
---------------

The keywords ``owned``, ``shared``, ``borrowed``, and ``unmanaged`` act
as a prefix unary operator when specifying the management strategy for a
class type. See :ref:`Class_Types`.

The unary postfix operator ``?`` results in the nilable variant of a
class type. See :ref:`Nilable_Classes`.

The unary postfix operator ``!`` asserts that the receiver is not
storing ``nil`` and borrows from it.
See :ref:`Nilable_Classes`.

.. _Miscellaneous_Operators:

Miscellaneous Operators
-----------------------

This section describes several miscellaneous operators. These operators
can be redefined over different types using operator
overloading (:ref:`Function_Overloading`).

.. _The_String_Concatenation_Operator:

The String Concatenation Operator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The string concatenation operator ``+`` is predefined for string
arguments and returns a new string that is the concatenation of its
arguments:



.. code-block:: chapel

   proc +(s0: string, s1: string): string 

..

   *Example (string-concat.chpl)*.

   The code: 

   .. code-block:: chapel

      var x: string = "hi";
      var y: string = " there";
      var z = x + y;

   

   .. BLOCK-test-chapelnoprint

      writeln(z);

   

   .. BLOCK-test-chapeloutput

      hi there

   will cause ``z`` to be a new string containing the value
   ``"hi there"``.

.. _The_By_Operator:

The By Operator
~~~~~~~~~~~~~~~

The operator ``by`` is predefined on ranges and rectangular domains. It
is described in :ref:`By_Operator_For_Ranges` for ranges
and :ref:`Domain_Striding` for domains.

.. _The_Align_Operator:

The Align Operator
~~~~~~~~~~~~~~~~~~

The operator ``align`` is predefined on ranges and rectangular domains.
It is described in :ref:`Align_Operator_For_Ranges` for ranges
and :ref:`Domain_Alignment` for domains.

.. _The_Range_Count_Operator:

The Range Count Operator
~~~~~~~~~~~~~~~~~~~~~~~~

The operator ``#`` is predefined on ranges. It is described in
 :ref:`Count_Operator`.

.. _Let_Expressions:

Let Expressions
---------------

A let expression allows variables to be declared at the expression level
and used within that expression. The syntax of a let expression is given
by: 

.. code-block:: syntax

   let-expression:
     `let' variable-declaration-list `in' expression

The scope of the variables is the let-expression.

   *Example (let.chpl)*.

   Let expressions are useful for defining variables in the context of
   an expression. In the code 

   .. BLOCK-test-chapelnoprint

        var a = 4;
        var b = 5;
        var l =

   

   .. code-block:: chapel

        let x: real = a*b, y = x*x in 1/y

   the value determined by ``a*b`` is computed and converted to type
   real if it is not already a real. The square of the real is then
   stored in ``y`` and the result of the expression is the reciprocal of
   that value. 

   .. BLOCK-test-chapelnoprint

        ;
        writeln(l);

   

   .. BLOCK-test-chapeloutput

      0.0025

.. _Conditional_Expressions:

Conditional Expressions
-----------------------

A conditional expression is given by the following syntax: 

.. code-block:: syntax

   if-expression:
     `if' expression `then' expression `else' expression
     `if' expression `then' expression

The conditional expression is evaluated in two steps. First, the
expression following the ``if`` keyword is evaluated. Then, if the
expression evaluated to true, the expression following the ``then``
keyword is evaluated and taken to be the value of this expression.
Otherwise, the expression following the ``else`` keyword is evaluated
and taken to be the value of this expression. In both cases, the
unselected expression is not evaluated.

The ‘else’ clause can be omitted only when the conditional expression is
nested immediately inside a for or forall expression. Such an expression
is used to filter predicates as described
in :ref:`Filtering_Predicates_For`
and :ref:`Filtering_Predicates_Forall`, respectively.

   *Example (condexp.chpl)*.

   This example shows how if-then-else can be used in a context in which
   an expression is expected. The code 

   .. code-block:: chapel

      writehalf(8);
      writehalf(21);
      writehalf(1000);

      proc writehalf(i: int) {
        var half = if (i % 2) then i/2 +1 else i/2;
        writeln("Half of ",i," is ",half); 
      }

   produces the output 

   .. code-block:: printoutput

      Half of 8 is 4
      Half of 21 is 11
      Half of 1000 is 500

.. _For_Expressions:

For Expressions
---------------

A for expression is given by the following syntax: 

.. code-block:: syntax

   for-expression:
     `for' index-var-declaration `in' iteratable-expression `do' expression
     `for' iteratable-expression `do' expression

A for expression is an iterator that executes a for loop
(:ref:`The_For_Loop`), evaluates the body expression on each
iteration of the loop, and yields each resulting value.

When a for expression is used to initialize a variable, such as


.. code-block:: chapel

   var X = for iterableExpression() do computeValue();

the variable will be inferred to have an array type. The array’s domain
is defined by the ``iterable-expression`` following the same rules as
for promotion, both in the regular case :ref:`Promotion` and in
the zipper case :ref:`Zipper_Promotion`.

.. _Filtering_Predicates_For:

Filtering Predicates in For Expressions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A conditional expression that is immediately enclosed in a for
expression and does not require an else clause filters the iterations of
the for expression. The iterations for which the condition does not hold
are not reflected in the result of the for expression.

When a for expression with a filtering predicate is captured into a
variable, the resulting array has a 1-based one-dimensional domain.

   *Example (yieldPredicates.chpl)*.

   The code 

   .. code-block:: chapel

      var A = for i in 1..10 do if i % 3 != 0 then i;

   

   .. BLOCK-test-chapelpost

      writeln(A);

   

   .. BLOCK-test-chapeloutput

      1 2 4 5 7 8 10

   declares an array A that is initialized to the integers between 1 and
   10 that are not divisible by 3.
