/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

pragma "atomic module"
module NetworkAtomics {
  private use ChapelStandard;
  private use MemConsistency;
  private use CPtr;

  private proc externFunc(param s: string, type T) param {
    if isInt(T)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(T):string;
    if isUint(T) then return "chpl_comm_atomic_" + s + "_uint" + numBits(T):string;
    if isReal(T) then return "chpl_comm_atomic_" + s + "_real" + numBits(T):string;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicBool {
    var _v: int(64);

    proc init=(other:RAtomicBool) {
      this._v = other.read():int(64);
    }

    proc init=(other:bool) {
      this._v = other:int(64);
    }

    inline proc _localeid(): int(32) {
      return _v.locale.id:int(32);
    }

    inline proc _addr(): c_void_ptr {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("read", int(64))
        proc atomic_read(ref result:int(64), l:int(32), const obj:c_void_ptr, order:memory_order): void;

      var ret: int(64);
      atomic_read(ret, _localeid(), _addr(), c_memory_order(order));
      return ret:bool;
    }

    inline proc write(value:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("write", int(64))
        proc atomic_write(ref desired:int(64), l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value:int(64);
      atomic_write(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc exchange(value:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("xchg", int(64))
        proc atomic_xchg(ref desired:int(64), l:int(32), obj:c_void_ptr, ref result:int(64), order:memory_order): void;

      var ret:int(64);
      var v = value:int(64);
      atomic_xchg(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret:bool;
    }

    inline proc compareExchange(ref expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc compareExchange(ref expected:bool, desired:bool, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", int(64))
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), obj:c_void_ptr, ref result:bool(32), succ:memory_order, fail:memory_order): void;

      var ret:bool(32);
      var te = expected:int(64);
      var td = desired:int(64);
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(success), c_memory_order(failure));
      if !ret then expected = te:bool;
      return ret:bool;
    }

    inline proc compareExchangeWeak(ref expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order);
    }
    inline proc compareExchangeWeak(ref expected:bool, desired:bool, param success: memoryOrder, param failure: memoryOrder): bool {
      return this.compareExchange(expected, desired, success, failure);
    }

    inline proc compareAndSwap(expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", int(64))
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), obj:c_void_ptr, ref result:bool(32), succ:memory_order, fail:memory_order): void;

      var ret:bool(32);
      var te = expected:int(64);
      var td = desired:int(64);
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(order), c_memory_order(readableOrder(order)));
      return ret:bool;
    }

    inline proc testAndSet(param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.exchange(true, order);
    }

    inline proc clear(param order: memoryOrder = memoryOrder.seqCst): void {
      this.write(false, order);
    }

    inline proc const waitFor(value:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != value) {
          chpl_task_yield();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    proc const writeThis(x) throws {
      x <~> read();
    }

  }

  operator :(rhs: bool, type t:RAtomicBool) {
    var lhs: RAtomicBool = rhs; // use init=
    return lhs;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicT {
    type T;
    var _v: T;

    proc init=(other:this.type) {
      this.T = other.T;
      this._v = other.read();
    }

    proc init=(other:this.type.T) {
      this.T = other.type;
      this._v = other;
    }

    inline proc _localeid(): int(32) {
      return _v.locale.id:int(32);
    }

    inline proc _addr(): c_void_ptr {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): T {
      pragma "insert line file info" extern externFunc("read", T)
        proc atomic_read(ref result:T, l:int(32), const obj:c_void_ptr, order:memory_order): void;

      var ret:T;
      atomic_read(ret, _localeid(), _addr(), c_memory_order(order));
      return ret;
    }

    inline proc write(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("write", T)
        proc atomic_write(ref desired:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_write(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc exchange(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      pragma "insert line file info" extern externFunc("xchg", T)
        proc atomic_xchg(ref desired:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_xchg(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc compareExchange(ref expected:T, desired:T, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc compareExchange(ref expected:T, desired:T, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", T)
        proc atomic_cmpxchg(ref expected:T, ref desired:T, l:int(32), obj:c_void_ptr, ref result:bool(32), succ:memory_order, fail:memory_order): void;

      var te = expected;
      var ret:bool(32);
      var td = desired;
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(success), c_memory_order(failure));
      if !ret then expected = te;
      return ret:bool;
    }

    inline proc compareExchangeWeak(ref expected:T, desired:T, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order);
    }
    inline proc compareExchangeWeak(ref expected:T, desired:T, param success: memoryOrder, param failure: memoryOrder): bool {
      return this.compareExchange(expected, desired, success, failure);
    }

    inline proc compareAndSwap(expected:T, desired:T, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", T)
        proc atomic_cmpxchg(ref expected:T, ref desired:T, l:int(32), obj:c_void_ptr, ref result:bool(32), succ:memory_order, fail:memory_order): void;

      var ret:bool(32);
      var te = expected;
      var td = desired;
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(order), c_memory_order(readableOrder(order)));
      return ret:bool;
    }

    inline proc fetchAdd(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      pragma "insert line file info" extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref op:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_fetch_add(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc add(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("add", T)
        proc atomic_add(ref op:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_add(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchSub(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      pragma "insert line file info" extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref op:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_fetch_sub(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc sub(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("sub", T)
        proc atomic_sub(ref op:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_sub(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchOr(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchOr is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref op:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_fetch_or(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc or(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("or", T)
        proc atomic_or(ref op:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_or(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchAnd(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchAnd is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref op:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_fetch_and(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc and(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("and", T)
        proc atomic_and(ref op:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_and(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchXor(value:T, param order: memoryOrder = memoryOrder.seqCst): T {
      if !isIntegral(T) then compilerError("fetchXor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref op:T, l:int(32), obj:c_void_ptr, ref result:T, order:memory_order): void;

      var ret:T;
      var v = value;
      atomic_fetch_xor(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc xor(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("xor", T)
        proc atomic_xor(ref op:T, l:int(32), obj:c_void_ptr, order:memory_order): void;

      var v = value;
      atomic_xor(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc const waitFor(value:T, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != value) {
          chpl_task_yield();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    proc const writeThis(x) throws {
      x <~> read();
    }

  }

  operator :(rhs, type t:RAtomicT)
  where rhs.type == t.T {
    var lhs: t = rhs; // use init=
    return lhs;
  }

  inline proc =(ref a:RAtomicBool, const b:RAtomicBool) {
    a.write(b.read());
  }
  inline proc =(ref a:RAtomicBool, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc =(ref a:RAtomicT, const b:RAtomicT) {
    a.write(b.read());
  }
  inline proc =(ref a:RAtomicT, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline proc +(a:RAtomicT, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline proc -(a:RAtomicT, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline proc *(a:RAtomicT, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline proc /(a:RAtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline proc %(a:RAtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
}
