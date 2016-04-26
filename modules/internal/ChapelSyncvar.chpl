/*
 * Copyright 2004-2016 Cray Inc.
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

/*
Synchronization variables have a logical state associated with the value. The
state of the variable is either full or empty. Normal reads of a
synchronization variable cannot proceed until the variable's state is full.
Normal writes of a synchronization variable cannot proceed until the variable's
state is empty.

Chapel supports two types of synchronization variables: sync and single. Both
types behave similarly, except that a single variable may only be written once.
Consequently, when a sync variable is read, its state transitions to empty,
whereas when a single variable is read, its state does not change. When either
type of synchronization variable is written, its state transitions to full.

If a task attempts to read or write a synchronization variable that is not in
the correct state, the task is suspended. When the variable transitions to the
correct state, the task is resumed. If there are multiple tasks blocked waiting
for the state transition, one is non-deterministically selected to proceed and
the others continue to wait if it is a sync variable; all tasks are selected to
proceed if it is a single variable.
*/
module ChapelSyncvar {
  use MemConsistency;

  pragma "no doc"
  inline proc chpl__readXX(x: sync) return x.readXX();
  pragma "no doc"
  inline proc chpl__readXX(x: single) return x.readXX();
  pragma "no doc"
  inline proc chpl__readXX(x) return x;

  proc chpl_ensureFEType(type t) {
    //
    // The following types are OK for full empty types (sync/single)
    // because they represent a single logical value.  (Note that for
    // the class it's the referenece to the object that has full/empty
    // semantics.  Note that this includes the internal type
    // chpl_taskID_t in order to keep parallel/taskPar/sungeun/private.chpl
    // working, but this does not seem to be more broadly necessary.
    //
    if !(isVoidType(t) || isBoolType(t) || isIntegralType(t) || 
         isEnumType(t) || isFloatType(t) || isStringType(t) || 
         isClassType(t) || t == chpl_taskID_t) {
      //
      // TODO: compilerError() does not seem to be propagating up to
      // the user-level modules... it should, shouldn't it?
      //
      compilerError("sync/single types cannot be of type '", t:string, "'");
    }
  }

  pragma "sync"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    pragma "ignore noinit"
    pragma "not plain old data"
    pragma "no doc"
    class _syncvar {
      type base_type;
      var  value: base_type;       // actual data - may need to be declared specially on some targets!
      pragma "omit from constructor" var sync_aux: _sync_aux_t; // data structure for locking, signaling, etc.
      // Ideally, the definition of this class should be target and base_type dependent,
      // since not all targets need to have a sync_aux field if base_type is sufficiently simple.

      proc ~_syncvar() { __primitive("sync_destroy", this); }

      proc initialize() {
        chpl_ensureFEType(base_type);
        __primitive("sync_init", this);
      }
    }

  /* Returns true if `t` is a sync type, false otherwise. */
  proc isSyncType(type t) param
    return __primitive("is sync type", t);

  pragma "no doc"
  proc isSyncValue(x: sync) param  return true;
  pragma "no doc"
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

  /*
     This method blocks until the sync variable is full. The state of the sync
     variable is set to empty when this method completes. This method
     implements the default read of a sync variable.

     :returns: The value of the sync variable.
  */
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

  /*
     This method blocks until the sync variable is full. The state of the sync
     variable remains full when this method completes.

     :returns: The value of the sync variable.
  */
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

  /*
     This method is non-blocking and the state of the sync variable is
     unchanged when this method completes.

     :returns: The value of the sync variable.
  */
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

  /*
     :arg val: New value of the sync variable.

     This method blocks until the sync variable is empty. The state of the sync
     variable is set to full when this method completes. This method implements
     the default write of a sync variable.
  */
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

  /*
     :arg val: New value of the sync variable.

     This method blocks until the sync variable is full. The state
     of the sync variable remains full when this method completes.
  */
  proc _syncvar.writeFF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("sync_wait_full_and_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  /*
     :arg val: New value of the sync variable.

     This method is non-blocking and the state of the sync
     variable is set to full when this method completes.
  */
  proc _syncvar.writeXF(val:base_type) {
    on this {
      chpl_rmem_consist_release();
      __primitive("sync_lock", this);
      value = val;
      __primitive("sync_mark_and_signal_full", this);
      chpl_rmem_consist_acquire();
    }
  }

  /*
     Resets the value of this sync variable to the default value of
     its type. This method is non-blocking and the state of the sync
     variable is set to empty when this method completes.
  */
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

  /*
     :returns: true if the state of the sync variable is full.

     This method is non-blocking and the state of the sync variable is
     unchanged when this method completes.
  */
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
  pragma "no doc"
  proc _syncvar.readThis(x) {
    compilerError("sync/single variables cannot currently be read - use writeEF/writeFF instead");
  }

  // Do not allow implicit writes of sync/single vars.
  pragma "no doc"
  proc _syncvar.writeThis(x) {
    compilerError("sync/single variables cannot currently be written - apply readFE/readFF() to those variables first");
  }


  // single variable support
    pragma "single"
    pragma "no object" // Optimize out the object base pointer.
    pragma "no default functions"
    pragma "ignore noinit"
    pragma "not plain old data"
    pragma "no doc"
    class _singlevar {
      type base_type;
      var  value: base_type;     // actual data - may need to be declared specially on some targets!
      pragma "omit from constructor" var single_aux: _single_aux_t; // data structure for locking, signaling, etc.
      // Ideally, the definition of this class should be target and base_type dependent,
      // since not all targets need to have a single_aux field if base_type is sufficiently simple.

      proc ~_singlevar() { __primitive("single_destroy", this); }

      proc initialize() {
        chpl_ensureFEType(base_type);
        __primitive("single_init", this);
      }
    }

  /* Returns true if `t` is a single type, false otherwise. */
  proc isSingleType(type t) param
    return __primitive("is single type", t);

  pragma "no doc"
  proc isSingleValue(x: single) param  return true;
  pragma "no doc"
  proc isSingleValue(x)         param  return false;

  /*
     This method blocks until the single variable is full. The state of the single
     variable remains full when this method completes. This method implements
     the default read of a single variable.

     :returns: The value of the single variable.
  */
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


  /*
     This method is non-blocking and the state of the single variable is
     unchanged when this method completes.

     :returns: The value of the single variable.
  */
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


  /*
     :arg val: New value of the single variable.

     This method blocks until the single variable is empty. The state of the single
     variable is set to full when this method completes. This method implements
     the default write of a single variable.
  */
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

  /*
     :returns: true if the state of the single variable is full.

     This method is non-blocking and the state of the single variable is
     unchanged when this method completes.
  */
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
  pragma "no doc"
  proc _singlevar.readThis(x) {
    compilerError("single/sync variables cannot currently be read - use writeEF/writeFF instead");
  }

  // Do not allow implicit writes of sync/single vars.
  pragma "no doc"
  proc _singlevar.writeThis(x) {
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

  // op= for sync variables
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
