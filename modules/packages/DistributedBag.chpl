/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
  A highly parallel segmented multiset. Each node gets its own bag, and in each
  bag it is segmented into 'here.maxTaskPar' segments. Segments allow for actual
  parallelism while operating in that it enables us to manage 'best-case',
  'average-case', and 'worst-case' scenarios by making multiple passes over each
  segment. In the case where there is no oversubscription, the best-case will
  always be achieved (considering any other conditions are also met), while in
  the case of oversubscription or, for example a near empty bag, we fall into
  the 'average-case', etc. Examples of 'best-case' scenarios for a removal would
  be when a segment is unlocked and contains some elements we can drain, and the
  'average-case' would be to find any segment, unlocked or not, that contains
  elements we can drain, and so on.

  This data structure also employs its own load balancing algorithm, beginning
  with a best-effort round-robin algorithm such that each task begins searching
  at another segment (with the added benefit of reducing overall contention), in
  particular is useful for locally distributing insertion operations. Next it also
  employs a work-stealing algorithm that horizontally steals a ratio of elements
  across segments. A segment is considered 'horizontal' if it shares the same
  segment index within [0, here.maxTaskPar); those other segments likely are also
  benefited from local distribution from the round-robin algorithm discussed above.
  We also steal a ratio of elements, say 25%, because it leaves all other segments
  with 75% of their work; the more elements they have, the more we take, but the
  less they have, the less we steal; this also has the added benefit of reducing
  unnecessary work stealing between segments when the bag is nearly emptied. Furthermore,
  stealing horizontally ensures that all segments remain filled and that we still
  achieve parallelism across segments for removal operations. Lastly, we attempt
  to steal a maximum of `N / sizeof(eltType)`, where N is some size in megabytes
  (representing how much data can be sent in one network request), which keeps
  down excessive communication.

  This data structure does not come without flaws; as work stealing is dynamic
  and triggered on demand, work stealing can still be performed in excess, which
  dramatically causes a performance drop. Furthermore, a severe imbalance across
  nodes, such as an unfair distribution of work to a single or small subset of nodes,
  may also causes an equally severe performance degradation. To circumvent this,
  we allow the user to determine manually when to 'balance', which will evenly distribute all
  elements across nodes. This data structure scales in terms of nodes, processors per node,
  and even work load. The larger the work load, the more data that gets stolen when
  stealing work, and better locality of elements among segments. As well, to
  achieve true parallelism, usage of a privatized instance is a requirement, as
  it avoids the overhead of remotely accessing class fields, bounding performance
  on communication.
*/


/*

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
*/

module DistributedBag {

  public use Collection;
  use BlockDist;
  private use SysCTypes;

  /*
    Below are segment statuses, which is a way to make visible to outsiders the
    current ongoing operation. In segments, we use test-and-test-and-set spinlocks
    to allow polling for other conditions other than lock state. `sync` variables
    do not offer a means of acquiring in a non-blocking way, so this is needed to
    ensure better 'best-case' and 'average-case' phases.
  */
  private param STATUS_UNLOCKED : uint = 0;
  private param STATUS_ADD : uint = 1;
  private param STATUS_REMOVE : uint = 2;
  private param STATUS_LOOKUP : uint = 3;
  private param STATUS_BALANCE : uint = 4;

  /*
    Below are statuses specific to the work stealing algorithm. These allow the
    shepherd tasks to know when its sub-helpers finish and the end status of their
    work stealing attempt.
  */
  private param WS_INITIALIZED = -1;
  private param WS_FINISHED_WITH_NO_WORK = 0;
  private param WS_FINISHED_WITH_WORK = 1;

  /*
    The phases for operations. An operation is composed of multiple phases,
    where they make a full pass searching for ideal conditions, then less-than-ideal
    conditions; this is required to ensure maximized parallelism at all times, and
    critical to good performance, especially when a node is oversubscribed.
  */
  private param ADD_BEST_CASE = 0;
  private param ADD_AVERAGE_CASE = 1;
  private param REMOVE_BEST_CASE = 2;
  private param REMOVE_AVERAGE_CASE = 3;
  private param REMOVE_WORST_CASE = 4;

  /*
    The initial amount of elements in an unroll block. Each successive unroll block
    is double the size of it's predecessor, allowing for better locality for when
    there are larger numbers of elements. The better the locality, the better raw
    performance and easier it is to redistribute work.
  */
  config const distributedBagInitialBlockSize = 1024;
  /*
    To prevent stealing too many elements (horizontally) from another node's segment
    (hence creating an artificial load imbalance), if the other node's segment has
    less than a certain threshold (see :const:`distributedBagWorkStealingMemCap`) but above
    another threshold (see :const:`distributedBagWorkStealingMinElems`), we steal a percentage of their
    elements, leaving them with majority of their elements. This way, the amount the
    other segment loses is proportional to how much it owns, ensuring a balance.
  */
  config const distributedBagWorkStealingRatio = 0.25;
  /*
    The maximum amount of work to steal from a horizontal node's segment. This
    should be set to a value, in megabytes, that determines the maximum amount of
    data that should be sent in bulk at once. The maximum number of elements is
    determined by: (:const:`distributedBagWorkStealingMemCap` * 1024 * 1024) / sizeof(``eltType``).
    For example, if we are storing 8-byte integers and have a 1MB limit, we would
    have a maximum of 125,000 elements stolen at once.
  */
  config const distributedBagWorkStealingMemCap : real = 1.0;
  /*
    The minimum number of elements a horizontal segment must have to become eligible
    to be stolen from. This may be useful if some segments produce less elements than
    others and should not be stolen from.
  */
  config const distributedBagWorkStealingMinElems = 1;
  /*
    The maximum amount of elements in an unroll block. This is crucial to ensure memory
    usage does not rapidly grow out of control.
  */
  config const distributedBagMaxBlockSize = 1024 * 1024;

  /*
    Reference counter for DistributedBag
  */
  pragma "no doc"
  class DistributedBagRC {
    type eltType;
    var _pid : int;

    proc deinit() {
      coforall loc in Locales do on loc {
          delete chpl_getPrivatizedCopy(unmanaged DistributedBagImpl(eltType), _pid);
      }
    }
  }

  /*
    A parallel-safe distributed multiset implementation that scales in terms of
    nodes, processors per node (PPN), and workload; The more PPN, the more segments
    we allocate to increase raw parallelism, and the larger the workload the better
    locality (see :const:`distributedBagInitialBlockSize`). This data structure is unordered and employs
    its own work-stealing algorithm, and provides a means to obtain a privatized instance of
    the data structure for maximized performance.
  */
  record DistBag {
    type eltType;

    /*
      The implementation of the Bag is forwarded. See :class:`DistributedBagImpl` for
      documentation.
    */
    // This is unused, and merely for documentation purposes. See '_value'.
    var _impl : unmanaged DistributedBagImpl(eltType)?;

    // Privatized id...
    pragma "no doc"
    var _pid : int = -1;

    // Reference Counting...
    pragma "no doc"
    var _rc : shared DistributedBagRC(eltType);

    pragma "no doc"
    proc init(type eltType, targetLocales = Locales) {
      this.eltType = eltType;
      this._pid = (new unmanaged DistributedBagImpl(eltType, targetLocales = targetLocales)).pid;
      this._rc = new shared DistributedBagRC(eltType, _pid = _pid);
    }

    pragma "no doc"
    inline proc _value {
      if _pid == -1 {
        halt("DistBag is uninitialized...");
      }
      return chpl_getPrivatizedCopy(unmanaged DistributedBagImpl(eltType), _pid);
    }

    forwarding _value;
  }

  class DistributedBagImpl : CollectionImpl {
    pragma "no doc"
    var targetLocDom : domain(1);
    /*
      The locales to allocate bags for and load balance across.
    */
    var targetLocales : [targetLocDom] locale;
    pragma "no doc"
    var pid : int = -1;

    // Node-local fields below. These fields are specific to the privatized instance.
    // To access them from another node, make sure you use 'getPrivatizedThis'
    pragma "no doc"
    var bag : unmanaged Bag(eltType)?;

    proc init(type eltType, targetLocales : [?targetLocDom] locale = Locales) {
      super.init(eltType);

      this.targetLocDom  = targetLocDom;
      this.targetLocales = targetLocales;

      complete();

      this.pid           = _newPrivatizedClass(this);
      this.bag           = new unmanaged Bag(eltType, this);
    }

    pragma "no doc"
    proc init(other, pid, type eltType = other.eltType) {
      super.init(eltType);

      this.targetLocDom  = other.targetLocDom;
      this.targetLocales = other.targetLocales;
      this.pid           = pid;

      complete();

      this.bag           = new unmanaged Bag(eltType, this);
    }

    pragma "no doc"
    proc deinit() {
      delete bag;
    }

    pragma "no doc"
    proc dsiPrivatize(pid) {
      return new unmanaged DistributedBagImpl(this, pid);
    }

    pragma "no doc"
    proc dsiGetPrivatizeData() {
      return pid;
    }

    pragma "no doc"
    inline proc getPrivatizedThis {
      return chpl_getPrivatizedCopy(this.type, pid);
    }

    pragma "no doc"
    iter targetLocalesNotHere() {
      for loc in targetLocales {
        if loc != here {
          yield loc;
        }
      }
    }

    /*
      Insert an element to this node's bag. The ordering is not guaranteed to be
      preserved.
    */
    override proc add(elt : eltType) : bool {
      return bag!.add(elt);
    }

    /*
      Remove an element from this node's bag. The order in which elements are removed
      are not guaranteed to be the same order it has been inserted. If this node's
      bag is empty, it will attempt to steal elements from bags of other nodes.
    */
    override proc remove() : (bool, eltType) {
      return bag!.remove();
    }

    /*
      Obtain the number of elements held in all bags across all nodes. This method
      is best-effort and can be non-deterministic for concurrent updates across nodes,
      and may miss elements or even count duplicates resulting from any concurrent
      insertion or removal operations.
    */
    override proc getSize() : int {
      var sz : atomic int;
      coforall loc in targetLocales do on loc {
        var instance = getPrivatizedThis;
        forall segmentIdx in 0..#here.maxTaskPar {
          sz.add(instance.bag!.segments[segmentIdx].nElems.read() : int);
        }
      }

      return sz.read();
    }

    /*
      Performs a lookup to determine if the requested element exists in this bag.
      This method is best-effort and can be non-deterministic for concurrent
      updates across nodes, and may miss elements resulting from any concurrent
      insertion or removal operations.
    */
    override proc contains(elt : eltType) : bool {
      var foundElt : atomic bool;
      forall elem in getPrivatizedThis {
        if elem == elt {
          foundElt.write(true);
        }
      }
      return foundElt.read();
    }

    /*
      Clear all bags across all nodes in a best-effort approach. Elements added or
      moved around from concurrent additions or removals may be missed while clearing.
    */
    proc clear() {
      var localThis = getPrivatizedThis;
      coforall loc in localThis.targetLocales do on loc {
        var instance = getPrivatizedThis;
        forall segmentIdx in 0 .. #here.maxTaskPar {
          ref segment = instance.bag.segments[segmentIdx];
          if segment.acquireIfNonEmpty(STATUS_REMOVE) {
            var block = segment.headBlock;
            while block != nil {
              var tmp = block;
              block = block.next;
              delete tmp;
            }

            segment.headBlock = nil;
            segment.tailBlock = nil;
            segment.nElems.write(0);
            segment.releaseStatus();
          }
        }
      }
    }

    /*
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
    */
    proc balance() {
      var localThis = getPrivatizedThis;
      // Phase 1: Acquire all locks from from first node and segment to last
      // node and segment (our global locking order...)
      for loc in localThis.targetLocales do on loc {
        var instance = getPrivatizedThis;
        for segmentIdx in 0 .. #here.maxTaskPar {
          ref segment = instance.bag!.segments[segmentIdx];
          segment.acquire(STATUS_BALANCE);
        }
      }

      // Phase 2: Concurrently redistribute elements from segments which contain
      // more than the computed average.
      coforall segmentIdx in 0 .. #here.maxTaskPar {
        var nSegmentElems : [0..#localThis.targetLocales.size] int;
        var locIdx = 0;
        for loc in localThis.targetLocales do on loc {
          var nElems = getPrivatizedThis.bag!.segments[segmentIdx].nElems.read() : int;
          nSegmentElems[locIdx] = nElems;
          locIdx += 1;
        }

        // Find the average and the excess. The excess is calculated as the amount
        // of elements a segment has over the average, which is used to calculate
        // the buffer size for each segment.
        var total = (+ reduce nSegmentElems);
        var avg = total / locIdx;
        var excess : int;
        for nElems in nSegmentElems {
          if nElems > avg {
            excess += nElems - avg;
          }
        }

        // Allocate buffer, which holds the 'excess' elements for redistribution.
        // Then fill it.
        var buffer = c_malloc(eltType, excess);
        var bufferOffset = 0;
        for loc in localThis.targetLocales do on loc {
          var average = avg;
          ref segment = getPrivatizedThis.bag!.segments[segmentIdx];
          var nElems = segment.nElems.read() : int;
          if nElems > average {
            var nTransfer = nElems - average;
            var tmpBuffer = buffer + bufferOffset;
            segment.transferElements(tmpBuffer, nTransfer, buffer.locale.id);
            bufferOffset += nTransfer;
          }
        }

        // With the excess elements, redistribute it...
        bufferOffset = 0;
        for loc in localThis.targetLocales do on loc {
          var average = avg;
          ref segment = getPrivatizedThis.bag!.segments[segmentIdx];
          var nElems = segment.nElems.read() : int;
          if average > nElems {
            var give = average - nElems;
            var arr : [1..give] eltType;
            on bufferOffset {
              var tmpBuffer = buffer;
              for i in 1 .. give {
                arr[i] = tmpBuffer[bufferOffset];
                bufferOffset += 1;
              }
            }
            for i in 1 .. give {
              segment.addElements(arr[i]);
            }
          }
        }

        // Lastly, if there are items left over, just add them to our locale's segment.
        if excess > bufferOffset {
          ref segment = localThis.bag!.segments[segmentIdx];
          var nLeftOvers = excess - bufferOffset;
          var tmpBuffer = buffer + bufferOffset;
          segment.addElementsPtr(tmpBuffer, nLeftOvers, buffer.locale.id);
        }

        c_free(buffer);
      }

      // Phase 3: Release all locks from first node and segment to last node and segment.
      for loc in localThis.targetLocales do on loc {
        var instance = getPrivatizedThis;
        for segmentIdx in 0 .. #here.maxTaskPar {
          ref segment = instance.bag!.segments[segmentIdx];
          segment.releaseStatus();
        }
      }
    }

    /*
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

    */
    override iter these() : eltType {
      for loc in targetLocales {
        for segmentIdx in 0..#here.maxTaskPar {
          // The size of the snapshot is only known once we have the lock.
          var dom : domain(1) = {0..-1};
          var buffer : [dom] eltType;

          on loc {
            ref segment = getPrivatizedThis.bag!.segments[segmentIdx];

            if segment.acquireIfNonEmpty(STATUS_LOOKUP) {
              dom = {0..#segment.nElems.read() : int};
              var block = segment.headBlock;
              var idx = 0;
              while block != nil {
                for i in 0 .. #block!.size {
                    buffer[idx] = block!.elems[i];
                    idx += 1;
                }
                block = block!.next;
              }
              segment.releaseStatus();
            }
          }

          // Process this chunk if we have one...
          for elem in buffer {
            yield elem;
          }
        }
      }
    }

    iter these(param tag : iterKind) where tag == iterKind.leader {
      coforall loc in targetLocales do on loc {
        var instance = getPrivatizedThis;
        coforall segmentIdx in 0 .. #here.maxTaskPar {
          ref segment = instance.bag!.segments[segmentIdx];

          if segment.acquireIfNonEmpty(STATUS_LOOKUP) {
            // Create a snapshot...
            var block = segment.headBlock;
            var bufferSz = segment.nElems.read() : int;
            var buffer = c_malloc(eltType, bufferSz);
            var bufferOffset = 0;

            while block != nil {
              if bufferOffset + block!.size > bufferSz {
                halt("DistributedBag Internal Error: Snapshot attempt with bufferSz(", bufferSz, ") with offset bufferOffset(", bufferOffset + block!.size, ")");
              }
              __primitive("chpl_comm_array_put", block!.elems[0], here.id, buffer[bufferOffset], block!.size);
              bufferOffset += block!.size;
              block = block!.next;
            }

            // Yield this chunk to be process...
            segment.releaseStatus();
            yield (bufferSz, buffer);
            c_free(buffer);
          }
        }
      }
    }

    iter these(param tag : iterKind, followThis) where tag == iterKind.follower {
      var (bufferSz, buffer) = followThis;
      for i in 0 .. #bufferSz {
        yield buffer[i];
      }
    }
  }

  /*
    A segment block is an unrolled linked list node that holds a contiguous buffer
    of memory. Each segment block size *should* be a power of two, as we increase the
    size of each subsequent unroll block by twice the size. This is so that stealing
    work is faster in that majority of elements are confined to one area.

    It should be noted that the block itself is not parallel-safe, and access must be
    synchronized.
  */
  pragma "no doc"
  class BagSegmentBlock {
    type eltType;

    // Contiguous memory containing all elements
    var elems :  c_ptr(eltType);
    var next : unmanaged BagSegmentBlock(eltType)?;

    // The capacity of this block.
    var cap : int;
    // The number of occupied elements in this block.
    var size : int;

    inline proc isEmpty {
      return size == 0;
    }

    inline proc isFull {
      return size == cap;
    }

    inline proc push(elt : eltType) {
      if elems == nil {
        halt("DistributedBag Internal Error: 'elems' is nil");
      }

      if isFull {
        halt("DistributedBag Internal Error: SegmentBlock is Full");
      }

      elems[size] = elt;
      size = size + 1;
    }

    inline proc pop() : eltType {
      if elems == nil {
        halt("DistributedBag Internal Error: 'elems' is nil");
      }

      if isEmpty {
        halt("DistributedBag Internal Error: SegmentBlock is Empty");
      }

      size = size - 1;
      var elt = elems[size];
      return elt;
    }

    proc init(type eltType, capacity) {
      this.eltType = eltType;
      if capacity == 0 {
        halt("DistributedBag Internal Error: Capacity is 0...");
      }

      this.elems = c_malloc(eltType, capacity);
      this.cap = capacity;
    }

    proc init(type eltType, ptr, capacity) {
      this.eltType = eltType;
      this.elems = ptr;
      this.cap = capacity;
      this.size = cap;
    }

    proc deinit() {
      c_free(elems);
    }
  }

  /*
    A segment is, in and of itself an unrolled linked list. We maintain one per core
    to ensure maximum parallelism.
  */
  pragma "no doc"
  record BagSegment {
    type eltType;

    // Used as a test-and-test-and-set spinlock.
    var status : atomic uint;

    var headBlock : unmanaged BagSegmentBlock(eltType)?;
    var tailBlock : unmanaged BagSegmentBlock(eltType)?;

    var nElems : atomic uint;

    inline proc isEmpty {
      return nElems.read() == 0;
    }

    inline proc acquireWithStatus(newStatus) {
      return status.compareAndSwap(STATUS_UNLOCKED, newStatus);
    }

    // Set status with a test-and-test-and-set loop...
    inline proc acquire(newStatus) {
      while true {
        if currentStatus == STATUS_UNLOCKED && acquireWithStatus(newStatus) {
          break;
        }

        chpl_task_yield();
      }
    }

    // Set status with a test-and-test-and-set loop, but only while it is not empty...
    inline proc acquireIfNonEmpty(newStatus) {
      while !isEmpty {
        if currentStatus == STATUS_UNLOCKED && acquireWithStatus(newStatus) {
          if isEmpty {
            releaseStatus();
            return false;
          } else {
            return true;
          }
        }

        chpl_task_yield();
      }

      return false;
    }

    inline proc isUnlocked {
      return status.read() == STATUS_UNLOCKED;
    }

    inline proc currentStatus {
      return status.read();
    }

    inline proc releaseStatus() {
      status.write(STATUS_UNLOCKED);
    }

    inline proc transferElements(destPtr, n, locId = here.id) {
      var destOffset = 0;
      var srcOffset = 0;
      while destOffset < n {
        if headBlock == nil || isEmpty {
          halt(here, ": DistributedBag Internal Error: Attempted transfer ", n, " elements to ", locId, " but failed... destOffset=", destOffset);
        }

        var len = headBlock!.size;
        var need = n - destOffset;
        // If the amount in this block is greater than what is left to transfer, we
        // cannot begin transferring at the beginning, so we set our offset from the end.
        if len > need {
          srcOffset = len - need;
          headBlock!.size = srcOffset;
          __primitive("chpl_comm_array_put", headBlock!.elems[srcOffset], locId, destPtr[destOffset], need);
          destOffset = destOffset + need;
        } else {
          srcOffset = 0;
          headBlock!.size = 0;
          __primitive("chpl_comm_array_put", headBlock!.elems[srcOffset], locId, destPtr[destOffset], len);
          destOffset = destOffset + len;
        }

        // Fix list if we consumed last one...
        if headBlock!.isEmpty {
          var tmp = headBlock;
          headBlock = headBlock!.next;
          delete tmp;

          if headBlock == nil then tailBlock = nil;
        }
      }

      nElems.sub(n : uint);
    }

    proc addElementsPtr(ptr, n, locId = here.id) {
      var offset = 0;
      while offset < n {
        var block = tailBlock;
        // Empty? Create a new one of initial size
        if block == nil then {
          tailBlock = new unmanaged BagSegmentBlock(eltType, distributedBagInitialBlockSize);
          headBlock = tailBlock;
          block = tailBlock;
        }

        // Full? Create a new one double the previous size
        if block!.isFull {
          block!.next = new unmanaged BagSegmentBlock(eltType, min(distributedBagMaxBlockSize, block!.cap * 2));
          tailBlock = block!.next;
          block = block!.next;
        }

        var nLeft = n - offset;
        var nSpace = block!.cap - block!.size;
        var nFill = min(nLeft, nSpace);
        __primitive("chpl_comm_array_get", block!.elems[block!.size], locId, ptr[offset], nFill);
        block!.size = block!.size + nFill;
        offset = offset + nFill;
      }

      nElems.add(n : uint);
    }

    inline proc takeElements(n) {
      var iterations = n : int;
      var arr : [{0..#n : int}] eltType;
      var arrIdx = 0;

      for 1 .. n : int{
        if isEmpty {
          halt("DistributedBag Internal Error: Attempted to take ", n, " elements when insufficient");
        }

        if headBlock!.isEmpty {
          halt("DistributedBag Internal Error: Iterating over ", n, " elements with headBlock empty but nElems is ", nElems.read());
        }

        arr[arrIdx] = headBlock.pop();
        arrIdx = arrIdx + 1;
        nElems.sub(1);

        // Fix list if we consumed last one...
        if headBlock!.isEmpty {
          var tmp = headBlock;
          headBlock = headBlock!.next;
          delete tmp;

          if headBlock == nil then tailBlock = nil;
        }
      }

      return arr;
    }

    inline proc takeElement() {
      if isEmpty {
        var default: eltType;
        return (false, default);
      }

      if headBlock!.isEmpty {
        halt("DistributedBag Internal Error: Iterating over 1 element with headBlock empty but nElems is ", nElems.read());
      }

      var elem = headBlock!.pop();
      nElems.sub(1);

      // Fix list if we consumed last one...
      if headBlock!.isEmpty {
        var tmp = headBlock;
        headBlock = headBlock!.next;
        delete tmp;

        if headBlock == nil then tailBlock = nil;
      }

      return (true, elem);
    }

    inline proc addElements(elt : eltType) {
      var block = tailBlock;

      // Empty? Create a new one of initial size
      if block == nil then {
        tailBlock = new unmanaged BagSegmentBlock(eltType, distributedBagInitialBlockSize);
        headBlock = tailBlock;
        block = tailBlock;
      }

      // Full? Create a new one double the previous size
      if block!.isFull {
        block!.next = new unmanaged BagSegmentBlock(eltType, min(distributedBagMaxBlockSize, block!.cap * 2));
        tailBlock = block!.next;
        block = block!.next;
      }

      block!.push(elt);
      nElems.add(1);
    }

    inline proc addElements(elts) {
      for elt in elts do addElements(elt);
    }
  }

  /*
    We maintain a multiset 'bag' per node. Each bag keeps a handle to it's parent,
    which is required for work stealing.
  */
  pragma "no doc"
  class Bag {
    type eltType;

    // A handle to our parent 'distributed' bag, which is needed for work stealing.
    var parentHandle : borrowed DistributedBagImpl(eltType);

    /*
      Helps evenly distribute and balance placement of elements in a best-effort
      round-robin approach. In the case where we have parallel enqueues or dequeues,
      they are less likely overlap with each other. Furthermore, it increases our
      chance to find our 'ideal' segment.
    */
    var startIdxEnq : atomic uint;
    var startIdxDeq : atomic uint;

    /*
      If a task makes 2 complete passes (1 best-case, 1 average-case) and has not
      found enough items, then it may attempt to balance the load distribution.
      Furthermore, if a task is waiting on a load balance, it may piggyback on the
      result.
    */
    var loadBalanceInProgress : atomic bool;
    var loadBalanceResult : atomic bool;



    var maxParallelSegmentSpace = {0 .. #here.maxTaskPar};
    var segments : [maxParallelSegmentSpace] BagSegment(eltType);

    inline proc nextStartIdxEnq {
      return (startIdxEnq.fetchAdd(1) % here.maxTaskPar : uint) : int;
    }

    inline proc nextStartIdxDeq {
      return (startIdxDeq.fetchAdd(1) % here.maxTaskPar : uint) : int;
    }

    proc init(type eltType, parentHandle) {
      this.eltType = eltType;
      this.parentHandle = parentHandle;
    }

    proc deinit() {
      forall segment in segments {
        var block = segment.headBlock;
        while block != nil {
          var tmp = block;
          block = block!.next;
          delete tmp;
        }
      }
    }

    proc add(elt : eltType) : bool {
      var startIdx = nextStartIdxEnq : int;
      var phase = ADD_BEST_CASE;

      while true {
        select phase {
          /*
            Pass 1: Best Case

            Find a segment that is unlocked and attempt to acquire it. As we are adding
            elements, we don't care how many elements there are, just that we find
            some place to add ours.
          */
          when ADD_BEST_CASE {
            for offset in 0 .. #here.maxTaskPar {
              ref segment = segments[(startIdx + offset) % here.maxTaskPar];

              // Attempt to acquire...
              if segment.acquireWithStatus(STATUS_ADD) {
                segment.addElements(elt);
                segment.releaseStatus();
                return true;
              }
            }

            phase = ADD_AVERAGE_CASE;
          }
          /*
            Pass 2: Average Case

            Find any segment (locked or unlocked) and make an attempt to acquire it.
          */
          when ADD_AVERAGE_CASE {
            ref segment = segments[startIdx];

            while true {
              select segment.currentStatus {
                // Quick acquire...
                when STATUS_UNLOCKED do {
                  if segment.acquireWithStatus(STATUS_ADD) {
                    segment.addElements(elt);
                    segment.releaseStatus();
                    return true;
                  }
                }
              }
              chpl_task_yield();
            }
          }
        }
      }

      halt("DistributedBag Internal Error: DEADCODE");
    }

    proc remove() : (bool, eltType) {
      var startIdx = nextStartIdxDeq;
      var idx = startIdx;
      var iterations = 0;
      var phase = REMOVE_BEST_CASE;
      var backoff = 0;

      while true {
        select phase {
          /*
            Pass 1: Best Case

            Find the first bucket that is both unlocked and contains elements. This is
            extremely helpful in the case where we have a good distribution of elements
            in each segment.
          */
          when REMOVE_BEST_CASE {
            while iterations < here.maxTaskPar {
              ref segment = segments[idx];

              // Attempt to acquire...
              if !segment.isEmpty && segment.acquireWithStatus(STATUS_REMOVE) {
                var (hasElem, elem) : (bool, eltType) = segment.takeElement();
                segment.releaseStatus();

                if hasElem {
                  return (hasElem, elem);
                }
              }

              iterations = iterations + 1;
              idx = (idx + 1) % here.maxTaskPar;
            }

            phase = REMOVE_AVERAGE_CASE;
          }

          /*
            Pass 2: Average Case

            Find the first bucket containing elements. We don't care if it is locked
            or unlocked this time, just that it contains elements; this handles majority
            of cases where we have elements anywhere in any segment.
          */
          when REMOVE_AVERAGE_CASE {
            while iterations < here.maxTaskPar {
              ref segment = segments[idx];

              // Attempt to acquire...
              while !segment.isEmpty {
                if segment.isUnlocked && segment.acquireWithStatus(STATUS_REMOVE) {
                  var (hasElem, elem) : (bool, eltType) = segment.takeElement();
                  segment.releaseStatus();

                  if hasElem {
                    return (hasElem, elem);
                  }
                }

                // Backoff
                chpl_task_yield();
              }

              iterations = iterations + 1;
              idx = (idx + 1) % here.maxTaskPar;
            }

            phase = REMOVE_WORST_CASE;
          }

          /*
            Pass 3: Worst Case

            After two full iterations, we're sure the queue is full at this point, so we
            can attempt to steal work from other nodes. In this pass, we find *any* segment
            and if it is empty, we attempt to become the work-stealer; if someone else is the
            current work stealer we assist them instead and lift an element for ourselves.

            Furthermore, in this phase we loop indefinitely until we are 100% certain it is
            empty or we get an item, so introduce some backoff here.
          */
          when REMOVE_WORST_CASE {
            while true {
              ref segment = segments[idx];

              select segment.currentStatus {
                // Quick acquire
                when STATUS_UNLOCKED {
                  if segment.acquireWithStatus(STATUS_REMOVE) {
                    // We're lucky; another element has been added to the current segment,
                    // take it and leave like normal...
                    if !segment.isEmpty {
                      var (hasElem, elem) : (bool, eltType) = segment.takeElement();
                      segment.releaseStatus();
                      return (hasElem, elem);
                    }

                    if parentHandle.targetLocales.size == 1 {
                      segment.releaseStatus();
                      var default: eltType;
                      return (false, default);
                    }

                    // Attempt to become the sole work stealer for this node. If we
                    // do not, we spin until they finish. We need to release the lock
                    // on our segment so our segment may be load balanced as well.
                    if loadBalanceInProgress.testAndSet() {
                      segment.releaseStatus();

                      loadBalanceInProgress.waitFor(false);
                      var notEmpty = loadBalanceResult.read();
                      if !notEmpty {
                        var default: eltType;
                        return (false, default);
                      }

                      // Reset our phase and scan for more elements...
                      phase = REMOVE_BEST_CASE;
                      break;
                    }

                    // We are the sole work stealer, and so it is our responsibility
                    // to balance the load for our node. We fork-join new worker
                    // tasks that will check horizontally across each node (as in
                    // across each segment with the same index), and vertically across
                    // each segment (each segment in a node). Horizontally, we steal
                    // at most a % of work from other nodes to give to ourselves.
                    // As load balancer, we also are the only one who knows whether
                    // or not all bags are empty.
                    var isEmpty : atomic bool;
                    isEmpty.write(true);
                    segment.releaseStatus();
                    coforall segmentIdx in 0..#here.maxTaskPar {
                      var stolenWork : [{0..#numLocales}] (int, c_ptr(eltType));
                      coforall loc in parentHandle.targetLocalesNotHere() {
                        if loc != here then on loc {
                          // As we jumped to the target node, 'localBag' returns
                          // the target's bag that we are attempting to steal from.
                          var targetBag = parentHandle.bag;

                          // Only proceed if the target is not load balancing themselves...
                          if !targetBag!.loadBalanceInProgress.read() {
                            ref targetSegment = targetBag!.segments[segmentIdx];

                            // As we only care that the segment contains data,
                            // we test-and-test-and-set until we gain ownership.
                            while targetSegment.nElems.read() >= distributedBagWorkStealingMinElems {
                              var backoff = 0;
                              if targetSegment.currentStatus == STATUS_UNLOCKED && targetSegment.acquireWithStatus(STATUS_REMOVE) {
                                // Sanity check: ensure segment did not fall under minimum since last check
                                if  targetSegment.nElems.read() < distributedBagWorkStealingMinElems {
                                  targetSegment.releaseStatus();
                                  break;
                                }

                                extern proc sizeof(type x): size_t;
                                // We steal at most 1MB worth of data. If the user has less than that, we steal a %, at least 1.
                                const mb = distributedBagWorkStealingMemCap * 1024 * 1024;
                                var toSteal = max(distributedBagWorkStealingMinElems, min(mb / sizeof(eltType), targetSegment.nElems.read() * distributedBagWorkStealingRatio)) : int;

                                // Allocate storage...
                                on stolenWork do stolenWork[loc.id] = (toSteal, c_malloc(eltType, toSteal));
                                var destPtr = stolenWork[here.id][2];
                                targetSegment.transferElements(destPtr, toSteal, stolenWork.locale.id);
                                targetSegment.releaseStatus();

                                // We are done...
                                break;
                              }

                              // Backoff...
                              chpl_task_yield();
                            }
                          }
                        }
                      }

                      // It is our job now to distribute all stolen data to the same
                      // horizontal segment on our node. Acquire lock...
                      ref recvSegment = segments[segmentIdx];
                      while true {
                        if recvSegment.currentStatus == STATUS_UNLOCKED && recvSegment.acquireWithStatus(STATUS_ADD) then break;
                        chpl_task_yield();
                      }

                      // Add stolen elements to segment...
                      for (nStolen, stolenPtr) in stolenWork {
                        if nStolen == 0 then continue;
                        recvSegment.addElementsPtr(stolenPtr, nStolen);
                        c_free(stolenPtr);

                        // Let parent know that the bag is not empty.
                        isEmpty.write(false);
                      }
                      recvSegment.releaseStatus();
                    }

                    loadBalanceResult.write(!isEmpty.read());
                    loadBalanceInProgress.write(false);

                    // At this point, if no work has been found, we will return empty...
                    if isEmpty.read() {
                      var default: eltType;
                      return (false, default);
                    } else {
                      // Otherwise, we try to get data like everyone else.
                      phase = REMOVE_BEST_CASE;
                      break;
                    }
                  }
                }
              }

              // Backoff to maximum...
              chpl_task_yield();
            }
          }

          otherwise do halt("DistributedBag Internal Error: Invalid phase #", phase);
        }

        // Reset variables...
        idx = startIdx;
        iterations = 0;
        backoff = 0;
      }

      halt("DistributedBag Internal Error: DEADCODE");
    }
  }
}
