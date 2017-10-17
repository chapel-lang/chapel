.. _chapel-evolution:

Chapel Evolution
================

Like any language, Chapel has changed over time. This page is designed to
capture significant language changes that have the possibility of breaking
existing user codes or code samples from old presentations or papers that
predated the changes.

version 1.15, April 2017
------------------------

Version 1.15 includes several language changes to improve array semantics.

In summary:

 * arrays are always destroyed when they go out of scope and
   in particular will not be preserved by use in `begin`.
   See :ref:`readme-evolution.array-lexical-scoping`.
 * the array alias operator `=>` has been deprecated in favor of creating
   references to an array or a slice of an array with `ref` or `const ref`.
   See :ref:`readme-evolution.alias-operator-deprecated`.
 * arrays now return by value by default instead of by `ref`.
   See :ref:`readme-evolution.arrays-return-by-value`.
 * arrays now pass by `ref` or `const ref` by default, depending on
   whether or not the formal argument is modified.
   See :ref:`readme-evolution.array-default-intent`.

Additionally, the default intent for record method receivers has changed:

 * the method receiver for records is passed by `ref` or `const ref` by
   default, depending on whether or not the formal argument is modified.
   See :ref:`readme-evolution.record-this-default-intent`.

.. _readme-evolution.array-lexical-scoping:

array lexical scoping
*********************

As described in the language changes for 1.12 in
:ref:`readme-evolution.lexical-scoping`, using arrays beyond their scope
is a user error. While such a program was in error starting with Chapel
1.12, such a pattern worked until Chapel 1.15.

For example, this program will probably crash in Chapel 1.15:

.. code-block:: chapel

  proc badBegin() {
    var A: [1..10000] int;
    begin {
      A += 1;
    }
    // Error: A destroyed here at function end, but the begin could still
    // be using it!
  }



Similarly, using a slice after an array has been destroyed is an error:

.. code-block:: chapel

  proc badBeginSlice() {
    var A: [1..10000] int;
    // slice1 sets up a slice using the => operator
    // note that the => operator is deprecated (see below)
    var slice1 => A[1..1000];
    // slice2 sets up a slice by creating a reference to it
    ref slice2 = A[1..1000];
    // either way, using the slice in a begin that can continue
    // after the function declaring the array exits is an error
    begin {
      slice1 += 1;
      slice2 += 1;
    }
    // Error: A destroyed here at function end, but the begin tries to
    // use it through the slices!
  }

.. _readme-evolution.alias-operator-deprecated:

array alias operator deprecated
*******************************

The array alias operator, `=>`, has been deprecated in Chapel 1.15.
Previously, the supported way to declare one array that aliases another
(or a slice of another) was to use `=>`. Now, the supported way to do
that is to use a `ref` or `const ref` variable:

For example, before Chapel 1.15 you might have written:

.. code-block:: chapel

  // pre-1.15
  var A:[1..10] int;
  // set up a const alias of A
  const alias => A;
  // set up a mutable slice of A
  var slice => A[2..5];
  // set up a re-indexing slice of A
  var reindex:[0..9] => A;

In Chapel 1.15, use `ref` or `const ref` to create the same pattern:

.. code-block:: chapel

  var A:[1..10] int;
  // set up a const alias of A
  const ref alias = A;
  // set up a mutable slice of A
  ref slice = A[2..5];
  // set up a re-indexing slice of A
  ref reindex = A.reindex({0..9});


.. _readme-evolution.arrays-return-by-value:

arrays return by value by default
*********************************

Before Chapel 1.15, returning an array would return the array by
reference. Now arrays return by value by default. That is, the act of
returning an array can make a copy:

.. code-block:: chapel

  var A: [1..4] int;
  proc returnsArray() {
    return A;
  }
  ref B = returnsArray();
  B = 1;
  writeln(A);
  // outputs 1 1 1 1 historically
  // outputs 0 0 0 0 after Chapel 1.15


This behavior applies to array slices as well.

The old behavior is available with the `ref` return intent. Note though that
returning a `ref` to a local array is an error just like it is an error to
return a local `int` variable by `ref`.

.. code-block:: chapel

  proc returnsArrayReference() ref {
    return A;
  }


.. _readme-evolution.array-default-intent:

array default intent
********************

Before 1.15, the default intent for arrays was `ref`. The rationale for
this feature was that it was a convenience for programmers who are used
to modifying array formal arguments in their functions. Unfortunately, it
interacted poorly with return intent overloading.
Additionally, the implementation had several bugs in this area.

The following example shows how it might be surprising that return intent
overloading behaves very differently for arrays than for other types. As
the example shows, this issue affects program behavior and not just
const-checking error messages from the compiler.

.. code-block:: chapel

  // First, let's try some of these things with an
  // associative array of ints:
  {
    var D:domain(int);
    var A:[D] int;

    // This adds index 1 to the domain, implicitly
    A[1] = 10;
    writeln(D.member(1)); // outputs `true`

    // This will halt, because index 2 is not in the domain
    //var tmp = A[2];

    // This will also halt, for the same reason
    //writeln(A[3]);
  }

  // Now, let's try the same things with an array of arrays:
  {
    var D:domain(int);
    var AA:[D] [1..4] int;
    var value:[1..4] int = [10,20,30,40];

    // This adds index 4 to the domain, implicitly
    AA[4] = value;
    writeln(D.member(4)); // outputs `true`

    // This will halt, because index 5 is not in the domain
    //var tmp = AA[5];

    // It seems that this *should* halt, but it does not (pre 1.15)
    // Instead, it adds index 6 to the domain
    writeln(AA[6]);
    writeln(D.member(6)); // outputs `true` !
  }

See GitHub issue #5217 for more examples and discussion.

In order to make such programs less surprising, version 1.15 changes the default
intent for arrays to `ref` if the formal argument is modified in the function
and `const ref` if not. As a result, the above example behaves similarly for an
associative array of integers and an associative array of dense arrays.

For example, in the following program, the default intent for the formal
argument `x` is `ref`:

.. code-block:: chapel

  proc setElementOne(x) {
    // x is modified, so x has ref intent
    x[1] = 1;
  }
  var A:[1..10] int;
  setElementOne(A);

In contrast, in the following program, the default intent for the formal argument `y` is `const ref`:

.. code-block:: chapel

  proc getElementOne(y) {
    // y is not modified, so y has const ref intent
    var tmp = y[1];
  }
  const B:[1..10] int;
  getElementOne(B);


.. _readme-evolution.record-this-default-intent:

record `this` default intent
****************************

Before 1.15, the default intent for the implicit `this` argument for
record methods was implemented as `ref` but specified as `const ref`. In
1.15, this changed to `ref` if the formal `this` argument is modified in
the body of the function and `const ref` if not.

See GitHub issue #5266 for more details and discussion.

.. code-block:: chapel

  record R {
    var field: int;

    proc setFieldToOne() {
      // this is modified, so this-intent is ref
      this.field = 1;
    }

    proc printField() {
      // this is not modified, so this-intent is const ref
      writeln(this.field);
    }
  }



version 1.13, April 2016
------------------------

ref return intent
*****************

Previous versions of Chapel included an implicit `setter` param of
type `bool` for `ref` return intent functions. In addition, the compiler
created a getter and setter version of each ref return intent function.
The getter version would return an rvalue, and the setter version would
return an lvalue by ref. For example:

.. code-block:: chapel

  var x = 1;

  proc refToX() ref {
    if setter then
      return x; // setter version
    else
      return 0; // getter version
  }

  refToX() = 3;       // uses the setter version
  writeln(x);         // prints 3
  var tmp = refToX(); // uses the getter version
  writeln(tmp);       // prints 0

This functionality has changed with version 1.13. It is still possible to
write a getter and a setter, but these must be written as pair of
related functions:

.. code-block:: chapel

  var x = 1;

  // setter version
  proc refToX() ref {
    return x;
  }

  // getter version
  proc refToX() {
    return 0;
  }

  refToX() = 3;       // uses the setter version
  writeln(x);         // prints 3
  var tmp = refToX(); // uses the getter version
  writeln(tmp);       // prints 0


In some cases, when migrating code over to the new functionality,
it is useful to put the old ref return intent function into a
helper function with an explicit param `setter` argument, and then to
call that function from the getter or setter.

version 1.12, October 2015
--------------------------

.. _readme-evolution.lexical-scoping:

lexical scoping
***************

Prior to version 1.12 of Chapel, variables could be kept alive past
their lexical scopes. For example:

.. code-block:: chapel

  {
    var A: [1..n] real;
    var count$: sync int;
    var x: real;
    begin with (ref x) {
      ... A ...;
      ... count$ ...;
      ... x ...;
    }
    // ^^^ this task and its references to A, count$, and x could outlive
    // the scope in which those variables are declared.
  } // So, previously, Chapel kept these variables alive past their
    // logical scope.

Disadvantages of this approach included:

  * It moves logical stack variables (like `x` and `count$` above) to
    the heap.
  * It complicates memory management by incurring reference counting
    overhead---or causing memory leaks in cases where reference
    counting hadn't been added.
  * It was not particularly valued or utilized by users.
  * It was arguably surprising ("x still exists even though it left
    scope?").

As of Chapel 1.12 (and moreso in subsequent releases), the
implementation no longer provides this property. Instead, it is a user
error to refer to a variable after it has left scope. For example:

.. code-block:: chapel

  var flag$: sync bool; // flag$ starts empty
  {
    var x: real;
    begin with(ref x) { // create task referring to x
      flag$;            // block task until flag$ is full
      ... x ...         // user error: access to x occurs after it leaves scope
    }                   // end task
  }                     // x`s scope ends
  flag$ = true;         // fill flag$ only after x's scope closes

Code that refers to lexically scoped variables within tasks in this manner
should use `sync` variables or blocks in order to guarantee the
tasks's completion before the enclosing block exits. Note that the
more commonly used `cobegin`, `coforall`, and `forall` statements
already guarantee that the tasks they create will complete before the
enclosing block exits.

version 1.11, April 2015
------------------------

forall intents
**************

In previous versions of Chapel, the bodies of forall-loops have referred to
all lexically visible variables by reference. In this release of Chapel, such
variables are treated more consistently with the task intent semantics and
syntax introduced in versions 1.8 and 1.10 respectively (described below).

Specifically, prior to this release, a loop like the following would
represent a data race:

.. code-block:: chapel

   var sum = 0.0;
   forall a in A do sum += a;


since multiple iterations of the loop could execute simultaneously, read
the identical value from the shared variable ``sum``, update it, and write the
result back in a way that could overwrite other simultaneous updates.

Under the new forall intent semantics, such variables are treated as though
they are passed by "blank intent" to the loop body (so ``const`` for variables of
scalar type like ``sum``, preventing races in such cases). This mirrors the task
intent semantics for variables referenced within begin, ``cobegin``, and ``coforall``
constructs. As in those cases, a user can specify semantics other than the
default via a *with-clause*. For example, to restore the previous race-y
semantics, one could write:

.. code-block:: chapel

       var sum = 0.0;
       forall a in A with (ref sum) do
         sum += a;


(Of course, the safe way to write such an idiom would be to use a
reduction, or a synchronization type like ``sync`` or ``atomic``).

type select statement
*********************

Chapel has traditionally supported a ``type select`` statement that was
like a ``select`` statement for types. However, this seemed inconsistent with the
fact that other constructs like ``if...then`` operate on types directly. For that
reason, this release removed support for ``type select x``. Instead, use the
equivalent ``select x.type``.


version 1.10, October 2014
--------------------------


task intents syntax
*******************

Task intent clauses were added to Chapel in version 1.8 to
support passing variables by reference into tasks. Since then, the need to pass
variables by other intents and into other parallel constructs has arisen. But,
the previous syntax was problematic to extend to other intents, while also
generating syntactic ambiguities for other additions we wished to make to the
language.

For these reasons, a new task intent syntax was designed to cleanly support
intents other than ``ref`` (particularly in looping contexts), to address the
pending ambiguity, and to better reflect the similarity of task intents to
formal argument lists. Where previously, task constructs could be followed by a
``ref`` clause, they can now be followed by a ``with`` clause that takes a list of
intents and variables, specifying how to pass them into the task.

Thus, where one would have previously written:

.. code-block:: chapel

        begin ref(x) update(x);

        cobegin ref(x, y) {
          process(x);
          process(y);
        }

        coforall x in A ref(y) {
          process(x, y);
        }


you would now write:

.. code-block:: chapel

        begin with (ref x) update(x);

        cobegin with(ref x, ref y) {
          process(x);
          process(y);
        }

        coforall x in A with (ref y) {
          process(x, y);
        }


As of the 1.10 release, only ``ref`` intents are supported, though we plan to
expand this set of intents for the 1.11 release while also extending
forall-loops to support task intents.

'var' function return intents changed to 'ref'
**********************************************

A ``var`` function return intent has traditionally been used to indicate that
a call to the function (referred to as a *var function*) could appear in either
an r-value or l-value context. The ``var`` keyword was chosen since the function
could be used in the same contexts as a variable could.

Since that time, the ``ref`` keyword has been introduced into Chapel to
support passing variables by reference to functions. Since returning an
expression by reference supports similar capabilities as ``var`` functions require,
while also being less unusual/more orthogonal, this release replaces ``var``
function return intents with ``ref`` intents.

Thus, where one would previously write:

.. code-block:: chapel

        proc getFoo() var { ... }


now you would write:

.. code-block:: chapel

        proc getFoo() ref { ... }


The ``var`` as a return intent is deprecated and generates a warning for the
current release, after which it will be removed.


version 1.9, April 2014
--------------------------

operator precedence changes to benefit common cases
***************************************************

Historically, Chapel's operator precedence choices have tended to follow the
lead of C for operators that are common to both languages, figuring that
following an established convention would be better than forging our own path.

With this change, we modified the precedence of bitwise operators to better
reflect what we think it intuitive to users and correct what is viewed in many
circles to be a regrettable mistake in C. At the same time, we changed the
binding of ``in`` and ``..`` to support some other Chapel idioms more naturally,
like ``1..10 == 1..10``. To see the current operator precedence, refer to the
:download:Quick Reference <https://chapel-lang.org/spec/quickReference.pdf> sheet.

improved interpretation of {D}
******************************

Historically, for a domain D, Chapel has interpreted ``{D}`` as being equivalent to
``D``, inheriting a precedent of sorts set by the ZPL language, and dating from a
time when we used square brackets for both domain literals and array types.

With this change, we began interpreting ``{D}`` as a domain literal with a
single index, ``D`` (i.e., an associative domain of domains). Associative domains
of domains are not yet implemented in the language, so the new syntax is not
yet useful, but at least the incongruity of ignoring the curly brackets has
been removed.


version 1.8, October 2013
--------------------------

task functions and intents; ref-clauses Chapel has three constructs for
creating tasks: ``begin``, ``cobegin``, and ``coforall``. Historically, variable references
within tasks followed standard lexical scoping rules. For example, the
following code:

.. code-block:: chapel

        var x = 0;
        begin writeln(x);
        x += 1;


could print either the value 0 or 1, depending on whether the ``writeln()``
task was executed before or after the increment of ``x``.

With this change, we view the creation of a task as an invocation of a *task function*
— a compiler-created function that implements the task. Any references
to variables outside of the task's scope (like ``x`` in the example above) are
treated as implicit arguments to the task function, passed by blank intent.

Thus, when ``x`` is an integer, as in the above code, the task will always
print the value of 0, even if the increment of ``x`` is executed before the
``writeln()`` task, since the value of ``x`` will have been passed to the task function
by blank intent (implying a ``const`` copy for integer arguments). In contrast, if
x were a sync variable in the example above, the blank intent would cause it to
be passed by reference to the task, permitting the task to see either of the
values 0 or 1.

To return to the previous behavior, a *ref-clause* can be added to the
tasking construct to indicate that a variable should be passed to the task
function by reference rather than blank intent. For example, the following
code:


.. code-block:: chapel

          var x = 0;
          begin ref(x) writeln(x);
          x += 1;


would revert to the previous behavior, even if ``x`` were an integer.

For more information on this feature, please refer to the *Task Intents*
section of the *Task Parallelism* and *Synchronization* chapter of the language
specification.


version 1.6, October 2012
-------------------------

domain literals
***************

Chapel's domain literals were historically specified using
square brackets, based on ZPL's region syntax. Thus ``[1..m, 1..n]`` represented an
*m × n* index set.

In this change, we made domain literals use curly brackets in order to
reflect their role as sets of indices, and also to make square brackets
available for supporting array literals. Thus, ``{1..m, 1..n}`` is an *m × n* index
set, ``[1.2, 3.4, 5.6]`` is a 3-element array of reals and ``[1..m, 1..n]`` is a
2-element array of ranges.

Emacs users working on updating existing code can use the following recipe
to update old-style domain literals to the new syntax:


.. code-block:: text

          M-x query-replace-regexp: \([=|,] *\)\[\(.*?\)\]\([;|)]\)
          with: \1{\2}\3

zippered iteration
******************

Zippered iteration in Chapel was traditionally supported
simply by iterating over a tuple of values. For example, forall ``(i,a)`` in
``(1..n, A)`` would iterate over the range ``1..n`` and the n-element array
``A`` in a zippered manner.

In this change, we introduced the zip keyword to make these zippered
iterations more explicit and to permit iteration over a tuple's values
directly. Thus, the zippered iteration above would now be written:

.. code-block:: chapel

    forall (i,a) in zip(1..n, A)

ignoring tuple components/underscore
************************************

Overtime, the mechanism used to ignore a tuple component when destructuring a tuple
has changed.
Originally, an underscore was used to drop a value on the floor. For example,
given a 3-tuple ``t``, the first and last components could be stored in ``x`` and ``z``,
dropping the second component on the floor using: ``var (x, _, z) = t;``.
In version 1.1 (Apr 2010), we changed this to use a blank space instead of an
underscore, for simplicity and to permit underscore to be used as an identifier
name. Thus, the example above would have been written as ``var (x, , z) = t;``
during this time period.

However, in 2012, we changed back to using the underscore again in order to
support the specification of 1-tuples using a dangling comma, similar to
Python. Thus, dropping a tuple component is expressed as ``var (x, _, z) = t;``
again while ``(1.2, )`` is a 1-tuple of reals.


version 1.4, October 2011
--------------------------

function declaration keywords
*****************************

Prior to this change, the keyword ``def`` was used to
define both procedures and iterators; the compiler inspected the body of the
function for yield statements to determine whether it was a procedure or an
iterator.

In this change, we introduced the ``proc`` and ``iter`` keywords to distinguish
between these two cases for the sake of clarity, to avoid mistakes, to support
semantics that clearly distinguish between these cases, and to better support
specifying interfaces.
