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

  pragma "use default init"
  class ArrayViewExternDom: BaseDom {
    const size: uint; // We don't need a lower bound, it will always be zero

    const distInst;

    proc dsiBuildArray(type eltType) {
      pragma "no auto destroy"
        // Create c_ptr to an array of the same size
      var arr = new unmanaged ArrayViewExternArr(eltType,
                                                 _to_unmanaged(this),
                                                 // UMMMMM
                                                 true);
    }

    proc dsiGetIndices() return 0..#size;

    proc dsiSetIndices(x) {
      halt("Can't change the indices of an external array");
    }

    iter these() {
      for i in dsiGetIndices() do
        yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      forall i in dsiGetIndices() do
        yield i;
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in dsiGetIndices() {
        yield followThis;
      }
    }

    iter these(param tag: iterKind, followThis)
      where tag == iterKind.follower {
      for i in dsiGetIndices() do
        yield i;
    }

    // Do I want dsiAssignDomain defined? (see ArrayViewReindex.chpl:262-264)
    // What about _getActualDom?  dsiDestroyDom?

    // Prolly want the privatization stuff eventually, but I don't need it right
    // now.
  }

  class ArrayViewExternArr: BaseArr {
    type eltType;

    const dom;
    
    const _ArrInstance;

    const _owned: bool;

    proc init(type eltType, const dom, const _ArrInstance, _owned: bool) {
      super.init(_decEltRefCounts = false);
      this.eltType = eltType;
      this.dom = dom;
      this._ArrInstance = _ArrInstance;
      this._owned = _owned;
    }

    proc idxType type return dom.idxType;
    proc rank param return 1; // Should I hardcode this for now?

    // do I want a "isExternArrayView" method on BaseArr?

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

    // Probably want some dsiSerialRead/Write? (see ArrayViewSlice.chpl:145-151)
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
      if boundsChecking then
        if !dom.dsiMember(i) {
          halt("array index out of bounds: " + _stringify_tuple(i));
        }
      return _ArrInstance(i(1));
    }

    inline proc dsiAccess(i)
      where shouldReturnRvalueByValue(eltType) {
      if boundsChecking then
        if !dom.dsiMember(i) {
          halt("array index out of bounds: " + _stringify_tuple(i));
        }
      return _ArrInstance(i(1));
    }

    inline proc dsiAccess(i) const ref
      where shouldReturnRvalueByConstRef(eltType)  {
      if boundsChecking then
        if !dom.dsiMember(i) {
          halt("array index out of bounds: " + _stringify_tuple(i));
        }
      return _ArrInstance(i(1));
    }
    // Maybe want checkBounds (see ArrayViewSlice.chpl:214-218)

    // arr inline proc?
    // dsiGetBaseDom (see ArrayViewSlice.chpl:299-301)

    // _getActualArray - useful for returning the array?  Or a dangerous insight
    // into the implementation?

    // PUNT FOR NOW, WILL NEED EVENTUALLY:
    // Do I want the locality-oriented queries? (see
    // ArrayViewSlice.chpl:225-230)
    // What about the privatization?
    // doiCanBulkTransferRankChange? doiBulkTransferFromKnown?
    // doiBulkTransferToKnown?

    proc dsiDestroyArr() {
      // Do something special on clean up if array is owned or not
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
}