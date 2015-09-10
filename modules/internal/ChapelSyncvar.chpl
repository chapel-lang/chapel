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

// ChapelSyncvar.chpl
//
// Sync and single variables.
//

module ChapelSyncvar {
  use MemConsistency;

  inline proc chpl__readXX(x: sync) return x.readXX();
  inline proc chpl__readXX(x: single) return x.readXX();
  inline proc chpl__readXX(x) return x;

  pragma "sync"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    pragma "ignore noinit"
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

  proc isSyncType(type t) param
    return __primitive("is sync type", t);

  proc isSyncValue(x: sync) param  return true;
  proc isSyncValue(x)       param  return false;

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
      chpl_rmem_consist_release();
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_empty", this);
      chpl_rmem_consist_acquire();
      ret = localRet;
    }
    return ret;
  }

  // Wait for full, set and signal full.
  proc _syncvar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      chpl_rmem_consist_release();
      __primitive("sync_wait_full_and_lock", this);
      localRet = value;
      __primitive("sync_mark_and_signal_full", this); // in case others are waiting
      chpl_rmem_consist_acquire();
      ret = localRet;
    }
    return ret;
  }

  // Ignore F/E.  Read value.  No state change or signals.
  proc _syncvar.readXX() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      chpl_rmem_consist_release();
      __primitive("sync_lock", this);
      localRet = value;
      __primitive("sync_unlock", this);
      chpl_rmem_consist_acquire();
      ret = localRet;
    }
    return ret;
  }

  // This is the default write on sync vars. Wait for empty, set and signal full.
  proc _syncvar.writeEF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("sync_wait_empty_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  proc =(ref sv: sync, val:sv.base_type) {
    sv.writeEF(val);
  }

  // Wait for full, set and signal full.
  proc _syncvar.writeFF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("sync_wait_full_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  // Ignore F/E, set and signal full.
  proc _syncvar.writeXF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("sync_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  // Ignore F/E, set to zero or default value and signal empty.
  proc _syncvar.reset() {
    on this {
      const default_value: base_type;
      chpl_rmem_consist_release();
      __primitive("sync_lock", this);
      value = default_value;
      __primitive("sync_mark_and_signal_empty", this);
      chpl_rmem_consist_acquire();
    }
  }

  proc _syncvar.isFull {
    var b: bool;
    on this {
      chpl_rmem_consist_release();
      b = __primitive("sync_is_full", this);
      chpl_rmem_consist_acquire();
    }
    return b;
  }

  // Do not allow implicit reads of sync/single vars.
  proc _syncvar.readThis(x: Reader) {
    compilerError("sync/single variables cannot currently be read - use writeEF/writeFF instead");
  }

  // Do not allow implicit writes of sync/single vars.
  proc _syncvar.writeThis(x: Writer) {
    compilerError("sync/single variables cannot currently be written - apply readFE/readFF() to those variables first");
  }


  // single variable support
    pragma "single"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    pragma "ignore noinit"
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

  proc isSingleType(type t) param
    return __primitive("is single type", t);

  proc isSingleValue(x: single) param  return true;
  proc isSingleValue(x)         param  return false;

  // Wait for full. Set and signal full.
  proc _singlevar.readFF() {
    var ret: base_type;
    on this {
      var localRet: base_type;
      chpl_rmem_consist_release();
      if this.isFull then
        localRet = value;
      else {
        __primitive("single_wait_full", this);
        localRet = value;
        __primitive("single_mark_and_signal_full", this); // in case others are waiting
      }
      chpl_rmem_consist_acquire();
      ret = localRet;
    }
    return ret;
  }


  // Ignore F/E.  Read value.  No state change or signals.
  proc _singlevar.readXX() {
    var ret: base_type;
    on this {
      chpl_rmem_consist_release();
      var localRet: base_type;
      if this.isFull then
        localRet = value;
      else {
        __primitive("single_lock", this);
        localRet = value;
        __primitive("single_unlock", this);
      }
      chpl_rmem_consist_acquire();
      ret = localRet;
    }
    return ret;
  }


  // Can only write once.  Otherwise, it is an error.
  proc _singlevar.writeEF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("single_lock", this);
      if this.isFull then
        halt("single var already defined");
      value = val;
      __primitive("single_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  proc =(ref sv: single, value:sv.base_type) {
    sv.writeEF(value);
  }

  proc _singlevar.isFull {
    var b: bool;
    on this {
      chpl_rmem_consist_release();
      b = __primitive("single_is_full", this);
      chpl_rmem_consist_acquire();
    }
    return b;
  }


  // Do not allow implicit reads of sync/single vars.
  proc _singlevar.readThis(x: Reader) {
    compilerError("single/sync variables cannot currently be read - use writeEF/writeFF instead");
  }

  // Do not allow implicit writes of sync/single vars.
  proc _singlevar.writeThis(x: Writer) {
    compilerError("single/sync variables cannot currently be written - apply readFF/readFE() to those variables first");
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
  inline proc chpl__maybeAutoDestroyed(x: _syncvar) param return true;
  inline proc chpl__maybeAutoDestroyed(x: _singlevar) param return true;

  pragma "auto destroy fn sync"
  inline proc chpl__autoDestroy(x: _syncvar) {
    delete x;
  }
  pragma "auto destroy fn sync"
  inline proc chpl__autoDestroy(x: _singlevar) {
    delete x;
  }

  proc chpl__syncBaseType(s) type {
    var x = s;
    return x.type;
  }

  /* op= for sync variables */
  inline proc  +=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  + rhs; }
  inline proc  -=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  - rhs; }
  inline proc  *=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  * rhs; }
  inline proc  /=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  / rhs; }
  inline proc  %=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  % rhs; }
  inline proc **=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs ** rhs; }
  inline proc  &=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  & rhs; }
  inline proc  |=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  | rhs; }
  inline proc  ^=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs  ^ rhs; }
  inline proc >>=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs >> rhs; }
  inline proc <<=(ref lhs:sync, rhs:chpl__syncBaseType(lhs)) { lhs = lhs << rhs; }
  
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
