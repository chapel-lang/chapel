// ChapelSyncvar.chpl
//
// Sync and single variables.
//

pragma "no use ChapelStandard"
module ChapelSyncvar {
  proc chpl__readXX(x: sync) return x.readXX();
  proc chpl__readXX(x: single) return x.readXX();
  proc chpl__readXX(x) return x;

  pragma "sync"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    class _syncvar {
      type base_type;
      var  value: base_type;       // actual data - may need to be declared specially on some targets!
      pragma "omit from constructor" var sync_aux: _sync_aux_t; // data structure for locking, signaling, etc.
      // Ideally, the definition of this class should be target and base_type dependent,
      // since not all targets need to have a sync_aux field if base_type is sufficiently simple.

      proc ~_syncvar() { __primitive("sync_destroy", this); }

      proc initialize() {
        __primitive("sync_init", this);
      }
    }

  // The operations are:
  //  readFE - wait for full, leave empty
  //  readFF - wait for full, leave full
  //  readXX - ignore F/E, leave F/E unchanged
  //  = (i.e., write_wait_empty_leave_full)
  //  writeEF  - wait for empty, leave full (same as =)
  //  writeFF  - wait for full, leave full
  //  writeXF  - ignore F/E, leave full
  //  reset - ignore F/E, write zero, leave empty
  //  isFull - query whether it is full

  // This is the default read on sync vars. Wait for full, set and signal empty.
  proc _syncvar.readFE(): base_type {
    var ret: base_type;
    on this {
      var localRet: base_type;
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_empty", this);
      ret = localRet;
    }
    return ret;
  }

  // Wait for full, set and signal full.
  proc _syncvar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_full", this); // in case others are waiting
      ret = localRet;
    }
    return ret;
  }

  // Ignore F/E.  Read value.  No state change or signals.
  proc _syncvar.readXX() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      __primitive("sync_lock", this);
      localRet = value;
      __primitive("sync_unlock", this);
      ret = localRet;
    }
    return ret;
  }

  // This is the default write on sync vars. Wait for empty, set and signal full.
  proc _syncvar.writeEF(val:base_type) {
    on this {
      __primitive("sync_wait_empty_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
    }
  }

  proc =(sv: sync, val:sv.base_type) {
    sv.writeEF(val);
  }

  // Wait for full, set and signal full.
  proc _syncvar.writeFF(val:base_type) {
    on this {
      __primitive("sync_wait_full_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
    }
  }

  // Ignore F/E, set and signal full.
  proc _syncvar.writeXF(val:base_type) {
    on this {
      __primitive("sync_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);

    }
  }

  // Ignore F/E, set to zero or default value and signal empty.
  proc _syncvar.reset() {
    on this {
      const default_value: base_type;
      __primitive("sync_lock", this);
      value = default_value;
      __primitive("sync_mark_and_signal_empty", this);
    }
  }

  proc _syncvar.isFull {
    var b: bool;
    on this {
      b = __primitive("sync_is_full", this);
    }
    return b;
  }


  // single variable support
  pragma "sync"
    pragma "single"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    class _singlevar {
      type base_type;
      var  value: base_type;     // actual data - may need to be declared specially on some targets!
      pragma "omit from constructor" var single_aux: _single_aux_t; // data structure for locking, signaling, etc.
      // Ideally, the definition of this class should be target and base_type dependent,
      // since not all targets need to have a single_aux field if base_type is sufficiently simple.

      proc ~_singlevar() { __primitive("single_destroy", this); }

      proc initialize() {
        __primitive("single_init", this);
      }
    }

  // Wait for full. Set and signal full.
  proc _singlevar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      if this.isFull then
        localRet = value;
      else {
        __primitive("single_wait_full", this);
        localRet = value;
        __primitive("single_mark_and_signal_full", this); // in case others are waiting
      }
      ret = localRet;
    }
    return ret;
  }


  // Ignore F/E.  Read value.  No state change or signals.
  proc _singlevar.readXX() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      if this.isFull then
        localRet = value;
      else {
        __primitive("single_lock", this);
        localRet = value;
        __primitive("single_unlock", this);
      }
      ret = localRet;
    }
    return ret;
  }


  // Can only write once.  Otherwise, it is an error.
  proc _singlevar.writeEF(val:base_type) {
    on this {
      __primitive("single_lock", this);
      if this.isFull then
        halt("single var already defined");
      value = val;
      __primitive("single_mark_and_signal_full", this);
    }
  }

  proc =(sv: single, value:sv.base_type) {
    sv.writeEF(value);
  }

  proc _singlevar.isFull {
    var b: bool;
    on this {
      b = __primitive("single_is_full", this);
    }
    return b;
  }

  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init: sync) {
    return init;
  }
  
  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init: single) {
    return init;
  }
  
  pragma "init copy fn"
  inline proc chpl__initCopy(sv: sync) {
    return sv.readFE();
  }
  
  pragma "init copy fn"
  inline proc chpl__initCopy(sv: single) {
    return sv.readFF();
  }

  pragma "dont disable remote value forwarding"
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: sync) return x;

  pragma "dont disable remote value forwarding"
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: single) return x;

  // Be explicit about whether syncs and singles are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x: _syncvar) param return false;
  inline proc chpl__maybeAutoDestroyed(x: _singlevar) param return false;

  pragma "auto destroy fn"
  pragma "auto destroy fn sync"
  inline proc chpl__autoDestroy(x: _syncvar) {
    delete x;
  }
  pragma "auto destroy fn"
  pragma "auto destroy fn sync"
  inline proc chpl__autoDestroy(x: _singlevar) {
    delete x;
  }

  proc chpl__syncBaseType(s) type {
    var x = s;
    return x.type;
  }

  /* op= for sync variables */
  inline proc  +=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  + rhs; }
  inline proc  -=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  - rhs; }
  inline proc  *=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  * rhs; }
  inline proc  /=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  / rhs; }
  inline proc  %=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  % rhs; }
  inline proc **=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs ** rhs; }
  inline proc  &=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  & rhs; }
  inline proc  |=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  | rhs; }
  inline proc  ^=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  ^ rhs; }
  inline proc >>=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs >> rhs; }
  inline proc <<=(lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs << rhs; }
  
  inline proc <=>(lhs: sync, ref rhs) {
    const tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }
  
  inline proc <=>(ref lhs, rhs: sync) {
    const tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }
  
  inline proc <=>(lhs: sync, rhs: sync) {
    const tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }
  
}
