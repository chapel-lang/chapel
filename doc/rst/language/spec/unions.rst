.. default-domain:: chpl

.. _Chapter-Unions:

Unions
======

Unions have the semantics of records, however, only one field in the
union can contain data at any particular point in the program’s
execution. Unions are safe so that an access to a field that does not
contain data is a runtime error. When a union is initialized, it is in
an unset state so that no field contains data.

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
     type-declaration-statement
     procedure-declaration-statement
     iterator-declaration-statement
     variable-declaration-statement
     empty-statement

If the ``extern`` keyword appears before the ``union`` keyword, then an
external union type is declared. An external union is used within Chapel
for type and field resolution, but no corresponding backend definition
is generated. It is presumed that the definition of an external union
type is supplied by a library or the execution environment.

.. _Union_Fields:

Union Fields
~~~~~~~~~~~~

Union fields are accessed in the same way that record fields are
accessed. It is a runtime error to access a field that is not currently
set.

Union fields should not be specified with initialization expressions.

.. _Union_Assignment:

Union Assignment
----------------

Union assignment is by value. The field set by the union on the
right-hand side of the assignment is assigned to the union on the
left-hand side of the assignment and this same field is marked as set.
