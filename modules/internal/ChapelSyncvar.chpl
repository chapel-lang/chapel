/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
*/

module ChapelSyncvar {
  use ChapelStandard;

  use AlignedTSupport;
  use MemConsistency;
  use SyncVarRuntimeSupport;

  use CTypes;
  use OS.POSIX;

  /************************************ | *************************************
  *                                                                           *
  * The implementation of the user-facing sync types are exposed to           *
  * the compiler as a pair of record-wrapped classes.                         *
  *                                                                           *
  * The record implements the compiler facing API and manages the memory      *
  * associated with the underlying class while the class provides the         *
  * identity behavior required for the semantics of sync/single.              *
  *                                                                           *
  ************************************* | ************************************/

  //
  // The following types are OK for sync variables to contain
  // because they represent a single logical value.  (Note that for
  // the class it's the reference to the object that has full/empty
  // semantics.  Note that this includes the internal type
  // chpl_taskID_t in order to keep parallel/taskPar/sungeun/private.chpl
  // working, but this does not seem to be more broadly necessary.
  //

  private proc isSupported(type t) param do
    return isBoolType(t)          ||
           isIntegralType(t)      ||
           isRealType(t)          ||
           isImagType(t)          ||
           isComplexType(t)       ||
           isEnumType(t)          ||
           isClassType(t)         ||
           isStringType(t)        ||
           isBytesType(t)         ||
           isRecordType(t)        ||
           isRangeType(t)         ||
           t == chpl_taskID_t;

  private proc ensureFEType(type t) {
    if isSupported(t) == false then
      compilerError("sync types cannot contain type '", t : string, "'");

    if isGenericType(t) then
      compilerError("sync types cannot contain generic types");
  }

  @chpldoc.nodoc
  config param useNativeSyncVar = true;

  // use native sync vars if they're enabled and supported for the valType
  private proc getSyncClassType(type valType) type {
    if useNativeSyncVar && supportsNativeSyncVar(valType) {
      return unmanaged _qthreads_synccls(valType);
    } else {
      return unmanaged _synccls(valType);
    }
  }

  // TODO: Should we replace this with functionality from 'MemMove'? Would
  // that cause problems with the module initialization order?
  pragma "unsafe"
  @chpldoc.nodoc
  private inline proc _moveSet(ref dst: ?t, ref src: t) lifetime src == dst {
    __primitive("=", dst, src);
  }

  pragma "unsafe"
  @chpldoc.nodoc
  private inline proc _retEmptyVal(type t) {
    pragma "no init"
    pragma "no auto destroy"
    var ret: t;
    // It will be an error to read the empty value
    // but we zero it just in case
    memset(c_addrOf(ret), 0, c_sizeof(t));
    return ret;
  }

  proc chpl__readXX(x) do return x;

  // TODO Jade 7/5/23: supports deprecation of returning atomics by value
  // can be removed when that is
  proc chpl__readXX(x) where isAtomicType(x.type) do return x.read();

  /************************************ | *************************************
  *                                                                           *
  * The record wrapper to implement sync                                      *
  *                                                                           *
  ************************************* | ************************************/

  pragma "sync"
  pragma "default intent is ref"
  @chpldoc.nodoc
  record _syncvar : writeSerializable, readDeserializable {
    type valType;                              // The compiler knows this name

    var  wrapped : getSyncClassType(valType);
    var  isOwned : bool                      = true;

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      ensureFEType(valType);
      this.valType = valType;
      this.wrapped = new (getSyncClassType(valType))();
    }

    pragma "dont disable remote value forwarding"
    proc init(type valType, in value: valType) {
      ensureFEType(valType);
      this.valType = valType;
      this.wrapped = new (getSyncClassType(valType))(value);
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
    proc init(const ref other : _syncvar) {
      this.valType = other.valType;
      this.wrapped = other.wrapped;
      this.isOwned = false;
    }

    @deprecated(notes="Initializing a type-inferred variable from a 'sync' is deprecated; apply a 'read??()' method to the right-hand side")
    proc init=(const ref other: _syncvar(?)) {
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
    proc init=(in other : this.type.valType) {
      this.init(this.type.valType, other);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      if isOwned == true then
        delete wrapped;
    }

    // Do not allow implicit reads of sync vars.
    proc deserialize(reader, ref deserializer) throws {
      compilerError("sync variables cannot currently be read - use writeEF/writeFF instead");
    }

    @chpldoc.nodoc
    proc type deserializeFrom(reader, ref deserializer) throws {
      var ret : this;
      compilerError("sync variables cannot currently be read - use writeEF/writeFF instead");
      return ret;
    }

    // Do not allow implicit writes of sync vars.
    proc serialize(writer, ref serializer) throws {
      compilerError("sync variables cannot currently be written - apply readFE/readFF() to those variables first");
    }
  }

  /*
    Read a full ``sync`` variable, leaving it empty.

    1) Block until the ``sync`` variable is full.
    2) Read the value of the ``sync`` variable and set the variable to empty.

    :returns: The value of the ``sync`` variable.
  */
  proc _syncvar.readFE() {
    return wrapped.readFE();
  }

  /*  Read a full ``sync`` variable, leaving it full.

    1) Block until the ``sync`` variable is full.
    2) Read the value of the ``sync`` variable and leave the variable full.

    :returns: The value of the ``sync`` variable.
  */
  proc _syncvar.readFF() {
    return wrapped.readFF();
  }

  /*  Read a ``sync`` variable regardless of its state, leaving its state unchanged.

    1) Without blocking, read the value of the ``sync`` variable
    2) Leaving the state unchanged, return a value based on the current state:

      * full: return a copy of the stored value.
      * empty: return either a new default-initialized value of the stored type
        or, the last value stored (implementation dependent).

    :returns: The value of the ``sync`` variable.
  */
  @unstable("'readXX' is unstable")
  proc _syncvar.readXX() {
    // Yield to allow readXX in a loop to make progress
    currentTask.yieldExecution();
    return wrapped.readXX();
  }

  /* Write into an empty ``sync`` variable, leaving it full.

    1) Block until the ``sync`` variable is empty.
    2) Write the value of the ``sync`` variable and leave the variable full.

    :arg val: New value of the ``sync`` variable.
  */
  proc ref _syncvar.writeEF(in val : valType) {
    wrapped.writeEF(val);
  }

  /* Write into a full ``sync`` variable, leaving it full.

    1) Block until the ``sync`` variable is full.
    2) Write the value of the ``sync`` variable and leave the variable full.

    :arg val: New value of the ``sync`` variable.
  */
  @unstable("'writeFF' is unstable")
  proc ref _syncvar.writeFF(in val : valType) {
    wrapped.writeFF(val);
  }

  /* Write into a ``sync`` variable regardless of its state, leaving it full.

    1) Do not block.
    2) Write the value of the ``sync`` variable, leave it's state full.

    :arg val: New value of the ``sync`` variable.
  */
  @unstable("'writeXF' is unstable")
  proc ref _syncvar.writeXF(in val : valType) {
    wrapped.writeXF(val);
  }

  /*
    Resets the value of this ``sync`` variable to the default value of
    its type. This method is non-blocking and the state of the ``sync``
    variable is set to empty when this method completes.
  */
  @unstable("'reset' is unstable")
  proc ref _syncvar.reset() {
    wrapped.reset();
  }

  /*
    Determine if the ``sync`` variable is full without blocking.
    Does not alter the state of the ``sync`` variable.

    :returns: ``true`` if the state of the ``sync`` variable is full, ``false`` if it's empty.
  */
  @unstable("'isFull' is unstable")
  proc _syncvar.isFull {
    return wrapped.isFull;
  }

  @chpldoc.nodoc
  @deprecated(notes="Direct assignment to 'sync' variables is deprecated; apply a 'write??()' method to modify one")
  operator =(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(rhs);
  }

  @chpldoc.nodoc
  inline operator :(from, type t:_syncvar)
  where from.type == t.valType {
    return new _syncvar(from);
  }

  @chpldoc.nodoc
  @deprecated(notes="Casting sync variables is deprecated")
  inline operator :(from: _syncvar, type toType:_syncvar) {
    // TODO: this doesn't seem right - it doesn't use toType
    return new _syncvar(from);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator +=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() +  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator -=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() -  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator *=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() *  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator /=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() /  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator %=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() %  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator **=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() ** rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator &=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() &  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator |=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() |  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator ^=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() ^  rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator >>=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() >> rhs);
  }

  @chpldoc.nodoc
  @deprecated("'op=' assignments to 'sync' variables are deprecated; add explicit '.read??'/'.write??' methods to modify one")
  operator <<=(ref lhs : _syncvar(?t), rhs : t) {
    lhs.wrapped.writeEF(lhs.wrapped.readFE() << rhs);
  }

  // TODO Jade: remove me when compiler generated inits are removed
  proc chpl__compilerGeneratedAssignSyncSingle(ref lhs: _syncvar(?),
                                               ref rhs: _syncvar(?)) {
    // TODO: Should this clone the value and the full/empty state instead?
    lhs.writeEF(rhs.readFE());
  }

// TODO Jade: remove me when compiler generated inits are removed
  proc chpl__compilerGeneratedCopySync(ref sv : _syncvar(?)) {
    // TODO: this should probably clone the value and full/empty state instead
    var ret: sv.type = sv.readFE();
    return ret;
  }

  pragma "init copy fn"
  @deprecated(notes="Initializing a type-inferred variable from a 'sync' is deprecated; apply a '.read??()' method to the right-hand side")
  proc chpl__initCopy(ref sv : _syncvar(?t), definedConst: bool) {
    return sv.readFE();
  }

  pragma "auto copy fn"
  proc chpl__autoCopy(const ref rhs : _syncvar, definedConst: bool) {
    // Does it make sense to have a const sync? If so, can we make use of that
    // information here?
    return new _syncvar(rhs);
  }

  // Be explicit about whether syncs are auto-destroyed.
  inline proc chpl__maybeAutoDestroyed(x : _syncvar(?t)) param do return true;

  // This version has to be available to take precedence
  inline proc chpl__autoDestroy(x : _syncvar(?)) {
    if x.isOwned == true then
      delete x.wrapped;
  }

  proc chpl__readXX(const ref x : _syncvar(?)) do return x.readXX();


  @chpldoc.nodoc
  @deprecated(notes="Swapping 'sync' variables is deprecated; perform the swap manually using explicit '.read??'/'.write??' methods")
  operator <=>(ref lhs : _syncvar, ref rhs) {
    const tmp = lhs.readFE();
    lhs.writeEF(rhs);
    rhs = tmp;
  }

  @chpldoc.nodoc
  @deprecated(notes="Swapping 'sync' variables is deprecated; perform the swap manually using explicit '.read??'/'.write??' methods")
  operator <=>(ref lhs, ref rhs : _syncvar) {
    const tmp = lhs;
    lhs = rhs.readFE();
    rhs.writeEF(tmp);
  }

  @chpldoc.nodoc
  @deprecated(notes="Swapping 'sync' variables is deprecated; perform the swap manually using explicit '.read??'/'.write??' methods")
  operator <=>(ref lhs : _syncvar, ref rhs : _syncvar) {
    const tmp = lhs.readFE();
    lhs.writeEF(rhs.readFE());
    rhs.writeEF(tmp);
  }

  /************************************ | *************************************
  *                                                                           *
  * Use of a class instance establishes the required identity property.       *
  *                                                                           *
  ************************************* | ************************************/

  @chpldoc.nodoc
  class _synccls {
    type valType;

    pragma "no auto destroy"
    var  value   : valType;

    var  syncAux : chpl_sync_aux_t;      // Locking, signaling, ...

    // If the sync variable is empty
    //   if valType can be default initialized, value is a
    //     default-initialized value
    //   if valType cannot be default initialized, value is
    //     zero'd memory, and calls that could access it should
    //     result in compilation error.

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      this.valType = valType;
      this.value = _retEmptyVal(valType);
      init this;
      chpl_sync_initAux(syncAux);
    }

    pragma "dont disable remote value forwarding"
    proc init(type valType, in value: valType) {
      this.valType = valType;
      this.value = value;
      init this;
      chpl_sync_initAux(syncAux);
      chpl_sync_lock(syncAux);
      chpl_sync_markAndSignalFull(syncAux);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      on this {
        if this.isFull {
          chpl__autoDestroy(value);
        }
        chpl_sync_destroyAux(syncAux);
      }
    }

    pragma "unsafe"
    proc readFE() {
      pragma "no init"
      pragma "no auto destroy"
      var ret : valType;

      on this {
        pragma "no init"
        pragma "no auto destroy"
        var localRet : valType;

        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        _moveSet(localRet, value);

        chpl_sync_markAndSignalEmpty(syncAux);
        chpl_rmem_consist_acquire();

        _moveSet(ret, localRet);
      }

      return ret;
    }

    pragma "unsafe"
    proc const readFF() {
      if !isConstCopyableType(valType) ||
         !isConstAssignableType(valType) {
        compilerError("readFF requires that the type contained in the sync variable be const-copyable and const-assignable");
      }

      pragma "no init"
      pragma "no auto destroy"
      var ret : valType;
      on this {
        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        // const-copy from value
        pragma "no auto destroy"
        var localRet: valType = value;

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();

        // assign back to the original locale
        _moveSet(ret, localRet);
      }

      return ret;
    }

    proc const readXX() {
      if !isDefaultInitializableType(valType) ||
         !isConstCopyableType(valType) ||
         !isConstAssignableType(valType) {
        compilerError("readXX requires that the type contained in the sync variable be default-initializable, const-copyable, and const-assignable");
      }

      var ret : valType;
      on this {
        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        var localRet : valType;

        if isPODType(valType) ||
           chpl_sync_isFull(c_addrOf(value), syncAux) {
          localRet = value;
        } else {
          // otherwise, just use the default value:
          // localRet already stores the default.
        }

        chpl_sync_unlock(syncAux);
        chpl_rmem_consist_acquire();

        // assign back to the original locale
        ret = localRet;
      }

      return ret;
    }

    proc writeEF(pragma "no auto destroy" in val : valType) lifetime this < val {
      on this {
        pragma "no init"
        pragma "no auto destroy"
        var localVal : valType;
        _moveSet(localVal, val);

        chpl_rmem_consist_release();
        chpl_sync_waitEmptyAndLock(syncAux);

        _moveSet(value, localVal);

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeFF(pragma "no auto destroy" in val : valType) lifetime this < val {
      on this {
        pragma "no init"
        pragma "no auto destroy"
        var localVal : valType;
        _moveSet(localVal, val);

        chpl_rmem_consist_release();
        chpl_sync_waitFullAndLock(syncAux);

        if chpl_sync_isFull(c_addrOf(value), syncAux) {
          chpl__autoDestroy(value);
        }
        _moveSet(value, localVal);

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc writeXF(pragma "no auto destroy" in val : valType) lifetime this < val {
      on this {
        pragma "no init"
        pragma "no auto destroy"
        var localVal : valType;
        _moveSet(localVal, val);

        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        if chpl_sync_isFull(c_addrOf(value), syncAux) {
          chpl__autoDestroy(value);
        }
        _moveSet(value, localVal);

        chpl_sync_markAndSignalFull(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc reset() {
      on this {
        chpl_rmem_consist_release();
        chpl_sync_lock(syncAux);

        if chpl_sync_isFull(c_addrOf(value), syncAux) {
          chpl__autoDestroy(value);
        }
        if isPODType(valType) {
          // assuming POD types are default initializable
          var defaultValue : valType;
          _moveSet(value, defaultValue);
        }

        chpl_sync_markAndSignalEmpty(syncAux);
        chpl_rmem_consist_acquire();
      }
    }

    proc isFull {
      var b : bool;

      on this {
        chpl_rmem_consist_release();
        b = chpl_sync_isFull(c_addrOf(value), syncAux);
        chpl_rmem_consist_acquire();
      }

      return b;
    }
  }

  @chpldoc.nodoc
  class _qthreads_synccls {
    type valType;

    var  alignedValue : aligned_t;

    pragma "dont disable remote value forwarding"
    proc init(type valType) {
      this.valType = valType;
      init this;
      // MPF: I think we can just call qthread_purge here
      // because all of the types supported here have a default of 0
      qthread_purge_to(alignedValue, defaultOfAlignedT(valType));
    }

    pragma "dont disable remote value forwarding"
    proc init(type valType, in value: valType) {
      // MPF: Should this call qthread_writeF_const ?
      this.init(valType);
      qthread_writeEF(alignedValue, value : aligned_t);
    }

    pragma "dont disable remote value forwarding"
    proc deinit() {
      // There's no explicit destroy function, but qthreads reclaims memory
      // for full variables that have no pending operations
      on this {
        qthread_fill(alignedValue);
      }
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

}


private module SyncVarRuntimeSupport {
  use ChapelStandard, CTypes;
  use AlignedTSupport;
  use CTypes;

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

  extern proc   chpl_sync_isFull(value   : c_ptr(void),
                                 ref aux : chpl_sync_aux_t) : bool;


  //
  // Native qthreads sync var helpers and externs
  //

  // native qthreads aligned_t sync vars only work on 64-bit platforms right
  // now, and we only support casting between certain types and aligned_t
  proc supportsNativeSyncVar(type t) param {
    use ChplConfig;
    return CHPL_TASKS == "qthreads" &&
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

  inline operator :(x : integral, type t:aligned_t) {
    return __primitive("cast", t, x);
  }
  inline operator :(x: bool, type t:aligned_t) {
    return __primitive("cast", t, x);
  }
  inline operator :(x : aligned_t, type t:bool) {
    return __primitive("cast", t, x);
  }
  inline operator :(x : aligned_t, type t:integral) {
    return __primitive("cast", t, x);
  }

  // not just _defaultOf, since the default depends on the "baseType"
  inline proc defaultOfAlignedT(type t) {
    const defaultValue : t;
    return defaultValue : aligned_t;
  }

  // read/write support
  proc aligned_t.deserialize(reader, ref deserializer) throws {
    this = reader.read(uint(64)) : aligned_t;
  }

  @chpldoc.nodoc
  proc aligned_t.serialize(writer, ref serializer) throws {
    var tmp : uint(64) = this : uint(64);
    writer.write(tmp);
  }
  proc type aligned_t.deserializeFrom(reader, ref deserializer) throws {
    var ret : aligned_t;
    ret.deserialize(reader, deserializer);
    return ret;
  }
}
