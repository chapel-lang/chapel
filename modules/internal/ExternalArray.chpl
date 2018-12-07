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

  // Creates an instance of our new array type
  pragma "no copy return"
  proc makeArrayFromPtr(value: c_ptr, size: uint) {
    var data = chpl_make_external_array_ptr(value : c_void_ptr, size);
    return makeArrayFromExternArray(data, value.eltType);
  }

  pragma "no copy return"
  proc makeArrayFromExternArray(value: chpl_external_array, type eltType) {
    var dom = defaultDist.dsiNewRectangularDom(rank=1,
                                               idxType=int,
                                               stridable=false,
                                               inds=(0..#value.size,),
                                               externalArray=true);
    dom._free_when_no_arrs = true;
    var arr = new unmanaged DefaultRectangularArr(eltType=eltType,
                                                  rank=1,
                                                  idxType=dom.idxType,
                                                  stridable=dom.stridable,
                                                  dom=dom,
                                                  data=value.elts: _ddata(eltType),
                                                  externData=value,
                                                  externArr=true,
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
