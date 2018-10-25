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

// ExternalArray
//
// This module provides an array domain map class that is used to represent
// arrays obtained from external code to Chapel (and thus that we do not own).
//
module ExternalArray {
  use ChapelStandard;

  extern record chpl_external_array {
    var elts: c_void_ptr;
    var size: uint;
  }

  extern proc
  chpl_make_external_array(elt_size: uint, num_elts: uint): chpl_external_array;

  extern proc chpl_make_external_array_ptr(elts: c_void_ptr,
                                           size: uint): chpl_external_array;

  extern proc chpl_free_external_array(x: chpl_external_array);

  class ExternDist: BaseDist {

    override proc dsiNewRectangularDom(param rank: int = 1, type idxType = int,
                                       param stridable: bool = false, inds) {
      if (rank != 1) {
        halt("external arrays are only allowed a rank of 1 right now");
      }
      if (stridable) {
        halt("external arrays are not allowed to be stridable right now");
      }
      if (!isIntegralType(idxType)) {
        halt("external arrays only allow integral indices");
      }
      if (inds.size != 1) {
        halt("there should only be one set of indices, not multiple dimensions");
      }
      var r = inds(1);
      // Allow empty arrays (which have the domain 1..0)
      if (r.low != 0 && !(r.low == 1 && r.high == 0)) {
        halt("non-empty external arrays always have a lower bound of 0");
      }
      var newdom = new unmanaged ExternDom(idxType,
                                           r.size,
                                           _to_unmanaged(this));
      return newdom;
    }

    proc dsiClone() {
      return _to_unmanaged(this);
    }

    proc trackDomains() param return false;
    override proc dsiTrackDomains() return false;

    proc singleton() param return true;

    proc dsiIsLayout() param return true;
  }

  var defaultExternDist = new unmanaged ExternDist();

  // Module deinit
  proc deinit() {
    delete defaultExternDist;
  }

  class ExternDom: BaseRectangularDom {
    const size: uint; // We don't need a lower bound, it will always be zero

    const dist;

    proc init(type idxType, size, dist) {
      super.init(1, idxType, false);
      this.size = size: uint;
      this.dist = dist;
    }

    proc dsiBuildArray(type eltType) {
      var data = chpl_make_external_array(c_sizeof(eltType), this.size);
      var arr = new unmanaged ExternArr(eltType,
                                        _to_unmanaged(this),
                                        data,
                                        true);
      // Only give the pointer initial contents if we created it ourselves.
      init_elts(data.elts:c_ptr(eltType), this.size, eltType);
      return arr;
    }

    proc domRange return 0..#size;

    proc dsiNumIndices return size;

    proc dsiGetIndices() return (domRange,);

    proc dsiSetIndices(x) {
      halt("Can't change the indices of an external array");
    }

    iter these() {
      for i in domRange do
        yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      forall i in domRange do
        yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in domRange {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      for i in domRange do
        yield i;
    }

    override proc dsiMyDist() {
      return dist;
    }

    proc linksDistribution() param return false;
    override proc dsiLinksDistribution()     return false;

    proc dsiMember(ind: rank*idxType) {
      if (ind(1) < size && ind(1) >= 0) {
        return true;
      } else {
        return false;
      }
    }

    proc dsiDim(d: int) {
      if (d != rank) {
        halt("domains over external arrays have only one dimension");
      }
      return domRange;
    }

    // Necessary?
    proc dsiDim(param d: int) {
      if (d != rank) {
        halt("domains over external arrays have only one dimension");
      }
      return dsiGetIndices();
    }

    proc dsiDims()
      return dsiGetIndices();

    proc dsiLow {
      return 0;
    }

    proc dsiAssignDomain(rhs: domain, lhsPrivate: bool) {
      chpl_assignDomainWithGetSetIndices(this, rhs);
    }

    // What about _getActualDom?  dsiDestroyDom?

    // Prolly want the privatization stuff eventually, but I don't need it right
    // now.
  }

  class ExternArr: BaseArr {
    type eltType;

    const dom;
    
    const _ArrInstance: chpl_external_array;
    const elts = _ArrInstance.elts: _ddata(eltType);

    const _owned: bool;

    proc init(type eltType, const dom, const _ArrInstance, _owned: bool) {
      super.init(_decEltRefCounts = false);
      this.eltType = eltType;
      this.dom = dom;
      this._ArrInstance = _ArrInstance;
      this._owned = _owned;
    }

    proc idxType type return dom.idxType;
    proc rank param return dom.rank;

    // do I want a "isExtern" method on BaseArr?

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
      forall i in privDom do yield dsiAccess(i);
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in privDom.these(tag) do {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis) ref
      where tag == iterKind.follower {
      for i in privDom.these(tag, followThis) {
        yield dsiAccess[i];
      }
    }

    proc dsiSerialWrite(f) {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    proc dsiSerialRead(f) {
      chpl_serialReadWriteRectangular(f, this, privDom);
    }

    // Probably want dsiDisplayRepresentation? (see ArrayViewSlice.chpl:153-161)

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
      return elts(i(1));
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      checkBounds(i);
      return elts(i(1));
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)  {
      checkBounds(i);
      return elts(i(1));
    }

    inline proc checkBounds(i) {
      if boundsChecking then
        if !dom.dsiMember(i) {
          halt("array index out of bounds: " + _stringify_tuple(i));
        }
    }

    override proc dsiGetBaseDom() {
      return dom;
    }

    // _getActualArray - useful for returning the array?  Or a dangerous insight
    // into the implementation?

    // PUNT FOR NOW, WILL NEED EVENTUALLY:
    // Do I want the locality-oriented queries? (see
    // ArrayViewSlice.chpl:225-230)
    // What about the privatization?
    // doiCanBulkTransferRankChange? doiBulkTransferFromKnown?
    // doiBulkTransferToKnown?

    override proc dsiDestroyArr() {
      if (_owned) {
        chpl_free_external_array(_ArrInstance);
      }
    }

    inline proc privDom {
      /*if _isPrivatized(dom) {
        return chpl_getPrivatizedCopy(dom.type, _DomPid);
        } else {*/
        return dom;
        //}
    }

    // proc dsiReallocate(d: domain) is not supported, so don't override.
    // proc _resize(length: int, old_map) is not supported, don't override.
  }

  // Creates an instance of our new array type
  pragma "no copy return"
  proc makeArrayFromPtr(value: c_ptr, size: uint) {
    var data = chpl_make_external_array_ptr(value : c_void_ptr, size);
    return makeArrayFromExternArray(data, value.eltType);
  }

  pragma "no copy return"
  proc makeArrayFromExternArray(value: chpl_external_array, type eltType) {
    var dom = defaultExternDist.dsiNewRectangularDom(idxType=int, inds=(0..#value.size,));
    dom._free_when_no_arrs = true;
    var arr = new unmanaged ExternArr(eltType,
                                      dom,
                                      value,
                                      _owned=false);
    dom.add_arr(arr, locking = false);
    return _newArray(arr);
  }

  proc convertToExternalArray(arr: []): chpl_external_array {
    if (!isIntegralType(arr.domain.idxType)) {
      compilerError("cannot return an array with indices that are not " +
                    "integrals");
    }
    if (arr.domain.stridable) {
      compilerError("cannot return a strided array");
    }
    if (arr.domain.rank != 1) {
      compilerError("cannot return an array with rank != 1");
    }
    if (arr.domain.low != 0) {
      halt("cannot return an array when the lower bounds is not 0");
    }
    var externalArr = chpl_make_external_array(c_sizeof(arr.eltType),
                                               arr.size: uint);
    chpl__uncheckedArrayTransfer(makeArrayFromExternArray(externalArr,
                                                          arr.eltType),
                                 arr);
    return externalArr;
  }
}
