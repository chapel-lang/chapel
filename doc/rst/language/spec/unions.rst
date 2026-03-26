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

.. code-block::

  union U {
    var x: int;
    var y: real;
  }
  var u: U;
  u.x = 3; // sets field x
  writeln(u.x);
  writeln(u.y); // runtime error: field y is not set

Union fields may also be accessed via a compile-time (``param``) index.

.. code-block::

  union U {
    var x: int;
    var y: real;
  }
  var u: U;
  u(0) = 3; // sets the first field, which is x
  writeln(u.(0)); // prints 3
  writeln(u.(1)); // runtime error: field y is not set

The currently active field of a union can be queried at runtime with the
``getActiveIndex`` method.

.. code-block::

  union U {
    var x: int;
    var y: real;
  }
  var u: U;
  u.x = 3; // sets field x
  writeln(u.getActiveIndex()); // prints 0

Union fields should not be specified with initialization expressions.

.. index::
   single: unions; assignment
.. _Union_Assignment:

Union Assignment
----------------

Union assignment is by value. The field set by the union on the
right-hand side of the assignment is assigned to the union on the
left-hand side of the assignment and this same field is marked as set.

Predefined Routines on Unions
-----------------------------

.. include:: builtins/ChapelUnion.rst
