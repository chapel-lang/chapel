.. _Chapter-Task_Parallelism_and_Synchronization:

Task Parallelism and Synchronization
====================================

Chapel supports both task parallelism and data parallelism. This chapter
details task parallelism as follows:

-  :ref:`Task_parallelism` introduces tasks and task
   parallelism.

-  :ref:`Begin` describes the begin statement, an unstructured
   way to introduce concurrency into a program.

-  :ref:`Synchronization_Variables` describes synchronization
   variables, an unstructured mechanism for synchronizing tasks.

-  :ref:`Atomic_Variables` describes atomic variables, a
   mechanism for supporting atomic operations.

-  :ref:`Cobegin` describes the cobegin statement, a structured
   way to introduce concurrency into a program.

-  :ref:`Coforall` describes the coforall loop, another
   structured way to introduce concurrency into a program.

-  :ref:`Task_Intents` specifies how variables from outer scopes
   are handled within ``begin``, ``cobegin`` and ``coforall``
   statements.

-  :ref:`Sync_Statement` describes the sync statement, a
   structured way to control parallelism.

-  :ref:`Serial` describes the serial statement, a structured
   way to suppress parallelism.

-  :ref:`Atomic_Statement` describes the atomic statement, a
   construct to support atomic transactions.

.. _Task_parallelism:

Tasks and Task Parallelism
--------------------------

A Chapel *task* is a distinct context of execution that may be running
concurrently with other tasks. Chapel provides a simple construct, the
``begin`` statement, to create tasks, introducing concurrency into a
program in an unstructured way. In addition, Chapel introduces two type
qualifiers, ``sync`` and ``single``, for synchronization between tasks.

Chapel provides two constructs, the ``cobegin`` and ``coforall``
statements, to introduce concurrency in a more structured way. These
constructs create multiple tasks but do not continue until these tasks
have completed. In addition, Chapel provides two constructs, the
``sync`` and ``serial`` statements, to insert synchronization and
suppress parallelism. All four of these constructs can be implemented
through judicious uses of the unstructured task-parallel constructs
described in the previous paragraph.

Tasks are considered to be created when execution reaches the start of a
``begin``, ``cobegin``, or ``coforall`` statement. When the tasks are
actually executed depends on the Chapel implementation and run-time
execution state.

A task is represented as a call to a *task function*, whose body
contains the Chapel code for the task. Variables defined in outer scopes
are considered to be passed into a task function by default intent,
unless a different *task intent* is specified explicitly by a
``task-intent-clause``.

Accesses to the same variable from different tasks are subject to the
Memory Consistency Model
(:ref:`Chapter-Memory_Consistency_Model`). Such
accesses can result from aliasing due to ``ref`` argument intents or
task intents, among others.

.. _Begin:

The Begin Statement
-------------------

The begin statement creates a task to execute a statement. The syntax
for the begin statement is given by 

.. code-block:: syntax

   begin-statement:
     `begin' task-intent-clause[OPT] statement

Control continues concurrently with the statement following the begin
statement.

   *Example (beginUnordered.chpl)*.

   The code 

   .. code-block:: chapel

      begin writeln("output from spawned task");
      writeln("output from main task");

   

   .. BLOCK-test-chapelprediff

      #!/usr/bin/env sh
      testname=$1
      outfile=$2
      sort $outfile > $outfile.2
      mv $outfile.2 $outfile

   

   .. BLOCK-test-chapeloutput

      output from main task
      output from spawned task

   executes two ``writeln`` statements that output the strings to the
   terminal, but the ordering is purposely unspecified. There is no
   guarantee as to which statement will execute first. When the begin
   statement is executed, a new task is created that will execute the
   ``writeln`` statement within it. However, execution will continue
   immediately after task creation with the next statement.

A begin statement creates a single task function, whose body is the body
of the begin statement. The handling of the outer variables within the
task function and the role of ``task-intent-clause`` are defined in
:ref:`Task_Intents`.

Yield and return statements are not allowed in begin blocks. Break and
continue statements may not be used to exit a begin block.

.. _Synchronization_Variables:

Synchronization Variables
-------------------------

Synchronization variables have a logical state associated with the
value. The state of the variable is either *full* or *empty*. Normal
reads of a synchronization variable cannot proceed until the variable’s
state is full. Normal writes of a synchronization variable cannot
proceed until the variable’s state is empty.

Chapel supports two types of synchronization variables: sync and single.
Both types behave similarly, except that a single variable may only be
written once. Consequently, when a sync variable is read, its state
transitions to empty, whereas when a single variable is read, its state
does not change. When either type of synchronization variable is
written, its state transitions to full.

``sync`` and ``single`` are type qualifiers and precede the type of the
variable’s value in the declaration. Sync and single are supported for
all Chapel primitive types ( :ref:`Primitive_Types`) except
complex. They are also supported for enumerated types
( :ref:`Enumerated_Types`) and variables of class type
( :ref:`Class_Types`). For sync variables of class type, the
full/empty state applies to the reference to the class object, not to
its member fields.

   *Rationale*.

   It is only well-formed to apply full-empty semantics to types that
   have no more than a single logical value. Booleans, integers, real
   and imaginary numbers, enums, and class references all meet this
   criteria. Since it is possible to read/write the individual elements
   of a complex value, it’s not obvious how the full-empty semantics
   would interact with such operations. While one could argue that
   record types with a single field could also be included, the user can
   more directly express such cases by declaring the field itself to be
   of sync type.

If a task attempts to read or write a synchronization variable that is
not in the correct state, the task is suspended. When the variable
transitions to the correct state, the task is resumed. If there are
multiple tasks blocked waiting for the state transition, one is
non-deterministically selected to proceed and the others continue to
wait if it is a sync variable; all tasks are selected to proceed if it
is a single variable.

A synchronization variable is specified with a sync or single type given
by the following syntax: 

.. code-block:: syntax

   sync-type:
     `sync' type-expression

   single-type:
     `single' type-expression

If a synchronization variable declaration has an initialization
expression, then the variable is initially full, otherwise it is
initially empty.

   *Example (beginWithSyncVar.chpl)*.

   The code 

   .. code-block:: chapel

      class Tree {
        var isLeaf: bool;
        var left, right: unmanaged Tree?;
        var value: int;

        proc sum():int {
          if (isLeaf) then
             return value;

          var x$: sync int;
          begin x$ = left!.sum();
          var y = right!.sum();
          return x$ + y;
        }
      }

   

   .. BLOCK-test-chapelpost

      var tree: unmanaged Tree = new unmanaged Tree(false, new unmanaged Tree(false, new unmanaged Tree(true, nil, nil, 1),
                                                       new unmanaged Tree(true, nil, nil, 1), 1),
                                       new unmanaged Tree(false, new unmanaged Tree(true, nil, nil, 1),
                                                       new unmanaged Tree(true, nil, nil, 1), 1), 1);
      writeln(tree.sum());
      proc Tree.deinit() {
        if isLeaf then return;
        delete left;
        delete right;
      }
      delete tree;

   

   .. BLOCK-test-chapeloutput

      4

   the sync variable ``x$`` is assigned by an
   asynchronous task created with the begin statement. The task
   returning the sum waits on the reading of ``x$``
   until it has been assigned. By convention, synchronization variables
   end in ``$`` to provide a visual cue to the programmer indicating
   that the task may block.

..

   *Example (syncCounter.chpl)*.

   Sync variables are useful for tallying data from multiple tasks as
   well. If all updates to an initialized sync variable are via compound
   assignment operators (or equivalently, traditional assignments that
   read and write the variable once), the full/empty state of the sync
   variable guarantees that the reads and writes will be interleaved in
   a manner that makes the updates atomic. For example, the code:
   

   .. code-block:: chapel

      var count$: sync int = 0;
      cobegin {
        count$ += 1;
        count$ += 1;
        count$ += 1;
      }

   

   .. BLOCK-test-chapelpost

      writeln("count is: ", count$.readFF());

   

   .. BLOCK-test-chapeloutput

      count is: 3

   creates three tasks that increment ``count$``. If
   ``count$`` were not a sync variable, this code
   would be unsafe because two tasks could then read the same value
   before either had written its updated value, causing one of the
   increments to be lost.

   *Example (singleVar.chpl)*.

   The following code implements a simple split-phase barrier using a
   single variable. 

   .. BLOCK-test-chapelpre

      config const n = 44;
      proc work(i) {
        // do nothing
      }

   

   .. code-block:: chapel

      var count$: sync int = n;  // counter which also serves as a lock
      var release$: single bool; // barrier release

      forall t in 1..n do begin {
        work(t);
        var myc = count$;  // read the count, set state to empty
        if myc!=1 {
          write(".");
          count$ = myc-1;  // update the count, set state to full
          // we could also do some work here before blocking
          release$;
        } else {
          release$ = true;  // last one here, release everyone
          writeln("done");
        }
      }

   

   .. BLOCK-test-chapeloutput

      ...........................................done

   In each iteration of the forall loop after the work is completed, the
   task reads the ``count$`` variable, which is used
   to tally the number of tasks that have arrived. All tasks except the
   last task to arrive will block while trying to read the variable
   ``release$``. The last task to arrive will write
   to ``release$``, setting its state to full at
   which time all the other tasks can be unblocked and run.

If a formal argument with a default intent either has a synchronization
type or the formal is generic
(:ref:`Formal_Arguments_of_Generic_Type`) and the actual has a
synchronization type, the actual must be an lvalue and is passed by
reference. In these cases the formal itself is an lvalue, too. The
actual argument is not read or written during argument passing; its
state is not changed or waited on. The qualifier ``sync`` or ``single``
without the value type can be used to specify a generic formal argument
that requires a ``sync`` or ``single`` actual.

When the actual argument is a ``sync`` or ``single`` and the
corresponding formal has the actual’s base type or is implicitly
converted from that type, a normal read of the actual is performed when
the call is made, and the read value is passed to the formal.

.. _Functions_on_Synchronization_Variables:

Predefined Single and Sync Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following methods are defined for variables of sync and single type.



::

   proc (sync t).readFE(): t

Returns the value of the sync variable. This method blocks until the
sync variable is full. The state of the sync variable is set to empty
when this method completes. This method implements the normal read of a
``sync`` variable.



::

   proc (sync t).readFF(): t
   proc (single t).readFF(): t

Returns the value of the sync or single variable. This method blocks
until the sync or single variable is full. The state of the sync or
single variable remains full when this method completes. This method
implements the normal read of a ``single`` variable.



::

   proc (sync t).readXX(): t
   proc (single t).readXX(): t

Returns the value of the sync or single variable. This method is
non-blocking and the state of the sync or single variable is unchanged
when this method completes.



::

   proc (sync t).writeEF(v: t)
   proc (single t).writeEF(v: t)

Assigns ``v`` to the value of the sync or single variable. This method
blocks until the sync or single variable is empty. The state of the sync
or single variable is set to full when this method completes. This
method implements the normal write of a ``sync`` or ``single`` variable.



::

   proc (sync t).writeFF(v: t)

Assigns ``v`` to the value of the sync variable. This method blocks
until the sync variable is full. The state of the sync variable remains
full when this method completes.



::

   proc (sync t).writeXF(v: t)

Assigns ``v`` to the value of the sync variable. This method is
non-blocking and the state of the sync variable is set to full when this
method completes.



::

   proc (sync t).reset()

Assigns the default value of type ``t`` to the value of the sync
variable. This method is non-blocking and the state of the sync variable
is set to empty when this method completes.



::

   proc (sync t).isFull: bool
   proc (single t).isFull: bool

Returns ``true`` if the sync or single variable is full and ``false``
otherwise. This method is non-blocking and the state of the sync or
single variable is unchanged when this method completes.

Note that ``writeEF`` and ``readFE``/``readFF`` methods (for ``sync``
and ``single`` variables, respectively) are implicitly invoked for
normal writes and reads of synchronization variables.

   *Example (syncMethods.chpl)*.

   Given the following declarations 

   .. BLOCK-test-chapelpre

      { // }

   

   .. code-block:: chapel

      var x$: sync int;
      var y$: single int;
      var z: int;

   the code 

   .. code-block:: chapel

      x$ = 5;
      y$ = 6;
      z = x$ + y$;

   

   .. BLOCK-test-chapelnoprint

      writeln((x$.readXX(), y$.readFF(), z));
      // {
      }
      { // }
      var x$: sync int;
      var y$: single int;
      var z: int;

   is equivalent to 

   .. code-block:: chapel

      x$.writeEF(5);
      y$.writeEF(6);
      z = x$.readFE() + y$.readFF();

   

   .. BLOCK-test-chapelpost

      writeln((x$.readXX(), y$.readFF(), z));
      // {
      }

   

   .. BLOCK-test-chapeloutput

      (5, 6, 11)
      (5, 6, 11)

.. _Atomic_Variables:

Atomic Variables
----------------

Atomic variables are variables that support atomic operations. Chapel
currently supports atomic operations for bools, all supported sizes of
signed and unsigned integers, as well as all supported sizes of reals.

   *Rationale*.

   The choice of supported atomic variable types as well as the atomic
   operations was strongly influenced by the C11 standard.

Atomic is a type qualifier that precedes the variable’s type in the
declaration. Atomic operations are supported for bools, and all sizes of
ints, uints, and reals.

An atomic variable is specified with an atomic type given by the
following syntax:



.. code-block:: syntax

   atomic-type:
     `atomic' type-expression

.. _Functions_on_Atomic_Variables:

Predefined Atomic Methods
~~~~~~~~~~~~~~~~~~~~~~~~~

The following methods are defined for variables of atomic type. Note
that not all operations are supported for all atomic types. The
supported types are listed for each method.

Most of the predefined atomic methods accept an optional argument named
``order`` of type memoryOrder. The ``order`` argument is used to specify
the ordering constraints of atomic operations. The supported memoryOrder
values are:

-  memoryOrder.relaxed

-  memoryOrder.acquire

-  memoryOrder.release

-  memoryOrder.acqRel

-  memoryOrder.seqCst

Unless specified, the default for the memoryOrder parameter is
memoryOrder.seqCst.

   *Implementors’ note*.

   Not all architectures or implementations may support all memoryOrder
   values. In these cases, the implementation should default to a more
   conservative ordering than specified.



::

   proc (atomic T).read(param order:memoryOrder = memoryOrder.seqCst): T

Reads and returns the stored value. Defined for all atomic types.



::

   proc (atomic T).write(v: T, param order:memoryOrder = memoryOrder.seqCst)

Stores ``v`` as the new value. Defined for all atomic types.



::

   proc (atomic T).exchange(v: T, param order:memoryOrder = memoryOrder.seqCst): T

Stores ``v`` as the new value and returns the original value. Defined
for all atomic types.

::

   proc (atomic T).compareExchange(ref e: T, v: T, param order:memoryOrder = memoryOrder.seqCst): bool
   proc (atomic T).compareExchange(ref e: T, v: T, param failure:memoryOrder, param success:memoryOrder): bool
   proc (atomic T).compareExchangeWeak(ref e: T, v: T, param order:memoryOrder = memoryOrder.seqCst): bool
   proc (atomic T).compareExchangeWeak(ref e: T, v: T, param failure:memoryOrder, param success:memoryOrder): bool

Stores ``v`` as the new value, if and only if the original value is
equal to ``e``. Returns ``true`` if ``v`` was stored, otherwise
returns ``false`` and updates ``e`` to the old value.  The weak
version is allowed to spuriously fail, but when using
``compareExchange`` in a loop anyways, it can can offer better
performance on some platforms. Defined for all atomic types.



::

   proc (atomic T).compareAndSwap(e: T, v: T, param order:memoryOrder = memoryOrder.seqCst): bool

Stores ``v`` as the new value, if and only if the original value is
equal to ``e``. Returns ``true`` if ``v`` was stored, ``false``
otherwise. Defined for all atomic types.



::

   proc (atomic T).add(v: T, param order:memoryOrder = memoryOrder.seqCst)
   proc (atomic T).sub(v: T, param order:memoryOrder = memoryOrder.seqCst)
   proc (atomic T).or(v: T, param order:memoryOrder = memoryOrder.seqCst)
   proc (atomic T).and(v: T, param order:memoryOrder = memoryOrder.seqCst)
   proc (atomic T).xor(v: T, param order:memoryOrder = memoryOrder.seqCst)

Applies the appropriate operator (``+``, ``-``, ``|``, ``&``, ``^``) to
the original value and ``v`` and stores the result. All of the methods
are defined for integral atomic types. Only add and sub (``+``, ``-``)
are defined for ``real`` atomic types. None of the methods are defined
for the ``bool`` atomic type.

   *Future*.

   In the future we may overload certain operations such as ``+=`` to call
   the above methods automatically for atomic variables.



::

   proc (atomic T).fetchAdd(v: T, param order:memoryOrder = memoryOrder.seqCst): T
   proc (atomic T).fetchSub(v: T, param order:memoryOrder = memoryOrder.seqCst): T
   proc (atomic T).fetchOr(v: T, param order:memoryOrder = memoryOrder.seqCst): T
   proc (atomic T).fetchAnd(v: T, param order:memoryOrder = memoryOrder.seqCst): T
   proc (atomic T).fetchXor(v: T, param order:memoryOrder = memoryOrder.seqCst): T

Applies the appropriate operator (``+``, ``-``, ``|``, ``&``, ``^``) to
the original value and ``v``, stores the result, and returns the original
value. All of the methods are defined for integral atomic types. Only add
and sub (``+``, ``-``) are defined for ``real`` atomic types. None of the
methods are defined for the ``bool`` atomic type.



::

   proc (atomic bool).testAndSet(param order:memoryOrder = memoryOrder.seqCst): bool

Stores ``true`` as the new value and returns the old value. Equivalent
to ``exchange(true)``. Only defined for the ``bool`` atomic type.



::

   proc (atomic bool).clear(param order:memoryOrder = memoryOrder.seqCst)

Stores ``false`` as the new value. Equivalent to ``write(false)``. Only
defined for the ``bool`` atomic type.



::

   proc (atomic T).waitFor(v: T)

Waits until the stored value is equal to ``v``. The implementation may
yield the running task while waiting. Defined for all atomic types.

.. _Cobegin:

The Cobegin Statement
---------------------

The cobegin statement is used to introduce concurrency within a block.
The ``cobegin`` statement syntax is 

.. code-block:: syntax

   cobegin-statement:
     `cobegin' task-intent-clause[OPT] block-statement

A new task and a corresponding task function are created for each
statement in the ``block-statement``. Control continues when all of the
tasks have finished. The handling of the outer variables within each
task function and the role of ``task-intent-clause`` are defined in
:ref:`Task_Intents`.

Return statements are not allowed in cobegin blocks. Yield statement may
only be lexically enclosed in cobegin blocks in parallel
iterators (:ref:`Parallel_Iterators`). Break and continue
statements may not be used to exit a cobegin block.

   *Example (cobeginAndEquivalent.chpl)*.

   The cobegin statement 

   .. BLOCK-test-chapelpre

      var s1, s2: sync int;
      proc stmt1() { s1; }
      proc stmt2() { s2; s1 = 1; }
      proc stmt3() { s2 = 1; }

   

   .. code-block:: chapel

      cobegin {
        stmt1();
        stmt2();
        stmt3();
      }

   is equivalent to the following code that uses only begin statements
   and single variables to introduce concurrency and synchronize:
   

   .. code-block:: chapel

      var s1$, s2$, s3$: single bool;
      begin { stmt1(); s1$ = true; }
      begin { stmt2(); s2$ = true; }
      begin { stmt3(); s3$ = true; }
      s1$; s2$; s3$;

   Each begin statement is executed concurrently but control does not
   continue past the final line above until each of the single variables
   is written, thereby ensuring that each of the functions has finished.

.. _Coforall:

The Coforall Loop
-----------------

The coforall loop is a variant of the cobegin statement in loop form.
The syntax for the coforall loop is given by 

.. code-block:: syntax

   coforall-statement:
     `coforall' index-var-declaration `in' iteratable-expression task-intent-clause[OPT] `do' statement
     `coforall' index-var-declaration `in' iteratable-expression task-intent-clause[OPT] block-statement
     `coforall' iteratable-expression task-intent-clause[OPT] `do' statement
     `coforall' iteratable-expression task-intent-clause[OPT] block-statement

The ``coforall`` loop creates a separate task for each iteration of the
loop. Control continues with the statement following the ``coforall``
loop after all tasks corresponding to the iterations of the loop have
completed.

The single task function created for a ``coforall`` and invoked by each
task contains the loop body. The handling of the outer variables within
the task function and the role of ``task-intent-clause`` are defined in
:ref:`Task_Intents`.

Return statements are not allowed in coforall blocks. Yield statement
may only be lexically enclosed in coforall blocks in parallel
iterators (:ref:`Parallel_Iterators`). Break and continue
statements may not be used to exit a coforall block.

   *Example (coforallAndEquivalent.chpl)*.

   The coforall statement 

   .. BLOCK-test-chapelpre

      iter iterator() { for i in 1..3 do yield i; }
      proc body() { }

   

   .. code-block:: chapel

      coforall i in iterator() {
        body();
      }

   is equivalent to the following code that uses only begin statements
   and sync and single variables to introduce concurrency and
   synchronize: 

   .. code-block:: chapel

      var runningCount$: sync int = 1;
      var finished$: single bool;
      for i in iterator() {
        runningCount$ += 1;
        begin {
          body();
          var tmp = runningCount$;
          runningCount$ = tmp-1;
          if tmp == 1 then finished$ = true;
        }
      }
      var tmp = runningCount$;
      runningCount$ = tmp-1;
      if tmp == 1 then finished$ = true;
      finished$;

   Each call to ``body()`` executes concurrently because it is in a
   begin statement. The sync variable
   ``runningCount$`` is used to keep track of the
   number of executing tasks plus one for the main task. When this
   variable reaches zero, the single variable
   ``finished$`` is used to signal that all of the
   tasks have completed. Thus control does not continue past the last
   line until all of the tasks have completed.

.. _Task_Intents:

Task Intents
------------

If a variable is referenced within the lexical scope of a ``begin``,
``cobegin``, or ``coforall`` statement and is declared outside that
statement, it is subject to *task intents*. That is, it is considered to
be passed as an actual argument to the corresponding task function at
task creation time. All references to the variable within the task
function implicitly refer to a *shadow variable*, i.e. the task
function’s corresponding formal argument.

When the task construct is inside a method on a record and accesses a
field of ``this``, the field is treated as a regular variable. That is,
it is passed as an actual argument to the task function and all
references to the field within the task function implicitly refer to the
corresponding shadow variable.

Each formal argument of a task function has the default argument intent
by default. For variables of primitive and class types, this has the
effect of capturing the value of the variable at task creation time and
referencing that value instead of the original variable within the
lexical scope of the task construct.

A formal can be given another argument intent explicitly by listing it
with that intent in the optional ``task-intent-clause``. For example,
for variables of most types, the ``ref`` intent allows the task
construct to modify the corresponding original variable or to read its
updated value after concurrent modifications.

The syntax of the task intent clause is:



.. code-block:: syntax

   task-intent-clause:
     `with' ( task-intent-list )

   task-intent-list:
     task-intent-item
     task-intent-item, task-intent-list

   task-intent-item:
     formal-intent identifier
     task-private-var-decl

| where the following intents can be used as a ``formal-intent``:
  ``ref``, ``in``, ``const``, ``const in``, ``const ref``.
  ``task-private-var-decl`` is defined in
  :ref:`Task_Private_Variables`. In addition,
  ``task-intent-item`` may define a ``reduce`` intent. Reduce intents
  are described in the *Reduce Intents* technical note in the online
  documentation:
| https://chapel-lang.org/docs/technotes/reduceIntents.html

The implicit treatment of outer scope variables as the task function’s
formal arguments applies to both module level and local variables. It
applies to variable references within the lexical scope of a task
construct, but does not extend to its dynamic scope, i.e., to the
functions called from the task(s) but declared outside of the lexical
scope. The loop index variables of a ``coforall`` statement are not
subject to such treatment within that statement; however, they are
subject to such treatment within nested task constructs, if any.

   *Rationale*.

   The primary motivation for task intents is to avoid some races on
   scalar/record variables, which are possible when one task modifies a
   variable and another task reads it. Without task intents, for
   example, it would be easy to introduce and overlook a bug illustrated
   by this simplified example:

   

   .. code-block:: chapel

        {
          var i = 0;
          while i < 10 {
            begin {
              f(i);
            }
            i += 1;
          }
        }

   If all the tasks created by the ``begin`` statement start executing
   only after the ``while`` loop completes, and ``i`` within the
   ``begin`` is treated as a reference to the original ``i``, there will
   be ten tasks executing ``f(10)``. However, the user most likely
   intended to generate ten tasks executing ``f(0)``, ``f(1)``, ...,
   ``f(9)``. Task intents ensure that, regardless of the timing of task
   execution.

   Another motivation for task intents is that referring to a captured
   copy in a task is often more efficient than referring to the original
   variable. That’s because the copy is a local constant, e.g. it could
   be placed in a register when it fits. Without task intents,
   references to the original variable would need to be implemented
   using a pointer dereference. This is less efficient and can hinder
   optimizations in the surrounding code, for example loop-invariant
   code motion.

   Furthermore, in the above example the scope where ``i`` is declared
   may exit before all the ten tasks complete. Without task intents, the
   user would have to protect ``i`` to make sure its lexical scope
   doesn’t exit before the tasks referencing it complete.

   We decided to treat ``cobegin`` and ``coforall`` statements the same
   way as ``begin``. This is for consistency and to make the
   race-avoidance benefit available to more code.

   We decided to apply task intents to module level variables, in
   addition to local variables. Again, this is for consistency. One
   could view module level variables differently than local variables
   (e.g. a module level variable is “always available”), but we favored
   consistency over such an approach.

   We decided not to apply task intents to “closure” variables, i.e.,
   the variables in the dynamic scope of a task construct. This is to
   keep this feature manageable, so that all variables subject to task
   intents can be obtained by examining just the lexical scope of the
   task construct. In general, the set of closure variables can be hard
   to determine, unwieldy to implement and reason about, it is unclear
   what to do with extern functions, etc.

   We do not provide ``inout`` or ``out`` as task intents because they
   will necessarily create a data race in a ``cobegin`` or ``coforall``.
   ``type`` and ``param`` intents are not available either as they do
   not seem useful as task intents.

..

   *Future*.

   For a given intent, we would also like to provide a blanket clause,
   which would apply the intent to all variables. An example of syntax
   for a blanket ``ref`` intent would be ``ref *``.

.. _Sync_Statement:

The Sync Statement
------------------

The sync statement acts as a join of all dynamically encountered begins
from within a statement. The syntax for the sync statement is given by


.. code-block:: syntax

   sync-statement:
     `sync' statement
     `sync' block-statement

Return statements are not allowed in sync statement blocks. Yield
statement may only be lexically enclosed in sync statement blocks in
parallel iterators (:ref:`Parallel_Iterators`). Break and
continue statements may not be used to exit a sync statement block.

   *Example (syncStmt1.chpl)*.

   The sync statement can be used to wait for many dynamically created
   tasks. 

   .. BLOCK-test-chapelpre

      config const n = 9;
      proc work() {
        write(".");
      }

   

   .. code-block:: chapel

      sync for i in 1..n do begin work();

   

   .. BLOCK-test-chapelpost

      writeln("done");

   

   .. BLOCK-test-chapeloutput

      .........done

   The for loop is within a sync statement and thus the tasks created in
   each iteration of the loop must complete before the continuing past
   the sync statement.

..

   *Example (syncStmt2.chpl)*.

   The sync statement 

   .. BLOCK-test-chapelpre

      proc stmt1() { }
      proc stmt2() { }

   

   .. code-block:: chapel

      sync {
        begin stmt1();
        begin stmt2();
      }

   is similar to the following cobegin statement 

   .. code-block:: chapel

      cobegin {
        stmt1();
        stmt2();
      }

   except that if begin statements are dynamically encountered when
   ``stmt1()`` or ``stmt2()`` are executed, then the former code will
   wait for these begin statements to complete whereas the latter code
   will not.

.. _Serial:

The Serial Statement
--------------------

The ``serial`` statement can be used to dynamically disable parallelism.
The syntax is: 

.. code-block:: syntax

   serial-statement:
     `serial' expression[OPT] `do' statement
     `serial' expression[OPT] block-statement

where the optional ``expression`` evaluates to a boolean value. If the
expression is omitted, it is as though ’true’ were specified. Whatever
the expression’s value, the statement following it is evaluated. If the
expression is true, any dynamically encountered code that would normally
create new tasks within the statement is instead executed by the
original task without creating any new ones. In effect, execution is
serialized. If the expression is false, code within the statement will
generates task according to normal Chapel rules.

   *Example (serialStmt1.chpl)*.

   In the code 

   .. BLOCK-test-chapelpre

      config const lo = 9;
      config const hi = 23;
      proc work(i) {
        if __primitive("task_get_serial") then
          writeln("serial ", i);
      }

   

   .. code-block:: chapel

      proc f(i) {
        serial i<13 {
          cobegin {
            work(i);
            work(i);
          }
        }
      }

      for i in lo..hi {
        f(i);
      }

   

   .. BLOCK-test-chapeloutput

      serial 9
      serial 9
      serial 10
      serial 10
      serial 11
      serial 11
      serial 12
      serial 12

   the serial statement in procedure f() inhibits concurrent execution
   of work() if the variable i is less than 13.

..

   *Example (serialStmt2.chpl)*.

   The code 

   .. BLOCK-test-chapelpre

      proc stmt1() { write(1); }
      proc stmt2() { write(2); }
      proc stmt3() { write(3); }
      proc stmt4() { write(4); }
      var n = 3;

   

   .. code-block:: chapel

      serial {
        begin stmt1();
        cobegin {
          stmt2();
          stmt3();
        }
        coforall i in 1..n do stmt4();
      }

   is equivalent to 

   .. code-block:: chapel

      stmt1();
      {
        stmt2();
        stmt3();
      }
      for i in 1..n do stmt4();

   

   .. BLOCK-test-chapelpost

      writeln();

   

   .. BLOCK-test-chapeloutput

      123444123444

   because the expression evaluated to determine whether to serialize
   always evaluates to true.

.. _Atomic_Statement:

Atomic Statements
-----------------

   *Open issue*.

   This section describes a feature that is a work-in-progress. We seek
   feedback and collaboration in this area from the broader community.

The *atomic statement* is used to specify that a statement should appear
to execute atomically from other tasks’ point of view. In particular, no
task will see memory in a state that would reflect that the atomic
statement had begun executing but had not yet completed.

   *Open issue*.

   This definition of the atomic statement provides a notion of *strong
   atomicity* since the action will appear atomic to any task at any
   point in its execution. For performance reasons, it could be more
   practical to support *weak atomicity* in which the statement’s
   atomicity is only guaranteed with respect to other atomic statements.
   We may also pursue using atomic type qualifiers as a means of marking
   data that should be accessed atomically inside or outside an atomic
   section.

The syntax for the atomic statement is given by: 

.. code-block:: syntax

   atomic-statement:
     `atomic' statement

..

   *Example*.

   The following code illustrates the use of an atomic statement to
   perform an insertion into a doubly-linked list:

   

   .. BLOCK-test-chapelpre

      class Node {
        var data: int;
        var next: Node;
        var prev: Node;
      }
      var head = new Node(1);
      head.insertAfter(new Node(4));
      head.insertAfter(new Node(2));

      var obj = new Node(3);
      head.next.insertAfter(obj);

   

   .. code-block:: chapel

      proc Node.insertAfter(newNode: Node) {
        atomic {
          newNode.prev = this;
          newNode.next = this.next;
          if this.next then this.next.prev = newNode;
          this.next = newNode;
        }
      }

   

   .. BLOCK-test-chapelpost

      writeln(head.data, head.next.data, head.next.next.data, head.next.next.next.data);
      proc Node.remove() {
        if this.prev then this.prev = this.next;
        if this.next then this.next = this.prev;
        return this;
      }
      while (head) {
        next = head.next;
        delete head;
        head = next;
      }

   

   .. BLOCK-test-chapeloutput

      atomic.chpl:13: warning: atomic keyword is ignored (not implemented)
      1234

   The use of the atomic statement in this routine prevents other tasks
   from viewing the list in a partially-updated state in which the
   pointers might not be self-consistent.
