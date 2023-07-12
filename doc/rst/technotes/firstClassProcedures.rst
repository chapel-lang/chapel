.. _readme-firstClassProcedures:

================================
First-class Procedures in Chapel
================================

.. warning::

  The ``lambda`` keyword (used to construct an anonymous procedure) has been
  deprecated as of the 1.31 release. The ``func()`` procedure type constructor
  has also been deprecated. Users should make use of the ``proc()`` syntax
  described in this document instead.

.. note::

  First-class procedures are under active development and are provided
  in their prototype form as a preview and to gather user feedback.

The 1.31 release of Chapel introduced new ``proc()`` syntax for constructing
anonymous procedures and procedure types. The next few sections provide 
details about this syntax.

The section :ref:`Capturing_First_Class_Procedures` describes when a
procedure  may be used as a value. The restrictions in this section may
weaken as development of first-class procedures continues.

New Syntax for Constructing Procedure Types
-------------------------------------------

Procedure types may now be constructed using new syntax which mirrors
the syntax for procedure definition.

To construct the type of a procedure which takes two integers and
returns an integer, users may write the following:

.. code-block:: chapel

  type T = proc(x: int, y: int): int;
  writeln(T:string); // 'proc(x: int, y: int): int'


.. note::

  The implementation currently expects a token with the text ``proc(`` when
  defining an anonymous procedure or procedure type. Users have expressed
  confusion about how writing ``proc (`` (the ``proc`` token, followed by
  one or more spaces, followed by a ``(`` token) results in a syntax error.
  This restriction is a parsing limitation and may be removed in the future.

Notice that the formals of the procedure type have specified names,
``x`` and ``y``. Currently, formal names participate in typing such
that a procedure defined with formals named ``x`` and ``y`` cannot be
assigned to a procedure defined with different formal names. In the
below example, assignment of two procedure values will fail because
the formal names are different:

.. code-block:: chapel

  proc foo(x: int, y: int): int { return x + y; }
  proc bar(a: int, b: int): int { return a + b; }
  writeln(foo.type:string);   // 'proc(x: int, y: int): int'
  var x = foo;                // ^ (same type) ^
  writeln(bar.type:string);   // 'proc(a: int, b: int): int'
  x = bar;                    // Error!

There are scenarios where a user may want to pass around procedures 
without regard for their formal names. Anonymous ``_`` formals are a
feature under active development that are intended to support this
behavior.

New Syntax for Constructing Anonymous Procedures 
------------------------------------------------

A new syntax for constructing anonymous procedures has been introduced
which more closely mirrors traditional procedure definition.

.. code-block:: chapel

  // Define a procedure named 'foo'.
  proc foo(x: int, y: int): int { return x + y; }

  // Define an anonymous procedure bound to the constant variable 'bar'.
  const bar = proc(a: int, b: int): int { return a + b; };

The Formals of Procedure Types May Be Anonymous
-----------------------------------------------

.. note::

  This section describes unstable functionality that may not be implemented.

A formal in a procedure type may be anonymous. This may be done by
naming the formal ``_``, similar to what is written to discard
tuple elements when de-tupling.

An anonymous formal in a procedure type indicates that values of this
type may have formals with any name. The formal name no longer plays
a significant role in typing.

.. code-block:: chapel

  proc foo(x: int, y: int): int { return x + y; }
  proc bar(a: int, b: int): int { return a + b; }

  // Here the formals of 'T' are anonymous.
  type T = proc(_: int, _: int): int;

  var x: T = foo;             // OK, T's formals are anonymous '_'.
  writeln(foo.type:string);   // 'proc(x: int, y: int): int'
  writeln(x.type:string);     // 'proc(_: int, _: int): int'
  writeln(bar.type:string);   // 'proc(a: int, b: int): int'
  x = bar;                    // OK!

In the above example, two procedures are declared with different formal
names, but otherwise identical types. A local variable `x` is declared
with a procedure type that has anonymous formals. This enables the
procedures `foo` and `bar` to be assigned freely to `x`.

.. note::

  Currently, it is not possible for procedure definitions to declare
  anonymous formals. It has been indicated that such a feature might
  be useful, so it may be added in the future.

.. note::

  It is undecided as to whether a procedure may mix anonymous formal
  names with named formals, or if all formals must be either named
  or anonymous.

.. _Capturing_First_Class_Procedures:

Capturing First-Class Procedures
--------------------------------

Procedures may be captured as values by referring to them by name.

For example:

.. code-block:: chapel

  proc myfunc(x:int) { return x + 1; }
  var f = myfunc;
  writeln(f(3));  // outputs: 4

Anonymous procedures may be captured as though they are named procedures,
by substituting them in places where a procedure name may also appear:

.. code-block:: chapel

  const p = proc(x: int) { return x + 1; };
  writeln(p(3));  // outputs: 4

Only procedures (defined with the ``proc`` keyword) may be captured.
Additionally, a captured procedure must not:

- Refer to any outer variable that is not at module scope
- Have a `type` or `param` return type
- Accept `type` or `param` formals
- Be a method
- Be overloaded
- Be generic
- Be parenless

.. note::

  These restrictions may be weakened or removed as development of
  first-class procedures continues. Explanation for them follows.

Capturing iterators, operators, and methods falls beyond the scope of
the current design.

Capturing a procedure that refers to outer scope variables necessitates the
creation of a closure. Closure support will be added at a later date,
at which point this restriction will be removed.

Procedures with `type` or `param` return types cannot be captured because
such procedures must be evaluatable at compile-time. The notion of a `param`
first-class procedure has been considered as a way to circumvent this
restriction, but has not been fully explored at this time.

Overloaded procedures cannot be captured without further disambiguation.
This is left as future work.

Future Directions
-----------------

Chapel's first-class procedure story is under active development.
If you have specific feature requests or suggestions, please let us
know on the `Chapel GitHub issues page`_ or community forums.

.. _Chapel GitHub issues page: https://github.com/chapel-lang/chapel/issues

