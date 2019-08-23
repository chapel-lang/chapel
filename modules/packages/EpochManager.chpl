/*
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

To use the :class:`LocalEpochManager`, first create an instance.

.. code-block:: chapel

 var manager = new LocalEpochManager();


Registering a Task
------------------
A task must be registered with the manager in order to use the manager.
Registration returns a token that is used when a task inters or exits a critical section.

.. code-block:: chapel

 var tok = manager.register();


Pinning/Unpinning a Task
------------------------
To avoid reclamation while a task is accessing a resource, I.E. to enter
critical section, a task must `pin`. Correspondingly to exit critical section,
the task must `unpin`.

.. code-block:: chapel

 // Enter critical section
 tok.pin();
 // Do something

 // Exit critical section
 tok.unpin();


Deleting an object
------------------
To `delete` an object:

.. code-block:: chapel

 tok.deferDelete(myObj);


.. note::
 A task must be `pinned` to `delete` an object. The manager can only be used to
 delete ``unmanaged`` objects.


Reclaiming deleted objects
--------------------------
To try to reclaim memory:

.. code-block:: chapel

 tok.tryReclaim();


.. note::
 Alternatively, a task may call ``manager.tryReclaim()``.


Unregister a Task
-----------------
In the end, a registered task needs to `unregister` from the manager. The
registration token is a scoped variable, and hence the ending of the scope in
which the task registered wiill automatically `unregister` the task.
`unregister` can also be performed manually:

.. code-block:: chapel

 tok.unregister();

Destroy
-------
To destroy the manager, and reclaim all the memory managed by the manager:

.. code-block:: chapel

 delete manager;
*/
module EpochManager {

  pragma "no doc"
  module LockFreeLinkedListModule {

    use AtomicObjects;

    class Node {
      type eltType;
      var val : eltType;
      var next : unmanaged Node(eltType);

      proc init(val : ?eltType) {
        this.eltType = eltType;
        this.val = val;
      }

      proc init(type eltType) {
        this.eltType = eltType;
      }
    }

    class LockFreeLinkedList {
      type objType;
      var _head : AtomicObject(unmanaged Node(objType), hasABASupport=true, hasGlobalSupport=true);

      proc init(type objType) {
        this.objType = objType;
      }

      proc append(newObj : objType) {
        var _node = new unmanaged Node(newObj);
        do {
          var oldHead = _head.readABA();
          _node.next = oldHead.getObject();
        } while(!_head.compareExchangeABA(oldHead, _node));
      }

      iter these() : objType {
        var ptr = _head.read();
        while (ptr != nil) {
          yield ptr.val;
          ptr = ptr.next;
        }
      }

      proc deinit() {
        var ptr = _head.read();
        while (ptr != nil) {
          var next = ptr.next;
          delete ptr;
          ptr = next;
        }
      }
    }
  }

  pragma "no doc"
  module LockFreeQueueModule {

    use AtomicObjects;

    class Node {
      type eltType;
      var val : eltType;
      var next : AtomicObject(unmanaged Node(eltType), hasABASupport=true, hasGlobalSupport=true);
      var freeListNext : unmanaged Node(eltType);

      proc init(val : ?eltType) {
        this.eltType = eltType;
        this.val = val;
      }

      proc init(type eltType) {
        this.eltType = eltType;
      }
    }

    class LockFreeQueue {
      type objType;
      var _head : AtomicObject(unmanaged Node(objType), hasABASupport=true, hasGlobalSupport=true);
      var _tail : AtomicObject(unmanaged Node(objType), hasABASupport=true, hasGlobalSupport=true);
      var _freeListHead : AtomicObject(unmanaged Node(objType), hasABASupport=true, hasGlobalSupport=true);
      // Flag to set if objects held in the queue are to be deleted or not.
      // By default initialised to true.
      const delete_val : bool;

      proc init(type objType) {
        this.objType = objType;
        delete_val = true;
        this.complete();
        var _node = new unmanaged Node(objType);
        _head.write(_node);
        _tail.write(_node);
      }

      proc init(type objType, delete_val : bool) {
        this.objType = objType;
        this.delete_val = delete_val;
        this.complete();
        var _node = new unmanaged Node(objType);
        _head.write(_node);
        _tail.write(_node);
      }

      proc recycleNode() : unmanaged Node(objType) {
        var oldTop : ABA(unmanaged Node(objType));
        var n : unmanaged Node(objType);
        do {
          oldTop = _freeListHead.readABA();
          n = oldTop.getObject();
          if (n == nil) {
            n = new unmanaged Node(objType);
            return n;
          }
          var newTop = n.freeListNext;
        } while (!_freeListHead.compareExchangeABA(oldTop, newTop));
        n.next.write(nil);
        n.freeListNext = nil;
        return n;
      }

      proc enqueue(newObj : objType) {
        var n = recycleNode();
        n.val = newObj;

        // Now enqueue
        while (true) {
          var tail = _tail.readABA();
          var next = tail.next.readABA();
          var next_node = next.getObject();
          var curr_tail = _tail.readABA();
          if (tail == curr_tail) {
            if (next_node == nil) {
              if (curr_tail.next.compareExchangeABA(next, n)) {
                _tail.compareExchangeABA(curr_tail, n);
                break;
              }
            }
            else {
              _tail.compareExchangeABA(curr_tail, next_node);
            }
          }
        }
      }

      proc dequeue() : objType {
        while (true) {
          var head = _head.readABA();
          var head_node = head.getObject();
          var curr_tail = _tail.readABA();
          var tail_node = curr_tail.getObject();
          var next = head.next.readABA();
          var next_node = next.getObject();
          var curr_head = _head.readABA();

          if (head == curr_head) {
            if (head_node == tail_node) {
              if (next_node == nil) then
                return nil;
              _tail.compareExchangeABA(curr_tail, next_node);
            }
            else {
              var ret_val = next_node.val;
              if (_head.compareExchangeABA(curr_head, next_node)) {
                retireNode(head_node);
                return ret_val;
              }
            }
          }
        }

        return nil;
      }

      // TODO: Reclaim retired nodes after a while
      proc retireNode(nextObj : unmanaged Node(objType)) {
        nextObj.val = nil;
        do {
          var oldTop = _freeListHead.readABA();
          nextObj.freeListNext = oldTop.getObject();
        } while (!_freeListHead.compareExchangeABA(oldTop, nextObj));
      }

      iter these() : objType {
        var ptr = _head.read().next.read();
        while (ptr != nil) {
          yield ptr.val;
          ptr = ptr.next.read();
        }
      }

      proc peek() : objType {
        var actual_head = _head.read().next.read();
        if (actual_head != nil) then
          return actual_head.val;
        return nil;
      }

      proc deinit() {
        var ptr = _head.read();
        if (delete_val) {
          while (ptr != nil) {
            _head = ptr.next;
            delete ptr.val;
            delete ptr;
            ptr = _head.read();
          }
        } else {
          while (ptr != nil) {
            _head = ptr.next;
            delete ptr;
            ptr = _head.read();
          }
        }

        ptr = _freeListHead.read();
        while (ptr != nil) {
          var head = ptr.freeListNext;
          delete ptr;
          ptr = head;
        }
      }
    }
  }

  pragma "no doc"
  module LimboListModule {

    use AtomicObjects;

    class Node {
      var val : unmanaged object;
      var next : unmanaged Node;

      proc init(val : unmanaged object) {
        this.val = val;
      }
    }

    class LimboList {
      var _head : AtomicObject(unmanaged Node, hasABASupport=true, hasGlobalSupport=true);
      var _freeListHead : AtomicObject(unmanaged Node, hasABASupport=true, hasGlobalSupport=true);

      proc push(obj : unmanaged object) {
        var node = recycleNode(obj);
        var oldHead = _head.exchange(node);
        node.next = oldHead;
      }

      proc recycleNode(obj : unmanaged object) : unmanaged Node {
        var oldTop : ABA(unmanaged Node);
        var n : unmanaged Node;
        do {
          oldTop = _freeListHead.readABA();
          n = oldTop.getObject();
          if (n == nil) {
            n = new unmanaged Node(obj);
            return n;
          }
          var newTop = n.next;
        } while (!_freeListHead.compareExchangeABA(oldTop, newTop));
        n.val = obj;
        return n;
      }

      proc retireNode(nextObj : unmanaged Node) {
        nextObj.val = nil;
        do {
          var oldTop = _freeListHead.readABA();
          nextObj.next = oldTop.getObject();
        } while (!_freeListHead.compareExchangeABA(oldTop, nextObj));
      }

      proc pop() {
        return _head.exchange(nil);
      }

      proc deinit() {
        var ptr = _head.read();
        while (ptr != nil) {
          var next = ptr.next;
          delete ptr.val;
          delete ptr;
          ptr = next;
        }
      }
    }
  }

  pragma "no doc"
  module VectorModule {
    /**
     * Obtained from https://github.com/pnnl/chgl/blob/master/src/Vectors.chpl
     */
    use CyclicDist;
    use BlockDist;

    config param VectorGrowthRate : real = 1.5;

    class Vector {
      type eltType;
      const growthRate : real;
      var dom = {0..-1};
      var arr : [dom] eltType;
      var sz : int;
      var cap : int;
      
      proc init(type eltType, initialSize : integral = 0, growthRate = VectorGrowthRate) {
        this.eltType = eltType;
        this.growthRate = growthRate;
        // Right now 0..#initialSize is bugged if initialSize is 0, it becomes 1..0
        this.dom = {0..initialSize : int - 1};
        this.complete();
        this.cap = dom.size;
      }

      proc init(arr : [?D] ?eltType, growthRate = VectorGrowthRate) {
        this.eltType = eltType;
        this.growthRate = growthRate;
        this.dom = {0..#D.size};
        this.complete();
        this.arr = arr;
        this.cap = arr.size;
        this.sz = arr.size;
      }

      proc append(elt : eltType) {
        if sz == cap {
          var oldCap = cap;
          cap = round(cap * growthRate) : int;
          if oldCap == cap then cap += 1;
          this.dom = {0..#cap};
        }
        
        this.arr[sz] = elt;
        sz += 1;
      }

      proc append(elts : [] eltType) {
        if sz + elts.size >= cap {
          cap = sz + elts.size;
          this.dom = {0..#cap};
        }

        this.arr[sz..#elts.size] = elts;
        sz += elts.size;
      }
      
      proc append(ir : _iteratorRecord) {
        if iteratorToArrayElementType(ir.type) != eltType {
          compilerError(
              "Attempt to append an iterable expression of type '", 
              iteratorToArrayElementType(ir.type) : string, "' when need type '", 
              eltType : string, "'"
          );
        }

        for elt in ir do append(elt);
      }

      proc this(idx : integral) ref {
        return arr[idx];
      }

      iter these() ref {
        for a in this.arr[0..#sz] do yield a;
      }

      iter these(param tag : iterKind) ref where tag == iterKind.standalone {
        forall a in this.arr[0..#sz] do yield a;
      }

      proc size return sz;

      proc clear() {
        this.sz = 0;
      }

      // Returns a reference to the array
      pragma "no copy return"
      proc toArray() {
        return arr[0..#sz];
      }

      proc getArray() {
      var dom = {0..#sz};
      var arr : [dom] eltType = this.arr[0..#sz];
      return arr;
      }

      proc readWriteThis(f) {
        f <~> "(Vector) {" <~> this.toArray() <~> "}";
      }
    }

  }

  private use LockFreeLinkedListModule;
  private use LockFreeQueueModule;
  private use LimboListModule;

  /*
    :class:`LocalEpochManager` manages reclamation of objects, ensuring
    thread-safety.
  */
  class LocalEpochManager {

    //  Total number of epochs
    pragma "no doc"
    const EBR_EPOCHS : uint = 3;

    pragma "no doc"
    const INACTIVE : uint = 0;

    //  Global Epoch is used to synchronize registered tasks' local epochs
    pragma "no doc"
    var global_epoch : atomic uint;

    //  flag to indicate a task is trying to advance global epoch
    pragma "no doc"
    var is_setting_epoch : atomic bool;

    //  List of all tokens
    pragma "no doc"
    var allocated_list : unmanaged LockFreeLinkedList(unmanaged _token);

    //  Collection of inactive tokens, which can be recycled
    pragma "no doc"
    var free_list : unmanaged LockFreeQueue(unmanaged _token);

    //  Collection of objects marked deleted
    pragma "no doc"
    var limbo_list : [1..EBR_EPOCHS] unmanaged LimboList();

    /*
      Default initialize the manager.
    */
    proc init() {
      allocated_list = new unmanaged LockFreeLinkedList(unmanaged _token);
      free_list = new unmanaged LockFreeQueue(unmanaged _token, false);
      this.complete();

      // Initialise the free list pool with here.maxTaskPar tokens
      forall i in 0..#here.maxTaskPar {
        var tok = new unmanaged _token();
        allocated_list.append(tok);
        free_list.enqueue(tok);
      }
      global_epoch.write(1);
      forall i in 1..EBR_EPOCHS do
        limbo_list[i] = new unmanaged LimboList();
    }

    /*
      Register a task.

      :returns: A handle to the manager
    */
    proc register() : owned TokenWrapper { // Should be called only once
      var tok = free_list.dequeue();
      if (tok == nil) {
        tok = new unmanaged _token();
        allocated_list.append(tok);
      }
      tok.is_registered.write(true);
      // return tok;
      return new owned TokenWrapper(tok, this:unmanaged);
    }

    pragma "no doc"
    proc unregister(tok: unmanaged _token) {
      if (tok.is_registered.read()) {
        unpin(tok);
        free_list.enqueue(tok);
        tok.is_registered.write(false);
      }
    }

    pragma "no doc"
    proc pin(tok: unmanaged _token) {
      // An inactive task has local_epoch set to 0. A value other than 0
      // implies active task
      if (tok.local_epoch.read() == INACTIVE) then
        tok.local_epoch.write(global_epoch.read());
    }

    pragma "no doc"
    proc unpin(tok: unmanaged _token) {
      tok.local_epoch.write(INACTIVE);
    }

    // Attempt to announce a new epoch
    pragma "no doc"
    proc tryAdvance() : uint {
      var epoch = global_epoch.read();
      for tok in allocated_list {
        var local_epoch = tok.local_epoch.read();
        if (local_epoch > 0 && local_epoch != epoch) then
          return 0;
      }

      // Advance the global epoch
      epoch = (epoch % EBR_EPOCHS) + 1;
      global_epoch.write(epoch);

      // Return epoch which is safe to be reclaimed. It is safe to
      // reclaim from e-2 epoch
      select epoch {
        when 1 do return EBR_EPOCHS - 1;
        when 2 do return EBR_EPOCHS;
        otherwise do return epoch - 2;
      }
    }

    pragma "no doc"
    proc deferDelete(tok : unmanaged _token, x : unmanaged object) {
      var del_epoch = tok.local_epoch.read();
      if (del_epoch == 0) {
        writeln("Bad local epoch! Please pin! Using global epoch!");
        del_epoch = global_epoch.read();
      }
      limbo_list[del_epoch].push(x);
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc tryReclaim() {
      var count = EBR_EPOCHS;

      // if nothing to reclaim, try the next epoch, but loop only for one
      // full cycle
      while (count) {
        count = count - 1;

        // Set a flag to let other tasks know that a task is already
        // trying to reclaim
        if (is_setting_epoch.testAndSet()) {
          return;
        }
        var reclaim_epoch = tryAdvance();
        if (reclaim_epoch == 0) {
          // tryAdvance failed
          is_setting_epoch.clear();
          return;
        }

        var reclaim_limbo_list = limbo_list[reclaim_epoch];
        var head = reclaim_limbo_list.pop();
        is_setting_epoch.clear();

        while (head != nil) {
          var next = head.next;
          delete head.val;
          reclaim_limbo_list.retireNode(head);
          head = next;
        }
      }
    }

    /*
      Reclaim all objects
    */
    proc deinit() {
      delete allocated_list;
      delete free_list;
      delete limbo_list;
    }
  }

  pragma "no doc"
  class _token {
    var local_epoch : atomic uint;
    var is_registered : atomic bool;

    proc init() {
    }
  }

  /*
    Handle to :class:`LocalEpochManager`
  */
  class TokenWrapper {

    pragma "no doc"
    var _tok : unmanaged _token;

    pragma "no doc"
    var manager : unmanaged LocalEpochManager;

    pragma "no doc"
    proc init(_tok : unmanaged _token, manager : unmanaged LocalEpochManager) {
      this._tok = _tok;
      this.manager = manager;
    }

    /*
      `Pin` a task
    */
    proc pin() {
      manager.pin(this._tok);
    }

    /*
      `Unpin` a task
    */
    proc unpin() {
      manager.unpin(this._tok);
    }

    /*
      Delete an object.

      :arg x: The class instance to be deleted. Must be of unmanaged class type
    */
    proc deferDelete(x) {
      manager.deferDelete(this._tok, x);
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc tryReclaim() {
      manager.tryReclaim();
    }

    /*
      Unregister the handle from the manager
    */
    proc unregister() {
      manager.unregister(this._tok);
      this._tok = nil;
    }

    /*
      Unregister the handle from the manager
    */
    proc deinit() {
      this.unregister();
    }
  }

  private use VectorModule;

  /*
    :record:`EpochManager` manages reclamation of objects, ensuring
    thread-safety. It employs privatization.
  */
  pragma "always RVF"
  record EpochManager {

    pragma "no doc"
    var _pid : int = -1;
    
    /*
      Default initialize with instance of privatized class.
    */
    proc init() {
      this._pid = (new unmanaged EpochManagerImpl()).pid;
    }

    /*
      Reclaim all allocated memory; destroy all privatized objects.
    */
    proc destroy() {
      coforall loc in Locales do on loc {
        delete chpl_getPrivatizedCopy(unmanaged EpochManagerImpl, _pid);
      }
    }

    forwarding chpl_getPrivatizedCopy(unmanaged EpochManagerImpl, _pid);
  }

  /*
    The class which is privatized on each locale for
    :record:`EpochManager`.
  */
  class EpochManagerImpl {

    pragma "no doc"
    var pid : int;


    //  Total number of epochs
    pragma "no doc"
    const EBR_EPOCHS : uint = 3;

    pragma "no doc"
    const INACTIVE : uint = 0;
    
    //  Global Epoch is used to synchronize registered tasks' local epochs
    pragma "no doc"
    var global_epoch : unmanaged GlobalEpoch;
    
    //  Locale Epoch is the copy of Global Epoch on each locale
    pragma "no doc"
    var locale_epoch : atomic uint;

    //  Number of active (pinned) tasks on current locale
    pragma "no doc"
    var active_tasks : atomic uint;

    //  Local flag to indicate a task is trying to advance global epoch
    pragma "no doc"
    var is_setting_epoch : atomic bool;

    //  List of all tokens on current locale
    pragma "no doc"
    var allocated_list : unmanaged LockFreeLinkedList(unmanaged _token);

    //  Collection of inactive tokens, which can be recycled, on current locale
    pragma "no doc"
    var free_list : unmanaged LockFreeQueue(unmanaged _token);

    //  Collection of objects marked deleted on current locale
    pragma "no doc"
    var limbo_list : [1..EBR_EPOCHS] unmanaged LimboList();

    //  Vector for bulk transfer of remote objects marked deleted on current
    //  locale
    pragma "no doc"
    var objsToDelete : [LocaleSpace] unmanaged Vector(unmanaged object);

    //  Initializer for master locale
    pragma "no doc"
    proc init() {
      this.global_epoch = new unmanaged GlobalEpoch(1:uint);
      allocated_list = new unmanaged LockFreeLinkedList(unmanaged _token);
      free_list = new unmanaged LockFreeQueue(unmanaged _token, false);
      this.complete();
      this.pid = _newPrivatizedClass(this);

      // Initialise the free list pool with here.maxTaskPar tokens
      forall i in 0..#here.maxTaskPar {
        var tok = new unmanaged _token();
        allocated_list.append(tok);
        free_list.enqueue(tok);
      }
      locale_epoch.write(global_epoch.read());
      forall i in 1..EBR_EPOCHS do
        limbo_list[i] = new unmanaged LimboList();

      forall i in LocaleSpace do
        objsToDelete[i] = new unmanaged Vector(unmanaged object);
    }


    //  Initializer for slave locales
    pragma "no doc"
    proc init(other, privatizedData, global_epoch) {
      allocated_list = new unmanaged LockFreeLinkedList(unmanaged _token);
      free_list = new unmanaged LockFreeQueue(unmanaged _token, false);
      this.complete();
      this.global_epoch = global_epoch;

      // Initialise the free list pool with here.maxTaskPar tokens
      forall i in 0..#here.maxTaskPar {
        var tok = new unmanaged _token();
        allocated_list.append(tok);
        free_list.enqueue(tok);
      }
      locale_epoch.write(global_epoch.read());
      forall i in 1..EBR_EPOCHS do
        limbo_list[i] = new unmanaged LimboList();
      forall i in LocaleSpace do
        objsToDelete[i] = new unmanaged Vector(unmanaged object);
      this.pid = privatizedData;
    }

    pragma "no doc"
    proc deinit() {
      // Delete locale-private data
      delete limbo_list;
      delete free_list;
      delete allocated_list;
      delete objsToDelete;

      // Delete global data
      if here == Locales[0] { // Is it necessary that global_epoch be on Locales[0]?
        delete global_epoch;
      }
    }

    /*
      Register a task.

      :returns: A handle to the manager
    */
    proc register() : owned DistTokenWrapper { // owned DistTokenWrapper { // Should be called only once
      var tok = free_list.dequeue();
      if (tok == nil) {
        tok = new unmanaged _token();
        allocated_list.append(tok);
      }
      tok.is_registered.write(true);
      // return tok;
      return new owned DistTokenWrapper(tok, this:unmanaged);
    }

    pragma "no doc"
    proc unregister(tok: unmanaged _token) {
      if (tok.is_registered.read()) {
        unpin(tok);
        free_list.enqueue(tok);
        tok.is_registered.write(false);
      }
    }

    pragma "no doc"
    proc pin(tok: unmanaged _token) {
      // An inactive task has local_epoch set to 0. A value other than 0
      // implies active task
      if (tok.local_epoch.read() == INACTIVE) {
        active_tasks.add(1);
        tok.local_epoch.write(locale_epoch.read());
      }
    }

    pragma "no doc"
    proc getMinimumEpoch() : uint {
      if active_tasks.read() > 0 {
        var minEpoch = max(uint);
        for tok in allocated_list {
          var local_epoch = tok.local_epoch.read();
          if (local_epoch > 0) then
            minEpoch = min(minEpoch, local_epoch);
        }

        if (minEpoch != max(uint)) then return minEpoch;
      }

      return 0;
    }

    pragma "no doc"
    proc deferDelete(tok : unmanaged _token, x : unmanaged object) {
      var del_epoch = tok.local_epoch.read();
      if (del_epoch == 0) {
        writeln("Bad local epoch! Please pin! Using global epoch!");
        del_epoch = global_epoch.read();
      }
      limbo_list[del_epoch].push(x);
    }

    // Return epoch which is safe to be reclaimed. It is safe to
    // reclaim from e-2 epoch
    pragma "no doc"
    proc getReclaimEpoch() : uint {
      const epoch = locale_epoch.read();
      select epoch {
        when 1 do return EBR_EPOCHS - 1;
        when 2 do return EBR_EPOCHS;
        otherwise do return epoch - 2;
      }
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc tryReclaim() {
      if (is_setting_epoch.testAndSet()) then return;
      if (global_epoch.is_setting_epoch.testAndSet()) {
        is_setting_epoch.clear();
        return;
      };

      var minEpoch = max(uint);
      coforall loc in Locales with (min reduce minEpoch) do on loc {
        var _this = getPrivatizedInstance();
        var localeMinEpoch = _this.getMinimumEpoch();
        if localeMinEpoch != 0 then
          minEpoch = min(minEpoch, localeMinEpoch);
      }
      const current_global_epoch = global_epoch.read();

      if minEpoch == current_global_epoch || minEpoch == max(uint) {
        const new_epoch = (current_global_epoch % EBR_EPOCHS) + 1;
        global_epoch.write(new_epoch);
        coforall loc in Locales do on loc {
          var _this = getPrivatizedInstance();
          _this.locale_epoch.write(new_epoch);

          const reclaim_epoch = _this.getReclaimEpoch();
          var reclaim_limbo_list = _this.limbo_list[reclaim_epoch];
          var head = reclaim_limbo_list.pop();

          // Prepare work to be scattered by locale it is intended for.
          while (head != nil) {
            var obj = head.val;
            var next = head.next;
            _this.objsToDelete[obj.locale.id].append(obj);
            delete head;
            head = next;
          }
          coforall loc in Locales do on loc {
            // Performs a bulk transfer
            var ourObjs = _this.objsToDelete[here.id].getArray();
            delete ourObjs;
          }
          forall i in LocaleSpace do
            _this.objsToDelete[i].clear();
        }
      }
      global_epoch.is_setting_epoch.clear();
      is_setting_epoch.clear();
    }

    pragma "no doc"
    proc unpin(tok: unmanaged _token) {
      if (tok.local_epoch.read() != INACTIVE) {
        active_tasks.sub(1);
        tok.local_epoch.write(INACTIVE);
      }
    }

    /*
      Destroy all objects. Not thread-safe
    */
    proc clear() {
      coforall loc in Locales do on loc {
        var _this = getPrivatizedInstance();

        // Reset epoch
        if here == global_epoch.locale {
          global_epoch.write(1);
        }
        locale_epoch.write(1);

        for limbo in limbo_list {
          var head = limbo.pop();

          // Prepare work to be scattered by locale it is intended for.
          while (head != nil) {
            var obj = head.val;
            var next = head.next;
            _this.objsToDelete[obj.locale.id].append(obj);
            delete head;
            head = next;
          }
        }
        coforall loc in Locales do on loc {
          // Bulk transfer
          var ourObjs = _this.objsToDelete[here.id].getArray();
          delete ourObjs;
        }
        forall i in LocaleSpace do
          _this.objsToDelete[i].clear();
      }
    }

    pragma "no doc"
    proc dsiPrivatize(privatizedData) {
      return new unmanaged EpochManagerImpl(this, pid, this.global_epoch);
    }

    pragma "no doc"
    proc dsiGetPrivatizeData() {
      return pid;
    }

    pragma "no doc"
    inline proc getPrivatizedInstance() {
      return chpl_getPrivatizedCopy(this.type, pid);
    }
  }

  pragma "no doc"
  class GlobalEpoch {
    var epoch : atomic uint;
    var is_setting_epoch : atomic bool;

    proc init(x : uint) {
      this.complete();
      epoch.write(x);
    }

    forwarding epoch;
  }

  /*
    Handle to :record:`EpochManager`
  */
  class DistTokenWrapper {

    pragma "no doc"
    var _tok : unmanaged _token;

    pragma "no doc"
    var manager : unmanaged EpochManagerImpl;

    pragma "no doc"
    proc init(_tok : unmanaged _token, manager : unmanaged EpochManagerImpl) {
      this._tok = _tok;
      this.manager = manager;
    }

    /*
      `Pin` a task
    */
    proc pin() {
      manager.pin(this._tok);
    }

    /*
      `Unpin` a task
    */
    proc unpin() {
      manager.unpin(this._tok);
    }

    /*
      Delete an object.

      :arg x: The class instance to be deleted. Must be of unmanaged class type
    */
    proc deferDelete(x) {
      manager.deferDelete(this._tok, x);
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc tryReclaim() {
      manager.tryReclaim();
    }

    /*
      Unregister the handle from the manager
    */
    proc unregister() {
      manager.unregister(this._tok);
      this._tok = nil;
    }

    /*
      Unregister the handle from the manager
    */
    proc deinit() {
      this.unregister();
    }
  }
}
