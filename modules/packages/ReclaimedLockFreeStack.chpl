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
