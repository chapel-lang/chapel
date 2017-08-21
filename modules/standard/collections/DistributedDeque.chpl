/*
 * Copyright 2004-2017 Cray Inc.
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


use Collection;

/*
  A parallel-safe scalable distributed deque. A deque is a double-ended queue that supports
  insertion and removal from both ends of the queue, effectively supporting both
  FIFO, LIFO, and a Total ordering, where the order in which you add them will be
  the exact order you remove them in; for emphasis, a Deque can be used as a Queue,
  a Stack, and a List respectively. The deque enforces a 'barrier', which is a method
  of checking whether you may proceed with an operation, that is wait-free under
  most cases, lock-free in the worst case, which guarantees scalability. In the
  barrier, we perform freeze checks (if DEQUE_NO_FREEZE is not enabled), and bounds
  checking (if a capacity is given). Deque operations are separated into two
  levels: global and local. At a global level, we use simple fetchAdd and fetchSub
  counters to denote which local deque we apply our operation to, and at a local level
  we use an unrolled linked list which further has its own (non-atomic) counter.
*/

/*
  Frozen states... If we are FREEZE_UNFROZEN, we are mutable. If we are FREEZE_FROZEN,
  we are immutable. If we are FREEZE_MARKED, we are in the middle of a state change.
*/
private const FREEZE_UNFROZEN = 0;
private const FREEZE_MARKED = 1;
private const FREEZE_FROZEN = 2;

/*
  Size of each unroll block for each local deque node.
*/
config param DEQUE_BLOCK_SIZE = 8;
/*
    Turn off checks for freezing the data structure.
*/
config param DEQUE_NO_FREEZE = false;

// For each node we manage an unroll block. This block needs to also support Deque
// operations, and as such we maintain our own mini headIdx and tailIdx. Since we can
// add and remove from either direction, we must also maintain the size ourselves...
pragma "no doc"
class LocalDequeNode {
  type eltType;
  var elements : DEQUE_BLOCK_SIZE * eltType;
  var headIdx : int = 1;
  var tailIdx : int = 1;
  var size : int;
  var next : LocalDequeNode(eltType);
  var prev : LocalDequeNode(eltType);

  inline proc isFull {
    return size == DEQUE_BLOCK_SIZE;
  }

  inline proc isEmpty {
    return size == 0;
  }

  inline proc pushBack(elt : eltType) {
    elements[tailIdx] = elt;

    tailIdx += 1;
    if tailIdx > DEQUE_BLOCK_SIZE {
      tailIdx = 1;
    }
    size += 1;
  }

  inline proc popBack() : eltType {
    tailIdx -= 1;
    if tailIdx == 0 {
      tailIdx = DEQUE_BLOCK_SIZE;
    }

    size -= 1;
    return elements[tailIdx];
  }

  inline proc pushFront(elt : eltType) {
    headIdx -= 1;
    if headIdx == 0 {
      headIdx = DEQUE_BLOCK_SIZE;
    }

    elements[headIdx] = elt;
    size += 1;
  }

  inline proc popFront() : eltType {
    var elt = elements[headIdx];
    headIdx += 1;
    if headIdx > DEQUE_BLOCK_SIZE {
      headIdx = 1;
    }

    size -= 1;
    return elt;
  }
}

/*
  For each slot we maintain a deque.
*/
pragma "no doc"
class LocalDeque {
  type eltType;

  // Locks are atomic to allow us to remotely contest for the lock.
  var lock$ : sync bool;

  var head : LocalDequeNode(eltType);
  var tail : LocalDequeNode(eltType);

  // We cache the last deleted node to handle cases where we have rapid mixed push/pop
  var cached : LocalDequeNode(eltType);

  // The size of a segment. This is used as both a means of knowing when an element
  // gets added, as well as a barrier to prevent the head and tail from being cached.
  var size : atomic int;

  inline proc recycleNode() {
    // If we have cached a previous used node, reuse it here...
    if cached != nil {
      var tmp = cached;
      cached = nil;
      return tmp;
    }

    // Create a new one...
    return  new LocalDequeNode(eltType);
  }

  inline proc retireNode(node) {
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
        if tail.isFull {
          tail.next = recycleNode();
          tail.next.prev = tail;
          tail = tail.next;
        }

        // Push...
        tail.pushBack(_elt);
        size.add(1);

        lock$;
      }
    }
  }

  // TODO: Make 'local'
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
          _elt = tail.popBack();
          if tail.isEmpty {
            var node = tail;
            tail = tail.prev;

            // If not empty, remove reference to retired node...
            // If it is empty, fix the head...
            if tail != nil {
              tail.next = nil;
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
        if head.isFull {
          head.prev = recycleNode();
          head.prev.next = head;
          head = head.prev;
        }

        // Push...
        head.pushFront(_elt);
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
          _elt = head.popFront();
          if head.isEmpty {
            var node = head;
            head = head.next;

            // If not empty, remove reference to retired node...
            // If it is empty, fix the tail...
            if head != nil {
              head.prev = nil;
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

  proc ~LocalDeque() {
    on this {
      var curr = head;

      while curr != nil {
        var tmp = curr.next;
        delete curr;
        curr = tmp;
      }

      if cached != nil {
        delete cached;
      }
    }
  }
}

/*
  A parallel-safe scalable distributed double-ended queue that supports both
  insertion and removal from either end of the queue. Can be used as a Queue,
  Stack, or even a List.
*/
class DistributedDeque : Collection {
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
  var globalHead : atomic int;
  pragma "no doc"
  var globalTail : atomic int;
  pragma "no doc"
  var queueSize : atomic int;

  // Freezing the queue consists of two phases: The 'marked' phase, where the queue
  // is marked for a state change, which prevents any new tasks for a particular state
  // from entering, followed by a 'waiting' phase where we wait any concurrent tasks
  // to find across all nodes (in case they did not notice the state change). This
  // applies to state changes to frozen and unfrozen state.
  pragma "no doc"
  var concurrentTasks : atomic int;
  pragma "no doc"
  var frozenState : atomic int;

  // We maintain an array of slots, wherein each slot is a pointer into a node's
  // address space. To maximize parallelism, we maintain numLocales * maxTaskPar
  // to reduce the amount of contention.
  pragma "no doc"
  var nSlots : int;
  pragma "no doc"
  var slotSpace = {0..-1};
  pragma "no doc"
  var slots : [slotSpace] LocalDeque(eltType);

  proc DistributedDeque(type eltType, cap : int = -1, targetLocales : [?locDom] locale =Locales) {
    this.cap = cap;
    this.nSlots = here.maxTaskPar * targetLocales.size;
    this.slotSpace = {0..#this.nSlots};
    this.targetLocDom = locDom;
    this.targetLocales = targetLocales;

    // Initialize each slot. We use a round-robin algorithm.
    var idx : atomic int;
    for 0 .. #here.maxTaskPar {
      for loc in targetLocales do on loc {
        var i = idx.fetchAdd(1);
        slots[i] = new LocalDeque(eltType);
      }
    }

    pid = _newPrivatizedClass(this);
  }

  pragma "no doc"
  proc DistributedDeque(other, privData, type eltType = other.eltType) {
    this.cap = other.cap;
    this.targetLocDom = other.targetLocDom;
    this.targetLocales = other.targetLocales;
    this.nSlots = other.nSlots;
    this.slotSpace = {0..#this.nSlots};
    slots = other.slots;
  }

  pragma "no doc"
  proc dsiPrivatize(privData) {
      return new DistributedDeque(this, privData);
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
  inline proc enterRemoveBarrier(localThis) {
    // Enter freeze barrier...
    if !DEQUE_NO_FREEZE {
      local {
        localThis.concurrentTasks.add(1);

        // Check if the queue is now 'immutable'.
        if localThis.frozenState.read() > FREEZE_UNFROZEN {
          localThis.concurrentTasks.sub(1);
          return false;
        }
      }
    }

    // If we have a capacity of 0, then we can't really remove anything anyway.
    if localThis.cap == 0 {
      if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
      return false;
    }

    // Similar to add, we use a wait-free method for updating the queue size,
    // using fetch-sub instead. Like enqueue, we make attempts to fix the queue
    // size in a lock-free manner (for as long as it is negative). It should be
    // noted that if we see that the queue size is over capacity, meaning an
    // enqueue operation has over committed, we consequentially help them as well
    // during the fetch-sub loop.
    while true {
      var size = queueSize.fetchSub(1);

      // Negative, fix before returning...
      if size <= 0 {
        on this {
          var readSize = queueSize.read();
          // Attempt to fix, but yield to reduce potential contention and CPU hogging.
          while readSize < 0 && !queueSize.compareExchangeWeak(readSize, 0) {
            chpl_task_yield();
            readSize = queueSize.read();
          }
        }

        // Fixed, return early...
        if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
        return false;
      }
      // If we have a cap, then at this point, we have something between [0, cap],
      // if we do not have a cap then there is no need to 'fix' the queue size;
      // in both cases we are safe.
      else if localThis.cap < 0 || size <= localThis.cap {
        break;
      }
      // If the size is over capacity, a enqueuer has overcommitted. Since there can
      // be an element for us, we loop again.
    }

    return true;
  }

  pragma "no doc"
  inline proc enterAddBarrier(localThis) {
    // Enter freeze barrier...
    if !DEQUE_NO_FREEZE {
      local {
        localThis.concurrentTasks.add(1);

        // Check if the queue is now 'immutable'.
        if localThis.frozenState.read() > FREEZE_UNFROZEN {
          localThis.concurrentTasks.sub(1);
          return false;
        }
      }
    }

    // If we have a capacity of 0, then we can't really add anything anyway.
    if localThis.cap == 0 {
      if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
      return false;
    }

    // If we have a capacity, we must ensure that there is enough space for our
    // operation ahead of time. We take a wait-free approach by performing a
    // fetch-add on the queue size. If there is space, then we succeed within one
    // communication round-trip. If we fail, then we make attempts to fix the queue
    // size in a lock-free manner (for as long as it is over capacity). It should be
    // noted that if we see that the queue size is negative, meaning a dequeue operation
    // has over committed, we consequentially help them as well during the fetch-add loop.
    if localThis.cap > 0 {
      while true {
        var size = queueSize.fetchAdd(1);

        // Over capacity, fix before returning...
        if size >= localThis.cap {
          on this {
            var readSize = queueSize.read();
            // Attempt to fix, but yield to reduce potential contention and CPU hogging.
            while readSize > this.cap && !queueSize.compareExchangeWeak(readSize, this.cap) {
              chpl_task_yield();
              readSize = queueSize.read();
            }
          }

          // Fixed, return early...
          if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
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
        var size = queueSize.fetchAdd(1);

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
  proc add(elt : eltType) : bool {
    return pushBack(elt);
  }

  /*
    Syntactic sugar for `popFront`.
  */
  proc remove() : (bool, eltType) {
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
    var localThis = getPrivatizedThis;

    // Test for if we can continue...
    if enterAddBarrier(localThis) == false {
      return false;
    }

    // At this point, we know we have a space for us. We find our slot based on another
    // fetch-add counter, making this wait-free as well.
    var tail = globalTail.fetchAdd(1) % localThis.nSlots;
    localThis.slots[abs(tail)].pushBack(elt);
    if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
    return true;
  }

  /*
    Removes the element at the tail.
  */
  proc popBack() : (bool, eltType) {
    var localThis = getPrivatizedThis;

    // Test for if we can continue...
    if enterRemoveBarrier(localThis) == false {
      return (false, _defaultOf(eltType));
    }

    // We find our slot based on another fetch-add counter, making this wait-free as well.
    var tail = (globalTail.fetchSub(1) - 1) % localThis.nSlots;
    var elt = localThis.slots[abs(tail)].popBack();
    if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
    return (true, elt);
  }

  /*
    Appends the element to the head.
  */
  proc pushFront(elt : eltType) : bool {
    var localThis = getPrivatizedThis;

    // Test if we can continue...
    if enterAddBarrier(localThis) == false {
      return false;
    }

    // We find our slot based on another fetch-add counter, making this wait-free as well.
    var head = (globalHead.fetchSub(1) - 1) % localThis.nSlots;
    localThis.slots[abs(head)].pushFront(elt);
    if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
    return true;
  }

  /*
    Removes the element at the head.
  */
  proc popFront() : (bool, eltType) {
    var localThis = getPrivatizedThis;

    // Test for if we can continue...
    if enterRemoveBarrier(localThis) == false {
      return (false, _defaultOf(eltType));
    }

    // We find our slot based on another fetch-add counter, making this wait-free as well.
    var head = globalHead.fetchAdd(1) % localThis.nSlots;
    var elt = localThis.slots[abs(head)].popFront();
    if !DEQUE_NO_FREEZE then local { localThis.concurrentTasks.sub(1); }
    return (true, elt);
  }

  proc canFreeze() : bool {
    return !DEQUE_NO_FREEZE;
  }

  /*
    If we are currently frozen. If we are in the middle of a state transition, we
    wait until it has completed.
  */
  proc isFrozen() : bool {
    if DEQUE_NO_FREEZE then return false;
    var localThis = getPrivatizedThis;
    var state = localThis.frozenState.read();

    // Current transitioning state, wait it out...
    while state == FREEZE_MARKED {
      chpl_task_yield();
      state = localThis.frozenState.read();
    }

    return state == FREEZE_FROZEN;
  }

  /*
    Freeze our state, becoming immutable; we wait for any ongoing concurrent
    operations to allow them to finish.
  */
  proc freeze() : bool {
    if DEQUE_NO_FREEZE then return false;
    var localThis = getPrivatizedThis;

    // Check if already frozen
    if localThis.frozenState.read() == FREEZE_FROZEN {
      return true;
    }

    // Mark as transient state...
    coforall loc in targetLocales do on loc {
      var localThis = getPrivatizedThis;
      localThis.frozenState.write(FREEZE_MARKED);
      localThis.concurrentTasks.waitFor(0);
    }

    // Mark as frozen...
    coforall loc in targetLocales do on loc {
      var localThis = getPrivatizedThis;
      localThis.frozenState.write(FREEZE_FROZEN);
    }

    return true;
  }

  /*
    Unfreezes our state, allowing mutating operations; we wait on any ongoing
    concurrent operations to allow them to finish.
  */
  proc unfreeze() : bool {
    if DEQUE_NO_FREEZE then return false;
    var localThis = getPrivatizedThis;

    // Check if already unfrozen
    if localThis.frozenState.read() == FREEZE_UNFROZEN {
      return true;
    }

    // Mark as transient state...
    coforall loc in targetLocales do on loc {
      var localThis = getPrivatizedThis;
      localThis.frozenState.write(FREEZE_MARKED);
      localThis.concurrentTasks.waitFor(0);
    }

    // Mark as unfrozen...
    coforall loc in targetLocales do on loc {
      var localThis = getPrivatizedThis;
      localThis.frozenState.write(FREEZE_UNFROZEN);
    }

    return true;
  }

  /*
    Obtains the number of elements held by this queue.
  */
  proc size() : int {
    return queueSize.read();
  }

  /*
    Performs a lookup for the element in the data structure in parallel if it is
    frozen, and sequentially otherwise.
  */
  proc contains(elt : eltType) : bool {
    // Frozen lookups can be done concurrently
    if isFrozen() {
      var containsElem : atomic bool;
      forall elem in this {
        if elem == elt {
          containsElem.write(true);
        }
      }
      return containsElem.read();
    }

    // Non-frozen lookups require us to obtain the lock to ensure mutual exclusion
    var localThis = getPrivatizedThis;
    var foundItem : atomic bool;
    for slot in localThis.slots {
      on slot {
        const targetElt = elt;
        slot.lock$ = true;

        var node = slot.head;
        while node != nil {
          var headIdx = node.headIdx;
          for 1 .. node.size {
            if node.elements[headIdx] == targetElt {
              foundItem.write(true);
              break;
            }

            headIdx += 1;
            if headIdx > DEQUE_BLOCK_SIZE {
              headIdx = 1;
            }
          }

          if foundItem.read() {
            break;
          }

          node = node.next;
        }

        // Release...
        slot.lock$;
      }

      if foundItem.read() {
        return true;
      }
    }

    return false;
  }

  /*
    Iterate over all elements in the deque. This iterator does not yield in any
    particular order (see `FIFO` or `LIFO` for specific ordering).
  */
  iter these() : eltType {
    var frozen = isFrozen();
    for slot in getPrivatizedThis.slots {
      if !frozen then slot.lock$ = true;
      var node = slot.head;

      while node != nil {
        var headIdx = node.headIdx;
        for 1 .. node.size {
          yield node.elements[headIdx];

          headIdx += 1;
          if headIdx > DEQUE_BLOCK_SIZE {
            headIdx = 1;
          }
        }
        node = node.next;
      }

      if !frozen then slot.lock$;
    }
  }

  iter these(param tag : iterKind) where tag == iterKind.leader {
    coforall slot in getPrivatizedThis.slots do on slot do yield slot;
  }

  iter these(param tag : iterKind, followThis) where tag == iterKind.follower {
    var frozen = isFrozen();

    if !frozen then followThis.lock$ = true;
    var node = followThis.head;

    while node != nil {
      var headIdx = node.headIdx;
      for 1 .. node.size {
        yield node.elements[headIdx];

        headIdx += 1;
        if headIdx > DEQUE_BLOCK_SIZE {
          headIdx = 1;
        }
      }
      node = node.next;
    }

    if !frozen then followThis.lock$;
  }

  /*
    Iterates over the deque in First-In-First-Out order, from front to back. The
    deque must be frozen or it will result in a halt. This operation is sequential.
  */
  iter FIFO() : eltType {
    if !isFrozen() {
      halt("Ordered iteration requires the queue to be frozen.");
    }

    var localThis = getPrivatizedThis;

    // Fill our slots to visit in FIFO order.
    var head = globalHead.read();
    var tail = globalTail.read();
    var size = queueSize.read();

    // Check if empty...
    if size == 0 {
      return;
    }

    // We iterate directly over the heads of each slot, so we capture them in advance.
    var nodes : [{0..#nSlots}] (int, int, LocalDequeNode(eltType));
    for i in 0 .. #nSlots {
      var node = slots[i].head;
      nodes[i] = (node.size, node.headIdx, node);
    }

    // Iterate over captured head nodes; each time we read them we advance them
    var iterations = size;
    for i in head..#tail {
      if iterations == 0 then break;
      iterations -= 1;

      var idx = i % nSlots;
      var (size, headIdx, node) = nodes[idx];
      if node == nil {
        halt("Iterating over nil nodes, head: ", head, ", tail: ", tail, ", idx: ", i);
      }
      yield node.elements[headIdx];

      // Update state...
      size -= 1;
      headIdx += 1;
      if headIdx > DEQUE_BLOCK_SIZE {
        headIdx = 1;
      }

      // Advance...
      if size == 0 {
        node = node.next;
        if node != nil {
          nodes[idx] = (node.size, node.headIdx, node);
        } else {
          nodes[idx] = (-1, -1, node);
        }
      } else {
        // Else update state...
        nodes[idx] = (size, headIdx, node);
      }
    }
  }

  /*
    Iterates over the deque in Last-In-First-Out order, from back to front. The
    deque must be frozen or it will result in a halt. This operation is sequential.
  */
  iter LIFO() : eltType {
    if !isFrozen() {
      halt("Ordered iteration requires the queue to be frozen.");
    }

    var localThis = getPrivatizedThis;

    // Fill our slots to visit in FIFO order.
    var head = globalHead.read();
    var tail = globalTail.read();
    var size = queueSize.read();

    // Check if empty...
    if size == 0 {
      return;
    }

    // We iterate directly over the heads of each slot, so we capture them in advance.
    var nodes : [{0..#nSlots}] (int, int, LocalDequeNode(eltType));
    for i in 0 .. #nSlots {
      var node = slots[i].tail;
      nodes[i] = (node.size, node.tailIdx, node);
    }

    // Iterate over captured head nodes; each time we read them we advance them
    var iterations = size;
    while tail - 1 >= head - 1 {
      if iterations == 0 then break;
      iterations -= 1;

      var idx = (tail - 1) % nSlots;
      var (size, tailIdx, node) = nodes[idx];
      if node == nil {
        halt("Iterating over nil nodes, head: ", head, ", tail: ", tail, ", idx: ", tail-1);
      }
      yield node.elements[tailIdx];

      // Update state...
      size -= 1;
      tailIdx -= 1;
      if tailIdx == 0 {
        tailIdx = DEQUE_BLOCK_SIZE;
      }

      // Advance...
      if size == 0 {
        node = node.prev;
        if node != nil {
          nodes[idx] = (node.size, node.tailIdx, node);
        } else {
          nodes[idx] = (-1, -1, node);
        }
      } else {
        // Else update state...
        nodes[idx] = (size, tailIdx, node);
      }
    }
  }

  proc ~DistributedDeque() {
    var localThis = getPrivatizedThis;
    for slot in localThis.slots do delete slot;
  }
}
