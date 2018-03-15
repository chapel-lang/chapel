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

// ArrayViewReindex
//
// This module provides an array domain map class that is used to
// represent reindexings of another array via a domain.
//
module ArrayViewReindex {
  use ChapelStandard;

  //
  // This class represents a distribution that knows how to create
  // reindexed domains and arrays similar to the one that caused
  // it to be created.  It stores a pointer down to the distribution
  // that it creates reindexed views of, and a down-facing and up-facing
  // domain indicating the old and new index sets, respectively.
  //
  class ArrayViewReindexDist: BaseDist {
    // a pointer down to the distribution that this class is creating
    // reindexed views of
    const downDistPid: int;
    const downDistInst;
    const updom;
    const downdomPid;
    const downdomInst;

    inline proc downDist {
      if _isPrivatized(downDistInst) then
        return chpl_getPrivatizedCopy(downDistInst.type, downDistPid);
      else
        return downDistInst;
    }

    proc dsiNewRectangularDom(param rank, type idxType, param stridable, inds) {
      var newdom = new ArrayViewReindexDom(rank=rank,
                                           idxType=idxType,
                                           //                                           stridable=true,
                                           stridable=stridable,
                                           downdomPid=downdomPid,
                                           downdomInst=downdomInst,
                                           distPid=this.pid,
                                           distInst=this);
      newdom.dsiSetIndices(inds);
      return newdom;
    }

    proc dsiClone() return new ArrayViewReindexDist(downDistPid=downDistPid,
                                                    downDistInst=downDistInst,
                                                    updom=updom,
                                                    downdomPid=downdomPid,
                                                    downdomInst=downdomInst);

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    proc dsiSupportsPrivatization() param
      return downDistInst.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (downDistPid, downDistInst, updom, downdomPid, downdomInst);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewReindexDist(downDistPid = privatizeData(1),
                                      downDistInst = privatizeData(2),
                                      updom = privatizeData(3),
                                      downdomPid = privatizeData(4),
                                      downdomInst = privatizeData(5));
    }

    proc dsiDestroyDist() {
      _delete_dom(updom, false);
      //      _delete_dom(downdomInst, _isPrivatized(downdomInst));
    }
  }

  //
  // This class represents the domain of a reindexed slice of an
  // array.  Like other domain class implementations, it supports the
  // standard dsi interface.  Note that reindexings only make sense
  // for rectangular domains so this is a subclass of
  // BaseRectangularDom.
  //
 class ArrayViewReindexDom: BaseRectangularDom {
    // the new reindexed index set that we represent upwards
    var updom: DefaultRectangularDom(rank, idxType, stridable);
    forwarding updom except these;

    // the old original index set that we're equivalent to
    var downdomPid;
    var downdomInst; //: downdomtype(rank, idxType, stridable);

    const distPid;  // a reference back to our ArrayViewReindexDist
    const distInst;

    var ownsDownDomInst = false;

    inline proc dist {
      if _isPrivatized(distInst) then
        return chpl_getPrivatizedCopy(distInst.type, distPid);
      else
        return distInst;
    }
    
    //
    // TODO: If we put this expression into the variable declaration
    // above, we get a memory leak.  File a future against this?
    //
    proc downdomtype(param rank: int, type idxType, param stridable: bool) type {
      var ranges : rank*range(idxType, BoundedRangeType.bounded, stridable);
      var a = dist.downDist.dsiNewRectangularDom(rank=rank, idxType=idxType,
                                              stridable=stridable, ranges);
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
                                        _DomPid = this.pid,
                                        dom = this,
                                        _ArrPid=downarr._pid,
                                        _ArrInstance=downarr._instance,
                                        ownsArrInstance=true);
    }

    proc dsiSetIndices(inds) {
      pragma "no auto destroy"
      var updomRec = {(...inds)};

      // Free the old updom
      if updom != nil then
        _delete_dom(updom, false);
      updom = updomRec._value;

      var ranges : rank*range(idxType, BoundedRangeType.bounded, dist.downdomInst.stridable);
      var downdomclass = dist.downDist.dsiNewRectangularDom(rank=rank,
                                                         idxType=idxType,
                                                         stridable=dist.downdomInst.stridable, ranges);
      pragma "no auto destroy"
      var downdomLoc = _newDomain(downdomclass);
      downdomLoc = chpl_reindexConvertDom(inds, updom, dist.downdomInst);
      downdomLoc._value._free_when_no_arrs = true;

      if downdomInst != nil && ownsDownDomInst {
        _delete_dom(downdomInst, _isPrivatized(downdomInst));
      }

      downdomPid = downdomLoc._pid;
      downdomInst = downdomLoc._instance;
      ownsDownDomInst = true;
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

    iter these(param tag: iterKind) where tag == iterKind.standalone
      && chpl__isDROrDRView(downdom)
      && __primitive("method call resolves", updom, "these", tag)
    {
      forall i in updom do
          yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone
      && !chpl__isDROrDRView(downdom)
      && __primitive("method call resolves", downdom, "these", tag)
    {
      forall i in downdom do
        yield downIdxToUpIdx(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if chpl__isDROrDRView(downdom) {
        for followThis in updom.these(tag) do
          yield followThis;
      } else {
        for followThis in downdom.these(tag) {
          yield followThis;
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


    inline proc downIdxToUpIdx(downIdx: integral) {
      compilerAssert(updom.rank == 1, updom.rank:string);
      return updom.dsiDim(1).orderToIndex(downdom.dsiDim(1).indexOrder(downIdx));
    }

    inline proc downIdxToUpIdx(i) {
      var ind: updom.rank*updom.idxType;
      for param d in 1..updom.rank {
        ind(d) = updom.dsiDim(d).orderToIndex(downdom.dsiDim(d).indexOrder(i(d)));
      }
      return ind;
    }

    proc dsiMyDist() {
      return dist;
    }

    proc dsiTargetLocales() {
      return downdom.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return downdom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      const dims = downdom.dsiLocalSubdomain().dims();
      return chpl_reindexConvertDom(dims, downdom, updom);
    }

    proc dsiAssignDomain(rhs: domain, lhsPrivate: bool) {
      chpl_assignDomainWithGetSetIndices(this, rhs);
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

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    proc dsiSupportsPrivatization() param
      return downdomInst.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (updom, downdomPid, downdomInst, distPid, distInst);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewReindexDom(rank = this.rank,
                                     idxType = this.idxType,
                                     stridable = this.stridable,
                                     updom = privatizeData(1),
                                     downdomPid = privatizeData(2),
                                     downdomInst = privatizeData(3),
                                     distPid = privatizeData(4),
                                     distInst = privatizeData(5)
                                     );
    }

    proc dsiGetReprivatizeData() {
      return (updom, downdomPid, downdomInst);
    }

    proc dsiReprivatize(other, reprivatizeData) {
      updom = reprivatizeData(1);
      //      collapsedDim = other.collapsedDim;
      //      idx = other.idx;
      //      distPid = other.distPid;
      //      distInst = other.distInst;
      downdomPid = reprivatizeData(2);
      downdomInst = reprivatizeData(3);
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

    const ownsArrInstance = false;

    forwarding arr except these,
                      doiBulkTransferFromKnown, doiBulkTransferToKnown,
                      doiBulkTransferFromAny,  doiBulkTransferToAny;

    proc downdom {
      // TODO: This routine may get a remote domain if this is a view
      // of a view and is called on a locale other than the
      // originating one for the domain.  Relax the requirement that
      // arrays have a field named 'dom' and let arr.dom return
      // whatever domain class is nearby/cheap.
      return arr.dom;
    }

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
      where tag == iterKind.standalone && !localeModelHasSublocales &&
           __primitive("method call resolves", privDom, "these", tag) {
      forall i in privDom {
        if shouldUseIndexCache() {
          const dataIdx = indexCache.getDataIndex(i);
          yield indexCache.getDataElem(dataIdx);
        } else {
          yield arr.dsiAccess(chpl_reindexConvertIdx(i, privDom, downdom));
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
          yield arr.dsiAccess(chpl_reindexConvertIdx(i, privDom, downdom));
        }
      }
    }


    //
    // I/O
    //

    proc dsiSerialWrite(f) {
      chpl_serialReadWriteRectangular(f, this, privDom.updom);
    }

    proc dsiSerialRead(f) {
      chpl_serialReadWriteRectangular(f, this, privDom.updom);
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
        return arr.dsiAccess(chpl_reindexConvertIdx(i, privDom, downdom));
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_reindexConvertIdx(i, privDom, downdom));
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_reindexConvertIdx(i, privDom, downdom));
      }
    }

    inline proc dsiLocalAccess(i) ref
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, privDom, downdom));

    inline proc dsiLocalAccess(i)
      where shouldReturnRvalueByValue(eltType)
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, privDom, downdom));

    inline proc dsiLocalAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)
      return arr.dsiLocalAccess(chpl_reindexConvertIdx(i, privDom, downdom));

    inline proc checkBounds(i) {
      if boundsChecking then
        if !privDom.dsiMember(i) then
          halt("array index out of bounds: ", i);
    }


    //
    // locality-oriented queries
    //

    proc dsiHasSingleLocalSubdomain() param
      return privDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      return privDom.dsiLocalSubdomain();
    }

    //
    // privatization
    //

    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    proc dsiSupportsPrivatization() param
      return _ArrInstance.dsiSupportsPrivatization();

    proc dsiGetPrivatizeData() {
      return (_DomPid, privDom, _ArrPid, _ArrInstance);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewReindexArr(eltType=this.eltType,
                                     _DomPid=privatizeData(1),
                                     dom=privatizeData(2),
                                     _ArrPid=privatizeData(3),
                                     _ArrInstance=privatizeData(4));
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
      return privDom;
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

    proc dsiDestroyArr() {
      if ownsArrInstance {
        _delete_arr(_ArrInstance, _isPrivatized(_ArrInstance));
      }
    }

    proc doiCanBulkTransferRankChange() param
      return arr.doiCanBulkTransferRankChange();

    proc doiBulkTransferFromKnown(destDom, srcClass, srcDom) : bool {
      const shifted = chpl_reindexConvertDom(destDom.dims(), destDom._value, this.dom.dist.downdomInst);
      return chpl__bulkTransferArray(this.arr, shifted, srcClass, srcDom);
    }

    proc doiBulkTransferToKnown(srcDom, destClass, destDom) : bool {
      const shifted = chpl_reindexConvertDom(srcDom.dims(), srcDom._value, this.dom.dist.downdomInst);
      return chpl__bulkTransferArray(destClass, destDom, this.arr, shifted);
    }
  }


    //
    // Helper routines to convert incoming new/reindex
    // indices/domains back into the original index set.
    //

  inline proc chpl_reindexConvertIdxDim(i: integral, updom, downdom, dim: int) {
    return downdom.dsiDim(dim).orderToIndex(updom.dsiDim(dim).indexOrder(i));
  }

    inline proc chpl_reindexConvertIdx(i: integral, updom, downdom) {
      compilerAssert(downdom.rank == 1, downdom.rank:string);
      return chpl_reindexConvertIdxDim(i, updom, downdom, 1);
    }

    inline proc chpl_reindexConvertIdx(i, updom, downdom) {
      var ind: downdom.rank*downdom.idxType;
      for param d in 1..downdom.rank {
        ind(d) = chpl_reindexConvertIdxDim(i(d), updom, downdom, d);
      }
      return ind;
    }


  inline proc chpl_reindexConvertDom(dims, updom, downdom) {
    if updom.rank != dims.size {
      compilerError("Called chpl_reindexConvertDom with incorrect rank. Got " + dims.size:string + ", expecting " + updom.rank:string);
    }

    var ranges : downdom.dsiDims().type;
    var actualLow, actualHigh: downdom.rank*downdom.idxType;
    for param d in 1..dims.size {
      if (dims(d).size == 0) {
        actualLow(d) = downdom.dsiDim(d).low;
        actualHigh(d) = downdom.dsiDim(d).high;
      } else {
        actualLow(d) = chpl_reindexConvertIdxDim(dims(d).first, updom, downdom, d);
        actualHigh(d) = chpl_reindexConvertIdxDim(dims(d).last, updom, downdom, d);
      }
    }
    for param d in 1..updom.rank {
      // Slicing the ranges preserves the stride
      ranges(d) = downdom.dsiDim(d)[actualLow(d)..actualHigh(d)];
    }
    return {(...ranges)};
  }
}
