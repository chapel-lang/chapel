
.. _readme-noinit:

.. default-domain:: chpl

=================================================
Avoiding Array Element Initialization with noinit
=================================================

Overview
--------

Chapel 1.23 includes an early implementation of ``noinit`` for arrays.
As of 1.23, ``noinit`` cannot be used with types other than arrays and it
only works for types that are trivially copyable.

Motivation
----------

In some cases, one would like to allocate memory for array elements
without initializing the elements. This might aid in optimization. The
elements might be initialized later and then the default initialization
might be redundant. Additionally, the default initialization might impact
the memory affinity of the array on a NUMA system.

Example
-------

To use the ``noinit`` feature, declare an array and initialize it with
the keyword ``noinit`` as this example shows:

.. code-block:: chapel

  config const n = 10;
  var D = {1..n};
  var A: [D] real = noinit;  // A's elements won't be initialized here

  // Reading one of A's elements here is a memory error -
  // it will read uninitialized memory.

  // A's elements can be initialized using the assignment operator
  forall a in A do
    a = 0.0;

Future Work
-----------

We are hoping to extend this feature to:

 * provide a way to indicate when the array is fully initialized (to
   support memory registration for communication, e.g.)
 * provide an explicit way to move-initialize elements (so that the
   feature can be extended to types that are not trivially copyable)
 * adjust the implementation to avoid initializing new elements when a
   ``noinit`` ed array is resized through its domain.
