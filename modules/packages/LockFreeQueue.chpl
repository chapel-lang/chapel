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

module LockFreeQueue {
  use EpochManager;
  use AtomicObjects;

  class Node {
    type eltType;
    var val : eltType;
    var next : AtomicObject(unmanaged Node(eltType), hasGlobalSupport=false, hasABASupport=false);

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
    var _head : AtomicObject(unmanaged Node(objType), hasGlobalSupport=false, hasABASupport=false);
    var _tail : AtomicObject(unmanaged Node(objType), hasGlobalSupport=false, hasABASupport=false);
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
        var curr_tail = _tail.read();
        var next = curr_tail.next.read();
        if (next == nil) {
          if (curr_tail.next.compareExchange(next, n)) {
            _tail.compareExchange(curr_tail, n);
            break;
          }
        }
        else {
          _tail.compareExchange(curr_tail, next);
        }
        chpl_task_yield();
      }
      tok.unpin();
    }

    proc dequeue(tok : owned TokenWrapper = getToken()) : (bool, objType) {
      tok.pin();
      while (true) {
        var curr_head = _head.read();
        var curr_tail = _tail.read();
        var next_node = curr_head.next.read();

        if (curr_head == curr_tail) {
          if (next_node == nil) {
            tok.unpin();
            var retval : objType;
            return (false, retval);
          }
          _tail.compareExchange(curr_tail, next_node);
        }
        else {
          var ret_val = next_node.val;
          if (_head.compareExchange(curr_head, next_node)) {
            tok.deferDelete(curr_head);
            tok.unpin();
            return (true, ret_val);
          }
        }
        chpl_task_yield();
      }

      tok.unpin();
      var retval : objType;
      return (false, retval);
    }

    proc tryReclaim() {
      _manager.tryReclaim();
    }
  }
}
