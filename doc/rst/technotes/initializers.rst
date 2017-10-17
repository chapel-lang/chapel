.. _readme-initializers:

============
Initializers
============

The Chapel team is implementing a new approach to user-defined
initialization of variables with record type or instances of class
type.  This approach relies on methods known as initializers rather
than the original methods known as constructors.

A discussion of the current design and rationale is provided in `CHIP 10`_.

.. _CHIP 10:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst

Release Chapel 1.16.0 provides a strong preview implementation of this
new feature.  Though some known bugs remain, the feature is rapidly
approaching full support and developers should feel encouraged to
write initializers where previously they had relied on constructors.

It is anticipated that the implementation will continue to advance
steadily after this release and that many aspects of the internal
implementation of Chapel will transition from constructor methods to
initializer methods during this release.


The Initializer Method
----------------------

An initializer is a method on a class or record named "init".  It is invoked
by the ``new`` operator, where the type name and initializer arguments are
preceded with the ``new`` keyword.

If the program declares an initializer method on a type, it is a user-defined
initializer.  If the program declares no initializers or constructors for a
class or record, a compiler generated constructor for that type is created
automatically.  Work has begun on having the compiler generate an initializer
instead a constructor, but it is not yet complete (see below for the status on
`Compiler Generated Initializers`_).

User-Defined Initializers
-------------------------

A user-defined initializer is an initializer method explicitly declared in the
program.  An initializer declaration has the same syntax as a method
declaration, except that the name of the function is "init", and there is no
return type specifier.

When an initializer is invoked, the usual function resolution mechanism is
applied to determine which user-defined initializer is required.

The following example shows a class with two initializers:

.. code-block:: chapel

   class MessagePoint {
     var x, y: real;
     var message: string;

     proc init(x: real, y: real) {
       this.x = x;
       this.y = y;
       this.message = "a point";
       super.init();
     }

     proc init(message: string) {
       this.x = 0;
       this.y = 0;
       this.message = message;
       super.init();
     }
   } // class MessagePoint

   // create two objects
   var mp1 = new MessagePoint(1.0,2.0);
   var mp2 = new MessagePoint("point mp2");

The first initializer lets the user specify the initial coordinates and the
second initializer lets the user specify the initial message when creating a
MessagePoint.

Order of Initialization
-----------------------

The specification of a class may be derived from, or inherit from, the
specification of one or more other classes.  The initialization of an
instance of a derived class requires that the initializer for each parent
class be executed in some well defined order.

Chapel initializes an instance in two phases that we refer to as "Phase 1"
and "Phase 2".

Phase 1 proceeds from the most derived class to the base class, and the fields
for each class are initialized in field declaration order. This implies that
the fields of any parent classes will be in an undefined state during phase 1.
This in turn requires that certain constraints be observed during phase 1.
These are described in more detail below.

Phase 2 proceeds from the base class to the most derived class once phase 1
has been completed for the base class.  At this point every field of the
instance is in a well defined state and so there are no restrictions on
the operations that may be performed.

Note that this protocol is well-defined for instances of classes that do
not include inheritance.  The fields of the instance are initialized in
field declaration order during Phase 1 and then Phase 2 may be used to
perform additional initialization.

Records in Chapel do not currently provide support for inheritance but we
choose to view the initialization of record values in the same manner.

The Initializer Body
--------------------

The code written in an initializer is divided into two sequentially-ordered
categories that define the operations to be performed in phase 1 and then
phase 2.  The two phases are separated by a phase division indicator.  When
the phase division indicator is not present, the body of an initializer is
assumed to be entirely composed of Phase 2 statements.  Otherwise, any code
prior to the phase division indicator is considered to be in Phase 1, and any
code following it is considered to be in Phase 2.  Phase 1 and Phase 2 will
be described in the next few subsections, and additional details and rationale
can be found in `CHIP 10`_.

Note that aside from ``try!`` statements without a ``catch`` block, error
handling constructs are not allowed in initializers.  An initializer cannot
be declared as ``throws``.  See `Interaction With Error Handling`_.

Phase 1
+++++++

The code residing in Phase 1 must follow a set of strong requirements.

Other methods on the ``this`` instance cannot be called.  The ``this``
instance may not be passed to another function.

Fields must be initialized in declaration order; however, fields can be
omitted. Omitted fields are given the declared initial value if present,
or the default of its declared type.  Fields with neither a declared initial
value nor a declared type cannot be omitted.

.. code-block:: chapel

   class Foo {
     var bar = 10;
     var baz = false;
     var dip: real;

     proc init(barVal, dipVal) {
       bar = barVal;
       // omitted initialization: baz = false;
       dip = dipVal;
       super.init();
     }
   }

   var foo = new Foo(11, 2.0);

Both explicit and implicit initialization of a field can depend on the values
of prior fields.  However, later fields may not be referenced.


.. code-block:: chapel

   class Foo2 {
     var bar = 10;
     var baz = 5;
     var dip = baz * 3;

     proc init(barVal) {
       bar = barVal;
       baz = divceil(bar, 2);
       // omitted initialization: dip = baz * 3;
       super.init();
     }
   }

   var foo2 = new Foo2(11);

Parent fields may not be accessed or initialized during Phase 1.

``const`` fields may be initialized during Phase 1.  Local variables may be
created and used.  Functions that are not methods on the ``this`` instance
may be called, so long as ``this`` is not provided as an argument.

Loops and parallel statements are allowed during Phase 1, but field
initialization within them is forbidden.  ``on`` statements whose bodies
extend into Phase 2 are not allowed, but more limited ``on`` statements are
acceptable.

When Phase 1 of the initializer body has completed and the phase division
indicator has been processed, it can safely be assumed that all fields are
in a usable state.

Phase Division Indicator
++++++++++++++++++++++++

An explicit call to another initializer ends Phase 1 and begins Phase 2.
This call takes one of two forms:

Form 1: call to an initializer defined on the parent type

.. code-block:: chapel

   super.init();

Form 2: call to another initializer defined on the same type

.. code-block:: chapel

   this.init();

If the type has no parent, an argument-less call of the first form will still
be valid, but otherwise treated as a no-op.

Example of initializers using the first form:

.. code-block:: chapel

   class Foo { // no parent type
     var x: int;

     proc init(xVal: int) {
       x = xVal;
       super.init(); // argument-less call ends Phase 1
     }
   }

   class Bar: Foo { // inherits from Foo
     var y: bool;

     proc init(yVal: bool) {
       y = yVal;
       super.init(10); // Calls the parent initializer
     }
   }

   var bar = new Bar(true);

When using the second form, field initialization statements are not permitted
in Phase 1, though other statements are allowed.  Omitted field initialization
will not be inserted prior to calls of the second form.

Example of an initializer using the second form:

.. code-block:: chapel

   class Rectangle {
     var len, width: int;

     proc init(val: int) {
       this.init(val, val); // calls the other initializer
       writeln("Making a square");
     }

     proc init(lenVal: int, widthVal: int) {
       len = lenVal;
       width = widthVal;
       super.init();
     }
   }

   var square = new Rectangle(4);

For a single control flow path through the body, only one phase division
indicator is allowed.  It is forbidden to have both calls, or multiple of
either, in a single control flow path.  It is forbidden to enclose the phase
division indicator in a parallel statement, on statement, or a loop statement.
If the phase division indicator is enclosed by a conditional, it must be a
``param`` conditional.

If no phase division indicator is provided, an argument-less first form call
will be inserted at the beginning of the body.  The
`Compiler Generated Initializers`_ will also include an argument-less first
form call after completing the initialization of its fields.  If the parent
type has defined an initializer that this call cannot resolve to, attempts
to initialize the child with the compiler generated initializer will result
in an error.


Phase 2
+++++++

Code in Phase 2 is functionally similar to other methods on the type, and less
restrictive than code in Phase 1.  Modifications to the fields are considered
assignment rather than initialization.  Other methods may be called on the
``this`` instance, and the ``this`` instance may be passed as an argument to
another function.  Parent fields may be accessed.

As in other methods, code in Phase 2 may not redefine ``const``, ``param``,
and ``type`` fields.


Generics
--------

A class or record with a ``param`` field, ``type`` field, or a ``var`` /
``const`` field with no type or initial value is considered generic over that
field.  Generic fields are treated similarly to other fields, with some
exceptions.  Only generic fields are capable of being declared without a type
or initial value, so only those generic fields without either must have an
explicit initialization in Phase 1 - other generic fields may rely on omitted
initialization like other fields do.  Like ``const`` fields, ``type`` and
``param`` fields may not be updated during Phase 2.

Note: user-defined constructors for generic classes and records required an
argument per generic field and did not allow generic fields to be set during
the constructor body.  Initializers do not have this constraint.


Copy Initializers
-----------------

An initializer may be defined to control the behavior when a copy of an
instance is made.  This initializer is define with a single argument on
the same type as the type being created:

.. code-block:: chapel

   class Foo {
     var x: int;
     var wasCopied = false;

     proc init(xVal: int) {
       x = xVal;
       super.init();
     }

     // copy initializer
     proc init(other: Foo) {
       x = other.x;
       wasCopied = true;
       super.init();
     }
   }

   var foo1 = new Foo(5);
   var foo2 = new Foo(foo1); // user inserted copy
   writeln(foo1);
   writeln(foo2);
   delete foo1;
   delete foo2;

For more details on when the copy initializer would be called, please refer to
`CHIP 13 - When Do Records and Array Copies Occur`_

.. _CHIP 13 - When Do Records and Array Copies Occur:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/13.rst

Remaining Work
--------------

With the 1.16.0 release, support for initializers is mostly stable with a few
bugs and some unimplemented features remaining.  It is recommended for
developers writing new classes and records to write initializers when possible.
Please report any bugs encountered using the guidance described at the `bugs`_
page.

.. _bugs:
   https://chapel-lang.org/docs/latest/usingchapel/bugs.html

Compiler Generated Initializers
+++++++++++++++++++++++++++++++

Prototypical support of compiler generated initializers has been added.  With
the 1.16.0 release and the developer-oriented flag ``--force-initializers``,
user-defined classes will attempt to generate default initializers instead of
default constructors.  User-defined records, and records and classes defined in
the internal, standard, or package modules will not yet generate default
initializers with this flag.  However, there are still failures with even that
limited application.

It is anticipated that compiler generated initializers will be fully supported
in the next release.

Interaction With Error Handling
+++++++++++++++++++++++++++++++

Due to time constraints, the 1.16.0 release went out with very limited support
for error handling constructs: an initializer cannot be declared as ``throws``,
and only ``try!`` statements without ``catch`` blocks are allowed in the body.

In later releases, we hope to support ``throw``, and ``try`` and ``try!``
statements with ``catch`` blocks during Phase 2, allowing initializers to be
declared as ``throws``.  It may be possible to allow these constructs in Phase
1, though for simplicity's sake they will likely still be banned around field
initialization statements and forbidden from crossing the Phase 1/Phase 2
divide.

In the world where initializers can ``throw``, we will only allow child classes
to ``throw`` if the parent initializer ``throws`` (though there may be
complications with chains of initializers, such as an initializer that calls
another initializer on the type, which calls a parent initializer that
``throws``, etc.).


Noinit
++++++

Variable initialization when provided the ``noinit`` keyword in place of an
initial value for a class or record should generate a call to an initializer
that has defined what ``noinit`` means for that type.  More details on the
direction for this support can be found in the `noinit section`_ of CHIP 10.

.. _noinit section:
   https://github.com/chapel-lang/chapel/blob/master/doc/rst/developer/chips/10.rst#noinit

Bugs
++++

- secondary initializers in outside modules when the type doesn't define an
  initializer in its original module
- nested types when the outer type and/or the inner type defines an initializer
  and the outer type and/or the inner type is generic.
- others

Other TODOs
+++++++++++

- Convert library types to utilize initializers instead of constructors
- Improve some slightly cryptic error messages
- Ensure we *always* error when a method is called in Phase 1 (we only
  sometimes do today)
- Extend on statement support to allow field initialization within its bounds
  after getting larger team buy in.
