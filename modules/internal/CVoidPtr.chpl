/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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


/*
 */
module CVoidPtr {
  use ChapelStandard;
  // use CTypes;

  // @chpldoc.nodoc
  // type _c_void_ptr = c_ptr(void);

  // pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  @chpldoc.nodoc
  class _c_void_ptr {
    //   Similar to _ddata from ChapelBase, but differs
    //   from _ddata because it can never be wide.

    /*
       The type that this pointer points to, which can be queried like so:

       .. code-block:: chapel

         var x: c_ptr = c_ptrTo(...);
         if x.eltType == c_int then do writeln("x is an int pointer");
    */
    proc eltType { return void;}
    /* Retrieve the i'th element (zero based) from a pointer to an array.
      Does the equivalent of ptr[i] in C.
    */
    inline proc this(i: integral) ref {
      return __primitive("array_get", this, i);
    }
    /* Get element pointed to directly by this pointer. If the pointer
      refers to an array, this will return ptr[0].
    */
    inline proc deref() ref {
      return __primitive("array_get", this, 0);
    }
    /* Print this pointer */
    // inline proc writeThis(ch) throws {
    //   this.writeThis(ch);
    // }
    // inline proc serialize(writer, ref serializer) throws {
    //   this.writeThis(writer);
    // }
  }
}
