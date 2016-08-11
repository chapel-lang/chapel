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

  /************************************ | *************************************
  *                                                                           *
  * The implementation of the user-facing sync/single types are exposed to    *
  * the compiler as a pair of record-wrapped classes.                         *
  *                                                                           *
  * The record implements the compiler facing API and manages the memory      *
  * associated with the underlying class while the class provides the         *
  * identity behavior required for the semantics of sync/single.              *
  *                                                                           *
  ************************************* | ************************************/

  //
  // The following types are OK for full empty types (sync/single)
  // because they represent a single logical value.  (Note that for
  // the class it's the reference to the object that has full/empty
  // semantics.  Note that this includes the internal type
  // chpl_taskID_t in order to keep parallel/taskPar/sungeun/private.chpl
  // working, but this does not seem to be more broadly necessary.
  //

  private proc isSupported(type t) param
    return isVoidType(t)          ||
           isBoolType(t)          ||
           isIntegralType(t)      ||
           isRealType(t)          ||
           isImagType(t)          ||
           isEnumType(t)          ||
           isClassType(t)         ||
           isStringType(t)        ||    // Should this be allowed?
           t == chpl_taskID_t;

  private proc ensureFEType(type t) {
    if isSupported(t) == false then
      compilerError("sync/single types cannot be of type '", t : string, "'");
  }

  /************************************ | *************************************
  *                                                                           *
  * The record wrapper to implement sync                                      *
  *                                                                           *
  ************************************* | ************************************/

  pragma "sync"
  pragma "default intent is ref"
  pragma "no doc"
  record _syncvar {
    type valType;                              // The compiler knows this name

    var  syncVar : _synccls(valType) = nil;
    var  isOwned : bool              = true;

    proc _syncvar(type valType) {
      ensureFEType(valType);

      syncVar = new _synccls(valType);
      isOwned = true;
    }

    proc _syncvar(type valType, other : _syncvar(valType)) {
      ensureFEType(valType);

      syncVar = other.syncVar;
      isOwned = false;
    }

    proc ~_syncvar() {
      if isOwned == true then
        delete syncVar;
    }

    // Do not allow implicit reads of sync vars.
    proc readThis(x) {
      compilerError("sync variables cannot currently be read - use writeEF/writeFF instead");
    }

    // Do not allow implicit writes of sync vars.
    proc writeThis(x) {
      compilerError("sync variables cannot currently be written - apply readFE/readFF() to those variables first");
     }
  }

  /*
     Noakes 2016/08/10

     These are defined as secondary methods so that chpldoc can render the
     documentation
  */

  pragma "no doc"
  proc isSyncType(type t) param where t : _syncvar {
    return true;
  }

  /* Returns true if `t` is a sync type, false otherwise. */
  proc isSyncType(type t) param {
    return false;
  }

  /*
    1) Block until the sync variable is full.
    2) Read the value of the sync variable and set the variable to empty.

    :returns: The value of the sync variable.
  */
  proc _syncvar.readFE() {
    return syncVar.readFE();
  }

  /*
    1) Block until the sync variable is full.
    2) Read the value of the sync variable and leave the variable full

    :returns: The value of the sync variable.
  */
  proc _syncvar.readFF() {
    return syncVar.readFF();
  }

  /*
    1) Read the value of the sync variable
    2) Do not inspect or change the full/empty state

    :returns: The value of the sync variable.
  */
  proc _syncvar.readXX() {
    return syncVar.readXX();
  }

  /*
    1) Block until the sync variable is empty.
    2) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeEF(x : valType) {
    syncVar.writeEF(x);
  }

  /*
    1) Block until the sync variable is full.
    2) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeFF(x : valType) {
    syncVar.writeFF(x);
  }

  /*
    1) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeXF(x : valType) {
    syncVar.writeXF(x);
  }

  /*
    Resets the value of this sync variable to the default value of
    its type. This method is non-blocking and the state of the sync
    variable is set to empty when this method completes.
  */
  proc _syncvar.reset() {
    syncVar.reset();
  }

  /*
     Determine if the sync variable is full without blocking.
     Does not alter the state of the sync variable

     :returns: true if the state of the sync variable is full.
  */
  proc _syncvar.isFull {
    return syncVar.isFull;
  }

  proc   = (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(rhs);
  }

  proc  += (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() +  rhs);
  }

  proc  -= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() -  rhs);
  }

  proc  *= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() *  rhs);
  }

  proc  /= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() /  rhs);
  }

  proc  %= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() %  rhs);
  }

  proc **= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() ** rhs);
  }

  proc  &= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() &  rhs);
  }

  proc  |= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() |  rhs);
  }

  proc  ^= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() ^  rhs);
  }

  proc >>= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() >> rhs);
  }

  proc <<= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.syncVar.writeEF(lhs.syncVar.readFE() << rhs);
  }

  pragma "init copy fn"
  proc chpl__initCopy(sv : _syncvar(?t)) {
    return sv.readFE();
  }

  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(const ref rhs : _syncvar(?t)) {
    return new _syncvar(t, rhs);
  }

  // Be explicit about whether syncs are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x : _syncvar(?t)) param return true;

  // This version has to be available to take precedence
  pragma "auto destroy fn sync"
    inline proc chpl__autoDestroy(x : _syncvar(?)) {
    if x.isOwned == true then
      delete x.syncVar;
  }

  pragma "no doc"
  proc chpl__readXX(x : _syncvar(?)) return x.readXX();

  proc <=>(lhs: _syncvar, ref rhs) {
    const tmp = lhs;

    lhs = rhs;
    rhs = tmp;
  }

  proc <=>(ref lhs, rhs: _syncvar) {
    const tmp = lhs;

    lhs = rhs;
    rhs = tmp;
  }

  proc <=>(lhs: _syncvar, rhs: _syncvar) {
    const tmp = lhs;

    lhs = rhs;
    rhs = tmp;
  }

  /************************************ | *************************************
  *                                                                           *
  * Use of a class instance establishes the required identity property.       *
  *                                                                           *
  * Potential future optimization: Some targets could rely on a class that    *
  * omits the syncAux variable for sufficiently simple valType.               *
  *                                                                           *
  ************************************* | ************************************/

  // Implementation is target dependent and opaque to Chapel code
  pragma "no doc"
  extern record chpl_sync_aux_t { };

  extern proc   chpl_sync_initAux(ref aux : chpl_sync_aux_t);
  extern proc   chpl_sync_destroyAux(ref aux : chpl_sync_aux_t);

  pragma "insert line file info"
  extern proc   chpl_sync_waitEmptyAndLock(ref aux : chpl_sync_aux_t);

  pragma "insert line file info"
  extern proc   chpl_sync_waitFullAndLock (ref aux : chpl_sync_aux_t);

  extern proc   chpl_sync_lock  (ref aux : chpl_sync_aux_t);
  extern proc   chpl_sync_unlock(ref aux : chpl_sync_aux_t);

  extern proc   chpl_sync_markAndSignalEmpty(ref aux : chpl_sync_aux_t);
  extern proc   chpl_sync_markAndSignalFull (ref aux : chpl_sync_aux_t);

  extern proc   chpl_sync_isFull(value : c_void_ptr,
                                 ref aux   : chpl_sync_aux_t) : bool;

  pragma "no doc"
  class _synccls {
    type valType;

    var  value   : valType;
    var  syncAux : chpl_sync_aux_t;      // Locking, signaling, ...

    proc _synccls(type valType) {
      chpl_sync_initAux(syncAux);
    }

    proc ~_synccls() {
      chpl_sync_destroyAux(syncAux);
    }

    proc readFE() {
      var ret: valType;

      on this {
        var localRet: valType;

        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        localRet = value;

        chpl_sync_markAndSignalEmpty(syncAux);
        chpl_rmem_consist_acquire();

        ret      = localRet;
      }

      return ret;
    }

    proc readFF() {
      var ret: valType;

      on this {
        var localRet: valType;

        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        localRet = value;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();

        ret = localRet;
      }

      return ret;
    }

    proc readXX() {
      var ret: valType;

      on this {
        var localRet: valType;

        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        localRet = value;

        chpl_sync_unlock(syncAux);
        chpl_rmem_consist_acquire();

        ret      = localRet;
      }

      return ret;
    }

    proc writeEF(val : valType) {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_waitEmptyAndLock(syncAux);

        value = val;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeFF(val:valType) {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        value = val;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeXF(val:valType) {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        value = val;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc reset() {
      on this {
        const default_value: valType;

        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        value = default_value;

        chpl_sync_markAndSignalEmpty(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc isFull {
      var b : bool;

      on this {
        chpl_rmem_consist_release();
        b = chpl_sync_isFull(c_ptrTo(value), syncAux);
        chpl_rmem_consist_acquire();
      }

      return b;
    }
  }

  pragma "no doc"
  proc isSyncValue(x: sync) param  return true;

  pragma "no doc"
  proc isSyncValue(x)       param  return false;





  /************************************ | *************************************
  *                                                                           *
  * Class based implementation of single                                      *
  *                                                                           *
  ************************************* | ************************************/

  pragma "single"
  pragma "no object" // Optimize out the object base pointer.
  pragma "no default functions"
  pragma "ignore noinit"
  pragma "not plain old data"
  pragma "no doc"
  class _singlevar {
    type valType;                              // The compiler knows this name

    // actual data - may need to be declared specially on some targets!
    var  value: valType;

    // data structure for locking, signaling, etc.
    pragma "omit from constructor"
    var single_aux: _single_aux_t;

    // Ideally, the definition of this class should be target and valType
    // dependent, since not all targets need to have a single_aux field if
    // valType is sufficiently simple.

    proc ~_singlevar() {
      __primitive("single_destroy", this);
    }

    proc initialize() {
      ensureFEType(valType);
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
     This method blocks until the single variable is full.
     The state of the single variable remains full when this method completes.
     This method implements the default read of a single variable.

     :returns: The value of the single variable.
  */
  proc _singlevar.readFF() {
    var ret: valType;
    on this {
      var localRet: valType;
      chpl_rmem_consist_release();
      if this.isFull then
        localRet = value;
      else {
        __primitive("single_wait_full", this);
        localRet = value;
        // in case others are waiting
        __primitive("single_mark_and_signal_full", this);
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
    var ret: valType;
    on this {
      chpl_rmem_consist_release();
      var localRet: valType;
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
  proc _singlevar.writeEF(val:valType) {
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

  proc =(ref sv: single, value:sv.valType) {
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


  // Do not allow implicit reads of single vars.
  pragma "no doc"
  proc _singlevar.readThis(x) {
    compilerError("single variables cannot currently be read - use writeEF/writeFF instead");
  }

  // Do not allow implicit writes of single vars.
  pragma "no doc"
  proc _singlevar.writeThis(x) {
    compilerError("single variables cannot currently be written - apply readFF/readFE() to those variables first");
  }

  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init: single) {
    return init;
  }

  pragma "init copy fn"
  inline proc chpl__initCopy(sv: single) {
    return sv.readFF();
  }

  pragma "dont disable remote value forwarding"
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x: single) return x;

  // Be explicit about whether singles are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x: _singlevar) param return true;

  pragma "auto destroy fn sync"
  inline proc chpl__autoDestroy(x: _singlevar) {
    delete x;
  }

  proc chpl__syncBaseType(s) type {
    var x = s;
    return x.type;
  }

  pragma "no doc"
  inline proc chpl__readXX(x: single) return x.readXX();

  pragma "no doc"
  inline proc chpl__readXX(x) return x;
}
