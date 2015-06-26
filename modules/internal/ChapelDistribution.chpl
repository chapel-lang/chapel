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

module ChapelDistribution {
  
  use List;

  extern proc chpl_task_yield();

  //
  // Abstract distribution class
  //
  pragma "base dist"
  class BaseDist {
    // The common case seems to be local access to this class, so we
    // will use explicit processor atomics, even when network
    // atomics are available
    var _distCnt: atomic_refcnt; // distribution reference count
    var _doms: list(BaseDom);   // domains declared over this distribution
    var _domsLock: atomicflag;  //   and lock for concurrent access
  
    pragma "dont disable remote value forwarding"
    proc destroyDist(): int {
      compilerAssert(!noRefCount);
      return decRefCount();
    }
  
    inline proc remove_dom(x) {
      on this {
        _lock_doms();
        _doms.remove(x);
        _unlock_doms();
      }
    }
  
    inline proc add_dom(x) {
      on this {
        _lock_doms();
        _doms.append(x);
        _unlock_doms();
      }
    }
  
    inline proc _lock_doms() {
      // WARNING: If you are calling this function directly from
      // a remote locale, you should consider wrapping the call in
      // an on clause to avoid excessive remote forks due to the
      // testAndSet()
      while (_domsLock.testAndSet()) do chpl_task_yield();
    }
  
    inline proc _unlock_doms() {
      _domsLock.clear();
    }
  
    proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool) {
      compilerError("rectangular domains not supported by this distribution");
    }
  
    proc dsiNewAssociativeDom(type idxType, param parSafe: bool) {
      compilerError("associative domains not supported by this distribution");
    }
  
    proc dsiNewAssociativeDom(type idxType, param parSafe: bool)
    where isEnumType(idxType) {
      compilerError("enumerated domains not supported by this distribution");
    }
  
    proc dsiNewOpaqueDom(type idxType, param parSafe: bool) {
      compilerError("opaque domains not supported by this distribution");
    }
  
    proc dsiNewSparseDom(param rank: int, type idxType, dom: domain) {
      compilerError("sparse domains not supported by this distribution");
    }
  
    inline proc incRefCount(cnt=1) {
      compilerAssert(!noRefCount);
      if debugDistRefCount > 0 {
        const cnt = _distCnt.read();
        if debugDistRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- INC _distCnt (%016lx) was %ld\n",
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_distCnt ", cnt, " is bogus!");
      }
      _distCnt.inc(cnt);
    }

    inline proc decRefCount() {
      compilerAssert(!noRefCount);
      const cnt = _distCnt.dec();
      if debugDistRefCount > 0 {
        if debugDistRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- DEC _distCnt (%016lx) now %ld\n",
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_distCnt ", cnt, " is bogus!");
        // Poison the distCount, so an attempt to move it away from zero a
        // second time will fail
        if cnt == 0 then _distCnt.dec();
      }
      return cnt;
    }

    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;
  
    proc dsiDestroyDistClass() { }
  
    proc dsiDisplayRepresentation() { }
  }
  
  //
  // Abstract domain classes
  //
  pragma "base domain"
  class BaseDom {
    // The common case seems to be local access to this class, so we
    // will use explicit processor atomics, even when network
    // atomics are available
    var _domCnt: atomic_refcnt; // domain reference count
    var _arrs: list(BaseArr);  // arrays declared over this domain
    var _arrsLock: atomicflag; //   and lock for concurrent access
  
    proc dsiMyDist(): BaseDist {
      halt("internal error: dsiMyDist is not implemented");
      return nil;
    }
  
    pragma "dont disable remote value forwarding"
    proc destroyDom(): int {
      compilerAssert(!noRefCount);
      return decRefCount();
    }

    inline proc remove_arr(x) {
      on this {
        _lock_arrs();
        _arrs.remove(x);
        _unlock_arrs();
      }
    }
  
    inline proc add_arr(x) {
      on this {
        _lock_arrs();
        _arrs.append(x);
        _unlock_arrs();
      }
    }
  
    inline proc _lock_arrs() {
      // WARNING: If you are calling this function directly from
      // a remote locale, you should consider wrapping the call in
      // an on clause to avoid excessive remote forks due to the
      // testAndSet()
      while (_arrsLock.testAndSet()) do chpl_task_yield();
    }
  
    inline proc _unlock_arrs() {
      _arrsLock.clear();
    }
  
    // used for associative domains/arrays
    proc _backupArrays() {
      for arr in _arrs do
        arr._backupArray();
    }
  
    proc _removeArrayBackups() {
      for arr in _arrs do
        arr._removeArrayBackup();
    }
  
    proc _preserveArrayElements(oldslot, newslot) {
      for arr in _arrs do
        arr._preserveArrayElement(oldslot, newslot);
    }
  
    inline proc incRefCount(cnt=1) {
      compilerAssert(!noRefCount);
      if debugDomRefCount > 0 {
        const cnt = _domCnt.read();
        if debugDomRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- INC _domCnt (%016lx) was %ld\n", 
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_domCnt ", cnt, " is bogus!");
      }
      _domCnt.inc(cnt);
    }

    inline proc decRefCount() {
      compilerAssert(!noRefCount);
      const cnt = _domCnt.dec(); //_domCnt.fetchSub(1)-1;
      if debugDomRefCount > 0 {
        if debugDomRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- DEC _domCnt (%016lx) now %ld\n", 
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_domCnt ", cnt, " is bogus!");
        // Poison the domCount, so an attempt to move it away from zero a
        // second time will fail
        if cnt == 0 then _domCnt.dec();
      }
      return cnt;
    }

    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;
  
    // false for default distribution so that we don't increment the
    // default distribution's reference count and add domains to the
    // default distribution's list of domains
    proc linksDistribution() param return true;
  
    // dynamically-dispatched counterpart of linksDistribution
    proc dsiLinksDistribution() return true;
  
    proc dsiDisplayRepresentation() { }
  }
  
  class BaseRectangularDom : BaseDom {
    proc dsiClear() {
      halt("clear not implemented for this distribution");
    }
  
    proc clearForIteratableAssign() {
      compilerError("Illegal assignment to a rectangular domain");
    }
  
    proc dsiAdd(x) {
      compilerError("Cannot add indices to a rectangular domain");
    }
  
    proc dsiRemove(x) {
      compilerError("Cannot remove indices from a rectangular domain");
    }
  }
  
  class BaseSparseDom : BaseDom {
    proc dsiClear() {
      halt("clear not implemented for this distribution");
    }
  
    proc clearForIteratableAssign() {
      dsiClear();
    }
  }
  
  class BaseAssociativeDom : BaseDom {
    proc dsiClear() {
      halt("clear not implemented for this distribution");
    }
  
    proc clearForIteratableAssign() {
      dsiClear();
    }
  }
  
  class BaseOpaqueDom : BaseDom {
    proc dsiClear() {
      halt("clear not implemented for this distribution");
    }
  
    proc clearForIteratableAssign() {
      dsiClear();
    }
  }
  
  //
  // Abstract array class
  //
  pragma "base array"
  class BaseArr {
    // The common case seems to be local access to this class, so we
    // will use explicit processor atomics, even when network
    // atomics are available
    var _arrCnt: atomic_refcnt; // array reference count
    var _arrAlias: BaseArr;    // reference to base array if an alias
  
    proc dsiStaticFastFollowCheck(type leadType) param return false;
  
    proc dsiGetBaseDom(): BaseDom {
      halt("internal error: dsiGetBaseDom is not implemented");
      return nil;
    }
  
    pragma "dont disable remote value forwarding"
    proc destroyArr(): int {
      compilerAssert(!noRefCount);
      var cnt = decRefCount();
      if cnt == 0 {
        if _arrAlias {
          on _arrAlias {
            var cnt = _arrAlias.destroyArr();
            if cnt == 0 then
              delete _arrAlias;
          }
        } else {
          dsiDestroyData();
        }
      }
      return cnt;
    }
  
    proc dsiDestroyData() { }
  
    proc dsiReallocate(d: domain) {
      halt("reallocating not supported for this array type");
    }
  
    proc dsiPostReallocate() {
    }
  
    // This method is unsatisfactory -- see bradc's commit entries of
    // 01/02/08 around 14:30 for details
    proc _purge( ind: int) {
      halt("purging not supported for this array type");
    }
  
    proc _resize( length: int, old_map) {
      halt("resizing not supported for this array type");
    }
  
    //
    // Ultimately, these routines should not appear here; instead, we'd
    // like to do a dynamic cast in the sparse array class(es) that call
    // these routines in order to call them directly and avoid the
    // dynamic dispatch and leaking of this name to the class.  In order
    // to do this we'd need to hoist eltType to the base class, which
    // would require better subclassing of generic classes.  A good
    // summer project for Jonathan?
    //
    proc sparseShiftArray(shiftrange, initrange) {
      halt("sparseGrowDomain not supported for non-sparse arrays");
    }
  
    proc sparseShiftArrayBack(shiftrange) {
      halt("sparseShiftArrayBack not supported for non-sparse arrays");
    }
  
    // methods for associative arrays
    proc clearEntry(idx, haveLock:bool = false) {
      halt("clearEntry() not supported for non-associative arrays");
    }
  
    proc _backupArray() {
      halt("_backupArray() not supported for non-associative arrays");
    }
  
    proc _removeArrayBackup() {
      halt("_removeArrayBackup() not supported for non-associative arrays");
    }
  
    proc _preserveArrayElement(oldslot, newslot) {
      halt("_preserveArrayElement() not supported for non-associative arrays");
    }
  
    inline proc incRefCount(cnt=1) {
      compilerAssert(!noRefCount);
      if debugArrRefCount > 0 {
        const cnt = _arrCnt.read();
        if debugArrRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- INC _arrCnt (%016lx) was %ld\n",
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_arrCnt ", cnt, " is bogus!");
      }
      _arrCnt.inc(cnt);
    }

    inline proc decRefCount() {
      compilerAssert(!noRefCount);
      const cnt = _arrCnt.dec(); //_arrCnt.fetchSub(1)-1;
      if debugArrRefCount > 0 {
        if debugArrRefCount > 1 {
          extern proc printf(fmt: c_string, arg...);
          printf("----- DEC _arrCnt (%016lx) now %ld\n",
                 __primitive("_wide_get_addr", this), cnt);
        }
        if cnt < 0 || cnt > 10000 then
          halt("_arrCnt ", cnt, " is bogus!");
        // Poison the arrCount, so an attempt to move it away from zero a
        // second time will fail
        if cnt == 0 then _arrCnt.dec();
      }
      return cnt;
    }

    proc dsiSupportsAlignedFollower() param return false;
  
    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;
  
    proc dsiSupportsBulkTransfer() param return false;
    proc doiCanBulkTransfer() param return false;
    proc doiBulkTransfer(B){ 
      halt("This array type does not support bulk transfer.");
    }
  
    proc dsiDisplayRepresentation() { }
    proc isDefaultRectangular() param return false;
    proc dsiSupportsBulkTransferInterface() param return false;
    proc doiCanBulkTransferStride() param return false;
  }
  
}
