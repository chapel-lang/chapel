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

  // int(64)
  pragma "atomic type"
  record RAtomicT {
    type T;
    var _v: T;

    inline proc _localeid: int(32) {
      return this.locale.id:int(32);
    }

    inline proc const read(order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("get", T)
        proc atomic_get(ref result:T, l:int(32), const ref obj:T): void;

      var ret:T;
      atomic_get(ret, _localeid, _v);
      return ret;
    }

    inline proc write(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("put", T)
        proc atomic_put(ref desired:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_put(v, _localeid, _v);
    }

    inline proc exchange(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("xchg", T)
        proc atomic_xchg(ref desired:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_xchg(v, _localeid, _v, ret);
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
        proc atomic_cmpxchg(ref expected:T, ref desired:T, l:int(32), ref obj:T, ref result:bool(32)): void;

      var ret:bool(32);
      var te = expected;
      var td = desired;
      atomic_cmpxchg(te, td, _localeid, _v, ret);
      return ret:bool;
    }

    inline proc fetchAdd(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("fetch_add", T)
        proc atomic_fetch_add(ref op:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_add(v, _localeid, _v, ret);
      return ret;
    }

    inline proc add(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("add", T)
        proc atomic_add(ref op:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_add(v, _localeid, _v);
    }

    inline proc fetchSub(value:T, order:memory_order = memory_order_seq_cst): T {
      pragma "insert line file info" extern externFunc("fetch_sub", T)
        proc atomic_fetch_sub(ref op:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_sub(v, _localeid, _v, ret);
      return ret;
    }

    inline proc sub(value:T, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("sub", T)
        proc atomic_sub(ref op:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_sub(v, _localeid, _v);
    }

    inline proc fetchOr(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchOr is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_or", T)
        proc atomic_fetch_or(ref op:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_or(v, _localeid, _v, ret);
      return ret;
    }

    inline proc or(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("or is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("or", T)
        proc atomic_or(ref op:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_or(v, _localeid, _v);
    }

    inline proc fetchAnd(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchAnd is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_and", T)
        proc atomic_fetch_and(ref op:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_and(v, _localeid, _v, ret);
      return ret;
    }

    inline proc and(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("and is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("and", T)
        proc atomic_and(ref op:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_and(v, _localeid, _v);
    }

    inline proc fetchXor(value:T, order:memory_order = memory_order_seq_cst): T {
      if !isIntegral(T) then compilerError("fetchXor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("fetch_xor", T)
        proc atomic_fetch_xor(ref op:T, l:int(32), ref obj:T, ref result:T): void;

      var ret:T;
      var v = value;
      atomic_fetch_xor(v, _localeid, _v, ret);
      return ret;
    }

    inline proc xor(value:T, order:memory_order = memory_order_seq_cst): void {
      if !isIntegral(T) then compilerError("xor is only defined for integer atomic types");
      pragma "insert line file info" extern externFunc("xor", T)
        proc atomic_xor(ref op:T, l:int(32), ref obj:T): void;

      var v = value;
      atomic_xor(v, _localeid, _v);
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

  inline proc =(ref a:RAtomicT, const b:RAtomicT) {
    a.write(b.read());
  }
  inline proc =(ref a:RAtomicT, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:RAtomicT, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:RAtomicT, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:RAtomicT, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:RAtomicT, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:RAtomicT, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // int(32)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_int32(ref result:int(32),
                                         l:int(32), const ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_put_int32(ref desired:int(32),
                                         l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_add_int32(ref op:int(32),
                                         l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_add_int32(ref op:int(32),
                                               l:int(32), ref obj:int(32),
                                               ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_sub_int32(ref op:int(32),
                                         l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_sub_int32(ref op:int(32),
                                               l:int(32), ref obj:int(32),
                                               ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_and_int32(ref op:int(32),
                                         l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_and_int32(ref op:int(32),
                                               l:int(32), ref obj:int(32),
                                               ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_or_int32(ref op:int(32),
                                        l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_or_int32(ref op:int(32),
                                              l:int(32), ref obj:int(32),
                                              ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xor_int32(ref op:int(32),
                                         l:int(32), ref obj:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_xor_int32(ref op:int(32),
                                               l:int(32), ref obj:int(32),
                                               ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xchg_int32(ref desired:int(32),
                                          l:int(32), ref obj:int(32),
                                          ref result:int(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_cmpxchg_int32(ref expected:int(32),
                                             ref desired:int(32),
                                             l:int(32), ref obj:int(32),
                                             ref result:bool(32));

  // int32
  pragma "atomic type"
  record ratomic_int32 {
    var _v: int(32);
    inline proc const read(order:memory_order = memory_order_seq_cst):int(32) {
      var ret: int(32);
      chpl_comm_atomic_get_int32(ret, this.locale.id:int(32), this._v);
      return ret;
    }
    inline proc write(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_put_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var ret:int(32);
      var v = value;
      chpl_comm_atomic_xchg_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchange(expected:int(32), desired:int(32),
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_int32(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchangeWeak(expected:int(32), desired:int(32),
                                    order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:int(32), desired:int(32),
                                      order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var v = value;
      var ret:int(32);
      chpl_comm_atomic_fetch_add_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc add(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_add_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchSub(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var v = value;
      var ret:int(32);
      chpl_comm_atomic_fetch_sub_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc sub(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_sub_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchOr(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var v = value;
      var ret:int(32);
      chpl_comm_atomic_fetch_or_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc or(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_or_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchAnd(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var v = value;
      var ret:int(32);
      chpl_comm_atomic_fetch_and_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc and(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_and_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchXor(value:int(32), order:memory_order = memory_order_seq_cst):int(32) {
      var v = value;
      var ret:int(32);
      chpl_comm_atomic_fetch_xor_int32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc xor(value:int(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_xor_int32(v, this.locale.id:int(32), this._v);
    }

    inline proc const waitFor(val:int(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():int(32) {
      return _v;
    }
    inline proc const poke(value:int(32)) {
      _v = value;
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  inline proc =(ref a:ratomic_int32, const b:ratomic_int32) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomic_int32, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomic_int32, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomic_int32, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomic_int32, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomic_int32, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomic_int32, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // uint(64)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_uint64(ref result:uint(64),
                                         l:int(32), const ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_put_uint64(ref desired:uint(64),
                                         l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_add_uint64(ref op:uint(64),
                                         l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_add_uint64(ref op:uint(64),
                                               l:int(32), ref obj:uint(64),
                                               ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_sub_uint64(ref op:uint(64),
                                         l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_sub_uint64(ref op:uint(64),
                                               l:int(32), ref obj:uint(64),
                                               ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_and_uint64(ref op:uint(64),
                                         l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_and_uint64(ref op:uint(64),
                                               l:int(32), ref obj:uint(64),
                                               ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_or_uint64(ref op:uint(64),
                                        l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_or_uint64(ref op:uint(64),
                                              l:int(32), ref obj:uint(64),
                                              ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xor_uint64(ref op:uint(64),
                                         l:int(32), ref obj:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_xor_uint64(ref op:uint(64),
                                               l:int(32), ref obj:uint(64),
                                               ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xchg_uint64(ref desired:uint(64),
                                          l:int(32), ref obj:uint(64),
                                          ref result:uint(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_cmpxchg_uint64(ref expected:uint(64),
                                             ref desired:uint(64),
                                             l:int(32), ref obj:uint(64),
                                             ref result:bool(32));

  // uint(64)
  pragma "atomic type"
  record ratomic_uint64 {
    var _v: uint(64);
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(64) {
      var ret: uint(64);
      chpl_comm_atomic_get_uint64(ret, this.locale.id:int(32), this._v);
      return ret;
    }
    inline proc write(value:uint(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_put_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var ret:uint(64);
      var v = value;
      chpl_comm_atomic_xchg_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchange(expected:uint(64), desired:uint(64),
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_uint64(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchangeWeak(expected:uint(64), desired:uint(64),
                                    order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:uint(64), desired:uint(64),
                                      order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var v = value;
      var ret:uint(64);
      chpl_comm_atomic_fetch_add_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc add(value:uint(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_add_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchSub(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var v = value;
      var ret:uint(64);
      chpl_comm_atomic_fetch_sub_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc sub(value:uint(64), order:memory_order = memory_order_seq_cst){
      var v = value;
      chpl_comm_atomic_sub_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchOr(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var v = value;
      var ret:uint(64);
      chpl_comm_atomic_fetch_or_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc or(value:uint(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_or_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchAnd(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var v = value;
      var ret:uint(64);
      chpl_comm_atomic_fetch_and_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc and(value:uint(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_and_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchXor(value:uint(64), order:memory_order = memory_order_seq_cst):uint(64) {
      var v = value;
      var ret:uint(64);
      chpl_comm_atomic_fetch_xor_uint64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc xor(value:uint(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_xor_uint64(v, this.locale.id:int(32), this._v);
    }

    inline proc const waitFor(val:uint(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc const peek():uint(64) {
      return _v;
    }
    inline proc poke(value:uint(64)) {
      _v = value;
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  inline proc =(ref a:ratomic_uint64, const b:ratomic_uint64) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomic_uint64, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomic_uint64, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomic_uint64, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomic_uint64, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomic_uint64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomic_uint64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // uint(32)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_uint32(ref result:uint(32),
                                         l:int(32), const ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_put_uint32(ref desired:uint(32),
                                         l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_add_uint32(ref op:uint(32),
                                         l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_add_uint32(ref op:uint(32),
                                               l:int(32), ref obj:uint(32),
                                               ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_sub_uint32(ref op:uint(32),
                                         l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_sub_uint32(ref op:uint(32),
                                               l:int(32), ref obj:uint(32),
                                               ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_and_uint32(ref op:uint(32),
                                         l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_and_uint32(ref op:uint(32),
                                               l:int(32), ref obj:uint(32),
                                               ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_or_uint32(ref op:uint(32),
                                        l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_or_uint32(ref op:uint(32),
                                              l:int(32), ref obj:uint(32),
                                              ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xor_uint32(ref op:uint(32),
                                         l:int(32), ref obj:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_xor_uint32(ref op:uint(32),
                                               l:int(32), ref obj:uint(32),
                                               ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xchg_uint32(ref desired:uint(32),
                                          l:int(32), ref obj:uint(32),
                                          ref result:uint(32));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_cmpxchg_uint32(ref expected:uint(32),
                                             ref desired:uint(32),
                                             l:int(32), ref obj:uint(32),
                                             ref result:bool(32));

  // uint(32)
  pragma "atomic type"
  record ratomic_uint32 {
    var _v: uint(32);
    inline proc const read(order:memory_order = memory_order_seq_cst):uint(32) {
      var ret: uint(32);
      chpl_comm_atomic_get_uint32(ret, this.locale.id:int(32), this._v);
      return ret;
    }
    inline proc write(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_put_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var ret:uint(32);
      var v = value;
      chpl_comm_atomic_xchg_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchange(expected:uint(32), desired:uint(32),
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_uint32(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchangeWeak(expected:uint(32), desired:uint(32),
                                    order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:uint(32), desired:uint(32),
                                      order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var v = value;
      var ret:uint(32);
      chpl_comm_atomic_fetch_add_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc add(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_add_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchSub(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var v = value;
      var ret:uint(32);
      chpl_comm_atomic_fetch_sub_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc sub(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_sub_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchOr(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var v = value;
      var ret:uint(32);
      chpl_comm_atomic_fetch_or_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc or(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_or_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchAnd(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var v = value;
      var ret:uint(32);
      chpl_comm_atomic_fetch_and_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc and(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_and_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchXor(value:uint(32), order:memory_order = memory_order_seq_cst):uint(32) {
      var v = value;
      var ret:uint(32);
      chpl_comm_atomic_fetch_xor_uint32(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc xor(value:uint(32), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_xor_uint32(v, this.locale.id:int(32), this._v);
    }

    inline proc const waitFor(val:uint(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc const peek():uint(32) {
      return _v;
    }
    inline proc poke(value:uint(32)) {
      _v = value;
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  inline proc =(ref a:ratomic_uint32, const b:ratomic_uint32) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomic_uint32, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomic_uint32, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomic_uint32, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomic_uint32, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomic_uint32, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomic_uint32, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // bool, implemented with int(64)
  pragma "atomic type"
  record RAtomicBool {
    var _v: int(64);

    inline proc _localeid: int(32) {
      return this.locale.id:int(32);
    }

    inline proc const read(order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("get", int(64))
        proc atomic_get(ref result:int(64), l:int(32), const ref obj:int(64)): void;

      var ret: int(64);
      atomic_get(ret, _localeid, _v);
      return ret:bool;
    }

    inline proc write(value:bool, order:memory_order = memory_order_seq_cst): void {
      pragma "insert line file info" extern externFunc("put", int(64))
        proc atomic_put(ref desired:int(64), l:int(32), ref obj:int(64)): void;

      var v = value:int(64);
      atomic_put(v, _localeid, _v);
    }

    inline proc exchange(value:bool, order:memory_order = memory_order_seq_cst): bool {
      pragma "insert line file info" extern externFunc("xchg", int(64))
        proc atomic_xchg(ref desired:int(64), l:int(32), ref obj:int(64), ref result:int(64)): void;

      var ret:int(64);
      var v = value:int(64);
      atomic_xchg(v, _localeid, _v, ret);
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
        proc atomic_cmpxchg(ref expected:int(64), ref desired:int(64), l:int(32), ref obj:int(64), ref result:bool(32)): void;

      var ret:bool(32);
      var te = expected:int(64);
      var td = desired:int(64);
      atomic_cmpxchg(te, td, _localeid, _v, ret);
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

  inline proc =(ref a:RAtomicBool, const b:RAtomicBool) {
    a.write(b.read());
  }
  inline proc =(ref a:RAtomicBool, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:RAtomicBool, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:RAtomicBool, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:RAtomicBool, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:RAtomicBool, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:RAtomicBool, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // real(64)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_real64(ref result:real(64),
                                          l:int(32), const ref obj:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_put_real64(ref desired:real(64),
                                          l:int(32), ref obj:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_add_real64(ref op:real(64),
                                          l:int(32), ref obj:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_add_real64(ref op:real(64),
                                                l:int(32), ref obj:real(64),
                                                ref result:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_sub_real64(ref op:real(64),
                                          l:int(32), ref obj:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_sub_real64(ref op:real(64),
                                                l:int(32), ref obj:real(64),
                                                ref result:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xchg_real64(ref desired:real(64),
                                           l:int(32), ref obj:real(64),
                                           ref result:real(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_cmpxchg_real64(ref expected:real(64),
                                              ref desired:real(64),
                                              l:int(32), ref obj:real(64),
                                              ref result:bool(32));
  
  pragma "atomic type"
  record ratomic_real64 {
    var _v: real(64);
    inline proc const read(order:memory_order = memory_order_seq_cst):real(64) {
      var ret: real(64);
      chpl_comm_atomic_get_real64(ret, this.locale.id:int(32), this._v);
      return ret;
    }
    inline proc write(value:real(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_put_real64(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var ret:real(64);
      var v = value;
      chpl_comm_atomic_xchg_real64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchange(expected:real(64), desired:real(64),
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_real64(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchangeWeak(expected:real(64), desired:real(64),
                                    order:memory_order = memory_order_seq_cst):bool {
      return compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:real(64), desired:real(64),
                                      order:memory_order = memory_order_seq_cst):bool {
      return compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var v = value;
      var ret:real(64);
      chpl_comm_atomic_fetch_add_real64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc add(value:real(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_add_real64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchSub(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      var v = value;
      var ret:real(64);
      chpl_comm_atomic_fetch_sub_real64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc sub(value:real(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_sub_real64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchOr(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      compilerError("or not defined for network atomic real");
    }
    inline proc or(value:real(64), order:memory_order = memory_order_seq_cst) {
      compilerError("or not defined for network atomic real");
    }

    inline proc fetchAnd(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      compilerError("and not defined for network atomic real");
    }
    inline proc and(value:real(64), order:memory_order = memory_order_seq_cst) {
      compilerError("and not defined for network atomic real");
    }

    inline proc fetchXor(value:real(64), order:memory_order = memory_order_seq_cst):real(64) {
      compilerError("xor not defined for network atomic real");
    }
    inline proc xor(value:real(64), order:memory_order = memory_order_seq_cst) {
      compilerError("xor not defined for network atomic real");
    }


    inline proc const waitFor(val:real(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc const peek():real(64) {
      return _v;
    }
    inline proc poke(value:real(64)) {
      _v = value;
    }

    proc const writeThis(x) {
      x <~> read();
    }
  }

  inline proc =(ref a:ratomic_real64, const b:ratomic_real64) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomic_real64, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomic_real64, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomic_real64, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomic_real64, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomic_real64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomic_real64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }

}

