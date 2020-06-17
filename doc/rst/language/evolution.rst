.. _chapel-evolution:

Chapel Evolution
================

Like any language, Chapel has changed over time. This page is designed
to describe significant language changes that have a high likelihood
of breaking existing user codes or code samples from presentations or
papers that predated the changes.

Note that the compiler flag ``--warn-unstable`` is available and can be
useful when migrating programs to the current version of the language.
The purpose of this flag is to identify portions of a program that use a
language feature which has changed meaning.  It also flags features that
are considered unstable and may change in the future.


version 1.22, April 2020
------------------------

0- vs. 1-based Indexing
***********************

Version 1.22 makes a major breaking change to Chapel with respect to
indexing for cases that involve implicit indices.  Historically,
Chapel has used 1-based indexing for such cases, where it now uses
0-based indexing.

The major types that are affected by this change are tuples, strings,
``bytes``, and lists.  In addition, arrays that don't have a
well-defined index set also start at 0.  Such cases include array
literals or inferred-type arrays formed by capturing a general
iterator expression.

This change also has a ripple-down effect to features and routines
related to these types.  For example, varargs arguments are equivalent
to tuples in Chapel, so inherit their 0-based indexing.  Similarly,
queries on rectangular domains and arrays are based on tuples,
so their dimensions are now numbered from 0 as well.
Certain library routines such as ``find()`` on strings used to return 0
when no match was found, but now return -1 in order to avoid returning
a legal string index.

The following sections summarize the rationale for this change and
then provide some tips for updating existing Chapel code.

Rationale for 0- vs. 1-based Indexing
*************************************

In the original design of Chapel, we hoped to make the language as
neutral to 1- vs. 0-based indexing as possible, to avoid running afoul
of the strong emotions that such choices evoke in users when it
doesn't match their preference.  As a result, Chapel's primary types
for parallel computation on regular collections of data—namely, its
ranges and rectangular domains, as well as rectangular arrays defined
by ranges or domains—require users to specify both low and high
bounds.  Happily, these core features are not affected by this change
in Chapel 1.22, so codes relying solely on such features will not
require updates.

However, for other types such as tuples and strings, we were forced to
make a decision.  At the time of Chapel's inception, the main
languages from which we were trying to attract users were C/C++, Java,
Fortran, and Matlab.  Since half of these languages used 0-based
indexing and the other half used 1-based, there didn't seem to be an
obvious best answer.  In the end, we decided to go with 1-based
indexing on the argument that we were striving to create a productive
language, and that counting from 1 is arguably most natural for most
people.

Over time, however, the vast majority of newer languages that we look
to for users or inspiration—most notably Python, Swift, and Rust—have
been almost exclusively 0-based.  Meanwhile, very few notable new
languages have used 1-based indexing.

Furthermore, when polled, the vast majority of active Chapel users
expressed a strong preference for 0-based programming, given the
choice (though there were also notable outliers, particularly from the
Fortran community).  We also realized (a) that Chapel's design should
be more concerned with lowering barriers for existing programmers than
for non-programmers; and (b) that even though we had arguably biased
the original design in favor of Fortran programmers, most of Chapel's
early adopters have come from C/C++ and Python backgrounds.

Based on this, we undertook an experiment to see what it would take to
convert from 1-based to 0-based programming.  Reviewing Chapel's
~10,000 tests and modules resulted in changes to ~1,000 of them.  We
also updated some significant applications such as Arkouda and Cray
HPO.  While the overall effort of making the change was not
insignificant, it also wasn't particularly difficult for the most
part.  Overall, our finding was that in cases where the changes
weren't simply neutral in their impact on style, it almost always
benefitted the code in terms of clarity, because there tended to
be fewer adjustments of +/- 1 in the code.

For these reasons, we decided to bite the bullet and make the switch
now, while we felt we still could, rather than later when it would
clearly be too late to do so and cause more of a revolt among our
users.

Index-neutral Features
**********************

This experience also led to a number of new programming features in
Chapel 1.21 designed to help write code in more of an index-neutral
style.  Chief among these are new ``.indices`` queries on most of the
relevant types as well as support for loops over heterogeneous tuples.
We also introduced features that we found to be useful in updating
code, such as support for open-interval ranges and ``.first`` and
``.last`` queries on enumerated types.  To this end, even though Chapel
still has cases that require making this 0- vs. 1-based indexing
decision, we encourage code to be written in an index-neutral style
whenever possible, and believe that most common code patterns can be.

Tips for Updating Existing Chapel code
**************************************

The following are some tips for updating codes based on our
experiences:

* First, updating code is easiest when it has some sort of testing
  infrastructure that can be used to validate that its behavior is
  unchanged.  If you don't already have such testing for your code, it
  may be worthwhile to invest in creating some before attempting this
  upgrade.

* Next, when transitioning code to Chapel 1.22, make sure to compile
  it with neither ``--fast`` nor ``--no-checks`` enabled so that bounds
  checks are turned on in the generated code.  In cases where a
  program is accessing all of the elements of a collection (as is
  common for tuples) this will help identify data structures that
  require updates.  When you do get an out-of-bounds error, don't
  simply update the specific access, but use it as a cue to look
  through the code for other references to that variable that will
  also need updating.

* When possible, try rewriting your updated code to use an
  index-neutral style of programming.  For example, given code like
  this:

  .. code-block:: chapel

      var t: 2*int = ...;

      var x = t(1),
          y = t(2);

      for i in 1..2 do
        writeln("t(", i, ") = ", t(i));

  It would be reasonable to rewrite it like this:

  .. code-block:: chapel

      var t: 2*int = ...;

      var x = t(0),
          y = t(1);

      for i in 0..1 do
        writeln("t(", i, ") = ", t(i));

  But arguably preferable to update it like this:

  .. code-block:: chapel

      var t: 2*int = ...;

      var (x, y) = t;

      for i in t.indices do
        writeln("t(", i, ") = ", t(i));

  If you have a pattern that you're trying to write in an
  index-neutral style, but can't, don't hesitate to `ask for tips
  <https://chapel-lang.org/users.html>`_.
        

* Some common pitfalls to check for in your code include:

  - Search for queries on the dimensions of rectangular domains and
    arrays.  For example, ``myDomain.dim(1)``, ``myDomain.low(1)``,
    ``myDomain.high(1)``, or ``myDomain.stride(1)`` will need to be
    updated to reflect that array dimensions now count from 0 rather
    than 1.  These will result in out-of-bounds errors in cases where
    you query all dimensions of an array, making them easy to find;
    but it can be worthwhile to grep your code for such patterns to
    make sure you don't miss any.

  - Also search for instances of ``find()`` or ``rfind()`` that are
    relying on comparisons to zero/nonzero values, and update them to
    compare against -1.  For example, patterns like ``if
    mystring.find('z')`` need to be updated to ``if mystring.find('z')
    != -1``.

  - Search for instances of ``split()``.  A common idiom is to write
    ``var substrs = mystring.split(5);`` and then to index into the
    result using ``substrs[1]``, ``substrs[2]``, etc.  Since this is
    an instance of capturing an iterator expression, you'll either
    need to subtract one from the indices, or else declare `substrs`
    to have a specific type, like ``var substrs: [1..5] string =
    mystring.split(5);``

  - Search for varargs functions and make sure they are updated to use
    0-based indexing or index-neutral features.

  - Search for any calls to ``Reflection.getField*()`` and update
    those the cases that use integer indices to reflect 0-based
    numbering.

  - Look for any calls on lists that use explicit offsets, as these
    will likely need updates.  For example ``mylist.pop(1);`` will
    need to become ``mylist.pop(0);``

  - Some other common string patterns to look for in your code that
    `may` indicate something requiring an update include:

    - ``1..``
    - ``[1]``
    - ``(1)``
    - ``[2]``
    - ``(2)``

  - Think about whether there are other places in your code that
    compute index values numerically yet which don't have obvious
    syntactic cues.


Need Help?
**********

If you are able to share your code with us and would like help
updating it to Chapel 1.22, please don't hesitate to `ask for help
<https://chapel-lang.org/users.html>`_.  Given our experience in
updating the Chapel code base itself, we have found it fairly easy to
update most codes, even when we're unfamiliar with them.


version 1.21, April 2020
------------------------

Version 1.21 made several improvements related to record initialization,
assignment, and deinitialization.

In summary:

 * Some patterns of default initialization followed by assignment are now
   converted to initialization. See :ref:`readme-evolution.split-init`.
 * Some patterns of copy initialization followed by deinitialization are
   converted to move initialization. See :ref:`readme-evolution.copy-elision`.
 * The result of a nested call expression can now be deinitialized at the end of
   the containing statement. See :ref:`readme-evolution.statement-deinit`.

.. _readme-evolution.split-init:

split initialization
********************

Split initialization a new language feature in 1.21 that is described in
the language specification - see :ref:`Split_Initialization`.

Consider the following example:

.. code-block:: chapel

  var x: myRecord;    // default-initialization in 1.20
  x = new myRecord(); // assignment in 1.20 -- initialization in 1.21

In 1.21, instead of default-initializing ``x`` and then assigning to it,
``x`` will be initialized on the second line.

Note that split initialization also changes the copy and assignment
behavior of ``out`` intent formal arguments.

Occasionally programs that are written to test assignment (separately
from copy initialization) need to avoid split initialization. One way to
do so is to add a mention of the variable immediately after it is
declared, as in the following code:

.. code-block:: chapel

  var x: myRecord;
  x; // adding this mention prevents split-initialization
     // instead, x is default-initialized at its declaration point above
  x = new myRecord();

.. _readme-evolution.copy-elision:

copy elision
************

Copy elision a new language feature in 1.21.
When the last mention of a variable is the source of a copy-initialization,
the copy-initialization is replaced by move-initialization.

For example:

.. code-block:: chapel

  class MyClass {
    var field;
    proc init(in arg) {
      this.field = arg;
    }
  }

  proc copyElisionExample() {
    var a = new myRecord();
    var b = a;             // now move-initializes `b` from `a`
    return new MyClass(b); // now move-initializes the field from `b`
  }


.. _readme-evolution.statement-deinit:

deinitialization point of nested call expressions
*************************************************

In 1.20, all variables are deinitialized at the end of the enclosing
block. That changed in 1.21. Compiler-introduced temporary
variables storing the result of a nested call expression can now be
deinitialized at the end of a statement. In particular, results of nested
call expressions are now deinitialized at the end of the statement unless the
statement is initializing a user variable.

For example:

.. code-block:: chapel

  proc makeRecord() {
    return new myRecord();
  }
  proc f(arg) {
    return arg;
  }
  proc deinitExample() {
    f(makeRecord());
    // Compiler converts the above statement into
    //   var tmp = makeRecord();
    //   f(tmp);
    // In 1.20, tmp is destroyed at the end of the block.
    // In 1.21, tmp is destroyed at the end of the above statement.

    var x = f(makeRecord());
    // In both 1.20 and 1.21, the temporary storing the result of
    // `makeRecord()` is deinitialized at the end of the block.
  }


version 1.20, September 2019
----------------------------

Version 1.20 made language changes that address problems with classes.

In summary:

 * variables of class type can no longer store `nil` by default but can
   opt-in to possibly being `nil` with `?`.
   See :ref:`readme-evolution.nilability-changes`
 * certain casts have changed behavior to support nilability changes
   See :ref:`readme-evolution.nilability-and-casts`
 * un-decorated class types such as `MyClass` (as opposed to `borrowed
   MyClass`) now have generic management
   See :ref:`readme-evolution.undecorated-classes-generic-management`
 * arguments with `owned` or `shared` declared type now use `const ref`
   default intent rather than `in` intent.
   See :ref:`readme-evolution.new-default-intent-for-owned-and-shared`
 * ``new C`` now creates an `owned C` rather than a `borrowed C`
   See :ref:`readme-evolution.new-C-is-owned`


.. _readme-evolution.nilability-changes:

nilability changes
******************

Previous to 1.20, variables of class type could always store ``nil``.  In
1.20, only nilable class types can store ``nil``. Non-nilable class types
and nilable class types are different types. A class type expression
such as ``borrowed C`` indicates a non-nilable class type.

As an aid in migrating code to this change, the flag ``--legacy-classes``
will disable this new behavior.

Consider the following example:

.. code-block:: chapel

  class C {
    var x:int;
  }

  var a: borrowed C = (new owned C()).borrow();

In 1.19, variables of type ``borrowed C`` could store ``nil``:

.. code-block:: chapel

  var b: borrowed C = nil;
  var c: borrowed C;
  a = nil;

The 1.20 compiler will report errors for all 3 of these lines. To resolve
the errors, it is necessary to use a nilable class type. Nilable class
types are written with ``?`` at the end of the type. In this example:

.. code-block:: chapel

  var a: borrowed C? = (new owned C()).borrow();
  var b: borrowed C? = nil;
  var c: borrowed C?;
  a = nil;

Implicit conversions are allowed from non-nilable class types to nilable
class types.

When converting variables to nilable types to migrate code, there will be
situations in which it is known by the developer that a variable cannot
be ``nil`` at a particular point in the code. For example:

.. code-block:: chapel

  proc f(arg: borrowed C) { }
  proc C.method() { }

  config const choice = true;
  var a: owned C?;
  if choice then
    a = new owned C(1);
  else
    a = new owned C(2);

  f(a);
  a.method();

Errors on the last two lines can be resolved by writing

.. code-block:: chapel

  f(a!);
  a!.method();

where here the ``!`` asserts that the value is not ``nil`` and it can
halt if the value is ``nil``.

Note that in ``prototype`` and implicit file-level modules, the compiler
will automatically add ``!`` on method calls with nilable receivers
(i.e. in the ``a.method()`` case above).

In the above case, a cleaner way to write the conditional would be to
create a function that always returns a value or throws if there is a
problem. For example:

.. code-block:: chapel

  proc makeC() throws {
    var a: owned C?;
    if choice then
      a = new owned C(1);
    else
      a = new owned C(2);
    return a:owned C; // this cast throws if a stores nil
  }

  proc main() throws {
    var a:owned C = makeC();
    f(a);
    a.method();
  }


.. _readme-evolution.nilability-and-casts:

nilability and casts
********************

Because casts to class types should necessarily return something of the
requested type, and because many class types now cannot store ``nil``,
certain patterns involving casts will need to change to work with 1.20.

class downcasts
^^^^^^^^^^^^^^^

In a class downcast, a class is casted to a subtype. If the dynamic type
of the variable does not match the requested subtype, the downcast fails.
In 1.19, a failed downcast would result in ``nil``. In 1.20, a failed
downcast will result in ``nil`` only if the target type is nilable and
will throw an error otherwise.

For example:

.. code-block:: chapel

  class Parent { }
  class Child : Parent { }

  var p:borrowed Parent = (new owned Parent()).borrow();
  var c:borrowed Parent = (new owned Child()).borrow();

  writeln(c:Child?); // downcast succeeds
  writeln(c:Child);  // downcast succeeds

  writeln(p:Child?); // this downcast fails and results in `nil`
  writeln(p:Child); // this downcast fails and will throw a ClassCastError

casting C pointers to classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Casts from ``c_void_ptr`` to class types were previously allowed. However,
since ``c_void_ptr`` can store ``NULL``, this case needs adjustment
following the nilability changes. Additionally, since ``c_void_ptr``
refers to a C pointer, and C pointers are manually managed (i.e. you call
``free`` on them at the appropriate time), it makes the most sense
for casts from ``c_void_ptr`` to end up with an unmanaged type.

Consider the following example:

.. code-block:: chapel

  class C {
    var x:int;
  }

  var myC = new owned C();
  var ptr:c_void_ptr = myC.borrow(); // store the instance in a C ptr

Now we can cast from ``ptr`` to the class type:

.. code-block:: chapel

  var c = ptr:C; // cast from a C pointer to the borrowed type

This example would work in 1.19. In 1.20, it needs to be updated to
cast to ``unmanaged C?``:

.. code-block:: chapel

  var c = ptr:unmanaged C?;

As with other values of type ``unmanaged C?``, from there it can:

 * be borrowed, e.g. ``c.borrow()``
 * have ``!`` applied to convert to a non-nilable value or halt, e.g. ``c!``
 * be cast to a non-nilable type, throwing if it is ``nil``, e.g.
   ``c:borrowed C``

.. _readme-evolution.undecorated-classes-generic-management:

undecorated classes have generic management
********************************************

Undecorated classes now have generic management. As an aid in migrating
code to this change, the flag ``--legacy-classes`` will disable this
new behavior.

Supposing that we have a ``class C`` declaration as in the following:

.. code-block:: chapel

  class C {
    var x:int;
  }

Code using ``C`` might refer to the type ``C`` on its own or it might use
a decorator to specify memory management strategy, as in ``borrowed C``.

The type expression ``C`` was the same as ``borrowed C`` in 1.18 and
1.19 but now means generic management. For example, in the following code:

.. code-block:: chapel

  var myC:C = new owned C();

``myC`` previously had type ``borrowed C``, and was initialized using
including an implicit conversion from ``owned C`` to ``borrowed C``. In 1.20,
``myC`` has type ``owned C``. Since the variable's type expression is
generic management, it takes its management from the initializing
expression.

This change combines with the nilability changes described above
to prevent compilation of existing code like the following:

.. code-block:: chapel

  var x:C;

Knowing that ``C`` now cannot store ``nil``, one might try to update this
program to:

.. code-block:: chapel

  var x:C?;

However this does not work either. ``C?`` indicates a nilable class type
with generic management, and a variable with generic type cannot be
default-initialized.

To update such a variable declaration to 1.20, it is necessary to include
a memory management decorator as well as ``?``. For example:

.. code-block:: chapel

  var x:borrowed C?;

The resulting variable will initially store ``nil``.

.. _readme-evolution.new-default-intent-for-owned-and-shared:

new default intent for owned and shared
***************************************

The default intent for `owned` and `shared` arguments is now
`const ref` where it was previously `in`. Cases where such arguments
will be interpreted differently can be reported with the ``--warn-unstable``
compilation flag.

Consider the following example:

.. code-block:: chapel

  class C {
    var x:int;
  }

  var global: owned C?;
  proc f(arg: owned C) {
    global = arg;
  }

  f(new owned C(1));

This program used to compile and run, performing ownership transfer
once when passing the result of ``new`` to ``f`` and a second time
in the assignment statement ``global = arg``.

This program does not work in 1.20. The compiler will issue an error for
the statement ``global = arg`` because the ownership transfer requires
modifying ``arg`` but it is not modifiable because it was passed with
``const ref`` intent.

To continue working, this program needs to be updated to add the `in`
intent to ``f``, as in ``proc f(in arg: owned C)``.

Note that for totally generic arguments, the 1.18 and 1.19 compiler
would instantiate the argument with the borrow type when passed
``owned`` or ``shared`` classes. For example:

.. code-block:: chapel

  class C {
    var x:int;
  }

  proc f(arg) { }

  var myC = new owned C(1);

  f(myC);       // does this call transfer ownership out of myC?
  writeln(myC); // prints `nil` if ownership transfer occurred

This example functions the same in 1.18 and 1.20, but for different
reasons. In 1.18, ``f`` is instantiated as accepting an argument of type
``borrowed C``. In the call ``f(myC)``, the compiler applies a coercion
from ``owned C`` to ``borrowed C``, so ownership transfer does not occur.
In 1.20, ``f`` is instantiated as accepting an argument of type ``owned C``
but this type uses the default intent (``const ref``). As a result,
ownership transfer does not occur.

.. _readme-evolution.new-C-is-owned:

new C is owned
**************

Supposing that `C` is a class type, `new C()` was equivalent to
`new borrowed C()` before this release - meaning that it resulted in
something of type `borrowed C`. However, it is now equivalent to `new
owned C()` which produces something of type `owned C`.


version 1.18, September 2018
----------------------------

Version 1.18 includes many language changes that address problems with
classes.

In summary:

 * constructors are deprecated and replaced with initializers
   See :ref:`readme-evolution.initializers-replace-constructors`
 * memory management for class types has changed
   See :ref:`readme-evolution.class-memory-management`
 * `override` is now required on overriding methods
   See :ref:`readme-evolution.mark-overriding`

.. _readme-evolution.initializers-replace-constructors:

initializers replace constructors
*********************************

Code that contained user-defined constructors will need to be updated
to use an initializer. For example:

.. code-block:: chapel

  record Point {
    var x, y: real;
    proc Point() {
      x = 0;
      y = 0;
      writeln("In Point()");
    }
    proc Point(x: real, y: real) {
      this.x = x;
      this.y = y;
      writeln("In Point(x,y)");
    }
  }
  var a:Point;
  var b = new Point(1.0, 2.0);

will now compile with deprecation warnings. Here is the same program
updated to use initializers:

.. code-block:: chapel

  record Point {
    var x, y: real;
    proc init() {
      x = 0;
      y = 0;
      writeln("In Point.init()");
    }
    proc init(x: real, y: real) {
      this.x = x;
      this.y = y;
      writeln("In Point.init(x,y)");
    }
  }
  var a:Point;
  var b = new Point(1.0, 2.0);

The change to initializers is much more than a change in the name of the
method. See the language specification for further details.

.. _readme-evolution.class-memory-management:

class memory management
***********************

Before 1.18, if ``C`` is a class type, a variable of type ``C`` needed
to be deleted in order to prevent a memory leak. For example:

.. code-block:: chapel

  class C {
    var x: int;
  }
  proc main() {
    var instance: C = new C(1);
    delete instance;
  }

Version 1.18 introduced four memory management strategies that form part
of a class type and are used with `new` expressions:

``owned C``
  ``owned`` classes will be deleted automatically when the ``owned``
  variable goes out of scope, but only one ``owned`` variable can refer to
  the instance at a time.
  Such instances can be created with ``new owned C()``.

``shared C``
  ``shared`` classes will be deleted when all of the ``shared`` variables
  referring to the instance go out of scope.
  Such instances can be created with ``new shared C()``.

``borrowed C``
  refers to a class instance that has a lifetime managed by
  another variable.
  Values of type ``borrowed C`` can be created with ``new borrowed
  C()``, by coercion from the other class ``C`` types, or by explicitly
  calling the ``.borrow()`` method on one of the other class ``C``
  types.
  ``new borrowed C()`` creates a temporary instance that will automatically
  be deleted at the end of the current block.

``unmanaged C``
  the instance must have `delete` called on it explicitly to
  reclaim its memory.
  Such instances can be created with ``new unmanaged C()``.

Further note that the default is ``borrowed``, that is:

``C``
  is now the same as ``borrowed C``

``new C()``
  is now the same as ``new borrowed C()``

Now, back to the example above. There are several ways to translate this
program.

First, the most semantically similar option is to replace uses of ``C``
with ``unmanaged C``:

.. code-block:: chapel

  class C {
    var x: int;
  }
  proc main() {
    var instance: unmanaged C = new unmanaged C(1);
    delete instance;
  }

Using ``unmanaged`` allows a Chapel programmer to opt in to manually
managing the memory of the instances.

A reasonable alternative would be to translate the program to use
``owned C``:

.. code-block:: chapel

  class C {
    var x: int;
  }
  proc main() {
    var instance: owned C = new owned C(1);
    // instance will now be automatically deleted at the end of this block
  }

If the program does not explicitly use ``owned C``, it can rely on
``new C()`` being equivalent to ``new borrowed C()``:

.. code-block:: chapel

  class C {
    var x: int;
  }
  proc main() {
    var instance: C = new C(1);

    // instance will now be automatically deleted at the end of this block
  }

See the *Class New* section in the *Classes* chapter of the language
specification for more details.

.. _readme-evolution.mark-overriding:

overriding methods must be marked
*********************************

Before 1.18, a class inheriting from another class can create an
overriding method that is a candidate for virtual dispatch:

.. code-block:: chapel

  class Person {
    var name: string;
    proc greet() {
      writeln("Hello ", name, "!");
    }
  }
  class Student: Person {
    var grade: int;
    proc greet() {
      writeln("Hello ", name, ", welcome to grade ", grade);
    }
  }
  proc main() {
    var person: Person = new Student("Jeannie", 5);
    person.greet(); // uses the run-time type of person (Student)
                    // and virtually dispatches to Student.greet()
  }

Now such overriding methods must be marked with the `override` keyword:

.. code-block:: chapel

  class Person {
    var name: string;
    proc greet() {
      writeln("Hello ", name, "!");
    }
  }
  class Student: Person {
    var grade: int;
    override proc greet() {
      writeln("Hello ", name, ", welcome to grade ", grade);
    }
  }
  proc main() {
    var person: Person = new Student("Jeannie", 5);
    person.greet(); // uses the run-time type of person (Student)
                    // and virtually dispatches to Student.greet()
  }


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

See `GitHub issue #5217 <https://github.com/chapel-lang/chapel/issues/5217>`_
for more examples and discussion.

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

See `GitHub issue #5266 <https://github.com/chapel-lang/chapel/issues/5266>`_
for more details and discussion.

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
`Quick Reference sheet <https://chapel-lang.org/spec/quickReference.pdf>`_.

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
