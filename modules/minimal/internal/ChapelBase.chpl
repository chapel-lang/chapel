/*
 * Copyright 2004-2016 Cray Inc.
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

// ChapelBase.chpl
//

module ChapelBase {

  //
  // useful internal types
  //

  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
  }

  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  class c_ptr {
    /* The type that this pointer points to */
    type eltType;
  }

  pragma "ignore noinit"
  pragma "no default functions" // avoid the default (read|write)This routines
  record string {
    var buff: c_ptr(uint(8));

    proc string(buff: c_ptr(uint(8)), length: int, size: int,
                owned: bool = true, needToCopy: bool = true) {
      this.buff = buff;
    }
  }

  //
  // default values for types - add more as needed
  //

  inline proc _defaultOf(type t) where t: string return "";
  inline proc _defaultOf(type t) where t: c_ptr  return nil;
  inline proc _defaultOf(type t) where t == int  return 0:t;
  inline proc _defaultOf(type t) where t == real return 0.0:t;

  //
  // implicit conversions; assignments - add more as needed
  //

  inline proc _cast(type t, x) return __primitive(c"cast", t, x);
  inline proc _cast(type t, str: string) where t: c_string return str.buff;
  inline proc _cast(type t, cs: c_string) where t: string return new string(cs: c_ptr(uint(8)));

  // NB this works incorrectly for string <-> c_string
  inline proc =(ref a, b) { __primitive(c"=", a, b); }

  //
  // initCopy et al.
  //

  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(x) return x;

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x) return chpl__initCopy(x);

  pragma "compiler generated"
  inline proc chpl__autoDestroy(x) { __primitive(c"call destructor", x); }

  //
  // syntactic support for tuples
  //

  pragma "tuple" record _tuple {
    param size : int;
  }

  // tuple type
  pragma "build tuple"
  inline proc _build_tuple(type t...) type
    return t;

  // tuple value with intents by default
  pragma "build tuple"
  inline proc _build_tuple(x...) {
      return x;
  }

  // tuple value with ref intents for actuals of ref types
  pragma "allow ref" 
  pragma "build tuple"
  inline proc _build_tuple_always_allow_ref(x...)
    return x;

}
