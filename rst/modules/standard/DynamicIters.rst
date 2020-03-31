.. default-domain:: chpl

.. module:: DynamicIters
   :synopsis: This module contains several iterators that can be used to drive a `forall`

DynamicIters
============
**Usage**

.. code-block:: chapel

   use DynamicIters;


This module contains several iterators that can be used to drive a `forall`
loop by performing dynamic and adaptive splitting of a range's iterations.

For more information, see *User-Defined Parallel Zippered Iterators
in Chapel*. Bradford L. Chamberlain, Sung-Eun Choi, Steven J. Deitz,
Angeles Navarro. *PGAS 2011: Fifth Conference on Partitioned Global
Address Space Programming Models*, October 2011.

.. data:: config param debugDynamicIters: bool = false

   
   Toggle debugging output.

.. iterfunction:: iter dynamic(c: range(?), chunkSize: int = 1, numTasks: int = 0)

   
   
   :arg c: The range to iterate over. The length of the range must be greater
           than zero.
   :type c: `range(?)`
   
   :arg chunkSize: The size of chunks to be yielded to each thread. Must be
                   greater than zero.
   :type chunkSize: `int`
   
   :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                  has the value 0, it will use the value indicated by
                  ``dataParTasksPerLocale``.
   :type numTasks: `int`
   
   :yields: Indices in the range ``c``.
   
   
   This iterator is equivalent to the dynamic scheduling approach of OpenMP.
   
   Given an input range ``c``, each task is assigned chunks of size
   ``chunkSize`` from ``c`` (or the remaining iterations if there are fewer
   than ``chunkSize``). This continues until there are no remaining iterations
   in ``c``.
   
   This iterator can be called in serial and zippered contexts.
   

.. iterfunction:: iter dynamic(c: domain, chunkSize: int = 1, numTasks: int = 0, parDim: int = 1)

.. iterfunction:: iter guided(c: range(?), numTasks: int = 0)

   
   
   :arg c: The range to iterate over. Must have a length greater than zero.
   :type c: `range(?)`
   
   :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                  has the value 0, it will use the value indicated by
                  ``dataParTasksPerLocale``.
   :type numTasks: `int`
   
   :yields: Indices in the range ``c``.
   
   This iterator is equivalent to the guided policy of OpenMP: Given an input
   range ``c``, each task is assigned chunks of variable size, until there are
   no remaining iterations in ``c``. The size of each chunk is the number of
   unassigned iterations divided by the number of tasks, ``numTasks``. The size
   decreases approximately exponentially to 1. The splitting strategy is
   therefore adaptive.
   
   This iterator can be called in serial and zippered contexts.
   

.. iterfunction:: iter guided(c: domain, numTasks: int = 0, parDim: int = 1)

.. iterfunction:: iter adaptive(c: range(?), numTasks: int = 0)

   
   
   :arg c: The range to iterate over. Must have a length greater than zero.
   :type c: `range(?)`
   
   :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                  has the value 0, it will use the value indicated by
                  ``dataParTasksPerLocale``.
   :type numTasks: `int`
   
   :yields: Indices in the range ``c``.
   
   This iterator implements a naive adaptive binary splitting work-stealing
   strategy: Initially the leader iterator distributes the range to split, ``c``,
   evenly among the ``numTasks`` tasks.
   
   Then, each task performs adaptive splitting on its local sub-range's iterations.
   When a task exhausts its local iterations, it steals and splits from the
   range of another task (the victim). The splitting method on the local range
   and on the victim range is binary: i.e. the size of each chunk is computed as
   the number of unassigned iterations divided by 2. There are three stealing
   strategies that can be selected at compile time using the config param
   :param:`methodStealing`.
   
   This iterator can be called in serial and zippered contexts.

.. enum:: enum Method { Whole = 0, RoundRobin = 1, WholeTail = 2 }

   
   The enum used to represent adaptive methods.
   
   - ``Whole``
     Each task without work tries to steal from its neighbor range
     until it exhausts that range. Then the task continues with the next
     neighbor range, and so on until there is no more work. This is the default
     policy.
   
   - ``RoundRobin``
     Each task without work tries to steal once from its neighbor range, next
     from the following neighbor range and so on in a round-robin way until
     there is no more work.
   
   - ``WholeTail``
     Similar to the ``Whole`` method, but now the splitting in the victim
     range is performed from its tail.


.. data:: config param methodStealing = Method.Whole

   
   Used to select the adaptive stealing method. Defaults to ``Whole``.
   See :data:`Method` for more information.

.. iterfunction:: iter adaptive(c: domain, numTasks: int = 0, parDim: int = 1)

