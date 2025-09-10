/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
 * Collection of mutexes/locks.
 */
module ChapelLocks {
  private use Atomics, ChapelBase;
  private use MemConsistency;
  import CTypes.{c_ptr, c_ptrTo, c_ptrToConst};

  /*
   * Local processor atomic spinlock. Intended for situations with minimal
   * contention or very short critical sections.
   */
  pragma "default intent is ref"
  record chpl_LocalSpinlock {
    var l: chpl__processorAtomicType(bool);

    // override compiler-generated default initializer because we don't
    // ever want/need to pass an atomic into it (and someday it'll
    // just take bool rather than atomic bool, and until then, we get
    // a --warn-unstable error)
    proc init() {
    }
    proc init=(other: chpl_LocalSpinlock) {
      init this;
      this.l.init_helper(other.l.read());
    }

    inline proc ref lock() {
      use ChapelBase.currentTask;
      on this do
        while l.read() || l.testAndSet(memoryOrder.acquire) do
          yieldExecution();
    }

    inline proc ref unlock() {
      l.clear(memoryOrder.release);
    }

    inline proc ref enterContext() do lock();
    inline proc ref exitContext(in e: owned Error?) {
      defer unlock();
      // TODO: We shouldn't halt here, but 'throws' is currently awkward,
      //       because if your 'exitContext()' throws then it requires
      //       your 'manage' to be wrapped in a 'try' in non-prototype
      //       contexts. To fix this, we can make the 'contextManager'
      //       interface context-sensitive by providing a version of this
      //       method signature that doesn't throw.
      if e then halt(e!.message());
    }
  }

  // TODO: Transform this into an interface.
  record chpl_readWriteLockWrapper {
    type lockType;
    var _lock: lockType;

    // Corresponds to some locking interface's "acquire write lock" method.
    inline proc ref acquireWriteLock() where lockType == chpl_LocalSpinlock {
      _lock.lock();
    }

    // Corresponds to some locking interface's "acquire read lock" method.
    inline proc ref acquireReadLock() where lockType == chpl_LocalSpinlock {
      _lock.lock();
    }

    // Corresponds to some locking interface's "release write lock" method.
    inline proc ref releaseWriteLock() where lockType == chpl_LocalSpinlock {
      _lock.unlock();
    }

    // Corresponds to some locking interface's "release read lock" method.
    inline proc ref releaseReadLock() where lockType == chpl_LocalSpinlock {
      _lock.unlock();
    }
  }

  // The default lock type is a spinlock since that is what we use most often.
  type chpl_defaultLockGuardLockType = chpl_LocalSpinlock;

  /*  A lock guard is a useful abstraction which allows you to bundle both
      a lock and a data type together in the same variable. Access to the
      data is controlled via use of the guard as a context manager.

      E.g., given a guard 'g', one must write 'manage g.write() as x' to get
      access to the data. Getting access requires taking the lock. */
  pragma "default intent is ref"
  record chpl_lockGuard {
    type dataType;
    type lockType = chpl_defaultLockGuardLockType;
    var _data: dataType;
    var _lock: chpl_readWriteLockWrapper(lockType);

    proc init(type dataType, type lockType=chpl_defaultLockGuardLockType) {
      this.dataType = dataType;
      this.lockType = lockType;
    }

    proc init(in x: ?t, type lockType=chpl_defaultLockGuardLockType) {
      this.dataType = t;
      this.lockType = lockType;
      this._data = x;
    }

    // Wrapper to create a new context manager instance.
    inline proc ref _createAccessManager(param isWriteAccess: bool) {
      type t = chpl_lockGuardAccessManager;
      return new t(c_ptrTo(this), isWriteAccess);
    }

    /** Return a new context manager that provides read access. */
    inline proc ref read() do
      return _createAccessManager(isWriteAccess=false);

    /** Return a new context manager that provides write access. */
    inline proc ref write() do
      return _createAccessManager(isWriteAccess=true);

    /** Allows unlocked access to the data. */
    proc ref unsafeAccess() ref do return _data;
  }

  /** An access manager is a context manager meant for use with a 'manage'
      statement and is created by a call to one of the guard methods such
      as 'read()', 'write()', or 'lock()'. */
  // TODO: This was originally nested inside `chpl_lockGuard`, but that
  //       causes a bizarre problem with `--verify` in programs that
  //       emit compiler errors during the `resolve()` pass. See the
  //       pull request #27795 for a detailed explanation.
  record chpl_lockGuardAccessManager {
    type dataType;
    type lockType;
    param isWriteAccess: bool;
    const _guardPtr: c_ptr(chpl_lockGuard(dataType, lockType));

    inline proc init(ptr: c_ptr(chpl_lockGuard(?t1, ?t2)),
                     param isWriteAccess: bool) {
      this.dataType = t1;
      this.lockType = t2;
      this.isWriteAccess = isWriteAccess;
      this._guardPtr = ptr;
    }

    proc isReading param do return !isWriteAccess;
    proc isWriting param do return isWriteAccess;
    proc _guard ref do return _guardPtr.deref();

    pragma "fn returns infinite lifetime"
    inline proc ref enterContext() ref where isWriting {
      _guard._lock.acquireWriteLock();
      return _guardPtr.deref().unsafeAccess();
    }

    pragma "fn returns infinite lifetime"
    inline proc ref enterContext() const ref where isReading {
      _guard._lock.acquireReadLock();
      return _guardPtr.deref().unsafeAccess();
    }

    inline proc ref exitContext(in e: owned Error?) where isWriting {
      defer _guard._lock.releaseWriteLock();
      // TODO: We shouldn't halt here, but 'throws' is currently awkward.
      if e then halt(e!.message());
    }

    inline proc ref exitContext(in e: owned Error?) where isReading {
      defer _guard._lock.releaseReadLock();
      // TODO: We shouldn't halt here, but 'throws' is currently awkward.
      if e then halt(e!.message());
    }
  }

  chpl_LocalSpinlock implements contextManager;
  chpl_lockGuardAccessManager implements contextManager;
}
