/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
  private use CTypes;

  private proc externFunc(param s: string, type valType) param {
    if isInt(valType)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(valType):string;
    if isUint(valType) then return "chpl_comm_atomic_" + s + "_uint" + numBits(valType):string;
    if isReal(valType) then return "chpl_comm_atomic_" + s + "_real" + numBits(valType):string;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicBool : writeSerializable {
    proc type valType type { return bool; }
    proc valType type { return bool; }

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

    inline proc _addr(): c_ptr(void) {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("read", int(64))
        proc atomic_read(ref result:int(64), l:int(32), const obj:c_ptr(void), order:memory_order): void;

      var ret: int(64);
      atomic_read(ret, _localeid(), _addr(), c_memory_order(order));
      return ret:bool;
    }

    inline proc write(val:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("write", int(64))
        proc atomic_write(ref desired:int(64), l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val:int(64);
      atomic_write(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc exchange(val:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("xchg", int(64))
        proc atomic_xchg(ref desired:int(64), l:int(32), obj:c_ptr(void), ref result:int(64), order:memory_order): void;

      var ret:int(64);
      var v = val:int(64);
      atomic_xchg(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret:bool;
    }

    inline proc compareExchange(ref expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc compareExchange(ref expected:bool, desired:bool, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", int(64))
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), obj:c_ptr(void), ref result:uint(32), succ:memory_order, fail:memory_order): void;

      var ret:uint(32);
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

    @unstable("'compareAndSwap' is unstable")
    inline proc compareAndSwap(expected:bool, desired:bool, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", int(64))
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), obj:c_ptr(void), ref result:uint(32), succ:memory_order, fail:memory_order): void;

      var ret:uint(32);
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

    inline proc const waitFor(val:bool, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != val) {
          currentTask.yieldExecution();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    proc const serialize(writer, ref serializer) throws {
      writer.write(read());
    }

  }

  pragma "do not resolve unless called"
  operator :(rhs: bool, type t:RAtomicBool) {
    var lhs: RAtomicBool = rhs; // use init=
    return lhs;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicT : writeSerializable {
    type valType;
    var _v: valType;

    proc init=(other:this.type) {
      this.valType = other.valType;
      this._v = other.read();
    }

    proc init=(other:this.type.valType) {
      this.valType = other.type;
      this._v = other;
    }

    inline proc _localeid(): int(32) {
      return _v.locale.id:int(32);
    }

    inline proc _addr(): c_ptr(void) {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "insert line file info" extern externFunc("read", valType)
        proc atomic_read(ref result:valType, l:int(32), const obj:c_ptr(void), order:memory_order): void;

      var ret:valType;
      atomic_read(ret, _localeid(), _addr(), c_memory_order(order));
      return ret;
    }

    inline proc write(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("write", valType)
        proc atomic_write(ref desired:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_write(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc exchange(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "insert line file info" extern externFunc("xchg", valType)
        proc atomic_xchg(ref desired:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_xchg(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc compareExchange(ref expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order, readableOrder(order));
    }
    inline proc compareExchange(ref expected:valType, desired:valType, param success: memoryOrder, param failure: memoryOrder): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", valType)
        proc atomic_cmpxchg(ref expected:valType, ref desired:valType, l:int(32), obj:c_ptr(void), ref result:uint(32), succ:memory_order, fail:memory_order): void;

      var te = expected;
      var ret:uint(32);
      var td = desired;
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(success), c_memory_order(failure));
      if !ret then expected = te;
      return ret:bool;
    }

    inline proc compareExchangeWeak(ref expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      return this.compareExchange(expected, desired, order);
    }
    inline proc compareExchangeWeak(ref expected:valType, desired:valType, param success: memoryOrder, param failure: memoryOrder): bool {
      return this.compareExchange(expected, desired, success, failure);
    }

    @unstable("'compareAndSwap' is unstable")
    inline proc compareAndSwap(expected:valType, desired:valType, param order: memoryOrder = memoryOrder.seqCst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", valType)
        proc atomic_cmpxchg(ref expected:valType, ref desired:valType, l:int(32), obj:c_ptr(void), ref result:uint(32), succ:memory_order, fail:memory_order): void;

      var ret:uint(32);
      var te = expected;
      var td = desired;
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret, c_memory_order(order), c_memory_order(readableOrder(order)));
      return ret:bool;
    }

    inline proc fetchAdd(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "insert line file info" extern externFunc("fetch_add", valType)
        proc atomic_fetch_add(ref op:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_fetch_add(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc add(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("add", valType)
        proc atomic_add(ref op:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_add(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchSub(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      pragma "insert line file info" extern externFunc("fetch_sub", valType)
        proc atomic_fetch_sub(ref op:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_fetch_sub(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc sub(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      pragma "insert line file info" extern externFunc("sub", valType)
        proc atomic_sub(ref op:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_sub(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchOr(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchOr is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_or", valType)
        proc atomic_fetch_or(ref op:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_fetch_or(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc or(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("or is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("or", valType)
        proc atomic_or(ref op:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_or(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchAnd(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchAnd is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_and", valType)
        proc atomic_fetch_and(ref op:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_fetch_and(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc and(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("and is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("and", valType)
        proc atomic_and(ref op:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_and(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchXor(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      if !isIntegral(valType) then compilerError("fetchXor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_xor", valType)
        proc atomic_fetch_xor(ref op:valType, l:int(32), obj:c_ptr(void), ref result:valType, order:memory_order): void;

      var ret:valType;
      var v = val;
      atomic_fetch_xor(v, _localeid(), _addr(), ret, c_memory_order(order));
      return ret;
    }

    inline proc xor(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      if !isIntegral(valType) then compilerError("xor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("xor", valType)
        proc atomic_xor(ref op:valType, l:int(32), obj:c_ptr(void), order:memory_order): void;

      var v = val;
      atomic_xor(v, _localeid(), _addr(), c_memory_order(order));
    }

    inline proc fetchMin(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      compilerError("fetchMin is not supported by CHPL_NETWORK_ATOMICS=ugni");
    }

    inline proc min(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      compilerError("min is not supported by CHPL_NETWORK_ATOMICS=ugni");
    }

    inline proc fetchMax(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      compilerError("fetchMax is not supported by CHPL_NETWORK_ATOMICS=ugni");
    }

    inline proc max(val:valType, param order: memoryOrder = memoryOrder.seqCst): valType {
      compilerError("max is not supported by CHPL_NETWORK_ATOMICS=ugni");
    }

    inline proc const waitFor(val:valType, param order: memoryOrder = memoryOrder.seqCst): void {
      on this {
        while (this.read(order=memoryOrder.relaxed) != val) {
          currentTask.yieldExecution();
        }
        chpl_atomic_thread_fence(c_memory_order(order));
      }
    }

    proc const serialize(writer, ref serializer) throws {
      writer.write(read());
    }

  }

  operator :(rhs, type t:RAtomicT)
  where rhs.type == t.valType {
    var lhs: t = rhs; // use init=
    return lhs;
  }

  inline operator RAtomicBool.=(ref a:RAtomicBool, const b:RAtomicBool) {
    a.write(b.read());
  }
  inline operator RAtomicBool.=(ref a:RAtomicBool, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline operator RAtomicT.=(ref a:RAtomicT, const b:RAtomicT) {
    a.write(b.read());
  }
  inline operator RAtomicT.=(ref a:RAtomicT, b) {
    compilerError("Cannot directly assign atomic variables");
  }
  inline operator RAtomicT.+(a:RAtomicT, b) {
    compilerError("Cannot directly add atomic variables");
    return a;
  }
  inline operator RAtomicT.-(a:RAtomicT, b) {
    compilerError("Cannot directly subtract atomic variables");
    return a;
  }
  inline operator RAtomicT.*(a:RAtomicT, b) {
    compilerError("Cannot directly multiply atomic variables");
    return a;
  }
  inline operator RAtomicT./(a:RAtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
  inline operator RAtomicT.%(a:RAtomicT, b) {
    compilerError("Cannot directly divide atomic variables");
    return a;
  }
}
