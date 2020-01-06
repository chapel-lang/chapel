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
  An implementation of the Michael & Scott [#]_, a lock-free queue. Concurrent safe
  memory reclamation is handled by an internal :record:`EpochManager`. Usage of the
  queue can be seen below.

  .. code-block:: chpl

    var lfq = new LockFreeQueue(int);
    forall i in 1..N do lfq.enqueue(i);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var (hasElt, elt) = lfq.dequeue();
      while hasElt {
        total += elt;
        (hasElt, elt) = lfq.dequeue();
      }
    }

  As an optimization, the user can register to receive a :class:`~EpochManager.TokenWrapper`, and pass this
  to the stack. This can provide significant improvement in performance by up to an order of magnitude
  by avoiding the overhead of registering and unregistering for each operation.

  .. code-block:: chpl

    var lfq = new LockFreeQueue(int);
    forall i in 1..N with (var tok = lfq.getToken()) do lfq.enqueue(i,tok);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var tok = lfq.getToken();
      var (hasElt, elt) = lfq.dequeue(tok);
      while hasElt {
        total += elt;
        (hasElt, elt) = lfq.dequeue(tok);
      }
    }

  Lastly, to safely reclaim memory, the user must explicitly invoke ``tryReclaim``, or else
  there will be a memory leak. This must be explicitly invoked so that the user may tune how often
  reclamation will be attempted. Reclamation is concurrent-safe, but if called too frequently,
  it can add unnecessary overhead. A complete example of what would be considered 'optimal'
  usage of this lock-free stack.

  .. code-block:: chpl

    var lfq = new LockFreeQueue(int);
    forall i in 1..N with (var tok = lfq.getToken()) do lfq.push(i,tok);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var tok = lfq.getToken();
      var (hasElt, elt) = lfq.dequeue(tok);
      var n : int;
      while hasElt {
        total += elt;
        (hasElt, elt) = lfq.dequeue(tok);
        n += 1;
        if n % GC_THRESHOLD == 0 then lfq.tryReclaim();
      }
    } 

  Also provided, is a utility method for draining the stack of all elements,
  called ``drain``. This iterator will implicitly call ``tryReclaim`` at the
  end and will optimally create one token per task.

  .. code-block:: chpl

    var lfq = new LockFreeQueue(int);
    forall i in 1..N with (var tok = lfq.getToken()) do lfq.enqueue(i,tok);
    var total = + reduce lfq.drain();
  
  .. [#] Michael, Maged M., and Michael L. Scott. 
      Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms. 
      No. TR-600. ROCHESTER UNIV NY DEPT OF COMPUTER SCIENCE, 1995.
*/
module LockFreeQueue {
  use EpochManager;
  use AtomicObjects;

  class Node {
    type eltType;
    var val : eltType?;
    var next : AtomicObject(unmanaged Node(eltType)?, hasGlobalSupport=true, hasABASupport=false);

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
    var _head : AtomicObject(unmanaged Node(objType), hasGlobalSupport=true, hasABASupport=false);
    var _tail : AtomicObject(unmanaged Node(objType), hasGlobalSupport=true, hasABASupport=false);
    var _manager = new owned LocalEpochManager();

    proc init(type objType) {
      this.objType = objType;
      this.complete();
      var _node = new unmanaged Node(objType);
      _head.write(_node);
      _tail.write(_node);
    }

    proc getToken() : owned TokenWrapper {
      return _manager.register();
    }

    proc enqueue(newObj : objType, tok : owned TokenWrapper = getToken()) {
      var n = new unmanaged Node(newObj);
      tok.pin();
      while (true) {
        var curr_tail = _tail.read()!;
        var next = curr_tail.next.read();
        if (next == nil) {
          if (curr_tail.next.compareAndSwap(next, n)) {
            _tail.compareAndSwap(curr_tail, n);
            break;
          }
        }
        else {
          _tail.compareAndSwap(curr_tail, next);
        }
        chpl_task_yield();
      }
      tok.unpin();
    }

    proc dequeue(tok : owned TokenWrapper = getToken()) : (bool, objType?) {
      tok.pin();
      while (true) {
        var curr_head = _head.read()!;
        var curr_tail = _tail.read();
        var next_node = curr_head.next.read();

        if (curr_head == curr_tail) {
          if (next_node == nil) {
            tok.unpin();
            var retval : objType?;
            return (false, retval);
          }
          _tail.compareAndSwap(curr_tail, next_node);
        }
        else {
          var ret_val = next_node!.val;
          if (_head.compareAndSwap(curr_head, next_node)) {
            tok.deferDelete(curr_head);
            tok.unpin();
            return (true, ret_val);
          }
        }
        chpl_task_yield();
      }

      tok.unpin();
      var retval : objType?;
      return (false, retval);
    }

    iter drain() : objType? {
      var tok = getToken();
      var (hasElt, elt) = dequeue(tok);
      while hasElt {
        yield elt;
        (hasElt, elt) = dequeue(tok);
      }
      tryReclaim();
    }

    iter drain(param tag : iterKind) : objType? where tag == iterKind.standalone {
      coforall tid in 1..here.maxTaskPar {
        var tok = getToken();
        var (hasElt, elt) = dequeue(tok);
        while hasElt {
          yield elt;
          (hasElt, elt) = dequeue(tok);
        }
      }
      tryReclaim();
    }

    proc tryReclaim() {
      _manager.tryReclaim();
    }
  }
}
