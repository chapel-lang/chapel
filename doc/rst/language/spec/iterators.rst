.. default-domain:: chpl

.. _Chapter-Iterators:

Iterators
=========

An *iterator* is a function that can generate, or *yield*, multiple
values (consecutively or in parallel) via its yield statements.

   *Open issue*.

   The parallel iterator story is under development. It is expected that
   the specification will be expanded regarding parallel iterators soon.

.. _Iterator_Function_Definitions:

Iterator Definitions
--------------------

The syntax to declare an iterator is given by: 

.. code-block:: syntax

   iterator-declaration-statement:
     privacy-specifier[OPT] 'iter' iterator-name argument-list[OPT] yield-intent[OPT] yield-type[OPT] where-clause[OPT]
     iterator-body

   iterator-name:
     identifier

   yield-intent:
     'const'
     'const ref'
     'ref'
     'param'
     'type'

   yield-type:
     : type-expression

   iterator-body:
     block-statement
     yield-statement

The syntax of an iterator declaration is similar to a procedure
declaration, with some key differences:

-  The keyword ``iter`` is used instead of the keyword ``proc``.

-  The name of the iterator cannot overload any operator.

-  ``yield`` statements may appear in the body of an iterator, but not
   in a procedure.

-  A ``return`` statement in the body of an iterator is not allowed to
   have an expression.

-  The intent and type specified after the argument list refer to the
   type yielded, not the type returned (see previous bullet). However,
   they are syntactically the same as a ``return-intent`` and a
   ``return-type``.

..

   *Open issue*.

   Iterators that yield types or params are not currently supported.

.. _The_Yield_Statement:

The Yield Statement
-------------------

The yield statement can only appear in iterators. The syntax of the
yield statement is given by 

.. code-block:: syntax

   yield-statement:
     'yield' expression ;

When an iterator is executed and a ``yield`` is encountered, the value
of the yield expression is returned to the iterator’s callsite. However,
the state of execution of the iterator is logically saved such that its
execution continues from the point immediately following the ``yield``
statement. A yield statement in an iterator that yields references must
yield an lvalue expression.

When a ``return`` is encountered, the iterator finishes without yielding
another index value. The ``return`` statements appearing in an iterator
are not permitted to have a return expression. An iterator also
completes after the last statement in the iterator is executed. An
iterator need not contain any yield statements.

.. _Iterator_Calls:

Iterator Calls
--------------

Iterators are invoked using regular call expressions: 

.. code-block:: syntax

   iteratable-call-expression:
     call-expression

All details of iterator calls, including argument passing, function
resolution, the use of parentheses versus brackets to delimit the
parameter list, and so on, are identical to procedure calls as described
in the :ref:`Procedures Chapter <Chapter-Procedures>`.

However, the result of an iterator call depends upon its context, as
described below.

.. _Iterators_in_For_and_Forall_Loops:

Iterators in For and Forall Loops
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When an iterator is accessed via a for or forall loop, the iterator is
evaluated alongside the loop body in an interleaved manner. For each
iteration, the iterator yields a value and the body is executed.

.. _Iterators_as_Arrays:

Iterators as Arrays
~~~~~~~~~~~~~~~~~~~

*Capturing an iterator* into a new variable creates a new rectangular
array storing the same elements that the iterator yielded.

An iterator can be captured by:

 * storing it into a new ``var`` or ``const`` variable declaration
 * passing it to function formal argument accepting it with ``in`` intent

In both cases, the variable or formal argument needs to be untyped or
have a compatible rectangular array type.

In other words, an iterator can be implicitly converted into an array
with matching shape and element type (see also :ref:`Implicit_Conversions`).

When an iterator is assigned to an existing array, the array and the
iterator will be iterated over with zippered iteration
(:ref:`Zipper_Iteration`) and the array elements assigned to the yielded
value.

   *Example (as-arrays.chpl)*.

   Given this iterator 

   .. code-block:: chapel

      iter squares(n: int): int {
        for i in 1..n do
          yield i * i;
      }
      var A = squares(5);

   

   .. BLOCK-test-chapelpost

      writeln(A);

   then the variable A will be an array storing: 

   .. code-block:: printoutput

      1 4 9 16 25

.. _Iterators_and_Generics:

Iterators and Generics
~~~~~~~~~~~~~~~~~~~~~~

An iterator call expression can be passed to a generic function argument
that has neither a declared type nor default value
(:ref:`Formal_Arguments_without_Types`). In this case the
iterator is passed without being evaluated. Within the generic function
the corresponding formal argument can be used as an iterator, e.g. in
for loops. The arguments to the iterator call expression, if any, are
evaluated at the call site, i.e. prior to passing the iterator to the
generic function.

.. _Recursive_Iterators:

Recursive Iterators
~~~~~~~~~~~~~~~~~~~

Recursive iterators are allowed. A recursive iterator invocation is
typically made by iterating over it in a loop.

   *Example (recursive.chpl)*.

   A post-order traversal of a tree data structure could be written like
   this: 

   .. BLOCK-test-chapelnoprint

      class Tree {
        var data: string;
        var left, right: unmanaged Tree?;
        proc deinit() {
          if left then delete left;
          if right then delete right;
        }
      }

      var tree = new unmanaged Tree("a", new unmanaged Tree("b"), new unmanaged Tree("c", new unmanaged Tree("d"), new unmanaged Tree("e")));

   

   .. code-block:: chapel

      iter postorder(tree: Tree?): string {
        if tree != nil {
          for child in postorder(tree!.left) do
            yield child;
          for child in postorder(tree!.right) do
            yield child;
          yield tree!.data;
        }
      }

   

   .. BLOCK-test-chapelnoprint

      proc Tree.writeThis(x)
      {
        var first = true;
        for node in postorder(this) {
          if first then first = false;
            else x.write(" ");
          write(node);
        }
      }
      writeln("Tree Data");
      writeln(tree);
      delete tree;

   By contrast, using calls ``postorder(tree.left)`` and
   ``postorder(tree.right)`` as stand-alone statements would result in
   generating temporary arrays containing the outcomes of these
   recursive calls, which would then be discarded.
   

   .. BLOCK-test-chapeloutput

      Tree Data
      b d e c a

.. _Iterator_Promotion_of_Scalar_Functions:

Iterator Promotion of Scalar Functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Iterator calls may be passed to a scalar function argument whose type
matches the iterator’s yielded type. This results in a promotion of the
scalar function as described in :ref:`Promotion`.

   *Example (iteratorPromotion.chpl)*.

   Given a function ``addOne(x:int)`` that accepts ``int`` values and an
   iterator ``firstN()`` that yields ``int`` values, ``addOne()`` can be
   called with ``firstN()`` as its actual argument. This pattern creates
   a new iterator that yields the result of applying ``addOne()`` to
   each value yielded by ``firstN()``.

   

   .. code-block:: chapel

      proc addOne(x:int) {
        return x + 1;
      }
      iter firstN(n:int) {
        for i in 1..n {
          yield i;
        }
      }
      for number in addOne(firstN(10)) {
        writeln(number);
      }

   

   .. BLOCK-test-chapeloutput

      2
      3
      4
      5
      6
      7
      8
      9
      10
      11

.. _Parallel_Iterators:

Parallel Iterators
------------------

Iterators used in explicit forall-statements or -expressions must be
parallel iterators. Reductions, scans and promotion over serial
iterators will be serialized.

Parallel iterators are defined for standard constructs in Chapel such as
ranges, domains, and arrays, thereby allowing these constructs to be
used with forall-statements and -expressions.

The left-most iteratable expression in a forall-statement or -expression
determines the number of tasks, the iterations each task executes, and
the locales on which these tasks execute. For ranges, default domains,
and default arrays, these values can be controlled via configuration
constants (:ref:`data_parallel_knobs`).

Domains and arrays defined using distributed domain maps will typically
implement forall loops with multiple tasks on multiple locales. For
ranges, default domains, and default arrays, all tasks are executed on
the current locale.

A more detailed definition of parallel iterators is forthcoming.
