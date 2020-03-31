.. default-domain:: chpl

.. module:: DistributedBag
   :synopsis: Summary

DistributedBag
==============
**Usage**

.. code-block:: chapel

   use DistributedBag;



Summary
_______

A parallel-safe distributed multiset implementation that scales in terms of
nodes, processors per node (PPN), and workload; The more PPN, the more segments
we allocate to increase raw parallelism, and the larger the workload the better
locality (see :const:`distributedBagInitialBlockSize`). This data structure is unordered
and employs its own work stealing algorithm to balance work across nodes.

.. note::

  This module is a work in progress and may change in future releases.

Usage
_____

To use :record:`DistBag`, the initializer must be invoked explicitly to
properly initialize the structure. Using the default state without initializing
will result in a halt.

.. code-block:: chapel

  var bag = new DistBag(int, targetLocales=ourTargetLocales);


While the bag is safe to use in a distributed manner, each node always operates on it's privatized
instance. This means that it is easy to add data in bulk, expecting it to be distributed, when in
reality it is not; if another node needs data, it will steal work on-demand. This may not always be
desired, and likely will more memory consumption on a single node. We offer a way for the user to
invoke a more static load balancing approach, called :proc:`DistributedBagImpl.balance`, which will redistributed work.

.. code-block:: chapel

  bag.addBulk(1..N);
  bag.balance();

Planned Improvements
____________________

1.  Dynamic work-stealing will require an overhaul to use a helper algorithm to keep down
    the number of tasks spawned. Currently user tasks will wait on the current work-stealer
    task, which will spawn is own helper tasks which act as shepherds, which then spawns more
    in a fork-join fashion. This leads to an excessive amount of tasks being spawned at once.
    To make matters worse, the waiting tasks don't even get any elements, nor does the work
    stealing task, which opens up the possibility of live-lock where nodes steal work back
    and forth before either can process it.
2.  Static work-stealing (A.K.A :proc:`DistributedBagImpl.balance`) requires a rework that performs a more distributed
    and fast way of distributing memory, as currently 'excess' elements are shifted to a single
    node to be redistributed in the next pass. On the note, we need to collapse the pass for moving
    excess elements into a single pass, hopefully with a zero-copy overhead.

Methods
_______

.. data:: config const distributedBagInitialBlockSize = 1024

   
   The initial amount of elements in an unroll block. Each successive unroll block
   is double the size of it's predecessor, allowing for better locality for when
   there are larger numbers of elements. The better the locality, the better raw
   performance and easier it is to redistribute work.
   

.. data:: config const distributedBagWorkStealingRatio = 0.25

   
   To prevent stealing too many elements (horizontally) from another node's segment
   (hence creating an artificial load imbalance), if the other node's segment has
   less than a certain threshold (see :const:`distributedBagWorkStealingMemCap`) but above
   another threshold (see :const:`distributedBagWorkStealingMinElems`), we steal a percentage of their
   elements, leaving them with majority of their elements. This way, the amount the
   other segment loses is proportional to how much it owns, ensuring a balance.
   

.. data:: config const distributedBagWorkStealingMemCap: real = 1.0

   
   The maximum amount of work to steal from a horizontal node's segment. This
   should be set to a value, in megabytes, that determines the maximum amount of
   data that should be sent in bulk at once. The maximum number of elements is
   determined by: (:const:`distributedBagWorkStealingMemCap` * 1024 * 1024) / sizeof(``eltType``).
   For example, if we are storing 8-byte integers and have a 1MB limit, we would
   have a maximum of 125,000 elements stolen at once.
   

.. data:: config const distributedBagWorkStealingMinElems = 1

   
   The minimum number of elements a horizontal segment must have to become eligible
   to be stolen from. This may be useful if some segments produce less elements than
   others and should not be stolen from.
   

.. data:: config const distributedBagMaxBlockSize = 1024*1024

   
   The maximum amount of elements in an unroll block. This is crucial to ensure memory
   usage does not rapidly grow out of control.
   

.. record:: DistBag

   
   A parallel-safe distributed multiset implementation that scales in terms of
   nodes, processors per node (PPN), and workload; The more PPN, the more segments
   we allocate to increase raw parallelism, and the larger the workload the better
   locality (see :const:`distributedBagInitialBlockSize`). This data structure is unordered and employs
   its own work-stealing algorithm, and provides a means to obtain a privatized instance of
   the data structure for maximized performance.
   


   .. attribute:: type eltType

   .. attribute:: var _impl: unmanaged nilable DistributedBagImpl(eltType)

.. class:: DistributedBagImpl : CollectionImpl

   .. attribute:: var targetLocales: [targetLocDom] locale

      
      The locales to allocate bags for and load balance across.
      

   .. method:: proc init(type eltType, targetLocales: [?targetLocDom] locale = Locales)

   .. method:: override proc add(elt: eltType): bool

      
      Insert an element to this node's bag. The ordering is not guaranteed to be
      preserved.
      

   .. method:: override proc remove(): (bool, eltType)

      
      Remove an element from this node's bag. The order in which elements are removed
      are not guaranteed to be the same order it has been inserted. If this node's
      bag is empty, it will attempt to steal elements from bags of other nodes.
      

   .. method:: override proc getSize(): int

      
      Obtain the number of elements held in all bags across all nodes. This method
      is best-effort and can be non-deterministic for concurrent updates across nodes,
      and may miss elements or even count duplicates resulting from any concurrent
      insertion or removal operations.
      

   .. method:: override proc contains(elt: eltType): bool

      
      Performs a lookup to determine if the requested element exists in this bag.
      This method is best-effort and can be non-deterministic for concurrent
      updates across nodes, and may miss elements resulting from any concurrent
      insertion or removal operations.
      

   .. method:: proc clear()

      
      Clear all bags across all nodes in a best-effort approach. Elements added or
      moved around from concurrent additions or removals may be missed while clearing.
      

   .. method:: proc balance()

      
      Triggers a more static approach to load balancing, fairly redistributing all
      elements fairly for bags across nodes. The result will result in all segments
      having roughly the same amount of elements.
      
      .. note::
      
        This method is very heavy-weight in that it should not be called too
        often. Dynamic work stealing handles cases where there is a relatively fair
        distribution across majority of nodes, but this should be called when you have
        a severe imbalance, or when you have a smaller number of elements to balance.
        Furthermore, while this operation is parallel-safe, it should be called from at
        most one task.
      

   .. itermethod:: override iter these(): eltType

      
      Iterate over each bag in each node. To avoid holding onto locks, we take
      a snapshot approach, increasing memory consumption but also increasing parallelism.
      This allows other concurrent, even mutating, operations while iterating,
      but opens the possibility to iterating over duplicates or missing elements
      from concurrent operations.
      
      .. note::
      
        `zip` iteration is not yet supported with rectangular data structures.
      
      .. warning::
      
        Iteration takes a snapshot approach, and as such can easily result in a
        Out-Of-Memory issue. If the data structure is large, the user is doubly advised to use
        parallel iteration, for both performance and memory benefit.
      
      

   .. itermethod:: iter these(param tag: iterKind)

   .. itermethod:: iter these(param tag: iterKind, followThis)

