.. default-domain:: chpl

.. module:: FunctionalOperations
   :synopsis: The `FunctionalOperations` module provides operations that act on

FunctionalOperations
====================
**Usage**

.. code-block:: chapel

   use FunctionalOperations;

The `FunctionalOperations` module provides operations that act on
iterators in a functional programming style.


.. itermethod:: iter _iteratorRecord.map(fn)

   Apply `fn` to each element yielded by this iterator and yield the value
   returned by `fn`.  `fn` must take a single argument and return a value.
   

.. itermethod:: iter _iteratorRecord.filter(fn)

   Apply `fn` to each element yielded by this iterator and yield the values
   it returns `true` for.  `fn` must take a single argument and return a
   boolean value.
   

.. method:: proc _iteratorRecord.consume(fn)

   Apply `fn` to each element yielded by this iterator and ignore the return
   value from `fn` if any.
   

