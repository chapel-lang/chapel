module LimboList {

  use LocalAtomics;

  class _node {
    var val : unmanaged object;
    var next : unmanaged _node;

    proc init(val : unmanaged object) {
      this.val = val;
    }
  }

  class LimboList {
    var _head : LocalAtomicObject(unmanaged _node);
    var _freeListHead : LocalAtomicObject(unmanaged _node);

    proc push(obj : unmanaged object) {
      var node = recycle_node(obj);
      var oldHead = _head.exchange(node);
      node.next = oldHead;
    }

    proc recycle_node(obj : unmanaged object) : unmanaged _node {
      var oldTop : ABA(unmanaged _node);
      var n : unmanaged _node;
      do {
        oldTop = _freeListHead.readABA();
        n = oldTop.getObject();
        if (n == nil) {
          n = new unmanaged _node(obj);
          return n;
        }
        var newTop = n.next;
      } while (!_freeListHead.compareExchangeABA(oldTop, newTop));
      n.val = obj;
      return n;
    }

    proc retire_node(nextObj : unmanaged _node) {
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

  class C {
    var x : int;
  }

  proc main() {
    var a = new unmanaged LimboList();
    coforall i in 1..4 {
      var b = new unmanaged C(i);
      a.push(b);
    }
    writeln(a);
    var c = a.pop();
    writeln(a);
    writeln(c);
    while (c != nil) {
      var d = c.next;
      a.retire_node(c);
      c = d;
    }
    writeln(a);
    coforall i in 5..8 {
      var b = new unmanaged C(i);
      a.push(b);
    }
    writeln(a);
    c = a.pop();
    writeln(a);
    writeln(c);
    while (c != nil) {
      var d = c.next;
      a.retire_node(c);
      c = d;
    }
    writeln(a);
    delete a;
  }
}
