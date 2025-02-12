/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
  Implementation Description:
  ___________________________

  A highly parallel segmented multi-pool specialized for depth-first search (DFS).
  Each locale gets its own bag, and each bag is segmented into 'here.maxTaskPar'
  segments. To allow high performance at scale, segments are implemented as
  non-blocking split deques. This scheme consists in splitting segments into a
  "public" and a "private" portion using an atomic "split pointer". Each task
  pushes/pops elements to/from the tail of its segment ensuring a Last-In First-Out
  (LIFO) ordering. The latter guarantees a local DFS exploration. During load
  balancing operations, elements are stolen by other tasks from the head of the
  segment, in a First-In First-Out (FIFO) manner. This implementation allows lock-free
  local access to the private portion of the segments and copy-free transfer of
  elements between the public and private portions. Work transfer is done by moving
  the split pointer in either direction using appropriate methods.

  The data structure is equipped with a dynamic load balancing mechanism, based on
  the work stealing (WS) paradigm. In few words, when a segment is (near)-empty,
  the associated task will try to steal elements from the public portion of other
  segments, potentially from other locales. During WS operations, thief tasks
  synchronize using a lock. This mechanism is transparently managed by the
  :proc:`distBag.remove` method of the distBag. Three scenarios may occur:

      * Best case: the private portion of the calling task is not empty, and we
                   simply get a element.

      * Local steal: if the best case failed, the calling task first tries to steal
                     elements locally, i.e. from another segment of its bag instance.
                     To select the victim task, we iterate over the segments using
                     either the "random" (default) or "round-robin" policy. When a
                     segment is eligible to be stolen from, one element is removed,
                     and the stealing process stops. Since DFS ensures that the
                     shallowest nodes in a search tree are stored at the head of each
                     segment, stealing one is generally sufficient since the stolen
                     node leads to a potentially large sub-tree.

      * Global steal: if the local steal failed, the calling task tries to steal
                      elements globally, i.e. from another bag instance. To choose
                      victim tasks, we iterate over locales to find an eligible
                      locale. When considering a locale, we iterate over its segments
                      similarly to the previous case. When an eligible locale is
                      found, we try to steal one element from each of its segments.
                      Since the global WS is a heavy-weight operation, stealing
                      one is no more appropriate, and we steal each segment in order
                      to not create load-unbalance between them.

  The data structure scales in terms of nodes, processors per node, and even workload.
  Nevertheless, it does not come without flaws; as WS is dynamic and triggered
  on demand, WS can still be performed in excess, which dramatically causes a
  performance drop. Furthermore, a severe imbalance across locales, such as an
  unfair distribution of work to a single or small subset of locales, may also
  causes an equally severe performance degradation. As well, to achieve true
  parallelism, usage of a privatized instance is a requirement, as it avoids the
  overhead of remotely accessing class fields, bounding performance on communication.
*/

/*
  Implements a parallel segmented multi-pool for depth-first tree-search.
  The data structure is sometimes referred to as ``DistBag_DFS``.

  Summary
  _______

  A parallel-safe distributed multi-pool implementation specialized for depth-first
  search (DFS), that scales in terms of nodes, processors per node (PPN), and workload;
  the more PPN, the more segments we allocate to increase raw parallelism, and the larger
  the workload the better locality (see :const:`distributedBagInitialSegmentCap`). This
  data structure is locally ordered (ensuring DFS), encapsulates a dynamic work stealing
  mechanism to balance work across nodes, and provides a means to obtain a privatized
  instance of the data structure for maximized performance.

  .. note::

    This module is a work in progress and may change in future releases.

  Usage
  _____

  To use :record:`distBag`, the initializer must be invoked explicitly to
  properly initialize the data structure. By default, one bag instance is
  initialized per locale, and one segment per task.

  .. code-block:: chapel

    var bag = new distBag(int, targetLocales=ourTargetLocales);

  The basic methods that distBag supports require a ``taskId`` argument. This
  ``taskId`` will serve as an index to the segment to be updated and it must be
  in ``0..<here.maskTaskPar``. More precisely, it is used to map each task to a
  segment, which ensures the parallel-safety of the data structure, as well as the
  local DFS ordering.

  .. code-block:: chapel

    bag.add(0, taskId);
    bag.addBulk(1..100, taskId);
    var (hasElt, elt) = bag.remove(taskId)

  While the bag is safe to use in a distributed manner, each locale always operates
  on its privatized instance. This means that it is easy to add data in bulk,
  expecting it to be distributed, when in reality it is not; if another locale needs
  data, it will steal work on-demand. Here is an example of concurrent operations
  on distBag across multiple locales and tasks:

  .. code-block:: chapel

    coforall loc in Locales do on loc {
      coforall taskId in 0..<here.maxTaskPar {
        var (hasElt, elt) = bag.remove(taskId);
        if hasElt {
          elt += 1;
          bag.add(elt, taskId);
        }
      }
    }

  Finally, distBag supports serial and parallel iteration, as well as a set of
  global operations. Here is an example of a distributed parallel iteration and a
  few global operations working with a ``distBag``:

  .. code-block:: chapel

    forall elts in bag do
      body();

    const size = bag.getSize();
    const foundElt = bag.contains(elt);
    bag.clear();

  Methods
  _______
*/

module DistributedBag
{
  public use Collection;
  private use IO;

  use Random;
  use List;
  use Math;

  /*
    The scenarios of the remove operation (See the Implementation Details section).
  */
  private param REMOVE_BEST_CASE    = 1;
  private param REMOVE_LOCAL_STEAL  = 2;
  private param REMOVE_GLOBAL_STEAL = 3;

  /*
    TODO: For some reasons, this patch allows the dynamic work stealing to alleviate
    the bottleneck that may occur in distributed settings. This is expected
    to be remove in the future. I need to understand the phenomenon, fix it
    and remove this patch.
  */
  private param PERFORMANCE_PATCH   = 4;

  /*
    Outputs of the remove operation. They are used to indicate the final status
    of the operation: success or fail.
  */
  private param REMOVE_SUCCESS   = true;
  private param REMOVE_FAIL      = false;
  /*
    NOTE: A third output could be used to optimize user code in scenarios
    where the remove operation fails, but there is a pending work stealing
    operation - meaning that the distBag is not necessarily empty.
    For now, it is considered as a usual fail.
  */
  private param REMOVE_FAST_EXIT = REMOVE_FAIL;

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
    stolen from. This may be useful if some segments contain fewer elements than
    others and should not be stolen from.
  */
  config const distributedBagWorkStealingMinElts: int = 1;

  /*
    The victim selection policy for work stealing.
  */
  enum VictimPolicy {
    /*
      Random selection.
    */
    RAND,
    /*
      Circular or ring-like selection, where each task steals work from the next
      task in sequence, with the last task stealing from the first.
    */
    RING
  }

  /*
    Reference counter for DistributedBag.
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
    A parallel-safe distributed multi-pool implementation specialized for depth-first
    search (DFS), that scales in terms of nodes, processors per node (PPN), and workload;
    the more PPN, the more segments we allocate to increase raw parallelism, and the
    larger the workload the better locality (see :const:`distributedBagInitialSegmentCap`).
    This data structure is locally DFS ordered, encapsulates a dynamic work stealing
    mechanism to balance work across nodes, and provides a means to obtain a privatized
    instance of the data structure for maximized performance.
  */
  pragma "always RVF"
  record distBag : serializable
  {
    /*
      The type of the elements contained in this distBag.
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
      if (_pid == -1) then halt("distBag is uninitialized.");
      return chpl_getPrivatizedCopy(unmanaged DistributedBagImpl(eltType), _pid);
    }

    @chpldoc.nodoc
    proc deserialize(reader, ref deserializer) throws {
      compilerError("Reading a distBag is not supported");
    }

    @chpldoc.nodoc
    proc init(type eltType, reader: fileReader, ref deserializer) {
      this.init(eltType);
      compilerError("Deserializing a distBag is not yet supported");
    }

    // Write the contents of this distBag to a channel.
    @chpldoc.nodoc
    proc serialize(writer, ref serializer) throws {
      writer.write("[");
      var size = this.getSize();
      for (i, iteration) in zip(this, 0..<size) {
        writer.write(i);
        if (iteration < size-1) then writer.write(", ");
      }
      writer.write("]");
    }

    forwarding _value;
  } // end 'distBag' record

  class DistributedBagImpl : CollectionImpl(?)
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
      Initialize an empty distBag.
    */
    proc init(type eltType, targetLocales: [?targetLocDom] locale = Locales)
    {
      super.init(eltType);

      this.targetLocDom  = targetLocDom;
      this.targetLocales = targetLocales;

      init this;

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

      init this;

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
      preserved. If :const:`distributedBagMaxSegmentCap` is reached, the insertion
      fails and returns `false`.

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

    /*
      Remove an element from segment ``taskId``. The order in which elements are
      removed is guaranteed to be the same order they have been inserted. If this
      bag instance is empty, it will attempt to steal elements from bags of other
      nodes.

      :arg taskId: The index of the segment from which the element is removed.
      :type taskId: `int`

      :arg policy: The victim selection policy for work stealing.
      :type policy: :type:`VictimPolicy`

      :return: Depending on the scenarios: `(true, elt)` if we successfully removed
               element `elt` from distBag; `(false, defaultOf(eltType))` otherwise.
      :rtype: `(bool, eltType)`
    */
    proc remove(taskId: int, param policy: VictimPolicy = VictimPolicy.RAND): (bool, eltType)
    {
      return bag!.remove(taskId, policy);
    }
    /*
      TODO: There are types in Chapel that cannot be default-initialized. One example
      is owned MyClass (rather than owned MyClass?). To support storing such types
      in a distBag, we would have it throw if the removal failed, and always return
      the removed element on success.
    */

    // TODO: implement 'removeBulk'.

    /*
      Obtain the number of elements held in this distBag.

      :return: The current number of elements contained in this distBag.
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
      distBag.

      :arg elt: An element to search for.
      :type elt: `eltType`

      :return: `true` if this distBag contains ``elt``.
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
      Clear this distBag.

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

    // TODO: implement a static load balancing?

    /*
      Iterate over the elements of this distBag. To avoid holding onto locks,
      we take a snapshot approach, increasing memory consumption but also
      increasing parallelism. This allows other concurrent, even mutating,
      operations while iterating, but opens the possibility to iterating over
      duplicates or missing elements from concurrent operations.

      :yields: A reference to one of the elements contained in this distBag.

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

            segment.lock_block.readFE();
            // Create a snapshot
            dom = {0..#segment.nElts};
            for i in dom {
              buffer[i] = segment.block.elts[segment.block.headId + i];
            }
            segment.lock_block.writeEF(true);
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

          segment.lock_block.readFE();
          // Create a snapshot
          var bufferSize = segment.nElts;
          var buffer: [0..#bufferSize] eltType;
          for i in 0..#bufferSize {
            buffer[i] = segment.block.elts[segment.block.headId + i];
          }
          segment.lock_block.writeEF(true);

          // Yield this chunk
          yield (bufferSize, buffer);
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
    We maintain a multi-pool 'bag' per locale. Each bag keeps a handle to its parent,
    which is required for work stealing.
  */
  @chpldoc.nodoc
  class Bag
  {
    type eltType;

    var parentHandle: borrowed DistributedBagImpl(eltType);

    var segments: [0..#here.maxTaskPar] Segment(eltType);

    var globalStealInProgress: atomic bool = false;

    proc init(type eltType, parentHandle)
    {
      this.eltType = eltType;
      this.parentHandle = parentHandle;
      // KNOWN ISSUE: 'init this' produces an error when 'eltType' is a Chapel
      // array (see Github issue #19859).
    }

    proc deinit()
    {
      forall segment in segments do
        delete segment.block;
    }

    /*
      Insert an element in segment ``taskId``. The ordering is guaranteed to be
      preserved. If :const:`distributedBagMaxSegmentCap` is reached, the insertion
      fails and returns `false`.
    */
    proc add(elt: eltType, const taskId: int): bool
    {
      return segments[taskId].addElement(elt);
    }

    /*
      Insert elements in bulk in segment ``taskId``. If the bag instance rejects
      an element (e.g., if :const:distributedBagMaxSegmentCap is reached), we
      cease to offer more. We return the number of elements successfully inserted.
    */
    proc addBulk(elts, const taskId: int): int
    {
      return segments[taskId].addElements(elts);
    }

    /*
      Iterate over the segments/locales eligible to be stolen from, according to
      the specified policy. By default, the random strategy is chosen and the
      calling task/locale cannot be chosen. We can also specify how many victims
      to check for eligibility; 1 by default.
    */
    iter victim(const N: int, const callerId: int, param policy: VictimPolicy,
      const tries: int = 1): int
    {
      var count: int;
      var limit: int = if (callerId == -1) then N else N-1;

      select policy {
        // Circular or ring-like selection.
        when VictimPolicy.RING {
          var id = (callerId + 1) % N;

          while ((count < limit) && (count < tries)) {
            yield id;
            count += 1;
            id = (id + 1) % N;
          }
        }
        // Random selection.
        when VictimPolicy.RAND {
          var id: int;
          const victims = permute(0..#N);

          while ((count < limit) && (count < tries)) {
            if (victims[id] != callerId) {
              yield victims[id];
              count += 1;
            }
            id += 1;
          }
        }
        otherwise halt("DistributedBag internal error: Unknown victim choice policy");
      }
    }
    /*
      TODO: Create a seed here for the RNG that includes the locale ID. Since we aren't
      specifying the seed, we get one based on the time, but this might lead multiple
      locales running this in parallel choosing the same permutation.
    */
    /*
      TODO: Computing the permutation as an array seems pretty heavy-weight if we
      are only going to try 1 segment. For tries < N, it might be better to create
      a random stream and get bounded numbers with getNext with min and max arguments;
      and then just accept that the same candidate might be checked multiple times.
      If the iteration is bounded by limit, then indeed it might be necessary to
      compute the permutation to ensure that each segment is visited. I suppose
      it could do some random searching and then a round-robin strategy to make
      sure each is visited.
    */

    /*
      Remove an element from segment ``taskId``. The order in which elements are
      removed is guaranteed to be the same order they have been inserted. If the
      best case fails, it trigger a work stealing mechanism (See the Implementation
      Details section).
    */
    proc remove(const taskId: int, param policy: VictimPolicy): (bool, eltType)
    {
      var phase = REMOVE_BEST_CASE;
      if (numLocales > 1) then phase = PERFORMANCE_PATCH;

      ref segment = segments[taskId];
      var default: eltType;

      while true {
        select phase {
          /*
            TODO: For some reasons, this patch allows the dynamic WS to alleviate
            the bottleneck that may occur in distributed settings. This is expected
            to be remove in the future. I need to understand the phenomenon, fix it
            and remove this patch.
          */
          when PERFORMANCE_PATCH {
            phase = REMOVE_BEST_CASE;
          }

          /*
            Best case: the private portion of the calling task is not empty, and we
                       simply get a element.
          */
          when REMOVE_BEST_CASE {
            if (segment.nElts_private > 0) {
              return (REMOVE_SUCCESS, segment.takeElement());
            }

            phase = REMOVE_LOCAL_STEAL;
          }

          /*
            Local steal: if the best case failed, the calling task first tries to steal
                         elements locally, i.e. from another segment of its bag instance.
                         To select the victim task, we iterate over the segments using
                         either the "random" (default) or "round-robin" policy. When a
                         segment is eligible to be stolen from, one element is removed,
                         and the stealing process stops. Since DFS ensures that the
                         shallowest nodes in a search tree are stored at the head of
                         each segment, stealing one is generally sufficient since the
                         stolen node lead to a potentially large sub-tree.
          */
          when REMOVE_LOCAL_STEAL {
            var splitreq: bool = false;

            if globalStealInProgress.read() then return (REMOVE_FAST_EXIT, default);

            // iterate over the victim tasks
            for victimTaskId in victim(here.maxTaskPar, taskId, policy, here.maxTaskPar) {
              ref targetSegment = segments[victimTaskId];

              if !targetSegment.globalSteal.read() {
                targetSegment.lock_block.readFE();
                // check eligibility
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
            Global steal: if the local steal failed, the calling task tries to steal
                          elements globally, i.e. from another bag instance. To choose
                          victim tasks, we iterate over locales to find an eligible
                          locale. When considering a locale, we iterate over its segments
                          similarly to the previous case. When an eligible locale is
                          found, we try to steal one element from each of its segments.
                          Since the global WS is a heavy-weight operation, stealing
                          one is no more appropriate, and we steal each segment in order
                          to not create load-unbalance between them.
          */
          when REMOVE_GLOBAL_STEAL {
            // fast exit for single-node execution
            if (numLocales == 1) then return (REMOVE_FAIL, default);

            // lock the global steal operation
            if !globalStealInProgress.compareAndSwap(false, true) {
              return (REMOVE_FAST_EXIT, default);
            }

            const parentPid = parentHandle.pid;
            var stolenElts: list(eltType);
            /*
              TODO: Such distributed list insertion should not be particularly efficient.
              The list data type works better if you are appending; inserting at
              the 0th position is O(n).
            */

            // iterate over the victim locales
            for victimLocaleId in victim(numLocales, here.id, policy, 1) {
              on Locales[victimLocaleId] {
                var targetBag = chpl_getPrivatizedCopy(parentHandle.type, parentPid).bag;
                // iterate over the victim tasks
                for victimTaskId in victim(here.maxTaskPar, -1, policy, here.maxTaskPar) {
                  ref targetSegment = targetBag!.segments[victimTaskId];

                  targetSegment.globalSteal.write(true);

                  targetSegment.lock_block.readFE();
                  // check eligibility
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
              // WS fail
              globalStealInProgress.write(false);
              return (REMOVE_FAIL, default);
            }
            else {
              // insert the stolen elements
              segment.addElements(stolenElts);
              globalStealInProgress.write(false);
              return (REMOVE_SUCCESS, segment.takeElement());
            }
          }

          otherwise do halt("DistributedBag Internal Error: Invalid phase #", phase);
        }
        currentTask.yieldExecution();
      }

      halt("DistributedBag Internal Error: DEADCODE.");
    }
  } // end 'Bag' class

  /*
    A Segment is a parallel-safe pool, implemented as a non-blocking split deque
    (see header). In few words, it is a buffer of memory, called Block, along with
    some logic to ensure parallel-safety.
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
    var nElts_shared: atomic int; // number of elements in the shared portion

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
      Return the size of the private portion.
    */
    inline proc nElts_private
    {
      return tail - o_split;
    }

    /*
      Return the global size.
    */
    inline proc nElts
    {
      return nElts_private + nElts_shared.read();
    }

    /*
      Check if empty.
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
      Insert an element. The ordering is guaranteed to be preserved. If
      :const:`distributedBagMaxSegmentCap` is reached, the insertion fails and
      returns `false`.
    */
    inline proc ref addElement(elt: eltType): bool
    {
      // allocate a larger block with the double capacity.
      if block.isFull {
        if (block.cap == distributedBagMaxSegmentCap) then
          return false;
        lock_block.readFE();
        block.cap *= 2;
        block.dom = {0..#block.cap};
        lock_block.writeEF(true);
      }

      // add the element to the tail
      block.pushTail(elt);
      tail += 1;

      // check split request
      if split_request.read() then split_release();

      return true;
    }

    /*
      Insert elements in bulk. If the segment rejects an element (e.g., if
      distributedBagMaxSegmentCap is reached), we cease to offer more. We return
      the number of elements successfully inserted.
    */
    inline proc ref addElements(elts): int
    {
      var size = elts.size;

      // allocate a larger block.
      if (block.tailId + size > block.cap) {
        const neededCap = block.cap*2**ceil(log2((block.tailId + size) / block.cap:real)):int;
        if (neededCap >= distributedBagMaxSegmentCap) then
          size = distributedBagMaxSegmentCap - block.tailId - 1;
        lock_block.readFE();
        block.cap = min(distributedBagMaxSegmentCap, neededCap);
        block.dom = {0..#block.cap};
        lock_block.writeEF(true);
      }

      // add the elements to the tail
      for elt in elts[0..#size] do block.pushTail(elt);
      tail += size;

      // check split request
      if split_request.read() then split_release();

      return size;
    }

    /*
      Remove an element.
    */
    inline proc ref takeElement(): eltType
    {
      // remove an element from the tail
      var elt = block.popTail();
      tail -= 1;

      // check split request
      if split_request.read() then split_release();

      return elt;
    }

    // TODO: implement 'takeElements', needed by 'removeBulk'

    // TODO: implement 'transferElements'

    /*
      Perform simultaneously two compareAndSwap operations. This ensures that
      both atomic variables are accessed at the same time.
    */
    inline proc ref simCAS(A: atomic int, B: atomic int, expA: int, expB: int,
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
      halt("DistributedBag Internal Error: DEADCODE");
    }

    /*
      Steal an element.
    */
    inline proc ref stealElement(): (bool, eltType)
    {
      var default: eltType;

      // if the shared region becomes empty due to a concurrent operation
      if (nElts_shared.read() == 0) then return (false, default);

      lock.readFE(); // set locked (empty)
      var (h, s): (int, int) = (head.read(), split.read());
      lock.writeEF(true); // set unlocked (full)

      // check eligibility, again
      if (h < s) {
        // try to move the pointers
        if simCAS(head, split, h, s, h+1, s) {
          lock_n.readFE();
          // get an element from the head of the shared portion
          var elt = block.popHead();
          nElts_shared.sub(1);
          lock_n.writeEF(true);

          return (true, elt);
        }
        else {
          return (false, default);
        }
      }

      // set the split request, if not already set
      if !split_request.read() then split_request.write(true);

      return (false, default);
    }

    /*
      Increase the shared portion of the segment (and decrease the private one).
    */
    inline proc ref split_release(): void
    {
      // fast exit
      if (nElts_private <= 1) then return;

      // compute the new split position
      var new_split: int = ((o_split + tail + 1) / 2): int;
      lock.readFE(); // block until its full and set locked (empty)
      split.write(new_split);
      lock.writeEF(true); // set unlocked (full)

      // updates counters
      lock_n.readFE();
      nElts_shared.add(new_split - o_split);
      lock_n.writeEF(true);

      o_split = new_split;

      // reset split request
      split_request.write(false);
    }

    /*
      Decrease the shared portion of the segment (and increase the private one).
    */
    inline proc ref split_reacquire(): bool
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
        // fence
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
    A Block is a Chapel array that holds a buffer of memory. Each block size
    *should* be a power of two, as we extend the size of each full block by
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
