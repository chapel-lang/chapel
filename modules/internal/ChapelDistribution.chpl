/*
 * Copyright 2004-2018 Cray Inc.
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
    var _doms: list(BaseDom);     // domains declared over this distribution
    var _domsLock: atomicbool;    //   and lock for concurrent access
    var _free_when_no_doms: bool; // true when the original _distribution
                                  // has been destroyed
    var pid:int = nullPid; // privatized ID, if privatization is supported

    proc deinit() {
    }

    // Returns a distribution that should be freed or nil.
    pragma "dont disable remote value forwarding"
    proc remove(): BaseDist {
      var free_dist = false;
      if dsiTrackDomains() {
        on this {
          var dom_count = -1;
          local {
            _lock_doms();
            // Set a flag to indicate it should be freed when _doms
            // becomes empty
            _free_when_no_doms = true;
            dom_count = _doms.size;
            _unlock_doms();
          }
          if dom_count == 0 then
            free_dist = true;
        }
      } else {
        // if we're not tracking domains, always free the
        // distribution when it goes out of scope.
        free_dist = true;
      }
      if free_dist then
        return this;
      else
        return nil;
    }

    // Returns true if the distribution should be removed.
    inline proc remove_dom(x:BaseDom): bool {
      var count = -1;
      on this {
        var cnt = -1;
        local {
          _lock_doms();
          _doms.remove(x);
          cnt = _doms.size;

          // add one for the main distribution instance
          if !_free_when_no_doms then
            cnt += 1;

          _unlock_doms();
        }
        count = cnt;
      }
      return (count==0);
    }

    //
    // TODO: There may be some opportunities to optimize out the
    // locking here, as in the add_arr() case.  For example, the
    // construction of the distribution and domain used for rank change
    // slicing could use an unlocked version because that operation
    // creates a new distribution followed immediately by a domain
    // over the distribution. It's unclear how important this
    // optimization is, though, because rank change slices are
    // arguably less common (and already more expensive in most cases
    // due to the creation of distribution and domain objects) than
    // rank-preserving slicing.
    //
    inline proc add_dom(x:BaseDom) {
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

    proc dsiNewRectangularDom(param rank: int, type idxType, param stridable: bool, inds) {
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

    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;

    proc dsiDestroyDist() { }

    proc dsiDisplayRepresentation() { writeln("<no way to display representation>"); }

    // Does the distribution keep a list of domains? Can the domains
    // keep the distribution alive longer? false for DefaultDist.
    proc trackDomains() param return true;

    // dynamically-dispatched counterpart of linksDistribution
    proc dsiTrackDomains() return true;

    // indicates if a distribution is a singleton. If so, we make no
    // effort to free it. DefaultDist is a singleton.
    proc singleton() param return false;
    // We could add dsiSingleton as a dynamically-dispatched counterpart
  }

  //
  // Abstract domain classes
  //
  pragma "base domain"
  class BaseDom {
    // The common case seems to be local access to this class, so we
    // will use explicit processor atomics, even when network
    // atomics are available
    var _arrs: list(BaseArr);  // arrays declared over this domain
    var _arrs_containing_dom: int; // number of arrays using this domain
                                   // as var A: [D] [1..2] real
                                   // is using {1..2}
    var _arrsLock: atomicbool; //   and lock for concurrent access
    var _free_when_no_arrs: bool;
    var pid:int = nullPid; // privatized ID, if privatization is supported

    proc deinit() {
    }

    proc dsiMyDist(): BaseDist {
      halt("internal error: dsiMyDist is not implemented");
      return nil;
    }

    // Returns (dom, dist).
    // if this domain should be deleted, dom=this; otherwise it is nil.
    // dist is nil or a distribution that should be removed.
    pragma "dont disable remote value forwarding"
    proc remove() : (BaseDom, BaseDist) {

      // TODO -- remove dsiLinksDistribution
      assert( dsiMyDist().dsiTrackDomains() == dsiLinksDistribution() );

      var ret_dom:BaseDom = nil;
      var ret_dist:BaseDist = nil;
      var dist = dsiMyDist();
      var free_dom = false;
      var remove_dist = false;
      on this {
        var arr_count = 0;
        // Count the number of arrays using this domain
        // and mark this domain to free itself when that number reaches 0.
        local {
          _lock_arrs();
          arr_count = _arrs.size;
          arr_count += _arrs_containing_dom;
          _free_when_no_arrs = true;
          _unlock_arrs();
        }

        if arr_count == 0 {
          free_dom = true;

          if dsiLinksDistribution() {
            // Remove the domain from the distribution
            // and find out if the distribution should be removed.
            remove_dist = dist.remove_dom(this);
          }
        }
      }
      if free_dom then
        ret_dom = this; // caller will delete this
      if remove_dist then
        ret_dist = dist; // caller will remove dist
      return (ret_dom, ret_dist);
    }

    // returns true if the domain should be removed
    inline proc remove_arr(x:BaseArr): bool {
      var count = -1;
      on this {
        var cnt = -1;
        local {
          _lock_arrs();
          _arrs.remove(x);
          cnt = _arrs.size;
          cnt += _arrs_containing_dom;
          // add one for the main domain record
          if !_free_when_no_arrs then
            cnt += 1;
          _unlock_arrs();
        }
        count = cnt;
      }
      return (count==0);
    }

    inline proc add_arr(x:BaseArr, param locking=true) {
      on this {
        if locking then
          _lock_arrs();
        _arrs.append(x);
        if locking then
          _unlock_arrs();
      }
    }

    inline proc remove_containing_arr(x:BaseArr): int {
      var count = -1;
      on this {
        _lock_arrs();
        _arrs_containing_dom -= 1;
        count = _arrs.size;
        count += _arrs_containing_dom;
        _unlock_arrs();
      }
      return count;
    }

    inline proc add_containing_arr(x:BaseArr) {
      on this {
        _lock_arrs();
        _arrs_containing_dom += 1;
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
    // MPF:  why do these need to be in BaseDom at all?
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

    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;

    // Does the distribution keep a list of domains? Can the
    // domains keep the distribution alive longer?
    proc linksDistribution() param return true;

    // dynamically-dispatched counterpart of linksDistribution
    proc dsiLinksDistribution() return true;

    // Overload to to customize domain destruction
    //
    // BHARSH 2017-02-05: Making dsiDestroyDom a virtual method 'tricks' the
    // compiler into thinking there's recursion for dsiDestroyDom when there is
    // none. This can result in incorrect generated code if recursive iterators
    // are inlined. See GitHub issue #5311 for more.
    //
    //proc dsiDestroyDom() { }

    proc dsiDisplayRepresentation() { writeln("<no way to display representation>"); }

    proc isDefaultRectangular() param return false;

    proc isSliceDomainView() param return false; // likely unnecessary?
    proc isRankChangeDomainView() param return false;
    proc isReindexDomainView() param return false;

    // proc dsiAssignDomain is a required overload to implement domain
    // assignment. It is not declared here because we do not wish
    // to use virtual dispatch to versions for all domain arguments.
    //
    // It should be declared like so:
    //     proc dsiAssignDomain(rhs: domain, lhsPrivate:bool)
    //
    // if lhsPrivate=true, the receiver is a private variable
    // that:
    //  hasn't yet been shared with other tasks
    //  doesn't yet have any arrays declared over it
    //
    // Note that assigning to a domain typically causes arrays
    // declared over that domain to be reallocated.
    //
    // Rectangular domains can implement this function with:
    //   proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    //     chpl_assignDomainWithGetSetIndices(this, rhs);
    //   }
    //
    // Irregular domains can implement this function with:
    //   proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
    //     chpl_assignDomainWithIndsIterSafeForRemoving(this, rhs);
    //   }
  }

  class BaseRectangularDom : BaseDom {
    param rank : int;
    type idxType;
    param stridable: bool;

    proc getBaseArrType() type {
      var tmp = new BaseArrOverRectangularDom(rank=rank, idxType=idxType, stridable=stridable);
      return tmp.type;
    }

    proc deinit() {
      // this is a bug workaround
    }

    proc dsiAdd(x) {
      compilerError("Cannot add indices to a rectangular domain");
      return 0;
    }

    proc dsiRemove(x) {
      compilerError("Cannot remove indices from a rectangular domain");
      return 0;
    }
  }

  class BaseSparseDomImpl : BaseSparseDom {

    var nnzDom = {1..nnz};

    proc deinit() {
      // this is a bug workaround
    }

    proc dsiBulkAdd(inds: [] index(rank, idxType),
        dataSorted=false, isUnique=false, preserveInds=true){

      if !dataSorted && preserveInds {
        var _inds = inds;
        return bulkAdd_help(_inds, dataSorted, isUnique);
      }
      else {
        return bulkAdd_help(inds, dataSorted, isUnique);
      }
    }

    proc bulkAdd_help(inds: [?indsDom] index(rank, idxType),
        dataSorted=false, isUnique=false){
      halt("Helper function called on the BaseSparseDomImpl");

      return -1;
    }

    // TODO: Would ChapelArray.resizeAllocRange() be too expensive?
    //       - would have to put the conditional outside of the call
    /* Grow domain if necessary */
    inline proc _grow(size: int, factor=arrayAsVecGrowthFactor) {
      const oldNNZDomSize = nnzDom.size;
      if (size > oldNNZDomSize) {
        const _newNNZDomSize = if (oldNNZDomSize) then ceil(factor*oldNNZDomSize):int else 1;
        nnzDom = {1..#_newNNZDomSize};
      }
    }

    // TODO: Would ChapelArray.resizeAllocRange() be too expensive?
    //       - would have to put the conditional outside of the call
    /* Shrink domain if necessary */
    inline proc _shrink(size: int, factor=arrayAsVecGrowthFactor) {
      if size == 0 {
        nnzDom = {1..0};
      } else {
        // TODO: floor() ?
        const shrinkThreshold = (nnzDom.size / (factor**2)): int;
        if (size < shrinkThreshold) {
          const _newNNZDomSize = (nnzDom.size / factor): int;
          nnzDom = {1.._newNNZDomSize};
        }
      }
    }

    // This method assumes nnz is updated according to the size
    // requested. So, a bulk addition into a sparse domain should: (1)
    // calculate new nnz and update it, (2) call this method, (3) add
    // indices
    inline proc _bulkGrow() {
      if (nnz > nnzDom.size) {
        const _newNNZDomSize = (exp2(log2(nnz)+1.0)):int;

        nnzDom = {1.._newNNZDomSize};
      }
    }

    inline proc _countDuplicates(arr) where isArray(arr) {
      var dupCount = -1;
      var prev = arr[arr.domain.low];
      for a in arr {
        if a == prev then
          dupCount += 1;
        else
          prev = a;;
      }
      return dupCount;
    }

    // (1) sorts indices if !dataSorted
    // (2) verifies the flags are set correctly if boundsChecking
    // (3) checks OOB if boundsChecking
    proc bulkAdd_prepareInds(inds, dataSorted, isUnique, cmp) {
      use Sort;
      if !dataSorted then sort(inds, comparator=cmp);

      //verify sorted and no duplicates if not --fast
      if boundsChecking {
        if dataSorted && !isSorted(inds, comparator=cmp) then
          halt("bulkAdd: Data not sorted, call the function with \
              dataSorted=false");

        //check duplicates assuming sorted
        if isUnique {
          const indsStart = inds.domain.low;
          const indsEnd = inds.domain.high;
          var lastInd = inds[indsStart];
          for i in indsStart+1..indsEnd {
            if inds[i] == lastInd then
              halt("bulkAdd: There are duplicates, call the function \
                  with isUnique=false");
            else lastInd = inds[i];
          }
        }

        //check OOB
        for i in inds do boundsCheck(i);
      }
    }

    // this is a helper function for bulkAdd functions in sparse subdomains.
    // NOTE:it assumes that nnz array of the sparse domain has non-negative
    // indices. If, for some reason it changes, this function and bulkAdds have to
    // be refactored. (I think it is a safe assumption at this point and keeps the
    // function a bit cleaner than some other approach. -Engin)
    proc __getActualInsertPts(d, inds, isUnique) {

      //find individual insert points
      //and eliminate duplicates between inds and dom
      var indivInsertPts: [inds.domain] int;
      var actualInsertPts: [inds.domain] int; //where to put in newdom

      //eliminate duplicates --assumes sorted
      if !isUnique {
        //make sure lastInd != inds[inds.domain.low]
        var lastInd = inds[inds.domain.low] + 1;
        for (i, p) in zip(inds, indivInsertPts)  {
          if i == lastInd then p = -1;
          else lastInd = i;
        }
      }

      forall (i,p) in zip(inds, indivInsertPts) {
        if isUnique || p != -1 { //don't do anything if it's duplicate
          const (found, insertPt) = d.find(i);
          p = if found then -1 else insertPt; //mark as duplicate
        }
      }

      //shift insert points for bulk addition
      //previous indexes that are added will cause a shift in the next indexes
      var actualAddCnt = 0;

      //NOTE: this can also be done with scan
      for (ip, ap) in zip(indivInsertPts, actualInsertPts) {
        if ip != -1 {
          ap = ip + actualAddCnt;
          actualAddCnt += 1;
        }
        else ap = ip;
      }

      return (actualInsertPts, actualAddCnt);
    }

    proc dsiClear(){
      halt("not implemented");
    }

  }

  class BaseSparseDom : BaseDom {
    // rank and idxType will be moved to BaseDom
    param rank: int;
    type idxType;
    var parentDom;

    // We currently cannot have dist here. It is due to a compiler bug due to
    // inheritance of generic var fields.
    // var dist;

    var nnz = 0; //: int;

    proc deinit() {
      // this is a bug workaround
    }

    proc dsiClear() {
      halt("clear not implemented for this distribution - BaseSparseDom");
    }

    proc dsiBulkAdd(inds: [] index(rank, idxType),
        dataSorted=false, isUnique=false, preserveInds=true){

      halt("Bulk addition is not supported by this sparse domain");
    }

    proc boundsCheck(ind: index(rank, idxType)):void {
      if boundsChecking then
        if !(parentDom.member(ind)) then
          halt("Sparse domain/array index out of bounds: ", ind,
              " (expected to be within ", parentDom, ")");
    }

    //basic DSI functions
    proc dsiDim(d: int) { return parentDom.dim(d); }
    proc dsiDims() { return parentDom.dims(); }
    proc dsiNumIndices { return nnz; }
    proc dsiSize { return nnz; }
    proc dsiLow { return parentDom.low; }
    proc dsiHigh { return parentDom.high; }
    proc dsiStride { return parentDom.stride; }
    proc dsiAlignment { return parentDom.alignment; }
    proc dsiFirst {
      halt("dsiFirst is not implemented");
      const _tmp: rank*idxType;
      return _tmp;
    }
    proc dsiLast {
      halt("dsiLast not implemented");
      const _tmp: rank*idxType;
      return _tmp;
    }
    proc dsiAlignedLow { return parentDom.alignedLow; }
    proc dsiAlignedHigh { return parentDom.alignedHigh; }

  } // end BaseSparseDom


  class BaseAssociativeDom : BaseDom {
    proc deinit() {
      // this is a bug workaround
    }

    proc dsiClear() {
      halt("clear not implemented for this distribution");
    }

    proc dsiAdd(idx) {
      compilerError("Index addition is not supported by this domain");
      return 0;
    }

  }

  class BaseOpaqueDom : BaseDom {
    proc deinit() {
      // this is a bug workaround
    }

    proc dsiClear() {
      halt("clear not implemented for this distribution");
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
    var pid:int = nullPid; // privatized ID, if privatization is supported
    var _decEltRefCounts : bool = false;

    proc isSliceArrayView() param {
      return false;
    }

    proc isRankChangeArrayView() param {
      return false;
    }

    proc isReindexArrayView() param {
      return false;
    }

    proc deinit() {
    }

    proc dsiStaticFastFollowCheck(type leadType) param return false;

    proc dsiGetBaseDom(): BaseDom {
      halt("internal error: dsiGetBaseDom is not implemented");
      return nil;
    }

    // returns (arr, dom)
    // arr is this if it should be deleted, or nil.
    // dom is a domain that should be removed, or nil.
    pragma "dont disable remote value forwarding"
    proc remove() {
      var ret_arr = this; // this array is always deleted
      var ret_dom:BaseDom = nil;
      var rm_dom = false;

      var dom = dsiGetBaseDom();
      // Remove the array from the domain
      // and find out if the domain should be removed.
      rm_dom = dom.remove_arr(this);

      if rm_dom then
        ret_dom = dom;

      return (ret_arr, ret_dom);
    }

    proc dsiDestroyArr() { }

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

    proc sparseBulkShiftArray(shiftMap, oldnnz) {
      halt("sparseBulkShiftArray not supported for non-sparse arrays");
    }

    // methods for associative arrays
    // MPF:  why do these need to be in BaseDom at all?
    proc clearEntry(idx) {
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

    proc dsiSupportsAlignedFollower() param return false;

    proc dsiSupportsPrivatization() param return false;
    proc dsiRequiresPrivatization() param return false;

    proc dsiDisplayRepresentation() { writeln("<no way to display representation>"); }
    proc isDefaultRectangular() param return false;

    proc doiCanBulkTransferRankChange() param return false;
  }

  /* BaseArrOverRectangularDom has this signature so that dsiReallocate
     can be overridden with the right tuple size.

     Note that eltType is not included here. eltType could be included
     in a base class, but here we're looking for a way to narrow
     overloaded functions to only those working with a particular
     kind of bounding box. So if eltType is included, we should make
     another base class.
   */
  pragma "base array"
  class BaseArrOverRectangularDom: BaseArr {
    param rank : int;
    type idxType;
    param stridable: bool;

    // the dsiReallocate to overload only uses the argument with
    // the matching tuple of ranges.

    // Q. Should this pass in a BaseRectangularDom or ranges?
    proc dsiReallocate(bounds:rank*range(idxType,BoundedRangeType.bounded,stridable)) {
      halt("reallocating not supported for this array type");
    }

    proc dsiPostReallocate() {
    }

    proc deinit() {
      // this is a bug workaround
    }


  }

  pragma "base array"
  class BaseRectangularArr: BaseArrOverRectangularDom {
    /* rank, idxType, stridable are from BaseArrOverRectangularDom */
    type eltType;

    proc deinit() {
      // this is a bug workaround
    }
  }

  /*
   * BaseSparseArr is very basic/generic so that we have some flexibility in
   * implementing sparse array classes.
   */
  pragma "base array"
  class BaseSparseArr: BaseArr {
    type eltType;
    param rank : int;
    type idxType;

    var dom; /* : DefaultSparseDom(?); */

    // NOTE I tried to put `data` in `BaseSparseArrImpl`. However, it wasn't
    // clear how to initialize this in that class.
    pragma "local field"
    var data: [dom.nnzDom] eltType;

    proc dsiGetBaseDom() return dom;

    proc deinit() {
      // this is a bug workaround
    }
  }

  /*
   * All the common helpers/methods in implementations of internal sparse arrays
   * go here.
   */
  pragma "base array"
  class BaseSparseArrImpl: BaseSparseArr {

    proc deinit() {
      // this is a bug workaround
    }


    // currently there is no support implemented for setting IRV for
    // SparseBlockArr, therefore I moved IRV related stuff to this class, and
    // have SparseBlockArr be a child class of BaseSparseArr directly instead
    // of this one
    var irv: eltType;
    proc IRV ref {
      return irv;
    }

    // shifts data array according to shiftMap where shiftMap[i] is the new index
    // of the ith element of the array. Called at the end of bulkAdd to move the
    // existing items in data array and initialize new indices with irv.
    // oldnnz is the number of elements in the array. As the function is called
    // at the end of bulkAdd, it is almost certain that oldnnz!=data.size
    proc sparseBulkShiftArray(shiftMap, oldnnz){
      var newIdx: int;
      var prevNewIdx = 1;

      // fill all new indices i s.t. i > indices[oldnnz]
      forall i in shiftMap.domain.high+1..dom.nnzDom.high do data[i] = irv;

      for (i, _newIdx) in zip(1..oldnnz by -1, shiftMap.domain.dim(1) by -1) {
        newIdx = shiftMap[_newIdx];
        data[newIdx] = data[i];

        //fill IRV up to previously added nnz
        for emptyIndex in newIdx+1..prevNewIdx-1 do data[emptyIndex] = irv;
        prevNewIdx = newIdx;
      }
      //fill the initial added space with IRV
      for i in 1..prevNewIdx-1 do data[i] = irv;
    }

    // shift data array after single index addition. Fills the new index with irv
    proc sparseShiftArray(shiftrange, initrange) {
      for i in initrange {
        data(i) = irv;
      }
      for i in shiftrange by -1 {
        data(i+1) = data(i);
      }
      data(shiftrange.low) = irv;
    }

    proc sparseShiftArrayBack(shiftrange) {
      for i in shiftrange {
        data(i) = data(i+1);
      }
    }
  }

  // delete helpers

  // param privatized here is a workaround for the fact that
  // we can't include the privatized freeing for DefaultRectangular
  // because of resolution order issues
  proc _delete_dist(dist:BaseDist, param privatized:bool) {
    dist.dsiDestroyDist();

    if privatized {
      _freePrivatizedClass(dist.pid, dist);
    }

    delete dist;
  }

  proc _delete_dom(dom, param privatized:bool) {

    // This is a workaround for the recursive iterator bug discussed in
    // GitHub issue #5311. Implementing 'dsiDestroyDom' on 'BaseDom' leads
    // the compiler into thinking there's recursion due to virtual methods,
    // when in fact there is no recursion at all.
    use Reflection;
    if canResolveMethod(dom, "dsiDestroyDom") {
      dom.dsiDestroyDom();
    }

    if privatized {
      _freePrivatizedClass(dom.pid, dom);
    }

    delete dom;
  }
  // arr is a subclass of :BaseArr but is generic so
  // that arr.eltType is meaningful.
  proc _delete_arr(arr, param privatized:bool) {
    // array implementation can destroy data or other members
    arr.dsiDestroyArr();

    if arr._decEltRefCounts {
      // unlink domain referred to by arr.eltType
      // not necessary for aliases/slices because the original
      // array will take care of it.
      // This needs to be done after the array elements are destroyed
      // (by dsiDestroyArray above) because the array elements might
      // refer to this inner domain.
      chpl_decRefCountsForDomainsInArrayEltTypes(arr, arr.eltType);
    }

    if privatized {
      _freePrivatizedClass(arr.pid, arr);
    }

    // runs the array destructor
    delete arr;
  }

  // These are used in ChapelLocale.chpl. They are here to
  // prevent an order-of-resolution issue.
  pragma "no doc"
  const chpl_emptyLocaleSpace: domain(1) = {1..0};
  pragma "no doc"
  const chpl_emptyLocales: [chpl_emptyLocaleSpace] locale;

  // domain assignment helpers

  // Implement simple reallocate/set indices/post reallocate
  // for compatibility.
  // Domain implementations may supply their own dsiAssignDomain
  // that does something else.
  // lhs is a subclass of BaseRectangularDom
  proc chpl_assignDomainWithGetSetIndices(lhs:?t, rhs: domain)
    where t:BaseRectangularDom
  {
    type arrType = lhs.getBaseArrType();
    param rank = lhs.rank;
    type idxType = lhs.idxType;
    param stridable = lhs.stridable;

    for e in lhs._arrs do {
      on e {
        var eCast = e:arrType;
        if eCast == nil then
          halt("internal error: ", t.type:string,
               " contains an bad array type ", arrType:string);

        var inds = rhs.getIndices();
        var tmp:rank * range(idxType,BoundedRangeType.bounded,stridable);

        // set tmp = inds with some error checking
        for param i in 1..rank {
          var from = inds(i);
          tmp(i) =
            from.safeCast(range(idxType,BoundedRangeType.bounded,stridable));
        }

        eCast.dsiReallocate(tmp);
      }
    }
    lhs.dsiSetIndices(rhs.getIndices());
    for e in lhs._arrs do {
      var eCast = e:arrType;
      on e do eCast.dsiPostReallocate();
    }

    if lhs.dsiSupportsPrivatization() {
      _reprivatize(lhs);
    }
  }


  proc chpl_assignDomainWithIndsIterSafeForRemoving(lhs:?t, rhs: domain)
    where t:BaseSparseDom || t:BaseAssociativeDom || t:BaseOpaqueDom
  {
    //
    // BLC: It's tempting to do a clear + add here, but because
    // we need to preserve array values that are in the intersection
    // between the old and new index sets, we use the following
    // instead.
    //
    // A domain implementation is free to write their own
    // dsiAssignDomain instead of using this method.

    for i in lhs.dsiIndsIterSafeForRemoving() {
      if !rhs.member(i) {
        lhs.dsiRemove(i);
      }
    }
    for i in rhs {
      if !lhs.dsiMember(i) {
        lhs.dsiAdd(i);
      }
    }
  }
}
