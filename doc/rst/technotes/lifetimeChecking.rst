.. _readme-lifetime-checking:

=================
Lifetime Checking
=================

As of Chapel 1.18, Chapel includes a compiler component called the
lifetime checker. The lifetime checker produces errors at compile time to
reveal potential memory errors. Note that the Chapel lifetime checker is
not complete - that is, there are programs with memory errors that it
will not detect. However, we hope that it offers a good balance between
being easy to work with and catching common memory errors at
compile-time. See also :ref:`readme-nil-checking` which discusses a
related component that discovers nil dereferences at compile time.

Defining Scope and Lifetime
===========================

Scope
+++++

Variables in Chapel have a lexical scope within which it is legal to
access the variable. For example:

.. code-block:: chapel

  module DemonstrateScopes {
    proc function() {
      var f: int;
      // scope of `f` includes the body of this function
      {
        // and any nested blocks (including loops, conditionals, etc).

        var x: int;
        // `x` scope ends here, but `f` scope does not
      }
      // it would be an error to access `x` here
    }
    // it would be an error to access `f` here

    {
      var b: int;
      // `b`s scope extends to the end of this block
    }
    // it would be an error to try to use `b` here

    var g: int;
    // `g` is a global variable and its scope extends
    // to any code using this module.
  }

A scope can be contained in another scope. For example, the scope of ``x``
is contained within the scope of ``f`` in the above example. In other
words, anywhere that ``x`` can be accessed, ``f`` can also be accessed.
In such a case we say that the scope of ``x`` is smaller than the scope of
``f``.

Lifetime
++++++++

The lifetime of a variable indicates when that variable can be safely
used.

 * Variables that cannot refer to another value, such as numeric
   variables, have a lifetime equal to their lexical scope.
 * Variables that can refer to other variables include ``borrowed`` class
   instances and ``ref`` variables. These variables get their lifetime
   from the lifetime of the variable that they refer to.
 * ``owned`` and ``shared`` variables have lifetime equal to their scope.

Note that ``owned`` and ``shared`` variables can be returned or assigned
without impacting their lifetime. The lifetime checker just checks that a
``borrow`` from such a variable does not outlive the variable itself.

.. code-block:: chapel

  module DemonstrateLifetimes {
    proc function() {
      var i: int;
      // `i`s lifetime extends to the end of this block
      ref r = i;
      // `r` refers to `i`, so it's lifetime == `i`s lifetime
      var own = new owned SomeClass();
      // `own`s lifetime extends to the end of this block,
      // (at which point the class instance may be deleted)
      var borrow = own.borrow();
      // `borrow`s lifetime extends to the end of this block
      // because its lifetime matches `own`
    }

    var global: owned SomeClass;
    proc settingGlobal() {
      var x = new owned SomeClass();
      // lifetime of x extends for entire function body

      global = x; // transfers the instance from x to global
      // leaving x storing `nil`

      // lifetime of `x` extends to here, but an attempt
      // to use `x` would result in an error from
      // compile-time nil checking.
    }
  }

Similarly to scopes, lifetimes may be contained within each other.
Ultimately, a lifetime is just the scope of some variable, and so we can
say that one lifetime is smaller or larger than another, just as we can
say that a scope is smaller or larger than another scope.

Example Errors
==============

The lifetime checker is designed to catch errors such as:

 * returning a reference to or borrow from a function-local variable
 * assigning a value with a shorter lifetime to something with a larger scope

When the lifetime for a variable is smaller than its scope, that usually
means that there is some point in the program where accessing that
variable could lead to a memory error. There are some cases where the
analysis indicates a memory error could occur, but a human programmer
might know that it cannot for other reasons.

Returning a Reference to a Local Variable
+++++++++++++++++++++++++++++++++++++++++

.. code-block:: chapel

  // returnsref.chpl
  proc refTo(ref x) ref {
    return x;
  }

  proc returnsRefLocal() ref // note `ref` return intent
  {
    var i: int;
    return refTo(i); // returns `i` by reference
                     // but `i` goes out of scope here
  }
  ref r = returnsRefLocal();
  var val = r; // accesses invalid memory

::

  returnsref.chpl:6: In function 'returnsRefLocal':
  returnsref.chpl:9: error: Reference to scoped variable cannot be returned
  returnsref.chpl:8: note: consider scope of i


Returning a Borrow From a Local Owned Instance
++++++++++++++++++++++++++++++++++++++++++++++

.. code-block:: chapel

  // returnsborrow.chpl
  class SomeClass { var field: int; }
  proc borrowLocal() {
    var obj = new owned SomeClass;
    return obj.borrow(); // returns borrow of `obj`
    // but `obj` goes out of scope (and `delete`s the instance) here
  }

  var b = borrowLocal();
  var y = b.field; // accesses deleted memory

::

  returnsborrow.chpl:3: In function 'borrowLocal':
  returnsborrow.chpl:5: error: Scoped variable cannot be returned
  returnsborrow.chpl:4: note: consider scope of obj

Assigning a Borrow to something with Longer Scope
+++++++++++++++++++++++++++++++++++++++++++++++++

.. code-block:: chapel

  // assignsborrow.chpl
  class SomeClass { }

  {
    var bor: borrowed SomeClass;
    {
      var obj = new owned SomeClass();
      bor = obj.borrow(); // borrow of `obj` escapes
      // but `obj` goes out of scope (and `delete`s the instance) here
    }
    writeln(bor); // uses freed memory
  }

::

  assignsborrow.chpl:8: error: Scoped variable bor would outlive the value it is set to
  assignsborrow.chpl:7: note: consider scope of obj


Lifetime Inference
==================

The lifetime checker starts by inferring the lifetime of each variable.
It considers the ways that the variable is set:

 * if the variable is a reference to another variable, then
   its lifetime will be the scope of that variable
 * if a borrow is assigned or initialized from another variable, then
   its lifetime will be at most the lifetime of the other variable
 * if the variable is set by a function call, then the lifetime is inferred
   according to rules described below

Inference proceeds until the minimum inferred lifetime of each variable is
established.

Inferred Lifetimes of Arguments
+++++++++++++++++++++++++++++++

For methods, the ``this`` argument is assumed to have longer lifetime than the
actual arguments and only the ``this`` argument is assumed to have a lifetime
that can be returned.

For non-methods, all formals are considered to have a lifetime that can be
returned.

Inferred Lifetime of Function Call Results
++++++++++++++++++++++++++++++++++++++++++

For ``x = f(a, b, c)``, the lifetime of ``x`` is inferred to be the
minimum lifetime of the arguments ``a``, ``b``, ``c`` that have lifetimes
that could be returned.

For a method call, such as ``y = receiver.f(a, b, c)``, the lifetime will
be inferred to be the lifetime of ``receiver``.

If these inferred lifetimes are not appropriate for a function, the lifetimes
can be specified with a lifetime annotation.

Lifetime Annotations
====================

Certain functions need to override the default lifetime inference rules.
This can be accomplished by placing a ``lifetime`` clause after the
return type. These ``lifetime`` clauses share some similarities with
``where`` clauses. For example:

.. code-block:: chapel

  class C { var x: int; }
  var globalOwned = new owned C(1);
  var globalBorrow = globalOwned.borrow();

  // Default lifetime inference assumes that the
  // returned lifetime is the lifetime of arg,
  // but that's not appropriate here.
  //
  // The lifetime annotation indicates that the returned value
  // has the lifetime of globalBorrow.
  proc returnsGlobalBorrow(arg: borrowed C)
    lifetime return globalBorrow
  {
    return globalBorrow;
  }

Other functions need to assert a relationship between the lifetimes of
their arguments. This pattern comes up with functions that append some
data to a data structure.

.. code-block:: chapel

  record Collection {
    type elementType;
    var element: elementType;
  }

  // Without lifetime annotation, the compiler will raise an error,
  // because `this` is assumed to have larger lifetime than `arg`,
  // and so the assignment will set something with a longer lifetime
  // to something with a shorter lifetime.
  //
  // The lifetime clause `lifetime this < arg` avoids that error
  // by informing the compiler that `this` (and by extension, `this.element`)
  // need to have lifetime no longer than `arg`.
  proc Collection.addElement(arg: elementType)
    lifetime this < arg
  {
    this.element = arg;
  }

Note that the lifetime clause needs to be written in terms of formal
arguments, including ``this`` for methods, and possible outer variables.
In particular, in the above, the constraint is between ``this`` and
``arg`` rather than ``this.element`` and ``arg``. ``this.element`` will
have its lifetime inferred to be the lifetime of ``this``, so these are
equivalent.

In some cases, it is more natural to write the lifetime annotation in
terms of what assignments the function may make. For example:

.. code-block:: chapel

  proc myswap(ref lhs: borrowed MyClass, ref rhs: borrowed MyClass)
    lifetime lhs=rhs, rhs=lhs
  {
    var tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }

Here the lifetime checker ensures that the lifetimes of the actual
arguments are suitable for performing the assignments between formals
that are indicated in the lifetime clause ``lifetime lhs=rhs, rhs=lhs``.
