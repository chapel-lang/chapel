// ChapelSyncvar.chpl
//
// Sync and single variables.
//

pragma "no use ChapelStandard"
module ChapelSyncvar {
  proc chpl__readXX(x: sync) return x.readXX();
  proc chpl__readXX(x: single) return x.readXX();
  proc chpl__readXX(x) return x;

  // Returns whether an object of type t occupies a 64-bit word on Cray's MTA/XMT
  // (The definition of this function should be target dependent.  This would avoid
  // the need to write C macros in the runtime that essentially duplicate
  // the functionality of the read/write methods of the _syncvar and _singlevar classes
  // for targets that don't have particularly fast ways of achieving this functionality
  // for simple base types.)
  proc isSimpleSyncBaseType (type t) param {
    if CHPL_TASKS == "mta" then
      if t == int(64) || t == uint(64) || t == int(32) || t == uint(32)
        || t == int(16) || t == uint(16) || t == int(8) || t == uint(8)
        || t == real(32) || t == real(64) || t == imag(32) || t == imag(64) then
        return true;
    else return false;
    else return false;
  }

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
        if (isSimpleSyncBaseType(this.base_type)) {
          // The sync_aux field might not be used on some targets!
          __primitive("sync_reset", this);
        }
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
      if isSimpleSyncBaseType(base_type) then
        localRet = __primitive("read_FE", localRet, this);
      else {
        __primitive("sync_wait_full_and_lock", this);
        localRet = value;
        __primitive("sync_mark_and_signal_empty", this);
      }
      ret = localRet;
    }
    return ret;
  }

  // Wait for full, set and signal full.
  proc _syncvar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      if isSimpleSyncBaseType(base_type) then
        localRet = __primitive("read_FF", localRet, this);
      else {
        __primitive("sync_wait_full_and_lock", this);
        localRet = value;
        __primitive("sync_mark_and_signal_full", this); // in case others are waiting
      }
      ret = localRet;
    }
    return ret;
  }

  // Ignore F/E.  Read value.  No state change or signals.
  proc _syncvar.readXX() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      if isSimpleSyncBaseType(base_type) then
        localRet = __primitive("read_XX", localRet, this);
      else {
        __primitive("sync_lock", this);
        localRet = value;
        __primitive("sync_unlock", this);
      }
      ret = localRet;
    }
    return ret;
  }

  // This is the default write on sync vars. Wait for empty, set and signal full.
  proc _syncvar.writeEF(val:base_type) {
    on this {
      if isSimpleSyncBaseType(base_type) then
        __primitive("write_EF", this, val);
      else {
        __primitive("sync_wait_empty_and_lock", this);
        value = val;
        __primitive("sync_mark_and_signal_full", this);
      }
    }
  }

  proc =(sv: sync, val:sv.base_type) {
    sv.writeEF(val);
  }

  // Wait for full, set and signal full.
  proc _syncvar.writeFF(val:base_type) {
    on this {
      if isSimpleSyncBaseType(base_type) then
        __primitive("write_FF", this, val);
      else {
        __primitive("sync_wait_full_and_lock", this);
        value = val;
        __primitive("sync_mark_and_signal_full", this);
      }
    }
  }

  // Ignore F/E, set and signal full.
  proc _syncvar.writeXF(val:base_type) {
    on this {
      if isSimpleSyncBaseType(base_type) then
        __primitive("write_XF", this, val);
      else {
        __primitive("sync_lock", this);
        value = val;
        __primitive("sync_mark_and_signal_full", this);
      }
    }
  }

  // Ignore F/E, set to zero or default value and signal empty.
  proc _syncvar.reset() {
    on this {
      if isSimpleSyncBaseType(base_type) then
        // Reset this's value to zero.
        __primitive("sync_reset", this);
      else {
        const default_value: base_type;
        __primitive("sync_lock", this);
        value = default_value;
        __primitive("sync_mark_and_signal_empty", this);
      }
    }
  }

  proc _syncvar.isFull {
    var b: bool;
    on this {
      b = __primitive("sync_is_full", this, isSimpleSyncBaseType(base_type));
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
        if (isSimpleSyncBaseType(this.base_type)) {
          // The single_aux field might not be used on some targets!
          __primitive("single_reset", this);  // No locking or unlocking done here!
        }
      }
    }

  // Wait for full. Set and signal full.
  proc _singlevar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      if isSimpleSyncBaseType(base_type) then
        localRet = __primitive("single_read_FF", localRet, this);
      else if this.isFull then
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
      if isSimpleSyncBaseType(base_type) then
        localRet = __primitive("single_read_XX", localRet, this);
      else if this.isFull then
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
      if isSimpleSyncBaseType(base_type) then
        __primitive("single_write_EF", this, val);
      else {
        __primitive("single_lock", this);
        if this.isFull then
          halt("single var already defined");
        value = val;
        __primitive("single_mark_and_signal_full", this);
      }
    }
  }

  proc =(sv: single, value:sv.base_type) {
    sv.writeEF(value);
  }

  proc _singlevar.isFull {
    var b: bool;
    on this {
      b = __primitive("single_is_full", this, isSimpleSyncBaseType(base_type));
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

  // These implement chpl_here_free for sync and single objects (permitting their
  // representations to be reclaimed under hierarchical locales).
  // The resulting code duplication is undesirable, but necessary for two reasons:
  // 1) We do not wish to allow the implicit coercion of syncs and singles to objects, and
  // 2) We do not yet have an implementation for user-definable coercions which would allow
  //    us to detect and diagnose this attempted coercion
  // We could detect and diagnose this undesirable coercion in the compiler, but that is even worse
  // (in terms of maintainability) than the code duplication appearing here.
  proc chpl_here_free(x:sync, lineno:int(32), filename:string) {
    // TODO: The pointer should really be of type opaque, but we don't 
    // handle object ==> opaque casts correctly.  (In codegen, opaque behaves 
    // like an lvalue, but in the type system it isn't one.)
    extern proc chpl_memhook_free_pre(ptr:opaque, lineno:int(32), filename:string)
      : void;
    chpl_memhook_free_pre(__primitive("cast_to_void_star", x), lineno, filename);
    __primitive("task_free", x);
  }

  proc chpl_here_free(x:single, lineno:int(32), filename:string) {
    // TODO: The pointer should really be of type opaque, but we don't 
    // handle object ==> opaque casts correctly.  (In codegen, opaque behaves 
    // like an lvalue, but in the type system it isn't one.)
    extern proc chpl_memhook_free_pre(ptr:opaque, lineno:int(32), filename:string)
      : void;
    chpl_memhook_free_pre(__primitive("cast_to_void_star", x), lineno, filename);
    __primitive("task_free", x);
  }

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

  /* op= for sync variables */
  inline proc +=(lhs: sync, rhs)  { lhs = lhs + rhs; }
  inline proc -=(lhs: sync, rhs)  { lhs = lhs - rhs; }
  inline proc *=(lhs: sync, rhs)  { lhs = lhs * rhs; }
  inline proc /=(lhs: sync, rhs)  { lhs = lhs / rhs; }
  inline proc %=(lhs: sync, rhs)  { lhs = lhs % rhs; }
  inline proc **=(lhs: sync, rhs) { lhs = lhs ** rhs; }
  inline proc &=(lhs: sync, rhs)  { lhs = lhs & rhs; }
  inline proc |=(lhs: sync, rhs)  { lhs = lhs | rhs; }
  inline proc ^=(lhs: sync, rhs)  { lhs = lhs ^ rhs; }
  inline proc >>=(lhs: sync, rhs) { lhs = lhs >> rhs; }
  inline proc <<=(lhs: sync, rhs) { lhs = lhs << rhs; }
  
  /* op= for sync variables with param rhs */
  inline proc +=(lhs: sync, param rhs)  { lhs = lhs + rhs; }
  inline proc -=(lhs: sync, param rhs)  { lhs = lhs - rhs; }
  inline proc *=(lhs: sync, param rhs)  { lhs = lhs * rhs; }
  inline proc /=(lhs: sync, param rhs)  { lhs = lhs / rhs; }
  inline proc %=(lhs: sync, param rhs)  { lhs = lhs % rhs; }
  inline proc **=(lhs: sync, param rhs) { lhs = lhs ** rhs; }
  inline proc &=(lhs: sync, param rhs)  { lhs = lhs & rhs; }
  inline proc |=(lhs: sync, param rhs)  { lhs = lhs | rhs; }
  inline proc ^=(lhs: sync, param rhs)  { lhs = lhs ^ rhs; }
  inline proc >>=(lhs: sync, param rhs) { lhs = lhs >> rhs; }
  inline proc <<=(lhs: sync, param rhs) { lhs = lhs << rhs; }

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
