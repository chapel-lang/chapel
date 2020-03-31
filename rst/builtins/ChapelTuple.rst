.. default-domain:: chpl

.. module:: ChapelTuple
   :synopsis: Predefined Functions on Tuples.

Tuples
======
Predefined Functions on Tuples.

Tuples are a predefined structured type in Chapel. They are specified
in the Tuples chapter of the Chapel Language Specification.
This page lists the predefined functions on tuples.
They are always available to all Chapel programs.

Besides the functions defined here, the Chapel Language specification
defines other operations available on tuples: indexing, iteration,
assignment, and unary, binary, and relational operators.

.. function:: proc tuple.size param

   Returns the number of components of the tuple.

.. function:: proc tuple.indices

   Returns the range of indices that are legal for indexing into the
   tuple: ``1..this.size``.



.. function:: proc isTupleType(type t) param

   
   Returns `true` if its argument is a tuple type.
   The argument must be a type.
   

.. function:: proc isHomogeneousTupleType(type t) param

   
   Returns `true` if its argument is a homogeneous tuple type.
   The argument must be a type.
   

.. function:: proc max(type t): t

   
   Returns a tuple of type t with each component set to ``max``
   of the type in the corresponding component of the argument.
   

.. function:: proc min(type t): t

   
   Returns a tuple of type t with each component set to ``min``
   of the type in the corresponding component of the argument.
   

