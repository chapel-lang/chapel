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
contain data is a runtime error. When a union is initialized, it is in
an unset state so that no field contains data.

.. warning::

   Unions are currently unstable and may change in ways that will break
   their current uses.

.. index::
   single: types; unions
   single: union types
.. _Union_Types:

Union Types
-----------

The syntax of a union type is summarized as follows:

.. code-block:: syntax

   union-type:
     identifier

The union type is specified by the name of the union type. This
simplification from class and record types is possible because generic
unions are not supported.

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
     procedure-declaration-statement
     iterator-declaration-statement
     variable-declaration-statement
     empty-statement

If the ``extern`` keyword appears before the ``union`` keyword, then an
external union type is declared. An external union is used within Chapel
for type and field resolution, but no corresponding backend definition
is generated. It is presumed that the definition of an external union
type is supplied by a library or the execution environment.

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


The currently active field of a union can be queried at runtime with the
:proc:`~ChapelUnion.union.getActiveIndex` method. If the union is not yet
initialized, then :proc:`~ChapelUnion.union.getActiveIndex` returns -1.

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
   single: unions; pattern matching
.. _Union_Pattern_Matching:

Union Pattern Matching
----------------------

There are two primary ways to perform pattern matching on unions: using a
``select`` statement or using the :proc:`~ChapelUnion.union.visit` method.

   *Example (patternMatchSelect.chpl)*.

   Unions can use a ``select`` statement to perform pattern matching on the
   active field of the union.

   .. BLOCK-test-chapelpre

      union U {
        var x: int;
        var y: real;
      }
      var u: U;
      u.x = 3;

   .. code-block:: chapel

      select u {
        when U.x {
          writeln("x is active with value ", u.x);
        }
        when U.y {
          writeln("y is active with value ", u.y);
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
   single: unions; assignment
.. _Union_Assignment:

Union Assignment
----------------

Union assignment is by value. The field set by the union on the
right-hand side of the assignment is assigned to the union on the
left-hand side of the assignment and this same field is marked as set.

.. index::
   single: unions; methods
.. _Union_Methods:

Predefined Routines on Unions
-----------------------------

.. include:: /builtins/ChapelUnion.rst
