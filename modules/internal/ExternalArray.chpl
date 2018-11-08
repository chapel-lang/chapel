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

  class ExternDom: BaseRectangularDom {
    const size: uint; // We don't need a lower bound, it will always be zero

    const dist;

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

    override proc dsiDestroyArr() {
      if (_owned) {
        chpl_free_external_array(_ArrInstance);
      }
    }
  }

  // Creates an instance of our new array type
  pragma "no copy return"
  proc makeArrayFromPtr(value: c_ptr, size: uint) {
    var data = chpl_make_external_array_ptr(value : c_void_ptr, size);
    return makeArrayFromExternArray(data, value.eltType);
  }

  pragma "no copy return"
  proc makeArrayFromExternArray(value: chpl_external_array, type eltType) {
    var dom = defaultDist.dsiNewRectangularDom(idxType=int, inds=(0..#value.size,), true);
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
