First line

left-hand side of the assignment and this same field is marked as set.

Ranges
======

[Ranges]

A *range* is a first-class, constant-space representation of a regular

-  Chapter \ `[Scope] <#Scope>`__, Scope, describes the scope of this
   specification.



A record declaration statement creates a record
type §\ `18.1 <#Record_Declarations>`__. A variable of record type
contains all and only the fields defined by that type
(§`18.1.1 <#Record_Types>`__). Value semantics imply that the type of a
record variable is known at compile time (i.e. it is statically typed).

   *Example (param-functions.chpl)*.

   In the code BLOCK-chapel

   ::

      proc sumOfSquares(param a: int, param b: int) param
        return a**2 + b**2;

      var x: sumOfSquares(2, 3)*int;

   BLOCK-test-chapelpost

   ::

      writeln(x);

   BLOCK-test-chapeloutput

   ::

      (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)


   *Example*.

   In the code BLOCK-chapel

   ::

      var D: domain(2) = {1..n, 1..n};

   ``D`` is defined as a two-dimensional, nonstridable rectangular
   domain with an index type of ``2*int`` and is initialized to contain
   the set of indices :math:`(i,j)` for all :math:`i` and :math:`j` such
   that :math:`i \in {1, 2, \ldots, n}` and
   :math:`j \in {1, 2, \ldots, n}`.


Last line
