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
  A parallel-safe scalable distributed deque. A deque is a double-ended queue that supports
  insertion and removal from both ends of the queue, effectively supporting both
  FIFO, LIFO, and a Total ordering, where the order in which you add them will be
  the exact order you remove them in; for emphasis, a Deque can be used as a Queue,
  a Stack, and a List respectively. The deque enforces a 'barrier', which is a method
  of checking whether you may proceed with an operation, that is wait-free which
  guarantees scalability, where we perform bounds checking. Bounds checking performs
  the necessary checks to ensure two invariants: A `pop` operation never proceeds
  if the deque is empty, and that a `push` operation never proceeds if the deque
  is full. In the case where we have a `pop` and a non-empty deque, or a `push`
  with a non-full deque, we proceed within a single atomic fetchSub or fetchAdd
  respectively, hence is wait-free. In the case that we have a `pop` with a
  empty deque, or a `push` with a full deque, the queueSize becomes either
  negative or over capacity. When this happens, the faulty operation will, via
  remote execution (on-statement), perform a CAS-retry loop. If the opposite operation
  (`push` is the opposite of `pop` and vice-versa) sees the size in a incorrect state,
  it will continue its fetchSub/fetchAdd cycle, also bringing the size closer to
  its correct state. Hence, the fetchSub/fetchAdd cycle is bounded in that the number
  of faulty operations is bounded by the number of opposing operations and
  is also bounded and wait-free. The CAS-retry loop is also bounded by the number
  of overlapping operations and is helped along by the opposing operation, and hence
  is also bounded and wait-free.

  Deque operations are separated into three levels: global, local, and node.
  At a global level, we use simple fetchAdd and fetchSub counters to denote which
  local deque we apply our operation to based on the globalHead and globalTail in
  a way that emulates a deque. At the local-level, after you acquire the LocalDeque
  lock for an operation, you operate on linked nodes in a deque-like manner. Finally,
  at the node-level, each linked list node is an unroll block which is also a deque
  in and of itself. This hierarchical system ensures that we maintain this ordering
  but opens up a bit of non-determinism in terms of overlapping tasks hashing to the
  same LocalDeque, which is linearized on it's lock. While the Deque's ordering
  of elements are always preserved, the ordering in which tasks enter the 'barrier'
  to the point they complete their insertion is not. It is possible that given two
  tasks A and B, for A to finish at the global level before B, but for B to finish
  at the local level before A. However, it should be noted that for any MPMC queue,
  this non-determinism is inherent in all lock-free and wait-free data structures.
*/



/*

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
*/
module DistributedDeque {

  public use Collection;

  /*
    Size of each unroll block for each local deque node.
  */
  config param distributedDequeBlockSize = 8;

  /*
    The ordering used for serial iteration. ``NONE``, the default, is the most performant
    and is algorithmically similar to parallel iteration.
  */
  enum Ordering {
    NONE,
    FIFO,
    LIFO
  }

  pragma "no doc"
  class DistributedDequeRC {
    type eltType;
    var _pid : int;

    proc deinit() {
      chpl_getPrivatizedCopy(unmanaged DistributedDequeImpl(eltType), _pid).Destroy();
      coforall loc in Locales do on loc {
        delete chpl_getPrivatizedCopy(unmanaged DistributedDequeImpl(eltType), _pid);
      }

    }
  }

  /*
    A parallel-safe scalable distributed double-ended queue that supports both
    insertion and removal from either end of the queue. Can be used as a Queue,
    Stack, or even a List.
  */
  record DistDeque {
    type eltType;
    /*
      The implementation of the Deque is forwarded. See :class:`DistributedDequeImpl` for
      documentation.
    */
    // This is unused, and merely for documentation purposes. See '_value'.
    var _impl : unmanaged DistributedDequeImpl(eltType)?;

    // Privatization id
    pragma "no doc"
    var _pid : int;
    // Reference counting
    pragma "no doc"
    var _rc : shared DistributedDequeRC(eltType);

    proc init(type eltType, cap = -1, targetLocales = Locales) {
      this.eltType = eltType;
      this._pid = (new unmanaged DistributedDequeImpl(eltType, cap, targetLocales)).pid;
      this._rc = new shared DistributedDequeRC(eltType, _pid = _pid);
    }

    pragma "no doc"
    inline proc _value {
      if _pid == -1 {
        halt("DistDeque is uninitialized...");
      }
      return chpl_getPrivatizedCopy(unmanaged DistributedDequeImpl(eltType), _pid);
    }

    forwarding _value;
  }

  /*
    Atomic counters that are allocated on a single node used by privatized instances.
    This comes with the benefit of being able to distribute communication across multiple
    nodes rather than one instance.
  */
  pragma "no doc"
  class DistributedDequeCounter {
    var _value : atomic int;

    forwarding _value;
  }

  class DistributedDequeImpl : CollectionImpl {
    /*
      Capacity, the maximum number of elements a Deque can hold. A `cap` of -1 is
      considered unbounded.
    */
    var cap : int;

    pragma "no doc"
    var targetLocDom : domain(1);

    /*
      Locales to distribute the `Deque` across.
    */
    var targetLocales : [targetLocDom] locale;

    // Privatization id
    pragma "no doc"
    var pid : int;

    // Keeps track of which slot we are on...
    pragma "no doc"
    var globalHead : unmanaged DistributedDequeCounter?;

    pragma "no doc"
    var globalTail : unmanaged DistributedDequeCounter?;

    pragma "no doc"
    var queueSize : unmanaged DistributedDequeCounter?;

    // We maintain an array of slots, wherein each slot is a pointer into a node's
    // address space. To maximize parallelism, we maintain numLocales * maxTaskPar
    // to reduce the amount of contention.
    pragma "no doc"
    var nSlots : int;

    pragma "no doc"
    var slotSpace = {0..-1};

    pragma "no doc"
    var slots : [slotSpace] unmanaged LocalDeque(eltType);

    proc init(type eltType,
              cap : int = -1,
              targetLocales : [?locDom] locale = Locales) {
      super.init(eltType);

      this.cap           = cap;
      this.targetLocDom  = locDom;
      this.targetLocales = targetLocales;
      this.nSlots        = here.maxTaskPar * targetLocales.size;
      this.slotSpace     = {0..#this.nSlots};

      complete();

      // Initialize each slot. We use a round-robin algorithm.
      var idx : atomic int;
      for 0 .. #here.maxTaskPar {
        for loc in targetLocales do on loc {
          var i = idx.fetchAdd(1);
          slots[i] = new unmanaged LocalDeque(eltType);
        }
      }

      // Distribute the globalHead, globalTail, and queueSize over the first 3 nodes...
      var countersLeftToAlloc = 3;
      while countersLeftToAlloc > 0 {
        for loc in targetLocales do on loc {
          select countersLeftToAlloc {
            when 3 do globalHead = new unmanaged DistributedDequeCounter();
            when 2 do globalTail = new unmanaged DistributedDequeCounter();
            when 1 do queueSize = new unmanaged DistributedDequeCounter();
          }

          countersLeftToAlloc -= 1;
        }
      }


      pid = _newPrivatizedClass(this);
    }

    pragma "no doc"
    proc init(other, privData, type eltType = other.eltType) {
      super.init(eltType);

      this.cap = other.cap;
      this.targetLocDom = other.targetLocDom;
      this.targetLocales = other.targetLocales;
      this.globalHead = other.globalHead;
      this.globalTail = other.globalTail;
      this.queueSize = other.queueSize;
      this.nSlots = other.nSlots;
      this.slotSpace = {0..#this.nSlots};
      slots = other.slots;

      complete();
    }

    pragma "no doc"
    proc dsiPrivatize(privData) {
        return new unmanaged DistributedDequeImpl(this, privData);
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
    inline proc enterRemoveBarrier() {
      // If we have a capacity of 0, then we can't really remove anything anyway.
      if cap == 0 {
        return false;
      }

      // Similar to add, we use a wait-free method for updating the queue size,
      // using fetch-sub instead. Like enqueue, we make attempts to fix the queue
      // size in a lock-free manner (for as long as it is negative). It should be
      // noted that if we see that the queue size is over capacity, meaning an
      // enqueue operation has over committed, we consequentially help them as well
      // during the fetch-sub loop.
      while true {
        var size = queueSize!.fetchSub(1);

        // Negative, fix before returning...
        if size <= 0 {
          on queueSize {
            var readSize = queueSize!.read();
            // Attempt to fix, but yield to reduce potential contention and CPU hogging.
            while readSize < 0 && !queueSize!.compareAndSwap(readSize, 0) {
              chpl_task_yield();
              readSize = queueSize!.read();
            }
          }

          // Fixed, return early...
          return false;
        }
        // If we have a cap, then at this point, we have something between [0, cap],
        // if we do not have a cap then there is no need to 'fix' the queue size;
        // in both cases we are safe.
        else if cap < 0 || size <= cap {
          break;
        }
        // If the size is over capacity, a enqueuer has overcommitted. Since there can
        // be an element for us, we loop again.
      }

      return true;
    }

    pragma "no doc"
    inline proc enterAddBarrier() {
      // If we have a capacity of 0, then we can't really add anything anyway.
      if cap == 0 {
        return false;
      }

      // If we have a capacity, we must ensure that there is enough space for our
      // operation ahead of time. We take a wait-free approach by performing a
      // fetch-add on the queue size. If there is space, then we succeed within one
      // communication round-trip. If we fail, then we make attempts to fix the queue
      // size in a lock-free manner (for as long as it is over capacity). It should be
      // noted that if we see that the queue size is negative, meaning a dequeue operation
      // has over committed, we consequentially help them as well during the fetch-add loop.
      if cap > 0 {
        while true {
          var size = queueSize!.fetchAdd(1);

          // Over capacity, fix before returning...
          if size >= cap {
            on queueSize {
              var readSize = queueSize!.read();
              // Attempt to fix, but yield to reduce potential contention and CPU hogging.
              while readSize > this.cap && !queueSize!.compareAndSwap(readSize, this.cap) {
                chpl_task_yield();
                readSize = queueSize!.read();
              }
            }

            // Fixed, return early...
            return false;
          }
          // At this point, we either have a capacity and something between [0, cap],
          // or we do not have a capacity and we have something greater than 0,
          // either of which means we are safe.
          else if size >= 0 {
            break;
          }
          // If the size is negative, a dequeuer has overcommitted. Since there can
          // be an element for us, we loop again.
        }
      }
      // If we have a 'negative' capacity, we do not have one and can hold an
      // infinite amount; this gives us a more optimized code path.
      else {
        while true {
          var size = queueSize!.fetchAdd(1);

          // We have reserved a spot for our operation.
          if size >= 0 {
            break;
          }
          // If the size is negative, a dequeuer has overcommitted. Since there can
          // be an element for us, we loop again.
        }
      }

      return true;
    }

    /*
      Syntactic sugar for `pushBack`.
    */
    override proc add(elt : eltType) : bool {
      return pushBack(elt);
    }

    /*
      Syntactic sugar for `popFront`.
    */
    override proc remove() : (bool, eltType) {
      return popFront();
    }

    /*
      Syntactic sugar for `pushBack`.
    */
    proc enqueue(elt : eltType) : bool {
      return pushBack(elt);
    }

    /*
      Syntactic sugar for `popFront`.
    */
    proc dequeue() : (bool, eltType) {
      return popFront();
    }

    /*
      Syntactic sugar for `pushBack`.
    */
    proc push(elt : eltType) : bool {
      return pushBack(elt);
    }

    /*
      Syntactic sugar for `popBack`.
    */
    proc pop() : (bool, eltType) {
      return popBack();
    }

    /*
      Appends the element to the tail.
    */
    proc pushBack(elt : eltType) : bool {
      // Test for if we can continue...
      if enterAddBarrier() == false {
        return false;
      }

      // At this point, we know we have a space for us. We find our slot based on another
      // fetch-add counter, making this wait-free as well.
      var tail = globalTail!.fetchAdd(1) % nSlots;
      slots[abs(tail)].pushBack(elt);
      return true;
    }

    /*
      Removes the element at the tail.
    */
    proc popBack() : (bool, eltType) {
      // Test for if we can continue...
      if enterRemoveBarrier() == false {
        var default: eltType;
        return (false, default);
      }

      // We find our slot based on another fetch-add counter, making this wait-free as well.
      var tail = (globalTail!.fetchSub(1) - 1) % nSlots;
      var elt = slots[abs(tail)].popBack();
      var retval = (true, elt);
      return retval;
    }

    /*
      Appends the element to the head.
    */
    proc pushFront(elt : eltType) : bool {
      // Test if we can continue...
      if enterAddBarrier() == false {
        return false;
      }

      // We find our slot based on another fetch-add counter, making this wait-free as well.
      var head = (globalHead!.fetchSub(1) - 1) % nSlots;
      slots[abs(head)].pushFront(elt);
      return true;
    }

    /*
      Removes the element at the head.
    */
    proc popFront() : (bool, eltType) {
      // Test for if we can continue...
      if enterRemoveBarrier() == false {
        var default: eltType;
        return (false, default);
      }

      // We find our slot based on another fetch-add counter, making this wait-free as well.
      var head = globalHead!.fetchAdd(1) % nSlots;
      var elt = slots[abs(head)].popFront();
      var retval = (true, elt);
      return retval;
    }

    /*
      Obtains the number of elements held by this queue.
    */
    override proc getSize() : int {
      return queueSize!.read();
    }

    /*
      Performs a lookup for the element in the data structure.
    */
    override proc contains(elt : eltType) : bool {
      var containsElem : atomic bool;
      forall elem in this {
        if elem == elt {
          containsElem.write(true);
        }
      }
      return containsElem.read();
    }

    /*
      Iterate over all elements in the deque in the order specified.
    */
    iter these(param order : Ordering = Ordering.NONE) : eltType where order == Ordering.NONE {
      for slot in slots {
        slot.lock$ = true;
        var node = slot.head;

        while node != nil {
          var headIdx = node!.headIdx;
          for 1 .. node!.size {
            yield node!.elements[headIdx];

            headIdx += 1;
            if headIdx > distributedDequeBlockSize {
              headIdx = 1;
            }
          }
          node = node!.next;
        }

        slot.lock$;
      }
    }

    iter these(param order : Ordering = Ordering.NONE) : eltType where order == Ordering.FIFO {
      // Fill our slots to visit in FIFO order.
      var head = globalHead!.read();
      var tail = globalTail!.read();
      var size = queueSize!.read();

      // Check if empty...
      if size == 0 {
        return;
      }

      // Acquire in locking order...
      for slot in slots do slot.lock$ = true;

      // We iterate directly over the heads of each slot, so we capture them in advance.
      var nodes : [{0..#nSlots}] (int, int, unmanaged LocalDequeNode(eltType)?);
      for i in 0 .. #nSlots {
        var node = slots[i].head;
        if node == nil {
          nodes[i] = (-1, -1, node);
        } else {
          nodes[i] = (node!.size, node!.headIdx, node);
        }
      }

      // Iterate over captured head nodes; each time we read them we advance them
      var iterations = size;
      for i in head..#tail {
        if iterations == 0 then break;
        iterations -= 1;

        var idx = abs(i % nSlots);
        var (size, headIdx, node) = nodes[idx];
        if node == nil || headIdx == -1 || size == -1 {
          halt("DistributedDeque Internal Error: Iterating over nil nodes, head: ", head, ", tail: ", tail, ", idx: ", i);
        }
        yield node!.elements[headIdx];

        // Update state...
        size -= 1;
        headIdx += 1;
        if headIdx > distributedDequeBlockSize {
          headIdx = 1;
        }

        // Advance...
        if size == 0 {
          node = node!.next;
          if node != nil {
            nodes[idx] = (node!.size, node!.headIdx, node);
          } else {
            nodes[idx] = (-1, -1, node);
          }
        } else {
          // Else update state...
          nodes[idx] = (size, headIdx, node);
        }
      }

      // Release in locking order...
      for slot in slots do slot.lock$;
    }

    iter these(param order : Ordering = Ordering.NONE) : eltType where order == Ordering.LIFO {
      // Fill our slots to visit in FIFO order.
      var head = globalHead!.read();
      var tail = globalTail!.read();
      var size = queueSize!.read();

      // Check if empty...
      if size == 0 {
        return;
      }

      // Acquire in locking order...
      for slot in slots do slot.lock$ = true;

      // We iterate directly over the heads of each slot, so we capture them in advance.
      var nodes : [{0..#nSlots}] (int, int, unmanaged LocalDequeNode(eltType)?);
      for i in 0 .. #nSlots {
        var node = slots[i].tail;
        nodes[i] = (node!.size, node!.tailIdx, node);
      }

      // Iterate over captured head nodes; each time we read them we advance them
      var iterations = size;
      while tail > head {
        if iterations == 0 then break;
        iterations -= 1;

        var idx = (tail - 1) % nSlots;
        var (size, tailIdx, node) = nodes[idx];
        if node == nil {
          halt("DistributedDeque Internal Error: Iterating over nil nodes, head: ", head, ", tail: ", tail);
        }

        tailIdx -= 1;
        if tailIdx == 0 {
          tailIdx = distributedDequeBlockSize;
        }
        yield node!.elements[tailIdx];

        // Update state...
        size -= 1;

        // Advance...
        if size == 0 {
          node = node!.prev;
          if node != nil {
            nodes[idx] = (node!.size, node!.tailIdx, node);
          } else {
            nodes[idx] = (-1, -1, node);
          }
        } else {
          // Else update state...
          nodes[idx] = (size, tailIdx, node);
        }

        tail -= 1;
      }

      // Release in locking order...
      for slot in slots do slot.lock$;
    }

    iter these(param order : Ordering = Ordering.NONE, param tag : iterKind) where tag == iterKind.leader {
      if order != Ordering.NONE {
        compilerWarning("Parallel iteration only supports ordering of type: ", Ordering.NONE);
      }
      coforall slot in slots do on slot do yield slot;
    }

    iter these(param order : Ordering = Ordering.NONE, param tag : iterKind, followThis) where tag == iterKind.follower {
      if order != Ordering.NONE {
        compilerWarning("Parallel iteration only supports ordering of type: ", Ordering.NONE);
      }

      followThis.lock$ = true;
      var node = followThis.head;

      while node != nil {
        var headIdx = node!.headIdx;
        for 1 .. node!.size {
          yield node!.elements[headIdx];

          headIdx += 1;
          if headIdx > distributedDequeBlockSize {
            headIdx = 1;
          }
        }
        node = node!.next;
      }

      followThis.lock$;
    }

    pragma "no doc"
    proc Destroy() {
      for slot in slots do on slot do delete slot;
      delete globalHead;
      delete globalTail;
      delete queueSize;
    }
  }

  // For each node we manage an unroll block. This block needs to also support Deque
  // operations, and as such we maintain our own mini headIdx and tailIdx. Since we can
  // add and remove from either direction, we must also maintain the size ourselves...
  pragma "no doc"
  class LocalDequeNode {
    type eltType;
    var elements : distributedDequeBlockSize * eltType;
    var headIdx : int = 1;
    var tailIdx : int = 1;
    var size : int;
    var next : unmanaged LocalDequeNode(eltType)?;
    var prev : unmanaged LocalDequeNode(eltType)?;

    inline proc isFull {
      return size == distributedDequeBlockSize;
    }

    inline proc isEmpty {
      return size == 0;
    }

    inline proc pushBack(elt : eltType) {
      elements[tailIdx] = elt;

      tailIdx += 1;
      if tailIdx > distributedDequeBlockSize {
        tailIdx = 1;
      }
      size += 1;
    }

    inline proc popBack() : eltType {
      tailIdx -= 1;
      if tailIdx == 0 {
        tailIdx = distributedDequeBlockSize;
      }

      size -= 1;
      return elements[tailIdx];
    }

    inline proc pushFront(elt : eltType) {
      headIdx -= 1;
      if headIdx == 0 {
        headIdx = distributedDequeBlockSize;
      }

      elements[headIdx] = elt;
      size += 1;
    }

    inline proc popFront() : eltType {
      var elt = elements[headIdx];
      headIdx += 1;
      if headIdx > distributedDequeBlockSize {
        headIdx = 1;
      }

      size -= 1;
      return elt;
    }
  }

  /*
    For each slot (core) we maintain a deque.

    **TODO:** Need to convert LocalDequeNode to use C_Ptr, double the size of each
    successor for better locality, and use the snapshot approach so that iteration
    does not prevent usage of other methods on the same Deque from the same task.
  */
  pragma "no doc"
  class LocalDeque {
    type eltType;

    var lock$ : sync bool;

    var head : unmanaged LocalDequeNode(eltType)?;
    var tail : unmanaged LocalDequeNode(eltType)?;

    // We cache the last deleted node to handle cases where we have rapid mixed push/pop
    var cached : unmanaged LocalDequeNode(eltType)?;

    // The size of a segment. This is used as both a means of knowing when an element
    // gets added, as well as a barrier to prevent the head and tail from being cached.
    var size : atomic int;

    inline proc recycleNode() {
      // If we have cached a previous used node, reuse it here...
      if cached != nil {
        var tmp = cached!;
        cached = nil;

        // Clean...
        tmp.headIdx = 1;
        tmp.tailIdx = 1;
        tmp.size = 0;
        tmp.next = nil;
        tmp.prev = nil;

        return tmp;
      }

      // Create a new one...
      return new unmanaged LocalDequeNode(eltType);
    }

    inline proc retireNode(node:unmanaged) {
      if cached != nil {
        delete cached;
      }

      cached = node;
    }

    proc pushBack(elt : eltType) {
      on this {
        var _elt = elt;
        local {
          lock$ = true;

          // Its empty...
          if tail == nil {
            tail = recycleNode();
            head = tail;
          }

          // Its full...
          if tail!.isFull {
            tail!.next = recycleNode();
            tail!.next!.prev = tail;
            tail = tail!.next;
          }

          // Push...
          tail!.pushBack(_elt);
          size.add(1);

          lock$;
        }
      }
    }

    proc popBack() : eltType {
      var elt : eltType;
      on this {
        var _elt : eltType;
        local {
          while true {
            // Check if there is an element for us...
            if size.read() == 0 {
              while size.read() == 0 {
                chpl_task_yield();
              }
            }

            lock$ = true;

            // Someone else came in and took a value, wait for the next one...
            if size.read() == 0 {
              lock$;
              continue;
            }

            // Pop...
            _elt = tail!.popBack();
            if tail!.isEmpty {
              var node = tail;
              tail = tail!.prev;

              // If not empty, remove reference to retired node...
              // If it is empty, fix the head...
              if tail != nil {
                tail!.next = nil;
              } else {
                head = nil;
              }

              retireNode(node);
            }

            size.sub(1);
            lock$;
            break;
          }
        }
        elt = _elt;
      }

      return elt;
    }

    proc pushFront(elt : eltType) {
      on this {
        var _elt = elt;
        local {
          lock$ = true;

          // Its empty...
          if head == nil {
            head = recycleNode();
            tail = head;
          }

          // Its full...
          if head!.isFull {
            head!.prev = recycleNode();
            head!.prev!.next = head;
            head = head!.prev;
          }

          // Push...
          head!.pushFront(_elt);
          size.add(1);

          lock$;
        }
      }
    }

    proc popFront() : eltType {
      var elt : eltType;
      on this {
        var _elt : eltType;
        local {
          while true {
            // Check if there is an element for us...
            if size.read() == 0 {
              while size.read() == 0 {
                chpl_task_yield();
              }
            }

            lock$ = true;

            // Someone else came in and took a value, wait for the next one...
            if size.read() == 0 {
              lock$;
              continue;
            }

            // Pop...
            _elt = head!.popFront();
            if head!.isEmpty {
              var node = head;
              head = head!.next;

              // If not empty, remove reference to retired node...
              // If it is empty, fix the tail...
              if head != nil {
                head!.prev = nil;
              } else {
                tail = nil;
              }

              retireNode(node);
            }

            size.sub(1);
            lock$;
            break;
          }
        }
        elt = _elt;
      }

      return elt;
    }

    proc deinit() {
      var curr = head;

      while curr != nil {
        var tmp = curr!.next;
        delete curr;
        curr = tmp;
      }

      if cached != nil {
        delete cached;
      }
    }
  }
}
