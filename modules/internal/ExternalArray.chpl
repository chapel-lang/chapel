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
    //proc rank param return arr.rank; // Should I hardcode this for now?

    // do I want a "isExternArrayView" method on BaseArr?

    // Probably want some these() iterators? (see ArrayViewSlice.chpl:116-138)
    // Probably want some dsiSerialRead/Write? (see ArrayViewSlice.chpl:145-151)
    // Probably want dsiDisplayRepresentation? (see ArrayViewSlice.chpl:153-161)
    // Probably want dsiAccess? (see ArrayViewSlice.chpl:168-212)
    // Probably want checkBounds? (see ArrayViewSlice.chpl:214-218)

    // privDom, arr inline procs?
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

    // proc dsiReallocate(d: domain) is not supported, so don't override.
    // proc _resize(length: int, old_map) is not supported, don't override.
  }
}