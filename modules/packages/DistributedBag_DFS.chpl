/*
  A highly parallel segmented multi-pool for depth-first search (DFS). Each node
  gets its own bag, and each bag is segmented into 'here.maxTaskPar' segments.
  To allow the best performance, segments are implemented as non-blocking split
  deques. This method consists in splitting segments into a public and a private
  portion using an atomic `split pointer`. Under this scheme, all processes push new
  tasks on the tail of the deque and pop tasks from the tail to get the next task
  to execute in LIFO order, while WS is done at the head in a FIFO manner. It allows
  lock-free local access to the private portion of the deque and copy-free transfer
  of work between the public and private portions. Work transfer is done by moving
  the split pointer in either directions using appropriate `release` or `acquire`
  operations. During work stealing phases, thieves synchronize using a lock and
  the local process only needs to take the lock when transferring work from a
  portion to the other of the deque.

  The data structure also employs its own work stealing algorithm to balance
  work across nodes. Depending on the state of the DistBag-DFS, three cases may
  occur:
    (1) BEST CASE (or SIMPLE CASE): the calling task get a node from its segment.
          This case fails if the private region of the segment is empty.
    (2) LOCAL STEAL: the calling task failed to get a node from its segment. In
          that case, it will try to steal another segment in its bag instance. To
          select the victims, we use an iterator that can be set to use the "random"
          or "ring" (or "round-robin") selection policy. This case fails if we fail
          to steal from all the eligible victims.
    (3) GLOBAL STEAL: the calling task failed to get a node from its segment, and
          also failed to steal work locally. In that case, it will try to steal
          another segment from another bag instance. Again, the "victim" iterator
          is used to selected both the target locale and the target segment. This
          case fails if we fail to steal from all the eligible victims.

  This data structure does not come without flaws; as WS is dynamic and triggered
  on demand, WS can still be performed in excess, which dramatically causes a
  performance drop. Furthermore, a severe imbalance across nodes, such as an unfair
  distribution of work to a single or small subset of nodes, may also causes an
  equally severe performance degradation. This data structure scales in terms of
  nodes, processors per node, and even work load. The larger the work load, the
  more data that gets stolen when WS, and better locality of elements among segments.
  As well, to achieve true parallelism, usage of a privatized instance is a requirement,
  as it avoids the overhead of remotely accessing class fields, bounding performance
  on communication.
*/

/* Implements a highly parallel segmented multi-pool for depth-first search (DFS).

  Summary
  _______

  A parallel-safe distributed multi-pool implementation that scales in terms of
  nodes, processors per node (PPN), and workload; the more PPN, the more segments
  we allocate to increase raw parallelism, and the larger the workload the better
  locality (see :const:`distributedBagInitialSegmentCap`). This data structure is
  locally ordered (ensuring DFS), employs its own work stealing algorithm to
  balance work across nodes, and provides a means to obtain a privatized instance
  of the data structure for maximized performance.

  .. note::

    This module is a work in progress and may change in future releases.

  Usage
  _____

  To use :record:`DistBag_DFS`, the initializer must be invoked explicitly to
  properly initialize the data structure. Using the default state without
  initializing will result in a halt.

  .. code-block:: chapel

    var bag = new DistBag_DFS(int, targetLocales=ourTargetLocales);

  While the bag is safe to use in a distributed manner, each node always operates
  on its privatized instance. This means that it is easy to add data in bulk,
  expecting it to be distributed, when in reality it is not; if another node needs
  data, it will steal work on-demand.

  .. code-block:: chapel

    bag.add(1, i);        // inserted in segment i
    bag.addBulk(2..N, j); // inserted in segment j
    bag.remove(k);        // removed from segment k

  Methods
  _______
*/

module DistributedBag_DFS
{
  public use Collection;

  use Random;
  use List;
  use Math;

  /*
    The phases of the 'remove' operation. This operation is composed of multiple
    phases, where they make a full pass searching for ideal conditions, then
    less-than-ideal conditions; this is required to ensure maximized parallelism
    at all times, and critical to good performance.
  */
  private param REMOVE_SIMPLE       = 1;
  private param REMOVE_LOCAL_STEAL  = 2;
  private param REMOVE_GLOBAL_STEAL = 3;
  private param PERFORMANCE_PATCH   = 4;

  /*
    The output phases of the 'remove' operation. They are used to indicate the
    final state of the operation: success, steal in progress, or fail.
  */
  private param REMOVE_SUCCESS   =  1;
  private param REMOVE_FAST_EXIT =  0;
  private param REMOVE_FAIL      = -1;

  /*
    The initial capacity of each segment. When a segment is full, we double its
    capacity.
  */
  config const distributedBagInitialSegmentCap: int = 1024;
  /*
    The maximum capacity of each segment. This is crucial to ensure memory usage
    does not grow out of control.
  */
  config const distributedBagMaxSegmentCap: int = 1024 * 1024;
  /*
    The minimum number of elements a segment must have to become eligible to be
    stolen from. This may be useful if some segments contain less elements than
    others and should not be stolen from.
  */
  config const distributedBagWorkStealingMinElts: int = 1;
  /*
    The maximum amount of work to steal from a segment. This should be set to a
    value, in megabytes, that determines the maximum amount of data that should
    be sent in bulk at once. The maximum number of elements is determined by:
    ``(distributedBagWorkStealingMemCap * 1024 * 1024) / sizeof(eltType)``.
    For example, if we are storing 8-byte integers and have a 1MB limit, we would
    have a maximum of 125,000 elements stolen at once.
  */
  /* config const distributedBagWorkStealingMemCap: real = 1.0; */
  /*
    To prevent stealing too many elements from another segment (hence creating an
    artificial load imbalance), if the segment has less than a certain threshold
    (see :const:`distributedBagWorkStealingMemCap`) but above another threshold
    (see :const:`distributedBagWorkStealingMinElts`), we steal a percentage of
    their elements, leaving them with majority of their elements. This way, the
    amount the other segment loses is proportional to how much it owns, ensuring
    a balance.
  */
  /* config const distributedBagWorkStealingRatio: real = 0.25; */

  /*
    Reference counter for DistributedBag_DFS.
  */
  @chpldoc.nodoc
  class DistributedBagRC
  {
    type eltType;
    var _pid: int;

    proc deinit()
    {
      coforall loc in Locales do on loc {
        delete chpl_getPrivatizedCopy(unmanaged DistributedBagImpl(eltType), _pid);
      }
    }
  }

  /*
    A parallel-safe distributed multi-pool implementation that scales in terms of
    nodes, processors per node (PPN), and workload; the more PPN, the more segments
    we allocate to increase raw parallelism, and the larger the workload the better
    locality (see :const:`distributedBagInitialSegmentCap`). This data structure
    is locally ordered (ensuring DFS), employs its own work-stealing algorithm to
    balance work across nodes, and provides a means to obtain a privatized instance
    of the data structure for maximized performance.
  */
  pragma "always RVF"
  record DistBag_DFS
  {
    /*
      The type of the elements contained in this DistBag_DFS.
    */
    type eltType;

    // This is unused, and merely for documentation purposes. See '_value'.
    /*
      The implementation of the Bag is forwarded. See :class:`DistributedBagImpl` for
      documentation.
    */
    var _impl: unmanaged DistributedBagImpl(eltType)?;

    // Privatized id
    @chpldoc.nodoc
    var _pid: int = -1;

    // Reference Counting
    @chpldoc.nodoc
    var _rc: shared DistributedBagRC(eltType);

    @chpldoc.nodoc
    proc init(type eltType, targetLocales = Locales)
    {
      this.eltType = eltType;
      this._pid = (new unmanaged DistributedBagImpl(eltType, targetLocales = targetLocales)).pid;
      this._rc = new shared DistributedBagRC(eltType, _pid = _pid);
    }

    @chpldoc.nodoc
    inline proc _value
    {
      if (_pid == -1) then halt("DistBag_DFS is uninitialized.");
      return chpl_getPrivatizedCopy(unmanaged DistributedBagImpl(eltType), _pid);
    }

    @chpldoc.nodoc
    proc readThis(f) throws {
      compilerError("Reading a DistBag_DFS is not supported");
    }

    // Write the contents of this DistBag_DFS to a channel.
    @chpldoc.nodoc
    proc writeThis(ch) throws {
      ch.write("[");
      var size = this.getSize();
      for (i, iteration) in zip(this, 0..<size) {
        ch.write(i);
        if (iteration < size-1) then ch.write(", ");
      }
      ch.write("]");
    }

    forwarding _value;
  } // end 'DistBag_DFS' record

  class DistributedBagImpl : CollectionImpl
  {
    @chpldoc.nodoc
    var targetLocDom: domain(1);

    /*
      The locales to allocate bags for and load balance across. ``targetLocDom``
      represents the corresponding range of locales.
    */
    var targetLocales: [targetLocDom] locale;

    @chpldoc.nodoc
    var pid: int = -1;

    // Node-local fields below. These fields are specific to the privatized bag
    // instance. To access them from another node, make sure you use
    // 'getPrivatizedThis'.
    @chpldoc.nodoc
    var bag: unmanaged Bag(eltType)?;

    /*
      Initialize an empty DistBag_DFS.
    */
    proc init(type eltType, targetLocales: [?targetLocDom] locale = Locales)
    {
      super.init(eltType);

      this.targetLocDom  = targetLocDom;
      this.targetLocales = targetLocales;

      this.complete();

      this.pid = _newPrivatizedClass(this);
      this.bag = new unmanaged Bag(eltType, this);
    }

    @chpldoc.nodoc
    proc init(other, pid, type eltType = other.eltType)
    {
      super.init(eltType);

      this.targetLocDom  = other.targetLocDom;
      this.targetLocales = other.targetLocales;
      this.pid           = pid;

      this.complete();

      this.bag = new unmanaged Bag(eltType, this);
    }

    @chpldoc.nodoc
    proc deinit()
    {
      delete bag;
    }

    @chpldoc.nodoc
    proc dsiPrivatize(pid)
    {
      return new unmanaged DistributedBagImpl(this, pid);
    }

    @chpldoc.nodoc
    proc dsiGetPrivatizeData()
    {
      return pid;
    }

    @chpldoc.nodoc
    inline proc getPrivatizedThis
    {
      return chpl_getPrivatizedCopy(this.type, pid);
    }

    @chpldoc.nodoc
    iter targetLocalesNotHere()
    {
      foreach loc in targetLocales {
        if (loc != here) then yield loc;
      }
    }

    /*
      Insert an element in segment ``taskId``. The ordering is guaranteed to be
      preserved.

      :arg elt: The element to insert.
      :type elt: `eltType`

      :arg taskId: The index of the segment into which the element is inserted.
      :type taskId: `int`

      :return: `true` if ``elt`` is successfully inserted in segment ``taskId``.
      :rtype: `bool`
    */
    proc add(elt: eltType, taskId: int): bool
    {
      return bag!.add(elt, taskId);
    }

    /*
      Insert elements in bulk in segment ``taskId``. If the bag instance rejects
      an element (e.g., when :const:`distributedBagMaxSegmentCap` is reached), we
      cease to offer more. We return the number of elements successfully inserted.

      :arg elts: The elements to insert.

      :arg taskId: The index of the segment into which the element is inserted.
      :type taskId: `int`

      :return: The number of elements successfully inserted in segment ``taskId``.
      :rtype: `int`
    */
    proc addBulk(elts, taskId: int): int
    {
      return bag!.addBulk(elts, taskId);
    }

    // TODO: detail WS mechanism and scenarios or hide it to the user?
    // TODO: how to better write :return: in that case (e.g., as a list)?
    /*
      Remove an element from segment ``taskId``. The order in which elements are
      removed is guaranteed to be the same order they have been inserted. If this
      bag instance is empty, it will attempt to steal elements from bags of other
      nodes.

      :arg taskId: The index of the segment from which the element is removed.
      :type taskId: `int`

      :return: 2-tuple containing the final status of the operation: success (=1), fast exit (=0) or fail (=-1), and the retrieved element, if any.
      :rtype: `(int, eltType)`
    */
    proc remove(taskId: int): (int, eltType)
    {
      return bag!.remove(taskId);
    }

    // TODO: implement 'removeBulk'.

    /*
      Obtain the number of elements held in this DistBag_DFS.

      :return: The current number of elements contained in this DistBag_DFS.
      :rtype: `int`

      .. warning::

        This method is best-effort and can be non-deterministic for concurrent
        updates across nodes, and may miss elements resulting from any concurrent
        insertion or removal operations.
    */
    override proc getSize(): int
    {
      var size: atomic int;
      coforall loc in targetLocales do on loc {
        var instance = getPrivatizedThis;
        forall taskId in 0..#here.maxTaskPar do
          size.add(instance.bag!.segments[taskId].nElts);
      }

      return size.read();
    }

    /*
      Perform a lookup to determine if the requested element exists in this
      DistBag_DFS.

      :arg elt: An element to search for.
      :type elt: `eltType`

      :return: `true` if this DistBag_DFS contains ``elt``.
      :rtype: `bool`

      .. warning::

        This method is best-effort and can be non-deterministic for concurrent
        updates across nodes, and may miss elements resulting from any concurrent
        insertion or removal operations.
    */
    override proc contains(elt: eltType): bool
    {
      var foundElt: atomic bool;
      forall elem in getPrivatizedThis {
        if (elem == elt) then foundElt.write(true);
      }

      return foundElt.read();
    }

    /*
      Clear this DistBag_DFS.

      .. warning::

        This method is best-effort and can be non-deterministic for concurrent
        updates across nodes, and may miss elements resulting from any concurrent
        insertion or removal operations.
    */
    override proc clear(): void
    {
      coforall loc in targetLocales do on loc {
        var instance = getPrivatizedThis;
        forall taskId in 0..#here.maxTaskPar {
          ref segment = instance.bag!.segments[taskId];

          segment.lock_block.readFE();

          delete segment.block;
          segment.block = new unmanaged Block(eltType, distributedBagInitialSegmentCap);
          segment.nElts_shared.write(0);
          segment.head.write(0);
          segment.split.write(0);
          segment.globalSteal.write(false);
          segment.split_request.write(false);
          segment.lock.writeXF(true);
          segment.lock_n.writeXF(true);
          segment.tail = 0;
          segment.o_split = 0;

          segment.lock_block.writeEF(true);
        }
        instance.bag!.globalStealInProgress.write(false);
      }
    }

    // TODO: is 'balance' needed?

    /*
      Iterate over the elements of this DistBag_DFS. To avoid holding onto locks,
      we take a snapshot approach, increasing memory consumption but also
      increasing parallelism. This allows other concurrent, even mutating,
      operations while iterating, but opens the possibility to iterating over
      duplicates or missing elements from concurrent operations.

      :yields: A reference to one of the elements contained in this DistBag_DFS.

      .. note::

        `zip` iteration is not yet supported with rectangular data structures.

      .. warning::

        Iteration takes a snapshot approach, and as such can easily result in a
        Out-Of-Memory issue. If the data structure is large, the user is doubly
        advised to use parallel iteration, for both performance and memory benefit.
    */
    override iter these(): eltType
    {
      for loc in targetLocales {
        for taskId in 0..#here.maxTaskPar {
          // The size of the snapshot is only known once we have the lock.
          var dom: domain(1) = {0..-1};
          var buffer: [dom] eltType;
          on loc {
            ref segment = getPrivatizedThis.bag!.segments[taskId];

              dom = {0..#segment.nElts};
              for i in dom {
                buffer[i] = segment.block.elts[segment.block.headId + i];
              }
          }
          // Process this chunk if we have one...
          foreach elt in buffer {
            yield elt;
          }
        }
      }
    }

    @chpldoc.nodoc
    iter these(param tag: iterKind) where tag == iterKind.leader
    {
      coforall loc in targetLocales do on loc {
        var instance = getPrivatizedThis;
        coforall taskId in 0..#here.maxTaskPar {
          ref segment = instance.bag!.segments[taskId];

          // Create a snapshot
          var block = segment.block;
          var bufferSize = segment.nElts;

          // Yield this chunk to be process
          yield (bufferSize, block.elts[block.headId..block.tailId]);
        }
      }
    }

    @chpldoc.nodoc
    iter these(param tag: iterKind, followThis) where tag == iterKind.follower
    {
      var (bufferSize, buffer) = followThis;
      foreach i in 0..#bufferSize {
        yield buffer[i];
      }
    }
  } // end 'DistributedBagImpl' class

  /*
    We maintain a multi-pool 'bag' per node. Each bag keeps a handle to its parent,
    which is required for work stealing.
  */
  @chpldoc.nodoc
  class Bag
  {
    type eltType;

    // A handle to our parent 'distributed' bag, which is needed for work stealing.
    var parentHandle: borrowed DistributedBagImpl(eltType);

    var segments: [0..#here.maxTaskPar] Segment(eltType);

    var globalStealInProgress: atomic bool = false;

    proc init(type eltType, parentHandle)
    {
      this.eltType = eltType;
      this.parentHandle = parentHandle;
      // KNOWN ISSUE: 'this.complete' produces an error when 'eltType' is a Chapel
      // array (see Github issue #19859).
    }

    proc deinit()
    {
      forall segment in segments do
        delete segment.block;
    }

    /*
      Insert an element in segment ``taskId``. The ordering is guaranteed to be
      preserved.
    */
    proc add(elt: eltType, const taskId: int): bool
    {
      return segments[taskId].addElement(elt);
    }

    /*
      Insert elements in bulk in segment ``taskId``. If the bag instance rejects
      an element (e.g., when :const:distributedBagMaxSegmentCap isreached), we
      cease to offer more. We return the number of elements successfully inserted.
    */
    proc addBulk(elts, const taskId: int): int
    {
      return segments[taskId].addElements(elts);
    }

    /*
      Iterate over the segments/locales eligible to be stolen from, according to
      the specified policy. By default, the random strategy is chosen and the
      calling thread/locale cannot be chosen. We can specify how many tries we
      want, by default, only 1 is performed.
    */
    iter victim(const N: int, const callerId: int, const policy: string = "rand",
      const tries: int = 1): int
    {
      var count: int;
      var limit: int = if (callerId == -1) then N else N-1;

      select policy {
        // In the 'ring' strategy, victims are selected in a round-robin fashion.
        when "ring" {
          var id = (callerId + 1) % N;

          while ((count < limit) && (count < tries)) {
            yield id;
            count += 1;
            id = (id + 1) % N;
          }
        }
        // In the 'rand' strategy, victims are randomly selected.
        when "rand" {
          var id: int;
          var victims: [0..#N] int = noinit;
          permutation(victims);

          while ((count < limit) && (count < tries)) {
            if (victims[id] != callerId) {
              yield victims[id];
              count += 1;
            }
            id += 1;
          }
        }
        otherwise halt("DistributedBag_DFS internal error: Unknown victim choice policy");
      }
    }

    /*
      Remove an element from segment ``taskId``. The order in which elements are
      removed is guaranteed to be the same order they have been inserted. If this
      bag instance is empty, it will attempt to steal elements from bags of other
      nodes.
    */
    proc remove(const taskId: int): (int, eltType)
    {
      var phase = REMOVE_SIMPLE;
      if (numLocales > 1) then phase = PERFORMANCE_PATCH;

      ref segment = segments[taskId];
      var default: eltType;

      while true {
        select phase {
          /*
            Without this patch, the WS mechanism is not able to perform well on
            distributed settings. This could be explained by some bottlenecks, as
            well as mix-up between priority levels of local and global steals.

            TODO: investigate this in order to remove the patch.
          */
          when PERFORMANCE_PATCH {
            phase = REMOVE_SIMPLE;
          }

          /*
            Try to remove an element in segment 'taskId'. This fails if its private
            region is empty.
          */
          when REMOVE_SIMPLE {
            if (segment.nElts_private > 0) {
              return (REMOVE_SUCCESS, segment.takeElement());
            }

            phase = REMOVE_LOCAL_STEAL;
          }

          /*
            Try a local work stealing. This fails if all the eligible victims
            cannot release elements, or fast exits if a global work stealing is
            in progress in the bag instance.
          */
          when REMOVE_LOCAL_STEAL {
            var splitreq: bool = false;

            if globalStealInProgress.read() then return (REMOVE_FAST_EXIT, default);

            for victimTaskId in victim(here.maxTaskPar, taskId, "rand", here.maxTaskPar) {
              ref targetSegment = segments[victimTaskId];

              if !targetSegment.globalSteal.read() {
                targetSegment.lock_block.readFE();
                if (distributedBagWorkStealingMinElts <= targetSegment.nElts_shared.read()) {
                  var (hasElt, elt): (bool, eltType) = targetSegment.stealElement();

                  if hasElt {
                    targetSegment.lock_block.writeEF(true);
                    return (REMOVE_SUCCESS, elt);
                  }
                }
                else if (targetSegment.nElts_private > 1) {
                  splitreq = true;
                  targetSegment.split_request.write(true);
                }
                targetSegment.lock_block.writeEF(true);
              }
            }

            if splitreq then return (REMOVE_FAST_EXIT, default);

            phase = REMOVE_GLOBAL_STEAL;
          }

          /*
            Try a global work stealing. This fails if all the eligible victims
            cannot release elements, or fast exits if a global work stealing is
            in progress in the bag instance.
          */
          when REMOVE_GLOBAL_STEAL {
            // fast exit for single-node execution
            if (numLocales == 1) then return (REMOVE_FAIL, default);

            // "Lock" the global steal operation
            if !globalStealInProgress.compareAndSwap(false, true) {
              return (REMOVE_FAST_EXIT, default);
            }

            const parentPid = parentHandle.pid;
            var stolenElts: list(eltType);

            for victimLocaleId in victim(numLocales, here.id, "rand", 1) { //numLocales-1) {
              on Locales[victimLocaleId] {
                var targetBag = chpl_getPrivatizedCopy(parentHandle.type, parentPid).bag;
                for victimTaskId in victim(here.maxTaskPar, -1, "rand", here.maxTaskPar) { //0..#here.maxTaskPar {
                  ref targetSegment = targetBag!.segments[victimTaskId];

                  targetSegment.globalSteal.write(true);

                  targetSegment.lock_block.readFE();
                  if (distributedBagWorkStealingMinElts < targetSegment.nElts_shared.read()) {
                    var (hasElt, elt): (bool, eltType) = targetSegment.stealElement();

                    if hasElt {
                      stolenElts.insert(0, elt);
                    }
                  }
                  else if (targetSegment.nElts_private > 1) {
                    targetSegment.split_request.write(true);
                  }

                  targetSegment.lock_block.writeEF(true);
                  targetSegment.globalSteal.write(false);
                }
              }
            }

            if (stolenElts.size == 0) {
              globalStealInProgress.write(false);
              return (REMOVE_FAIL, default);
            }
            else {
              segment.addElements(stolenElts);
              globalStealInProgress.write(false);
              return (REMOVE_SUCCESS, segment.takeElement());
            }
          }

          otherwise do halt("DistributedBag_DFS Internal Error: Invalid phase #", phase);
        }
        chpl_task_yield();
      }

      halt("DistributedBag_DFS Internal Error: DEADCODE.");
    }
  } // end 'Bag' class

  /*
    A bag segment is, in and of itself an unrolled linked list. We maintain one
    per core to ensure maximum parallelism.
  */
  @chpldoc.nodoc
  record Segment
  {
    type eltType;

    var globalSteal: atomic bool = false;

    var block: unmanaged Block(eltType);

    // private variables
    var o_split: int;
    var tail: int;

    // shared variables
    var split: atomic int;
    var head: atomic int;
    var split_request: atomic bool;
    var nElts_shared: atomic int; // number of elements in the shared space

    // locks (initially unlocked)
    var lock: sync bool = true;
    var lock_n: sync bool = true;
    var lock_block: sync bool = true;

    proc init(type eltType)
    {
      this.eltType = eltType;
      this.block = new unmanaged Block(eltType, distributedBagInitialSegmentCap);
    }

    /*
      Return the size of the private region of the segment only.
    */
    inline proc nElts_private
    {
      return tail - o_split;
    }

    /*
      Return the size of the segment.
    */
    inline proc nElts
    {
      return nElts_private + nElts_shared.read();
    }

    /*
      Check if the segment is empty.
    */
    inline proc isEmpty
    {
      lock_n.readFE();
      var n_shared = nElts_shared.read();
      var n_private = nElts_private;
      lock_n.writeEF(true);
      return (n_shared + n_private) == 0;
    }

    /*
      Insert an element in the segment.
    */
    inline proc addElement(elt: eltType): bool
    {
      // allocate a larger block with the double capacity.
      if block.isFull {
        if (block.cap == distributedBagMaxSegmentCap) then
          return false;
        lock_block.readFE();
        block.cap = min(distributedBagMaxSegmentCap, 2*block.cap);
        block.dom = {0..#block.cap};
        lock_block.writeEF(true);
      }

      // we add the element at the tail
      block.pushTail(elt);
      tail += 1;

      // if there is a split request...
      if split_request.read() then split_release();

      return true;
    }

    /*
      Insert elements in the segment. If the bag instance rejects an element (e.g.,
      when :const:distributedBagMaxSegmentCap is reached), we cease to offer more.
      We return the number of elements successfully inserted.
    */
    inline proc addElements(elts): int
    {
      const size = elts.size;
      var realSize = size;

      // allocate a larger block.
      if (block.tailId + size > block.cap) {
        const neededCap = block.cap*2**ceil(log2(block.tailId + size / block.cap:real)):int;
        if (neededCap >= distributedBagMaxSegmentCap) {
          realSize = distributedBagMaxSegmentCap - block.tailId;
          block.cap = distributedBagMaxSegmentCap;
        }
        lock_block.readFE();
        block.cap = neededCap;
        block.dom = {0..#block.cap};
        lock_block.writeEF(true);
      }

      // TODO: find a better way to do the following.
      var c = 0;
      for elt in elts {
        if (c >= realSize) then break;
        block.pushTail(elt);
        c += 1;
      }
      tail += realSize;

      // if there is a split request...
      if split_request.read() then split_release();

      return realSize;
    }

    /*
      Remove an element from the segment.
    */
    inline proc takeElement(): eltType
    {
      // if the private region is not empty...
      var elt = block.popTail();
      tail -= 1;

      // if there is a split request...
      if split_request.read() then split_release();

      return elt;
    }

    // TODO: implement 'takeElements'

    // TODO: implement 'transferElements'

    /*
      Perform simultaneously two 'compareAndSwap' operations. This ensures that
      both atomic variables are accessed at the same time.
    */
    inline proc simCAS(A: atomic int, B: atomic int, expA: int, expB: int,
      desA: int, desB: int): bool
    {
      var casA, casB: bool;
      lock.readFE(); // set locked (empty)
      casA = A.compareAndSwap(expA, desA);
      casB = B.compareAndSwap(expB, desB);
      if (casA && casB) {
        lock.writeEF(true); // set unlocked (full)
        return true;
      }
      else {
        if casA then A.write(expA);
        if casB then B.write(expB);
        lock.writeEF(true); // set unlocked (full)
        return false;
      }
      halt("DistributedBag_DFS Internal Error: DEADCODE");
    }

    /*
      Steal an element in that segment.
    */
    inline proc stealElement(): (bool, eltType)
    {
      var default: eltType;

      // if the shared region becomes empty due to a concurrent operation...
      if (nElts_shared.read() == 0) then return (false, default);

      lock.readFE(); // set locked (empty)
      var (h, s): (int, int) = (head.read(), split.read());
      lock.writeEF(true); // set unlocked (full)

      // if there are elements to steal...
      if (h < s) {
        // if we successfully moved the pointers...
        if simCAS(head, split, h, s, h+1, s) {
          lock_n.readFE();
          var elt = block.popHead();

          nElts_shared.sub(1);
          lock_n.writeEF(true);

          return (true, elt);
        }
        else {
          return (false, default);
        }
      }

      // set the split request, if not already set...
      if !split_request.read() then split_request.write(true);

      return (false, default);
    }

    /*
      Increase the shared region of the segment (and decrease the private one).
    */
    inline proc split_release(): void
    {
      // fast exit
      if (nElts_private <= 1) then return;

      // compute the new split position
      var new_split: int = ((o_split + tail + 1) / 2): int;
      lock.readFE(); // block until its full and set locked (empty)
      split.write(new_split);
      lock.writeEF(true); // set unlocked (full)

      // updates the counters
      lock_n.readFE();
      nElts_shared.add(new_split - o_split);
      lock_n.writeEF(true);

      o_split = new_split;

      // reset split_request
      split_request.write(false);
    }

    /*
      Decrease the shared region of the segment (and increase the private one).
    */
    inline proc split_reacquire(): bool
    {
      // fast exit
      if (nElts_shared.read() <= 1) then return false;

      lock.readFE(); // block until its full and set locked (empty)
      var (h, s): (int, int) = (head.read(), split.read()); // o_split ?
      lock.writeEF(true); // set unlocked (full)
      if (h != s) {
        var new_split: int = ((h + s) / 2): int;
        lock.readFE(); // block until its full and set locked (empty)
        split.write(new_split);
        lock.writeEF(true); // set unlocked (full)
        lock_n.readFE();
        nElts_shared.sub(new_split - o_split);
        lock_n.writeEF(true);
        o_split = new_split;
        // ADD FENCE
        atomicFence();
        h = head.read();
        if (h != s) {
          if (h > new_split) {
            new_split = ((h + s) / 2): int;
            lock.readFE(); // block until its full and set locked (empty)
            split.write(new_split);
            lock.writeEF(true); // set unlocked (full)
            lock_n.readFE();
            nElts_shared.sub(new_split - o_split);
            lock_n.writeEF(true);
            o_split = new_split;
          }
          return false;
        }
      }

      return true;
    }
  } // end 'Segment' record

  /*
    A segment block is a Chapel array that holds a buffer of memory. Each block
    size *should* be a power of two, as we extend the size of each full block by
    twice the size. It should be noted that the block itself is not parallel-safe,
    and access must be synchronized.
  */
  @chpldoc.nodoc
  class Block
  {
    type eltType;
    var dom: domain(1);
    var elts: [dom] eltType;
    var cap: int;    // capacity of the block
    var headId: int; // index of the head element
    var tailId: int; // index of the tail element

    inline proc isFull
    {
      return tailId == cap;
    }

    proc init(type eltType, capacity)
    {
      this.eltType = eltType;
      this.dom = {0..#capacity};
      this.cap = capacity;
    }

    inline proc pushTail(elt: eltType): void
    {
      elts[tailId] = elt;
      tailId += 1;
    }

    inline proc popTail(): eltType
    {
      tailId -= 1;
      return elts[tailId];
    }

    inline proc popHead(): eltType
    {
      var elt = elts[headId];
      headId += 1;
      return elt;
    }
  } // end 'Block' class
} // end module
