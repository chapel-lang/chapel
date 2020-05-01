/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  private use ChapelStandard;

  extern record chpl_opaque_array {
    var _pid: int;
    var _instance: c_void_ptr;
    var _unowned: bool;
  }

  pragma "export wrapper"
  extern record chpl_external_array {
    var elts: c_void_ptr;
    var num_elts: uint;

    var freer: c_void_ptr;
  }

  extern proc
  chpl_make_external_array(elt_size: uint, num_elts: uint): chpl_external_array;

  extern proc chpl_make_external_array_ptr(elts: c_void_ptr,
                                           num_elts: uint): chpl_external_array;

  extern proc
  chpl_make_external_array_ptr_free(elts: c_void_ptr,
                                    num_elts: uint): chpl_external_array;

  extern proc chpl_free_external_array(x: chpl_external_array);
  extern proc chpl_call_free_func(func: c_void_ptr, elts: c_void_ptr);

  // Creates an instance of our new array type
  pragma "no copy return"
  proc makeArrayFromPtr(value: c_ptr, num_elts: uint) {
    var data = chpl_make_external_array_ptr(value : c_void_ptr, num_elts);
    return makeArrayFromExternArray(data, value.eltType);
  }

  pragma "no copy return"
  proc makeArrayFromExternArray(value: chpl_external_array, type eltType) {
    var dom = defaultDist.dsiNewRectangularDom(rank=1,
                                               idxType=int,
                                               stridable=false,
                                               inds=(0..#value.num_elts,));
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

  // Creates an _array wrapper to store the information given by chpl_opaque_array
  // arrType is a subclass of BaseArr
  pragma "no copy return"
  proc makeArrayFromOpaque(value: chpl_opaque_array, type arrType) {
    var asArrType = value._instance: arrType?;
    if asArrType == nil then
      halt("nil array passed to makeArrayFromOpaque");
    var asNonNilArrType = try! asArrType: arrType;
    var ret = _newArray(asNonNilArrType);
    // Don't clean up arrays we create in this way or the user will have garbage
    // memory after the first function call
    ret._pid = value._pid;
    ret._unowned = true;
    return ret;
  }

  proc convertStringOrBytes(ref arr: []): chpl_external_array
    where arr.eltType == string || arr.eltType == bytes {

    var wrapper: [arr.domain] chpl_byte_buffer;

    // Move existing string buffers over to a new shell.
    for i in 0..#arr.size {
      ref item = arr[i];

      // This call assumes ownership of the string's buffer.
      var val = chpl__exportRetStringOrBytes(item);
      wrapper[i] = val;
    }

    var ext = chpl_make_external_array_ptr_free(c_ptrTo(wrapper[0]),
                                                wrapper.size: uint);

    return ext;
  }

  proc convertToExternalArray(in arr: []): chpl_external_array
    where (getExternalArrayType(arr) == chpl_external_array) {
    if (!isExternArrEltType(arr.eltType)) {
      use HaltWrappers;
      safeCastCheckHalt("Cannot build an external array that stores " +
                        arr.eltType: string);
    }
    if (!isIntegralType(arr.domain.idxType)) {
      // Probably not reachable any more, but may become reachable again
      // once support for interoperability with array types expands.
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

    //
    // If the array element type is string or bytes, silently convert it to
    // an array of 'chpl_byte_buffer'. Then in compiler code, we iterate
    // through the elements and convert them to string or bytes as desired.
    //
    if arr.eltType == string || arr.eltType == bytes then
      return convertStringOrBytes(arr);

    var externalArr = chpl_make_external_array_ptr_free(c_ptrTo(arr[0]),
                                                        arr.size: uint);
    // Change the source array so that it does not clean up its memory, so we
    // can safely return a chpl_external_array wrapper using it.
    arr.externArr = true;
    arr._borrowed = true;
    return externalArr;
  }

  // Shares name with the version returning chpl_external_array so the compiler
  // can make the same function call and get the appropriate type depending on
  // the argument.  Unsupported array types are turned into opaque
  // representations
  proc convertToExternalArray(arr: []): chpl_opaque_array
    where (getExternalArrayType(arr) == chpl_opaque_array) {

    var ret: chpl_opaque_array;
    ret._pid = arr._pid;
    ret._instance = arr._value: c_void_ptr;
    ret._unowned = arr._unowned;
    if (!arr._unowned) {
      arr._unowned = true;
    }
    return ret;
  }

  // Returns a bool indicating if the type would be appropriate to use in an
  // extern array.
  // NOTE: once we can export types, those should also be supported here.
  private proc isExternArrEltType(type t) param {
    if (isPrimitive(t)) {
      return true;
    } else if (t == c_string) {
      return true;
    } else if (__primitive("is extern type", t)) {
      return true;
    } else {
      return false;
    }
  }

  // Determine whether chpl_external_array or chpl_opaque array should be used.
  // Will be updated as more types are supported via chpl_external_array
  proc getExternalArrayType(arg) type {
    if (!isArrayType(arg.type)) {
      compilerError("must call with an array");
    } else {
      if (arg._value.isDefaultRectangular()) {
        return chpl_external_array;
      } else {
        return chpl_opaque_array;
      }
    }
  }

  proc getExternalArrayType(type arg) type {
    if (!isArrayType(arg))  {
      compilerError("must call with an array");
    } else {
      type _instanceType = __primitive("static field type", arg, "_instance");
      // Workaround until #12095 is resolved (problem calling static method on
      // type of unmanaged field)
      type borrowedAlt = _to_borrowed(_instanceType);
      if (borrowedAlt.isDefaultRectangular()) {
        return chpl_external_array;
      } else {
        return chpl_opaque_array;
      }
    }
  }

  // Can't create an _array wrapper to call the cleanup function for us, so do
  // the next best thing.
  export proc cleanupOpaqueArray(arr: chpl_opaque_array) {
    var cleanup = arr._instance: unmanaged BaseArr?;
    if cleanup then
      _do_destroy_arr(arr._unowned, cleanup!);
  }
}
