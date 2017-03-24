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

// ArrayViewRankChange
//
// This module provides an array domain map class that is used to
// represent rank change slices of another array via a domain.
//
// Throughout this file, 'down*' refers to the higher-dimensional
// array/domain/distribution that's been rank-changed in this view,
// while 'up*' refers to the lower-dimensional, user-facing view.
//
// Note that because only rectangular domains have a notion of having
// rank > 1, rank-change views are only supported for rectangular
// domains and arrays.
//
// TODOs for this file:
//
// * the dist, dom, and arr classes store a certain amount of
//   duplicated information -- should these be focused in the higher
//   classes (dist/dom) and referenced by lower ones (dom/dist)?
//
// * I don't think I've done anything to privatize the dist or dom
//   classes... should I?
//
module ArrayViewRankChange {

  //
  // This class represents a distribution that knows how to create
  // rank-change domains and arrays similar to the one that caused
  // it to be created.
  //
  class ArrayViewRankChangeDist: BaseDist {
    // a pointer down to the distribution that this class is creating
    // lower-dimensional views of
    var downDistPid:int;
    var downDistInst;
    
    // These two fields represent whether or not each dimension was
    // collapsed as part of the rank-change; and if so, what the
    // index of that collapsed dimension was.  So for A[lo..hi, 3]
    // these would be (false, true) and (?, 3) respectively.
    //
    // As future work (TODO), note that collapsedDim ought to be able
    // to be changed into a param tuple rather than an execution-time
    // one.
    const collapsedDim;
    const idx;

    inline proc downDist {
      if _isPrivatized(downDistInst) then
        return chpl_getPrivatizedCopy(downDistInst.type, downDistPid);
      else
        return downDistInst;
    }

    proc dsiNewRectangularDom(param rank, type idxType, param stridable, inds) {
      var newdom = new ArrayViewRankChangeDom(rank=rank,
                                              idxType=idxType,
                                              stridable=stridable,
                                              collapsedDim=collapsedDim,
                                              idx=idx,
                                              dist=this);
      newdom.dsiSetIndices(inds);
      return newdom;
    }

    proc dsiClone() return new ArrayViewRankChangeDist(downDistPid=this.downDistPid,
                                                       downDistInst=this.downDistInst,
                                                       collapsedDim=collapsedDim,
                                                       idx=idx);

    proc dsiDestroyDist() {
    }

    // TODO: privatization
  }

  //
  // This class represents the domain of a rank-change slice of an
  // array.  Like other domain class implementations, it supports the
  // standard dsi interface.  Note that rank changes only make sense
  // for rectangular domains (because they're the only ones with
  // rank>1), so this is a subclass of BaseRectangularDom.
  //
 class ArrayViewRankChangeDom: BaseRectangularDom {
    param rank;
    type idxType;
    param stridable;

    // the lower-dimensional index set that we represent upwards
    var upDom: DefaultRectangularDom(rank, idxType, stridable);

    // the collapsed dimensions and indices in those dimensions
    //
    // Note that these are the same as in the distribution and could
    // potentially be pulled from there.
    const collapsedDim;
    const idx;

    const dist;  // a reference back to our ArrayViewRankChangeDist

    // the higher-dimensional domain that we're equivalent to
    var downDomPid:int;
    var downDomInst: downDomType(downrank, idxType, stridable);

    //
    // TODO: If we put this expression into the variable declaration
    // above, we get a memory leak.  File a future against this?
    //
    proc downDomType(param rank: int, type idxType, param stridable: bool) type {
      var ranges: rank*range(idxType, BoundedRangeType.bounded, stridable);
      var a = dist.downDist.dsiNewRectangularDom(rank=rank, idxType,
                                                 stridable=stridable, ranges);
      return a.type;
    }

    proc downrank param {
      return collapsedDim.size;
    }

    inline proc downDom {
      if _isPrivatized(downDomInst) then
        return chpl_getPrivatizedCopy(downDomInst.type, downDomPid);
      else
        return downDomInst;
    }

    proc dsiBuildArray(type eltType) {
      pragma "no auto destroy"
      const downarr = _newArray(downDom.dsiBuildArray(eltType));
      return new ArrayViewRankChangeArr(eltType  =eltType,
      // TODO: Update once we start privatizing vvv
                                        _DomPid = nullPid,
                                        dom = this,
                                        _ArrPid=downarr._pid,
                                        _ArrInstance=downarr._instance,
                                        collapsedDim=collapsedDim,
                                        idx=idx,
                                        ownsArrInstance=true);
    }

    // TODO: Use delegation feature for these?
    // TODO: Can't all these be implemented in ChapelArray given dsiDim()?

    proc dsiNumIndices {
      return upDom.dsiNumIndices;
    }

    proc dsiLow {
      return upDom.dsiLow;
    }

    proc dsiHigh {
      return upDom.dsiHigh;
    }

    proc dsiAlignedLow {
      return upDom.dsiAlignedLow;
    }

    proc dsiAlignedHigh {
      return upDom.dsiAlignedHigh;
    }

    proc dsiStride {
      return upDom.dsiStride;
    }

    proc dsiAlignment {
      return upDom.dsiAlignment;
    }

    proc dsiFirst {
      return upDom.dsiFirst;
    }

    proc dsiLast {
      return upDom.dsiLast;
    }

    proc dsiDim(upDim: int) {
      return upDom.dsiDim(upDim);
    }

    proc dsiDims() {
      return upDom.dsiDims();
    }

    proc dsiGetIndices() {
      return upDom.dsiGetIndices();
    }

    proc dsiSetIndices(inds) {
      // Free underlying domains if necessary
      this.dsiDestroyDom();

      pragma "no auto destroy"
      var upDomRec = {(...inds)};
      upDom = upDomRec._value;

      var ranges: downrank*range(idxType, BoundedRangeType.bounded, stridable);
      var downDomClass = dist.downDist.dsiNewRectangularDom(rank=downrank,
                                                           idxType,
                                                           stridable=stridable,
                                                           ranges);
      pragma "no auto destroy"
      var downDomLoc = _newDomain(downDomClass);
      downDomLoc = chpl_rankChangeConvertDom(inds, inds.size, collapsedDim, idx);
      downDomLoc._value._free_when_no_arrs = true;
      downDomPid = downDomLoc._pid;
      downDomInst = downDomLoc._instance;
    }

    proc dsiMember(i) {
      return upDom.dsiMember(i);
    }

    iter these() {
      if chpl__isDROrDRView(downDom) {
        for i in upDom do
          yield i;
      } else {
        for i in downDom do
          yield downIdxToUpIdx(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone && !localeModelHasSublocales {
      if chpl__isDROrDRView(downDom) {
        for i in upDom.these(tag) do
          yield i;
      } else {
        for i in downDom.these(tag) do
          yield downIdxToUpIdx(i);
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if chpl__isDROrDRView(downDom) {
        for followThis in upDom.these(tag) do
          yield followThis;
      } else {
        for followThis in downDom.these(tag) {
          const followThisLoD = chpl_rankChangeConvertHiDTupleToLoD(followThis);
          yield followThisLoD;
        }
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      if chpl__isDROrDRView(downDom) {
        for i in upDom.these(tag, followThis) do
          yield i;
      } else {
        const followThisHiD = chpl_rankChangeConvertLoDTupleToHiD(followThis);
        for i in downDom.these(tag, followThisHiD) {
          yield chpl_rankChangeConvertIdxHiDToLoD(i, collapsedDim, idx, rank);
        }
      }
    }

    proc chpl_rankChangeConvertLoDTupleToHiD(tup) {
      var tupHiD: downrank*tup(1).type;
      var i = 1;
      for param d in 1..downrank do
        if collapsedDim(d) then
          tupHiD(d) = 0..0;
        else {
          tupHiD(d) = tup(i);
          i += 1;
        }
      return tupHiD;
    }

    proc chpl_rankChangeConvertHiDTupleToLoD(tup) {
      var tupLoD: rank*tup(1).type;
      var i = 1;
      for param d in 1..downrank do
        if !collapsedDim(d) {
          tupLoD(i) = tup(d);
          i += 1;
        }
      return tupLoD;
    }


    inline proc downIdxToUpIdx(downIdx) {
      var upIdx: rank*idxType;
      var upDim = 1;
      for param downDim in 1..downrank {
        if !collapsedDim(downDim) {
          upIdx(upDim) = downIdx(downDim);
          upDim += 1;
        }
      }
      if rank == 1 then
        return upIdx(1);
      else
        return upIdx;
    }

    // TODO: Is there something we can re-use here?
    proc dsiSerialWrite(f) {
      var first = true;
      for d in 1..downrank do
        if !collapsedDim(d) {
          if first {
            write("{");
            first = false;
          } else
            write(", ");
          write(downDom.dsiDim(d));
        }
      write("}");
    }

    proc dsiMyDist() {
      return dist;
    }

    proc dsiTargetLocales() {
      //
      // BLC: there's a bit of a question in my mind about whether
      // rank-change slices (and regular slices for that matter) ought
      // to be expected to list only the subset of locales that have
      // non-empty subdomains for a given domain/array, or whether all
      // locales in the domain's/distribution's target locale set
      // should be listed since they are part of the target locale
      // set.  Both seem like they could be useful, though the former
      // seems as though it could be challenging to compute precisely
      // for less regular distributions.  Here I've done the easy
      // thing and simply returned all the locales that own the domain
      // below us, as slicing does.
      //
      return downDom.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return downDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      return downDom.dsiLocalSubdomain();
    }

    proc isRankChangeDomainView() param {
      return true;
    }

    proc _getActualDomain() {
      if chpl__isDomainView(downDom) {
        return downDom._getActualDomain();
      } else {
        return downDom;
      }
    }

    proc dsiDestroyDom() {
      if upDom != nil then
        _delete_dom(upDom, false);
      if downDomInst != nil then
        _delete_dom(downDomInst, _isPrivatized(downDomInst));
    }

  } // end of class ArrayViewRankChangeDom

  //
  // The class representing a rank-change slice of an array.  Like
  // other array class implementations, it supports the standard dsi
  // interface.
  //
  class ArrayViewRankChangeArr: BaseArr {
    type eltType;  // see note on commented-out proc eltType below...

    // the representation of the slicing domain.  For a rank change
    // like A[lo..hi, 3] this is the lower-dimensional domain {lo..hi}.
    // It is represented as an ArrayViewRankChangeDom.
    //
    const _DomPid;
    const dom; // Seems like the compiler requires a field called 'dom'...

    // the higher-dimensional representation of the array being sliced
    const _ArrPid;
    const _ArrInstance;

    // These two fields represent whether or not each dimension was
    // collapsed as part of the rank-change; and if so, what the
    // index of that collapsed dimension was.  So for A[lo..hi, 3]
    // these would be (false, true) and (?, 3) respectively.

    // TODO: Use copies in domain rather than replicating these here?

    const collapsedDim;  // rank*bool    TODO: constrain this
    const idx;           // rank*idxType TODO: and this

    // If this is an array view on a DefaultRectangular array
    // (eventually...), the indexCache provides a mean of directly
    // accessing the array's ddata to avoid indirection overheads
    // through the array field above.
    const indexCache = buildIndexCache();

    const ownsArrInstance = false;


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
    proc isRankChangeArrayView() param {
      return true;
    }


    //
    // Helper routines to convert incoming low-D indices/domains into
    // their higher-D equivalents.
    //




    //
    // standard iterators
    //

    iter these() ref {
      for elem in chpl__serialViewIter(this, privDom) do
        yield elem;
    }

    // TODO: We seem to run into compile-time bugs when using multiple yields.
    // For now, work around them by using an if-expr
    iter these(param tag: iterKind) ref
      where tag == iterKind.standalone && !localeModelHasSublocales {
      for i in privDom.these(tag) {
        yield if shouldUseIndexCache()
                then indexCache.getDataElem(indexCache.getDataIndex(i))
                else arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
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
          yield arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
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
      writeln("Rank Change view");
      writeln("----------");
      writeln("of domain:");
      privDom.dsiDisplayRepresentation();
      writeln("on array:");
      arr.dsiDisplayRepresentation();
      writeln("where the dims are collapsed as follows:");
      writeln(collapsedDim);
      writeln("and the missing indices are:");
      writeln(idx);
      writeln("----------");
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
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
      }
    }

    inline proc dsiLocalAccess(i) ref
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc dsiLocalAccess(i)
      where shouldReturnRvalueByValue(eltType)
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc dsiLocalAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)
      return arr.dsiLocalAccess(chpl_rankChangeConvertIdx(i, collapsedDim, idx));

    inline proc checkBounds(i) {
      if boundsChecking then
        if !privDom.dsiMember(i) then
          halt("array index out of bounds: ", i);
    }


    //
    // locality-oriented queries
    //

    proc dsiTargetLocales() {
      //
      // See commentary on ArrayViewRankChangeDom.dsiTargetLocales() above.
      //
      return arr.dsiTargetLocales();
    }

    proc dsiHasSingleLocalSubdomain() param
      return privDom.upDom.dsiHasSingleLocalSubdomain();

    proc dsiLocalSubdomain() {
      return privDom.upDom.dsiLocalSubdomain();
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
      return (_DomPid, dom, _ArrPid, _ArrInstance, collapsedDim, idx);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewRankChangeArr(eltType=this.eltType,
                                        _DomPid=privatizeData(1),
                                        dom=privatizeData(2),
                                        _ArrPid=privatizeData(3),
                                        _ArrInstance=privatizeData(4),
                                        collapsedDim=privatizeData(5),
                                        idx=privatizeData(6));
    }


    //
    // bulk-transfer
    //

    proc dsiSupportsBulkTransfer() param {
      return arr.dsiSupportsBulkTransfer();
    }

    proc dsiSupportsBulkTransferInterface() param
      return arr.dsiSupportsBulkTransferInterface();

    // Recursively builds up the view-domain given an initial tuple of
    // dimensions. Handles nested rank-changes.
    proc _viewHelper(dims) {
      // If 'dims.size != arr.rank', assume that we still need to do the
      // conversion for the current rank-change.
      var goodDims = if dims.size != arr.rank
        then chpl_rankChangeConvertDom(dims, rank, collapsedDim, idx).dims() else dims;
      if goodDims.size != arr.rank {
        compilerError("Error while composing view domain for rank-change view.");
      }
      if _containsRCRE() {
        var nextView = arr._getRCREView();
        return nextView._viewHelper(goodDims);
      } else {
        return {(...goodDims)};
      }
    }

    proc _getViewDom() {
      return _viewHelper(dom.dsiDims());
    }

    // contiguous transfer support
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
            return _ArrInstance._getActualArray().dsiGetRAD().toSlice(_ArrInstance.dom).toRankChange(dom, collapsedDim, idx);
          } else {
            return _ArrInstance.indexCache.toRankChange(dom, collapsedDim, idx);
          }
        } else {
          return _ArrInstance.dsiGetRAD().toRankChange(dom, collapsedDim, idx);
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

    proc _containsRCRE() param {
      if chpl__isArrayView(arr) {
        return (arr.isRankChangeArrayView() ||
                arr.isReindexArrayView() ||
                arr._containsRCRE());
      } else {
        return false;
      }
    }

    proc _getRCREView() {
      return this;
    }

    proc dsiDestroyArr(isalias:bool) {
      if ownsArrInstance {
        _delete_arr(_ArrInstance, _isPrivatized(_ArrInstance));
      }
    }
  }  // end of class ArrayViewRankChangeArr


  // helper routines for converting things from higher- to
  // lower-dimensions and back again

  inline proc chpl_rankChangeConvertIdx(i: integral, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    var ind = idx;
    var j = 1;
    for param d in 1..downrank {
      if !collapsedDim(d) {
        ind(d) = i;
        j += 1;
      }
    }
    assert (j == 2);
    return ind;
  }

  inline proc chpl_rankChangeConvertIdx(i, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    var ind = idx;
    var j = 1;
    for param d in 1..downrank {
      if !collapsedDim(d) {
        ind(d) = i(j);
        j += 1;
      }
    }
    return ind;
  }

  inline proc chpl_rankChangeConvertIdxHiDToLoD(i, collapsedDim, idx, param rank) {
    param downrank = collapsedDim.size;
    var ind: rank*i(1).type;
    var j = 1;
    for param d in 1..downrank {
      if !collapsedDim(d) {
        ind(j) = i(d);
        j += 1;
      }
    }
    return ind;
  }

    inline proc chpl_rankChangeConvertDom(dims, param uprank, collapsedDim, idx) {
    param downrank = collapsedDim.size;
    if uprank != dims.size then
      compilerError("Called chpl_rankChangeConvertDom with incorrect rank. Got ", dims.size:string, ", expecting ", uprank:string);

    var ranges : downrank*dims(1).type;
    var j = 1;
    for param d in 1..downrank {
      if !collapsedDim(d) {
        ranges(d) = dims(j);
        j += 1;
      } else {
        ranges(d) = idx(d)..idx(d);
      }
    }
    return {(...ranges)};
  }


}
