/*
 * Copyright 2004-2020 Cray Inc.
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
  private use ChapelStandard;

  use AlignedTSupport;
  private use MemConsistency;
  use SyncVarRuntimeSupport;

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
    return isNothingType(t)       ||
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
      compilerError("sync/single types cannot contain type '", t : string, "'");

    if isNonNilableClass(t) then
      compilerError("sync/single types cannot contain non-nilable classes");

    if isGenericType(t) then
      compilerError("sync/single types cannot contain generic types");
  }

  pragma "no doc"
  config param useNativeSyncVar = true;

  // use native sync vars if they're enabled and supported for the valType
  private proc getSyncClassType(type valType) type {
    if useNativeSyncVar && supportsNativeSyncVar(valType) {
      return unmanaged _qthreads_synccls(valType);
    } else {
      return unmanaged _synccls(valType);
    }
  }

  pragma "no doc"
  proc chpl__readXX(x) return x;

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

    var  wrapped : getSyncClassType(valType);
    var  isOwned : bool                      = true;

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      ensureFEType(valType);
      this.valType = valType;
      this.wrapped = new (getSyncClassType(valType))();
    }

    //
    // This is technically a copy-initializer, but it's only called through
    // chpl__autoCopy.
    //
    // Also note that syncs **need** to use chpl__initCopy in order to return
    // a different type. For example:
    //
    //   var a = x$;
    //
    // ``a`` needs to be a ``valType``, not a sync.
    //
    pragma "dont disable remote value forwarding"
    proc init(const other : _syncvar) {
      this.valType = other.valType;
      this.wrapped = other.wrapped;
      this.isOwned = false;
    }

    proc init=(const other : _syncvar) {
      // Allow initialization from compatible sync variables, e.g.:
      //   var x : sync int = 5;
      //   var y : sync real = x;
      if isCoercible(other.valType, this.type.valType) == false {
        param theseTypes = "'" + this.type:string + "' from '" + other.type:string + "'";
        param because = "because '" + other.valType:string + "' is not coercible to '" + this.type.valType:string + "'";
        compilerError("cannot initialize ", theseTypes, " ",  because);
      }
      this.init(this.type.valType);
      this.writeEF(other.readFE());
    }

    pragma "dont disable remote value forwarding"
    proc init=(const other : this.valType) {
      this.init(other.type);
      // TODO: initialize the sync class impl with 'other'
      this.writeEF(other);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      if isOwned == true then
        delete wrapped;
    }

    // Do not allow implicit reads of sync vars.
    proc readThis(x) throws {
      compilerError("sync variables cannot currently be read - use writeEF/writeFF instead");
    }

    // Do not allow implicit writes of sync vars.
    proc writeThis(x) throws {
      compilerError("sync variables cannot currently be written - apply readFE/readFF() to those variables first");
     }
  }

  /*
     Noakes 2016/08/10

     These are defined as secondary methods so that chpldoc can render the
     documentation
  */

  pragma "no doc"
  proc isSyncType(type t:_syncvar) param {
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
    return wrapped.readFE();
  }

  /*
    1) Block until the sync variable is full.
    2) Read the value of the sync variable and leave the variable full

    :returns: The value of the sync variable.
  */
  proc _syncvar.readFF() {
    return wrapped.readFF();
  }

  /*
    1) Read the value of the sync variable
    2) Do not inspect or change the full/empty state

    :returns: The value of the sync variable.
  */
  proc _syncvar.readXX() {
    // Yield to allow readXX in a loop to make progress
    chpl_task_yield();
    return wrapped.readXX();
  }

  /*
    1) Block until the sync variable is empty.
    2) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeEF(x : valType) {
    wrapped.writeEF(x);
  }

  /*
    1) Block until the sync variable is full.
    2) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeFF(x : valType) {
    wrapped.writeFF(x);
  }

  /*
    1) Write the value of the sync variable and leave the variable full

    :arg val: New value of the sync variable.
  */
  proc _syncvar.writeXF(x : valType) {
    wrapped.writeXF(x);
  }

  /*
    Resets the value of this sync variable to the default value of
    its type. This method is non-blocking and the state of the sync
    variable is set to empty when this method completes.
  */
  proc _syncvar.reset() {
    wrapped.reset();
  }

  /*
     Determine if the sync variable is full without blocking.
     Does not alter the state of the sync variable

     :returns: true if the state of the sync variable is full.
  */
  proc _syncvar.isFull {
    return wrapped.isFull;
  }

  proc   = (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(rhs);
  }

  proc  += (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() +  rhs);
  }

  proc  -= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() -  rhs);
  }

  proc  *= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() *  rhs);
  }

  proc  /= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() /  rhs);
  }

  proc  %= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() %  rhs);
  }

  proc **= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() ** rhs);
  }

  proc  &= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() &  rhs);
  }

  proc  |= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() |  rhs);
  }

  proc  ^= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() ^  rhs);
  }

  proc >>= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() >> rhs);
  }

  proc <<= (ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() << rhs);
  }

  pragma "init copy fn"
  proc chpl__initCopy(ref sv : _syncvar(?t)) {
    return sv.readFE();
  }

  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(const ref rhs : _syncvar) {
    return new _syncvar(rhs);
  }

  // Be explicit about whether syncs are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x : _syncvar(?t)) param return true;

  // This version has to be available to take precedence
  inline proc chpl__autoDestroy(x : _syncvar(?)) {
    if x.isOwned == true then
      delete x.wrapped;
  }

  pragma "no doc"
  proc chpl__readXX(const ref x : _syncvar(?)) return x.readXX();

  proc <=>(lhs : _syncvar, ref rhs) {
    const tmp = lhs;

    lhs = rhs;
    rhs = tmp;
  }

  proc <=>(ref lhs, rhs : _syncvar) {
    const tmp = lhs;

    lhs = rhs;
    rhs = tmp;
  }

  proc <=>(lhs : _syncvar, rhs : _syncvar) {
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

  pragma "no doc"
  class _synccls {
    type valType;

    var  value   : valType;
    var  syncAux : chpl_sync_aux_t;      // Locking, signaling, ...

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      this.valType = valType;
      this.complete();
      chpl_sync_initAux(syncAux);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      chpl_sync_destroyAux(syncAux);
    }

    proc readFE() {
      var ret : valType;

      on this {
        var localRet : valType;

        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        localRet = value;

        chpl_sync_markAndSignalEmpty(syncAux);
        chpl_rmem_consist_acquire();

        ret = localRet;
      }

      return ret;
    }

    proc readFF() {
      var ret : valType;

      on this {
        var localRet : valType;

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
      var ret : valType;

      on this {
        var localRet : valType;

        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        localRet = value;

        chpl_sync_unlock(syncAux);
        chpl_rmem_consist_acquire();

        ret = localRet;
      }

      return ret;
    }

    proc writeEF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_waitEmptyAndLock(syncAux);

        value = val;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeFF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        value = val;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeXF(val : valType) lifetime this < val {
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
        const defaultValue : valType;

        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        value = defaultValue;

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
  class _qthreads_synccls {
    type valType;

    var  alignedValue : aligned_t;

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      this.valType = valType;
      this.complete();
      qthread_purge_to(alignedValue, defaultOfAlignedT(valType));
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      // There's no explicit destroy function, but qthreads reclaims memory
      // for full variables that have no pending operations
      qthread_fill(alignedValue);
    }

    proc readFE() {
      var ret : valType;

      on this {
        var alignedLocalRet : aligned_t;

        chpl_rmem_consist_release();
        qthread_readFE(alignedLocalRet, alignedValue);
        chpl_rmem_consist_acquire();

        ret = alignedLocalRet : valType;
      }

      return ret;
    }

    proc readFF() {
      var ret : valType;

      on this {
        var alignedLocalRet : aligned_t;

        chpl_rmem_consist_release();
        qthread_readFF(alignedLocalRet, alignedValue);
        chpl_rmem_consist_acquire();

        ret = alignedLocalRet : valType;
      }

      return ret;
    }

    proc readXX() {
      var ret : valType;

      on this {
        var alignedLocalRet : aligned_t;

        chpl_rmem_consist_release();
        qthread_readXX(alignedLocalRet, alignedValue);
        chpl_rmem_consist_acquire();

        ret = alignedLocalRet : valType;
      }

      return ret;
    }

    proc writeEF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        qthread_writeEF(alignedValue, val : aligned_t);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeFF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        qthread_writeFF(alignedValue, val : aligned_t);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeXF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        qthread_writeF(alignedValue, val : aligned_t);
        chpl_rmem_consist_acquire();
      }
    }

    proc reset() {
      on this {
        chpl_rmem_consist_release();
        qthread_purge_to(alignedValue, defaultOfAlignedT(valType));
        chpl_rmem_consist_acquire();
      }
    }

    proc isFull {
      var b : bool;

      on this {
        chpl_rmem_consist_release();
        b = qthread_feb_status(alignedValue) : bool;
        chpl_rmem_consist_acquire();
      }

      return b;
    }
  }

  pragma "no doc"
  proc isSyncValue(x : sync) param  return true;

  pragma "no doc"
  proc isSyncValue(x)       param  return false;





  /************************************ | *************************************
  *                                                                           *
  * The record wrapper to implement single                                    *
  *                                                                           *
  ************************************* | ************************************/

  pragma "single"
  pragma "default intent is ref"
  pragma "no doc"
  record _singlevar {
    type valType;                              // The compiler knows this name

    var  wrapped : unmanaged _singlecls(valType);
    var  isOwned : bool                = true;

    proc init(type valType) {
      ensureFEType(valType);
      this.valType = valType;
      wrapped = new unmanaged _singlecls(valType);
    }

    //
    // This is technically a copy-initializer, but it's only called through
    // chpl__autoCopy.
    //
    // Also note that singles **need** to use chpl__initCopy in order to return
    // a different type. For example:
    //
    //   var a = x$;
    //
    // ``a`` needs to be a ``valType``, not a single.
    //
    pragma "dont disable remote value forwarding"
    proc init(const other : _singlevar) {
      this.valType = other.valType;
      wrapped = other.wrapped;
      isOwned = false;
    }

    proc init=(const other : _singlevar) {
      // Allow initialization from compatible single variables, e.g.:
      //   var x : single int = 5;
      //   var y : single real = x;
      if isCoercible(other.valType, this.type.valType) == false {
        param theseTypes = "'" + this.type:string + "' from '" + other.type:string + "'";
        param because = "because '" + other.valType:string + "' is not coercible to '" + this.type.valType:string + "'";
        compilerError("cannot initialize ", theseTypes, " ",  because);
      }
      this.init(this.type.valType);
      this.writeEF(other.readFF());
    }

    pragma "dont disable remote value forwarding"
    proc init=(const other : this.type.valType) {
      this.init(other.type);
      this.writeEF(other);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      if isOwned == true then
        delete wrapped;
    }

    // Do not allow implicit reads of single vars.
    proc readThis(x) throws {
      compilerError("single variables cannot currently be read - use writeEF instead");
    }

    // Do not allow implicit writes of single vars.
    proc writeThis(x) throws {
      compilerError("single variables cannot currently be written - apply readFF() to those variables first");
     }
  }

  /*
     Noakes 2016/08/12

     These are defined as secondary methods so that chpldoc can render the
     documentation
  */

  pragma "no doc"
  proc isSingleType(type t:_singlevar) param {
    return true;
  }

  /* Returns true if `t` is a single type, false otherwise. */
  proc isSingleType(type t) param {
    return false;
  }

  /*
    1) Block until the single variable is full.
    2) Read the value of the single variable and leave the variable full

    :returns: The value of the single variable.
  */
  proc _singlevar.readFF() {
    return wrapped.readFF();
  }

  /*
    1) Read the value of the single variable
    2) Do not inspect or change the full/empty state

    :returns: The value of the single variable.
  */
  proc _singlevar.readXX() {
    // Yield to allow readXX in a loop to make progress
    chpl_task_yield();
    return wrapped.readXX();
  }

  /*
    1) Block until the single variable is empty.
    2) Write the value of the single variable and leave the variable full

    :arg val: New value of the single variable.
  */
  proc _singlevar.writeEF(x : valType) {
    wrapped.writeEF(x);
  }

  /*
     Determine if the single variable is full without blocking.
     Does not alter the state of the single variable

     :returns: true if the state of the single variable is full.
  */
  proc _singlevar.isFull {
    return wrapped.isFull;
  }

  proc =(ref lhs : _singlevar(?t), rhs : t) {
    lhs.wrapped.writeEF(rhs);
  }

  pragma "init copy fn"
  proc chpl__initCopy(ref sv : _singlevar(?t)) {
    return sv.readFF();
  }

  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(const ref rhs : _singlevar) {
    return new _singlevar(rhs);
  }

  // Be explicit about whether singles are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x : _singlevar(?t)) param return true;

  // This version has to be available to take precedence
  inline proc chpl__autoDestroy(x : _singlevar(?)) {
    if x.isOwned == true then
      delete x.wrapped;
  }

  pragma "no doc"
  proc chpl__readXX(const ref x : _singlevar(?)) return x.readXX();

  /************************************ | *************************************
  *                                                                           *
  * Use of a class instance establishes the required identity property.       *
  *                                                                           *
  * Potential future optimization: Some targets could rely on a class that    *
  * omits the singleAux variable for sufficiently simple valType.             *
  *                                                                           *
  ************************************* | ************************************/



  pragma "no doc"
  class _singlecls {
    type valType;

    var  value     : valType;
    var  singleAux : chpl_single_aux_t;      // Locking, signaling, ...

    proc init(type valType) {
      this.valType = valType;
      this.complete();
      chpl_single_initAux(singleAux);
    }

    proc deinit() {
      chpl_single_destroyAux(singleAux);
    }

    proc readFF() {
      var ret : valType;

      on this {
        var localRet : valType;

        chpl_rmem_consist_release();

        if this.isFull then
          localRet = value;
        else {
          chpl_single_waitFullAndLock(singleAux);   // single_wait_full
          localRet = value;
          chpl_single_markAndSignalFull(singleAux);
        }

        chpl_rmem_consist_acquire();

        ret = localRet;
      }

      return ret;
    }

    proc readXX() {
      var ret : valType;

      on this {
        var localRet : valType;

        chpl_rmem_consist_release();

        if this.isFull then
          localRet = value;
        else {
          chpl_single_lock(singleAux);
          localRet = value;
          chpl_single_unlock(singleAux);
        }

        chpl_rmem_consist_acquire();
        ret = localRet;
      }

      return ret;
    }

    proc writeEF(val : valType) lifetime this < val {
      on this {
        chpl_rmem_consist_release();
        chpl_single_lock(singleAux);

        if this.isFull then
          halt("single var already defined");

        value = val;

        chpl_single_markAndSignalFull(singleAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc isFull {
      var b : bool;

      on this {
        chpl_rmem_consist_release();
        b = chpl_single_isFull(c_ptrTo(value), singleAux);
        chpl_rmem_consist_acquire();
      }

      return b;
    }
  }

  pragma "no doc"
  proc isSingleValue(x : single) param  return true;

  pragma "no doc"
  proc isSingleValue(x)         param  return false;
}


private module SyncVarRuntimeSupport {
  private use ChapelStandard, SysCTypes;
  use AlignedTSupport;

  //
  // Sync var externs
  //

  // Implementation is target dependent and opaque to Chapel code
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

  extern proc   chpl_sync_isFull(value   : c_void_ptr,
                                 ref aux : chpl_sync_aux_t) : bool;


  //
  // Single var externs
  //

  // Implementation is target dependent and opaque to Chapel code
  extern record chpl_single_aux_t { };

  extern proc   chpl_single_initAux(ref aux : chpl_single_aux_t);
  extern proc   chpl_single_destroyAux(ref aux : chpl_single_aux_t);

  pragma "insert line file info"
  extern proc   chpl_single_waitFullAndLock (ref aux : chpl_single_aux_t);

  extern proc   chpl_single_lock  (ref aux : chpl_single_aux_t);
  extern proc   chpl_single_unlock(ref aux : chpl_single_aux_t);

  extern proc   chpl_single_markAndSignalFull (ref aux : chpl_single_aux_t);

  extern proc   chpl_single_isFull(value   : c_void_ptr,
                                   ref aux : chpl_single_aux_t) : bool;


  //
  // Native qthreads sync var helpers and externs
  //

  // native qthreads aligned_t sync vars only work on 64-bit platforms right
  // now, and we only support casting between certain types and aligned_t
  proc supportsNativeSyncVar(type t) param {
    return CHPL_TASKS == "qthreads"    &&
           castableToAlignedT(t) &&
           numBits(c_uintptr) == 64;
  }

  extern proc qthread_readFE     (ref dest : aligned_t, const ref src: aligned_t) : c_int;
  extern proc qthread_readFF     (ref dest : aligned_t, const ref src: aligned_t) : c_int;
  extern proc qthread_readXX     (ref dest : aligned_t, const ref src: aligned_t) : c_int;

  extern proc qthread_writeEF    (ref dest : aligned_t, const ref src: aligned_t) : c_int;
  extern proc qthread_writeFF    (ref dest : aligned_t, const ref src: aligned_t) : c_int;
  extern proc qthread_writeF     (ref dest : aligned_t, const ref src: aligned_t) : c_int;

  extern proc qthread_purge_to   (ref dest : aligned_t, const ref src: aligned_t) : c_int;
  extern proc qthread_empty      (const ref dest : aligned_t) : c_int;
  extern proc qthread_fill       (const ref dest : aligned_t) : c_int;
  extern proc qthread_feb_status (const ref dest : aligned_t) : c_int;
}


// Support for aligned_t including the definition, casts, defaultValue, and
// read/write routines. aligned_t is the type used by native qthreads sync vars
private module AlignedTSupport {

  // Implemented in qthreads tasking layer
  extern type aligned_t;

  // Support casts to/from uint/int/bool. Note that for qthreads aligned_t is
  // an aligned uint64_t, so casting to/from int assumes 2's compliment.
  //
  // TODO add support for casting to/from any primitive type that's no bigger
  // than 64-bits. This will require doing a memcpy for most of them though.
  proc castableToAlignedT(type t) param {
    return isIntegralType(t) || isBoolType(t);
  }

  inline proc _cast(type t:aligned_t, x : integral) {
    return __primitive("cast", t, x);
  }
  inline proc _cast(type t:aligned_t, x : bool) {
    return __primitive("cast", t, x);
  }
  inline proc _cast(type t:chpl_anybool, x : aligned_t) {
    return __primitive("cast", t, x);
  }
  inline proc _cast(type t:integral, x : aligned_t) {
    return __primitive("cast", t, x);
  }

  // not just _defaultOf, since the default depends on the "baseType"
  inline proc defaultOfAlignedT(type t) {
    const defaultValue : t;
    return defaultValue : aligned_t;
  }

  // read/write support
  proc aligned_t.writeThis(f) throws {
    var tmp : uint(64) = this : uint(64);
    f <~> tmp;
  }
  proc aligned_t.readThis(f) throws {
    var tmp : uint(64);
    f <~> tmp;
    this = tmp : aligned_t;
  }
}
