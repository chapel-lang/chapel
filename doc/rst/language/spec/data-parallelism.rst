.. _Chapter-Data_Parallelism:

Data Parallelism
================

Chapel provides two explicit data-parallel constructs (the
forall-statement and the forall-expression) and several idioms that
support data parallelism implicitly (whole-array assignment, function
and operator promotion, reductions, and scans).

This chapter details data parallelism as follows:

-  :ref:`Forall` describes the forall statement.

-  :ref:`Forall_Expressions` describes forall expressions

-  :ref:`Forall_Intents` specifies how variables from outer
   scopes are handled within forall statements and expressions.

-  :ref:`Promotion` describes promotion.

-  :ref:`Reductions_and_Scans` describes reductions and scans.

-  :ref:`data_parallel_knobs` describes the configuration
   constants for controlling default data parallelism.

Data-parallel constructs may result in accesses to the same variable
from different tasks, possibly due to aliasing using ``ref`` argument
intents or forall intents, among others. Such accesses are subject to
the Memory Consistency Model
(:ref:`Chapter-Memory_Consistency_Model`).

.. _Forall:

The Forall Statement
--------------------

The forall statement is a concurrent variant of the for statement
described in :ref:`The_For_Loop`.

.. _forall_syntax:

Syntax
~~~~~~

The syntax of the forall statement is given by 

.. code-block:: syntax

   forall-statement:
     `forall' index-var-declaration `in' iteratable-expression task-intent-clause[OPT] `do' statement
     `forall' index-var-declaration `in' iteratable-expression task-intent-clause[OPT] block-statement
     `forall' iteratable-expression task-intent-clause[OPT] `do' statement
     `forall' iteratable-expression task-intent-clause[OPT] block-statement
     [ index-var-declaration `in' iteratable-expression task-intent-clause[OPT] ] statement
     [ iteratable-expression task-intent-clause[OPT] ] statement

As with the for statement, the indices may be omitted if they are
unnecessary and the ``do`` keyword may be omitted before a block
statement.

The square bracketed form will resort to serial iteration when
``iteratable-expression`` does not support parallel iteration. The
``forall`` form will result in an error when parallel iteration is not
available.

The handling of the outer variables within the forall statement and the
role of ``task-intent-clause`` are defined in
:ref:`Forall_Intents`.

.. _forall_semantics:

Execution and Serializability
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The forall statement evaluates the loop body once for each element
yielded by the ``iteratable-expression``. Each instance of the forall
loop’s body may be executed concurrently with the others, but this is
not guaranteed. In particular, the loop must be serializable. Details
regarding concurrency and iterator implementation are described
in \ `23.4 <#Parallel_Iterators>`__.

This differs from the semantics of the ``coforall`` loop, discussed
in :ref:`Coforall`, where each iteration is guaranteed to run
using a distinct task. The ``coforall`` loop thus has potentially higher
overhead than a forall loop with the same number of iterations, but in
cases where concurrency is required for correctness, it is essential.

In practice, the number of tasks that will be used to evaluate a
``forall`` loop is determined by the object or iterator that is
*leading* the execution of the loop, as is the mapping of iterations to
tasks.

| This concept will be formalized in future drafts of the Chapel
  specification. For now, the primer on parallel iterators in the online
  documentation provides a brief introduction:
| https://chapel-lang.org/docs/primers/parIters.html
| Please also refer to *User-Defined Parallel Zippered Iterators in
  Chapel*, published in the PGAS 2011 workshop.

Control continues with the statement following the forall loop only
after every iteration has been completely evaluated. At this point, all
data accesses within the body of the forall loop will be guaranteed to
be completed.

A ``return`` statement may not be lexically enclosed in a forall
statement. A ``yield`` statement may only be lexically enclosed in a
forall statement that is within a parallel iterator
:ref:`Parallel_Iterators`. A ``break`` statement may not be used
to exit a forall statement. A ``continue`` statement skips the rest of
the current iteration of the forall loop.

   *Example (forallStmt.chpl)*.

   In the code 

   .. BLOCK-test-chapelpre

      config const N = 5;
      var a: [1..N] int;
      var b = [i in 1..N] i;

   

   .. code-block:: chapel

      forall i in 1..N do
        a(i) = b(i);

   the user has stated that the element-wise assignments can execute
   concurrently. This loop may be executed serially with a single task,
   or by using a distinct task for every iteration, or by using a number
   of tasks where each task executes a number of iterations. This loop
   can also be written as 

   .. code-block:: chapel

      [i in 1..N] a(i) = b(i);

   

   .. BLOCK-test-chapelpost

      writeln(a);

   

   .. BLOCK-test-chapeloutput

      1 2 3 4 5

.. _forall_zipper:

Zipper Iteration
~~~~~~~~~~~~~~~~

Zipper iteration has the same semantics as described
in :ref:`Zipper_Iteration`
and :ref:`Parallel_Iterators` for parallel iteration.

.. _Forall_Expressions:

The Forall Expression
---------------------

The forall expression is a concurrent variant of the for expression
described in :ref:`For_Expressions`.

.. _forall_expr_syntax:

Syntax
~~~~~~

The syntax of a forall expression is given by 

.. code-block:: syntax

   forall-expression:
     `forall' index-var-declaration `in' iteratable-expression task-intent-clause[OPT] `do' expression
     `forall' iteratable-expression task-intent-clause[OPT] `do' expression
     [ index-var-declaration `in' iteratable-expression task-intent-clause[OPT] ] expression
     [ iteratable-expression task-intent-clause[OPT] ] expression

As with the for expression, the indices may be omitted if they are
unnecessary. The ``do`` keyword is always required in the keyword-based
notation.

As with the forall statement, the square bracketed form will resort to
serial iteration when ``iteratable-expression`` does not support
parallel iteration. The ``forall`` form will result in an error when
parallel iteration is not available.

The handling of the outer variables within the forall expression and the
role of ``task-intent-clause`` are defined in
:ref:`Forall_Intents`.

.. _Forall_Expression_Execution:

Execution
~~~~~~~~~

A forall expression is an iterator that executes a forall loop
(:ref:`Forall`), evaluates the body expression on each iteration
of the loop, and yields each resulting value.

When a forall expression is used to initialize a variable, such as


.. code-block:: chapel

   var X = forall iterableExpression() do computeValue();

the variable will be inferred to have an array type. The array’s domain
is defined by the ``iterable-expression`` following the same rules as
for promotion, both in the regular case :ref:`Promotion` and in
the zipper case :ref:`Zipper_Promotion`.

   *Example (forallExpr.chpl)*.

   The code 

   .. code-block:: chapel

      writeln(+ reduce [i in 1..10] i**2);

   

   .. BLOCK-test-chapeloutput

      385

   applies a reduction to a forall-expression that evaluates the square
   of the indices in the range ``1..10``.

The forall expression follows the semantics of the forall statement as
described in \ `27.1.2 <#forall_semantics>`__.

Zipper Iteration
~~~~~~~~~~~~~~~~

Forall expression also support zippered iteration semantics as described
in :ref:`Zipper_Iteration`
and :ref:`Parallel_Iterators` for parallel iteration.

.. _Filtering_Predicates_Forall:

Filtering Predicates in Forall Expressions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A filtering predicate is an if expression that is immediately enclosed
by a forall expression and does not have an else clause. Such an if
expression filters the iterations of the forall expression. The
iterations for which the condition does not hold are not reflected in
the result of the forall expression.

When a forall expression with a filtering predicate is captured into a
variable, the resulting array has a 0-based one-dimensional domain.

   *Example (forallFilter.chpl)*.

   The following expression returns every other element starting with
   the first: 

   .. BLOCK-test-chapelpre

      var s: [1..10] int = [i in 1..10] i;
      var result =

   

   .. code-block:: chapel

      [i in 1..s.size] if i % 2 == 1 then s(i)

   

   .. BLOCK-test-chapelpost

      ;
      writeln(result);
      writeln(result.domain);

   

   .. BLOCK-test-chapeloutput

      1 3 5 7 9
      {0..4}

.. _Forall_Intents:

Forall Intents
--------------

If a variable is referenced within the lexical scope of a forall
statement or expression and is declared outside that forall construct,
it is subject to *forall intents*, analogously to task intents
(:ref:`Task_Intents`) for task-parallel constructs. That is, the
variable is considered to be passed as an actual argument to each task
function created by the object or iterator leading the execution of the
loop. If no tasks are created, it is considered to be an actual argument
to the leader or standalone iterator itself. All references to the
variable within the forall construct implicitly refer to a *shadow
variable*, i.e. the corresponding formal argument of the task function
or the leader/standalone iterator.

When the forall construct is inside a method on a record and accesses a
field of ``this``, the field is treated as a regular variable. That is,
it is subject to forall intents and all references to this field within
the forall construct implicitly refer to the corresponding shadow
variable.

Each formal argument of a task function or iterator has the default
intent by default. For variables of primitive, enum, and class types,
this has the effect of capturing the value of the variable at task
creation time. Within the lexical scope of the forall construct, the
variable name references the captured value instead of the original
value.

| A formal can be given another intent explicitly by listing it with
  that intent in the optional ``task-intent-clause``. For example, for
  variables of most types, the ``ref`` intent allows the body of the
  forall loop to modify the corresponding original variable or to read
  its updated value after concurrent modifications. The ``in`` intent is
  an alternative way to obtain task-private variables
  (:ref:`Task_Private_Variables`). A ``reduce`` intent can be used
  to reduce values across iterations of a forall or coforall loop.
  Reduce intents are described in the *Reduce Intents* technical note in
  the online documentation:
| https://chapel-lang.org/docs/technotes/reduceIntents.html

   *Rationale*.

   A forall statement or expression may create tasks in its
   implementation. Forall intents affect those tasks in the same way
   that task intents :ref:`Task_Intents` affect the behavior of
   a task construct such as a ``coforall`` loop.

.. _Task_Private_Variables:

Task-Private Variables
----------------------

A *task-private variable* declared in a forall loop results in a
separate shadow variable in each task created by the forall loop’s
parallel iterator, as well as a "top-level" shadow variable created at
the top level of the parallel iterator itself. In contrast to regular
forall intents :ref:`Forall_Intents`, these shadow variables are
unrelated to outer variables of the same name, if any.

A given shadow variable is created at the start and destroyed at the end
of its task. Within the lexical scope of the body of the forall
statement or expression, the variable name refers to the shadow variable
created in the task that executed the current yield statement.

The "top-level" shadow variable is created at the start and destroyed at
the end of the parallel iterator. It is referenced in those iterations
of the forall loop that are due to "top-level" yields, i.e. yields that
are outside any of the task constructs that the iterator may have.

The syntax of a task-private variable declaration in a forall
statement’s with-clause is:



.. code-block:: syntax

   task-private-var-decl:
     task-private-var-kind identifier type-part[OPT] initialization-part[OPT]

   task-private-var-kind:
     `const'
     `var'
     `ref'

The declaration of a ``const`` or ``var`` task-private variable must
have at least one of ``type-part`` and ``initialization-part``. A
``ref`` task-private variable must have ``initialization-part`` and
cannot have ``type-part``. A ``ref`` shadow variable is a reference to
the ``initialization-part`` as calculated at the start of the
corresponding task or the iterator. ``ref`` shadow variables are never
destroyed.

..

   *Example (task-private-variable.chpl)*.

   In the following example, the ``writeln()`` statement will observe
   the first shadow variable 4 times: twice each for the yields "before
   coforall" and "after coforall". An additional shadow variable will be
   created and observed twice for each of the three ``coforall`` tasks.
   

   .. code-block:: chapel

      var cnt: atomic int;                     // count our shadow variables
      record R { var id = cnt.fetchAdd(1); }

      iter myIter() { yield ""; }              // serial iterator, unused

      iter myIter(param tag) where tag == iterKind.standalone {
        for 1..2 do
          yield "before coforall";             // shadow var 0 ("top-level")
        coforall 1..3 do
          for 1..2 do
            yield "inside coforall";           // shadow vars 1..3
        for 1..2 do
          yield "after coforall";              // shadow var 0, again
      }

      forall str in myIter()
        with (var tpv: R)                      // declare a task-private variable
      do
        writeln("shadow var: ", tpv.id, "  yield: ", str);

   

   .. BLOCK-test-chapelprediff

      #!/usr/bin/env sh
      testname=$1
      outfile=$2
      sort $outfile > $outfile.2
      mv $outfile.2 $outfile

   

   .. BLOCK-test-chapeloutput

      shadow var: 0  yield: after coforall
      shadow var: 0  yield: after coforall
      shadow var: 0  yield: before coforall
      shadow var: 0  yield: before coforall
      shadow var: 1  yield: inside coforall
      shadow var: 1  yield: inside coforall
      shadow var: 2  yield: inside coforall
      shadow var: 2  yield: inside coforall
      shadow var: 3  yield: inside coforall
      shadow var: 3  yield: inside coforall

.. _Promotion:

Promotion
---------

A function that expects one or more scalar arguments but is called with
one or more arrays, domains, ranges, or iterators is promoted if the
element types of the arrays, the index types of the domains and/or
ranges, or the yielded types of the iterators can be resolved to the
type of the argument. The rules of when an overloaded function can be
promoted are discussed in :ref:`Function_Resolution`.

Functions that can be promoted include procedures, operators, casts, and
methods. Also note that since class and record field access is performed
with getter methods (:ref:`Getter_Methods`), field access can
also be promoted.

If the original function returns a value or a reference, the
corresponding promoted expression is an iterator yielding each computed
value or reference.

When a promoted expression is used to initialize a variable, such as
``var X = A.x;`` in the above example, the variable’s type will be
inferred to be an array. The array’s domain is defined by the expression
that causes promotion:

================ ============================================
input expression resulting array’s domain
================ ============================================
array            that array’s domain
domain           that domain
range            one-dimensional domain built from that range
iterator         0-based one-dimensional domain
================ ============================================

..

   *Future*.

   We would like to allow the iterator author to specify the shape of
   the iterator, i.e. the domain of the array that would capture the
   result of the corresponding promoted expression, such as 

   .. code-block:: chapel

      var myArray = myScalarFunction(myIterator());

   This will be helpful, for example, when the iterator yields one value
   per an array or domain element that it iterates over internally.

   *Example (promotion.chpl)*.

   Given the array 

   .. code-block:: chapel

      var A: [1..5] int = [i in 1..5] i;

   and the function 

   .. code-block:: chapel

      proc square(x: int) return x**2;

   then the call ``square(A)`` results in the promotion of the
   ``square`` function over the values in the array ``A``. The result is
   an iterator that returns the values ``1``, ``4``, ``9``, ``16``, and
   ``25``. 

   .. BLOCK-test-chapelnoprint

      for s in square(A) do writeln(s);

   

   .. BLOCK-test-chapeloutput

      1
      4
      9
      16
      25

..

   *Example (field-promotion.chpl)*.

   Given an array of points, such as ``A`` defined below: 

   .. code-block:: chapel

      record Point {
        var x: real;
        var y: real;
      }
      var A: [1..5] Point = [i in 1..5] new Point(x=i, y=i);

   the following statement will create a new array consisting of the
   ``x`` field value for each value in A: 

   .. code-block:: chapel

      var X = A.x;

   and the following call will set the ``y`` field values for each
   element in A to 1.0: 

   .. code-block:: chapel

      A.y = 1.0;

   

   .. BLOCK-test-chapelnoprint

      writeln(X);
      writeln(A);

   

   .. BLOCK-test-chapeloutput

      1.0 2.0 3.0 4.0 5.0
      (x = 1.0, y = 1.0) (x = 2.0, y = 1.0) (x = 3.0, y = 1.0) (x = 4.0, y = 1.0) (x = 5.0, y = 1.0)

.. _Promotion_Default_Arguments:

Default Arguments
~~~~~~~~~~~~~~~~~

When a call is promoted and that call relied upon default
arguments (:ref:`Default_Values`), the default argument
expression can be evaluated many times. For example:

   *Example (promotes-default.chpl)*.

   

   .. code-block:: chapel

        var counter: atomic int;

        proc nextCounterValue():int {
          var i = counter.fetchAdd(1);
          return i;
        }

        proc assignCounter(ref x:int, counter=nextCounterValue()) {
          x = counter;
        }

   Here the function assignCounter has a default argument providing the
   next value from an atomic counter as the value to set.

   

   .. code-block:: chapel

        var A: [1..5] int;
        assignCounter(A);

   The assignCounter call uses both the default argument for counter as
   well as promotion. When these features are combined, the default
   argument will be evaluated once per promoted element. As a result,
   after this command, A will contain the elements 0 1 2 3 4 in some
   order.

   

   .. BLOCK-test-chapelnoprint

      writeln(A.sorted());

   

   .. BLOCK-test-chapeloutput

      0 1 2 3 4

.. _Zipper_Promotion:

Zipper Promotion
~~~~~~~~~~~~~~~~

Promotion also supports zippered iteration semantics as described
in :ref:`Zipper_Iteration`
and :ref:`Parallel_Iterators` for parallel iteration.

Consider a function ``f`` with formal arguments ``s1``, ``s2``, ... that
are promoted and formal arguments ``a1``, ``a2``, ... that are not
promoted. The call 

.. code-block:: chapel

   f(s1, s2, ..., a1, a2, ...)

is equivalent to 

.. code-block:: chapel

   [(e1, e2, ...) in zip(s1, s2, ...)] f(e1, e2, ..., a1, a2, ...)

The usual constraints of zipper iteration apply to zipper promotion so
the promoted actuals must have the same shape.

A zipper promotion can be captured in a variable, such as
``var X = f(s1, s2, ..., a1, a2, ...);`` using the above example. If so,
the domain of the resulting array is defined by the first argument that
causes promotion. The rules are the same as in the non-zipper case.

   *Example (zipper-promotion.chpl)*.

   Given a function defined as 

   .. code-block:: chapel

      proc foo(i: int, j: int) {
        return (i,j);
      }

   and a call to this function written 

   .. code-block:: chapel

      writeln(foo(1..3, 4..6));

   then the output is 

   .. code-block:: printoutput

      (1, 4) (2, 5) (3, 6)

.. _Whole_Array_Operations:

Whole Array Operations and Evaluation Order
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Whole array operations are a form of promotion as applied to operators
rather than functions.

Whole array assignment is one example. It is is implicitly parallel. The
array assignment statement: 

.. code-block:: chapel

   LHS = RHS;

is equivalent to 

.. code-block:: chapel

   forall (e1,e2) in zip(LHS,RHS) do
     e1 = e2;

The semantics of whole array assignment and promotion are different from
most array programming languages. Specifically, the compiler does not
insert array temporaries for such operations if any of the right-hand
side array expressions alias the left-hand side expression.

   *Example*.

   If ``A`` is an array declared over the indices ``1..5``, then the
   following codes are not equivalent: 

   .. code-block:: chapel

      A[2..4] = A[1..3] + A[3..5];

   and 

   .. code-block:: chapel

      var T = A[1..3] + A[3..5];
      A[2..4] = T;

   This follows because, in the former code, some of the new values that
   are assigned to ``A`` may be read to compute the sum depending on the
   number of tasks used to implement the data parallel statement.

.. _Reductions_and_Scans:

Reductions and Scans
--------------------

Chapel provides reduction and scan expressions that apply operators to
aggregate expressions in stylized ways. Reduction expressions collapse
the aggregate’s values down to a summary value. Scan expressions compute
an aggregate of results where each result value stores the result of a
reduction applied to all of the elements in the aggregate up to that
expression. Chapel provides a number of predefined reduction and scan
operators, and also supports a mechanism for the user to define
additional reductions and scans
(Chapter `[User_Defined_Reductions_and_Scans] <#User_Defined_Reductions_and_Scans>`__).

.. _reduce:

Reduction Expressions
~~~~~~~~~~~~~~~~~~~~~

A reduction expression applies a reduction operator to an aggregate
expression, collapsing the aggregate’s dimensions down into a result
value (typically a scalar or summary expression that is independent of
the input aggregate’s size). For example, a sum reduction computes the
sum of all the elements in the input aggregate expression.

The syntax for a reduction expression is given by: 

.. code-block:: syntax

   reduce-expression:
     reduce-scan-operator `reduce' iteratable-expression
     class-type `reduce' iteratable-expression

   reduce-scan-operator: one of
     + * && || & | ^ `min' `max' `minloc' `maxloc'

Chapel’s predefined reduction operators are defined by
``reduce-scan-operator`` above. In order, they are: sum, product,
logical-and, logical-or, bitwise-and, bitwise-or, bitwise-exclusive-or,
minimum, maximum, minimum-with-location, and maximum-with-location. The
minimum reduction returns the minimum value as defined by the ``<``
operator. The maximum reduction returns the maximum value as defined by
the ``>`` operator. The minimum-with-location reduction returns the lowest
index position with the minimum value (as defined by the ``<`` operator).
The maximum-with-location reduction returns the lowest index position
with the maximum value (as defined by the ``>`` operator). When a minimum,
maximum, minimum-with-location, or maximum-with-location reduction
encounters a NaN, the result is a NaN.

The expression on the right-hand side of the ``reduce`` keyword can be
of any type that can be iterated over, provided the reduction operator
can be applied to the values yielded by the iteration. For example, the
bitwise-and operator can be applied to arrays of boolean or integral
types to compute the bitwise-and of all the values in the array.

For the minimum-with-location and maximum-with-location reductions, the
argument on the right-hand side of the ``reduce`` keyword must be a
2-tuple. Its first component is the collection of values for which the
minimum/maximum value is to be computed. The second argument component
is a collection of indices with the same size and shape that provides
names for the locations of the values in the first component. The
reduction returns a tuple containing the minimum/maximum value in the
first argument component and the value at the corresponding location in
the second argument component.

   *Example (reduce-loc.chpl)*.

   The first line below computes the smallest element in an array ``A``
   as well as its index, storing the results in ``minA`` and
   ``minALoc``, respectively. It then computes the largest element in a
   forall expression making calls to a function ``foo()``, storing the
   value and its number in ``maxVal`` and ``maxValNum``.
   

   .. BLOCK-test-chapelnoprint

      config const n = 10;
      const D = {1..n};
      var A: [D] int = [i in D] i % 7;
      proc foo(x) return x % 7;

   

   .. code-block:: chapel

      var (minA, minALoc) = minloc reduce zip(A, A.domain); 
      var (maxVal, maxValNum) = maxloc reduce zip([i in 1..n] foo(i), 1..n);

   

   .. BLOCK-test-chapelnoprint

      writeln((minA, minALoc));
      writeln((maxVal, maxValNum));

   

   .. BLOCK-test-chapeloutput

      (0, 7)
      (6, 6)

User-defined reductions are specified by preceding the keyword
``reduce`` by the class type that implements the reduction interface as
described
in :ref:`Chapter-User_Defined_Reductions_and_Scans`.

.. _scan:

Scan Expressions
~~~~~~~~~~~~~~~~

A scan expression applies a scan operator to an aggregate expression,
resulting in an aggregate expression of the same size and shape. The
output values represent the result of the operator applied to all
elements up to and including the corresponding element in the input.

The syntax for a scan expression is given by: 

.. code-block:: syntax

   scan-expression:
     reduce-scan-operator `scan' iteratable-expression
     class-type `scan' iteratable-expression

The predefined scans are defined by ``reduce-scan-operator``. These are
identical to the predefined reductions and are described
in :ref:`reduce`.

The expression on the right-hand side of the scan can be of any type
that can be iterated over and to which the operator can be applied.

   *Example*.

   Given an array 

   .. code-block:: chapel

      var A: [1..3] int = 1;

   that is initialized such that each element contains one, then the
   code 

   .. code-block:: chapel

      writeln(+ scan A);

   outputs the results of scanning the array with the sum operator. The
   output is 

   .. code-block:: printoutput

      1 2 3

User-defined scans are specified by preceding the keyword ``scan`` by
the class type that implements the scan interface as described in
Chapter \ `[User_Defined_Reductions_and_Scans] <#User_Defined_Reductions_and_Scans>`__.

.. _data_parallel_knobs:

Configuration Constants for Default Data Parallelism
----------------------------------------------------

The following configuration constants are provided to control the degree
of data parallelism over ranges, default domains, and default arrays:

============================= ======== =============================================================
**Config Const**              **Type** **Default**
============================= ======== =============================================================
``dataParTasksPerLocale``     ``int``  top level ``.maxTaskPar``   (see :ref:`Locale_Methods`)
``dataParIgnoreRunningTasks`` ``bool`` ``true``
``dataParMinGranularity``     ``int``  ``1``
============================= ======== =============================================================

The configuration constant ``dataParTasksPerLocale`` specifies the
number of tasks to use when executing a forall loop over a range,
default domain, or default array. The actual number of tasks may be
fewer depending on the other two configuration constants. A value of
zero results in using the default value.

The configuration constant ``dataParIgnoreRunningTasks``, when true, has
no effect on the number of tasks to use to execute the forall loop. When
false, the number of tasks per locale is decreased by the number of
tasks that are already running on the locale, with a minimum value of
one.

The configuration constant ``dataParMinGranularity`` specifies the
minimum number of iterations per task created. The number of tasks is
decreased so that the number of iterations per task is never less than
the specified value.

For distributed domains and arrays that have these same configuration
constants (*e.g.*, Block and Cyclic distributions), these same module
level configuration constants are used to specify their default behavior
within each locale.
