/*
 * Copyright 2004-2017 Cray Inc.
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

// ArrayViewReindex
//
// This module provides an array domain map class that is used to
// represent reindexings of another array via a domain.
//
module ArrayViewReindex {

  //
  // This class represents a distribution that knows how to create
  // reindexed domains and arrays similar to the one that caused
  // it to be created.  It stores a pointer down to the distribution
  // that it creates lower-dimensional views of, and two tuples
  // indicating which dimensions were collapsed by the reindexed
  // and what the indices of those dimensions are.
  //
  class ArrayViewReindexDist: BaseDist {
    const downdist;
    const updom;
    const downdomPid;
    const downdomInst;

    proc dsiNewRectangularDom(param rank, type idxType, param stridable, inds) {
      var newdom = new ArrayViewReindexDom(rank=rank,
                                           idxType=idxType,
                                           stridable=stridable,
                                           updom=updom,
                                           downdomPid=downdomPid,
                                           downdomInst=downdomInst,
                                           dist=this);
      newdom.dsiSetIndices(inds);
      return newdom;
    }

    proc dsiClone() return new ArrayViewReindexDist(downdist=downdist,
                                                    updom=updom,
                                                    downdomPid=downdomPid,
                                                    downdomInst=downdomInst);
  }

  //
  // This class represents the domain of a reindexed slice of an
  // array.  Like other domain class implementations, it supports the
  // standard dsi interface.  Note that reindexings only make sense
  // for rectangular domains so this is a subclass of
  // BaseRectangularDom.
  //
 class ArrayViewReindexDom: BaseRectangularDom {
    param rank;
    type idxType;
    param stridable;

    // the lower-dimensional index set that we represent upwards
    var updom: DefaultRectangularDom(rank, idxType, stridable);

    // the higher-dimensional domain that we're equivalent to
    var downdomPid;
    var downdomInst;

    var dist;

    //
    // TODO: If we put this expression into the variable declaration
    // above, we get a memory leak.  File a future against this?
    //
    proc downdomtype(param rank: int, type idxType, param stridable: bool) type {
      var a = dist.downdist.newRectangularDom(rank=rank, idxType=idxType,
                                              stridable=stridable);
      return a.type;
    }

    inline proc downdom {
      if _isPrivatized(downdomInst) then
        return chpl_getPrivatizedCopy(downdomInst.type, downdomPid);
      else
        return downdomInst;
    }

    proc dsiBuildArray(type eltType) {
      pragma "no auto destroy"
      const downarr = _newArray(downdom.dsiBuildArray(eltType));
      return new ArrayViewReindexArr(eltType  =eltType,
      // TODO: Update once we start privatizing vvv
                                        _DomPid = nullPid,
                                        dom = this,
                                        _ArrPid=downarr._pid,
                                        _ArrInstance=downarr._instance,
                                        ownsArrInstance=true);
    }

    // TODO: Use delegation feature for these?
    // TODO: Can't all these be implemented in ChapelArray given dsiDim()?

    proc dsiNumIndices {
      return updom.dsiNumIndices;
    }

    proc dsiLow {
      return updom.dsiLow;
    }

    proc dsiHigh {
      return updom.dsiHigh;
    }

    proc dsiAlignedLow {
      return updom.dsiAlignedLow;
    }

    proc dsiAlignedHigh {
      return updom.dsiAlignedHigh;
    }

    proc dsiStride {
      return updom.dsiStride;
    }

    proc dsiAlignment {
      return updom.dsiAlignment;
    }

    proc dsiFirst {
      return updom.dsiFirst;
    }

    proc dsiLast {
      return updom.dsiLast;
    }

    proc dsiDim(d: int) {
      return updom.dsiDim(d);
    }

    proc dsiDims() {
      return updom.dsiDims();
    }

    proc dsiGetIndices() {
      return updom.dsiGetIndices();
    }

    proc dsiSetIndices(inds) {
      pragma "no auto destroy"
      var updomRec = {(...inds)};
      updom = updomRec._value;
      var downdomclass = dist.downdist.newRectangularDom(rank=rank,
                                                         idxType=idxType,
                                                         stridable=stridable);
      pragma "no auto destroy"
      var downdomLoc = _newDomain(downdomclass);
      downdomLoc = chpl_reindexConvertDom(inds, downdom);
      downdomLoc._value._free_when_no_arrs = true;
      downdomPid = downdomLoc._pid;
      downdomInst = downdomLoc._instance;
    }

    proc dsiMember(i) {
      return updom.dsiMember(i);
    }

    iter these() {
      if chpl__isDROrDRView(downdom) {
        for i in updom do
          yield i;
      } else {
        for i in downdom do
          yield downIdxToUpIdx(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      if chpl__isDROrDRView(downdom) {
        for i in updom.these(tag) do
          yield i;
      } else {
        for i in downdom.these(tag) do
          yield downIdxToUpIdx(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if chpl__isDROrDRView(downdom) {
        for followThis in updom.these(tag) do
          yield followThis;
      } else {
        for followThis in downdom.these(tag) {
          const followThisLoD = chpl_reindexConvertHiDTupleToLoD(followThis);
          yield followThisLoD;
        }
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      for i in updom.these(tag, followThis) do
        yield i;
    }

    proc chpl_reindexConvertLoDTupleToHiD(tup) {
      compilerError("Unimplemented routine");
    }

    proc chpl_reindexConvertHiDTupleToLoD(tup) {
      compilerError("Unimplemented routine");
    }


    inline proc downIdxToUpIdx(downIdx) {
      compilerError("Unimplemented routine");
    }

    // TODO: Is there something we can re-use here?
    proc dsiSerialWrite(f) {
      updom.dsiSerialWrite(f);
    }

    proc dsiMyDist() {
      return dist;
    }

    proc dsiTargetLocales() {
      //
      // BLC: To tighten this up, we'd need to query the distribution to
      // see what subset of target locales the reindexed slice hit vs.
      // not.
      //
      //      compilerWarning("Calls to .targetLocales() on reindexed slices may currently return a superset of the locales targeted.");
      return downdom.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return downdom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      return downdom.dsiLocalSubdomain();
    }

    proc isReindexDomainView() param {
      return true;
    }

    proc _getActualDomain() {
      if chpl__isDomainView(downdom) {
        return downdom._getActualDomain();
      } else {
        return downdom;
      }
    }

    proc dsiDestroyDom() {
      _delete_dom(updom, false);
      _delete_dom(downdomInst, _isPrivatized(downdomInst));
    }

  } // end of class ArrayViewReindexDom

  //
  // The class representing a slice of an array.  Like other array
  // class implementations, it supports the standard dsi interface.
  //
  class ArrayViewReindexArr: BaseArr {
    type eltType;  // see note on commented-out proc eltType below...

    // the representation of the slicing domain
    //
    // TODO: Can we privatize upon creation of the array-view slice and cache
    // the results?
    const _DomPid;
    const dom; // Seems like the compiler requires a field called 'dom'...

    // the representation of the sliced array
    const _ArrPid;
    const _ArrInstance;

    // If this is an array view on a DefaultRectangular array
    // (eventually...), the indexCache provides a mean of directly
    // accessing the array's ddata to avoid indirection overheads
    // through the array field above.
    const indexCache = buildIndexCache();


    //
    // standard generic aspects of arrays
    //

    // these could be fields, but indirecting works just as well and
    // makes the class less generic.
    proc idxType type return dom.idxType;
    proc rank param return dom.rank;

    // The following seems like it ought to work, but it causes an
    // error in the compiler for non-devel mode...  presumably due to
    // a direct query of eltType in the compiler(?).  As a TODO we
    // might want to hunt this down in the future...
    //
    //  proc eltType type return arr.eltType;


    //
    // introspection routine used elsewhere to filter array views
    //
    // TODO: Could this be replaced with more type-based introspection?
    // I shied away from it since this is a generic class, but there
    // must be (or should be) some way to do it without relying on
    // methods like this...
    //
    proc isReindexArrayView() param {
      return true;
    }



    //
    // standard iterators
    //

    iter these() ref {
      for elem in chpl__serialViewIter(this, privDom) do
        yield elem;
    }

    iter these(param tag: iterKind) ref
      where tag == iterKind.standalone && !localeModelHasSublocales {
      for i in privDom.these(tag) {
        if shouldUseIndexCache() {
          const dataIdx = indexCache.getDataIndex(i);
          yield indexCache.getDataElem(dataIdx);
        } else {
          yield arr.dsiAccess(chpl_reindexConvertIdx(i, dom, downdom));
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in privDom.these(tag) do {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis) ref
      where tag == iterKind.follower {
      for i in privDom.these(tag, followThis) {
        if shouldUseIndexCache() {
          const dataIdx = indexCache.getDataIndex(i);
          yield indexCache.getDataElem(dataIdx);
        } else {
          yield arr.dsiAccess(chpl_reindexConvertIdx(i, dom, downdom));
        }
      }
    }


    //
    // I/O
    //

    proc dsiSerialWrite(f) {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    proc dsiSerialRead(f) {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    proc dsiDisplayRepresentation() {
      writeln("Reindex view");
      writeln("------------");
      writeln("of domain:");
      privDom.dsiDisplayRepresentation();
      writeln("on array:");
      arr.dsiDisplayRepresentation();
      writeln("------------");
    }


    //
    // accessors
    //

    inline proc dsiAccess(i: idxType ...rank) ref {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank)
      where shouldReturnRvalueByValue(eltType) {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i) ref {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_reindexConvertIdx(i, dom, downdom));
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_reindexConvertIdx(i, dom, downdom));
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_reindexConvertIdx(i, dom, downdom));
      }
    }

    inline proc dsiLocalAccess(i) ref
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, dom, downdom));

    inline proc dsiLocalAccess(i)
      where shouldReturnRvalueByValue(eltType)
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, dom, downdom));

    inline proc dsiLocalAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, dom, downdom));

    inline proc checkBounds(i) {
      if boundsChecking then
        if !privDom.dsiMember(i) then
          halt("array index out of bounds: ", i);
    }


    //
    // locality-oriented queries
    //

    proc dsiTargetLocales() {
      return arr.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return privDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      return privDom.dsiLocalSubdomain();
    }

    proc dsiNoFluffView() {
      // For now avoid implementing 'noFluffView' on each class and use
      // 'canResolve' to print a better error message.
      if canResolveMethod(arr, "dsiNoFluffView") {
        return arr.dsiNoFluffView();
      } else {
        compilerError("noFluffView is not supported on this array type.");
      }
    }


    //
    // privatization
    //

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    proc dsiSupportsPrivatization() param
      return _ArrInstance.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (_DomPid, dom, _ArrPid, _ArrInstance);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewReindexArr(eltType=this.eltType,
                                     _DomPid=privatizeData(1),
                                     dom=privatizeData(2),
                                     _ArrPid=privatizeData(3),
                                     _ArrInstance=privatizeData(4));
    }


    //
    // bulk-transfer
    //

    proc dsiSupportsBulkTransfer() param {
      return arr.dsiSupportsBulkTransfer();
    }
    proc dsiSupportsBulkTransferInterface() param return arr.dsiSupportsBulkTransferInterface();

    proc _viewHelper(dims) {
      if dims.size != dom.rank {
        compilerError("Error while composing view domain for reindex view.");
      }
      const goodDims = chpl_reindexConvertDom(dims, dom).dims();
      if _containsRCRE() {
        var nextView = arr._getRCREView();
        return nextView._viewHelper(goodDims);
      } else {
        return {(...goodDims)};
      }
    }

    proc _getViewDom() {
      // BHARSH TODO
      return _viewHelper(dom.dsiDims());
    }

    proc doiUseBulkTransfer(B) {
      return arr.doiUseBulkTransfer(B);
    }

    proc doiCanBulkTransfer(viewDom) {
      return arr.doiCanBulkTransfer(viewDom);
    }

    proc doiBulkTransfer(B, viewDom) {
      arr.doiBulkTransfer(B, viewDom);
    }

    // strided transfer support
    proc doiUseBulkTransferStride(B) {
      return arr.doiUseBulkTransferStride(B);
    }

    proc doiCanBulkTransferStride(viewDom) {
      return arr.doiCanBulkTransferStride(viewDom);
    }

    proc doiBulkTransferStride(B, viewDom) {
      arr.doiBulkTransferStride(B, viewDom);
    }


    // distributed transfer support
    proc doiBulkTransferToDR(B, viewDom) {
      arr.doiBulkTransferToDR(B, viewDom);
    }

    proc doiBulkTransferFromDR(B, viewDom) {
      arr.doiBulkTransferFromDR(B, viewDom);
    }

    proc doiBulkTransferFrom(B, viewDom) {
      arr.doiBulkTransferFrom(B, viewDom);
    }


    //
    // utility functions used to set up the index cache
    //

    proc shouldUseIndexCache() param {
      return chpl__isDROrDRView(_ArrInstance);
    }

    proc buildIndexCache() {
      if shouldUseIndexCache() {
        if (chpl__isArrayView(_ArrInstance)) {
          if _ArrInstance.isSliceArrayView() && !_ArrInstance._containsRCRE() {
            // Only slices below in the view stack, which won't have built up
            // an indexCache.
            return _ArrInstance._getActualArray().dsiGetRAD().toSlice(_ArrInstance.dom).toReindex(dom);
          } else {
            return _ArrInstance.indexCache.toReindex(dom);
          }
        } else {
          return _ArrInstance.dsiGetRAD().toReindex(dom);
        }
      } else {
        return false;
      }
    }


    //
    // routines relating to the underlying domains and arrays
    //

    inline proc privDom {
      if _isPrivatized(dom) {
        return chpl_getPrivatizedCopy(dom.type, _DomPid);
      } else {
        return dom;
      }
    }

    inline proc arr {
      if _isPrivatized(_ArrInstance) {
        return chpl_getPrivatizedCopy(_ArrInstance.type, _ArrPid);
      } else {
        return _ArrInstance;
      }
    }

    // not sure what this is, but everyone seems to have one...
    inline proc dsiGetBaseDom() {
      return dom;
    }

    proc _getActualArray() {
      if chpl__isArrayView(arr) {
        return arr._getActualArray();
      } else {
        return arr;
      }
    }

    // Returns true if 'arr' is a rank-change, a reindex, or if itself
    // contains a rank-change or reindex.
    proc _containsRCRE() param {
      if chpl__isArrayView(arr) {
        return arr.isRankChangeArrayView() ||
               arr.isReindexArrayView() ||
               arr._containsRCRE();
      } else {
        return false;
      }
    }

    // Returns the topmost rank-change view in this view-stack
    proc _getRCREView() {
      return this;
    }
  }


    //
    // Helper routines to convert incoming new/reindex
    // indices/domains back into the original index set.
    //

    inline proc chpl_reindexConvertIdx(i: integral, dom, downdom) {
      compilerAssert(dom.rank == 1, dom.rank:string);
      return downdom.dsiDim(1).orderToIndex(dom.dsiDim(1).indexOrder(i));
    }

    inline proc chpl_reindexConvertIdx(i, dom, downdom) {
      var ind: dom.rank*dom.idxType;
      for param d in 1..dom.rank {
        ind(d) = downdom.dom.dsiDim(d).orderToIndex(dom.dsiDim(d).indexOrder(i(d)));
      }
      return ind;
    }


  inline proc chpl_reindexConvertDom(dims, dom) {
    if dom.rank != dims.size {
      compilerError("Called chpl_reindexConvertDom with incorrect rank. Got ", dims.size:string, ", expecting ", dom.rank:string);
    }

    var ranges : dom.dsiDims().type;
    var low , high : dom.rank*dom.idxType;
    for param d in 1..dims.size do low(d) = dims(d).first;
    for param d in 1..dims.size do high(d) = dims(d).last;

    var actualLow = chpl_reindexConvertIdx(low, dom, downdom);
    var actualHigh = chpl_reindexConvertIdx(high, dom, downdom);
    for param d in 1..dom.rank {
      var lowered = actualLow(d)..actualHigh(d);
      // TODO: does it matter which range slices the other?
      ranges(d) = dom.dsiDim(d)[lowered];
    }
    return {(...ranges)};
  }
}
