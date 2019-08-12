module ReclaimedLockFreeStack {
  use EpochManager;
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

  class ReclaimedLockFreeStack {
    type objType;
    var _top : LocalAtomicObject(unmanaged node(objType));
    var _manager = new owned EpochManager();

    proc init(type objType) {
      this.objType = objType;
    }

    proc getToken() {
      return _manager.register();
    }

    proc push(newObj : objType, tok) {
      var n = new unmanaged node(newObj);
      tok.pin();
      do {
        var oldTop = _top.read();
        n.next = oldTop;
      } while (!_top.compareExchange(oldTop, n));
      tok.unpin();
    }

    proc pop(tok) : (bool, objType) {
      var oldTop : unmanaged node(objType);
      tok.pin();
      do {
        oldTop = _top.read();
        if (oldTop == nil) {
          tok.unpin();
          var retval : objType;
          return (false, retval);
        }
        var newTop = oldTop.next;
      } while (!_top.compareExchange(oldTop, newTop));
      var retval = oldTop.val;
      tok.delete_obj(oldTop);
      tok.unpin();
      return (true, retval);
    }

    proc try_reclaim() {
      _manager.try_reclaim();
    }
  }
}
