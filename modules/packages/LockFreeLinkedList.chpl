module LockFreeLinkedList {

  use LocalAtomics;

  class node {
    type eltType;
    var val : eltType;
    var next : unmanaged node(eltType);

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
    var _head : LocalAtomicObject(unmanaged node(objType));

    proc init(type objType) {
      this.objType = objType;
    }

    proc append(newObj : objType) {
      var _node = new unmanaged node(newObj);
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
