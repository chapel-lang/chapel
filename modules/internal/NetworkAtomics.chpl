/*
 * Copyright 2004-2018 Cray Inc.
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
  use ChapelStandard;

  private proc externFunc(param s: string, type T) param {
    if isInt(T)  then return "chpl_comm_atomic_" + s + "_int"  + numBits(T):string;
    if isUint(T) then return "chpl_comm_atomic_" + s + "_uint" + numBits(T):string;
    if isReal(T) then return "chpl_comm_atomic_" + s + "_real" + numBits(T):string;
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicBool {
    var _v: int(64);

    inline proc _localeid(): int(32) {
      return _v.locale.id:int(32);
    }

    inline proc _addr(): c_void_ptr {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("read", int(64))
        proc atomic_read(ref result:int(64), l:int(32), const obj:c_void_ptr): void;

      var ret: int(64);
      atomic_read(ret, _localeid(), _addr());
      return ret:bool;
    }

    inline proc write(value:bool, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("write", int(64))
        proc atomic_write(ref desired:int(64), l:int(32), obj:c_void_ptr): void;

      var v = value:int(64);
      atomic_write(v, _localeid(), _addr());
    }

    inline proc exchange(value:bool, order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("xchg", int(64))
        proc atomic_xchg(ref desired:int(64), l:int(32), obj:c_void_ptr, ref result:int(64)): void;

      var ret:int(64);
      var v = value:int(64);
      atomic_xchg(v, _localeid(), _addr(), ret);
      return ret:bool;
    }

    inline proc compareExchange(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    inline proc compareExchangeWeak(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    inline proc compareExchangeStrong(expected:bool, desired:bool, order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", int(64))
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), obj:c_void_ptr, ref result:bool(32)): void;

      var ret:bool(32);
      var te = expected:int(64);
      var td = desired:int(64);
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret);
      return ret:bool;
    }

    inline proc testAndSet(order:memory_order = memory_order_seq_cst): bool {
      return this.exchange(true, order);
    }

    inline proc clear(order:memory_order = memory_order_seq_cst): void {
      this.write(false, order);
    }

    inline proc const waitFor(value:bool, order:memory_order = memory_order_seq_cst): void {
      on this {
        while (this.read(order=memory_order_relaxed) != value) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek(): bool {
      return _v:bool;
    }

    inline proc poke(value:bool): void {
      _v = value:int(64);
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  pragma "atomic type"
  pragma "ignore noinit"
  record RAtomicT {
    type T;
    var _v: T;

    inline proc _localeid(): int(32) {
      return _v.locale.id:int(32);
    }

    inline proc _addr(): c_void_ptr {
      return __primitive("_wide_get_addr", _v);
    }

    inline proc const read(order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("read", T)
        proc atomic_read(ref result:T, l:int(32), const obj:c_void_ptr): void;

      var ret:T;
      atomic_read(ret, _localeid(), _addr());
      return ret;
    }

    inline proc write(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("write", T)
        proc atomic_write(ref desired:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_write(v, _localeid(), _addr());
    }

    inline proc exchange(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("xchg", T)
        proc atomic_xchg(ref desired:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_xchg(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc compareExchange(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    inline proc compareExchangeWeak(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      return this.compareExchangeStrong(expected, desired, order);
    }

    inline proc compareExchangeStrong(expected:T, desired:T, order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("cmpxchg", T)
        proc atomic_cmpxchg(ref expected:T, ref desired:T, l:int(32), obj:c_void_ptr, ref result:bool(32)): void;

      var ret:bool(32);
      var te = expected;
      var td = desired;
      atomic_cmpxchg(te, td, _localeid(), _addr(), ret);
      return ret:bool;
    }

    inline proc fetchAdd(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref op:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_add(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc add(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("add", T)
        proc atomic_add(ref op:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_add(v, _localeid(), _addr());
    }

    inline proc fetchSub(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref op:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_sub(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc sub(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("sub", T)
        proc atomic_sub(ref op:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_sub(v, _localeid(), _addr());
    }

    inline proc fetchOr(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchOr is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref op:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_or(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc or(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("or", T)
        proc atomic_or(ref op:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_or(v, _localeid(), _addr());
    }

    inline proc fetchAnd(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchAnd is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref op:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_and(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc and(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("and", T)
        proc atomic_and(ref op:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_and(v, _localeid(), _addr());
    }

    inline proc fetchXor(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchXor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref op:T, l:int(32), obj:c_void_ptr, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_xor(v, _localeid(), _addr(), ret);
      return ret;
    }

    inline proc xor(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("xor", T)
        proc atomic_xor(ref op:T, l:int(32), obj:c_void_ptr): void;

      var v = value;
      atomic_xor(v, _localeid(), _addr());
    }

    inline proc const waitFor(value:T, order:memory_order = memory_order_seq_cst): void {
      on this {
        while (this.read(order=memory_order_relaxed) != value) {
          chpl_task_yield();
        }
        atomic_thread_fence(order);
      }
    }

    inline proc const peek(): T {
      return _v;
    }

    inline proc poke(value:T): void {
      _v = value;
    }

    proc const writeThis(x) {
      x <~> read();
    }
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
