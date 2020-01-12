/*
 * Copyright 2004-2020 Cray Inc.
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
  An implementation of the Treiber Stack [#]_, a lock-free stack. Concurrent safe
  memory reclamation is handled by an internal :record:`EpochManager`. Usage of the
  stack can be seen below.

  .. code-block:: chpl

    var lfs = new LockFreeStack(int);
    forall i in 1..N do lfs.push(i);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var (hasElt, elt) = lfs.pop();
      while hasElt {
        total += elt;
        (hasElt, elt) = lfs.pop();
      }
    }

  As an optimization, the user can register to receive a :class:`~EpochManager.TokenWrapper`, and pass this
  to the stack. This can provide significant improvement in performance by up to an order of magnitude
  by avoiding the overhead of registering and unregistering for each operation.

  .. code-block:: chpl

    var lfs = new LockFreeStack(int);
    forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var tok = lfs.getToken();
      var (hasElt, elt) = lfs.pop(tok);
      while hasElt {
        total += elt;
        (hasElt, elt) = lfs.pop(tok);
      }
    }

  Lastly, to safely reclaim memory, the user must explicitly invoke ``tryReclaim``, or else
  there will be a memory leak. This must be explicitly invoked so that the user may tune how often
  reclamation will be attempted. Reclamation is concurrent-safe, but if called too frequently,
  it can add unnecessary overhead. A complete example of what would be considered 'optimal'
  usage of this lock-free stack.

  .. code-block:: chpl

    var lfs = new LockFreeStack(int);
    forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
    var total : int;
    coforall tid in 1..here.maxTaskPar with (+ reduce total) {
      var tok = lfs.getToken();
      var (hasElt, elt) = lfs.pop(tok);
      var n : int;
      while hasElt {
        total += elt;
        (hasElt, elt) = lfs.pop(tok);
        n += 1;
        if n % GC_THRESHOLD == 0 then lfs.tryReclaim();
      }
    } 

  Also provided, is a utility method for draining the stack of all elements,
  called ``drain``. This iterator will implicitly call ``tryReclaim`` at the
  end and will optimally create one token per task.

  .. code-block:: chpl

    var lfs = new LockFreeStack(int);
    forall i in 1..N with (var tok = lfs.getToken()) do lfs.push(i,tok);
    var total = + reduce lfs.drain();
  
  .. [#] Hendler, Danny, Nir Shavit, and Lena Yerushalmi. 
      "A scalable lock-free stack algorithm." Proceedings of the sixteenth annual 
      ACM symposium on Parallelism in algorithms and architectures. ACM, 2004.
*/
module LockFreeStack {
  use EpochManager;
  use AtomicObjects;

  class Node {
    type eltType;
    var val : eltType?;
    var next : unmanaged Node(eltType)?;

    proc init(val : ?eltType) {
      this.eltType = eltType;
      this.val = val;
    }

    proc init(type eltType) {
      this.eltType = eltType;
    }
  }

  class LockFreeStack {
    type objType;
    var _top : AtomicObject(unmanaged Node(objType)?, hasGlobalSupport=true, hasABASupport=false);
    var _manager = new owned LocalEpochManager();

    proc init(type objType) {
      this.objType = objType;
    }

    proc getToken() : owned TokenWrapper {
      return _manager.register();
    }

    proc push(newObj : objType, tok : owned TokenWrapper = getToken()) {
      var n = new unmanaged Node(newObj);
      tok.pin();
      var shouldYield = false;
      do {
        var oldTop = _top.read();
        n.next = oldTop;
        if shouldYield then chpl_task_yield();
        shouldYield = true;
      } while (!_top.compareAndSwap(oldTop, n));
      tok.unpin();
    }

    proc pop(tok : owned TokenWrapper = getToken()) : (bool, objType) {
      var oldTop : unmanaged Node(objType)?;
      tok.pin();
      var shouldYield = false;
      do {
        oldTop = _top.read();
        if (oldTop == nil) {
          tok.unpin();
          var retval : objType;
          return (false, retval);
        }
        var newTop = oldTop!.next;
        if shouldYield then chpl_task_yield();
        shouldYield = true;
      } while (!_top.compareAndSwap(oldTop, newTop));
      var retval = oldTop!.val;
      tok.deferDelete(oldTop);
      tok.unpin();
      return (true, retval);
    }

    iter drain() : objType? {
      var tok = getToken();
      var (hasElt, elt) = pop(tok);
      while hasElt {
        yield elt;
        (hasElt, elt) = pop(tok);
      }
      tryReclaim();
    }

    iter drain(param tag : iterKind) : objType? where tag == iterKind.standalone {
      coforall tid in 1..here.maxTaskPar {
        var tok = getToken();
        var (hasElt, elt) = pop(tok);
        while hasElt {
          yield elt;
          (hasElt, elt) = pop(tok);
        }
      }
      tryReclaim();
    }

    proc tryReclaim() {
      _manager.tryReclaim();
    }
  }
}
