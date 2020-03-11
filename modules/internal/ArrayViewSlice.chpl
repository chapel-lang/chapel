/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
  private use ChapelStandard;

  config param chpl_debugSerializeSlice = false,
               chpl_serializeSlices = false;

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
  class ArrayViewSliceArr: AbsBaseArr {
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
      super.init(eltType = eltType);
      this._DomPid      = _DomPid;
      this.dom          = dom;
      this._ArrPid      = _ArrPid;
      this._ArrInstance = _ArrInstance;

      this.indexCache = buildIndexCacheHelper(_ArrInstance, dom);
    }

    forwarding arr except these,
                      doiBulkTransferFromKnown, doiBulkTransferToKnown,
                      doiBulkTransferFromAny,  doiBulkTransferToAny,
                      chpl__serialize, chpl__deserialize;


    //
    // A helper routine to encode logic about when slices should be RVF'd.
    // Currently we RVF slices whose domains and arrays are privatized
    // (since doing so involves RVFing their PIDs) and that support the
    // chpl__serialize() routine (and, we assume, chpl__deserialize())
    //
    proc chpl__rvfMe() param {
      use Reflection;

      if chpl_serializeSlices == false then
        return false;
      if (dom.dsiSupportsPrivatization() && arr.dsiSupportsPrivatization() &&
          canResolveMethod(dom, "chpl__serialize") &&
          canResolveMethod(arr, "chpl__serialize")) {
        return true;
      } else {
        return false;
      }
    }

    //
    // Serialize a slice (when chpl__rvfMe() says to) by serializing its
    // domain and array
    //
    proc chpl__serialize() where chpl__rvfMe() {
      use SysCTypes;
      if chpl_debugSerializeSlice {
        // use printf to avoid messing up tests checking comm counts
        extern proc printf(x...);
        printf("%d serializing a slice\n", here.id:c_int);
      }
      return (_to_borrowed(dom).chpl__serialize(),
              _to_borrowed(arr).chpl__serialize());
    }

    //
    // Deserialize a slice by deserializing its domain / array components
    // and then returning a new ArrayViewSliceArr() instance referring
    // to them.
    //
    proc type chpl__deserialize(data) {
      type domType = __primitive("static field type", this, "dom");
      type arrType = __primitive("static field type", this, "_ArrInstance");
      const dom = _to_borrowed(domType).chpl__deserialize(data(1));
      const arr = _to_borrowed(arrType).chpl__deserialize(data(2));
      return new unmanaged ArrayViewSliceArr(eltType=arr.eltType,
                                             _DomPid=data(1),
                                             dom = dom,
                                             _ArrPid=data(2),
                                             _ArrInstance=arr);
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
    override proc isSliceArrayView() param {
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
      const ref myarr = arr;
      forall i in privDom do yield myarr.dsiAccess(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in privDom.these(tag) do {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis) ref
      where tag == iterKind.follower {
      const ref myarr = arr;
      for i in privDom.these(tag, followThis) {
        yield myarr.dsiAccess[i];
      }
    }


    //
    // I/O
    //

    proc dsiSerialWrite(f) throws {
      chpl_serialReadWriteRectangular(f, arr, privDom);
    }

    proc dsiSerialRead(f) throws {
      chpl_serialReadWriteRectangular(f, arr, privDom);
    }

    override proc dsiDisplayRepresentation() {
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
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(i);
      }
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(i);
      }
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType) {
      if shouldUseIndexCache() {
        const dataIdx = indexCache.getDataIndex(i);
        return indexCache.getDataElem(dataIdx);
      } else {
        return arr.dsiAccess(i);
      }
    }

    inline proc dsiBoundsCheck(i) {
      return privDom.dsiMember(i);
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

    // If we're serializing slices then we don't want to privatize
    // them proactively anymore.  If we're not serializing them, then we:
    // Don't want to privatize a DefaultRectangular, so pass the query on to
    // the wrapped array
    override proc dsiSupportsPrivatization() param
    {
      if chpl_serializeSlices then return false;
      return _ArrInstance.dsiSupportsPrivatization();
    }

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

    override proc doiCanBulkTransferRankChange() param {
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
