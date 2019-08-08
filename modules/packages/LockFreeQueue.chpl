/**
 * Based on Michael Scott Queue.
 */
module LockFreeQueue {

  use LocalAtomics;

  class node {
    type eltType;
    var val : eltType;
    var next : LocalAtomicObject(unmanaged node(eltType));
    var freeListNext : unmanaged node(eltType);

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
    var _head : LocalAtomicObject(unmanaged node(objType));
    var _tail : LocalAtomicObject(unmanaged node(objType));
    var _freeListHead : LocalAtomicObject(unmanaged node(objType));
    // Flag to set if objects held in the queue are to be deleted or not.
    // By default initialised to true.
    const delete_val : bool;

    proc init(type objType) {
      this.objType = objType;
      delete_val = true;
      this.complete();
      var _node = new unmanaged node(objType);
      _head.write(_node);
      _tail.write(_node);
    }

    proc init(type objType, delete_val : bool) {
      this.objType = objType;
      this.delete_val = delete_val;
      this.complete();
      var _node = new unmanaged node(objType);
      _head.write(_node);
      _tail.write(_node);
    }

    proc recycle_node() : unmanaged node(objType) {
      var oldTop : ABA(unmanaged node(objType));
      var n : unmanaged node(objType);
      do {
        oldTop = _freeListHead.readABA();
        n = oldTop.getObject();
        if (n == nil) {
          n = new unmanaged node(objType);
          return n;
        }
        var newTop = n.freeListNext;
      } while (!_freeListHead.compareExchangeABA(oldTop, newTop));
      n.next.write(nil);
      n.freeListNext = nil;
      return n;
    }

    proc enqueue(newObj : objType) {
      var n = recycle_node();
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
              retire_node(head_node);
              return ret_val;
            }
          }
        }
      }

      return nil;
    }

    // TODO: Reclaim retired nodes after a while
    proc retire_node(nextObj : unmanaged node(objType)) {
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
