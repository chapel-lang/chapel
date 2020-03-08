.. _Chapter-Tuples:

Tuples
======

A tuple is an ordered set of components that allows for the
specification of a light-weight collection of values. As the examples in
this chapter illustrate, tuples are a boon to the Chapel programmer. In
addition to making it easy to return multiple values from a function,
tuples help to support multidimensional indices, to group arguments to
functions, and to specify mathematical concepts.

.. _Tuple_Types:

Tuple Types
-----------

A tuple type is defined by a fixed number (a compile-time constant) of
component types. It can be specified by a parenthesized, comma-separated
list of types. The number of types in the list defines the size of the
tuple; the types themselves specify the component types.

The syntax of a tuple type is given by: 

.. code-block:: syntax

   tuple-type:
     ( type-expression , type-list )
     ( type-expression , )

   type-list:
     type-expression
     type-expression , type-list

A homogeneous tuple is a special-case of a general tuple where the types
of the components are identical. Homogeneous tuples have fewer
restrictions for how they can be
indexed (:ref:`Tuple_Indexing`). Homogeneous tuple types can be
defined using the above syntax, or they can be defined as a product of
an integral parameter (a compile-time constant integer) and a type.

   *Rationale*.

   Homogeneous tuples require the size to be specified as a parameter (a
   compile-time constant). This avoids any overhead associated with
   storing the runtime size in the tuple. It also avoids the question as
   to whether a non-parameter size should be part of the type of the
   tuple. If a programmer requires a non-parameter value to define a
   data structure, an array may be a better choice.

..

   *Example (homogeneous.chpl)*.

   The statement 

   .. code-block:: chapel

      var x1: (string, real),
          x2: (int, int, int),
          x3: 3*int;

   defines three variables. Variable ``x1`` is a 2-tuple with component
   types ``string`` and ``real``. Variables ``x2`` and ``x3`` are
   homogeneous 3-tuples with component type ``int``. The types of ``x2``
   and ``x3`` are identical even though they are specified in different
   ways. 

   .. BLOCK-test-chapelpost

      writeln((x1,x2,x3));

   

   .. BLOCK-test-chapeloutput

      ((, 0.0), (0, 0, 0), (0, 0, 0))

Note that if a single type is delimited by parentheses, the parentheses
only impact precedence. Thus ``(int)`` is equivalent to ``int``.
Nevertheless, tuple types with a single component type are legal and
useful. One way to specify a 1-tuple is to use the overloaded ``*``
operator since every 1-tuple is trivially a homogeneous tuple.

   *Rationale*.

   Like parentheses around expressions, parentheses around types are
   necessary for grouping in order to avoid the default precedence of
   the grammar. Thus it is not the case that we would always want to
   create a tuple. The type ``3*(3*int)`` specifies a 3-tuple of
   3-tuples of integers rather than a 3-tuple of 1-tuples of 3-tuples of
   integers. The type ``3*3*int``, on the other hand, specifies a
   9-tuple of integers.

.. _Tuple_Values:

Tuple Values
------------

A value of a tuple type attaches a value to each component type. Tuple
values can be specified by a parenthesized, comma-separated list of
expressions. The number of expressions in the list defines the size of
the tuple; the types of these expressions specify the component types of
the tuple. A trailing comma is allowed.

The syntax of a tuple expression is given by: 

.. code-block:: syntax

   tuple-expression:
     ( tuple-component , )
     ( tuple-component , tuple-component-list )
     ( tuple-component , tuple-component-list , )

   tuple-component:
     expression
     `_'

   tuple-component-list:
     tuple-component
     tuple-component , tuple-component-list

An underscore can be used to omit components when splitting a tuple (see
`16.6.1 <#Assignments_in_a_Tuple>`__).

   *Example (values.chpl)*.

   The statement 

   .. code-block:: chapel

      var x1: (string, real) = ("hello", 3.14),
          x2: (int, int, int) = (1, 2, 3),
          x3: 3*int = (4, 5, 6);

   defines three tuple variables. Variable ``x1`` is a 2-tuple with
   component types ``string`` and ``real``. It is initialized such that
   the first component is ``"hello"`` and the second component is
   ``3.14``. Variables ``x2`` and ``x3`` are homogeneous 3-tuples with
   component type ``int``. Their initialization expressions specify
   3-tuples of integers. 

   .. BLOCK-test-chapelpost

      writeln((x1,x2,x3));

   

   .. BLOCK-test-chapeloutput

      ((hello, 3.14), (1, 2, 3), (4, 5, 6))

Note that if a single expression is delimited by parentheses, the
parentheses only impact precedence. Thus ``(1)`` is equivalent to ``1``.
To specify a 1-tuple, use the form with the trailing comma ``(1,)``.

   *Example (onetuple.chpl)*.

   The statement 

   .. code-block:: chapel

      var x: 1*int = (7,);

   creates a 1-tuple of integers storing the value 7.
   

   .. BLOCK-test-chapelpost

      writeln(x); 

   

   .. BLOCK-test-chapeloutput

      (7)

Tuple expressions are evaluated similarly to function calls where the
arguments are all generic with no explicit intent. So a tuple expression
containing an array does not copy the array.

When a tuple is passed as an argument to a function, it is passed as if
it is a record type containing fields of the same type and in the same
order as in the tuple.

.. _Tuple_Indexing:

Tuple Indexing
--------------

A tuple component may be accessed by an integral parameter (a
compile-time constant) as if the tuple were an array. Indexing is
1-based, so the first component in the tuple is accessed by the index
``1``, and so forth.

   *Example (access.chpl)*.

   The loop 

   .. code-block:: chapel

      var myTuple = (1, 2.0, "three");
      for param i in 1..3 do
        writeln(myTuple(i));

   uses a param loop to output the components of a tuple.
   

   .. BLOCK-test-chapeloutput

      1
      2.0
      three

Homogeneous tuples may be accessed by integral values that are not
necessarily compile-time constants.

   *Example (access-homogeneous.chpl)*.

   The loop 

   .. code-block:: chapel

      var myHTuple = (1, 2, 3);
      for i in 1..3 do
        writeln(myHTuple(i));

   uses a serial loop to output the components of a homogeneous tuple.
   Since the index is not a compile-time constant, this would result in
   an error were tuple not homogeneous. 

   .. BLOCK-test-chapeloutput

      1
      2
      3

..

   *Rationale*.

   Non-homogeneous tuples can only be accessed by compile-time constants
   since the type of an expression must be statically known.

.. _Iteration_over_Tuples:

Iteration over Tuples
---------------------

Only homogeneous tuples support iteration via standard ``for``,
``forall`` and ``coforall`` loops. These loops iterate over all of the
tuple’s elements. A loop of the form:



.. code-block:: chapel

   [for|forall|coforall] e in t do
     ...e...

where t is a homogeneous tuple of size ``n``, is semantically equivalent
to:



.. code-block:: chapel

   [for|forall|coforall] i in 1..n do
     ...t(i)...

The iterator variable for an tuple iteration is a either a const value
or a reference to the tuple element type, following default intent
semantics.

.. _Tuple_Assignment:

Tuple Assignment
----------------

In tuple assignment, the components of the tuple on the left-hand side
of the assignment operator are each assigned the components of the tuple
on the right-hand side of the assignment. These assignments occur in
component order (component one followed by component two, etc.).

.. _Tuple_Destructuring:

Tuple Destructuring
-------------------

Tuples can be split into their components in the following ways:

-  In assignment where multiple expression on the left-hand side of the
   assignment operator are grouped using tuple notation.

-  In variable declarations where multiple variables in a declaration
   are grouped using tuple notation.

-  In for, forall, and coforall loops (statements and expressions) where
   multiple indices in a loop are grouped using tuple notation.

-  In function calls where multiple formal arguments in a function
   declaration are grouped using tuple notation.

-  In an expression context that accepts a comma-separated list of
   expressions where a tuple expression is expanded in place using the
   tuple expansion expression.

.. _Assignments_in_a_Tuple:

Splitting a Tuple with Assignment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When multiple expression on the left-hand side of an assignment operator
are grouped using tuple notation, the tuple on the right-hand side is
split into its components. The number of grouped expressions must be
equal to the size of the tuple on the right-hand side. In addition to
the usual assignment evaluation order of left to right, the assignment
is evaluated in component order.

   *Example (splitting.chpl)*.

   The code 

   .. code-block:: chapel

      var a, b, c: int;
      (a, (b, c)) = (1, (2, 3));

   defines three integer variables ``a``, ``b``, and ``c``. The second
   line then splits the tuple ``(1, (2, 3))`` such that ``1`` is
   assigned to ``a``, ``2`` is assigned to ``b``, and ``3`` is assigned
   to ``c``. 

   .. BLOCK-test-chapelpost

      writeln((a, b, c));

   

   .. BLOCK-test-chapeloutput

      (1, 2, 3)

..

   *Example (aliasing.chpl)*.

   The code 

   .. code-block:: chapel

      var A = [i in 1..4] i;
      writeln(A);
      (A(1..2), A(3..4)) = (A(3..4), A(1..2));
      writeln(A);

   creates a non-distributed, one-dimensional array containing the four
   integers from ``1`` to ``4``. Line 2 outputs ``1 2 3 4``. Line 3 does
   what appears to be a swap of array slices. However, because the tuple
   is created with array aliases (like a function call), the assignment
   to the second component uses the values just overwritten in the
   assignment to the first component. Line 4 outputs ``3 4 3 4``.
   

   .. BLOCK-test-chapeloutput

      1 2 3 4
      3 4 3 4

When splitting a tuple with assignment, the underscore token can be used
to omit storing some of the components. In this case, the full
expression on the right-hand side of the assignment operator is
evaluated, but the omitted values will not be assigned to anything.

   *Example (omit-component.chpl)*.

   The code 

   .. code-block:: chapel

      proc f()
        return (1, 2);

      var x: int;
      (x,_) = f();

   defines a function that returns a 2-tuple, declares an integer
   variable ``x``, calls the function, assigns the first component in
   the returned tuple to ``x``, and ignores the second component in the
   returned tuple. The value of ``x`` becomes ``1``.
   

   .. BLOCK-test-chapelpost

      writeln(x);

   

   .. BLOCK-test-chapeloutput

      1

.. _Variable_Declarations_in_a_Tuple:

Splitting a Tuple in a Declaration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When multiple variables in a declaration are grouped using tuple
notation, the tuple initialization expression is split into its type
and/or value components. The number of grouped variables must be equal
to the size of the tuple initialization expression. The variables are
initialized in component order.

The syntax of grouped variable declarations is defined
in :ref:`Variable_Declarations`.

   *Example (decl.chpl)*.

   The code 

   .. code-block:: chapel

      var (a, (b, c)) = (1, (2, 3));

   defines three integer variables ``a``, ``b``, and ``c``. It splits
   the tuple ``(1, (2, 3))`` such that ``1`` initializes ``a``, ``2``
   initializes ``b``, and ``3`` initializes ``c``. 

   .. BLOCK-test-chapelpost

      writeln((a, b, c));

   

   .. BLOCK-test-chapeloutput

      (1, 2, 3)

Grouping variable declarations using tuple notation allows a 1-tuple to
be destructured by enclosing a single variable declaration in
parentheses.

   *Example (onetuple-destruct.chpl)*.

   The code 

   .. code-block:: chapel

      var (a) = (1, );

   initialize the new variable ``a`` to 1. 

   .. BLOCK-test-chapelpost

      writeln(a);

   

   .. BLOCK-test-chapeloutput

      1

When splitting a tuple into multiple variable declarations, the
underscore token may be used to omit components of the tuple rather than
declaring a new variable for them. In this case, no variables are
defined for the omitted components.

   *Example (omit-component-decl.chpl)*.

   The code 

   .. code-block:: chapel

      proc f()
        return (1, 2);

      var (x,_) = f();

   defines a function that returns a 2-tuple, calls the function,
   declares and initializes variable ``x`` to the first component in the
   returned tuple, and ignores the second component in the returned
   tuple. The value of ``x`` is initialized to ``1``.
   

   .. BLOCK-test-chapelpost

      writeln(x);

   

   .. BLOCK-test-chapeloutput

      1

.. _Indices_in_a_Tuple:

Splitting a Tuple into Multiple Indices of a Loop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When multiple indices in a loop are grouped using tuple notation, the
tuple returned by the iterator (:ref:`Chapter-Iterators`) is split
across the index tuple’s components. The number of indices in the index
tuple must equal the size of the tuple returned by the iterator.

   *Example (indices.chpl)*.

   The code 

   .. code-block:: chapel

      iter bar() {
        yield (1, 1);
        yield (2, 2);
      }

      for (i,j) in bar() do
        writeln(i+j);

   defines a simple iterator that yields two 2-tuples before completing.
   The for-loop uses a tuple notation to group two indices that take
   their values from the iterator. 

   .. BLOCK-test-chapeloutput

      2
      4

When a tuple is split across an index tuple, indices in the index tuple
(left-hand side) may be omitted. In this case, no indices are defined
for the omitted components.

However even when indices are omitted, the iterator is evaluated as if
an index were defined. Execution proceeds as if the omitted indices are
present but invisible. This means that the loop body controlled by the
iterator may be executed multiple times with the same set of (visible)
indices.

.. _Formal_Argument_Declarations_in_a_Tuple:

Splitting a Tuple into Multiple Formal Arguments in a Function Call
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When multiple formal arguments in a function declaration are grouped
using tuple notation, the actual expression is split into its components
during a function call. The number of grouped formal arguments must be
equal to the size of the actual tuple expression. The actual arguments
are passed in component order to the formal arguments.

The syntax of grouped formal arguments is defined
in :ref:`Function_Definitions`.

   *Example (formals.chpl)*.

   The function 

   .. code-block:: chapel

      proc f(x: int, (y, z): (int, int)) {
        // body
      }

   is defined to take an integer value and a 2-tuple of integer values.
   The 2-tuple is split when the function is called into two formals. A
   call may look like the following: 

   .. code-block:: chapel

      f(1, (2, 3));

An implicit ``where`` clause is created when arguments are grouped using
tuple notation, to ensure that the function is called with an actual
tuple of the correct size. Arguments grouped in tuples may be nested
arbitrarily. Functions with arguments grouped into tuples may not be
called using named-argument passing on the tuple-grouped arguments. In
addition, tuple-grouped arguments may not be specified individually with
types or default values (only in aggregate). They may not be specified
with any qualifier appearing before the group of arguments (or
individual arguments) such as ``inout`` or ``type``. They may not be
followed by ``...`` to indicate that there are a variable number of
them.

   *Example (implicit-where.chpl)*.

   The function ``f`` defined as 

   .. code-block:: chapel

      proc f((x, (y, z))) {
        writeln((x, y, z));
      }

   is equivalent to the function ``g`` defined as 

   .. code-block:: chapel

      proc g(t) where isTuple(t) && t.size == 2 && isTuple(t(2)) && t(2).size == 2 {
        writeln((t(1), t(2)(1), t(2)(2)));
      }

   except without the definition of the argument name ``t``.
   

   .. BLOCK-test-chapelpost

      f((1, (2, 3)));
      g((1, (2, 3)));

   

   .. BLOCK-test-chapeloutput

      (1, 2, 3)
      (1, 2, 3)

Grouping formal arguments using tuple notation allows a 1-tuple to be
destructured by enclosing a single formal argument in parentheses.

   *Example (grouping-Formals.chpl)*.

   The empty function 

   .. code-block:: chapel

      proc f((x)) { }

   accepts a 1-tuple actual with any component type.
   

   .. BLOCK-test-chapelpost

      f((1, ));
      var y: 1*real;
      f(y);

When splitting a tuple into multiple formal arguments, the arguments
that are grouped using the tuple notation may be omitted. In this case,
no names are associated with the omitted components. The call is
evaluated as if an argument were defined.

.. _Tuple_Expansion:

Splitting a Tuple via Tuple Expansion
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Tuples can be expanded in place using the following syntax: 

.. code-block:: syntax

   tuple-expand-expression:
     ( ... expression )

In this expression, the tuple defined by ``expression`` is expanded in
place to represent its components. This can only be used in a context
where a comma-separated list of components is valid.

   *Example (expansion.chpl)*.

   Given two 2-tuples 

   .. code-block:: chapel

      var x1 = (1, 2.0), x2 = ("three", "four");

   the following statement 

   .. code-block:: chapel

      var x3 = ((...x1), (...x2));

   creates the 4-tuple ``x3`` with the value
   ``(1, 2.0, "three", "four")``. 

   .. BLOCK-test-chapelpost

      writeln(x3);

   

   .. BLOCK-test-chapeloutput

      (1, 2.0, three, four)

..

   *Example (expansion-2.chpl)*.

   The following code defines two functions, a function ``first`` that
   returns the first component of a tuple and a function ``rest`` that
   returns a tuple containing all of the components of a tuple except
   for the first: 

   .. code-block:: chapel

      proc first(t) where isTuple(t) {
        return t(1);
      }
      proc rest(t) where isTuple(t) {
        proc helper(first, rest...)
          return rest;
        return helper((...t));
      }

   

   .. BLOCK-test-chapelpost

      writeln(first((1, 2, 3)));
      writeln(rest((1, 2, 3)));

   

   .. BLOCK-test-chapeloutput

      1
      (2, 3)

.. _Tuple_Operators:

Tuple Operators
---------------

.. _Tuple_Unary_Operators:

Unary Operators
~~~~~~~~~~~~~~~

The unary operators ``+``, ``-``, ``~``, and ``!`` are overloaded on
tuples by applying the operator to each argument component and returning
the results as a new tuple.

The size of the result tuple is the same as the size of the argument
tuple. The type of each result component is the result type of the
operator when applied to the corresponding argument component.

The type of every element of the operand tuple must have a well-defined
operator matching the unary operator being applied. That is, if the
element type is a user-defined type, it must supply an overloaded
definition for the unary operator being used. Otherwise, a compile-time
error will be issued.

.. _Tuple_Binary_Operators:

Binary Operators
~~~~~~~~~~~~~~~~

The binary operators ``+``, ``-``, ``*``, ``/``, ``%``, ``**``, ``&``,
``|``, ``^``, ``<<``, and ``>>`` are overloaded on tuples by applying
them to pairs of the respective argument components and returning the
results as a new tuple. The sizes of the two argument tuples must be the
same. These operators are also defined for homogeneous tuples and scalar
values of matching type.

The size of the result tuple is the same as the argument tuple(s). The
type of each result component is the result type of the operator when
applied to the corresponding pair of the argument components.

When a tuple binary operator is used, the same operator must be
well-defined for successive pairs of operands in the two tuples.
Otherwise, the operation is illegal and a compile-time error will
result.

   *Example (binary-ops.chpl)*.

   The code 

   .. code-block:: chapel

      var x = (1, 1, "1") + (2, 2.0, "2");

   creates a 3-tuple of an int, a real and a string with the value
   ``(3, 3.0, "12")``. 

   .. BLOCK-test-chapelpost

      writeln(x);

   

   .. BLOCK-test-chapeloutput

      (3, 3.0, 12)

.. _Tuple_Relational_Operators:

Relational Operators
~~~~~~~~~~~~~~~~~~~~

The relational operators ``>``, ``>=``, ``<``, ``<=``, ``==``, and
``!=`` are defined over tuples of matching size. They return a single
boolean value indicating whether the two arguments satisfy the
corresponding relation.

The operators ``>``, ``>=``, ``<``, and ``<=`` check the corresponding
lexicographical order based on pair-wise comparisons between the
argument tuples’ components. The operators ``==`` and ``!=`` check
whether the two arguments are pair-wise equal or not. The relational
operators on tuples may be short-circuiting, i.e. they may execute only
the pair-wise comparisons that are necessary to determine the result.

However, just as for other binary tuple operators, the corresponding
operation must be well-defined on each successive pair of operand types
in the two operand tuples. Otherwise, a compile-time error will result.

   *Example (relational-ops.chpl)*.

   The code 

   .. code-block:: chapel

      var x = (1, 1, 0) > (1, 0, 1);

   creates a variable initialized to ``true``. After comparing the first
   components and determining they are equal, the second components are
   compared to determine that the first tuple is greater than the second
   tuple. 

   .. BLOCK-test-chapelpost

      writeln(x);

   

   .. BLOCK-test-chapeloutput

      true

.. _Predefined_Functions_and_Methods_on_Tuples:

Predefined Functions and Methods on Tuples
------------------------------------------

.. code-block:: chapel

   proc tuple.size param

Returns the size of the tuple.


.. code-block:: chapel

   proc tuple.indices

Returns the range ``1..this.size`` representing the indices that are
legal for indexing into the tuple.


.. code-block:: chapel

   proc isHomogeneousTuple(t: tuple) param

Returns true if ``t`` is a homogeneous tuple; otherwise false.



.. code-block:: chapel

   proc isTuple(t: tuple) param

Returns true if ``t`` is a tuple; otherwise false.



.. code-block:: chapel

  proc isTupleType(type t) param

Returns true if ``t`` is a tuple of types; otherwise false.



.. code-block:: chapel

   proc max(type t) where isTupleType(t)

Returns a tuple of type ``t`` with each component set to the maximum
value that can be stored in its position.



.. code-block:: chapel

   proc min(type t) where isTupleType(t)

Returns a tuple of type ``t`` with each component set to the minimum
value that can be stored in its position.



