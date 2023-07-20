.. _readme-firstClassProcedures:

================================
First-class Procedures in Chapel
================================

.. warning::

  The ``lambda`` keyword (used to construct an anonymous procedure) has been
  deprecated as of the 1.31 release. The ``func()`` procedure type constructor
  has also been deprecated. Instead, users should make use of the ``proc()``
  syntax described in this document.

.. note::

  First-class procedures are under active development and are provided
  in their prototype form as a preview and to gather user feedback.

The 1.31 release of Chapel introduced ``proc()`` syntax for constructing
anonymous procedures and procedure types. The next few sections provide 
details about this syntax.

The section :ref:`Capturing_First_Class_Procedures` describes when a
procedure may be used as a value. The restrictions in this section may
weaken as development of first-class procedures continues.

Procedure Types
---------------

Procedure types may be constructed using syntax which mirrors the
syntax for named procedure definition.

To construct the type of a procedure which takes two integers and
returns an integer, users may write the following:

.. code-block:: chapel

  type T = proc(x: int, y: int): int;
  writeln(T:string); // 'proc(x: int, y: int): int'


.. note::
  Currently ``proc(`` must be written without any spaces occurring between
  ``proc`` and the open parenthesis ``(``. Otherwise the compiler will
  report a syntax error. This is an implementation restriction and may be
  removed in the future.

Notice that the formals of the procedure type have specified names,
``x`` and ``y``. Currently, the intention is for formal names to participate
in typing such that two procedures that vary only by their formal names
have different types. In the below example, assignment of two procedure
values will fail because the formal names are different:

.. code-block:: chapel

  proc foo(x: int, y: int): int { return x + y; }
  proc bar(a: int, b: int): int { return a + b; }
  writeln(foo.type:string);   // 'proc(x: int, y: int): int'
  var x = foo;                // 'x' is typed 'proc(x: int, y: int): int'
  writeln(bar.type:string);   // 'proc(a: int, b: int): int'
  x = bar;                    // Error!

There are scenarios where a user may want to pass around procedures
without regard for their formal names. Anonymous ``_`` formals are a
feature under active development that is intended to support this
behavior.

Anonymous Procedures
--------------------

The syntax for constructing anonymous procedures also mirrors named
procedure definition.

.. code-block:: chapel

  // The procedure named 'foo' is not an anonymous procedure.
  proc foo(x: int, y: int): int { return x + y; }

  // Define an anonymous procedure bound to the constant variable 'bar'.
  const bar = proc(a: int, b: int): int { return a + b; };

The Formals of Procedure Types May Be Anonymous
-----------------------------------------------

.. note::

  Currently, anonymous formals are not implemented for either procedure
  types or procedure definitions. These features may be added in the
  future.

A formal in a procedure type may be anonymous. This can be done by
naming the formal ``_``, similar to what is written to discard
tuple elements when de-tupling.

If a procedure type `T` declares a formal at position `N` to be anonymous,
then a value of type `T` may use any name for its formal at position `N`.

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

  It is undecided as to whether a procedure type may mix anonymous formal
  names with named formals, or if all formals must be either named
  or anonymous.

Procedure definitions may declare anonymous formals as well. When a formal
is declared anonymous, it cannot be referenced within the body of the
procedure.

.. code-block:: chapel

  // The second formal of 'baz' is anonymous and cannot be used.
  proc baz(x: int, _: int): int { return x + 1; }

.. _Capturing_First_Class_Procedures:

Capturing First-Class Procedures
--------------------------------

Procedures may be captured as values by referring to them by name.

For example:

.. code-block:: chapel

  proc myfunc(x:int) { return x + 1; }
  const p = myfunc;
  writeln(p(3));  // outputs: 4

Anonymous procedures may be captured as though they are named procedures,
by substituting them in places where a procedure name may also appear:

.. code-block:: chapel

  const p = proc(x: int) { return x + 1; };
  writeln(p(3));  // outputs: 4

Today, only procedures (defined with the ``proc`` keyword) may be
captured. Additionally, a captured procedure must not:

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

Procedures with `type` or `param` formals or return types cannot be captured
because such procedures must be evaluatable at compile-time. The notion of a
`param` first-class procedure has been considered as a way to circumvent
this restriction, but has not been fully explored at this time.

Overloaded procedures cannot be captured without further disambiguation.
This is left as future work.

Future Directions
-----------------

Chapel's first-class procedure story is under active development.
If you have specific feature requests or suggestions, please let us
know on the `Chapel GitHub issues page`_ or community forums.

.. _Chapel GitHub issues page: https://github.com/chapel-lang/chapel/issues

