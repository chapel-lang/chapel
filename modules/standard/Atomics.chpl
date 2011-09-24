module Atomics {
  // Wrapping these GASNet macros
  // See GASNet's README-tools.
  extern proc gasnett_local_wmb();
  extern proc gasnett_local_rmb();
  extern proc gasnett_local_mb();
  extern proc gasnett_compiler_fence();

  extern type gasnett_atomic_t;
  extern type gasnett_atomic_val_t = c_long;

  extern const GASNETT_ATOMIC_MAX:c_long;
  extern const GASNETT_ATOMIC_SIGNED_MIN:c_long;
  extern const GASNETT_ATOMIC_SIGNED_MAX:c_long;

  extern const GASNETT_ATOMIC_NONE:c_int;
  extern const GASNETT_ATOMIC_RMB_PRE:c_int;
  extern const GASNETT_ATOMIC_WMB_PRE:c_int;
  extern const GASNETT_ATOMIC_MB_PRE:c_int;

  extern const GASNETT_ATOMIC_RMB_POST:c_int;
  extern const GASNETT_ATOMIC_WMB_POST:c_int;
  extern const GASNETT_ATOMIC_MB_POST:c_int;

  extern const GASNETT_ATOMIC_RMB_POST_IF_TRUE:c_int;
  extern const GASNETT_ATOMIC_WMB_POST_IF_FALSE:c_int;

  extern const GASNETT_ATOMIC_REL:c_int;
  extern const GASNETT_ATOMIC_ACQ:c_int;
  extern const GASNETT_ATOMIC_ACQ_IF_TRUE:c_int;
  extern const GASNETT_ATOMIC_ACQ_IF_FALSE:c_int;

  extern proc gasnett_atomic_set(inout p:gasnett_atomic_t, v:gasnett_atomic_val_t, flags:c_int);
  extern proc gasnett_atomic_read(inout p:gasnett_atomic_t, flags:c_int):gasnett_atomic_val_t;
  extern proc gasnett_atomic_increment(inout p:gasnett_atomic_t, flags:c_int);
  extern proc gasnett_atomic_decrement(inout p:gasnett_atomic_t, flags:c_int);
  extern proc gasnett_atomic_decrement_and_test(inout p:gasnett_atomic_t, flags:c_int):c_int;
  extern proc gasnett_atomic_add(inout p:gasnett_atomic_t, op:gasnett_atomic_val_t, flags:c_int):gasnett_atomic_val_t;
  extern proc gasnett_atomic_subtract(inout p:gasnett_atomic_t, op:gasnett_atomic_val_t, flags:c_int):gasnett_atomic_val_t;
  extern proc gasnett_atomic_compare_and_swap(inout p:gasnett_atomic_t, oldval:gasnett_atomic_val_t, newval:gasnett_atomic_val_t, flags:c_int):c_int;

  extern type gasnett_atomic32_t = uint(32);
  extern type gasnett_atomic64_t = uint(64);

  extern proc gasnett_atomic32_set(inout p:gasnett_atomic32_t, v:uint(32), flags:c_int);
  extern proc gasnett_atomic64_set(inout p:gasnett_atomic64_t, v:uint(64), flags:c_int);

  extern proc gasnett_atomic32_read(inout p:gasnett_atomic32_t, flags:c_int):uint(32);
  extern proc gasnett_atomic64_read(inout p:gasnett_atomic64_t, flags:c_int):uint(64);

  extern proc gasnett_atomic32_compare_and_swap(inout p:gasnett_atomic32_t, oldval:uint(32), newval:uint(32), flags:c_int):c_int;
  extern proc gasnett_atomic64_compare_and_swap(inout p:gasnett_atomic64_t, oldval:uint(64), newval:uint(64), flags:c_int):c_int;

  // Begin Chapel interface for atomic integers.

  pragma "inline"
  proc localWriteBarrier() {
    gasnett_local_wmb();
  }
  pragma "inline"
  proc localReadBarrier() {
    gasnett_local_rmb();
  }
  pragma "inline"
  proc localMemoryBarrier() {
    gasnett_local_mb();
  }
  pragma "inline"
  proc compilerFence() {
    gasnett_compiler_fence();
  }

  const FULL_BARRIER        = GASNETT_ATOMIC_MB_PRE | GASNETT_ATOMIC_MB_POST;
  const WRITE_BARRIER_BEFORE= GASNETT_ATOMIC_WMB_PRE;
  const READ_BARRIER_BEFORE = GASNETT_ATOMIC_RMB_PRE;
  const FULL_BARRIER_BEFORE = GASNETT_ATOMIC_MB_PRE;

  const WRITE_BARRIER_AFTER = GASNETT_ATOMIC_WMB_POST;
  const READ_BARRIER_AFTER  = GASNETT_ATOMIC_RMB_POST;
  const FULL_BARRIER_AFTER  = GASNETT_ATOMIC_MB_POST;

  record atomicuint {
    var _v:gasnett_atomic_t;
    pragma "inline"
    proc set(v: gasnett_atomic_val_t, barrier=FULL_BARRIER) {
      on this do gasnett_atomic_set(this._v, v, barrier);
    }
    pragma "inline"
    proc read(barrier=FULL_BARRIER):gasnett_atomic_val_t {
      var ret:gasnett_atomic_val_t;
      on this do ret = gasnett_atomic_read(this._v, barrier);
      return ret;
    }
    pragma "inline"
    proc increment(barrier=FULL_BARRIER) {
      on this do gasnett_atomic_increment(this._v, barrier);
    }
    pragma "inline"
    proc decrement(barrier=FULL_BARRIER) {
      on this do gasnett_atomic_decrement(this._v, barrier);
    }
    // Returns true if the new value is 0.
    pragma "inline"
    proc decrementAndTest(barrier=FULL_BARRIER):bool {
      var ret:c_int;
      on this do ret = gasnett_atomic_decrement_and_test(this._v, barrier);
      return ret!=0;
    }
    // Returns the value after addition, v must be positive.
    pragma "inline"
    proc add(v: gasnett_atomic_val_t, barrier=FULL_BARRIER):gasnett_atomic_val_t {
      var ret:gasnett_atomic_val_t;
      on this do ret = gasnett_atomic_add(this._v, v, barrier);
      return ret;
    }
    // Returns the value after subtraction, v must be positive.
    pragma "inline"
    proc subtract(v: gasnett_atomic_val_t, barrier=FULL_BARRIER):gasnett_atomic_val_t {
      var ret:gasnett_atomic_val_t;
      on this do ret = gasnett_atomic_subtract(this._v, v, barrier);
      return ret;
    }
    /* Atomic equivalent of
       if (*p == oldval) {
         *p = newval;
         return true;
       } else {
         return false;
       }
   */
    proc compareAndSwap(oldval:gasnett_atomic_val_t, newval:gasnett_atomic_val_t, barrier=FULL_BARRIER):bool {
      var ret:c_int;
      on this do ret = gasnett_atomic_compare_and_swap(this._v, oldval, newval, barrier);
      return ret != 0;
    }
  }

  proc max(type t) where t == atomicuint {
    return GASNETT_ATOMIC_MAX;
  }

  proc min(type t) where t == atomicuint {
    return 0:c_long;
  }

  record atomicuint32 {
    var _v:uint(32);
    pragma "inline"
    proc set(v: uint(32), barrier=FULL_BARRIER) {
      on this do gasnett_atomic32_set(this._v, v, barrier);
    }
    pragma "inline"
    proc read(barrier=FULL_BARRIER):uint(32) {
      var ret:uint(32);
      on this do ret = gasnett_atomic32_read(this._v, barrier);
      return ret;
    }
    /* Atomic equivalent of
       if (*p == oldval) {
         *p = newval;
         return true;
       } else {
         return false;
       }
   */
    proc compareAndSwap(oldval:uint(32), newval:uint(32), barrier=FULL_BARRIER):bool {
      var ret:c_int;
      on this do ret = gasnett_atomic32_compare_and_swap(this._v, oldval, newval, barrier);
      return ret != 0;
    }
  }

  record atomicuint64 {
    var _v:uint(64);
    pragma "inline"
    proc set(v: uint(64), barrier=FULL_BARRIER) {
      on this do gasnett_atomic64_set(this._v, v, barrier);
    }
    pragma "inline"
    proc read(barrier=FULL_BARRIER):uint(64) {
      var ret:uint(64);
      on this do ret = gasnett_atomic64_read(this._v, barrier);
      return ret;
    }
    /* Atomic equivalent of
       if (*p == oldval) {
         *p = newval;
         return true;
       } else {
         return false;
       }
   */
    proc compareAndSwap(oldval:uint(64), newval:uint(64), barrier=FULL_BARRIER):bool {
      var ret:c_int;
      on this do ret = gasnett_atomic64_compare_and_swap(this._v, oldval, newval, barrier);
      return ret != 0;
    }
  }

}

