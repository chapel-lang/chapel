.. default-domain:: chpl

.. index::
   single: unions
.. _Chapter-Unions:

======
Unions
======

Unions have the semantics of records, however, only one field in the
union can contain data at any particular point in the program's
execution. Unions are safe so that an access to a field that does not
contain data is a runtime error. When a union is default-initialized,
it is in an unset state so that no field contains data.

.. warning::

   Unions are currently unstable and may change in ways that will break
   their current uses.

.. index::
   single: union
   single: declarations; union
.. _Union_Declarations:

Union Declarations
------------------

A union is defined with the following syntax:

.. code-block:: syntax

   union-declaration-statement:
     'extern'[OPT] 'union' identifier { union-statement-list }

   union-statement-list:
     union-statement
     union-statement union-statement-list

   union-statement:
     variable-declaration-statement
     procedure-declaration-statement
     iterator-declaration-statement
     empty-statement

The identifier following the ``union`` keyword defines the name of the
union, while the variable declaration statements define its fields and
their types.  Procedure and iterator declaration statements define
methods on the union.

If the ``extern`` keyword appears before the ``union`` keyword, then an
external union type is declared. An external union is used within Chapel
for type and field resolution, but no corresponding backend definition
is generated. It is presumed that the definition of an external union
type is supplied by a library or the execution environment.

.. index::
   single: types; unions
   single: union types
.. _Union_Types:

Union Types
~~~~~~~~~~~

A union type is referred to using the identifier representing its
name:

.. code-block:: syntax

   union-type:
     identifier

This is simpler than class and record types because generic unions are
not currently supported.

.. index::
   single: unions; fields
.. _Union_Fields:

Union Fields
~~~~~~~~~~~~

Union fields are accessed in the same way that record fields are
accessed. It is a runtime error to access a field that is not currently
set.

   *Example (fieldAccess.chpl)*.

   .. code-block:: chapel

      union U {
        var x: int;
        var y: real;
      }
      var u: U;
      u.x = 3; // sets field x
      writeln(u.x);
      writeln(u.y); // runtime error: field y is not set

   .. BLOCK-test-chapeloutput

      3
      fieldAccess.chpl:8: error: halt reached - illegal union access: attempted to access field 'y' but 'x' is currently active

   .. BLOCK-test-chapelcompopts

      --checks

The currently active field of a union can be queried at runtime with
the :proc:`~ChapelUnion.union.getActiveIndex` method. If the union is
not yet explicitly initialized, then
:proc:`~ChapelUnion.union.getActiveIndex` returns -1.

   *Example (getActiveIndex.chpl)*.

   .. BLOCK-test-chapelpre

      union U {
        var x: int;
        var y: real;
      }
      var u: U;

   .. code-block:: chapel

      writeln(u.getActiveIndex()); // prints -1
      u.y = 3.0; // sets field y
      writeln(u.getActiveIndex()); // prints 1

   .. BLOCK-test-chapeloutput

      -1
      1

Each union field also has an associated index, this can be queried by accessing
the field name as a member of the union type.

   *Example (fieldIndex.chpl)*.

    .. code-block:: chapel

      union U {
        var x: int;
        var y: real;
      }
      writeln(U.x); // prints 0
      writeln(U.y); // prints 1

   .. BLOCK-test-chapeloutput

      0
      1

Union fields should not be specified with initialization expressions.

.. index::
   single: unions; initialization
   single: initialization; union
.. _Union_Initialization:

Union Initialization
--------------------


Default Initialization
~~~~~~~~~~~~~~~~~~~~~~

As with records, when a union is declared with no explicit
initialization, ``init`` will be called with no arguments, invoking its
default initializer.  The compiler-generated default initializer for a
union is defined to set up the union such that none of its fields are
active.  A user may define their own zero-argument initializer to get
a different default initialization behavior for a union type.


Compiler-Generated Initializers
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If no user-defined primary or secondary initializers are declared, the
compiler creates the aforementioned zero-argument initializer as well
as an initializer per field that accepts an ``in`` argument with the
field's name and whose type matches the field's.  Thus, a union like
the following:

   .. code-block:: chapel

      union u {
        var x: real;
        var y: int;
        var z: int;
      }

would result in the following compiler-generated initializers:

   .. code-block:: chapel

      proc u.init() {
        init this;
      }

      proc u.init(in x: real) {
        this.x = x;
      }

      proc u.init(in y: int) {
        this.y = y;
      }

      proc u.init(in z: int) {
        this.z = z;
      }

These initializers would then support initialization expressions as
follows:

   *Example (unionCompilerInits.chpl)*.

   .. BLOCK-test-chapelpre

      union u {
        var x: real;
        var y: int;
        var z: int;
      }

   .. code-block:: chapel

      var u0 = new u(),
          u1 = new u(1.2),
          u2 = new u(x=3.4),
          u3 = new u(y=5),
          u4 = new u(z=6);

      // var u5 = new u(7);  // ambiguous due to y and z

   .. BLOCK-test-chapelpost

      writeln((u0, u1, u2, u3, u4));

   .. BLOCK-test-chapeloutput

      ((), (x = 1.2), (x = 3.4), (y = 5), (z = 6))

.. index::
   single: user-defined initializers; unions
   single: unions; user-defined initializers
.. _User_Defined_Union_Initializers:

User-Defined Initializers
~~~~~~~~~~~~~~~~~~~~~~~~~

User-defined initializers for unions are similar to those of classes
and records in that initial assignments to fields are interpreted as
initializations rather than assignments.  However, they differ since
only one field must be initialized for the union to be considered
initialized as a whole.

Thus, where a class or record initializer implicitly or explicitly
initializes every field in the type after which subsequent assignments
are treated as initialization, in a union only the first assignment
along any control flow path is treated as initialization, and all
subsequent ones are assignments.  For that reason, it is only
important that branches of a conditional are consistent in whether or
not they initialize *any* field of a union; there is no need to
initialize similar fields along each branch as with records and
classes.

As with user-defined initializers on records and classes, the ``init
this;`` statement can be used to explicitly mark the transition from
uninitialized object to initialized.  If it precedes any field
initializations, it will cause the union to be initialized such that
no field is active.

The following program demonstrates some of these principles using
a simple union type:

   *Example (unionUserInits.chpl)*.

   .. code-block:: chapel

      union u {
        var x: real;
        var y: int;
        var z: int;

        proc init() {
          this.x = 1.2;  // initializes the 'x' field
          this.y = 3;    // assigns the 'y' field

          // Note that this default initializer does pointless work.
          // Since only one field can be active, the assignment to 'y'
          // overrides the initialization of 'x', so the body could
          // simply be 'this.y = 3;' to get the same behavior.
        }

        proc init(initY: bool, val: int) {
          // an initializer that initializes one of y or z

          if initY then
            this.y = val;  // initialize the 'y' field
          else
            this.z = val;  // initialize the 'z' field
        }

        proc init(msg: string) {
          init this;           // initializes union with no active field
          writeln(msg, this);  // prints '()' for 'this' since no field is active
        }
      }

      var u0 = new u(),
          u1 = new u(initY=true,  val=45),
          u2 = new u(initY=false, val=78),
          u3 = new u("no active field:");

   .. BLOCK-test-chapelpost

      writeln((u0, u1, u2, u3));

   .. BLOCK-test-chapeloutput

      no active field:()
      ((y = 3), (y = 45), (z = 78), ())


Note that the distinction between initialization and assignment are
fairly inconsequential for the simple scalar types in this example,
but can be more important for richer field types, such as records.


Common Operations
-----------------

.. index::
   single: unions; assignment
.. _Union_Assignment:

Union Assignment
~~~~~~~~~~~~~~~~

Union assignment is by value. The active field of the union on the
right-hand side of the assignment is assigned to same field of the
union on the left-hand side, and this field is made active.

.. index::
   single: unions; equality
   single: unions; inequality
   single: unions; ==
   single: unions; !=
   single: == (union)
   single: != (union)
.. _Union_Comparison_Operators:

Default Comparison Operators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Default comparison operators are defined for unions such that if no
more specific comparison is found, these defaults will be used.  These
default comparisons are defined using the following signatures:

.. code-block:: chapel

   operator ==(a: union, b: union) : bool
   operator !=(a: union, b: union) : bool

When these comparisons are applied to two union values of distinct
types, a compiler error is generated.

These default comparisons consider two union values to be equal if (a)
both unions have the same active field and (b) the respective values
of this field are considered equal using ``==``.  Otherwise they are
considered not equal.


.. index::
   single: unions; pattern matching
.. _Union_Pattern_Matching:

Union Pattern Matching
----------------------

There are two primary ways to perform pattern matching on unions: using a
``union select`` statement or using the :proc:`~ChapelUnion.union.visit` method.
Pattern matching allows users to decompose a union based on the currently
active field.

   *Example (patternMatching.chpl)*.

   Unions can use a ``union select`` statement to perform pattern matching on the
   active field of the union.

   .. BLOCK-test-chapelpre

      union U {
        var x: int;
        var y: real;
      }
      var u: U;
      u.x = 3;

   .. code-block:: chapel

      union select u {
        when x {
          writeln("x is active with value ", x);
        }
        when y {
          writeln("y is active with value ", y);
        }
        otherwise {
          writeln("no field is active");
        }
      }

   .. BLOCK-test-chapeloutput

      x is active with value 3

   *Example (patternMatchVisit.chpl)*.

   The :proc:`~ChapelUnion.union.visit` method can be used to perform pattern matching on the
   active field of the union, with an associated visitor functor.

   .. BLOCK-test-chapelpre

      union U {
        var x: int;
        var y: real;
      }
      var u: U;
      u.x = 3;

   .. code-block:: chapel

      u.visit(proc(x: int) { writeln("x is active with value ", x); },
              proc(y: real) { writeln("y is active with value ", y); });

   .. BLOCK-test-chapeloutput

      x is active with value 3

It is also possible to check the active field of a union with normal
conditionals.

   *Example (patternMatchConditional.chpl)*.

   .. BLOCK-test-chapelpre

      union U {
        var x: int;
        var y: real;
      }
      var u: U;
      u.x = 3;

   .. code-block:: chapel

      if u.getActiveIndex() == U.x {
        writeln("x is active with value ", u.x);
      } else if u.getActiveIndex() == U.y {
        writeln("y is active with value ", u.y);
      } else {
        writeln("no field is active");
      }

   .. BLOCK-test-chapeloutput

      x is active with value 3


.. index::
   single: unions; methods
.. _Union_Methods:

Predefined Routines on Unions
-----------------------------

.. include:: /builtins/ChapelUnion.rst
