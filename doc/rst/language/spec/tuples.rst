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
   component types ``string`` and ``real``. It is initialized such
   that the two components are ``"hello"`` and ``3.14``, respectively.
   Variables ``x2`` and ``x3`` are homogeneous 3-tuples with component
   type ``int``. Their initialization expressions specify 3-tuples of
   integers.

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
0-based, so the first component in the tuple is accessed by the index
``0``, and so forth.

   *Example (access.chpl)*.

   The loop 

   .. code-block:: chapel

      var myTuple = (1, 2.0, "three");
      for param i in 0..2 do
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
      for i in 0..2 do
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

Homogeneous tuples support iteration via standard ``for``, ``forall``
and ``coforall`` loops. These loops iterate over all of the tuple’s
elements. A loop of the form:



.. code-block:: chapel

   [for|forall|coforall] e in t do
     ...e...

where t is a homogeneous tuple of size ``n``, is semantically equivalent
to:



.. code-block:: chapel

   [for|forall|coforall] i in 0..n-1 do
     ...t(i)...

The iterator variable for a tuple iteration is a either a const value
or a reference to the tuple element type, following default intent
semantics.

Heterogeneous tuples support iteration via standard ``for`` and
``coforall`` loops.  These loops iterate over all of the tuple's
elements, giving each iteration its own index variable that is a
``const ref`` to the tuple element (note: this may change in the
future to include ``const`` or ``ref`` index variables).  Thus, a
loop of the form:

.. code-block:: chapel

  for e in t do
    ...e...

where t is a heterogeneous tuple of size ``n`` is semantically
equivalent to:

.. code-block:: chapel

  { // iteration 0
    const ref e = t(0);
    ...e...
  }
  { // iteration 1
    const ref e = t(1);
    ...e...
  }
  ...
  { // iteration n-1
    const ref e = t(n-1);
    ...e...
  }

Similarly, a `coforall` loop is equivalent to the `cobegin` statement
whose body is the series of compound statements from the serial case.

.. _Tuple_Assignment:

Tuple Assignment
----------------

In tuple assignment, the components of the tuple on the left-hand side
of the assignment operator are each assigned the components of the tuple
on the right-hand side of the assignment. These assignments occur in
component order (component zero followed by component one, etc.).

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
   the returned tuple to ``x``, and ignores the other component in the
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
   returned tuple, and ignores the other component in the returned
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
called using named-argument passing on the tuple-grouped arguments. 

In addition, tuple-grouped arguments may not be specified individually with
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

      proc g(t) where isTuple(t) && t.size == 2 && isTuple(t(1)) && t(1).size == 2 {
        writeln((t(0), t(1)(0), t(1)(1)));
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
   returns the initial component of a tuple and a function ``rest`` that
   returns a tuple containing all of the remaining components:

   .. code-block:: chapel

      proc first(t) where isTuple(t) {
        return t(0);
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

.. _Value_Tuples_and_Referential_Tuples:

Value Tuples and Referential Tuples
-----------------------------------

Throughout the next few sections, the terms referential tuple and value
tuple are used frequently to describe two different ways that tuples can
capture elements.

Tuple expressions or tuple arguments with default argument intent are two
examples of referential tuples. They store elements by reference where it
makes sense to do so. Referential tuples may be viewed as analogous to a
group of function arguments that each have default argument intent.

Tuple variables or tuple arguments with ``in`` intent are two examples of
value tuples. They store all elements by value and may store elements copy
initialized from another tuple. Value tuples may be viewed as analogous to
a group of function arguments that each have the ``in`` intent.

In short, some or all of the elements of a referential tuple may be
references, while a value tuple will never contain a reference.

.. _Tuple_Expression_Behavior:

Tuple Expression Behavior
~~~~~~~~~~~~~~~~~~~~~~~~~

Tuple expressions are a form of referential tuple. Like most other
referential tuples, tuple expressions capture each element based on the
default argument intent of the element's type.

More specifically:

-  If the default argument intent of the element's type is a variation of
   ``ref``, then the tuple expression will refer to the element instead of
   capturing it by value.
-  Otherwise, the tuple expression will capture the element by value.

Consider the following example:

   *Example (tuple-expression-behavior.chpl)*.

   .. code-block:: chapel

      record R { var x: int; }

      var a: [0..0] int;
      var i: int;
      var r: R;

      //
      // The int `i` is copied when captured into the tuple expression,
      // but `a` and `r` are not.
      //
      test((a, i, r));

      // Modify the globals, then print the tuple.
      proc test(tup) {
        a[0] = 1;
        i = 2;
        r.x = 3;

        // Outputs (1, 0, (x = 3)).
        writeln(tup);
      }

   .. BLOCK-test-chapeloutput

      (1, 0, (x = 3))

The tuple expression ``(a, i, r)`` will capture the array ``a`` and the
record ``r`` by ``ref``, but will create a copy of the integer ``i``.

   *Rationale*

   Tuple expressions and other forms of referential tuple are designed to act
   like a light-weight bundle of arguments. They behave similarly to the
   individual arguments of a function call.

   It would be prohibitively expensive for some argument types (such as
   arrays) to be copied by default when passed as an argument to a
   function call.

   The same logic applies to tuple expressions. When the default argument
   intent of a value's type is some form of ``ref``, a tuple expression will
   capture the value by reference in order to avoid a potentially
   expensive copy operation.

Tuple Variable Behavior
~~~~~~~~~~~~~~~~~~~~~~~

Tuple variables are a form of value tuple. Like other value tuples, tuple
variables will copy elements in a manner similar to passing the element
to an ``in`` intent argument.

For example, in this code:

   *Example (tuple-variable-behavior.chpl)*.

   .. code-block:: chapel

      record R { var x: int; }

      var a: [0..0] int;
      var i: int;
      var r = new R(0);

      // The tuple variable `tup` stores copies of `a`, `i`, and `r`.
      var tup = (a, i, r);

      a[0] = 1;
      i = 2;
      r.x = 3;

      // This will output (0, 0, (x = 0)).
      writeln(tup);

   .. BLOCK-test-chapeloutput

      (0, 0, (x = 0))

Initialization of the tuple variable ``tup`` will make a copy of the
array ``a``, the record ``r``, and the integer ``i``. Because ``tup`` stores
a copy of these three variables, changes made to them are not visible
in ``tup`` when it is written to standard output.

.. _Tuple_Argument_Intents:

Tuple Argument Intents
~~~~~~~~~~~~~~~~~~~~~~

A tuple argument to a function may be either a referential tuple or a value
tuple depending on its argument intent.

If the tuple argument has the default argument intent, then it is a 
referential tuple and some of its elements may be captured by ``ref``
depending on their default argument intent.

A tuple argument declared with ``const`` intent will work similarly to one
with a default intent, except that all the elements of the tuple are
considered to be ``const`` and cannot be modified.

If the tuple argument has the ``in`` or ``const in`` intent, then it is a
value tuple. All of its elements are captured by value as though each
element is passed to an ``in`` intent argument.

.. _Tuple_Argument_Behavior:

Tuple Argument Behavior
~~~~~~~~~~~~~~~~~~~~~~~

If a function argument is a tuple with the default argument intent and a
value tuple (such as a tuple variable) is passed to it, the value tuple
will be implicitly converted into a referential tuple. The resulting
referential tuple may refer to elements from the original value tuple.

A conversion from referential tuple to value tuple also occurs when a
referential tuple (such as a tuple expression) is passed to a tuple argument
that has the ``in`` intent. The referential tuple will be converted to
a value tuple by copy initializing each element.

Consider the following example:

   *Example (tuple-argument-behavior.chpl)*.

   .. code-block:: chapel

      record R { var x: int; }

      var modTup = (0, new R(0));

      //
      // The argument `tup` of `referentialTupleArg` is a referential tuple
      // due to the default argument intent.
      //
      proc referentialTupleArg(tup) {

        // Modify the module variable `modTup`.
        modTup = (3, new R(6));

        //
        // Should print (0, (x = 6)). Recall that a tuple argument with the
        // default argument intent copies integer elements.
        //
        writeln(tup);

        //
        // When `tup` is passed to `valueTupleArg`, a copy of each element
        // is made because the `valueTup` argument has the `in` intent.
        //
        valueTupleArg(tup);

        // Should still print (0, (x = 6)).
        writeln(tup);
      }

      // The argument `valueTup` is a value tuple due to the `in` intent.
      proc valueTupleArg(in valueTup) {
        valueTup = (64, new R(128));
      }

      //
      // When `modTup` is passed to `referentialTupleArg`, its first
      // element is copied while its second element is passed as though
      // it were `const ref`.
      //
      referentialTupleArg(modTup);

   .. BLOCK-test-chapeloutput

      (0, (x = 6))
      (0, (x = 6))

Tuple arguments with the ``ref`` intent are references to value tuples.
Actual arguments are restricted to value tuples (a tuple variable or a
returned tuple). Since the argument itself is passed by ``ref``, the
entire tuple will refer to a tuple from the call site.

   *Example (tuple-argument-ref-intent.chpl)*.

   .. code-block:: chapel

      //
      // Because the intent of `tup` is `ref`, only value tuples can be
      // passed to `passTupleByRef`.
      //
      proc passTupleByRef(ref tup) {
        tup = (64, 128);
      }

      var modTup = (0, 0);

      //
      // Passing `modTup` to `passTupleByRef` will construct a referential
      // tuple where each element refers to an element from `modTup`.
      //
      passTupleByRef(modTup);

      // Should print (64, 128).
      writeln(modTup);

   .. BLOCK-test-chapeloutput

      (64, 128)

.. _Tuple_Return_Behavior:

Tuple Return Behavior
~~~~~~~~~~~~~~~~~~~~~

When a tuple is returned from a function with ``ref`` or ``const ref`` return
intent, it must refer to some form of value tuple that exists outside of
the current scope. Otherwise there is a compilation error.
  
Functions that return by value always return a value tuple. If an expression
returned by such a function is a referential tuple, it will be implicitly
converted to a value tuple.

   *Example (tuple-return-behavior.chpl)*.

   .. code-block:: chapel

      record R { var x: int; }
      var a: [0..0] int;
      var i: int;
      var r = new R(0);

      //
      // The value tuple returned by `returnTuple` is passed to the
      // function `updateGlobalsAndOutput`. It is implicitly converted
      // into a referential tuple because the formal argument `tup`
      // has the default argument intent.
      //
      updateGlobalsAndOutput(returnTuple());

      //
      // The function `returnTuple` returns a value tuple that contains
      // a copy of the array `a`, the integer `i`, and the record `r`.
      //
      proc returnTuple() {
        return (a, i, r);
      }
      
      proc updateGlobalsAndOutput(tup) {
        a[0] = 1;
        i = 2;
        r.x = 3;

        //
        // Because the tuple passed to `updateGlobalsAndOutput` is a value
        // tuple and contains no references, the assignments made to `a`,
        // `i`, and `r` above are not visible in `tup` when it is printed.
        // This `writeln` will output (0, 0, (x = 0)).
        //
        writeln(tup);
      }

   .. BLOCK-test-chapeloutput

      (0, 0, (x = 0))

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
   components and determining they are equal, the next components are
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



