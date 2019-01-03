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

    var freer: c_void_ptr;
  }

  extern proc
  chpl_make_external_array(elt_size: uint, num_elts: uint): chpl_external_array;

  extern proc chpl_make_external_array_ptr(elts: c_void_ptr,
                                           size: uint): chpl_external_array;

  extern proc
  chpl_make_external_array_ptr_free(elts: c_void_ptr,
                                    size: uint): chpl_external_array;

  extern proc chpl_free_external_array(x: chpl_external_array);
  extern proc chpl_call_free_func(func: c_void_ptr, elts: c_void_ptr);

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
                                               inds=(0..#value.size,));
    dom._free_when_no_arrs = true;
    var arr = new unmanaged DefaultRectangularArr(eltType=eltType,
                                                  rank=1,
                                                  idxType=dom.idxType,
                                                  stridable=dom.stridable,
                                                  dom=dom,
                                                  data=value.elts: _ddata(eltType),
                                                  externFreeFunc=value.freer,
                                                  externArr=true,
                                                  _borrowed=true);
    dom.add_arr(arr, locking = false);
    return _newArray(arr);
  }

  proc convertToExternalArray(in arr: []): chpl_external_array {
    if (!isExternArrEltType(arr.eltType)) {
      use HaltWrappers;
      safeCastCheckHalt("Cannot build an external array that stores " +
                        arr.eltType: string);
    }
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
    var externalArr = chpl_make_external_array_ptr_free(c_ptrTo(arr[0]),
                                                        arr.size: uint);
    // Change the source array so that it does not clean up its memory, so we
    // can safely return a chpl_external_array wrapper using it.
    arr.externArr = true;
    arr._borrowed = true;
    return externalArr;
  }

  // Returns a bool indicating if the type would be appropriate to use in an
  // extern array.
  // NOTE: once we can export types, those should also be supported here.
  private proc isExternArrEltType(type t) param {
    if (isPrimitive(t) && t != string) {
      return true;
    } else if (t == c_string) {
      return true;
    } else if (__primitive("is extern type", t)) {
      return true;
    } else {
      return false;
    }
  }
}
