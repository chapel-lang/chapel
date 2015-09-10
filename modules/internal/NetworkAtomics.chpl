/*
 * Copyright 2004-2015 Cray Inc.
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

  // int(64)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_int64(ref result:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_put_int64(ref desired:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_add_int64(ref op:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_add_int64(ref op:int(64),
                                               l:int(32), ref obj:int(64),
                                               ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_sub_int64(ref op:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_sub_int64(ref op:int(64),
                                               l:int(32), ref obj:int(64),
                                               ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_and_int64(ref op:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_and_int64(ref op:int(64),
                                               l:int(32), ref obj:int(64),
                                               ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_or_int64(ref op:int(64),
                                        l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_or_int64(ref op:int(64),
                                              l:int(32), ref obj:int(64),
                                              ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xor_int64(ref op:int(64),
                                         l:int(32), ref obj:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_fetch_xor_int64(ref op:int(64),
                                               l:int(32), ref obj:int(64),
                                               ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_xchg_int64(ref desired:int(64),
                                          l:int(32), ref obj:int(64),
                                          ref result:int(64));
  pragma "insert line file info"
  extern proc chpl_comm_atomic_cmpxchg_int64(ref expected:int(64),
                                             ref desired:int(64),
                                             l:int(32), ref obj:int(64),
                                             ref result:bool(32));

  // int(64)
  pragma "atomic type"
  record ratomic_int64 {
    var _v: int(64);
    inline proc read(order:memory_order = memory_order_seq_cst):int(64) {
      var ret: int(64);
      chpl_comm_atomic_get_int64(ret, this.locale.id:int(32), this._v);
      return ret;
    }
    inline proc write(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_put_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var ret:int(64);
      var v = value;
      chpl_comm_atomic_xchg_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchange(expected:int(64), desired:int(64),
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_int64(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc compareExchangeWeak(expected:int(64), desired:int(64),
                                    order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:int(64), desired:int(64),
                                      order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_add_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc add(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_add_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchSub(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_sub_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc sub(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_sub_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchOr(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_or_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc or(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_or_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchAnd(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_and_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc and(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_and_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc fetchXor(value:int(64), order:memory_order = memory_order_seq_cst):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_xor_int64(v, this.locale.id:int(32), this._v, ret);
      return ret;
    }
    inline proc xor(value:int(64), order:memory_order = memory_order_seq_cst) {
      var v = value;
      chpl_comm_atomic_xor_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc waitFor(val:int(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        // After waiting for the value, do a thread fence
        // in order to guarantee e.g. an acquire barrier even
        // if the on statement is not included.
        atomic_thread_fence(order);
      }
    }

    inline proc peek():int(64) {
      return _v;
    }
    inline proc poke(value:int(64)) {
      _v = value;
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomic_int64, b:ratomic_int64) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomic_int64, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomic_int64, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomic_int64, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomic_int64, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomic_int64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomic_int64, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // int(32)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_int32(ref result:int(32),
                                         l:int(32), ref obj:int(32));
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
    inline proc read(order:memory_order = memory_order_seq_cst):int(32) {
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

    inline proc waitFor(val:int(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():int(32) {
      return _v;
    }
    inline proc poke(value:int(32)) {
      _v = value;
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomic_int32, b:ratomic_int32) {
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
                                         l:int(32), ref obj:uint(64));
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
    inline proc read(order:memory_order = memory_order_seq_cst):uint(64) {
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

    inline proc waitFor(val:uint(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():uint(64) {
      return _v;
    }
    inline proc poke(value:uint(64)) {
      _v = value;
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomic_uint64, b:ratomic_uint64) {
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
                                         l:int(32), ref obj:uint(32));
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
    inline proc read(order:memory_order = memory_order_seq_cst):uint(32) {
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

    inline proc waitFor(val:uint(32), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():uint(32) {
      return _v;
    }
    inline proc poke(value:uint(32)) {
      _v = value;
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomic_uint32, b:ratomic_uint32) {
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
  record ratomicflag {
    var _v: int(64);
    inline proc read(order:memory_order = memory_order_seq_cst):bool {
      var ret: int(64);
      chpl_comm_atomic_get_int64(ret, this.locale.id:int(32), this._v);
      return ret:bool;
    }
    inline proc write(value:bool, order:memory_order = memory_order_seq_cst) {
      var v = value:int(64);
      chpl_comm_atomic_put_int64(v, this.locale.id:int(32), this._v);
    }

    inline proc exchange(value:bool, order:memory_order = memory_order_seq_cst):bool {
      var ret:int(64);
      var v = value:int(64);
      chpl_comm_atomic_xchg_int64(v, this.locale.id:int(32), this._v, ret);
      return ret:bool;
    }
    inline proc compareExchange(expected:bool, desired:bool,
                                order:memory_order = memory_order_seq_cst):bool {
      var ret:bool(32);
      var te = expected:int(64);
      var td = desired:int(64);
      chpl_comm_atomic_cmpxchg_int64(te, td, this.locale.id:int(32), this._v, ret);
      return ret;
    }
inline proc compareExchangeWeak(expected:bool, desired:bool,
                                order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:bool, desired:bool,
                                      order:memory_order = memory_order_seq_cst):bool {
      return this.compareExchange(expected, desired);
    }

    inline proc testAndSet():bool {
      return this.exchange(true);
    }
    inline proc clear() {
      this.write(false);
    }

    inline proc waitFor(val:bool, order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():bool {
      return _v:bool;
    }
    inline proc poke(value:bool) {
      _v = value:int(64);
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomicflag, b:ratomicflag) {
    a.write(b.read());
  }
  inline proc =(ref a:ratomicflag, b) {
    compilerError("Cannot directly assign network atomic variables");
  }
  inline proc +(a:ratomicflag, b) {
    compilerError("Cannot directly add network atomic variables");
    return a;
  }
  inline proc -(a:ratomicflag, b) {
    compilerError("Cannot directly subtract network atomic variables");
    return a;
  }
  inline proc *(a:ratomicflag, b) {
    compilerError("Cannot directly multiply network atomic variables");
    return a;
  }
  inline proc /(a:ratomicflag, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }
  inline proc %(a:ratomicflag, b) {
    compilerError("Cannot directly divide network atomic variables");
    return a;
  }


  // real(64)
  pragma "insert line file info"
  extern proc chpl_comm_atomic_get_real64(ref result:real(64),
                                          l:int(32), ref obj:real(64));
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
    inline proc read(order:memory_order = memory_order_seq_cst):real(64) {
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


    inline proc waitFor(val:real(64), order:memory_order = memory_order_seq_cst) {
      on this {
        while (read(memory_order_relaxed) != val) do chpl_task_yield();
        atomic_thread_fence(order);
      }
    }

    inline proc peek():real(64) {
      return _v;
    }
    inline proc poke(value:real(64)) {
      _v = value;
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(ref a:ratomic_real64, b:ratomic_real64) {
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

