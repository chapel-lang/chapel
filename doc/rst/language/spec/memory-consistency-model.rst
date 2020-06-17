.. _Chapter-Memory_Consistency_Model:

Memory Consistency Model
========================

In this section, we describe Chapel’s memory consistency model. The
model is based on *sequential consistency for data-race-free* programs
as adopted by C11, C++11, Java, UPC, and Fortran 2008.

Sequential consistency (SC) means that all Chapel tasks agree on the
interleaving of memory operations and this interleaving results in an
order is consistent with the order of operations in the program source
code. *Conflicting memory operations*, i.e., operations to the same
variable, or memory location, and one of which is a write, form a data
race if they are from different Chapel tasks and can be executed
concurrently. Accesses to the same variable from different tasks can
result from the tasks referencing the same variable directly – or
indirectly via aliases. Aliases arise, for example, when using ``ref``
variables, argument intents, return intents, task intents and forall
intents.

Any Chapel program with a data race is not a valid program, and an
implementation cannot be relied upon to produce consistent behavior.
Valid Chapel programs will use synchronization constructs such as
*sync*, *single*, or *atomic* variables or higher-level constructs based
on these to enforce ordering for conflicting memory operations.

The following design principles were used in developing Chapel’s memory
consistency model:

#. Sequential programs have program order semantics. Programs that are
   completely sequential cannot have data races and should appear to
   execute as though each statement was executed one at a time and in
   the expected order.

#. Chapel’s fork-join constructs introduce additional order
   dependencies. Operations within a task cannot behave as though they
   started before the task started. Similarly, all operations in a task
   must appear to be completed to a parent task when the parent task
   joins with that task.

#. Multi-locale programs have the same memory consistency model as
   single-locale programs. The Chapel language seeks to allow a single
   description of an algorithm to work with different data
   distributions. A result of this property is that an expression of a
   program must be correct whether it is working on local or distributed
   data.

#. Chapel’s memory model should be as relaxed as possible while still
   consistent with these design principles. In particular, making all
   operations sequentially consistent is not likely to enable good
   performance. At the same time, sequential consistency should be
   available to programmers when requested.

See *A Primer on Memory Consistency and Cache Coherence* by Sorin,
*et al.* for more background information on memory consistency models.
This chapter will proceed in a manner inspired by the :math:`XC` memory
model described there.

.. _SC_for_DRF:

Sequential Consistency for Data-Race-Free Programs
--------------------------------------------------

Sequential consistency for data-race-free programs is described in terms
of two orders: *program order* and *memory order*. The *program order*
:math:`<_p` is a partial order describing serial or fork-join
parallelism dependencies between variable reads and writes. The *memory
order* :math:`<_m` is a total order that describes the semantics of
synchronizing memory operations (via ``atomic``, ``sync`` or ``single``
variables) with sequential consistency. Non-SC atomic operations
(described in Section \ `31.2 <#non_sc_atomics>`__) do not create this
total order.

Note that ``sync/single`` variables have memory consistency behavior
equivalent to a sequence of SC operations on ``atomic`` variables. Thus
for the remainder of the chapter, we will primarily focus on operations
on ``atomic`` variables.

We will use the following notation:

-  :math:`L(a)` indicates a *load* from a variable at address :math:`a`.
   :math:`a` could refer to local or remote memory.

-  :math:`S(a)` indicates a *store* to a variable at address :math:`a`.
   :math:`a` could refer to local or remote memory.

-  :math:`A_{sc}(a)` indicates an *atomic operation* on a variable at
   address :math:`a` with sequential consistency. The variable at
   address :math:`a` could refer to local or remote memory. Atomic
   operations must be completed as a single operation (i.e. atomically),
   and so it is not possible to observe an intermediate state from an
   atomic operation under any circumstances.

-  :math:`A_r(a,o)` indicates an *atomic operation* on a variable at
   address :math:`a` with ordering constraint :math:`o`, where :math:`o`
   can be one of *relaxed*, *acquire*, or *release* (see
   Section \ `31.2 <#non_sc_atomics>`__). As with :math:`A_{sc}(a)`,
   relaxed atomic operations must be completed as a single operation.

-  :math:`L(a)`, :math:`S(a)`, :math:`A_{sc}(a)`, and :math:`A_r(a,o)`
   are also called *memory operations*

-  :math:`X <_p Y` indicates that :math:`X` precedes :math:`Y` in
   program order

-  :math:`X <_m Y` indicates that :math:`X` precedes :math:`Y` in memory
   order

-  ``t = begin{X}`` starts a new task named :math:`t` to execute
   :math:`X`

-  ``waitFor($t_1$..$t_n$)`` waits for tasks :math:`t_1..t_n` to
   complete

-  ``on(L)`` migrates the running task to locale :math:`L`. Note that
   while the ``on`` statement may change the locale on which the current
   task is running, it has no impact on the memory consistency
   requirements.

For the purposes of describing this memory model, it is assumed that
Chapel programs will be translated into sequences of *memory
operations*, ``begin`` statements, and ``waitFor`` statements. The
translation of a Chapel program into a sequence of *memory operations*
must preserve sequential program semantics. That is, if we have a
snippet of a Chapel program without task operations, such as ``X; Y;``,
the statements :math:`X` and :math:`Y` will be converted into a sequence
of *load*, *store*, and *atomic operations* in a manner that preserves
the behavior of this serial portion of the program. That is,
:math:`X=x_1,x_2,...` and :math:`Y=y_1,y_2,...` where :math:`x_i` and
:math:`y_j` are each a sequence of *load*, *store*, or *atomic
operations* and we have :math:`x_i <_p y_j`.

Likewise, for the purposes of this memory model, Chapel’s parallelism
keywords are viewed as a sequence of operations including the primitives
of starting a task (``begin``) and waiting for some number of tasks
(``waitFor($t_1$..$t_n$)``). In particular:

-  ``forall`` (including promotion) creates some number of tasks
   :math:`m` to execute the :math:`n` iterations of the loop

   (``$t_i$ = begin{some-loop-bodies}`` for each task
   :math:`i=1`..\ :math:`m`) and waits for them to complete
   (``waitFor(t_1..t_m)``). The number of tasks :math:`m` is defined
   by the implementation of the parallel iterator (See
   Section \ `[Iterators] <#Iterators>`__ for details on iterators).

-  ``coforall`` creates one task per loop iteration
   (``t_i = begin{loop-body}`` for all loop iterations
   :math:`i=1..n`) and then waits for them all to complete
   (``waitFor(t_1..t_n)``).

-  ``cobegin`` creates one task per enclosed statement
   (``t_i = begin{X_i}`` for statements
   :math:`X_1`..\ :math:`X_n`) and then waits for them all to complete
   (``waitFor(t_1..t_n)``).

-  ``begin`` creates a task to execute the enclosed statement
   (``t = begin{X}``). The ``sync`` statement waits for all tasks
   :math:`t_i` created by a ``begin`` statement in the dynamic scope of
   the ``sync`` statement that are not within other, nested ``sync``
   statements (``waitFor(t_1..t_n)`` for all :math:`n` such tasks).

.. _program_order:

Program Order
~~~~~~~~~~~~~

Task creation and task waiting create a conceptual tree of program
statements. The task bodies, task creation, and task wait operations
create a partial order :math:`<_p` of program statements. For the
purposes of this section, the statements in the body of each Chapel task
will be implemented in terms of *load*, *store*, and *atomic
operations*.

-  If we have a program snippet without tasks, such as ``X; Y;``, where
   :math:`X` and :math:`Y` are memory operations, then :math:`X <_p Y`.

-  The program ``X; begin{Y}; Z;`` implies :math:`X` :math:`<_p`
   :math:`Y`. However, there is no particular relationship between
   :math:`Y` and :math:`Z` in program order.

-  The program ``t = begin{Y}; waitFor(t); Z;`` implies :math:`Y`
   :math:`<_p` :math:`Z`

-  :math:`X` :math:`<_p` :math:`Y` and :math:`Y` :math:`<_p` :math:`Z`
   imply :math:`X` :math:`<_p` :math:`Z`

.. _memory_order:

Memory Order
~~~~~~~~~~~~

The memory order :math:`<_m` of SC atomic operations in a given task
respects program order as follows:

-  If :math:`A_{sc}(a)<_pA_{sc}(b)` then :math:`A_{sc}(a)<_mA_{sc}(b)`

Every SC atomic operation gets its value from the last SC atomic
operation before it to the same address in the total order :math:`<_m`:

-  Value of :math:`A_{sc}(a)` = Value of
   :math:`max_{<_m} ( A_{sc}'(a)|A_{sc}'(a) <_m A_{sc}(a) )`

For data-race-free programs, every load gets its value from the last
store before it to the same address in the total order :math:`<_m`:

-  Value of :math:`L(a)` = Value of :math:`max_{<_m}`
   :math:`( S(a)|S(a)` :math:`<_m` :math:`L(a)` or :math:`S(a)`
   :math:`<_p` :math:`L(a) )`

For data-race-free programs, loads and stores are ordered with SC
atomics. That is, loads and stores for a given task are in total order
:math:`<_m` respecting the following rules which preserve the order of
loads and stores relative to SC atomic operations:

-  If :math:`L(a)<_pA_{sc}(b)` then :math:`L(a)<_mA_{sc}(b)`

-  If :math:`S(a)<_pA_{sc}(b)` then :math:`S(a)<_mA_{sc}(b)`

-  If :math:`A_{sc}(a)<_pL(b)` then :math:`A_{sc}(a)<_mL(b)`

-  If :math:`A_{sc}(a)<_pS(b)` then :math:`A_{sc}(a)<_mS(b)`

For data-race-free programs, loads and stores preserve sequential
program behavior. That is, loads and stores to the same address in a
given task are in the order :math:`<_m` respecting the following rules
which preserve sequential program behavior:

-  If :math:`L(a) <_p L'(a)` then :math:`L(a) <_m L'(a)`

-  If :math:`L(a) <_p S(a)` then :math:`L(a) <_m S(a)`

-  If :math:`S(a) <_p S'(a)` then :math:`S(a) <_m S'(a)`

.. _non_sc_atomics:

Non-Sequentially Consistent Atomic Operations
---------------------------------------------

Sequential consistency for atomic operations can be a performance
bottleneck under some circumstances. Chapel provides non-SC atomic
operations to help alleviate such situations. Such uses of atomic
operations must be done with care and should generally not be used to
synchronize tasks.

Non-SC atomic operations are specified by providing a *memory order*
argument to the atomic operations. See
Section \ `26.4.1 <#Functions_on_Atomic_Variables>`__ for more
information on the memory order types.

.. _relaxed_atomics:

Relaxed Atomic Operations
~~~~~~~~~~~~~~~~~~~~~~~~~

Although Chapel’s relaxed atomic operations (``memory_order_relaxed``)
do not complete in a total order by themselves and might contribute to
non-deterministic programs, relaxed atomic operations cannot contribute
to a data race that would prevent sequential consistency.

When relaxed atomics are used only for atomicity and not as part of
synchronizing tasks, their effect can be understood in the memory
consistency model described in `31.1 <#SC_for_DRF>`__ by treating them
as ordinary loads or stores with two exceptions:

-  Atomic operations (including relaxed atomic operations) cannot create
   data races.

-  All atomic operations (including relaxed atomic operations) will
   eventually be visible to all other threads. This property is not true
   for normal loads and stores.

.. _unordered_operations:

Unordered Memory Operations
---------------------------

   *Open issue*.

   Syntax for *unordered* operations has not yet been finalized.

..

   *Open issue*.

   Should Chapel provide a memory fence that only completes unordered
   operations started by the current task?

   *Open issue*.

   Should unordered operations on a particular memory address always
   wait for the address to be computed?

..

   *Open issue*.

   Does starting a task or joining with a task necessarily wait for
   unordered operations to complete?

Rather than issuing normal loads and stores to read or write local or
remote memory, a Chapel program can use *unordered* loads and stores
when preserving sequential program behavior is not important. The
following notation for unordered memory operations will be used in this
section:

-  :math:`UL(a)` indicates an *unordered* *load* from a variable at
   address :math:`a`. :math:`a` could point to local or remote memory.

-  :math:`US(a)` indicates an *unordered* *store* to a variable at
   address :math:`a`. Again, :math:`a` could point to local or remote
   memory.

The *unordered* loads and stores :math:`UL(a)` and :math:`US(a)` respect
fences but not program order. As in
Section \ `31.1.2 <#memory_order>`__, unordered loads and stores are
ordered with SC atomics. That is, unordered loads and stores for a given
task are in total order :math:`<_m` respecting the following rules which
preserve the order of unordered loads and stores relative to SC atomic
operations:

-  If :math:`UL(a)<_pA_{sc}(b)` then :math:`UL(a)<_mA_{sc}(b)`

-  If :math:`US(a)<_pA_{sc}(b)` then :math:`US(a)<_mA_{sc}(b)`

-  If :math:`A_{sc}(a)<_pUL(b)` then :math:`A_{sc}(a)<_mUL(b)`

-  If :math:`A_{sc}(a)<_pUS(b)` then :math:`A_{sc}(a)<_mUS(b)`

Unordered loads and stores do not preserve sequential program behavior.

Unordered Memory Operations Examples
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Unordered operations should be thought of as happening in a way that
overlaps with the program task. Unordered operations started in
different program statements can happen in any order unless an SC atomic
operation orders them.

Since unordered operations started by a single task can happen in any
order, totally sequential programs can have a data race when using
unordered operations. This follows from our original definition of data
race.



.. code-block:: chapel

   var x: int = 0;
   unordered_store(x, 10);
   unordered_store(x, 20);
   writeln(x);

The value of *x* at the end of this program could be 0, 10, or 20. As a
result, programs using unordered loads and stores are not sequentially
consistent unless the program can guarantee that unordered operations
can never operate on the same memory at the same time when one of them
is a store. In particular, the following are safe:

-  Unordered stores to disjoint regions of memory.

-  Unordered loads from potentially overlapping regions of memory when
   no store could overlap with the loads.

-  Unordered loads and stores to the same memory location when these are
   always separated by an SC atomic operation.

Unordered loads and stores are available as a performance optimization.
For example, a program computing a permutation on an array might want to
move data between two arrays without requiring any ordering:



.. code-block:: chapel

   const n = 10;
   // P is a permutation on 1..n, in this case reversing its input
   var P = for i in 1..n by -1 do i;
   // A is an array to permute
   var A = for i in 1..n do i;
   // Compute, in B, the permutation applied to A
   var B:[1..n] int;

   for i in 1..n {
     unordered_store(B[P[i]], A[i]);
   }

.. _MCM_examples:

Examples
--------

   *Example*.

   In this example, a synchronization variable is used to (a) ensure
   that all writes to an array of unsynchronized variables are complete,
   (b) signal that fact to a second task, and (c) pass along the number
   of values that are valid for reading.

   The program 

   .. code-block:: chapel

      var A: [1..100] real;
      var done$: sync int;           // initially empty
      cobegin {
        { // Reader task
          const numToRead = done$;   // block until writes are complete
          for i in 1..numToRead do
            writeln("A[", i, "] = ", A[i]);
        }
        {  // Writer task
          const numToWrite = 14;     // an arbitrary number
          for i in 1..numToWrite do
            A[i] = i/10.0;
          done$ = numToWrite;        // fence writes to A and signal done
        }
      }

   produces the output 

   .. code-block:: printoutput

      A[1] = 0.1
      A[2] = 0.2
      A[3] = 0.3
      A[4] = 0.4
      A[5] = 0.5
      A[6] = 0.6
      A[7] = 0.7
      A[8] = 0.8
      A[9] = 0.9
      A[10] = 1.0
      A[11] = 1.1
      A[12] = 1.2
      A[13] = 1.3
      A[14] = 1.4

..

   *Example (syncSpinWait.chpl)*.

   One consequence of Chapel’s memory consistency model is that a task
   cannot spin-wait on a normal variable waiting for another task to
   write to that variable. The behavior of the following code is
   undefined:

   

   .. BLOCK-test-chapelpre

      if false { // }

   

   .. code-block:: chapel

      var x: int;
      cobegin with (ref x) {
        while x != 1 do ;  // INCORRECT spin wait
        x = 1;
      }

   

   .. BLOCK-test-chapelnoprint

      // {
      }

   In contrast, spinning on a synchronization variable has well-defined
   behavior: 

   .. code-block:: chapel

      var x$: sync int;
      cobegin {
        while x$.readXX() != 1 do ;  // spin wait
        x$.writeXF(1);
      }

   In this code, the first statement in the cobegin statement executes a
   loop until the variable is set to one. The second statement in the
   cobegin statement sets the variable to one. Neither of these
   statements block.

   *Example (atomicSpinWait.chpl)*.

   Atomic variables provide an alternative means to spin-wait. For
   example:

   

   .. code-block:: chapel

      var x: atomic int;
      cobegin {
        while x.read() != 1 do ;  // spin wait - monopolizes processor
        x.write(1);
      }

..

   *Example (atomicWaitFor.chpl)*.

   The main drawback of the above example is that it prevents the thread
   executing the spin wait from doing other useful work. Atomic
   variables include a waitFor method that will block the calling thread
   until a read of the atomic value matches a particular value. In
   contrast to the spin wait loop above, waitFor will allow other tasks
   to be scheduled. For example:

   

   .. code-block:: chapel

      var x: atomic int;
      cobegin {
        x.waitFor(1);
        x.write(1);
      }

   .. note::

      *Future:*

      Upon completion, Chapel’s atomic
      statement (:ref:`Atomic_Statement`) will serve as an
      additional means of correctly synchronizing between tasks.
