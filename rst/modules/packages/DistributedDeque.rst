.. default-domain:: chpl

.. module:: DistributedDeque
   :synopsis: Summary

DistributedDeque
================
**Usage**

.. code-block:: chapel

   use DistributedDeque;



Summary
________


A parallel-safe scalable distributed deque. A deque is a double-ended queue that supports
insertion and removal from both ends of the queue, effectively supporting both
FIFO, LIFO, and a Total ordering, where the order in which you add them will be
the exact order you remove them in; for emphasis, a Deque can be used as a Queue,
a Stack, and a List respectively.

.. note::

  This module is a work in progress and may change in future releases.

Usage
_____

First, the :record:`DistDeque` must be initialized before use by calling its initializer.

.. code-block:: chapel

  var deque = new DistDeque(int, cap=maxElem, targetLocales=ourLocales);

The deque can be used as a queue by using the :proc:`DistributedDequeImpl.enqueue` and :proc:`DistributedDequeImpl.dequeue` convenience
methods or inserting from one end to remove from another...

.. code-block:: chapel

  deque.enqueue(1);
  var (hasElem, elem) = deque.dequeue();

The deque can be used as a stack by using the :proc:`DistributedDequeImpl.push` and :proc:`DistributedDequeImpl.pop` convenience methods,
or insertion and removing from the same ends...

.. code-block:: chapel

  deque.push(1);
  var (hasElem, elem) = deque.pop();

The deque can be used as a list by using the :proc:`DistributedDequeImpl.pushBack`, :proc:`DistributedDequeImpl.pushFront`, :proc:`DistributedDequeImpl.popBack`,
and :proc:`DistributedDequeImpl.popFront` methods. While the deque is not indexable, the ability to `append` or `prepend`
is powerful enough to allow a total ordering, allowing the user to define the order by letting them
insert and remove at whichever ends they so choose.

.. code-block:: chapel

  var deque = new DistDeque(int);
  forall i in 1 .. N {
    if i % 2 == 0 then deque.pushFront(i);
    else deque.pushBack(i);
  }

The deque supports both serial and parallel iteration, and a means to iterate in a particular order
(currently only FIFO and LIFO) using the ``Ordering`` enumerator.

.. code-block:: chapel

  for elt in deque.these(Ordering.FIFO) {
    // ...
  }

  for elt in deque.these(Ordering.LIFO) {
    // ...
  }

The deque can also be used in a reduction, although currently reduction only used parallel-iteration, hence
reduction will be performed in an unordered fashion. In the future, a specific function may be created to
allow reduction in a certain ordering...

.. code-block:: chapel

  deque.addBulk(1..100);
  var result = + reduce deque;

Bugs and Known Issues
_____________________

1.  It is not safe to call other methods while iterating, as it will lead to deadlock. It is an open question
    whether using a snapshot approach is better to allow concurrent operations at the expense of elevated memory
    consumption, and iterating directly over elements while holding locks, which strangles potential concurrency.

2.  Reduction cannot be performed in any ordered way. This may be fixed in the near future, either by adding
    pseudo-parallel iterators that merely yield sequentially in order, or by creating a method to perform reduction
    for the user in a specified ordering.

3.  This data structure **requires** network atomic support for scalability, and without it will result in degrading
    performance. It is another open question whether a specific implementation that is more friendly for remote-execution
    atomic operations should be provided.

4.  The ordered serial iterators currently do not work when the ``globalHead`` or ``globalTail`` are negative, which is a
    result of iteration being an after-thought. This will be improved upon soon, but for now if you use :proc:`DistributedDequeImpl.pushBack`
    or :proc:`DistributedDequeImpl.pushFront` methods, I would advise against using them for now.

Planned Improvements
____________________

1.  Double the size of each successor up to some maximum, similar to :mod:`DistributedBag` for unroll blocks.
    Currently they are fixed-sized, but it can benefit from improved locality if a lot of elements are added at
    once.

Methods
_______

.. data:: config param distributedDequeBlockSize = 8

   
   Size of each unroll block for each local deque node.
   

.. enum:: enum Ordering { NONE, FIFO, LIFO }

   
   The ordering used for serial iteration. ``NONE``, the default, is the most performant
   and is algorithmically similar to parallel iteration.
   


.. record:: DistDeque

   
   A parallel-safe scalable distributed double-ended queue that supports both
   insertion and removal from either end of the queue. Can be used as a Queue,
   Stack, or even a List.
   


   .. attribute:: type eltType

   .. attribute:: var _impl: unmanaged nilable DistributedDequeImpl(eltType)

   .. method:: proc init(type eltType, cap = -1, targetLocales = Locales)

.. class:: DistributedDequeImpl : CollectionImpl

   .. attribute:: var cap: int

      
      Capacity, the maximum number of elements a Deque can hold. A `cap` of -1 is
      considered unbounded.
      

   .. attribute:: var targetLocales: [targetLocDom] locale

      
      Locales to distribute the `Deque` across.
      

   .. method:: proc init(type eltType, cap: int = -1, targetLocales: [?locDom] locale = Locales)

   .. method:: override proc add(elt: eltType): bool

      
      Syntactic sugar for `pushBack`.
      

   .. method:: override proc remove(): (bool, eltType)

      
      Syntactic sugar for `popFront`.
      

   .. method:: proc enqueue(elt: eltType): bool

      
      Syntactic sugar for `pushBack`.
      

   .. method:: proc dequeue(): (bool, eltType)

      
      Syntactic sugar for `popFront`.
      

   .. method:: proc push(elt: eltType): bool

      
      Syntactic sugar for `pushBack`.
      

   .. method:: proc pop(): (bool, eltType)

      
      Syntactic sugar for `popBack`.
      

   .. method:: proc pushBack(elt: eltType): bool

      
      Appends the element to the tail.
      

   .. method:: proc popBack(): (bool, eltType)

      
      Removes the element at the tail.
      

   .. method:: proc pushFront(elt: eltType): bool

      
      Appends the element to the head.
      

   .. method:: proc popFront(): (bool, eltType)

      
      Removes the element at the head.
      

   .. method:: override proc getSize(): int

      
      Obtains the number of elements held by this queue.
      

   .. method:: override proc contains(elt: eltType): bool

      
      Performs a lookup for the element in the data structure.
      

   .. itermethod:: iter these(param order: Ordering = Ordering.NONE): eltType

      
      Iterate over all elements in the deque in the order specified.
      

   .. itermethod:: iter these(param order: Ordering = Ordering.NONE): eltType

   .. itermethod:: iter these(param order: Ordering = Ordering.NONE): eltType

   .. itermethod:: iter these(param order: Ordering = Ordering.NONE, param tag: iterKind)

   .. itermethod:: iter these(param order: Ordering = Ordering.NONE, param tag: iterKind, followThis)

