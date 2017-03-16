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

  class ArrayViewRankChangeDist: BaseDist {
    const upinds;
    const downdomPid;
    const downdomInst;
    const collapsedDim;
    const idx;

    proc deinit() {
      // This distribution "owns" downdom, so should destroy it when
      // it's being destroyed.
      _delete_dom(downdomInst, _isPrivatized(downdomInst));
    }
    
    proc dsiNewRectangularDom(param rank, type idxType, param stridable) {
      //      compilerWarning("rank arg = " + rank);
      //      compilerWarning("downrank = " + downrank);
      return new ArrayViewRankChangeDom(upinds=upinds,
                                        downdomPid=downdomPid,
                                        downdomInst=downdomInst,
                                        collapsedDim=collapsedDim,
                                        idx=idx,
                                        dist=this);
    }

    //
    // TODO: Is this OK?  Probably not
    //
    proc dsiClone() return new ArrayViewRankChangeDist(upinds=upinds,
                                                       downdomPid=downdomPid,
                                                       downdomInst=downdomInst,
                                                       collapsedDim=collapsedDim,
                                                       idx=idx);
  }
  
  //
  // This class represents the domain of a rank-change slice of an
  // array.  Like other domain class implementations, it supports the
  // standard dsi interface.  Note that rank changes only make sense
  // for rectangular domains so this is a subclass of
  // BaseRectangularDom.
  //
  class ArrayViewRankChangeDom: BaseRectangularDom {
    const upinds;

    // the domain for the sliced array
    const downdomPid;
    const downdomInst;

    const collapsedDim;
    const idx;

    const dist;

    // TODO: This is overly simplistic and should be improved; maybe
    // this is what calling _getRankChangeRanges() on upinds would
    // help with?
    proc idxType type {
      if isRange(upinds(1)) then
        return upinds(1).idxType;
      else
        return upinds(1);
    }

    proc rank param {
      return upinds.size;
    }

    proc downrank param {
      return collapsedDim.size;
    }

    inline proc downdom {
      if _isPrivatized(downdomInst) then
        return chpl_getPrivatizedCopy(downdomInst.type, downdomPid);
      else
        return downdomInst;
    }

    proc stridable param {
      for param d in 1..upinds.size do
        if isRange(upinds(d)) && upinds(d).stridable then
          return true;
      return false;
    }

    proc dsiBuildArray(type eltType) {
      pragma "no auto destroy"
      const arr = _newArray(downdom.dsiBuildArray(eltType));
      // TODO: Update once we start privatizing
      return new ArrayViewRankChangeArr(eltType  =eltType,
                                        _DomPid = nullPid,
                                        dom = this,
                                        downdomPid = downdomPid,
                                        downdomInst = downdomInst,
                                        _ArrPid=arr._pid,
                                        _ArrInstance=arr._instance,
                                        collapsedDim=collapsedDim,
                                        idx=idx);
    }

    proc dsiNumIndices {
      return downdom.dsiNumIndices;
    }

    proc dsiDim(upDim: int) {
      //      writeln("Call to dsiDim(", upDim, ")");
      //      writeln("collapsedDim = ", collapsedDim);
      var downDim = 0;
      for d in 1..downrank {
        if !collapsedDim(d) {
          downDim += 1;
          if (upDim == downDim) then {
            //            writeln("returning dim ", d, ": ", downdom.dsiDim(d));
            return downdom.dsiDim(d);
          }
        }
      }
      halt("Fell out of loop in dsiDim():", (upDim, downDim));
    }

    proc dsiDims() {
      var upDims: rank*range(idxType, stridable=stridable);
      var upDim = 1;
      for param downDim in 1..downrank {
        if !collapsedDim(downDim) {
          upDims(upDim) = downdom.dsiDim(downDim);
          upDim += 1;
        }
      }
      return upDims;
    }
    
    proc dsiGetIndices() {
      return upinds;
    }

    proc dsiSetIndices(inds) {
      if (inds != upinds) then
        halt("Can only support matching indices so far");
    }

    proc dsiMember(i) {
      return downdom.dsiMember(chpl_rankChangeConvertIdx(i, collapsedDim, idx));
    }

    iter these() {
      for i in downdom do
        yield downIdxToUpIdx(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      //      writeln("About to do a forall loop over ");
      //      downdom.dsiSerialWrite(stdout);
      forall i in downdom do
        yield downIdxToUpIdx(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in downdom.these(tag) do {
        //        writeln("Leader wants to lead ", followThis);
        const followThisLoD = chpl_rankChangeConvertHiDTupleToLoD(followThis);
        //        writeln("Changed it to ", followThisLoD);
        yield followThisLoD;
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      const followThisHiD = chpl_rankChangeConvertLoDTupleToHiD(followThis);
      for i in downdom.these(tag, followThisHiD) {
        yield chpl_rankChangeConvertIdxHiDToLoD(i, collapsedDim, idx, rank);
      }
    }

    proc chpl_rankChangeConvertLoDTupleToHiD(tup) {
      //      compilerWarning("tuple type: ", tup.type:string);
      var tupHiD: downrank*tup(1).type;
      //      compilerWarning("tupHiD type: ", tupHiD.type:string);
      var i = 1;
      for param d in 1..downrank do
        if collapsedDim(d) then
          tupHiD(d) = 0..0;  // TODO: This seems bogus/arbitrary... or is it?
        else {
          tupHiD(d) = tup(i);
          i += 1;
        }
      return tupHiD;
    }

    proc chpl_rankChangeConvertHiDTupleToLoD(tup) {
      //      compilerWarning("tup type: ", tup.type:string);
      var tupLoD: rank*tup(1).type;
      var i = 1;
      for param d in 1..downrank do
        if !collapsedDim(d) {
          tupLoD(i) = tup(d);
          i += 1;
        }
      return tupLoD;
    }


    proc downIdxToUpIdx(downIdx) {
      var upIdx: rank*idxType;
      var upDim = 1;
      for param downDim in 1..downrank {
        if !collapsedDim(downDim) {
          upIdx(upDim) = downIdx(downDim);
          upDim += 1;
        }
      }
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
          write(downdom.dsiDim(d));
        }
      write("}");
    }

    proc dsiMyDist() {
      return dist;
    }
        
  } // ArrayViewRankChangeDom
  
  //
  // The class representing a rank-change slice of an array.  Like
  // other array class implementations, it supports the standard dsi
  // interface.
  //
  class ArrayViewRankChangeArr: BaseArr {
    type eltType;  // see note on commented-out proc eltType below...

    // the representation of the slicing domain.  For a rank change
    // like A[lo..hi, 3] this is the lower-dimensional domain {lo..hi}.
    //
    // TODO: Can we privatize upon creation of the array-view slice and cache
    // the results?
    const _DomPid;
    const dom; // Seems like the compiler requires a field called 'dom'...

    const downdomPid;
    const downdomInst;

    // the representation of the sliced array
    const _ArrPid;
    const _ArrInstance;

    // These two fields represent whether or not each dimension was
    // collapsed as part of the rank-change; and if so, what the
    // index of that collapsed dimension was.  So for A[lo..hi, 3]
    // these would be (false, true) and (?, 3) respectively.

    // TODO: Use copies in domain rather than replicating these here
    
    const collapsedDim;  // rank*bool    TODO: constrain this
    const idx;           // rank*idxType TODO: and this

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
              else arr.dsiAccess(chpl_rankChangeConvertIdx(i));
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      //o      compilerWarning("in array leader, rank is " + rank);
      for followThis in privDom.these(tag) do {
        //        writeln("leader yielding: ", followThis);
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
      //      writeln("Writing array over ");
      //      privDom.dsiSerialWrite(f);
      //      writeln();

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
      //      writeln("In dsiAccess A");
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank)
      where shouldReturnRvalueByValue(eltType) {
      //      writeln("In dsiAccess B");
      return dsiAccess(i);
    }

    inline proc dsiAccess(i: idxType ...rank) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      return dsiAccess(i);
    }

    inline proc dsiAccess(i) ref {
      //      writeln("In dsiAccess C");
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
      //      writeln("In dsiAccess D");
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
      //      writeln("In dsiAccess E");
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
      // BLC: To tighten this up, we'd need to query the distribution to
      // see what subset of target locales the rank-change slice hit vs.
      // not.
      //
      compilerWarning("Calls to .targetLocales() on rank-change slices may currently return a superset of the locales targeted.");
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
      return (_DomPid, dom, downdomPid, downdomInst, _ArrPid, _ArrInstance, collapsedDim, idx);
    }

    proc dsiPrivatize(privatizeData) {
      return new ArrayViewRankChangeArr(eltType=this.eltType,
                                        _DomPid=privatizeData(1),
                                        dom=privatizeData(2),
                                        downdomPid=privatizeData(3),
                                        downdomInst=privatizeData(4),
                                        _ArrPid=privatizeData(5),
                                        _ArrInstance=privatizeData(6),
                                        collapsedDim=privatizeData(7),
                                        idx=privatizeData(8));
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
      return _ArrInstance.isDefaultRectangular();
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

    inline proc downdom {
      if _isPrivatized(downdomInst) then
        return chpl_getPrivatizedCopy(downdomInst.type, downdomPid);
      else
        return downdomInst;
    }


    inline proc arr {
      if _isPrivatized(_ArrInstance) {
        //        writeln("in arr, _ArrPid is", _ArrPid);
        return chpl_getPrivatizedCopy(_ArrInstance.type, _ArrPid);
      } else {
        return _ArrInstance;
      }
    }

    // not sure what this is, but everyone seems to have one...
    inline proc dsiGetBaseDom() {
      return dom;
    }

    proc isDefaultRectangular() param return arr.isDefaultRectangular();

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
  }

  // TODO: Move these into domain
  
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
