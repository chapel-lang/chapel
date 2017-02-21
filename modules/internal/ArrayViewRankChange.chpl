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
module ArrayViewRankChange {

class ArrayViewRankChangeArr: BaseArr {
  type eltType;  // see note on commented-out proc eltType below...

  // TODO: Can we privatize upon creation of the array-view slice and cache
  // the results?
  const _DomPid;
  const dom; // Seems like the compiler requires a field called 'dom'...

  const _ArrPid;
  const _ArrInstance;

  const collapsedDim;  // rank*bool    TODO: constrain this
  const idx;           // rank*idxType TODO: and this
  
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

  proc idxType type return dom.idxType;
  proc rank param return dom.rank;

  // This seems like it ought to work, but it causes an error in the
  // compiler for non-devel mode...  presumably due to a direct
  // query of eltType...
  //  proc eltType type return arr.eltType;

  //
  // TODO: Could this be replaced with more type-based introspection?
  // I shied away from it since this is a generic class, but there
  // must be (or should be) some way to do it without relying on
  // methods like this...
  //
  proc isRankChangeArrayView() param {
    return true;
  }

  inline proc dsiGetBaseDom() {
    return dom;
  }

  //
  // standard iterators
  //
  iter these() ref {
    for i in privDom do
      yield arr.dsiAccess(chpl_rankChangeConvertIdx(i));
  }

  iter these(param tag: iterKind) ref where tag == iterKind.standalone {
    for i in privDom.these(tag) do
      yield arr.dsiAccess(chpl_rankChangeConvertIdx(i));
  }

  iter these(param tag: iterKind) where tag == iterKind.leader {
    //    writeln("In sliceview leader");
    for followThis in privDom.these(tag) do {
      yield followThis;
    }
  }

  iter these(param tag: iterKind, followThis) ref
    where tag == iterKind.follower {
    for i in privDom.these(tag, followThis) {
      yield arr.dsiAccess(chpl_rankChangeConvertIdx(i));
    }
  }

  //
  // standard I/O stuff
  //
  proc dsiSerialWrite(f) {
    chpl_serialReadWriteRectangular(f, this, privDom);
  }

  inline proc checkBounds(i) {
    if boundsChecking then
      if !privDom.dsiMember(i) then
        halt("array index out of bounds: ", i);
  }

  //
  // standard accessors
  //
  inline proc dsiAccess(i: idxType ...rank) ref {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i: idxType ...rank)
  where !shouldReturnRvalueByConstRef(eltType) {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i: idxType ...rank) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    return dsiAccess(i);
  }

  inline proc dsiAccess(i) ref {
    checkBounds(i);
    return arr.dsiAccess(chpl_rankChangeConvertIdx(i));
  }

  inline proc dsiAccess(i)
  where !shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    return arr.dsiAccess(chpl_rankChangeConvertIdx(i));
  }

  inline proc dsiAccess(i) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    return arr.dsiAccess(chpl_rankChangeConvertIdx(i));
  }

  //
  // This helper routine converts an incoming low-D index into a
  // high-D index
  //
  inline proc chpl_rankChangeConvertIdx(i: integral) {
    var ind = idx;
    var j = 1;
    for param d in 1..arr.rank {
      if !collapsedDim(d) {
        ind(d) = i;
        j += 1;
      }
    }
    assert (j == 2);
    return ind;
  }

  inline proc chpl_rankChangeConvertIdx(i) {
    var ind = idx;
    var j = 1;
    for param d in 1..arr.rank {
      if !collapsedDim(d) {
        ind(d) = i(j);
        j += 1;
      }
    }
    return ind;
  }

  inline proc chpl_rankChangeConvertDom(dims) {
    if dom.rank != dims.size {
      compilerError("Called chpl_rankChangeConvertDom with incorrect rank. Got ", dims.size:string, ", expecting", dom.rank:string);
    }
    //
    // TODO: I worry that I'm being too fast and loose with domain
    // records and classes here
    //
    var ranges : arr.rank*dims(1).type;
    //    writeln("*** dom was: ", dom);
    var j = 1;
    for param d in 1..arr.rank {
      if !collapsedDim(d) {
        ranges(d) = dims(j);
        j += 1;
      } else {
        ranges(d) = idx(d)..idx(d);
      }
    }
    return {(...ranges)};
  }

  // TODO: Haven't looked below here yet...
  
/*  I don't think these should be needed...
  proc dataChunk(x) ref {
    return arr.dataChunk(x);
  }
*/

  proc dsiSlice(newdom) {
    compilerError("slicing not supported on array views yet");
  }

  proc dsiReindex(newdom) {
    compilerError("reindexing not supported on array views yet");
  }

  proc dsiNoFluffView() {
    if canResolveMethod(arr, "dsiNoFluffView") {
      return arr.dsiNoFluffView();
    } else {
      compilerError("noFluffView is not supported on this array type.");
    }
  }

  //
  // Local subdomain interface
  //
  proc dsiHasSingleLocalSubdomain() param
    return privDom.dsiHasSingleLocalSubdomain();

  //
  // TODO: Is this correct in distributed memory?
  //
  proc dsiLocalSubdomain() {
    return privDom.dsiLocalSubdomain();
  }

  // Don't want to privatize a DefaultRectangular, so pass the query on to
  // the wrapped array
  proc dsiSupportsPrivatization() param return _ArrInstance.dsiSupportsPrivatization();

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

  proc dsiSupportsBulkTransfer() param {
    return arr.dsiSupportsBulkTransfer();
  }

  proc dsiSupportsBulkTransferInterface() param return arr.dsiSupportsBulkTransferInterface();

  // Recursively builds up the view-domain given an initial tuple of
  // dimensions. Handles nested rank-changes.
  proc _viewHelper(dims) {
    // If 'dims.size != arr.rank', assume that we still need to do the
    // conversion for the current rank-change.
    var goodDims = if dims.size != arr.rank then chpl_rankChangeConvertDom(dims).dims() else dims;
    if goodDims.size != arr.rank {
      compilerError("Error while composing view domain for rank-change view.");
    }
    if _containsRankChange() {
      var nextChange = arr._getRankChangeView();
      return nextChange._viewHelper(goodDims);
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

  proc isDefaultRectangular() param return arr.isDefaultRectangular();

  proc _getActualArray() {
    if chpl__isArrayView(arr) {
      return arr._getActualArray();
    } else {
      return arr;
    }
  }

  proc _containsRankChange() param {
    if chpl__isArrayView(arr) {
      return arr.isRankChangeArrayView() || arr._containsRankChange();
    } else {
      return false;
    }
  }

  proc _getRankChangeView() {
    return this;
  }
}

}
