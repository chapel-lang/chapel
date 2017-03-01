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

// ArrayViewSlice
//
module ArrayViewSlice {

class ArrayViewSliceArr: BaseArr {
  type eltType;  // see note on commented-out proc eltType below...

  // TODO: Can we privatize upon creation of the array-view slice and cache
  // the results?
  const _DomPid;
  const dom; // Seems like the compiler requires a field called 'dom'...

  const _ArrPid;
  const _ArrInstance;

  const indexCache = buildIndexCache();

  proc shouldUseIndexCache() param {
    return _ArrInstance.isDefaultRectangular() &&
           _containsRCRE() &&
           defRectSimpleDData;
  }

  // No modification of the index cache is necessary or a slice because the
  // index set and rank remain the same.
  proc buildIndexCache() {
    if shouldUseIndexCache() {
      if (chpl__isArrayView(_ArrInstance)) {
        return _ArrInstance.indexCache.toSlice(dom);
      } else {
        return _ArrInstance.dsiGetRAD().toSlice(dom);
      }
    } else {
      return false;
    }
  }

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
  proc rank param return arr.rank;

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
  proc isSliceArrayView() param {
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
      yield arr.dsiAccess(i);
  }

  iter these(param tag: iterKind) ref where tag == iterKind.standalone && !localeModelHasSublocales {
    for i in privDom.these(tag) do yield arr.dsiAccess(i);
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
      yield arr.dsiAccess[i];
    }
  }

  //
  // standard I/O stuff
  //
  proc dsiSerialWrite(f) {
    chpl_serialReadWriteRectangular(f, arr, privDom);
  }

  proc dsiSerialRead(f) {
    chpl_serialReadWriteRectangular(f, arr, privDom);
  }

  proc dsiDisplayRepresentation() {
    writeln("Slice view");
    writeln("----------");
    writeln("of domain:");
    privDom.dsiDisplayRepresentation();
    writeln("on array:");
    arr.dsiDisplayRepresentation();
    writeln("----------");
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
    if shouldUseIndexCache() {
      const dataIdx = indexCache.getRADDataIndex(dom.stridable, i);
      return indexCache.shiftedDataElem(dataIdx);
    } else {
      return arr.dsiAccess(i);
    }
  }

  inline proc dsiAccess(i)
  where !shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    if shouldUseIndexCache() {
      const dataIdx = indexCache.getRADDataIndex(dom.stridable, i);
      return indexCache.shiftedDataElem(dataIdx);
    } else {
      return arr.dsiAccess(i);
    }
  }

  inline proc dsiAccess(i) const ref
  where shouldReturnRvalueByConstRef(eltType) {
    checkBounds(i);
    if shouldUseIndexCache() {
      const dataIdx = indexCache.getRADDataIndex(dom.stridable, i);
      return indexCache.shiftedDataElem(dataIdx);
    } else {
      return arr.dsiAccess(i);
    }
  }

  inline proc dsiLocalAccess(i) ref
    return arr.dsiLocalAccess(i);

  inline proc dsiLocalAccess(i)
    where !shouldReturnRvalueByConstRef(eltType)
    return arr.dsiLocalAccess(i);

  inline proc dsiLocalAccess(i) const ref
    where shouldReturnRvalueByConstRef(eltType)
    return arr.dsiLocalAccess(i);


  proc dsiTargetLocales() {
    return arr.dsiTargetLocales();
  }
  /*  I don't think these should be needed...
  proc dataChunk(x) ref {
    return arr.dataChunk(x);
  }
*/

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
    return (_DomPid, dom, _ArrPid, _ArrInstance);
  }

  proc dsiPrivatize(privatizeData) {
    return new ArrayViewSliceArr(eltType=this.eltType, _DomPid=privatizeData(1),
                                 dom=privatizeData(2),
                                 _ArrPid=privatizeData(3),
                                 _ArrInstance=privatizeData(4));
  }

  proc dsiSupportsBulkTransfer() param {
    return arr.dsiSupportsBulkTransfer();
  }
  proc dsiSupportsBulkTransferInterface() param return arr.dsiSupportsBulkTransferInterface();

  proc _viewHelper(dims) {
    compilerError("viewHelper not supported on ArrayViewSlice.");
  }

  proc _getViewDom() {
    if _containsRCRE() {
      // Rank-changes and reindexes know how to deal with nested rank-changes
      // and reindexes, so hand off our indices and let them handle the rest.
      var nextView = _getRCREView();
      return nextView._viewHelper(dom.dsiDims());
    } else {
      return {(...dom.dsiDims())};
    }
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

  proc _containsRCRE() param {
    if chpl__isArrayView(arr) {
      return arr.isRankChangeArrayView() ||
             arr.isReindexArrayView() ||
             arr._containsRCRE();
    } else {
      return false;
    }
  }

  // Returns the topmost rank-change or reindex in the stack, skipping over
  // slices
  proc _getRCREView() {
    compilerAssert(this._containsRCRE());
    return arr._getRCREView();
  }
}

}
