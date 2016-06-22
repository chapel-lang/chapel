.. _chapel-evolution:

Chapel Evolution
================

Like any language, Chapel has changed over time. This page is designed to
capture significant language changes that have the possibility of breaking
existing user codes or code samples from old presentations or papers that
predated the changes.

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
:download:Quick Reference <http://chapel.cray.com/spec/quickReference.pdf> sheet.

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
