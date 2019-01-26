/*
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

// ArrayViewSlice
//
// This module provides an array domain map class that is used to
// represent slices of another array via a domain.
//
module ArrayViewSlice {
  use ChapelStandard;
  use DefaultRectangular;
  use BlockDist;

  private proc buildIndexCacheHelper(arr, dom) {
    param isRankChangeReindex = arr.isRankChangeArrayView() ||
                                arr.isReindexArrayView() ||
                                (chpl__isArrayView(arr) && arr._containsRCRE());
    if chpl__isDROrDRView(arr) && isRankChangeReindex {
      if chpl__isArrayView(arr) then
        return arr.indexCache.toSlice(dom);
      else
        return arr.dsiGetRAD().toSlice(dom);
    } else {
      return false;
    }
  }

  //
  // The class representing a slice of an array.  Like other array
  // class implementations, it supports the standard dsi interface.
  //
  pragma "aliasing array"
  class ArrayViewSliceArr: BaseArr {
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
    //const indexCache = buildIndexCache();
    const indexCache;

    proc init(type eltType, const _DomPid, const dom, const _ArrPid, const _ArrInstance) {
      this.eltType      = eltType;
      this._DomPid      = _DomPid;
      this.dom          = dom;
      this._ArrPid      = _ArrPid;
      this._ArrInstance = _ArrInstance;

      this.indexCache = buildIndexCacheHelper(_ArrInstance, dom);
    }

    forwarding arr except these,
                      doiBulkTransferFromKnown, doiBulkTransferToKnown,
                      doiBulkTransferFromAny,  doiBulkTransferToAny;


    proc chpl__rvfMe() param {
      compilerWarning("In ArrayViewSliceArr.rvfMe, type = " + this.type:string);
      param retval = dsiSupportsPrivatization();
      compilerWarning("retval = " + retval:string);
      return retval;
    }

    record mySliceHelper {
      param domRank: int;
      param domStridable: bool;
      type domIdxType;

      param arrRank: int;
      param arrStridable: bool;
      type arrIdxType;
      type arrEltType;

      const dompid: int;
      const arrpid: int;
    }

    proc chpl__serialize() {
      var buff: chpl__inPlaceBuffer;
      writeln("[", here.id, "] In serialize, sending ", (_DomPid, _ArrPid));
      return new mySliceHelper(privDom.rank, privDom.stridable, privDom.idxType, arr.rank, arr.stridable, arr.idxType, arr.eltType, _DomPid, _ArrPid);
    }

    proc type myArrayRank(type t) param {
      var x: t;
      return x.rank;
    }

    proc type myArrayIdxType(type t) type {
      var x: t;
      return x.idxType;
    }

    proc type myArrayEltType(type t) type {
      var x: t;
      return x.eltType;
    }

    // TODO: The following assumes that we're only slicing into Block-distributed arrays using Block domains, but ultimately, we need to
    // be able to slice into them using DefaultRectangular domains as well...

    proc type chpl__deserialize(data) {
      //    compilerWarning(this:string);
      param rank = myArrayRank(this);
      //    compilerWarning(rank:string);

      type idxType = myArrayIdxType(this);
      //    compilerWarning(idxType:string);

      type eltType = myArrayEltType(this);
      //    compilerWarning(eltType:string);

      writeln("[", here.id, "] in my deserialize routine, received", (data.dompid, data.arrpid));
      const dompid = data.dompid;
      //    const dom = chpl_getPrivatizedCopy(BlockDom(rank=2, idxType=int, stridable=false, sparseLayoutType=unmanaged DefaultDist), dompid);
      const dom = chpl_getPrivatizedCopy(unmanaged BlockDom(data.domRank, data.domIdxType, data.domStridable, unmanaged DefaultDist), dompid);
      const arrpid = data.arrpid;
      const arr = chpl_getPrivatizedCopy(unmanaged BlockArr(data.arrRank, data.arrIdxType, data.arrStridable, data.arrEltType, sparseLayoutType=unmanaged DefaultDist), arrpid);

      // This is not so helpful because the "array" pragma causes us to sugar
      // this thing's type in unfortunate ways...
      //
      // writeln(this:string);


      var retval = new unmanaged ArrayViewSliceArr(eltType=eltType,
                                            _DomPid=data.dompid,
                                            dom = dom,
                                            _ArrPid=data.arrpid,
                                            _ArrInstance = arr);

      compilerWarning("returning ", retval.type:string);
      return retval;
    }

    
    //
    // standard generic aspects of arrays
    //

    // these could be fields, but indirecting works just as well and
    // makes the class less generic.
    proc idxType type return dom.idxType;
    proc rank param return arr.rank;

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
    proc isSliceArrayView() param {
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
      forall i in privDom do yield arr.dsiAccess(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
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
    // I/O
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
        return arr.dsiAccess(i);
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(i);
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      checkBounds(i);
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(i);
      }
    }

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

    proc dsiLocalSubdomain(loc: locale) {
      return privDom.dsiLocalSubdomain(loc);
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
      return new unmanaged ArrayViewSliceArr(eltType=this.eltType,
                                   _DomPid=privatizeData(1),
                                   dom=privatizeData(2),
                                   _ArrPid=privatizeData(3),
                                   _ArrInstance=privatizeData(4));
    }

    //
    // utility functions used to set up the index cache
    //

    proc shouldUseIndexCache() param {
      return (chpl__isDROrDRView(_ArrInstance) &&
              _containsRCRE());
    }

    // No modification of the index cache is necessary for a slice
    // because the index set and rank remain the same.
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
    override proc dsiGetBaseDom() {
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

    // Returns the topmost rank-change or reindex in the stack, skipping over
    // slices
    proc _getRCREView() {
      compilerAssert(this._containsRCRE());
      return arr._getRCREView();
    }

    proc doiCanBulkTransferRankChange() param {
      return arr.doiCanBulkTransferRankChange();
    }

    proc doiBulkTransferFromKnown(destDom, srcClass, srcDom) : bool {
      return chpl__bulkTransferArray(this.arr, destDom, srcClass, srcDom);
    }

    proc doiBulkTransferToKnown(srcDom, destClass, destDom) : bool {
      return chpl__bulkTransferArray(destClass, destDom, this.arr, srcDom);
    }
  }
}
